#pragma once
#include <string>

int get_next_user_id(const std::string& filename);

void add_user(const std::string& filename);
bool user_exists(int user_id, const std::string& filename);
bool is_admin(int user_id, const std::string& filename);

void display_users(const std::string& filename);
