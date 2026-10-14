
#include "Loan.h"

using namespace std;

Loan::Loan()
    : id(0),
      bookId(0),
      readerId(0),
      returned(false) {
}

Loan::Loan(int id, int bookId, int readerId,
           const string& issueDate,
           const string& dueDate)
    : id(id),
      bookId(bookId),
      readerId(readerId),
      issueDate(issueDate),
      dueDate(dueDate),
      returned(false) {
}

int Loan::getId() const {
    return id;
}

int Loan::getBookId() const {
    return bookId;
}

int Loan::getReaderId() const {
    return readerId;
}

string Loan::getIssueDate() const {
    return issueDate;
}

string Loan::getDueDate() const {
    return dueDate;
}

bool Loan::isReturned() const {
    return returned;
}

void Loan::markReturned() {
    returned = true;
}
