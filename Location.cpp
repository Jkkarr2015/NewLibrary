//
// Created by jkarr on 12/8/2017.
//
#include "Location.h"
#include <math.h>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;
// Default Constructor
Location::Location(){
    timeStamp = 0;
    longitude = 0;
    latitude = 0;
    provider = "";
}

//Constructor
Location::Location(string provider)
{
        timeStamp = 0;
        latitude = 0;
        longitude = 0;
        this -> provider = provider;
}

//Copy Constructor
Location::Location(Location& loc)
{
    longitude = loc.longitude;
    latitude = loc.latitude;
    timeStamp = loc.timeStamp;
    accuracy = loc.accuracy;
    altitude = loc.altitude;
    bearing = loc.bearing;
    bearingAccuracyDegrees = loc.bearingAccuracyDegrees;
    elapsedRealtimeNanos = loc.elapsedRealtimeNanos;
    provider = loc.provider;
    speed = loc.speed;
    speedAccuracyMetersPerSecond = loc.speedAccuracyMetersPerSecond;
    verticalAccuracymeters = loc.verticalAccuracymeters;
}


float Location::bearingTo(Location dest)
//Formula for Initial Bearing from current location to dest
{
    float lat1 = (latitude * M_PI)/180; //converts degrees to radians
    float lat2 = (dest.latitude * M_PI)/180;
    float lon1 = (longitude * M_PI)/180;
    float lon2 = (dest.longitude * M_PI)/180;

    float y = sin(lon2 - lon1) * cos(lat2);
    float x = cos(lat1) * sin(lat2) -
              sin(lat1) * cos(lat2) * cos(lon2 - lon1);
    float bring = (atan2(y, x) * 180)/M_PI;//initial bearing
    bring = fmod((bring + 360),360);

    return bring;
}




void Location::distanceBetween(double startLat, double startLon, double endLat, double endLon,
                               float resultArray[3])
//Using haversine formula basis on Spherical earth model which can give errors of up to 0.3%.
// resultsArray is sturctured like this 0:distance, 1: intial bearing, 2: ending bearing
{
    //Distance
    int R = 6371e3;//radius of earth in meters
    float lat1 = (startLat * M_PI)/180; //converts degrees to radians
    float lat2 = (endLat * M_PI)/180;
    float deltaLat = ((startLat - endLat) * M_PI)/180;
    float deltaLon = ((startLon - endLon) * M_PI)/180;

    float a = sin(deltaLat/2) * sin(deltaLat/2) +
              cos(lat1) * cos(lat2) * sin(deltaLon/2) * sin(deltaLon/2);
    float c = 2 * atan2(sqrt(a) , sqrt(1-a));

    float d = R * c;//distance in meters

    resultArray[0] = d;

    //Initial Bearing
    float lon1 = (startLon * M_PI)/180;
    float lon2 = (endLon * M_PI)/180;

    float y = sin(lon2 - lon1) * cos(lat2);
    float x = cos(lat1) * sin(lat2) -
              sin(lat1) * cos(lat2) * cos(lon2 - lon1);
    float bring = (atan2(y, x) * 180)/M_PI;//initial bearing
    bring = fmod((bring + 360),360);


    resultArray[1] = bring;

    //Final Bearing
     y = sin(lon1 - lon2) * cos(lat1);
     x = cos(lat2) * sin(lat1) -
        sin(lat2) * cos(lat1) * cos(lon1 - lon2);
     float fbring = (atan2(y,x) *180 )/M_PI;
     fbring = fmod((fbring + 180),360);

    resultArray[2] = fbring;





}

float Location::distanceTo(Location dest)
{
    int R = 6371e3;//radius of earth in meters
    float lat1 = (latitude * M_PI)/180; //converts degrees to radians
    float lat2 = (dest.latitude * M_PI)/180;
    float deltaLat = ((latitude - dest.latitude) * M_PI)/180;
    float deltaLon = ((longitude - dest.longitude) * M_PI)/180;

    float a = sin(deltaLat/2) * sin(deltaLat/2) +
              cos(lat1) * cos(lat2) * sin(deltaLon/2) * sin(deltaLon/2);
    float c = 2 * atan2(sqrt(a) , sqrt(1-a));

    float d = R * c;//distance in meters
   return d;
}

//Getters
float Location::getAccuracy()
{
    return accuracy;
}

double Location::getAltitude()
{
    return altitude;
}

float Location::getBearing()
{
    return bearing;
}

float Location::getBearingAccuracyDegrees()
{
    return bearingAccuracyDegrees;
}

long Location::getElapsedRealtimeNanos()
{
    return elapsedRealtimeNanos;
}

double Location::getLatitude()
{
    return latitude;
}

double Location::getLongitude()
{
    return longitude;
}

string Location::getProvider()
{
    return provider;
}

float Location::getSpeed()
{
    return speed;
}

float Location::getSpeedAccuracyMetersPerSecond()
{
    return speedAccuracyMetersPerSecond;
}

long Location::getTime()
{
    return timeStamp;
}

float Location::getVerticalAcuracyMeters()
{
    return verticalAccuracymeters;
}




bool Location::hasAccuracy()
{
    if(accuracy != 0)
        return true;
    else
        return false;

}

bool Location::hasAltitude()
{
    if(altitude != 0)
        return true;
    else
        return false;
}

bool Location::hasBearing()
{
    if(bearing != 0)
        return true;
    else
        return false;
}

bool Location::hasBearingAccuracyDegrees()
{
    if(bearingAccuracyDegrees != 0)
        return true;
    else
        return false;
}

bool Location::hasSpeed()
{
    if(speed != 0)
        return true;
    else
        return false;
}

bool Location::hasSpeedAccuracy()
{
    if(speedAccuracyMetersPerSecond != 0)
        return true;
    else
        return false;
}

bool Location::hasVerticalAcuracy()
{
    if(verticalAccuracymeters != 0)
        return true;
    else
        return false;
}


bool Location::isFromMockProvider()
{
    return false; //Temporary until we find out if needed
}

void Location::removeAccuracy()
{
    accuracy = 0.0;
}

void Location::removeAltitude()
{
    altitude = 0.0;
}

void Location::removeBearing()
{
    bearing = 0.0;
}

void Location::removeSpeed()
{
    speed = 0.0;
}

void Location::reset()
{
    longitude = 0.0;
    latitude = 0.0;
    timeStamp = 0;
    accuracy = 0.0;
    altitude = 0.0;
    bearing = 0.0;
    bearingAccuracyDegrees = 0.0;
    elapsedRealtimeNanos = 0;
    provider = "";
    speed = 0.0;
    speedAccuracyMetersPerSecond = 0.0;
    verticalAccuracymeters = 0.0;
}

//Setters
void Location::set(Location loc)
{
    longitude = loc.longitude;
    latitude = loc.latitude;
    timeStamp = loc.timeStamp;
    accuracy = loc.accuracy;
    altitude = loc.altitude;
    bearing = loc.bearing;
    bearingAccuracyDegrees = loc.bearingAccuracyDegrees;
    elapsedRealtimeNanos = loc.elapsedRealtimeNanos;
    provider = loc.provider;
    speed = loc.speed;
    speedAccuracyMetersPerSecond = loc.speedAccuracyMetersPerSecond;
    verticalAccuracymeters = loc.verticalAccuracymeters;
}

void Location::setAccuracy(float horizontalAccuracy)
{
    accuracy = horizontalAccuracy;
}

void Location::setAltitude(double altitude)
{
    this->altitude = altitude;
}

void Location::setBearing(float bearing)
{
    this->bearing = bearing;
}

void Location::setBearingAccuracyDegrees(float bearingAccuracyDegrees)
{
    this->bearingAccuracyDegrees = bearingAccuracyDegrees;
}

void Location::setElapsedRealtimeNanos(long time)
{
    elapsedRealtimeNanos = time;
}

void Location::setLatitude(double latitude)
{
    this->latitude = latitude;
}

void Location::setLongitude(double longitude)
{
    this->longitude = longitude;
}

void Location::setProvider(string provider)
{
    this->provider = provider;
}

void Location::setSpeed(float speed)
{
    this->speed = speed;
}

void Location::setSpeedAccuracyMetersPerSecond(float speedAccuracyMeterPerSecond)
{
    this->speedAccuracyMetersPerSecond = speedAccuracyMeterPerSecond;
}

void Location::setTime(long time)
{
    timeStamp = time;
}

void Location::setVerticalAcuracyMeters(float verticalAccuracyMeters)
{
    this->verticalAccuracymeters = verticalAccuracyMeters;
}

