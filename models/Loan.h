
#ifndef LOAN_H
#define LOAN_H

#include <string>

using namespace std;

class Loan {
private:
    int id;
    int bookId;
    int readerId;
    string issueDate;
    string dueDate;
    bool returned;

public:
    Loan();

    Loan(int id, int bookId, int readerId,
         const string& issueDate,
         const string& dueDate);

    int getId() const;
    int getBookId() const;
    int getReaderId() const;

    string getIssueDate() const;
    string getDueDate() const;

    bool isReturned() const;
    void markReturned();
};

#endif
