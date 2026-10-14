
#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>

#include "../models/Book.h"

using namespace std;

class Library {
private:
    vector<Book> books;

public:
    // Добавляет книгу, если её ID ещё не занят.
    bool addBook(const Book& book);

    // Удаляет книгу по ID.
    bool removeBook(int id);

    // Выводит все книги.
    void showAllBooks() const;

    // Ищет книги по названию или его части.
    void searchByTitle(const string& query) const;

    // Даёт доступ к списку для сохранения в файл.
    const vector<Book>& getBooks() const;

    // Заменяет каталог загруженными данными.
    void replaceBooks(const vector<Book>& newBooks);

    // TODO: добавить редактирование книги.
    // TODO: добавить поиск по автору и ISBN.
    // TODO: добавить фильтрацию по жанру.
    // TODO: добавить сортировку.
    // TODO: добавить регистрацию читателей и выдачу книг.
};

#endif
