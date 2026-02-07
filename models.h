#pragma once
#include <string>

struct User
{
    int id;
    std::string login;
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
