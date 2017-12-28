//
// Created by jkarr on 11/27/2017.
//
//Added this line in
//Added a little more stuff
#pragma once

#include <string>



class Location{
public:
    const static int FORMAT_DEGREES = 0;
    const static int FORMAT_MINUTES = 1;
    const static int FORMAT_SECONDS = 2;
    double longitude;
    double latitude;
    long timeStamp; //in milliseconds since January 1,1970
    float accuracy;
    double altitude;
    float bearing;
    float bearingAccuracyDegrees;
    long elapsedRealtimeNanos;
    std::string provider;
    float speed;
    float speedAccuracyMetersPerSecond;
    float verticalAccuracymeters;

    //Member functions
    Location();
    Location(std::string provider);
    Location(Location& loc); //Copy constructor

    float bearingTo(Location dest);
    void distanceBetween(double startLat, double startLon, double endLat, double endLon, float resultArray[]);
    float distanceTo(Location dest);
    float getAccuracy();
    double getAltitude();
    float getBearing();
    float getBearingAccuracyDegrees();
    long getElapsedRealtimeNanos();
    double getLatitude();
    double getLongitude();
    std::string getProvider();
    float getSpeed();
    float getSpeedAccuracyMetersPerSecond();
    long getTime();
    float getVerticalAcuracyMeters();
    bool hasAccuracy();
    bool hasAltitude();
    bool hasBearing();
    bool hasBearingAccuracyDegrees();
    bool hasSpeed();
    bool hasSpeedAccuracy();
    bool hasVerticalAcuracy();
    bool isFromMockProvider();
    void removeAccuracy();
    void removeAltitude();
    void removeBearing();
    void removeSpeed();
    void reset();
    void set(Location loc);
    void setAccuracy(float horizontalAccuracy);
    void setAltitude(double altitude);
    void setBearing(float bearing);
    void setBearingAccuracyDegrees(float bearingAccuracyDegrees);
    void setElapsedRealtimeNanos(long time);
    void setLatitude(double latitude);
    void setLongitude(double longitude);
    void setProvider(std::string provider);
    void setSpeed(float speed);
    void setSpeedAccuracyMetersPerSecond(float speedAccuracyMeterPerSecond);
    void setTime(long time);
    void setVerticalAcuracyMeters(float verticalAccuracyMeters);
};
