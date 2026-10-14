
#include "Database.h"

#include <iostream>

using namespace std;

Database::Database() : db(nullptr) {
}

Database::~Database() {
    close();
}

bool Database::open(const string& filename) {
    if (db != nullptr) {
        close();
    }

    int result = sqlite3_open(filename.c_str(), &db);

    if (result != SQLITE_OK) {
        cerr << "Ошибка открытия базы данных: "
             << (db ? sqlite3_errmsg(db) : "неизвестная ошибка")
             << '\n';

        close();
        return false;
    }

    // Включаем проверку внешних ключей.
    char* errorMessage = nullptr;

    result = sqlite3_exec(
        db,
        "PRAGMA foreign_keys = ON;",
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {
        cerr << "Ошибка настройки SQLite: "
             << (errorMessage ? errorMessage : "неизвестная ошибка")
             << '\n';

        sqlite3_free(errorMessage);
        close();
        return false;
    }

    return true;
}

bool Database::initialize() {
    if (db == nullptr) {
        cerr << "База данных не открыта.\n";
        return false;
    }

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            year INTEGER NOT NULL,
            genre TEXT NOT NULL,
            total_copies INTEGER NOT NULL DEFAULT 1,
            available_copies INTEGER NOT NULL DEFAULT 1,
            CHECK (total_copies >= 0),
            CHECK (available_copies >= 0),
            CHECK (available_copies <= total_copies)
        );

        CREATE TABLE IF NOT EXISTS readers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            full_name TEXT NOT NULL,
            phone TEXT
        );

        CREATE TABLE IF NOT EXISTS loans (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            book_id INTEGER NOT NULL,
            reader_id INTEGER NOT NULL,
            issue_date TEXT NOT NULL,
            due_date TEXT NOT NULL,
            returned INTEGER NOT NULL DEFAULT 0,
            FOREIGN KEY (book_id) REFERENCES books(id),
            FOREIGN KEY (reader_id) REFERENCES readers(id),
            CHECK (returned IN (0, 1))
        );
    )";

    char* errorMessage = nullptr;

    int result = sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK) {
        cerr << "Ошибка создания таблиц: "
             << (errorMessage ? errorMessage : "неизвестная ошибка")
             << '\n';

        sqlite3_free(errorMessage);
        return false;
    }

    cout << "Таблицы базы данных готовы.\n";
    return true;
}

void Database::close() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

sqlite3* Database::getConnection() const {
    return db;
}


bool Database::addBook(const string& title,
                       const string& author,
                       int year,
                       const string& genre) {
    const char* sql =
        "INSERT INTO books (title, author, year, genre, total_copies, available_copies) "
        "VALUES (?, ?, ?, ?, 1, 1);";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << '\n';
        return false;
    }

    sqlite3_bind_text(statement, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, author.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement, 3, year);
    sqlite3_bind_text(statement, 4, genre.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(statement) == SQLITE_DONE;

    if (success) {
        cout << "Книга добавлена в базу данных. ID: "
             << sqlite3_last_insert_rowid(db) << '\n';
    } else {
        cerr << "Ошибка добавления книги: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(statement);
    return success;
}

void Database::showAllBooks() {
    const char* sql =
        "SELECT id, title, author, year, genre, available_copies "
        "FROM books ORDER BY id;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cerr << "Ошибка чтения книг: " << sqlite3_errmsg(db) << '\n';
        return;
    }

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {
        found = true;

        cout << "\nID: " << sqlite3_column_int(statement, 0)
             << "\nНазвание: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 1))
             << "\nАвтор: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 2))
             << "\nГод: " << sqlite3_column_int(statement, 3)
             << "\nЖанр: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 4))
             << "\nДоступна: "
             << (sqlite3_column_int(statement, 5) > 0 ? "Да" : "Нет")
             << "\n-------------------------\n";
    }

    if (!found) {
        cout << "В базе данных пока нет книг.\n";
    }

    sqlite3_finalize(statement);
}


void Database::searchBooksByTitle(const string& query) {
    const char* sql =
        "SELECT id, title, author, year, genre, available_copies "
        "FROM books WHERE title LIKE ? ORDER BY id;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cerr << "Ошибка поиска: " << sqlite3_errmsg(db) << '\n';
        return;
    }

    string pattern = "%" + query + "%";
    sqlite3_bind_text(statement, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {
        found = true;

        cout << "\nID: " << sqlite3_column_int(statement, 0)
             << "\nНазвание: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 1))
             << "\nАвтор: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 2))
             << "\nГод: " << sqlite3_column_int(statement, 3)
             << "\nЖанр: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 4))
             << "\n-------------------------\n";
    }

    if (!found) {
        cout << "Книги по этому запросу не найдены.\n";
    }

    sqlite3_finalize(statement);
}

bool Database::deleteBook(int id) {
    const char* sql = "DELETE FROM books WHERE id = ?;";
    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cerr << "Ошибка удаления: " << sqlite3_errmsg(db) << '\n';
        return false;
    }

    sqlite3_bind_int(statement, 1, id);

    bool success = sqlite3_step(statement) == SQLITE_DONE;

    if (success && sqlite3_changes(db) > 0) {
        cout << "Книга удалена.\n";
    } else if (success) {
        cout << "Книга с таким ID не найдена.\n";
    } else {
        cerr << "Не удалось удалить книгу: " << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(statement);
    return success && sqlite3_changes(db) > 0;
}


bool Database::addReader(const string& fullName, const string& phone) {
    const char* sql =
        "INSERT INTO readers (full_name, phone) VALUES (?, ?);";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << '\n';
        return false;
    }

    sqlite3_bind_text(statement, 1, fullName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, phone.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(statement) == SQLITE_DONE;

    if (success) {
        cout << "Читатель добавлен. ID: "
             << sqlite3_last_insert_rowid(db) << '\n';
    } else {
        cerr << "Ошибка добавления читателя: "
             << sqlite3_errmsg(db) << '\n';
    }

    sqlite3_finalize(statement);
    return success;
}

void Database::showReaders() {
    const char* sql =
        "SELECT id, full_name, phone FROM readers ORDER BY id;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cerr << "Ошибка чтения читателей: " << sqlite3_errmsg(db) << '\n';
        return;
    }

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {
        found = true;

        cout << "\nID: " << sqlite3_column_int(statement, 0)
             << "\nФИО: "
             << reinterpret_cast<const char*>(
                    sqlite3_column_text(statement, 1))
             << "\nТелефон: ";

        const unsigned char* phone = sqlite3_column_text(statement, 2);
        cout << (phone ? reinterpret_cast<const char*>(phone) : "")
             << "\n-------------------------\n";
    }

    if (!found) {
        cout << "Читателей пока нет.\n";
    }

    sqlite3_finalize(statement);
}


bool Database::issueBook(int bookId, int readerId,
                         const string& issueDate, const string& dueDate) {
    if (db == nullptr) {
        cerr << "База данных не открыта.\n";
        return false;
    }

    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        cerr << "Не удалось начать транзакцию.\n";
        return false;
    }

    // Проверяем наличие доступного экземпляра.
    const char* updateSql =
        "UPDATE books SET available_copies = available_copies - 1 "
        "WHERE id = ? AND available_copies > 0;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, updateSql, -1, &statement, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Ошибка подготовки выдачи книги.\n";
        return false;
    }

    sqlite3_bind_int(statement, 1, bookId);
    bool updated = sqlite3_step(statement) == SQLITE_DONE;
    int changed = sqlite3_changes(db);
    sqlite3_finalize(statement);

    if (!updated || changed == 0) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cout << "Книга не найдена или нет доступных экземпляров.\n";
        return false;
    }

    // Записываем выдачу. Внешние ключи проверят ID читателя.
    const char* insertSql =
        "INSERT INTO loans (book_id, reader_id, issue_date, due_date, returned) "
        "VALUES (?, ?, ?, ?, 0);";

    if (sqlite3_prepare_v2(db, insertSql, -1, &statement, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Ошибка подготовки записи выдачи.\n";
        return false;
    }

    sqlite3_bind_int(statement, 1, bookId);
    sqlite3_bind_int(statement, 2, readerId);
    sqlite3_bind_text(statement, 3, issueDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 4, dueDate.c_str(), -1, SQLITE_TRANSIENT);

    bool inserted = sqlite3_step(statement) == SQLITE_DONE;
    sqlite3_finalize(statement);

    if (!inserted) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Не удалось записать выдачу: " << sqlite3_errmsg(db) << '\n';
        return false;
    }

    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Не удалось завершить выдачу.\n";
        return false;
    }

    cout << "Книга выдана. Номер операции: "
         << sqlite3_last_insert_rowid(db) << '\n';
    return true;
}

bool Database::returnBook(int loanId) {
    if (db == nullptr) {
        cerr << "База данных не открыта.\n";
        return false;
    }

    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        cerr << "Не удалось начать транзакцию.\n";
        return false;
    }

    // Находим активную выдачу и отмечаем её возвращённой.
    const char* updateLoanSql =
        "UPDATE loans SET returned = 1 "
        "WHERE id = ? AND returned = 0;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, updateLoanSql, -1, &statement, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Ошибка подготовки возврата.\n";
        return false;
    }

    sqlite3_bind_int(statement, 1, loanId);
    bool updated = sqlite3_step(statement) == SQLITE_DONE;
    int changed = sqlite3_changes(db);
    sqlite3_finalize(statement);

    if (!updated || changed == 0) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cout << "Активная выдача с таким ID не найдена.\n";
        return false;
    }

    // Увеличиваем число доступных экземпляров именно этой книги.
    const char* updateBookSql =
        "UPDATE books SET available_copies = available_copies + 1 "
        "WHERE id = (SELECT book_id FROM loans WHERE id = ?);";

    if (sqlite3_prepare_v2(db, updateBookSql, -1, &statement, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Ошибка подготовки обновления книги.\n";
        return false;
    }

    sqlite3_bind_int(statement, 1, loanId);
    bool bookUpdated = sqlite3_step(statement) == SQLITE_DONE;
    int bookChanged = sqlite3_changes(db);
    sqlite3_finalize(statement);

    if (!bookUpdated || bookChanged == 0) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Не удалось обновить доступность книги.\n";
        return false;
    }

    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        cerr << "Не удалось завершить возврат.\n";
        return false;
    }

    cout << "Книга возвращена.\n";
    return true;
}

void Database::showLoans() {
    const char* sql =
        "SELECT l.id, b.title, r.full_name, l.issue_date, l.due_date, l.returned "
        "FROM loans l "
        "JOIN books b ON b.id = l.book_id "
        "JOIN readers r ON r.id = l.reader_id "
        "ORDER BY l.id;";

    sqlite3_stmt* statement = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK) {
        cerr << "Ошибка чтения истории выдач: " << sqlite3_errmsg(db) << '\n';
        return;
    }

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {
        found = true;

        cout << "\nID операции: " << sqlite3_column_int(statement, 0)
             << "\nКнига: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 1))
             << "\nЧитатель: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 2))
             << "\nДата выдачи: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 3))
             << "\nСрок возврата: "
             << reinterpret_cast<const char*>(sqlite3_column_text(statement, 4))
             << "\nСтатус: "
             << (sqlite3_column_int(statement, 5) == 0 ? "На руках" : "Возвращена")
             << "\n-------------------------\n";
    }

    if (!found) {
        cout << "История выдач пока пуста.\n";
    }

    sqlite3_finalize(statement);
}
