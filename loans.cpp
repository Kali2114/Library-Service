#include <fstream>
#include <string>
#include <cstdio>

#include "loans.h"

using namespace std;

void ensure_loans_file_exists(const string& filename)
{
    ifstream in(filename);
    if (in.good())
        return;

    ofstream out(filename, ios::app);
}

bool loan_exists(int user_id, int book_id, const string& filename)
{
    ifstream file(filename);
    if (!file)
        return false;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        size_t sep = line.find('|');
        if (sep == string::npos)
            continue;

        int uid = stoi(line.substr(0, sep));
        int bid = stoi(line.substr(sep + 1));

        if (uid == user_id && bid == book_id)
            return true;
    }
    return false;
}

bool book_is_borrowed_by_someone(int book_id, const string& filename)
{
    ifstream file(filename);
    if (!file)
        return false;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        size_t sep = line.find('|');
        if (sep == string::npos)
            continue;

        int bid = stoi(line.substr(sep + 1));
        if (bid == book_id)
            return true;
    }
    return false;
}

bool add_loan(int user_id, int book_id, const string& filename)
{
    ofstream file(filename, ios::app);
    if (!file)
        return false;

    file << user_id << "|" << book_id << '\n';
    return true;
}

bool remove_loan(int user_id, int book_id, const string& filename)
{
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

        size_t sep = line.find('|');
        if (sep == string::npos)
            continue;

        int uid = stoi(line.substr(0, sep));
        int bid = stoi(line.substr(sep + 1));

        if (uid == user_id && bid == book_id)
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
