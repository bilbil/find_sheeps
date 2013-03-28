//
//  beacon.h
//  
//
//  Created by Josh Shelley on 2013-02-15.
//
//  Variables are global within the Beacon class. Suitable for a single threaded environment only.

#include "Arduino.h"

#ifndef ____beacon__
#define ____beacon__

#define frontAnalog 3
#define frontDigital 2
#define backDigital 4
#define rightDigital 7
#define leftDigital 8

class Beacon {
public:
    Beacon();
    void init();
    float getFrontDistance();
    int getFrontBeacon();
    int getBackBeacon();
    int getRightBeacon();
    int getLeftBeacon();
private:
    void checkBeacon();
    float rawAnalog;
    int beacon;
    unsigned long timeLow;
};

#endif /* defined(____beacon__) */
