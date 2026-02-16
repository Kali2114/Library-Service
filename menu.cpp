#include <iostream>

#include "menu.h"
#include "users.h"
#include "config.h"

using namespace std;

int display_menu()
{
    cout << "\n--- MENU ---\n";
        cout << "1. Display books\n";
        cout << "2. My borrowed books\n";
        cout << "3. Borrow book\n";
        cout << "4. Return book\n";
        cout << "5. Remove book (admin)\n";
        cout << "6. Add book (admin)\n";
        cout << "7. Add user (admin)\n";
        cout << "8. Display users (admin)\n";
        
        cout << "0. Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;
        return choice;
}

int login_user()
{
    int user_id;

    cout << "Enter your user id: ";
    cin >> user_id;

    if (!user_exists(user_id, USERS_FILE))
    {
        cout << "User does not exist." << endl;
        return -1;
    }

    cout << "Logged in as user id: " << user_id << endl;
    return user_id;
}