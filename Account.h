/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Header File : Account.h
  Purpose      :
      - Defines the abstract base class 'Account' used for user authentication
      - Serves as base for 'ClientAccount' and 'AdminAccount'

  Class Summary:
    1. Account (Abstract Base Class)
        - Stores login credentials (username and password)
        - Provides basic getters for credentials
        - Declares a pure virtual function performActions() for derived user roles to implement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Transport.h"

#include <string>

using namespace std;

class Account {
protected:
    string username;
    string password;

public:
    // Constructor
    Account(string uname, string pwd) : username(uname), password(pwd) {}
    virtual ~Account() {}

    // Pure virtual function to be implemented by derived classes
    virtual void performActions(vector<Transport*>& fleet) = 0;

    // Getters for username and password
    string getUsername() const { return username; }
    string getPassword() const { return password; }
};

#endif
