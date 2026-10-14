
#include "Book.h"

#include <iostream>

using namespace std;

// Конструктор по умолчанию.
Book::Book()
    : id(0), year(0), available(true) {
}

// Конструктор с параметрами.
Book::Book(int id, const string& title,
           const string& author, int year,
           const string& genre)
    : id(id),
      title(title),
      author(author),
      year(year),
      genre(genre),
      available(true) {
}

int Book::getId() const {
    return id;
}

string Book::getTitle() const {
    return title;
}

string Book::getAuthor() const {
    return author;
}

int Book::getYear() const {
    return year;
}

string Book::getGenre() const {
    return genre;
}

bool Book::isAvailable() const {
    return available;
}

void Book::setAvailable(bool value) {
    available = value;
}

void Book::printInfo() const {
    cout << "ID: " << id << '\n'
         << "Название: " << title << '\n'
         << "Автор: " << author << '\n'
         << "Жанр: " << genre << '\n'
         << "Год издания: " << year << '\n'
         << "Статус: "
         << (available ? "Доступна" : "Выдана")
         << '\n'
         << "-----------------------------------\n";
}
