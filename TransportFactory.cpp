#include "Transport.h"

#include <fstream>
#include <sstream>

using namespace std;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Module     : Transport::deserialize()
Purpose    : Reconstruct a specific Transport-derived object from a line of CSV-formatted text (read fleet database).

Function   :
    1) deserialize( string from databse )
        - Splits a comma-separated string to extract transport attributes.
        - Determines the transport type from the first token (e.g., Car, Truck, SpeedBoat).
        - Dynamically dispatches to the correct Transport subclass based on type.
        - If the token list has more than 9 elements:
            > Second-to-last token is interpreted as booking status (isBooked)
            > Last token is the username of the client who booked it (bookedBy)
        - All other tokens are parsed and assigned to their respective constructor parameters.

Error Handling :
    - Wraps parsing logic in a try-catch block to handle conversion errors or malformed data.
    - Logs an error message to stderr if parsing fails.
    - Returns nullptr on failure or if the transport type is unrecognized.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
Transport* Transport::deserialize(const string& line) {
    istringstream ss(line);
    string token;
    vector<string> tokens;

    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.empty()) return nullptr;

    // Transport type token
    string type = tokens[0];
    bool isBooked = false;
    string bookedBy;

    try {
        if (tokens.size() > 9) {
            bookedBy = tokens.back();                                           // Username of the client who booked it (bookedBy)
            string bookedStr = tokens[tokens.size() - 2];
            isBooked = (bookedStr == "1" || bookedStr == "true"); //booking status (isBooked)
        }

        Transport* t = nullptr;

        if (type == "Car")
            t = new Car(tokens[1], tokens[2], tokens[3], tokens[4],
                        stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]));
        else if (type == "Bus")
            t = new Bus(tokens[1], tokens[2], tokens[3], tokens[4],
                        stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]));
        else if (type == "Van")
            t = new Van(tokens[1], tokens[2], tokens[3], tokens[4],
                        stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]));
        else if (type == "Truck")
            t = new Truck(tokens[1], tokens[2], tokens[3], tokens[4],
                          stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]), tokens[9]);
        else if (type == "PickupTruck")
            t = new PickupTruck(tokens[1], tokens[2], tokens[3], tokens[4],
                                stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]), tokens[9]);
        else if (type == "Trailer")
            t = new Trailer(tokens[1], tokens[2], tokens[3], tokens[4],
                            stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]), tokens[9]);
        else if (type == "SpeedBoat")
            t = new SpeedBoat(tokens[1], tokens[2], tokens[3], tokens[4],
                              stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]),
                              stod(tokens[9]), tokens[10]);
        else if (type == "CargoShip")
            t = new CargoShip(tokens[1], tokens[2], tokens[3], tokens[4],
                              stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]));
        else if (type == "PrivateAirplane")
            t = new PrivateAirplane(tokens[1], tokens[2], tokens[3], tokens[4],
                                    stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]),
                                    stod(tokens[9]));
        else if (type == "CargoPlane")
            t = new CargoPlane(tokens[1], tokens[2], tokens[3], tokens[4],
                               stod(tokens[5]), stod(tokens[6]), stoi(tokens[7]), stod(tokens[8]),
                               stod(tokens[9]));
        if (t) {
            t->setBooked(isBooked);
            t->setBookedBy(bookedBy);
        }
        return t;
    } catch (...) {
        cerr << "Error parsing line: " << line << endl;
        return nullptr;
    }
}
