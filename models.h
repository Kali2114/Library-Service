#pragma once
#include <string>

struct User
{
    int id;
    std::string name;
    bool is_admin = false;
};

struct Book
{
    int id;
    std::string title;
    std::string author;
    int year;
    bool available = true;
};
