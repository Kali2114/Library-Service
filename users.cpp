#include <iostream>
#include <fstream>
#include <string>
#include <limits>

#include "users.h"

using namespace std;

static void ignore_line()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int get_next_user_id(const string& filename)
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

void add_user(const string& filename)
{
    string name;
    int admin_flag;

    ignore_line();
    cout << "Enter user name: ";
    getline(cin, name);

    cout << "Is admin? (1/0): ";
    cin >> admin_flag;

    int id = get_next_user_id(filename);

    ofstream file(filename, ios::app);
    if (!file)
    {
        cout << "Error opening users file." << endl;
        return;
    }

    file << id << "|" << name << "|" << admin_flag << '\n';
    cout << "User added." << endl;
}

bool user_exists(int user_id, const string& filename)
{
    ifstream file(filename);
    if (!file)
        return false;

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        int id = stoi(line.substr(0, line.find('|')));
        if (id == user_id)
            return true;
    }
    return false;
}

bool is_admin(int user_id, const string& filename)
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
        int id = stoi(line.substr(0, sep));
        if (id == user_id)
        {
            size_t last = line.rfind('|');
            return stoi(line.substr(last + 1)) == 1;
        }
    }
    return false;
}

void display_users(const string& filename)
{
    ifstream file(filename);
    if (!file)
    {
        cout << "Error opening users file." << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (!line.empty())
            cout << line << endl;
    }
}
