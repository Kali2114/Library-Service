#include <iostream>
#include <string>

#include "books.h"
#include "users.h"
#include "config.h"

using namespace std;

int main()
{
    int user_id;

    cout << "Enter your user id: ";
    cin >> user_id;

    if (!user_exists(user_id, USERS_FILE))
    {
        cout << "User does not exist." << endl;
        return 0;
    }

    cout << "Logged in as user id: " << user_id << endl;

    while (true)
    {
        cout << "\n--- MENU ---\n";
        cout << "1. Display books\n";
        cout << "2. Borrow book\n";
        cout << "3. Return book\n";
        cout << "4. Remove book (admin)\n";
        cout << "5. Add book (admin)\n";
        cout << "6. Add user (admin)\n";
        cout << "7. Display users (admin)\n";
        cout << "8. My borrowed books\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:
                display_books(BOOKS_FILE);
                break;

            case 2:
            {
                int book_id;
                cout << "Enter book id: ";
                cin >> book_id;
                borrow_book(user_id, book_id, BOOKS_FILE);
                break;
            }

            case 3:
            {
                int book_id;
                cout << "Enter book id: ";
                cin >> book_id;
                return_book(user_id, book_id, BOOKS_FILE);
                break;
            }

            case 4:
                if (!is_admin(user_id, USERS_FILE))
                {
                    cout << "Access denied (admin only)." << endl;
                    break;
                }
                remove_book(BOOKS_FILE);
                break;

            case 5:
                if (!is_admin(user_id, USERS_FILE))
                {
                    cout << "Access denied (admin only)." << endl;
                    break;
                }
                add_book(BOOKS_FILE);
                break;

            case 6:
                if (!is_admin(user_id, USERS_FILE))
                {
                    cout << "Access denied (admin only)." << endl;
                    break;
                }
                add_user(USERS_FILE);
                break;

            case 7:
                if (!is_admin(user_id, USERS_FILE))
                {
                    cout << "Access denied (admin only)." << endl;
                    break;
                }
                display_users(USERS_FILE);
                break;

            case 8:
                display_my_borrowed_books(user_id, BOOKS_FILE);
                break;

            case 0:
                cout << "Bye." << endl;
                return 0;

            default:
                cout << "Unknown option." << endl;
        }
    }
}
