#include <iostream>
#include <string>
#include "IListable.h"
#include "IVehicle.h"
using namespace std;


class Engine {
    string cylinderConfig;
    int powerOutput;
    float cubicCapacity;
    bool hasTurbo;
    string fuelKind;
    const string madeIn;

public:
    Engine() : madeIn("Japan") {
        cylinderConfig = "V4";
        powerOutput = 120;
        cubicCapacity = 1.6f;
        hasTurbo = false;
        fuelKind = "Petrol";
    }

    Engine(string config, int power, float cc, bool turbo, string fuel) : madeIn("Japan") {
        cylinderConfig = config;
        powerOutput = power;
        cubicCapacity = cc;
        hasTurbo = turbo;
        fuelKind = fuel;
    }

    Engine(const Engine &src) : madeIn("Japan") {
        cylinderConfig = src.cylinderConfig;
        powerOutput = src.powerOutput;
        cubicCapacity = src.cubicCapacity;
        hasTurbo = src.hasTurbo;
        fuelKind = src.fuelKind;
    }

    void ignite() { cout << "Engine firing up..." << endl; }
    void shutDown() { cout << "Engine shut down." << endl; }

    void printSpecs() const {
        cout << "Config: " << cylinderConfig
             << " | Power: " << powerOutput << "hp"
             << " | CC: " << cubicCapacity << "L"
             << " | Turbo: " << (hasTurbo ? "Yes" : "No")
             << " | Fuel: " << fuelKind
             << " | Built In: " << madeIn << endl;
    }

    void runDiagnostic() const {
        cout << "Diagnostic: Engine operating at optimal condition." << endl;
    }

    int getPowerOutput() const { return powerOutput; }
    string getCylinderConfig() const { return cylinderConfig; }
    string getFuelKind() const { return fuelKind; }

    friend bool engineMatch(const Engine &a, const Engine &b);
};

bool engineMatch(const Engine &a, const Engine &b) {
    return (a.cylinderConfig == b.cylinderConfig && a.fuelKind == b.fuelKind);
}


class Vehicle : public IVehicle {
protected:
    string brand;
    string modelName;
    int manufactureYear;
    double askingPrice;
    int odometerKm;
    Engine engineUnit;
    const int chassisID;

public:
    Vehicle(int chassis) : chassisID(chassis) {
        brand = "";
        modelName = "";
        manufactureYear = 0;
        askingPrice = 0;
        odometerKm = 0;
    }

    Vehicle(int chassis, string b, string m, int yr, double price, int km)
        : chassisID(chassis) {
        brand = b;
        modelName = m;
        manufactureYear = yr;
        askingPrice = price;
        odometerKm = km;
    }

    Vehicle(int chassis, string b, string m, int yr, double price, int km, Engine eng)
        : chassisID(chassis), engineUnit(eng) {
        brand = b;
        modelName = m;
        manufactureYear = yr;
        askingPrice = price;
        odometerKm = km;
    }

    Vehicle(const Vehicle &src) : chassisID(src.chassisID), engineUnit(src.engineUnit) {
        brand = src.brand;
        modelName = src.modelName;
        manufactureYear = src.manufactureYear;
        askingPrice = src.askingPrice;
        odometerKm = src.odometerKm;
    }

    void setPrice(double p) override { askingPrice = p; }
    void logMileage(int km) { odometerKm += km; }

    void displayDetails() const override {
        cout << manufactureYear << " " << brand << " " << modelName
             << " | Price: Rs." << askingPrice
             << " | KM: " << odometerKm << endl;
        engineUnit.printSpecs();
    }

    void comparePriceTo(double otherPrice) const {
        if (askingPrice < otherPrice)
            cout << brand << " " << modelName << " is cheaper by Rs." << (otherPrice - askingPrice) << endl;
        else
            cout << brand << " " << modelName << " is pricier by Rs." << (askingPrice - otherPrice) << endl;
    }

    string getCategory() const override { return "Vehicle"; }
    string getBrand() const { return brand; }
    string getModelName() const { return modelName; }
    double getPrice() const override { return askingPrice; }
    int getManufactureYear() const { return manufactureYear; }
    int getOdometerKm() const { return odometerKm; }
    int getChassisID() const { return chassisID; }

    bool operator==(const Vehicle &other) const {
        return (chassisID == other.chassisID);
    }

    friend void printVehicleSummary(const Vehicle &v);
};

void printVehicleSummary(const Vehicle &v) {
    cout << "[Summary] " << v.brand << " " << v.modelName
         << " (" << v.manufactureYear << ") - Rs." << v.askingPrice << endl;
}


class Car : public Vehicle {
    int doorCount;
    string bodyStyle;
    bool sunroofPresent;
    string transmissionType;

public:
    Car(int chassis) : Vehicle(chassis) {
        doorCount = 4;
        bodyStyle = "Sedan";
        sunroofPresent = false;
        transmissionType = "Manual";
    }

    Car(int chassis, string brand, string model, int year, double price, int km,
        Engine eng, int doors, string style, bool sunroof, string trans)
        : Vehicle(chassis, brand, model, year, price, km, eng) {
        doorCount = doors;
        bodyStyle = style;
        sunroofPresent = sunroof;
        transmissionType = trans;
    }

    Car(const Car &src) : Vehicle(src) {
        doorCount = src.doorCount;
        bodyStyle = src.bodyStyle;
        sunroofPresent = src.sunroofPresent;
        transmissionType = src.transmissionType;
    }

    void displayDetails() const override {
        Vehicle::displayDetails();
        cout << "  Body: " << bodyStyle << " | Doors: " << doorCount
             << " | Transmission: " << transmissionType
             << " | Sunroof: " << (sunroofPresent ? "Yes" : "No") << endl;
    }

    string getCategory() const override { return "Car"; }

    void showInteriorFeatures() const {
        cout << "Interior: " << doorCount << "-door " << bodyStyle
             << " with " << transmissionType << " gearbox." << endl;
    }

    void setTransmission(string t) { transmissionType = t; }
    int getDoorCount() const { return doorCount; }
    string getBodyStyle() const { return bodyStyle; }
    string getTransmissionType() const { return transmissionType; }

    Car operator+(const Car &other) const {
        Car combined(chassisID + other.chassisID);
        combined.askingPrice = askingPrice + other.askingPrice;
        combined.brand = brand + "+" + other.brand;
        combined.modelName = modelName + "+" + other.modelName;
        combined.manufactureYear = (manufactureYear + other.manufactureYear) / 2;
        return combined;
    }
};


class Bike : public Vehicle {
    string bikeType;
    bool hasCarrier;
    string coolingMethod;
    int seatHeight;

public:
    Bike(int chassis) : Vehicle(chassis) {
        bikeType = "Standard";
        hasCarrier = false;
        coolingMethod = "Air";
        seatHeight = 780;
    }

    Bike(int chassis, string brand, string model, int year, double price,
         int km, Engine eng, string btype, bool carrier, string cooling, int seatH)
        : Vehicle(chassis, brand, model, year, price, km, eng) {
        bikeType = btype;
        hasCarrier = carrier;
        coolingMethod = cooling;
        seatHeight = seatH;
    }

    Bike(const Bike &src) : Vehicle(src) {
        bikeType = src.bikeType;
        hasCarrier = src.hasCarrier;
        coolingMethod = src.coolingMethod;
        seatHeight = src.seatHeight;
    }

    void displayDetails() const override {
        Vehicle::displayDetails();
        cout << "  Type: " << bikeType
             << " | Cooling: " << coolingMethod
             << " | Carrier: " << (hasCarrier ? "Yes" : "No") << endl;
    }

    string getCategory() const override { return "Bike"; }

    void showBikeSpecs() const {
        cout << "Bike Specs -> Type: " << bikeType
             << " | Seat Height: " << seatHeight << "mm"
             << " | Cooling: " << coolingMethod << endl;
    }

    void setCarrier(bool c) { hasCarrier = c; }
    string getBikeType() const { return bikeType; }
    bool getHasCarrier() const { return hasCarrier; }
};