#ifndef ISEARCHABLE_H
#define ISEARCHABLE_H

#include <string>
using namespace std;

class ISearchable {
public:
    virtual void searchByBrand(string brand) = 0;
    virtual void searchByModel(string model) = 0;
    virtual void filterByPrice(double maxPrice) const = 0;
    virtual void filterByYear(int minYear) const = 0;
    virtual ~ISearchable() {}
};

#endif