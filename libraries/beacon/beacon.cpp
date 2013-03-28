//
//  beacon.cpp
//  
//
//  Created by Josh Shelley on 2013-02-15.
//
//  Variables are global within the Beacon class. Suitable for a single threaded environment only.

#include "beacon.h"

Beacon::Beacon() {
}

void Beacon::init() {
    pinMode(frontDigital, INPUT);
    pinMode(backDigital, INPUT);
    pinMode(rightDigital, INPUT);
    pinMode(leftDigital, INPUT);
}

float Beacon::getFrontDistance() {
    rawAnalog = analogRead(frontAnalog);
    return rawAnalog;
}

int Beacon::getFrontBeacon() {
    timeLow = pulseIn(frontDigital, LOW, 580);   // Add Timeout
    checkBeacon();
    return beacon;
}

int Beacon::getBackBeacon() {
    timeLow = pulseIn(backDigital, LOW, 580);   // Add Timeout
    checkBeacon();
    return beacon;
}

int Beacon::getRightBeacon() {
    timeLow = pulseIn(rightDigital, LOW, 580);   // Add Timeout
    checkBeacon();
    return beacon;
}

int Beacon::getLeftBeacon() {
    timeLow = pulseIn(leftDigital, LOW, 580);   // Add Timeout
    checkBeacon();
    return beacon;
}

void Beacon::checkBeacon() {
    if (timeLow > 210 && timeLow < 350) {
        beacon = 1;
    }
    else if (timeLow > 400 && timeLow < 565) {
        beacon = 2;
    }
//    else if (timeLow > 100 && timeLow < 102) {
//        beacon = 3;
//    }
    else {
        beacon = 0;
    }
}