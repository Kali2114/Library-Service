#pragma once
#include <string>

bool loan_exists(int user_id, int book_id, const std::string& filename);
bool book_is_borrowed_by_someone(int book_id, const std::string& filename);

bool add_loan(int user_id, int book_id, const std::string& filename);
bool remove_loan(int user_id, int book_id, const std::string& filename);
