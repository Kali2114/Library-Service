/* Wykonawca projektu - Changmil Changlicki */

#include <iostream>
#include <string>

#include "books.h"
#include "users.h"
#include "config.h"
#include "menu.h"

using namespace std;

int main()
{
    int user_id = login_user();
    if (user_id == -1)
        return 0;
    int choice = -1;
    while (choice != 0)
    {   
        choice = display_menu();
        switch (choice)
        {
            case 1:
                display_books(BOOKS_FILE);
                break;

            case 2:
                my_books(user_id, BOOKS_FILE);
                break;

            case 3:
            {
                int book_id;
                cout << "Enter book id: ";
                cin >> book_id;
                borrow_book(user_id, book_id, BOOKS_FILE);
                break;
            }

            case 4:
            {
                int book_id;
                cout << "Enter book id: ";
                cin >> book_id;
                return_book(user_id, book_id, BOOKS_FILE);
                break;
            }

            case 5:
                if (!is_admin(user_id, USERS_FILE))
                {
                    cout << "Access denied (admin only)." << endl;
                    break;
                }
                remove_book(BOOKS_FILE);
                break;

            case 6:
                if (!is_admin(user_id, USERS_FILE))
                {
                    cout << "Access denied (admin only)." << endl;
                    break;
                }
                add_book(BOOKS_FILE);
                break;

            case 7:
                if (!is_admin(user_id, USERS_FILE))
                {
                    cout << "Access denied (admin only)." << endl;
                    break;
                }
                add_user(USERS_FILE);
                break;

            case 8:
                if (!is_admin(user_id, USERS_FILE))
                {
                    cout << "Access denied (admin only)." << endl;
                    break;
                }
                display_users(USERS_FILE);
                break;

            case 0:
                cout << "Bye." << endl;
                break;

            default:
                cout << "Unknown option." << endl;
        }
    }
    return 0;
}
