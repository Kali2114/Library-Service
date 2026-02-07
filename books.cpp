#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "books.h"

using namespace std;


bool open_files(const string& filename, ifstream& in, ofstream& tmp)
{
    in.open(filename);
    if (!in)
        return false;

    tmp.open(filename + ".tmp");
    if (!tmp)
        return false;

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
    cout << "Enter title:" << endl;
    getline(cin, book.title);

    cout << "Enter author:" << endl;
    getline(cin, book.author);

    cout << "Enter year:" << endl;
    cin >> book.year;

    book.id = get_next_id(filename);
    book.available = true;

    ofstream file(filename, ios::app);
    if (!file)
    {
        cout << "Cant open file." << endl;
        return;
    }

    file << book.id << "|"
         << book.title << "|"
         << book.author << "|"
         << book.year << "|"
         << book.available << '\n';

    cout << "Book added successfully." << endl;
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
    ifstream in(filename);
    if (!in)
        return false;

    ofstream out(filename + ".tmp");
    if (!out)
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

        out << line << '\n';
    }

    in.close();
    out.close();

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
        cout << "Library is empty, no books." << endl;
        return;
    }

    while (true)
    {
        int choice;
        cout << "Enter book id to remove, press 0 to exit:" << endl;
        cin >> choice;

        if (choice == 0)
            return;

        if (choice < 0)
        {
            cout << "Invalid id, try again." << endl;
            continue;
        }

        if (remove_book_by_id(filename, choice))
        {
            cout << "Book removed." << endl;
            return;
        }
        else
        {
            cout << "No book with this id." << endl;
        }
    }
}

bool borrow_book(int user_id, int book_id, const string& filename)
{
    ifstream in;
    ofstream tmp;

    if (!open_files(filename, in, tmp))
    {
        cout << "Error open file." << endl;
        return false;
    }

    string line;
    bool found = false;
    bool borrowed = false;

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
            int avail = stoi(line.substr(last + 1));

            if (avail == 0)
            {
                tmp << line << '\n';
            }
            else
            {
                string prefix = line.substr(0, last + 1);
                tmp << prefix << 0 << '\n';
                borrowed = true;
            }
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

    if (!borrowed)
    {
        remove((filename + ".tmp").c_str());
        cout << "Book already borrowed." << endl;
        return false;
    }

    remove(filename.c_str());
    rename((filename + ".tmp").c_str(), filename.c_str());
    cout << "Book borrowed." << endl;
    return true;
}

bool return_book(int user_id, int book_id, const string& filename)
{
    ifstream in;
    ofstream tmp;

    if (!open_files(filename, in, tmp))
    {
        cout << "Error open file." << endl;
        return false;
    }

    string line;
    bool found = false;
    bool returned = false;

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
            int avail = stoi(line.substr(last + 1));

            if (avail == 1)
            {
                tmp << line << '\n';
            }
            else
            {
                string prefix = line.substr(0, last + 1);
                tmp << prefix << 1 << '\n';
                returned = true;
            }
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

    if (!returned)
    {
        remove((filename + ".tmp").c_str());
        cout << "Book was not borrowed." << endl;
        return false;
    }

    remove(filename.c_str());
    rename((filename + ".tmp").c_str(), filename.c_str());
    cout << "Book returned." << endl;
    return true;
}

