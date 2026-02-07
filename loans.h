#pragma once
#include <string>

bool loan_exists(int user_id, int book_id, const std::string& filename);
bool book_is_borrowed_by_someone(int book_id, const std::string& filename);

bool add_loan(int user_id, int book_id, const std::string& filename);
bool remove_loan(int user_id, int book_id, const std::string& filename);

bool is_book_borrowed_by_user(int user_id, int book_id, const std::string& filename);
void display_user_loans(int user_id, const std::string& filename);
void ensure_loans_file_exists(const std::string& filename);