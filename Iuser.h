#ifndef IUSER_H
#define IUSER_H

#include <string>
using namespace std;

class IUser {
public:
    virtual void showProfile() const = 0;
    virtual string getRole() const = 0;
    virtual int getID() const = 0;
    virtual ~IUser() {}
};

#endif