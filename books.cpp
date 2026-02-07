#include <iostream>
#include <fstream>
#include <cstdio>
#include "books.h"

using namespace std;


int get_next_id(const string & filename)
{
    ifstream file (filename);
    if (!file)
        return 1;
        
    string line;
    int max_id = 0;

    while (getline(file, line))
    {
        if (line.empty()) continue;
        
        int id = stoi(line.substr(0, line.find('|')));
        if (id > max_id)
            max_id = id;
    }
    return max_id + 1;  
}


void add_book(const string & filename)
{ 
    Book book;

    cin.ignore();

    cout << "Enter title:" << endl;
    getline(cin, book.title);

    cout << "Enter author:" << endl;
    getline(cin, book.author);

    cout << "Enter year: " << endl;
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
         << book.available << endl;

    cout << "Book added successfully.";
}


bool is_library_empty(const string& filename)
{
    ifstream file(filename);
    if (!file)
        return true;
    string line;
    if (!getline(file, line))
        return true;
    return false;
}
        

bool remove_book_by_id(const string& filename, int book_id)
{
    ifstream in(filename);
    if (!in)
    {
        cout << "Error open file." << endl;
        return false;
    }
    const string tmp = filename + ".tmp";
    ofstream out(tmp);
    if (!out)
        return false;
    bool found = false;
    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;
        int sep = line.find('|');
        int id = stoi(line.substr(0, sep));
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
        remove(tmp.c_str());      
        return false;
    }

    remove(filename.c_str());
    rename(tmp.c_str(), filename.c_str());  
    return true;
}


void remove_book(const string& filename)
{   
    if (is_library_empty(filename))
    {
        cout << "Library is empty, no books.";
        return;
    }
    while (true)
    {
        int choice;
        cout << "Enter book id to remove, press 0 to exit: " << endl;
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


bool borrow_book(int user_id, int book_id)
{
    ifstream in("books.txt");
    if (!in)
    {
        cout << "Error open file." << endl;
        return false;
    }
    ifstream tmp("books.txt");
    if (!tmp)
    {
        cout << "Error open file." << endl;
        return false;
    }
}