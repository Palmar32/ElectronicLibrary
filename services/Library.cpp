
#include "Library.h"

#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

bool Library::addBook(const Book& book) {
    // Проверяем, что ID книги уникален.
    for (const Book& existingBook : books) {
        if (existingBook.getId() == book.getId()) {
            return false;
        }
    }

    books.push_back(book);
    return true;
}

bool Library::removeBook(int id) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getId() == id) {
            books.erase(it);
            return true;
        }
    }

    return false;
}

void Library::showAllBooks() const {
    if (books.empty()) {
        cout << "Каталог пока пуст.\n";
        return;
    }

    cout << "\n========== СПИСОК КНИГ ==========\n";

    for (const Book& book : books) {
        book.printInfo();
    }
}

void Library::searchByTitle(const string& query) const {
    if (query.empty()) {
        cout << "Введите непустой поисковый запрос.\n";
        return;
    }

    string normalizedQuery = query;

    // Упрощённое приведение к нижнему регистру.
    // Для русского текста понадобится отдельная обработка Unicode.
    transform(
        normalizedQuery.begin(),
        normalizedQuery.end(),
        normalizedQuery.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        }
    );

    bool found = false;

    for (const Book& book : books) {
        string title = book.getTitle();

        transform(
            title.begin(),
            title.end(),
            title.begin(),
            [](unsigned char c) {
                return static_cast<char>(tolower(c));
            }
        );

        if (title.find(normalizedQuery) != string::npos) {
            book.printInfo();
            found = true;
        }
    }

    if (!found) {
        cout << "Книги по запросу \""
             << query << "\" не найдены.\n";
    }
}

const vector<Book>& Library::getBooks() const {
    return books;
}

void Library::replaceBooks(const vector<Book>& newBooks) {
    books = newBooks;
}
