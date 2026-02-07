#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <limits>

#include "books.h"
#include "loans.h"

using namespace std;

const string LOANS_FILE = "loans.txt";

static void ignore_line()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool open_files(const string& filename, ifstream& in, ofstream& tmp)
{
    in.open(filename);
    if (!in)
        return false;

    tmp.open(filename + ".tmp");
    if (!tmp)
    {
        in.close();
        return false;
    }
    return true;
}

bool parse_id(const string& line, int& id)
{
    size_t sep = line.find('|');
    if (sep == string::npos)
        return false;

    id = stoi(line.substr(0, sep));
    return true;
}

int get_next_id(const string& filename)
{
    ifstream file(filename);
    if (!file)
        return 1;

    string line;
    int max_id = 0;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        int id = stoi(line.substr(0, line.find('|')));
        if (id > max_id)
            max_id = id;
    }
    return max_id + 1;
}

void add_book(const string& filename)
{
    Book book;

    ignore_line();
    cout << "Enter title: ";
    getline(cin, book.title);

    cout << "Enter author: ";
    getline(cin, book.author);

    cout << "Enter year: ";
    cin >> book.year;

    book.id = get_next_id(filename);
    book.available = true;

    ofstream file(filename, ios::app);
    if (!file)
    {
        cout << "Cannot open books file." << endl;
        return;
    }

    file << book.id << "|"
         << book.title << "|"
         << book.author << "|"
         << book.year << "|"
         << book.available << '\n';

    cout << "Book added." << endl;
}

bool is_library_empty(const string& filename)
{
    ifstream file(filename);
    if (!file)
        return true;

    string line;
    return !getline(file, line);
}

bool remove_book_by_id(const string& filename, int book_id)
{
    // NIE usuwamy książki jeśli jest wypożyczona (żeby loans się nie rozjechały)
    if (book_is_borrowed_by_someone(book_id, LOANS_FILE))
        return false;

    ifstream in(filename);
    if (!in)
        return false;

    const string tmp_name = filename + ".tmp";
    ofstream tmp(tmp_name);
    if (!tmp)
        return false;

    bool found = false;
    string line;

    while (getline(in, line))
    {
        if (line.empty())
            continue;

        int id;
        if (!parse_id(line, id))
            continue;

        if (id == book_id)
        {
            found = true;
            continue;
        }

        tmp << line << '\n';
    }

    in.close();
    tmp.close();

    if (!found)
    {
        remove(tmp_name.c_str());
        return false;
    }

    remove(filename.c_str());
    rename(tmp_name.c_str(), filename.c_str());
    return true;
}

void remove_book(const string& filename)
{
    if (is_library_empty(filename))
    {
        cout << "Library is empty." << endl;
        return;
    }

    int book_id;
    cout << "Enter book id to remove: ";
    cin >> book_id;

    if (book_is_borrowed_by_someone(book_id, LOANS_FILE))
    {
        cout << "Cannot remove: book is currently borrowed." << endl;
        return;
    }

    if (remove_book_by_id(filename, book_id))
        cout << "Book removed." << endl;
    else
        cout << "No book with this id (or cannot remove)." << endl;
}

bool borrow_book(int user_id, int book_id, const string& filename)
{
    // 1) jeśli loans mówi, że ktoś ma tę książkę -> stop
    if (book_is_borrowed_by_someone(book_id, LOANS_FILE))
    {
        cout << "Book already borrowed." << endl;
        return false;
    }

    ifstream in;
    ofstream tmp;
    if (!open_files(filename, in, tmp))
    {
        cout << "Error opening books file." << endl;
        return false;
    }

    string line;
    bool found = false;
    bool changed = false;

    while (getline(in, line))
    {
        if (line.empty())
            continue;

        int id;
        if (!parse_id(line, id))
        {
            tmp << line << '\n';
            continue;
        }

        if (id != book_id)
        {
            tmp << line << '\n';
            continue;
        }

        // id == book_id
        found = true;

        size_t last = line.rfind('|');
        int avail = stoi(line.substr(last + 1));

        if (avail == 0)
        {
            // już niedostępna wg books.txt
            tmp << line << '\n';
        }
        else
        {
            // zmieniamy na unavailable
            string prefix = line.substr(0, last + 1);
            tmp << prefix << 0 << '\n';
            changed = true;
        }
    }

    in.close();
    tmp.close();

    if (!found)
    {
        remove((filename + ".tmp").c_str());
        cout << "No book with this id." << endl;
        return false;
    }

    if (!changed)
    {
        remove((filename + ".tmp").c_str());
        cout << "Book already borrowed (books file says unavailable)." << endl;
        return false;
    }

    // podmiana pliku books
    remove(filename.c_str());
    rename((filename + ".tmp").c_str(), filename.c_str());

    // zapis do loans
    if (!add_loan(user_id, book_id, LOANS_FILE))
    {
        cout << "Loan save error (loans file)." << endl;
        // minimalnie: informacja; (idealnie: rollback books)
        return false;
    }

    cout << "Book borrowed." << endl;
    return true;
}

bool return_book(int user_id, int book_id, const string& filename)
{
    if (!loan_exists(user_id, book_id, LOANS_FILE))
    {
        cout << "You did not borrow this book." << endl;
        return false;
    }

    ifstream in;
    ofstream tmp;
    if (!open_files(filename, in, tmp))
    {
        cout << "Error opening books file." << endl;
        return false;
    }

    string line;
    bool found = false;
    bool changed = false;

    while (getline(in, line))
    {
        if (line.empty())
            continue;

        int id;
        if (!parse_id(line, id))
        {
            tmp << line << '\n';
            continue;
        }

        if (id != book_id)
        {
            tmp << line << '\n';
            continue;
        }

        // id == book_id
        found = true;

        size_t last = line.rfind('|');
        int avail = stoi(line.substr(last + 1));

        if (avail == 1)
        {
            // już dostępna wg books.txt
            tmp << line << '\n';
        }
        else
        {
            // zmieniamy na available
            string prefix = line.substr(0, last + 1);
            tmp << prefix << 1 << '\n';
            changed = true;
        }
    }

    in.close();
    tmp.close();

    if (!found)
    {
        remove((filename + ".tmp").c_str());
        cout << "No book with this id." << endl;
        return false;
    }

    if (!changed)
    {
        remove((filename + ".tmp").c_str());
        cout << "Book already returned (books file says available)." << endl;
        return false;
    }

    remove(filename.c_str());
    rename((filename + ".tmp").c_str(), filename.c_str());

    if (!remove_loan(user_id, book_id, LOANS_FILE))
    {
        cout << "Loan remove error (loans file)." << endl;
        return false;
    }

    cout << "Book returned." << endl;
    return true;
}

void display_books(const string& filename)
{
    ifstream in(filename);
    if (!in)
    {
        cout << "Error opening books file." << endl;
        return;
    }

    string line;
    while (getline(in, line))
    {
        if (!line.empty())
            cout << line << endl;
    }
}
