#pragma once
#include <string>
#include <fstream>
#include "models.h"

int get_next_id(const std::string& filename);

void add_book(const std::string& filename);
bool is_library_empty(const std::string& filename);

bool remove_book_by_id(const std::string& filename, int book_id);
void remove_book(const std::string& filename);

bool borrow_book(int user_id, int book_id, const std::string& filename);
bool return_book(int user_id, int book_id, const std::string& filename);

bool open_files(const std::string& filename, std::ifstream& in, std::ofstream& tmp);
bool parse_id(const std::string& line, int& id);

void display_books(const std::string& filename);

void my_books(int user_id, const std::string& books_filename);
