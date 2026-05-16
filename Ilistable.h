#ifndef ILISTABLE_H
#define ILISTABLE_H

#include <string>
using namespace std;

class IListable {
public:
    virtual void display() const = 0;
    virtual bool isActive() const = 0;
    virtual int getListID() const = 0;
    virtual string getPostDate() const = 0;
    virtual ~IListable() {}
};

#endif