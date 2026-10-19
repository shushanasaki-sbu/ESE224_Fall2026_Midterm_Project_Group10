// Drone.cpp
// ESE 224 Fall 2026 Midterm Project
// Team members: Katie Ni

#include "Drone.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// ---------- Constructors ---------- // 

Drone::Drone()
{
    name = "";
    ID = -1;
    model = "";
    battery = 100.0;
    maxPayload = 0.0;
    position[0] = 0; //x
    position[1] = 0; //y
    status = "IDLE";
    deliveriesCompleted = 0;
}

Drone::Drone(const string& n, int id, const string& m, double b, double p,
             int x, int y, const string& s) : Drone()
{
    //sets each member by callings its mutator
    setName(n);
    setID(id);
    setModel(m);
    setBattery(b);
    setMaxPayload(p);
    setPosition(0, x);
    setPosition(1, y);
    setStatus(s);
    deliveriesCompleted = 0;
}

// ---------- Accessors ----------

string Drone::getName() const { return name; }
int Drone::getID() const { return ID; }
string Drone::getModel() const { return model; }
double Drone::getBattery() const { return battery; }
double Drone::getMaxPayload() const { return maxPayload; }
string Drone::getStatus() const { return status; }
int Drone::getDeliveriesCompleted() const { return deliveriesCompleted; }

int Drone::getPosition(int index) const
{
    //only index 0 (x) and index 1 (y) valid
    if (index == 0 || index == 1) {
        return position[index];
    }
    else {
        cerr << "Error: Invalid input.\n";
        return -1;
    }
}

// ---------- Mutators ----------

void Drone::setName(const string& n)
{
    name = n;
}

bool Drone::setID(int id)
{
    if (id > 0) {
        ID = id;
        return true;
    }
    else {
        cerr << "Error: Invalid input.\n";
        return false;
    }
}

bool Drone::setModel(const string& m)
{
    if (m == "Kestrel" || m == "Falcon" || m == "Condor") {
        model = m;
        return true;
    }
    else {
        cerr << "Error: Invalid input.\n";
        return false;
    }
}

bool Drone::setBattery(double b)
{
    if (b >= 0 && b<= 100) {
        battery = b;
        return true;
    }
    else {
        cerr << "Error: Invalid input.\n";
        return false;
    }
}

bool Drone::setMaxPayload(double p)
{
    if (p > 0) {
        maxPayload = p;
        return true;
    }
    else {
        cerr << "Error: Invalid input.\n";
        return false;
    }
}

bool Drone::setPosition(int index, int value)
{
    if ((index == 0 || index == 1) && value >= 0) {
        position[index] = value;
        return true;
    }
    else {
        cerr << "Error: Invalid input.\n";
        return false;
    }
}

bool Drone::setStatus(const string& s)
{
    if (s == "IDLE" || s == "CHARGING" || s == "MAINTENANCE") {
        status = s;
        return true;
    }
    else {
        cerr << "Error: Invalid input.\n";
        return false;
    }
}

// ---------- Delivery methods ---------- 

double Drone::distanceTo(int x, int y) const
{
    //calculate straight-line distance from drone's position to (x, y)
    return sqrt(pow(x - position[0], 2) + pow(y - position[1], 2));
}

double Drone::batteryNeeded(int x, int y, double weight) const
{
    //calculate battery needed for drone to move to (x, y) carrying weight
    return distanceTo(x, y) * BATTERY_PER_UNIT * (1 + PAYLOAD_FACTOR * weight);
}

bool Drone::canDeliver(int x, int y, double weight) const
{
    //drone can deliver order if IDLE, weight <= maxPayload, and the battery after delivery is >= 20
    return (status == "IDLE" && weight <= maxPayload && (battery - batteryNeeded(x, y, weight)) >= SAFETY_RESERVE);
}

void Drone::completeDelivery(int x, int y, double weight)
{
    //after delivery, subtract battery used, move drone to (x, y), and inc deliveriesCompleted
    battery -= batteryNeeded(x, y, weight);
    setPosition(0, x);
    setPosition(1, y);
    deliveriesCompleted++;
    //if battery < 30, set drone status to CHARGING
    if (battery < LOW_BATTERY)
        status = "CHARGING";
}

// ---------- Display and comparison ----------

void Drone::displayDrone() const
{
    cout << "Name: " << name << endl;
    cout << "ID: " << ID << endl;
    cout << "Model: " << model << endl;
    cout << "Battery: " << battery << endl;
    cout << "Max Payload: " << maxPayload << endl;
    cout << "Position: (" << position[0] << ", " << position[1] << ")" << endl;
    cout << "Status: " << status << endl;
    cout << "Deliveries Completed " << deliveriesCompleted << endl;
}

bool Drone::operator==(const Drone& other) const
{
    //two drones are equal if they have the same ID
    return ID == other.getID();
}

