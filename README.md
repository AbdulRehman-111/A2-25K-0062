# Car Marketplace System
OOP Assignment 02 - Spring 2026

Name: Abdul Rehman
ID: 25K-0062
Instructor: Mr. Talha Shahid

---

## About

This is assignment 2 for OOP. I extended my assignment 1 car marketplace project by adding inheritance, polymorphism, abstraction, operator overloading and friend functions. The system is based on pakwheels.com

---

## Files in this repo

- CarMarketPlace.cpp : all concrete classes and main function
- Ilistable.h : abstract interface for listings
- Ivehicle.h : abstract interface for vehicles  
- Iuser.h : abstract interface for users
- Isearchable.h : abstract interface for search

---

## How to compile and run

```
g++ -std=c++11 CarMarketPlace.cpp -o carMarketplace
./carMarketplace
```

---

## Commits

- Commit 1 : added header files (abstract interfaces)
- Commit 2 : added Engine, Vehicle, Car, Bike classes
- Commit 3 : added User, Seller, Buyer, Review, Message classes
- Commit 4 : added CarListing, Admin, Marketplace classes
- Commit 5 : added main function and tested
- Commit 6 : final code cleanup

---

## PakWheels Website Screenshots and Code

### 1. Homepage Search — Search by Make, Model, City, Price

This is the main search bar on pakwheels homepage. Users can search by car make, model, city and price range.

<img width="875" height="323" alt="image" src="https://github.com/user-attachments/assets/0c8ab9e5-0899-46fd-bfdc-191d248c12d9" />

I replicated this in the Marketplace class which implements ISearchable. The four pure virtual functions in ISearchable match exactly what pakwheels search does:

```cpp
class ISearchable {
public:
    virtual void searchByBrand(string brand) = 0;
    virtual void searchByModel(string model) = 0;
    virtual void filterByPrice(double maxPrice) const = 0;
    virtual void filterByYear(int minYear) const = 0;
    virtual ~ISearchable() {}
};
```

Marketplace implements all of these:

```cpp
void searchByBrand(string brand) override {
    totalSearches++;
    for (int i = 0; i < listingCount; i++) {
        if (listings[i].isActive() &&
            listings[i].getVehicle()->getBrand() == brand)
            listings[i].display();
    }
}

void filterByPrice(double maxPrice) const override {
    for (int i = 0; i < listingCount; i++) {
        if (listings[i].isActive() &&
            listings[i].getVehicle()->getPrice() <= maxPrice)
            listings[i].display();
    }
}
```

---

### 2. Sell Your Car — Seller Posting a Listing

Pakwheels lets sellers post ads themselves and connect directly with buyers.

<img width="875" height="261" alt="image" src="https://github.com/user-attachments/assets/a186523f-897d-4b58-ba89-82bf24f70bd0" />

I replicated this with the Seller class (inherits User) and postListing() in Marketplace:

```cpp
class Seller : public User {
    float sellerRating;
    int activeListings;
    string baseCity;
    bool accountVerified;

    void addListing() { activeListings++; }
    void adjustRating(float r) { if (r>=0 && r<=5) sellerRating = r; }
    bool operator==(const Seller &other) const {
        return (userID == other.userID && emailAddr == other.emailAddr);
    }
};
```

Usage in main:

```cpp
Seller seller1(1, "Raza Ahmed", "raza@email.com", "03001234567", "Karachi");
seller1.addListing();
CarListing cl1(1, &car1, &seller1);
market.postListing(cl1);
```

---

### 3. Browse by Category (Automatic, 4 Door, Sedan, SUV etc)

Pakwheels lets users filter cars by category like automatic, family cars, 4 door, 5 door etc.

<img width="875" height="327" alt="image" src="https://github.com/user-attachments/assets/e424a50b-aa37-4496-8bcd-f875caedcfd8" />

I replicated this in the Car class which stores body type, door count and transmission:

```cpp
class Car : public Vehicle {
    int doorCount;
    string bodyStyle;
    bool sunroofPresent;
    string transmissionType;
public:
    void displayDetails() const override {
        Vehicle::displayDetails();
        cout << "  Body: " << bodyStyle
             << " | Doors: " << doorCount
             << " | Transmission: " << transmissionType << endl;
    }
    string getCategory() const override { return "Car"; }
};
```

---

### 4. Browse by City (Karachi, Lahore, Islamabad etc)

Pakwheels allows users to filter listings by city across Pakistan.

<img width="875" height="356" alt="image" src="https://github.com/user-attachments/assets/70f0c4bc-d064-4f1f-8eb3-0fc2219b49b4" />

In my system the Seller stores the base city and CarListing displays it alongside the vehicle:

```cpp
class Seller : public User {
    string baseCity;
public:
    string getBaseCity() const { return baseCity; }
};

void CarListing::display() const {
    vehiclePtr->displayDetails();
    cout << "  Seller: " << sellerPtr->getFullName()
         << " | " << sellerPtr->getBaseCity() << endl;
}
```

---

### 5. Browse by Make (Toyota, Honda, Suzuki, KIA etc)

Pakwheels shows all car brands and users can browse listings by manufacturer.

<img width="875" height="341" alt="image" src="https://github.com/user-attachments/assets/b67ff253-2c44-46e8-91e4-885e7b05a799" />

Brand is stored in the Vehicle base class and searchByBrand() in Marketplace handles filtering:

```cpp
class Vehicle : public IVehicle {
protected:
    string brand;
    string modelName;
    const int chassisID;
    ...
};

void searchByBrand(string brand) override {
    for (int i = 0; i < listingCount; i++)
        if (listings[i].getVehicle()->getBrand() == brand)
            listings[i].display();
}
```

---

### 6. Browse by Model (Corolla, Civic, Alto, Prado etc)

Pakwheels shows popular models so users can go directly to the model they want.

<img width="875" height="366" alt="image" src="https://github.com/user-attachments/assets/c8c72493-4df3-4a5d-a555-ac4c46bf92c9" />

Replicated with searchByModel() in Marketplace:

```cpp
void searchByModel(string model) override {
    totalSearches++;
    for (int i = 0; i < listingCount; i++)
        if (listings[i].getVehicle()->getModelName() == model)
            listings[i].display();
}
```

---

### 7. Browse by Budget (Under 5 Lakh, 10 Lakh, 1 Crore etc)

Pakwheels lets users filter cars by price budget ranges.

<img width="875" height="388" alt="image" src="https://github.com/user-attachments/assets/04fabd69-a268-4d46-857a-d4816fb1a4be" />

Replicated with filterByPrice() in Marketplace. askingPrice is stored in Vehicle:

```cpp
void filterByPrice(double maxPrice) const override {
    for (int i = 0; i < listingCount; i++)
        if (listings[i].getVehicle()->getPrice() <= maxPrice)
            listings[i].display();
}
```

---

### 8. Browse by Body Type (Hatchback, Sedan, SUV, Van etc)

Pakwheels lets users filter by body type of the car.

<img width="875" height="353" alt="image" src="https://github.com/user-attachments/assets/59f4533c-e814-442b-8d4f-5eb262cb67ac" />

bodyStyle is a data member in the Car class. Car inherits from Vehicle which inherits from IVehicle:

```cpp
class Car : public Vehicle {
    string bodyStyle;   // Sedan, Hatchback, SUV etc
    ...
    void showInteriorFeatures() const {
        cout << doorCount << "-door " << bodyStyle
             << " with " << transmissionType << " gearbox." << endl;
    }
};
```

---

## OOP Concepts Implemented

Inheritance
Buyer Seller Admin all inherit from User. Car and Bike both inherit from Vehicle. 9 inheritance relationships total.

Polymorphism
displayDetails() is virtual and overridden differently in Car and Bike. showProfile() overridden differently in Buyer Seller Admin. Demonstrated using IVehicle pointer array in main.

Abstraction
4 abstract classes with pure virtual functions each in separate .h file as required by the assignment.

Operator Overloading
== for Vehicle (compares chassisID), + for Car (combines two cars), == for Seller (checks duplicate accounts), = and == for Review.

Friend Functions
printVehicleSummary accesses protected Vehicle members. logMessageExchange accesses private Message members. engineMatch compares private Engine members.

---

## Features

- add update delete vehicle listings
- search by brand model price year mileage
- buyer saves favourite vehicles
- buyer sends message to seller
- admin approves or removes listings
