
#include "Reader.h"

#include <iostream>

using namespace std;

Reader::Reader() : id(0) {
}

Reader::Reader(int id, const string& fullName,
               const string& phone)
    : id(id),
      fullName(fullName),
      phone(phone) {
}

int Reader::getId() const {
    return id;
}

string Reader::getFullName() const {
    return fullName;
}

string Reader::getPhone() const {
    return phone;
}

void Reader::printInfo() const {
    cout << "ID читателя: " << id << '\n'
         << "ФИО: " << fullName << '\n'
         << "Телефон: " << phone << '\n';
}
