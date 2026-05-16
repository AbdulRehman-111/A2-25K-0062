#ifndef IVEHICLE_H
#define IVEHICLE_H

#include <string>
using namespace std;

class IVehicle {
public:
    virtual void displayDetails() const = 0;
    virtual string getCategory() const = 0;
    virtual double getPrice() const = 0;
    virtual void setPrice(double p) = 0;
    virtual ~IVehicle() {}
};

#endif