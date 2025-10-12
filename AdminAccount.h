/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Header File : AdminAccount.h
  Purpose      :
      - Defines the 'AdminAccount' class, derived from abstract base class 'Account'
      - Implements administrative actions to manage the transport fleet

  Class Summary:
    1. AdminAccount (Derived from Account)
        - Overrides performActions()
        - Provides access to a console-based admin panel (via performActions())
        - Supports:
            a) Adding new transport entries via TransportFactory
            b) Deleting existing transports from the system
            c) Viewing details of all registered transports
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef ADMIN_ACCOUNT_H
#define ADMIN_ACCOUNT_H

#include "Account.h"
#include "Transport.h"
#include "TransportFactory.h"

#include <fstream>

class AdminAccount : public Account {
public:
    AdminAccount(string uname, string pwd) : Account(uname, pwd) {}

    void performActions(vector<Transport*>& fleet) override {
        int choice;
        do {
            cout << "\n[Admin Panel]\n"
                 << "1. Add Transport\n"
                 << "2. Delete Transport\n"
                 << "3. View All Transports\n"
                 << "0. Exit Admin Panel\n"
                 << "Enter choice: ";
            cin >> choice;
            cout << "------------------------\n";

            if (choice == 1) {
                Transport* t = TransportFactory::createTransport();
                if (t) {
                    t->addTransport();
                    fleet.push_back(t);
                }
            } else if (choice == 2) {

                for (size_t i = 0; i < fleet.size(); ++i) {
                    cout << "[" << i + 1 << "] "; fleet[i]->displayDetail();
                    cout << "------------------------\n";
                }
                cout << "Enter transport number to delete: ";
                int del;
                cin >> del;
                if (del > 0 && del <= fleet.size()) {
                    delete fleet[del - 1];
                    fleet.erase(fleet.begin() + del - 1);
                    cout << "Transport"<< "[" << del << "] " <<"deleted.\n";
                }
            } else if (choice == 3) {
                for (Transport* t : fleet) {
                    t->displayDetail();
                    cout << "------------------------\n";
                }
            }
        } while (choice != 0);
    }
};

#endif
