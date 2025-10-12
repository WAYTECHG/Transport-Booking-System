/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Header File : client.h
  Purpose      : - Defines Client base class and derived client types (Tourist, BusinessPerson, LogisticsCompany)
                       - Manages client interactions: booking, viewing, and cancelling transport bookings.
  Class Hierarchy:
    1. Client (Abstract Base Class)
        - Stores common user info (name, contact).
        - Provides a virtual interface for:
            a) Booking transports (pure virtual)
            b) Viewing bookings
            c) Cancelling bookings (with double confirmation)
        - Includes a shared input method for booking requirements.

  Note: The 'viewBooking()'& 'cancelBooking()' are virtual to allow future customization in child classes.

    2. Tourist (Derived from Client)
        - Can only book unbooked passenger-type transports
    3. BusinessPerson (Derived from Client)
        - Can book both passenger and cargo transports.
    4. LogisticsCompany (Derived from Client)
        - Can only book cargo transports
    5. Utility Function: displayAndSelect()
        - Displays a list of available transport options and processes user selection.
        - Print out the transport ID as proof of booking (Plate number)

  Error Handling:
    - Displays appropriate messages when:
        ~No available transport options for booking
        ~No current bookings found for the client
  Flexibility:
    - Allows users to abort booking or cancellation by entering 0, in case they change their mind
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef CLIENT_H
#define CLIENT_H

#include "Transport.h"

#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// Displays available transport list and returns the selected one
inline Transport* displayAndSelect(vector<Transport*>& list);

// ==========================
//                  Client Base Class
// ==========================
class Client {
protected:
    string name;
    string contact;

public:
    Client() {}
    Client(string name, string contact) : name(name), contact(contact) {}
    virtual ~Client() {}

    void setInfo(string name, string contact) {
        this->name = name;
        this->contact = contact;
    }
    // Gathers transport requirement input from the user
    // Usage of '&': Collects user input for booking criteria and updates passed variables by reference
    void getBookingInput(int& passengers, double& cargoWeight, TransportType& typeChoice) {
        int typeInt;
        cout << "Enter number of passengers: ";
        cin >> passengers;

        cout << "Enter cargo weight (kg): ";
        cin >> cargoWeight;

        cout << "Choose transport type (1 = Land, 2 = Water, 3 = Air): ";
        cin >> typeInt;
        cout << "------------------------\n";

        typeChoice = static_cast<TransportType>(typeInt);
    }
    // Abstract booking method to be implemented by derived classes
    virtual void booking(const vector<Transport*>& fleet) = 0;
    // Displays all transports currently booked by this client (matched by transport's 'bookedBy' attribute)
    virtual void viewBooking(vector<Transport*>& fleet) {
            cout << "\n--- Viewing Booking Records ---\n";
            vector<int> bookedIndices;

            for (size_t i = 0; i < fleet.size(); ++i) {
                if (fleet[i]->getBooked() && fleet[i]->getBookedBy() == this->name) {
                    cout << bookedIndices.size() + 1 << ". " ;
                    fleet[i]->displayDetail() ;
                    cout << "------------------------\n";
                    bookedIndices.push_back(i);
                }
            }
            if (bookedIndices.empty()) {
                cout << "No transport is currently booked.\n";
                return;
            }
    }
    // Cancel a booking associated with this client
    virtual void cancelBooking(vector<Transport*>& fleet) {
        cout << "\n--- Cancel Booking ---\n";
        vector<int> bookedIndices;

        for (size_t i = 0; i < fleet.size(); ++i) {
            if (fleet[i]->getBooked() && fleet[i]->getBookedBy() == this->name) {
                cout << bookedIndices.size() + 1 << ". ";
                fleet[i]->displayDetail();
                cout << "------------------------\n";
                bookedIndices.push_back(i);
            }
        }
        if (bookedIndices.empty()) {
            cout << "You have no bookings to cancel.\n";
            return;
        }
        cout << "Enter the number of the transport to cancel (0 to exit): ";
        int choice;
        cin >> choice;
        if (choice == 0) {
            cout << "Cancellation aborted.\n";
            return;
        }
        if (choice < 1 || choice > static_cast<int>(bookedIndices.size())) {
            cout << "Invalid choice.\n";
            return;
        }
        int indexToCancel = bookedIndices[choice - 1];
        cout << "------------------------\n";
        cout << "\nYou selected to cancel:\n";
        fleet[indexToCancel]->displayDetail();

        char confirm;
        cout << "Are you sure you want to cancel this booking? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            fleet[indexToCancel]->setBooked(false);
            fleet[indexToCancel]->setBookedBy("");
            cout << "Booking cancelled successfully.\n";
        } else {
            cout << "Cancellation aborted.\n";
        }
    }
};

// ==========================
//                           Tourist
// ==========================
class Tourist : public Client {
public:
    Tourist() {}
    Tourist(string name, string contact) : Client(name, contact) {}

    // Only allows booking of passenger transports
    void booking(const vector<Transport*>& fleet) override {
        cout << "\n[Tourist Booking - Passenger Transport Only]\n";

        int passengers;
        double cargoWeight;
        TransportType type;
        getBookingInput(passengers, cargoWeight, type);

        vector<Transport*> candidates;
        for (Transport* t : fleet) {
            if (!t->getBooked() &&
                matchTransport(t, passengers, 0.0, type, "Tourist") &&
                (dynamic_cast<Car*>(t) || dynamic_cast<Bus*>(t) || dynamic_cast<Van*>(t) ||
                 dynamic_cast<SpeedBoat*>(t) || dynamic_cast<PrivateAirplane*>(t))) {
                candidates.push_back(t);
            }
        }
        // Calls displayAndSelect(); if a transport is selected, marks it as booked and returns it
        Transport* selected = displayAndSelect(candidates);
        if (selected) selected->setBookedBy(name);
    }
};

// ==========================
//                  BusinessPerson
// ==========================
class BusinessPerson : public Client {
public:
    BusinessPerson() {}
    BusinessPerson(string name, string contact) : Client(name, contact) {}

    // Allows booking of both passenger and cargo transports
    void booking(const vector<Transport*>& fleet) override {
        cout << "\n[BusinessPerson Booking - Passenger & Cargo]\n";

        int passengers;
        double cargoWeight;
        TransportType type;
        getBookingInput(passengers, cargoWeight, type);

        vector<Transport*> candidates;
        for (Transport* t : fleet) {
            if (!t->getBooked() &&
                matchTransport(t, passengers, cargoWeight, type, "BusinessPerson")) {
                candidates.push_back(t);
            }
        }
        Transport* selected = displayAndSelect(candidates);
        if (selected) selected->setBookedBy(name);
    }
};

// ==========================
//                  LogisticsCompany
// ==========================
class LogisticsCompany : public Client {
public:
    LogisticsCompany() {}
    LogisticsCompany(string name, string contact) : Client(name, contact) {}

    // Only allows booking of cargo transports
    void booking(const vector<Transport*>& fleet) override {
        cout << "\n[Logistics Company Booking - Cargo Only]\n";

        int passengers;
        double cargoWeight;
        TransportType type;
        getBookingInput(passengers, cargoWeight, type);

        vector<Transport*> candidates;
        for (Transport* t : fleet) {
            if (!t->getBooked() &&
                matchTransport(t, 0, cargoWeight, type, "LogisticsCompany") &&
                (dynamic_cast<Truck*>(t) || dynamic_cast<PickupTruck*>(t) ||
                 dynamic_cast<Trailer*>(t) || dynamic_cast<CargoShip*>(t) ||
                 dynamic_cast<CargoPlane*>(t))) {
                candidates.push_back(t);
            }
        }
        Transport* selected = displayAndSelect(candidates);
        if (selected) selected->setBookedBy(name);
    }
};

// ==========================
//                  Utility Function
// ==========================
inline Transport* displayAndSelect(vector<Transport*>& list) {
    vector<Transport*> available;
    for (Transport* t : list) {
        if (!t->getBooked()) {
            available.push_back(t);
        }
    }

    if (available.empty()) {
        cout << "No available transports for booking.\n";
        return nullptr;
    }

    for (size_t i = 0; i < available.size(); ++i) {
        cout << "[" << i + 1 << "]\n";
        available[i]->displayDetail();
        cout << "------------------------\n";
    }

    int choice;
    cout << "Enter the number of the transport to book (0 to cancel): ";
    cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(available.size())) {
        Transport* selected = available[choice - 1];
        selected->setBooked(true);

        // Generate Booking ID after booking successfully.
        cout << "Booking successful.\n";
        cout << "Booking ID: #" <<selected->getPlateNumber() << endl;
        return available[choice - 1];
    } else {
        cout << "Booking cancelled.\n";
        return nullptr;
    }
}

#endif
