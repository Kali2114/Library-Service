#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "books.h"
#include "loans.h"
#include "config.h"

using namespace std;

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

    cin.ignore();
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
    ensure_loans_file_exists(LOANS_FILE);

    if (book_is_borrowed_by_someone(book_id, LOANS_FILE))
    {
        cout << "Cannot remove: book is currently borrowed." << endl;
        return false;
    }

    ifstream in(filename);
    if (!in)
        return false;

    ofstream tmp(filename + ".tmp");
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
        remove((filename + ".tmp").c_str());
        return false;
    }

    remove(filename.c_str());
    rename((filename + ".tmp").c_str(), filename.c_str());
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

    if (remove_book_by_id(filename, book_id))
        cout << "Book removed." << endl;
    else
        cout << "No book with this id (or borrowed)." << endl;
}

bool borrow_book(int user_id, int book_id, const string& filename)
{
    ensure_loans_file_exists(LOANS_FILE);

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
        }
        else
        {
            found = true;
            size_t last = line.rfind('|');
            string prefix = line.substr(0, last + 1);
            tmp << prefix << 0 << '\n';
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

    remove(filename.c_str());
    rename((filename + ".tmp").c_str(), filename.c_str());

    if (!add_loan(user_id, book_id, LOANS_FILE))
    {
        cout << "Failed to save loan record (loans.txt path/permissions)." << endl;
        return false;
    }

    cout << "Book borrowed." << endl;
    return true;
}

bool return_book(int user_id, int book_id, const string& filename)
{
    ensure_loans_file_exists(LOANS_FILE);

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
        }
        else
        {
            found = true;
            size_t last = line.rfind('|');
            string prefix = line.substr(0, last + 1);
            tmp << prefix << 1 << '\n';
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

    remove(filename.c_str());
    rename((filename + ".tmp").c_str(), filename.c_str());

    if (!remove_loan(user_id, book_id, LOANS_FILE))
    {
        cout << "Failed to remove loan record." << endl;
        return false;
    }

    cout << "Book returned." << endl;
    return true;
}

void display_books(const string& filename)
{
    ensure_loans_file_exists(LOANS_FILE);

    ifstream in(filename);
    if (!in)
    {
        cout << "Error opening books file." << endl;
        return;
    }

    cout << "\n--- BOOKS ---\n";
    cout << "Format: id | title | author | year | status\n\n";

    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;

        int book_id;
        if (!parse_id(line, book_id))
        {
            cout << line << endl;
            continue;
        }

        size_t last = line.rfind('|');
        string prefix = (last == string::npos) ? line : line.substr(0, last);

        bool borrowed = book_is_borrowed_by_someone(book_id, LOANS_FILE);
        cout << prefix << " | " << (borrowed ? "BORROWED" : "AVAILABLE") << endl;
    }
}

void display_my_borrowed_books(int user_id, const string& books_filename)
{
    ensure_loans_file_exists(LOANS_FILE);

    ifstream books(books_filename);
    if (!books)
    {
        cout << "Error opening books file." << endl;
        return;
    }

    bool any = false;
    string line;

    cout << "\n--- MY BORROWED BOOKS ---\n";

    while (getline(books, line))
    {
        if (line.empty())
            continue;

        int book_id;
        if (!parse_id(line, book_id))
            continue;

        if (loan_exists(user_id, book_id, LOANS_FILE))
        {
            any = true;
            size_t last = line.rfind('|');
            string prefix = (last == string::npos) ? line : line.substr(0, last);
            cout << prefix << endl;
        }
    }

    if (!any)
        cout << "(none)" << endl;
}
