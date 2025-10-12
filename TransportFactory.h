/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Header File  : TransportFactory.h
  Purpose      :
      - Defines the 'TransportFactory' class for centralized transport creation
      - Provides a console-based interface to instantiate specific Transport subclasses

  Class Summary:
    1. TransportFactory (Static Utility Class)
        - Contains:   
            a) createTransport(): Prompts user for input and constructs the appropriate
               derived Transport object (Car, Truck, SpeedBoat)
        - Handles all subclass-specific attributes dynamically (drive type, zone, range)
        - Returns a pointer to the newly constructed Transport object or nullptr if invalid
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    #ifndef TRANSPORT_FACTORY_H
    #define TRANSPORT_FACTORY_H

    #include "transport.h"

    #include <limits>

    class TransportFactory {
    public:
        static Transport* createTransport() {
            int choice;
            cout << "Choose Transport Type:\n"
                << "1. Car\n2. Bus\n3. Van\n"
                << "4. Truck\n5. Pickup Truck\n6. Trailer\n"
                << "7. Speed Boat\n8. Cargo Ship\n"
                << "9. Private Airplane\n10. Cargo Plane\n"
                << "Enter choice: ";
            cin >> choice;
            cout << "------------------------\n";
            cin.ignore();

            string brand, model, color, plate;
            double hp, cost, load;
            int passengers;

            cout << "Enter brand: "; getline(cin, brand);
            cout << "Enter model (with manufacture year): "; getline(cin, model);
            cout << "Enter color: "; getline(cin, color);
            cout << "Enter plate number: "; getline(cin, plate);
            cout << "Enter horsepower: "; cin >> hp;
            cout << "Enter cost per day (RM): "; cin >> cost;
            cout << "Enter passenger capacity: "; cin >> passengers;
            cout << "Enter load capacity (kg): "; cin >> load;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear buffer

            switch (choice) {
                case 1: {
                    cout << "------------------------\n";
                    return new Car(brand, model, color, plate, hp, cost, passengers, load);
                }
                case 2:{
                    cout << "------------------------\n";
                    return new Bus(brand, model, color, plate, hp, cost, passengers, load);
                }
                case 3:{
                    cout << "------------------------\n";
                    return new Van(brand, model, color, plate, hp, cost, passengers, load);
                }
                case 4: {
                    string drive;
                    cout << "Enter drive type (e.g., 4x4, 6x4): "; getline(cin, drive);
                    cout << "------------------------\n";
                    return new Truck(brand, model, color, plate, hp, cost, passengers, load, drive);
                }
                case 5: {
                    string drive;
                    cout << "Enter drive type (e.g., \"4WD\", \"FWD\"): "; getline(cin, drive);
                    cout << "------------------------\n";
                    return new PickupTruck(brand, model, color, plate, hp, cost, passengers, load, drive);
                }
                case 6: {
                    string type;
                    cout << "Enter trailer type (e.g., \"Flatbed\", \"Box\"): "; getline(cin, type);
                    cout << "------------------------\n";
                    return new Trailer(brand, model, color, plate, hp, cost, passengers, load, type);
                }
                case 7: {
                    double knotSpeed;
                    string zone;
                    cout << "Enter max knot spe ed (Maximum speed in knots (nautical miles per hour)): "; cin >> knotSpeed;
                    cin.ignore();
                    cout << "Enter zone (Operational zone (e.g., coastal, offshore)): "; getline(cin, zone);
                    cout << "------------------------\n";
                    return new SpeedBoat(brand, model, color, plate, hp, cost, passengers, load, knotSpeed, zone);
                }
                case 8:{
                    return new CargoShip(brand, model, color, plate, hp, cost, passengers, load);
                    cout << "------------------------\n";
                }
                case 9: {
                    double range;
                    cout << "Enter range (Maximum flight time in hours): "; cin >> range;
                    cout << "------------------------\n";
                    return new PrivateAirplane(brand, model, color, plate, hp, cost, passengers, load, range);
                }
                case 10: {
                    double range;
                    cout << "Enter range (Maximum flight time in hours): "; cin >> range;
                    cout << "------------------------\n";
                    return new CargoPlane(brand, model, color, plate, hp, cost, passengers, load, range);
                }
                default:
                    cout << "Invalid choice.\n";
                    cout << "------------------------\n";
                    return nullptr;
            }
        }
    };

    #endif
