// main.cpp
// ESE 224 Fall 2026 Midterm Project (starter code)
// Team members: Shushana
//

#include "Drone.h"
#include "Fleet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

void login()
{
    string username, password;
    bool loginPassed = false;

    while (!loginPassed) {
        cout << "Please enter your username: ";
        cin >> username;
        cout << "Please enter your password: ";
        cin >> password;

        ifstream file("users.txt");
        if (!file) {
            cerr << "Error: Unable to open users.txt." << endl;
            return;
        }

        string user, pass;
        while (file >> user >> pass) {
            if (user == username && pass == password) {
                loginPassed = true;
                break;
            }
        }

        file.close();
        
        if (loginPassed) {
            cout << "Login successful!\n" << endl;
        } else {
            cout << "Login unsuccessful. Please try again.\n" << endl;
        }
        
    }
}

void loadFleet(Fleet& fleet, const string& filename)
{
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open " << filename << endl;
        return;
    }

    string name, model, status;
    int id, x, y;
    double battery, maxPayload;
    int count = 0;

    while (file >> name >> id >> model >> battery >> maxPayload >> x >> y >> status) {
        Drone d(name, id, model, battery, maxPayload, x, y, status);
        if (fleet.addDrone(d)) {
            count++;
        }
    }
    file.close();
    cout << "Loaded " << count << " drones from " << filename << "." << endl;
}

void showMenu()
{
    cout << endl
         << "===== SkyDrop Menu =====" << endl
         << " 1. Display all drones" << endl
         << " 2. Sort by ID" << endl
         << " 3. Sort by battery" << endl
         << " 4. Sort by name" << endl
         << " 5. Shuffle fleet" << endl
         << " 6. Search by name" << endl
         << " 7. Search by ID" << endl
         << " 8. Find nearest available drone" << endl
         << " 9. Dispatch a delivery" << endl
         << "10. Recharge a drone" << endl
         << "11. Fleet status summary" << endl
         << "12. Process orders file" << endl
         << "13. Write fleet to file" << endl
         << "14. Add a new drone" << endl
         << " 0. Quit" << endl;
}

int main()
{
    login();

    Fleet fleet;
    loadFleet(fleet, "drones.txt");

    int choice = -1;
    while (choice != 0)
    {
        showMenu();
        cout << "Choose an option: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid option. Please enter a number." << endl;
            continue;
        }

        switch (choice)
        {
        case 1:
            fleet.displayAll();
            break;
        case 2:
            fleet.selectionSortByID();
            break;
        case 3:
            fleet.insertionSortByBattery();
            break;
        case 4:
            fleet.sortByName();
            break;
        case 5: {
            unsigned long long seed;
            cout << "Enter random seed: ";
            cin >> seed;
            fleet.shuffleFleet(seed);
            cout << "Fleet shuffled." << endl;
            break;
        }
        case 6: {
            string searchName;
            cout << "Enter drone name to search: ";
            cin >> searchName;
            int idx = fleet.linearSearchByName(searchName);
            if (idx != -1) {
                fleet.displayDrone(idx);
            }
            break;
        }
        case 7: {
            int searchID;
            cout << "Enter drone ID to search: ";
            cin >> searchID;
            int idx = fleet.binarySearchByID(searchID);
            if (idx != -1) {
                fleet.displayDrone(idx);
            }
            break;
        }
        case 8: {
            int x, y;
            double weight;
            cout << "Enter x, y, and weight: ";
            cin >> x >> y >> weight;
            int idx = fleet.findNearestAvailable(x, y, weight);
            if (idx != -1) {
                cout << "Nearest available drone:" << endl;
                fleet.displayDrone(idx);
            } else {
                cout << "No available drone can make this delivery." << endl;
            }
            break;
        }
        case 9: {
            int x, y;
            double weight;
            cout << "Enter x, y, and weight: ";
            cin >> x >> y >> weight;
            int idx = fleet.findNearestAvailable(x, y, weight);
            if (idx != -1) {
                cout << "Dispatching drone:" << endl;
                fleet.displayDrone(idx);
                fleet.dispatchDrone(idx, x, y, weight);
                cout << "Delivery successful!. Updated drone status:" << endl;
                fleet.displayDrone(idx);
            } else {
                cout << "Drones cannot make this delivery." << endl;
            }
            break;
        }
        case 10: {
            int id;
            cout << "Enter drone ID to recharge: ";
            cin >> id;
            fleet.rechargeDrone(id);
            break;
        }
        case 11:
            fleet.countByStatus();
            break;
        case 12:
            fleet.processOrders("orders.txt", "delivery_log.txt");
            cout << "Orders processed succesfully!." << endl;
            break;
        case 13:
            fleet.writeFleetToFile("fleet_output.txt");
            cout << "Fleet written to file sucessfully!" << endl;
            break;
        case 14: {
            string name, model, status;
            int id, x, y;
            double battery, maxPayload;
            cout << "Enter name, ID, model, battery, maxPayload, x, y, status: ";
            cin >> name >> id >> model >> battery >> maxPayload >> x >> y >> status;
            
            Drone d;
            d.setName(name);
            if (!d.setID(id) || !d.setModel(model) || !d.setBattery(battery) || 
                !d.setMaxPayload(maxPayload) || !d.setPosition(0, x) || 
                !d.setPosition(1, y) || !d.setStatus(status)) {
                cout << "Failed to add drone: invalid input values." << endl;
            } else {
                if (fleet.addDrone(d)) {
                    cout << "Drone added successfully." << endl;
                } else {
                    cout << "Failed to add drone: Duplicate ID." << endl;
                }
            }
            break;
        }
        case 0:
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid option." << endl;
        }
    }
    return 0;
}
