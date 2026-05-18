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

### 1. Homepage Search : Search by Make, Model, City, Price

This is the main search bar on pakwheels homepage. Users can search by car make, model, city and price range.

<img width="875" height="323" alt="image" src="https://github.com/user-attachments/assets/0c8ab9e5-0899-46fd-bfdc-191d248c12d9" />

**Implementation and Reasoning:**

PakWheels allows users to search vehicles using multiple criteria from a single unified interface. To replicate this cleanly in OOP, I used an abstract interface `ISearchable` that defines the search contract without tying it to any specific class. The `Marketplace` class implements this interface, meaning any future class that needs search functionality can also implement `ISearchable` without code duplication.

Each pure virtual function in `ISearchable` corresponds directly to a real filter on the PakWheels website:

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

`Marketplace` implements all four. For example, `searchByBrand()` iterates through all active listings and checks if a vehicle's brand matches. The `totalSearches` static counter tracks how many searches have been performed across all instances:

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

Using an interface here enforces the Open/Closed Principle: the search behavior is defined separately from the data, so adding a new search filter only requires adding to the interface and implementing it, without touching existing code.

---

### 2. Sell Your Car : Seller Posting a Listing

Pakwheels lets sellers post ads themselves and connect directly with buyers.

<img width="875" height="261" alt="image" src="https://github.com/user-attachments/assets/a186523f-897d-4b58-ba89-82bf24f70bd0" />

**Implementation and Reasoning:**

In PakWheels, a seller has a dedicated profile with their city, verification status and rating. This is a natural fit for inheritance: `Seller` extends the base `User` class and adds seller-specific attributes. This avoids duplicating common fields (name, email, phone) that all users share.

The `accountVerified` flag mirrors PakWheels' "Verified Seller" badge, and `sellerRating` tracks trust like the platform's seller score. `addListing()` keeps count of how many active ads the seller has posted.

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

The `==` operator overload prevents duplicate seller accounts: two sellers are considered the same if both their ID and email match. In main, a seller object is created and then passed to a `CarListing` by pointer so the listing knows which seller it belongs to:

```cpp
Seller seller1(1, "Raza Ahmed", "raza@email.com", "03001234567", "Karachi");
seller1.addListing();
CarListing cl1(1, &car1, &seller1);
market.postListing(cl1);
```

Passing by pointer means the listing does not own or copy the seller; it just references them. This reflects the real-world relationship: a seller exists independently of any single listing.

---

### 3. Browse by Category (Automatic, 4 Door, Sedan, SUV etc)

Pakwheels lets users filter cars by category like automatic, family cars, 4 door, 5 door etc.

<img width="875" height="327" alt="image" src="https://github.com/user-attachments/assets/e424a50b-aa37-4496-8bcd-f875caedcfd8" />

**Implementation and Reasoning:**

PakWheels shows category filters like body type, door count, and transmission type prominently. These attributes are specific to cars (not bikes), so I put them inside the `Car` class rather than the `Vehicle` base class. This keeps the base class clean and avoids giving bikes irrelevant fields like `doorCount`.

`Car` inherits from `Vehicle` and overrides `displayDetails()` to add these extra fields. This is polymorphism in action: calling `displayDetails()` on any `IVehicle*` pointer will automatically show car-specific or bike-specific output depending on the actual object type.

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

`Vehicle::displayDetails()` is called first so that the parent class's output (year, brand, price, km, engine) is printed before car-specific details are appended. This reuses the parent's logic without rewriting it.

---

### 4. Browse by City (Karachi, Lahore, Islamabad etc)

Pakwheels allows users to filter listings by city across Pakistan.

<img width="875" height="356" alt="image" src="https://github.com/user-attachments/assets/70f0c4bc-d064-4f1f-8eb3-0fc2219b49b4" />

**Implementation and Reasoning:**

PakWheels prominently shows the city of the seller on every listing. Rather than storing the city in the listing itself, I store it inside the `Seller` object. The listing then accesses it through the seller pointer. This is correct OOP design because the city belongs to the seller, not to the car — the same car could be relisted from a different city by a different seller.

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

This avoids data redundancy. If the seller updates their city, all their listings automatically reflect the new city without needing to update each listing separately.

---

### 5. Browse by Make (Toyota, Honda, Suzuki, KIA etc)

Pakwheels shows all car brands and users can browse listings by manufacturer.

<img width="875" height="341" alt="image" src="https://github.com/user-attachments/assets/b67ff253-2c44-46e8-91e4-885e7b05a799" />

**Implementation and Reasoning:**

`brand` is stored in the `Vehicle` base class because every vehicle — whether a car or a bike — has a brand. Placing it in the base class means both `Car` and `Bike` inherit it automatically without any duplication. The `searchByBrand()` function in `Marketplace` uses the `getBrand()` getter from `Vehicle` to filter listings:

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

Because `getVehicle()` returns an `IVehicle*` in the listing, and `getBrand()` is defined in `Vehicle`, the design depends on `Vehicle` implementing `IVehicle`. The search function works the same for both cars and bikes since brand is a common attribute.

---

### 6. Browse by Model (Corolla, Civic, Alto, Prado etc)

Pakwheels shows popular models so users can go directly to the model they want.

<img width="875" height="366" alt="image" src="https://github.com/user-attachments/assets/c8c72493-4df3-4a5d-a555-ac4c46bf92c9" />

**Implementation and Reasoning:**

`modelName` is also stored in the `Vehicle` base class for the same reason as `brand` — it applies to all vehicle types. `searchByModel()` increments the static `totalSearches` counter (shared across all `Marketplace` instances) and then loops through listings looking for a match:

```cpp
void searchByModel(string model) override {
    totalSearches++;
    for (int i = 0; i < listingCount; i++)
        if (listings[i].getVehicle()->getModelName() == model)
            listings[i].display();
}
```

The `totalSearches` static variable is useful for analytics, similar to how PakWheels tracks the most searched models to display popular listings.

---

### 7. Browse by Budget (Under 5 Lakh, 10 Lakh, 1 Crore etc)

Pakwheels lets users filter cars by price budget ranges.

<img width="875" height="388" alt="image" src="https://github.com/user-attachments/assets/04fabd69-a268-4d46-857a-d4816fb1a4be" />

**Implementation and Reasoning:**

`askingPrice` is stored in the `Vehicle` base class and exposed through the `IVehicle` interface via `getPrice()`. This is important because `filterByPrice()` receives an `IVehicle*` through `getVehicle()`, and it needs to call `getPrice()`. By defining `getPrice()` as a pure virtual in `IVehicle`, any vehicle type can be price-filtered without the marketplace needing to know if it's a Car or Bike:

```cpp
void filterByPrice(double maxPrice) const override {
    for (int i = 0; i < listingCount; i++)
        if (listings[i].getVehicle()->getPrice() <= maxPrice)
            listings[i].display();
}
```

`setPrice()` is also in `IVehicle`, which allows the Admin or the marketplace to update a vehicle's price through the interface pointer without needing a cast.

---

### 8. Browse by Body Type (Hatchback, Sedan, SUV, Van etc)

Pakwheels lets users filter by body type of the car.

<img width="875" height="353" alt="image" src="https://github.com/user-attachments/assets/59f4533c-e814-442b-8d4f-5eb262cb67ac" />

**Implementation and Reasoning:**

`bodyStyle` is a member of `Car` rather than `Vehicle` because bikes do not have a body style in this sense. Keeping it in `Car` is an example of proper class responsibility — the `Car` class is responsible for all car-specific attributes.

The `showInteriorFeatures()` method in `Car` exposes these details for display:

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

This method is not in `Vehicle` or any interface because it is purely car-specific behavior. This is an example of how derived classes can extend functionality beyond what the base class defines, without forcing that functionality on unrelated sibling classes like `Bike`.

---

## OOP Concepts Implemented

### Inheritance

The project uses a two-level inheritance hierarchy throughout:

- `User` (base) → `Buyer`, `Seller`, `Admin` (derived)
- `Vehicle` (base) → `Car`, `Bike` (derived)
- `IUser`, `IVehicle`, `IListable`, `ISearchable` (abstract interfaces) → implemented by concrete classes

`User` holds all shared data (userID, fullName, emailAddr, contactNo, country) and defines `showProfile()` as virtual. Each derived class overrides `showProfile()` to show role-specific information. For example, `Seller::showProfile()` additionally shows city, rating and verification status, while `Buyer::showProfile()` shows budget and locality. Inheritance avoids duplicating these common fields in every subclass.

`Vehicle` similarly holds all shared vehicle data and is inherited by both `Car` and `Bike`. There are 9 inheritance relationships in total across the system.

### Polymorphism

Polymorphism is demonstrated using `IVehicle*` pointers. An array of `IVehicle*` can hold both `Car` and `Bike` objects, and calling `displayDetails()` or `getCategory()` on each pointer automatically dispatches to the correct overridden version at runtime:

```cpp
IVehicle *vList[3];
vList[0] = &car1;   // Car object
vList[1] = &car2;   // Car object
vList[2] = &bike1;  // Bike object
for (int i = 0; i < 3; i++) {
    cout << "Category: " << vList[i]->getCategory() << endl;
    vList[i]->displayDetails();
}
```

When `vList[2]->displayDetails()` is called, C++ uses the vtable to call `Bike::displayDetails()` even though the pointer type is `IVehicle*`. This is runtime polymorphism. Similarly, `showProfile()` is overridden differently in `Buyer`, `Seller`, and `Admin`, so calling it through a `User*` pointer would dispatch correctly to each subclass.

### Abstraction

Four abstract interfaces are defined, each in its own header file as required:

- `IUser.h` — defines the contract for any user (showProfile, getRole, getID)
- `IVehicle.h` — defines the contract for any vehicle (displayDetails, getPrice, setPrice, getCategory)
- `IListable.h` — defines the contract for any marketplace listing (display, isActive, getListID, getPostDate)
- `ISearchable.h` — defines the contract for any searchable entity (searchByBrand, searchByModel, filterByPrice, filterByYear)

None of these interfaces can be instantiated directly — they only define what a class *must* do, not how. This is the essence of abstraction: hiding implementation details and only exposing the necessary interface to the outside world.

### Operator Overloading

Four operators are overloaded across different classes:

- `Vehicle::operator==` — compares two vehicles by their `chassisID` (a const int that uniquely identifies each vehicle). Two vehicles with the same chassis number are the same physical vehicle.
- `Car::operator+` — combines two car listings into one, adding their prices and averaging their years. This was designed to simulate creating a bundle listing.
- `Seller::operator==` — checks for duplicate seller accounts by comparing both `userID` and `emailAddr`. A seller is only a duplicate if both match.
- `Review::operator=` and `Review::operator==` — assignment and equality operators for Review objects, used when copying reviews into listings.

### Friend Functions

Three friend functions are defined to access private or protected members that no getter exposes:

- `printVehicleSummary(const Vehicle &v)` — declared a friend of `Vehicle`. Accesses the protected `brand`, `modelName`, `manufactureYear`, and `askingPrice` directly to print a compact single-line summary. This is used for reporting without exposing these fields publicly.
- `logMessageExchange(const Message &m)` — declared a friend of `Message`. Accesses the private `msgID`, `senderID`, `receiverID`, and `sentAt` fields to log message activity. This is called automatically inside `Marketplace::sendMessage()` every time a message is sent.
- `engineMatch(const Engine &a, const Engine &b)` — declared a friend of `Engine`. Compares private `cylinderConfig` and `fuelKind` of two engines to check if they are compatible. This cannot be done through getters alone if the fields are private.

---

## Features

- add update delete vehicle listings
- search by brand model price year mileage
- buyer saves favourite vehicles
- buyer sends message to seller
- admin approves or removes listings
