#include "Account.h"
#include "AdminAccount.h"
#include "Client.h"
#include "ClientAccount.h"
#include "TransportFactory.h"

#include <fstream>
#include <sstream>
#include <string>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Module     : DatabaseManager
Purpose    : Provide unified loading and saving functionalities for transport fleet
                   and user account data. This module handles persistent data I/O between
                   the program and database files.
Functions  :
    1) loadAccounts(destination, source)
       - Load user account data (admin/client) from file and populate into account list.
    2) saveAccounts(source, destination)
       - Write account data (with role/contact/id if client) into output file.
    3) loadFleet(destination, source)
       - Load serialized transport data from file and construct objects into fleet vector.
    4) saveFleet(source, destination)
       - Serialize and write transport objects into output file.

Database's format :
    1) Account file format  :
        - if admin: (username),(password),(role)
        - if client: ( username/ name ),(password),(role),(contact),
          (Type of client: 1 ->Tourist; 2 ->BusinessPerson; 3 -> LogisticsCompany)
    2) Fleet file format        :
        - (Type of transport),(Brand),(Model&Manufacture year),(colour),(plate number),
          (horsepower),(cost per day (RM)),(passenger capacity),(load capacity (kg)),...,(isBooked),(bookingBy)
        - "..." is for special class has special attribute to save in database

Error handling :
    - Exception handling for invalid client data in loadAccounts
    - Warning & show message when destination file can't be opened in saveAccounts.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// +++++++++++++++++++++++++++++++++++++++++++
// | Notes for Developers:
// | Admin Username : admin1  ; Admin Password£º adminpass
// +++++++++++++++++++++++++++++++++++++++++++
void loadFleet(vector<Transport*>& fleet, const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        Transport* t = Transport::deserialize(line);
        if (t) fleet.push_back(t);
    }
}

void saveFleet(const vector<Transport*>& fleet, const string& filename) {
    ofstream file(filename);
    for (auto t : fleet) {
        file << t->serialize() << endl;
    }
}

void loadAccounts(vector<Account*>& accountList, const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string u, p, role, contact, idStr;
        getline(ss, u, ',');
        getline(ss, p, ',');
        getline(ss, role, ',');

        if (role == "admin") {
            accountList.push_back(new AdminAccount(u, p));
        } else if (role == "client") {
            getline(ss, contact, ',');
            getline(ss, idStr);
            try {
                int id = stoi(idStr);
                accountList.push_back(new ClientAccount(u, p, contact, id));
            } catch (...) {
                cerr << "Failed to parse identity for: " << u << endl;
            }
        }
    }
}

void saveAccounts(vector<Account*>& accountList, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Could not open " << filename << " for writing.\n";
        return;
    }

    for (Account* acc : accountList) {
        if (auto* admin = dynamic_cast<AdminAccount*>(acc)) {
            file << admin->getUsername() << "," << admin->getPassword() << ",admin\n";
        } else if (auto* client = dynamic_cast<ClientAccount*>(acc)) {
            file << client->getUsername() << "," << client->getPassword()
                 << ",client," << client->contact << "," << client->identity << "\n";
        }
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Module     : AccountsManager
Purpose    : Handle user login and client registration functionalities.
Functions  :
    1) login()
       - Prompt user input for username and password.
       - Search for a matching account in the given list.
       - Return: Pointer to matching Account object or nullptr if not found.

    2) registerClient()
       - Prompt user input to register a new client account with conflict checks. (case-sensitive, spaces allowed)
       - Call registerAccount() on the new ClientAccount.
       - Append new client to the account list.
       - Return: Pointer to the newly created ClientAccount.

Error Handling :
    - login(): Display error message if credentials do not match.
    - registerClient(): Loop to ensure the username is unique among all existing accounts.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
Account* login(const vector<Account*>& accountList) {
    string uname, pwd;
    cout << "\n--- Login ---\n";
    cout << "Username (case-sensitive, spaces allowed): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean up input
    getline(cin,uname);
    cout << "Password: ";
    cin >> pwd;

    for (Account* acc : accountList) {
        if (acc->getUsername() == uname && acc->getPassword() == pwd) {
            return acc;
        }
    }
    cout << "Login failed.\n";
    cout << "------------------------\n";
    return nullptr;
}
ClientAccount* registerClient(vector<Account*>& accountList) {
    string uname, pwd;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (true) {
        cout << "\n--- Register Client Account ---\n";
        cout << "Create Username (case-sensitive, spaces allowed): ";
        getline(cin,uname);
        bool exists = false;
        for (Account* acc : accountList) {
            if (acc->getUsername() == uname) {
                exists = true;
                break;
            }
        }
        if (exists) {
            cout << "Username already exists. Please try a different one.\n";
        } else {
            break;
        }
    }
    cout << "Create Password: ";
    cin >> pwd;
    ClientAccount* newUser = new ClientAccount(uname, pwd, "", 0);
    newUser->registerAccount();
    accountList.push_back(newUser);

    return newUser;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Module     : Main Function
Company  : Universal Transports Company
Purpose    : Entry point of the system. Provides interactive menu for user login and registration.
                   After login:
                    - Clients can view, rent, and interact with various types of transport (for people, goods, or both).
                    - Admins have special privileges to manage the transport database, including:
                      adding, viewing, and deleting transport records.
Menu        : 1. Login  2. Register (auto login upon successful registration)  0. Exit
Procedure/Flow :
    - Load data (accounts + fleet)
    - Display main menu in a loop
    - Handle login or registration
    - On valid login/registration: Call performActions() for user interaction
    - Save data on exit and free all heap memory
Error Handling :
    - Input validation for menu selection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int main() {
    // === File Configuration ===
    const string accountsFile = "accounts.txt";
    const string fleetFile = "fleet.txt";

    // === In-memory Database ===
    vector<Account*> accountList;
    vector<Transport*> fleet;
    loadAccounts(accountList, accountsFile);
    loadFleet(fleet, fleetFile);

//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    // Initial Fleet Setup (Hardcoded Transport Database)
//    // Builds the initial transport fleet with one instance of each transport subclass
//    // Notes for Developers:
//    // - This is intended for **initial run only**
//    // - After the first successful launch, this section should be commented out or removed
//    //   to avoid duplicating entries or memory leaks (with 'Ctrl + Shift+ C' in CodeBlock)
//    fleet.push_back(new Car("Toyota", "Vios", "Red", "CAR1234", 109.5, 120.0, 5, 200.0, false, ""));
//    fleet.push_back(new Bus("Mercedes", "Sprinter", "White", "BUS5678", 190.0, 300.0, 20, 800.0, false, ""));
//    fleet.push_back(new Van("Nissan", "NV350", "Silver", "VAN8765", 130.0, 150.0, 10, 400.0, false, ""));
//    fleet.push_back(new Truck("Volvo", "FH16", "Blue", "TRK4567", 540.0, 800.0, 2, 10000.0, "6x4"));
//    fleet.push_back(new PickupTruck("Ford", "Ranger", "Black", "PICK1122", 200.0, 250.0, 5, 1000.0, "4WD"));
//    fleet.push_back(new Trailer("Scania", "XT", "Gray", "TRL3344", 400.0, 600.0, 1, 12000.0, "Flatbed"));
//    fleet.push_back(new SpeedBoat("Yamaha", "212X", "White", "SPD7788", 300.0, 500.0, 8, 300.0, 45.0, "coastal"));
//    fleet.push_back(new CargoShip("Maersk", "Triple-E", "Navy", "SHIP9999", 80000.0, 5000.0, 25, 200000.0, false, ""));
//    fleet.push_back(new PrivateAirplane("Cessna", "Citation X", "White", "JET123", 1100.0, 10000.0, 6, 1500.0, 5.5));
//    fleet.push_back(new CargoPlane("Boeing", "747-8F", "White", "CPL456", 66000.0, 20000.0, 2, 137000.0, 10.0));
//    cout << "Fleet database initialized with " << fleet.size() << " transport units.\n";
//    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    Account* user = nullptr;

    int mainChoice;
    // === Menu Loop ===
    do {
        cout << "\n--- Welcome to Universal Transports ---\n";
        cout << "\n1. Login\n2. Register\n0. Exit Application\nChoice: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1:{
                user = login(accountList);
                if (user) {
                    user->performActions(fleet);
                }
                break;
            }
            case 2: {
                ClientAccount* newUser = registerClient(accountList);
                cout << "Registration successful. Logging you in...\n";
                newUser->performActions(fleet);
                break;
            }
            case 0:
                cout << "Thank you for using our system!\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            default:
                cout << "Invalid option. Try again.\n";
                cout << "------------------------\n";
        }
    } while (mainChoice != 0);

    // === Persist data and clean memory ===
    saveFleet(fleet, fleetFile);
    saveAccounts(accountList, accountsFile);

    for (auto t : fleet) delete t;
    for (auto acc : accountList) delete acc;

    return 0;
}
