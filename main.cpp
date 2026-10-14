
#include <iostream>
#include <limits>
#include <string>

#include "storage/Database.h"

using namespace std;

void showMenu() {
    cout << "\n===== Электронная библиотека =====\n"
         << "1. Показать все книги\n"
         << "2. Добавить книгу\n"
         << "3. Найти книгу по названию\n"
         << "4. Удалить книгу\n"
         << "5. Добавить читателя\n"
         << "6. Показать всех читателей\n"
         << "7. Выдать книгу\n"
         << "8. Вернуть книгу\n"
         << "9. История выдач\n"
         << "0. Выход\n"
         << "Ваш выбор: ";
}

int main() {
    Database database;

    if (!database.open("library.db")) {
        return 1;
    }

    if (!database.initialize()) {
        return 1;
    }

    int choice = -1;

    while (choice != 0) {
        showMenu();

        // Проверяем, что пользователь ввёл число.
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            cout << "Ошибка: введите номер пункта меню.\n";
            continue;
        }

        // Удаляем остаток строки после ввода числа.
        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        switch (choice) {
            case 1: {
                database.showAllBooks();
                break;
            }

            case 2: {
                string title, author, genre;
                int year;

                cout << "Введите название книги: ";
                getline(cin >> ws, title);

                cout << "Введите автора: ";
                getline(cin, author);

                cout << "Введите год издания: ";
                cin >> year;

                cout << "Введите жанр: ";
                getline(cin >> ws, genre);

                if (title.empty() || author.empty() || genre.empty()) {
                    cout << "Поля не должны быть пустыми.\n";
                } else if (year <= 0) {
                    cout << "Год должен быть положительным.\n";
                } else {
                    database.addBook(title, author, year, genre);
                }
                break;
            }

            case 3: {
                string query;

                cout << "Введите название или его часть: ";
                getline(cin, query);

                database.searchBooksByTitle(query);
                break;
            }

            case 4: {
                int id;

                cout << "Введите ID книги для удаления: ";
                cin >> id;

                database.deleteBook(id);
                break;
            }

            case 5: {
                string fullName, phone;

                cout << "Введите ФИО читателя: ";
                getline(cin >> ws, fullName);

                cout << "Введите телефон: ";
                getline(cin, phone);

                if (fullName.empty()) {
                    cout << "ФИО не должно быть пустым.\n";
                } else {
                    database.addReader(fullName, phone);
                }

                break;
            }

            case 6:
                database.showReaders();
                break;


            case 7: {
                int bookId, readerId;
                string issueDate, dueDate;

                cout << "ID книги: ";
                cin >> bookId;

                cout << "ID читателя: ";
                cin >> readerId;

                cout << "Дата выдачи (ГГГГ-ММ-ДД): ";
                cin >> issueDate;

                cout << "Срок возврата (ГГГГ-ММ-ДД): ";
                cin >> dueDate;

                database.issueBook(bookId, readerId, issueDate, dueDate);
                break;
            }

            case 8: {
                int loanId;

                cout << "ID операции выдачи: ";
                cin >> loanId;

                database.returnBook(loanId);
                break;
            }

            case 9:
                database.showLoans();
                break;


            case 0: {
                break;
            }

            default:
                cout << "Такого пункта меню нет.\n";
        }
    }

    return 0;
}
