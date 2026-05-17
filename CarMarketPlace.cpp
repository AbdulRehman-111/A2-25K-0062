#include <iostream>
#include <string>
#include "IListable.h"
#include "IVehicle.h"
#include "IUser.h"
#include "ISearchable.h"
using namespace std;

class Engine
{
    string cylinderConfig;
    int powerOutput;
    float cubicCapacity;
    bool hasTurbo;
    string fuelKind;
    const string madeIn;

public:
    Engine() : madeIn("Japan")
    {
        cylinderConfig = "V4";
        powerOutput = 120;
        cubicCapacity = 1.6f;
        hasTurbo = false;
        fuelKind = "Petrol";
    }

    Engine(string config, int power, float cc, bool turbo, string fuel) : madeIn("Japan")
    {
        cylinderConfig = config;
        powerOutput = power;
        cubicCapacity = cc;
        hasTurbo = turbo;
        fuelKind = fuel;
    }

    Engine(const Engine &src) : madeIn("Japan")
    {
        cylinderConfig = src.cylinderConfig;
        powerOutput = src.powerOutput;
        cubicCapacity = src.cubicCapacity;
        hasTurbo = src.hasTurbo;
        fuelKind = src.fuelKind;
    }

    void ignite() { cout << "Engine firing up..." << endl; }
    void shutDown() { cout << "Engine shut down." << endl; }

    void printSpecs() const
    {
        cout << "Config: " << cylinderConfig
             << " | Power: " << powerOutput << "hp"
             << " | CC: " << cubicCapacity << "L"
             << " | Turbo: " << (hasTurbo ? "Yes" : "No")
             << " | Fuel: " << fuelKind
             << " | Built In: " << madeIn << endl;
    }

    void runDiagnostic() const
    {
        cout << "Diagnostic: Engine operating at optimal condition." << endl;
    }

    int getPowerOutput() const { return powerOutput; }
    string getCylinderConfig() const { return cylinderConfig; }
    string getFuelKind() const { return fuelKind; }

    friend bool engineMatch(const Engine &a, const Engine &b);
};

bool engineMatch(const Engine &a, const Engine &b)
{
    return (a.cylinderConfig == b.cylinderConfig && a.fuelKind == b.fuelKind);
}

class Vehicle : public IVehicle
{
protected:
    string brand;
    string modelName;
    int manufactureYear;
    double askingPrice;
    int odometerKm;
    Engine engineUnit;
    const int chassisID;

public:
    Vehicle(int chassis) : chassisID(chassis)
    {
        brand = "";
        modelName = "";
        manufactureYear = 0;
        askingPrice = 0;
        odometerKm = 0;
    }

    Vehicle(int chassis, string b, string m, int yr, double price, int km)
        : chassisID(chassis)
    {
        brand = b;
        modelName = m;
        manufactureYear = yr;
        askingPrice = price;
        odometerKm = km;
    }

    Vehicle(int chassis, string b, string m, int yr, double price, int km, Engine eng)
        : chassisID(chassis), engineUnit(eng)
    {
        brand = b;
        modelName = m;
        manufactureYear = yr;
        askingPrice = price;
        odometerKm = km;
    }

    Vehicle(const Vehicle &src) : chassisID(src.chassisID), engineUnit(src.engineUnit)
    {
        brand = src.brand;
        modelName = src.modelName;
        manufactureYear = src.manufactureYear;
        askingPrice = src.askingPrice;
        odometerKm = src.odometerKm;
    }

    void setPrice(double p) override { askingPrice = p; }
    void logMileage(int km) { odometerKm += km; }

    void displayDetails() const override
    {
        cout << manufactureYear << " " << brand << " " << modelName
             << " | Price: Rs." << askingPrice
             << " | KM: " << odometerKm << endl;
        engineUnit.printSpecs();
    }

    void comparePriceTo(double otherPrice) const
    {
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

    bool operator==(const Vehicle &other) const
    {
        return (chassisID == other.chassisID);
    }

    friend void printVehicleSummary(const Vehicle &v);
};

void printVehicleSummary(const Vehicle &v)
{
    cout << "[Summary] " << v.brand << " " << v.modelName
         << " (" << v.manufactureYear << ") - Rs." << v.askingPrice << endl;
}

class Car : public Vehicle
{
    int doorCount;
    string bodyStyle;
    bool sunroofPresent;
    string transmissionType;

public:
    Car(int chassis) : Vehicle(chassis)
    {
        doorCount = 4;
        bodyStyle = "Sedan";
        sunroofPresent = false;
        transmissionType = "Manual";
    }

    Car(int chassis, string brand, string model, int year, double price, int km,
        Engine eng, int doors, string style, bool sunroof, string trans)
        : Vehicle(chassis, brand, model, year, price, km, eng)
    {
        doorCount = doors;
        bodyStyle = style;
        sunroofPresent = sunroof;
        transmissionType = trans;
    }

    Car(const Car &src) : Vehicle(src)
    {
        doorCount = src.doorCount;
        bodyStyle = src.bodyStyle;
        sunroofPresent = src.sunroofPresent;
        transmissionType = src.transmissionType;
    }

    void displayDetails() const override
    {
        Vehicle::displayDetails();
        cout << "  Body: " << bodyStyle << " | Doors: " << doorCount
             << " | Transmission: " << transmissionType
             << " | Sunroof: " << (sunroofPresent ? "Yes" : "No") << endl;
    }

    string getCategory() const override { return "Car"; }

    void showInteriorFeatures() const
    {
        cout << "Interior: " << doorCount << "-door " << bodyStyle
             << " with " << transmissionType << " gearbox." << endl;
    }

    void setTransmission(string t) { transmissionType = t; }
    int getDoorCount() const { return doorCount; }
    string getBodyStyle() const { return bodyStyle; }
    string getTransmissionType() const { return transmissionType; }

    Car operator+(const Car &other) const
    {
        Car combined(chassisID + other.chassisID);
        combined.askingPrice = askingPrice + other.askingPrice;
        combined.brand = brand + "+" + other.brand;
        combined.modelName = modelName + "+" + other.modelName;
        combined.manufactureYear = (manufactureYear + other.manufactureYear) / 2;
        return combined;
    }
};

class Bike : public Vehicle
{
    string bikeType;
    bool hasCarrier;
    string coolingMethod;
    int seatHeight;

public:
    Bike(int chassis) : Vehicle(chassis)
    {
        bikeType = "Standard";
        hasCarrier = false;
        coolingMethod = "Air";
        seatHeight = 780;
    }

    Bike(int chassis, string brand, string model, int year, double price,
         int km, Engine eng, string btype, bool carrier, string cooling, int seatH)
        : Vehicle(chassis, brand, model, year, price, km, eng)
    {
        bikeType = btype;
        hasCarrier = carrier;
        coolingMethod = cooling;
        seatHeight = seatH;
    }

    Bike(const Bike &src) : Vehicle(src)
    {
        bikeType = src.bikeType;
        hasCarrier = src.hasCarrier;
        coolingMethod = src.coolingMethod;
        seatHeight = src.seatHeight;
    }

    void displayDetails() const override
    {
        Vehicle::displayDetails();
        cout << "  Type: " << bikeType
             << " | Cooling: " << coolingMethod
             << " | Carrier: " << (hasCarrier ? "Yes" : "No") << endl;
    }

    string getCategory() const override { return "Bike"; }

    void showBikeSpecs() const
    {
        cout << "Bike Specs -> Type: " << bikeType
             << " | Seat Height: " << seatHeight << "mm"
             << " | Cooling: " << coolingMethod << endl;
    }

    void setCarrier(bool c) { hasCarrier = c; }
    string getBikeType() const { return bikeType; }
    bool getHasCarrier() const { return hasCarrier; }
};
class User : public IUser
{
protected:
    int userID;
    string fullName;
    string emailAddr;
    string contactNo;
    const string country;
    static int registeredCount;

public:
    User() : country("Pakistan")
    {
        userID = 0;
        fullName = "";
        emailAddr = "";
        contactNo = "";
        registeredCount++;
    }

    User(int id, string name, string email, string phone) : country("Pakistan")
    {
        userID = id;
        fullName = name;
        emailAddr = email;
        contactNo = phone;
        registeredCount++;
    }

    User(const User &src) : country("Pakistan")
    {
        userID = src.userID;
        fullName = src.fullName;
        emailAddr = src.emailAddr;
        contactNo = src.contactNo;
        registeredCount++;
    }

    virtual ~User() {}

    virtual void showProfile() const
    {
        cout << "User [" << userID << "]: " << fullName << " | " << emailAddr << endl;
    }

    void updateEmail(string newEmail) { emailAddr = newEmail; }
    void updateContact(string newPhone) { contactNo = newPhone; }
    virtual string getRole() const { return "User"; }
    static int getRegisteredCount() { return registeredCount; }

    string getFullName() const { return fullName; }
    int getID() const { return userID; }
    string getEmailAddr() const { return emailAddr; }
};

int User::registeredCount = 0;

class Seller : public User
{
    float sellerRating;
    int activeListings;
    string baseCity;
    bool accountVerified;
    int avgResponseMins;

public:
    Seller() : User()
    {
        sellerRating = 0.0f;
        activeListings = 0;
        baseCity = "Unknown";
        accountVerified = false;
        avgResponseMins = 60;
    }

    Seller(int id, string name, string email, string phone, string city) : User(id, name, email, phone)
    {
        sellerRating = 5.0f;
        activeListings = 0;
        baseCity = city;
        accountVerified = true;
        avgResponseMins = 15;
    }

    Seller(const Seller &src) : User(src)
    {
        sellerRating = src.sellerRating;
        activeListings = src.activeListings;
        baseCity = src.baseCity;
        accountVerified = src.accountVerified;
        avgResponseMins = src.avgResponseMins;
    }

    void addListing() { activeListings++; }

    void adjustRating(float r)
    {
        if (r >= 0 && r <= 5)
            sellerRating = r;
    }

    void toggleVerification() { accountVerified = !accountVerified; }

    void showProfile() const override
    {
        cout << "Seller [" << userID << "]: " << fullName
             << " | City: " << baseCity
             << " | Rating: " << sellerRating
             << " | Verified: " << (accountVerified ? "Yes" : "No") << endl;
    }

    string getRole() const override { return "Seller"; }
    string getBaseCity() const { return baseCity; }
    float getSellerRating() const { return sellerRating; }
    bool isAccountVerified() const { return accountVerified; }
    int getActiveListings() const { return activeListings; }

    bool operator==(const Seller &other) const
    {
        return (userID == other.userID && emailAddr == other.emailAddr);
    }
};

class Buyer : public User
{
    Vehicle *savedVehicles[5];
    int savedCount;
    double spendingLimit;
    string locality;
    bool isPremium;

public:
    Buyer() : User()
    {
        savedCount = 0;
        spendingLimit = 0;
        locality = "Unknown";
        isPremium = false;
        for (int i = 0; i < 5; i++)
            savedVehicles[i] = NULL;
    }

    Buyer(int id, string name, string email, string phone) : User(id, name, email, phone)
    {
        savedCount = 0;
        spendingLimit = 2000000;
        locality = "Karachi";
        isPremium = false;
        for (int i = 0; i < 5; i++)
            savedVehicles[i] = NULL;
    }

    Buyer(const Buyer &src) : User(src)
    {
        savedCount = src.savedCount;
        spendingLimit = src.spendingLimit;
        locality = src.locality;
        isPremium = src.isPremium;
        for (int i = 0; i < 5; i++)
            savedVehicles[i] = src.savedVehicles[i];
    }

    void saveVehicle(Vehicle *v)
    {
        if (savedCount < 5)
        {
            savedVehicles[savedCount++] = v;
            cout << "Saved: " << v->getBrand() << " " << v->getModelName() << endl;
        }
        else
        {
            cout << "Saved list is full." << endl;
        }
    }

    void displaySaved() const
    {
        cout << "--- " << fullName << "'s Saved Vehicles ---" << endl;
        for (int i = 0; i < savedCount; i++)
            if (savedVehicles[i])
                savedVehicles[i]->displayDetails();
    }

    void setSpendingLimit(double limit) { spendingLimit = limit; }

    void removeSaved(int index)
    {
        if (index >= 0 && index < savedCount)
        {
            for (int i = index; i < savedCount - 1; i++)
                savedVehicles[i] = savedVehicles[i + 1];
            savedVehicles[--savedCount] = NULL;
            cout << "Removed from saved list." << endl;
        }
    }

    void showProfile() const override
    {
        cout << "Buyer [" << userID << "]: " << fullName
             << " | Budget: Rs." << spendingLimit
             << " | Area: " << locality
             << " | Premium: " << (isPremium ? "Yes" : "No") << endl;
    }

    string getRole() const override { return "Buyer"; }
    double getSpendingLimit() const { return spendingLimit; }
    bool getPremiumStatus() const { return isPremium; }
};

class Review
{
    int reviewID;
    int starRating;
    string reviewBody;
    int authorID;
    string postedOn;
    bool flagged;
    const int maxAllowedRating;

public:
    Review() : maxAllowedRating(5)
    {
        reviewID = 0;
        starRating = 0;
        reviewBody = "";
        authorID = 0;
        postedOn = "";
        flagged = false;
    }

    Review(int id, int aID, string date) : maxAllowedRating(5)
    {
        reviewID = id;
        starRating = 0;
        reviewBody = "";
        authorID = aID;
        postedOn = date;
        flagged = false;
    }

    Review(const Review &src) : maxAllowedRating(5)
    {
        reviewID = src.reviewID;
        starRating = src.starRating;
        reviewBody = src.reviewBody;
        authorID = src.authorID;
        postedOn = src.postedOn;
        flagged = src.flagged;
    }

    void submitReview(int stars, string body)
    {
        if (stars >= 1 && stars <= maxAllowedRating)
        {
            starRating = stars;
            reviewBody = body;
            cout << "Review submitted successfully." << endl;
        }
        else
        {
            cout << "Stars must be between 1 and " << maxAllowedRating << "." << endl;
        }
    }

    void flagReview() { flagged = true; }
    void editBody(string newBody) { reviewBody = newBody; }

    void printReview() const
    {
        cout << starRating << "/" << maxAllowedRating << " - " << reviewBody;
        if (flagged)
            cout << " [FLAGGED]";
        cout << endl;
    }

    int getStarRating() const { return starRating; }
    bool isFlagged() const { return flagged; }

    Review &operator=(const Review &src)
    {
        if (this != &src)
        {
            reviewID = src.reviewID;
            starRating = src.starRating;
            reviewBody = src.reviewBody;
            authorID = src.authorID;
            postedOn = src.postedOn;
            flagged = src.flagged;
        }
        return *this;
    }

    bool operator==(const Review &other) const
    {
        return (reviewID == other.reviewID && authorID == other.authorID);
    }
};

class Message
{
    int msgID;
    int senderID;
    int receiverID;
    string msgBody;
    string sentAt;
    bool readStatus;
    const int maxBodyLength;

public:
    Message() : maxBodyLength(500)
    {
        msgID = 0;
        senderID = 0;
        receiverID = 0;
        msgBody = "";
        sentAt = "";
        readStatus = false;
    }

    Message(int id, int from, int to, string body, string timestamp) : maxBodyLength(500)
    {
        msgID = id;
        senderID = from;
        receiverID = to;
        msgBody = body;
        sentAt = timestamp;
        readStatus = false;
    }

    Message(const Message &src) : maxBodyLength(500)
    {
        msgID = src.msgID;
        senderID = src.senderID;
        receiverID = src.receiverID;
        msgBody = src.msgBody;
        sentAt = src.sentAt;
        readStatus = src.readStatus;
    }

    Message &operator=(const Message &src)
    {
        if (this != &src)
        {
            msgID = src.msgID;
            senderID = src.senderID;
            receiverID = src.receiverID;
            msgBody = src.msgBody;
            sentAt = src.sentAt;
            readStatus = src.readStatus;
        }
        return *this;
    }

    void markRead() { readStatus = true; }

    void printMessage() const
    {
        cout << "[Msg #" << msgID << "] From: " << senderID
             << " To: " << receiverID
             << " | " << sentAt
             << " | " << (readStatus ? "Read" : "Unread")
             << "\n  >> " << msgBody << endl;
    }

    void editBody(string newText)
    {
        if ((int)newText.length() <= maxBodyLength)
            msgBody = newText;
        else
            cout << "Message too long. Max " << maxBodyLength << " characters." << endl;
    }

    bool isRead() const { return readStatus; }
    int getMsgID() const { return msgID; }
    string getMsgBody() const { return msgBody; }
    int getSenderID() const { return senderID; }
    int getReceiverID() const { return receiverID; }

    bool operator==(const Message &other) const
    {
        return (msgID == other.msgID);
    }

    friend void logMessageExchange(const Message &m);
};

void logMessageExchange(const Message &m)
{
    cout << "[LOG] Message #" << m.msgID << " sent from user "
         << m.senderID << " to user " << m.receiverID
         << " at " << m.sentAt << endl;
}
class CarListing : public IListable
{
    int listingID;
    Vehicle *vehiclePtr;
    Seller *sellerPtr;
    bool listingActive;
    string dateListed;
    Review storedReviews[3];
    int reviewCount;
    static int totalCreated;

public:
    CarListing()
    {
        listingID = 0;
        vehiclePtr = NULL;
        sellerPtr = NULL;
        listingActive = false;
        dateListed = "01-01-2026";
        reviewCount = 0;
        totalCreated++;
    }

    CarListing(int id, Vehicle *v, Seller *s)
    {
        listingID = id;
        vehiclePtr = v;
        sellerPtr = s;
        listingActive = true;
        dateListed = "18-05-2026";
        reviewCount = 0;
        totalCreated++;
    }

    CarListing(const CarListing &src)
    {
        listingID = src.listingID;
        vehiclePtr = src.vehiclePtr;
        sellerPtr = src.sellerPtr;
        listingActive = src.listingActive;
        dateListed = src.dateListed;
        reviewCount = src.reviewCount;
        for (int i = 0; i < reviewCount; i++)
            storedReviews[i] = src.storedReviews[i];
        totalCreated++;
    }

    CarListing &operator=(const CarListing &src)
    {
        if (this != &src)
        {
            listingID = src.listingID;
            vehiclePtr = src.vehiclePtr;
            sellerPtr = src.sellerPtr;
            listingActive = src.listingActive;
            dateListed = src.dateListed;
            reviewCount = src.reviewCount;
            for (int i = 0; i < reviewCount; i++)
                storedReviews[i] = src.storedReviews[i];
        }
        return *this;
    }

    void deactivateListing() { listingActive = false; }
    void activateListing() { listingActive = true; }
    void rescheduleDate(string d) { dateListed = d; }

    void attachReview(Review r)
    {
        if (reviewCount < 3)
        {
            storedReviews[reviewCount++] = r;
            cout << "Review attached to listing #" << listingID << endl;
        }
        else
        {
            cout << "Review limit reached for this listing." << endl;
        }
    }

    void printAllReviews() const
    {
        cout << "Reviews for listing #" << listingID << ":" << endl;
        for (int i = 0; i < reviewCount; i++)
            storedReviews[i].printReview();
    }

    void display() const override
    {
        if (vehiclePtr && listingActive)
        {
            cout << "[Listing #" << listingID << " | Posted: " << dateListed << "]" << endl;
            vehiclePtr->displayDetails();
            if (sellerPtr)
                cout << "  Seller: " << sellerPtr->getFullName() << " | " << sellerPtr->getBaseCity() << endl;
            cout << endl;
        }
    }

    bool isActive() const override { return listingActive; }
    Vehicle *const getVehicle() const { return vehiclePtr; }
    int getListID() const override { return listingID; }
    string getPostDate() const override { return dateListed; }
    static int getTotalCreated() { return totalCreated; }
};

int CarListing::totalCreated = 0;

class Admin : public User
{
    int clearanceLevel;
    string department;
    int actionsLogged;
    bool onDuty;
    string workShift;

public:
    Admin() : User()
    {
        clearanceLevel = 1;
        department = "General";
        actionsLogged = 0;
        onDuty = false;
        workShift = "Morning";
    }

    Admin(int id, string name, string email, string phone, string dept) : User(id, name, email, phone)
    {
        clearanceLevel = 3;
        department = dept;
        actionsLogged = 0;
        onDuty = true;
        workShift = "Morning";
    }

    Admin(const Admin &src) : User(src)
    {
        clearanceLevel = src.clearanceLevel;
        department = src.department;
        actionsLogged = src.actionsLogged;
        onDuty = src.onDuty;
        workShift = src.workShift;
    }

    void approveListing(CarListing &cl)
    {
        cl.activateListing();
        actionsLogged++;
        cout << "Admin " << fullName << " approved listing #" << cl.getListID() << endl;
    }

    void removeListing(CarListing &cl)
    {
        cl.deactivateListing();
        actionsLogged++;
        cout << "Admin " << fullName << " removed listing #" << cl.getListID() << endl;
    }

    void switchShift(string s) { workShift = s; }

    void showProfile() const override
    {
        cout << "Admin [" << userID << "]: " << fullName
             << " | Dept: " << department
             << " | Clearance: " << clearanceLevel
             << " | On Duty: " << (onDuty ? "Yes" : "No") << endl;
    }

    string getRole() const override { return "Admin"; }
    int getActionsLogged() const { return actionsLogged; }
    string getWorkShift() const { return workShift; }
    bool isOnDuty() const { return onDuty; }
};

class Marketplace : public ISearchable
{
    CarListing listings[10];
    int listingCount;
    Message inbox[20];
    int msgCount;
    string platformName;
    static int totalSearches;
    static int totalMessages;

public:
    Marketplace()
    {
        listingCount = 0;
        msgCount = 0;
        platformName = "PakWheels Clone";
    }

    Marketplace(string name)
    {
        listingCount = 0;
        msgCount = 0;
        platformName = name;
    }

    void postListing(CarListing cl)
    {
        if (listingCount < 10)
        {
            listings[listingCount++] = cl;
            cout << "Listing #" << cl.getListID() << " posted on " << platformName << endl;
        }
        else
        {
            cout << "Marketplace is full." << endl;
        }
    }

    void deleteListing(int listingID)
    {
        for (int i = 0; i < listingCount; i++)
        {
            if (listings[i].getListID() == listingID)
            {
                listings[i].deactivateListing();
                cout << "Listing #" << listingID << " deactivated." << endl;
                return;
            }
        }
        cout << "Listing not found." << endl;
    }

    void updateListingPrice(int listingID, double newPrice)
    {
        for (int i = 0; i < listingCount; i++)
        {
            if (listings[i].getListID() == listingID && listings[i].isActive())
            {
                listings[i].getVehicle()->setPrice(newPrice);
                cout << "Price updated for listing #" << listingID << endl;
                return;
            }
        }
        cout << "Active listing not found." << endl;
    }

    void displayAllListings() const
    {
        cout << "\n===== " << platformName << " - All Active Listings =====\n";
        for (int i = 0; i < listingCount; i++)
            if (listings[i].isActive())
                listings[i].display();
    }

    void searchByBrand(string brand) override
    {
        totalSearches++;
        cout << "\n--- Search Results for Brand: " << brand << " ---\n";
        bool found = false;
        for (int i = 0; i < listingCount; i++)
        {
            if (listings[i].isActive() && listings[i].getVehicle() != NULL)
            {
                if (listings[i].getVehicle()->getBrand() == brand)
                {
                    listings[i].display();
                    found = true;
                }
            }
        }
        if (!found)
            cout << "No listings found for brand: " << brand << endl;
    }

    void searchByModel(string model) override
    {
        totalSearches++;
        cout << "\n--- Search Results for Model: " << model << " ---\n";
        bool found = false;
        for (int i = 0; i < listingCount; i++)
        {
            if (listings[i].isActive() && listings[i].getVehicle() != NULL)
            {
                if (listings[i].getVehicle()->getModelName() == model)
                {
                    listings[i].display();
                    found = true;
                }
            }
        }
        if (!found)
            cout << "No listings found for model: " << model << endl;
    }

    void filterByPrice(double maxPrice) const override
    {
        cout << "\n--- Listings Under Rs." << maxPrice << " ---\n";
        for (int i = 0; i < listingCount; i++)
        {
            if (listings[i].isActive() && listings[i].getVehicle() != NULL)
            {
                if (listings[i].getVehicle()->getPrice() <= maxPrice)
                    listings[i].display();
            }
        }
    }

    void filterByYear(int minYear) const override
    {
        cout << "\n--- Listings From Year " << minYear << " Onwards ---\n";
        for (int i = 0; i < listingCount; i++)
        {
            if (listings[i].isActive() && listings[i].getVehicle() != NULL)
            {
                if (listings[i].getVehicle()->getManufactureYear() >= minYear)
                    listings[i].display();
            }
        }
    }

    void filterByMileage(int maxKm) const
    {
        cout << "\n--- Listings With Under " << maxKm << " KM ---\n";
        for (int i = 0; i < listingCount; i++)
        {
            if (listings[i].isActive() && listings[i].getVehicle() != NULL)
            {
                if (listings[i].getVehicle()->getOdometerKm() <= maxKm)
                    listings[i].display();
            }
        }
    }

    void sendMessage(Message m)
    {
        if (msgCount < 20)
        {
            inbox[msgCount++] = m;
            totalMessages++;
            logMessageExchange(m);
        }
        else
        {
            cout << "Inbox full." << endl;
        }
    }

    void showInbox(int userID) const
    {
        cout << "\n--- Inbox for User #" << userID << " ---\n";
        for (int i = 0; i < msgCount; i++)
        {
            if (inbox[i].getReceiverID() == userID)
                inbox[i].printMessage();
        }
    }

    static int getTotalSearches() { return totalSearches; }
    static int getTotalMessages() { return totalMessages; }
    string getPlatformName() const { return platformName; }
};

int Marketplace::totalSearches = 0;
int Marketplace::totalMessages = 0;
int main()
{

    Engine eng1("V4", 150, 1.8f, false, "Petrol");
    Engine eng2("V6", 200, 2.4f, true, "Petrol");
    Engine eng3("V2", 80, 0.125f, false, "Petrol");

    Car car1(101, "Toyota", "Corolla", 2020, 3500000, 45000, eng1, 4, "Sedan", false, "Automatic");
    Car car2(102, "Honda", "Civic", 2022, 4800000, 12000, eng2, 4, "Sedan", true, "Automatic");
    Bike bike1(201, "Honda", "CD70", 2023, 185000, 8000, eng3, "Standard", true, "Air", 760);

    Seller seller1(1, "Raza Ahmed", "raza@email.com", "03001234567", "Karachi");
    Seller seller2(2, "Imran Malik", "imran@email.com", "03111234567", "Lahore");
    seller1.addListing();
    seller2.addListing();

    Buyer buyer1(10, "Ali Hassan", "ali@email.com", "03211234567");
    buyer1.setSpendingLimit(5000000);

    Admin admin1(99, "Sara Khan", "sara@admin.com", "03311234567", "Listings");

    CarListing cl1(1, &car1, &seller1);
    CarListing cl2(2, &car2, &seller2);
    CarListing cl3(3, &bike1, &seller1);

    Marketplace market("PakWheels Clone");
    market.postListing(cl1);
    market.postListing(cl2);
    market.postListing(cl3);

    cout << "\n========== USER PROFILES ==========\n";
    seller1.showProfile();
    seller2.showProfile();
    buyer1.showProfile();
    admin1.showProfile();

    cout << "\n========== ALL LISTINGS ==========\n";
    market.displayAllListings();

    cout << "\n========== SEARCH BY BRAND ==========\n";
    market.searchByBrand("Toyota");

    cout << "\n========== SEARCH BY MODEL ==========\n";
    market.searchByModel("Civic");

    cout << "\n========== FILTER BY PRICE ==========\n";
    market.filterByPrice(4000000);

    cout << "\n========== FILTER BY YEAR ==========\n";
    market.filterByYear(2021);

    cout << "\n========== FILTER BY MILEAGE ==========\n";
    market.filterByMileage(20000);

    cout << "\n========== BUYER SAVES VEHICLES ==========\n";
    buyer1.saveVehicle(&car1);
    buyer1.saveVehicle(&bike1);
    buyer1.displaySaved();

    cout << "\n========== MESSAGING ==========\n";
    Message msg1(1, buyer1.getID(), seller1.getID(), "Is this car still available?", "18-05-2026 10:00");
    Message msg2(2, buyer1.getID(), seller2.getID(), "Can you negotiate on the Civic?", "18-05-2026 10:15");
    market.sendMessage(msg1);
    market.sendMessage(msg2);
    market.showInbox(seller1.getID());
    market.showInbox(seller2.getID());

    cout << "\n========== REVIEWS ==========\n";
    Review r1(1, buyer1.getID(), "18-05-2026");
    r1.submitReview(4, "Good condition, fair price.");
    cl1.attachReview(r1);
    cl1.printAllReviews();

    cout << "\n========== ADMIN ACTIONS ==========\n";
    admin1.removeListing(cl2);
    admin1.approveListing(cl2);

    cout << "\n========== OPERATOR OVERLOADING ==========\n";
    if (car1 == car2)
        cout << "Same vehicle." << endl;
    else
        cout << "Different vehicles (== overload works)." << endl;

    Car combined = car1 + car2;
    cout << "Combined listing price: Rs." << combined.getPrice() << endl;

    if (seller1 == seller2)
        cout << "Same seller." << endl;
    else
        cout << "Different sellers (== overload works)." << endl;

    cout << "\n========== POLYMORPHISM ==========\n";
    IVehicle *vList[3];
    vList[0] = &car1;
    vList[1] = &car2;
    vList[2] = &bike1;
    for (int i = 0; i < 3; i++)
    {
        cout << "Category: " << vList[i]->getCategory() << endl;
        vList[i]->displayDetails();
        cout << endl;
    }

    cout << "\n========== FRIEND FUNCTIONS ==========\n";
    printVehicleSummary(car1);
    logMessageExchange(msg1);
    cout << "Engines match: " << (engineMatch(eng1, eng2) ? "Yes" : "No") << endl;

    cout << "\n========== STATIC MEMBERS ==========\n";
    cout << "Total Users Registered: " << User::getRegisteredCount() << endl;
    cout << "Total Listings Created: " << CarListing::getTotalCreated() << endl;
    cout << "Total Searches Performed: " << Marketplace::getTotalSearches() << endl;
    cout << "Total Messages Sent: " << Marketplace::getTotalMessages() << endl;

    cout << "\n========== ENGINE DIAGNOSTIC ==========\n";
    eng1.runDiagnostic();
    eng2.printSpecs();

    return 0;
}
