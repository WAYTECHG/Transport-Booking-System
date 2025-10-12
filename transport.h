/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Header File : transport.h
  Purpose      : - Defines the Transport base class and all derived types (Land, Water, Air)
                       - Manages transport attributes, booking status, and serialization support
  Class Hierarchy:
    1. Transport (Abstract Base Class)
        - Stores common transport attributes (brand, model, plate number, capacities, etc.)
        - Handles booking status and customer association
        - Provides interfaces for:
            a) Displaying transport details (virtual)
            b) Adding transport to system (pure virtual)
            c) Serializing and deserializing transport data (pure virtual + static)

    2. LandTransport / WaterTransport / AirTransport (Intermediate Abstract Classes)
        - Serve as base classes for specific types of transport within each category
        - Implement inheritance of shared functionality from Transport

    3. 3rd generation Transport class:
        a) Land:
            - Car, Bus, Van (Passenger)
            - Truck, PickupTruck, Trailer (Cargo)
        b) Water:
            - SpeedBoat (Passenger), CargoShip (Cargo)
        c) Air:
            - PrivateAirplane (Passenger), CargoPlane (Cargo)

Special Attributes Overview of Transport Subclasses
    1. Car / Bus / Van: no special attributes
    2. Truck & PickupTruck class: driveType (e.g., "4WD", "FWD")
    3. Trailer: trailerType (e.g., "Flatbed", "Box")
    4. SpeedBoat:
        - maxKnotSpeed: Maximum speed in knots (nautical miles per hour)
        - zone: Operational zone (e.g., coastal, offshore)
    5. PrivateAirplane & CargoPlane: rangeHours = Maximum flight time in hours
 
  Booking Information:
    - Each transport can be booked by a client (tracked using isBooked and bookedBy)
    - Only one active booking per transport at a time
    - Display includes current booking status
  Serialization: ( serialize()/ deserialize() )
    - Each transport type supports conversion to and from string format for file I/O
  Matching Utility:
    - matchTransport(): Filters transport options based on client type and booking requirements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Transport {
protected:
    // Basic transport attributes
    string brand, model, color, plateNumber;
     // Technical specs
    double horsepower, costPerDay, loadCapacityKg;
    int passengerCapacity;
     // Booking info with default value
    bool isBooked = false;
    string bookedBy = "";

public:
   // Constructor for new transport (used when adding a new vehicle by admin)
    Transport(string brand, string model, string color, string plateNumber,
              double horsepower, double costPerDay,
              int passengerCapacity, double loadCapacityKg)
        : brand(brand), model(model), color(color), plateNumber(plateNumber),
          horsepower(horsepower), costPerDay(costPerDay),
          passengerCapacity(passengerCapacity), loadCapacityKg(loadCapacityKg) {}

    // Constructor for transport with booking data
    // (used when first building the transport database from hardcoding)
    Transport(string brand, string model, string color, string plateNumber,
              double horsepower, double costPerDay,
              int passengerCapacity, double loadCapacityKg,
              bool isBooked, const string& bookedBy)
        : brand(brand), model(model), color(color), plateNumber(plateNumber),
          horsepower(horsepower), costPerDay(costPerDay),
          passengerCapacity(passengerCapacity), loadCapacityKg(loadCapacityKg),
          isBooked(isBooked), bookedBy(bookedBy) {}

    virtual ~Transport() {}
    // ---------------- Getters ----------------
    string getPlateNumber() const { return plateNumber; }
    double getCostPerDay() const { return costPerDay; }
    double getLoadCapacityKg() const { return loadCapacityKg; }
    int getPassengerCapacity() const { return passengerCapacity; }
    bool getBooked() const { return isBooked; }
    string getBookedBy() const { return bookedBy; }

    // ---------------- Setters ----------------
    void setCostPerDay(double price) { costPerDay = price; }
    void setBooked(bool status) { isBooked = status; }
    void setBookedBy(const string& name) { bookedBy = name; }

    // ---------------- Display Details ----------------
    virtual void displayDetail() const {
        cout << "Brand: " << brand
             << "\nModel: " << model
             << "\nColor: " << color
             << "\nPlate Number: " << plateNumber
             << "\nHorsepower: " << horsepower
             << "\nCost per Day: RM" << costPerDay
             << "\nPassenger Capacity: " << passengerCapacity
             << "\nLoad Capacity: " << loadCapacityKg << " kg"
             << "\nBooked Status: " << (isBooked ? "Booked by " + bookedBy : "Available") << "\n";
    }

    // ---------------- Pure Virtual Methods ----------------
    // Each derived transport class must implement these:
    virtual void addTransport() = 0;       // Used when adding transport interactively
    virtual string serialize() const = 0;  // Used for file saving (CSV-like string)

    // Static factory method to recreate a Transport from saved data (implemented elsewhere)
    static Transport* deserialize(const string& line);
};
// -- Intermediate Abstract Classes --
class LandTransport : public Transport {
public:
    using Transport::Transport;
    void addTransport() override = 0;
};

class WaterTransport : public Transport {
public:
    using Transport::Transport;
    void addTransport() override = 0;
};

class AirTransport : public Transport {
public:
    using Transport::Transport;
    void addTransport() override = 0;
};

// ---- Land Passenger Types ----
class Car : public LandTransport {
public:
    using LandTransport::LandTransport;
    void addTransport() override { cout << "Car added.\n"; }
    string serialize() const override {
        return "Car," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg) + "," +
                   to_string(isBooked) + "," + bookedBy;
    }
};

class Bus : public LandTransport {
public:
    using LandTransport::LandTransport;
    void addTransport() override { cout << "Bus added.\n"; }
    string serialize() const override {
        return "Bus," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg)+ "," +
                   to_string(isBooked)+ "," + bookedBy;
    }
};

class Van : public LandTransport {
public:
    using LandTransport::LandTransport;
    void addTransport() override { cout << "Van added.\n"; }
    string serialize() const override {
        return "Van," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg)+ "," +
                   to_string(isBooked)+ "," + bookedBy;
    }
};

// ---- Land Cargo Types ----
class Truck : public LandTransport {
    string driveType;
public:
    Truck(string brand, string model, string color, string plateNumber,
              double horsepower, double costPerDay, int passengerCapacity,
              double loadCapacityKg, string driveType)
        : LandTransport(brand, model, color, plateNumber, horsepower, costPerDay,
              passengerCapacity, loadCapacityKg), driveType(driveType) {}
    void displayDetail() const override {
        LandTransport::displayDetail();
        cout << "Drive Type: " << driveType << endl;
    }
    void addTransport() override { cout << "Truck added.\n"; }
    string serialize() const override {
        return "Truck," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg) + "," +
                   driveType + "," + to_string(isBooked) + "," + bookedBy;
    }
};

class PickupTruck : public LandTransport {
    string driveType;
public:
    PickupTruck(string brand, string model, string color, string plateNumber,
                        double horsepower, double costPerDay, int passengerCapacity,
                        double loadCapacityKg, string driveType)
        : LandTransport(brand, model, color, plateNumber, horsepower, costPerDay,
                        passengerCapacity, loadCapacityKg), driveType(driveType) {}
    void displayDetail() const override {
        LandTransport::displayDetail();
        cout << "Drive Type: " << driveType << endl;
    }
    void addTransport() override { cout << "Pickup Truck added.\n"; }
    string serialize() const override {
        return "PickupTruck," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg) + "," +
                   driveType+ "," + to_string(isBooked)+ "," + bookedBy;
    }
};

class Trailer : public LandTransport {
    string trailerType;
public:
    Trailer(string brand, string model, string color, string plateNumber,
               double horsepower, double costPerDay, int passengerCapacity,
               double loadCapacityKg, string trailerType)
        : LandTransport(brand, model, color, plateNumber, horsepower, costPerDay,
                passengerCapacity, loadCapacityKg), trailerType(trailerType) {}
    void displayDetail() const override {
        LandTransport::displayDetail();
        cout << "Trailer Type: " << trailerType << endl;
    }
    void addTransport() override { cout << "Trailer added.\n"; }
    string serialize() const override {
        return "Trailer," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg) + "," +
                   trailerType+ "," + to_string(isBooked)+ "," + bookedBy;
    }
};

// ---- Water Passenger ----
class SpeedBoat : public WaterTransport {
    double maxKnotSpeed;
    string zone;
public:
    SpeedBoat(string brand, string model, string color, string plateNumber,
                      double horsepower, double costPerDay, int passengerCapacity,
                      double loadCapacityKg, double maxKnotSpeed, string zone)
        : WaterTransport(brand, model, color, plateNumber, horsepower, costPerDay,
                      passengerCapacity, loadCapacityKg),
                      maxKnotSpeed(maxKnotSpeed), zone(zone) {}
    void displayDetail() const override {
        WaterTransport::displayDetail();
        cout << "Speed: " << maxKnotSpeed << " knots\nZone: " << zone << endl;
    }
    void addTransport() override { cout << "Speed Boat added.\n"; }
    string serialize() const override {
        return "SpeedBoat," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg) + "," +
                   to_string(maxKnotSpeed) + "," +zone+ "," + to_string(isBooked)+ "," + bookedBy;
    }
};

// ---- Water Cargo ----
class CargoShip : public WaterTransport {
public:
    using WaterTransport::WaterTransport;
    void addTransport() override { cout << "Cargo Ship added.\n"; }
    string serialize() const override {
        return "CargoShip," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg)+ "," +
                   to_string(isBooked)+ "," + bookedBy;
    }
};

// ---- Air Passenger ----
class PrivateAirplane : public AirTransport {
    double rangeHours;
public:
    PrivateAirplane(string brand, string model, string color, string plateNumber,
                    double horsepower, double costPerDay, int passengerCapacity,
                    double loadCapacityKg, double rangeHours)
        : AirTransport(brand, model, color, plateNumber, horsepower, costPerDay,
                    passengerCapacity, loadCapacityKg), rangeHours(rangeHours) {}
    void displayDetail() const override {
        AirTransport::displayDetail();
        cout << "Range: " << rangeHours << " hours\n";
    }
    void addTransport() override { cout << "Private Airplane added.\n"; }
    string serialize() const override {
        return "PrivateAirplane," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg)+ "," +
                   to_string(rangeHours)+ "," + to_string(isBooked)+ "," + bookedBy;
    }
};

// ---- Air Cargo ----
class CargoPlane : public AirTransport {
    double rangeHours;
public:
    CargoPlane(string brand, string model, string color, string plateNumber,
                   double horsepower, double costPerDay, int passengerCapacity,
                   double loadCapacityKg, double rangeHours)
        : AirTransport(brand, model, color, plateNumber, horsepower, costPerDay,
                    passengerCapacity, loadCapacityKg), rangeHours(rangeHours) {}
    void displayDetail() const override {
        AirTransport::displayDetail();
        cout << "Range: " << rangeHours << " hours\n";
    }
    void addTransport() override { cout << "Cargo Plane added.\n"; }
    string serialize() const override {
        return "CargoPlane," + brand + "," + model + "," + color + "," + plateNumber + "," +
                   to_string(horsepower) + "," + to_string(costPerDay) + "," +
                   to_string(passengerCapacity) + "," + to_string(loadCapacityKg)+ "," +
                   to_string(rangeHours)+ "," + to_string(isBooked)+ "," + bookedBy;
    }
};

// TransportType is used to specify the category of transport in matchTransport() function
enum class TransportType { Land = 1, Water=2, Air=3 };

// Determines if a given transport matches client needs (availability, capacity, type, and client profile)
inline bool matchTransport(Transport* t, int passengers, double cargoWeight, TransportType type, const string& clientType) {
    // 1. Transport must be available (not booked)
    if (t->getBooked()) return false;

    // 2. Must meet required capacity
    if (t->getPassengerCapacity() < passengers) return false;
    if (t->getLoadCapacityKg() < cargoWeight) return false;

    // 3. Must match requested transport type (Land / Water / Air)
    bool typeMatch = false;
    switch (type) {
        case TransportType::Land:
            typeMatch = dynamic_cast<Car*>(t) || dynamic_cast<Bus*>(t) || dynamic_cast<Van*>(t) ||
                                 dynamic_cast<Truck*>(t) || dynamic_cast<PickupTruck*>(t) || dynamic_cast<Trailer*>(t);
            break;
        case TransportType::Water:
            typeMatch = dynamic_cast<SpeedBoat*>(t) || dynamic_cast<CargoShip*>(t);
            break;
        case TransportType::Air:
            typeMatch = dynamic_cast<PrivateAirplane*>(t) || dynamic_cast<CargoPlane*>(t);
            break;
    }
    if (!typeMatch) return false;

    // 4. Must match client-specific allowed transport types
    if (clientType == "Tourist") {
        return dynamic_cast<Car*>(t) || dynamic_cast<Bus*>(t) || dynamic_cast<Van*>(t) ||
                    dynamic_cast<SpeedBoat*>(t) || dynamic_cast<PrivateAirplane*>(t);
    } else if (clientType == "LogisticsCompany") {
        return dynamic_cast<Truck*>(t) || dynamic_cast<PickupTruck*>(t) || dynamic_cast<Trailer*>(t) ||
                    dynamic_cast<CargoShip*>(t) || dynamic_cast<CargoPlane*>(t);
    } else if (clientType == "BusinessPerson") {
        return true;// Business persons can use any matching transport
    }
    return false;// Error handling: Unknown client type
}

#endif
