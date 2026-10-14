
#ifndef BOOK_H
#define BOOK_H

#include <string>

using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    int year;
    string genre;
    bool available;

public:
    Book();

    Book(int id, const string& title,
         const string& author, int year,
         const string& genre);

    int getId() const;
    string getTitle() const;
    string getAuthor() const;
    int getYear() const;
    string getGenre() const;

    bool isAvailable() const;
    void setAvailable(bool value);

    void printInfo() const;
};

#endif
