// Fleet.cpp
// ESE 224 Fall 2026 Midterm Project (starter code)
// Team members: TODO
//
// Every function below compiles but does nothing useful yet.
// Replace each TODO with your implementation (see Section 4 of the handout).
// Remember: no <algorithm>, no std::sort, std::swap, std::find, or std::shuffle.

#include "Fleet.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

// ---------- Private helpers ---------- // YANGZOOM

// Provided random number generator. Copy exactly; do not change.
unsigned long long Fleet::nextRandom(unsigned long long &state) const
{
    state = (state * 1103515245ULL + 12345ULL) % 2147483648ULL;
    return state;
}

void Fleet::swapDrones(int i, int j)
{
    // TODO: exchange drones[i] and drones[j] using a temporary Drone
    Drone temp = drones[i];
    drones[i] = drones[j];
    drones[j] = temp;
}

bool Fleet::validIndex(int index) const
{
    // TODO: true if 0 <= index < number of drones
    if (index >= 0 && index < (int)drones.size())
        return true;
    return false;
}

// ---------- Accessors and mutators ----------

bool Fleet::addDrone(const Drone& d)
{
    // TODO: reject a duplicate ID (use operator==), otherwise add to the end
    for (const Drone& drone : drones) {
        if (drone == d){
            return false; // Duplicate ID found
        }
    }

    drones.push_back(d); // Add the drone to the end of the vector
    return true;
}

Drone Fleet::getDrone(int index) const
{
    // TODO: check the range; print an error and return Drone() if invalid
    if (!validIndex(index)) {
        cout << "Error: Invalid index." << endl;
    }
    return Drone();
}

int Fleet::getSize() const
{
    return (int)drones.size();
}

// ---------- Sorting ----------
// Follow the counting rules in Section 4.2 exactly; your counts are checked.

void Fleet::selectionSortByID()
{
    // TODO: ascending by ID. Print: Selection sort by ID: C comparisons, S swaps.
    int c=0, s=0;

    for(int i =0; i< getSize() ; i++){
        int min = i;
        for (int j =i+1; j< getSize(); i++){
            c++;
            if(drones[j].getID() < drones[min].getID()){
                min=j;
            }
        }
        if (min!= i){
            swapDrones(i, min);
            s++; 
        }
    }
    cout << " election sort by ID: comparisons "  << c << "swaps " << s; 
}

void Fleet::insertionSortByBattery()
{
    // TODO: descending by battery. Print: Insertion sort by battery: C comparisons, H shifts.
    int c=0, h=0;
    for (int i = 1; i < getSize(); i++) {
        Drone key = drones[i];
        int j = i - 1;

        while (j >= 0) {
            c++;                                        
            if (drones[j].getBattery() < key.getBattery()) {
                drones[j + 1] = drones[j];             
                h++;
                j--;
            } else {
                break;                                 
            }
        }

        drones[j + 1] = key;
    }
    cout << "Insertion sort by battery: comparision " <<  c << "shifts "  << h;
}

void Fleet::sortByName()
{
    // TODO: insertion sort, alphabetical. Print: Insertion sort by name: C comparisons, H shifts.
    int c=0, h=0;

    for(int i =0; i< getSize() ; i++){
        Drone key = drones[i];
        int j = i - 1;
    
        while (j >= 0) {
            c++;
            if(drones[j].getName() >key.getName()){
                drones[j + 1] = drones[j];             
                h++;
                j--;
            } else {
                break;                                  
            }
        }

        drones[j + 1] = key;
    }
    cout << "Insertion sort by name: " << c << " comparisons, " << h << " shifts" << endl;

}


bool Fleet::isSortedByID() const
{
    // TODO
   for (int i = 1; i < (int)drones.size(); i++) {
        if (drones[i - 1].getID() > drones[i].getID()) {   
            return false;
        }
    }
    return true;
}

// ---------- Searching ---------- // SHUSHANA

int Fleet::linearSearchByName(const string& name) const
{
    // TODO: one comparison per drone checked; print the count; return index or -1
    int count=0;

    for (int i =0; i <getSize(); i++){
        count++;
        if(drones[i].getName()==name){
            cout << "Count" << count << endl;
        }
    }
    return -1;
}

int Fleet::binarySearchByID(int id)
{
    // TODO: sort first if needed (print a message), then binary search.
    // One comparison per loop iteration. Print the count; return index or -1.
    int count=0;
    if (!isSortedByID()){
        cout<< "not sorted" << endl;
        selectionSortByID();
    }

    int low = 0;
    int high = getSize() - 1;

    while (low <= high) {
        count++;                                       
        int mid = low + (high - low) / 2;

        if (drones[mid].getID() == id) {
            cout << "Binary search by ID: " << count << " comparisons" << endl;
            return mid;
        } else if (drones[mid].getID() < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

}

// ---------- Dispatching ---------- 

int Fleet::findNearestAvailable(int x, int y, double weight) const
{
    // TODO: closest drone that canDeliver; ties go to the smaller ID; -1 if none
    int best= 0, bestDistance =0;
    bool found = false;


    for (int i=0; i< getSize(); i++){
        if(drones[i].canDeliver(x, y, weight)){
            continue; // can deliver is true
        }

        int x= drones[i].getPosition(0) - x; //x coordinate of the drone
        int y= drones[i].getPosition(1) - y; //y cordinate 

        int distance = pow(x,2) +pow(y,2);

        if (!found || distance < bestDistance || (distance == bestDistance && drones[i].getID() < drones[best].getID()) ){
            best = i;
            bestDistance = distance;
            found = true;
        }

    }
    if (found) {
        return best;
    }
    return -1;
}

bool Fleet::dispatchDrone(int index, int x, int y, double weight)
{
    // TODO: check index and canDeliver, then completeDelivery

    if(!validIndex(index)){
        cout << "invalid index" <<endl;
        return false;
    }

    if(!drones[index].canDeliver(x, y, weight)){
            return false; 
        }
    drones[index].completeDelivery(x,y, weight);
    return true;
}

bool Fleet::rechargeDrone(int id)
{
    // TODO: battery 100 and IDLE, unless MAINTENANCE; message if not found
     for (int i = 0; i < getSize(); i++) {
        if (drones[i].getID() == id) {
            if (drones[i].getStatus() == "MAINTENANCE") {
                cout << "Drone " << id << " is in MAINTENANCE and cannot be recharged." << endl;
                return false;
            }

            drones[i].setBattery(100);
            drones[i].setStatus("IDLE");
            return true;
        }
    }

    cout << "Drone with ID " << id << " not found." << endl;
    return false;
}

void Fleet::countByStatus() const
{
    // TODO: counts of IDLE, CHARGING, MAINTENANCE and the average battery
    int countIdle =0, countMaintenance =0, battery=0, charging= 0;
    for (int i = 0; i < getSize(); i++) {
        if (drones[i].getStatus() == "MAINTENANCE") {
            countMaintenance++;

        }
        else if (drones[i].getStatus() == "IDLE"){
            countIdle++;
        }
        else if (drones[i].getStatus() == "CHARGING"){
            charging++;
        }
        battery+= drones[i].getBattery();
        
    }
    cout << "IDLE: " << countIdle << endl;
    cout << "CHARGING: " << charging << endl;
    cout << "MAINTENANCE: " << countMaintenance << endl;
    cout << "Average battery: " << fixed << setprecision(1) << battery/getSize() << endl;

}

void Fleet::processOrders(const string& orderFile, const string& logFile)
{
    // TODO: for each order in file order, find the nearest available drone and dispatch it.
    // Write one line per order to the screen and the log, then the summary line.
    // Sum the exact distances; round only when printing.
    ifstream input(orderFile);
    ofstream output(logFile);

    if (!input || !output ){
        cerr << "error: can't open the file " << endl;
    }


    int orderID, x, y;
    double weight;
    int total = 0, assigned = 0;
    double totalDistance = 0; 

    while (input >> orderID >> x >> y >> weight) {
        total++;
        
        int idx = findNearestAvailable(x, y, weight);

        if (idx == -1) {
            cout << "Order " << orderID << ": UNASSIGNED" << endl;
            output << "Order " << orderID << ": UNASSIGNED" << endl;
            continue;

        } 
 
        double distance = drones[idx].distanceTo(x, y);
        double batteryBefore = drones[idx].getBattery();
        int droneID = drones[idx].getID();
        string droneName = drones[idx].getName();

        dispatchDrone(idx, x, y, weight);

        assigned++;
        totalDistance += distance;

        cout << "Order " << orderID << ": Drone " << droneID << " (" << droneName
             << ") distance " << distance << " battery " << batteryBefore
             << " -> " << drones[idx].getBattery() << endl;
        output << "Order " << orderID << ": Drone " << droneID << " (" << droneName
             << ") distance " << distance << " battery " << batteryBefore
             << " -> " << drones[idx].getBattery() << endl;
    }

    cout << "Assigned " << assigned << " of " << total
         << " orders. Total distance flown: " << totalDistance << endl;
    output << "Assigned " << assigned << " of " << total
        << " orders. Total distance flown: " << totalDistance << endl;

}

// ---------- Other ----------

void Fleet::shuffleFleet(unsigned long long seed)
{
    // TODO: Fisher-Yates. state = seed; for i from n-1 down to 1:
    //       j = nextRandom(state) % (i + 1), then swap drones i and j.
    unsigned long long state = seed;

    for (int i = getSize() - 1; i > 0; i--) {
        int j = (int)(nextRandom(state) % (i + 1));
        swapDrones(i, j);
    }

}

void Fleet::displayAll() const
{
    // TODO: print every drone with its index
    if (drones.empty()){
        cout<< "NO DRONES" << endl;
    
    }
    for (int i=0; i<getSize(); i++){
        drones[i].displayDrone();
    }
}

void Fleet::displayDrone(int index) const
{
    // TODO: check the range, then print one drone
    if (!validIndex(index)){
        cout << "Error: Invalid index"<<endl;
    }
    drones[index].displayDrone();
}

void Fleet::writeFleetToFile(const string& filename) const
{
    // TODO: same format as drones.txt plus deliveriesCompleted; one decimal for battery and payload

    ofstream outputFile(filename);
    if (!outputFile){
        cerr << "error: file can't be open" <<endl;
    }
    outputFile <<fixed << setprecision(1);
    
    for (int i = 0; i < getSize(); i++) {
        outputFile << drones[i].getName() << " "
            << drones[i].getID() << " "
            << drones[i].getModel() << " "
            << drones[i].getBattery() << " "
            << drones[i].getMaxPayload() << " "
            << drones[i].getPosition(0) << " "
            << drones[i].getPosition(1) << " "
            << drones[i].getStatus() << " "
            << drones[i].getDeliveriesCompleted() << endl;
    }

}
