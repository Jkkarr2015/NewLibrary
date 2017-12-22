//
// Created by jkarr on 11/22/2017.
//
#pragma once
#include <string>
#include <vector>
#include "Location.h"
class Geopoint {
public:
    std::string mName;
    double mDistance;
    double mBearing;
    Location mLocation;

    //member functions
    Geopoint();//Default Constructor
    Geopoint(Geopoint& point);//Copy Constructor
};
