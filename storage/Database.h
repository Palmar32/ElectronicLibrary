
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

using namespace std;

class Database {
private:
    sqlite3* db;

public:
    Database();
    ~Database();

    bool addBook(const string& title,
             const string& author,
             int year,
             const string& genre);

    void searchBooksByTitle(const string& query);
    bool deleteBook(int id);

    bool addReader(const string& fullName, const string& phone);
    void showReaders();

    bool issueBook(int bookId, int readerId,
               const string& issueDate, const string& dueDate);

    bool returnBook(int loanId);

    void showLoans();

    void showAllBooks();

    // Запрещаем случайное копирование подключения.
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    bool open(const string& filename);
    bool initialize();
    void close();

    sqlite3* getConnection() const;
};

#endif
