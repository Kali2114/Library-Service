#pragma once
#include <string>
#include "models.h"


int get_next_id(const std::string&);

void add_book(const std::string&);
bool is_library_empty(const std::string&);
bool remove_book_by_id(const std::string&, int);

void remove_book(const std::string&);
bool borrow_book(int, int);