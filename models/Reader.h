
#ifndef READER_H
#define READER_H

#include <string>

using namespace std;

class Reader {
private:
    int id;
    string fullName;
    string phone;

public:
    Reader();
    Reader(int id, const string& fullName,
           const string& phone);

    int getId() const;
    string getFullName() const;
    string getPhone() const;

    void printInfo() const;
};

#endif
