/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Header File  : ClientAccount.h
  Purpose      :
      - Defines the 'ClientAccount' class, derived from the abstract base class 'Account'
      - Provides a client-facing interface for interaction with the transport system

  Class Summary:
    1. ClientAccount (Derived from Account)
        - Overrides performActions()
        - Acts as a controller for:
            a) Collecting registration input (contact info and identity type)
            b) Presenting an interactive menu for booking-related operations (via performActions())
            c) Delegating core booking/viewing/cancellation logic to appropriate Client subclass
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef CLIENT_ACCOUNT_H
#define CLIENT_ACCOUNT_H

#include "Account.h"
#include "Client.h"
#include "Transport.h"

#include <fstream>
#include <sstream>

class ClientAccount : public Account {
public:
    string contact;
    int identity;

    ClientAccount(string uname, string pwd, string c, int id) : Account(uname, pwd), contact(c), identity(id) {}

    void registerAccount() {
        cout << "Enter contact number: ";
        cin.ignore();
        getline(cin, contact);
        cout << "------------------------\n";
        cout << "Select identity:\n";
        cout << "1. Tourist\n";
        cout << "2. Business Person\n";
        cout << "3. Logistics Company\n";
        cout << "Enter choice: ";
        cin >> identity;
        cout << "------------------------\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Client* client = nullptr;

        switch (identity) {
            case 1:
                client = new Tourist(username, contact);
                break;
            case 2:
                client = new BusinessPerson(username, contact);
                break;
            case 3:
                client = new LogisticsCompany(username, contact);
                break;
            default:
                cout << "Invalid identity selected.\n";
        }
    }

    void performActions(vector<Transport*>& fleet) override {
        Client* client = nullptr;
        int choice;
        do {
            switch (identity) {
                case 1:
                    client = new Tourist(username, contact);
                    break;
                case 2:
                    client = new BusinessPerson(username, contact);
                    break;
                case 3:
                    client = new LogisticsCompany(username, contact);
                    break;
            }
            cout << "\n--- Client Menu ---\n"
                 << "1. Book a Transport\n"
                 << "2. View Booking Records\n"
                 << "3. Cancel Booking\n"
                 << "0. Logout\n"
                 << "Choice: ";
            cin >> choice;
            cout << "------------------------\n";
            switch (choice) {
                case 1:
                    client->booking(fleet);
                    break;
                case 2:
                    client->viewBooking(fleet);
                    break;
                case 3:
                    client->cancelBooking(fleet);
                    break;
                case 0:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid option.\n";
                    cout << "------------------------\n";
            }
        } while (choice != 0);
        delete client;
    }
};

#endif
 