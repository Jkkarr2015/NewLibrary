//
// Created by jkarr on 11/22/2017.
//
#pragma once
#include "Geopoint.h"

class POI: public Geopoint{
public:
    long long mDbId;
    std::string mAddress;
    std::string mPhone;
    std::string mWebsite;
    std::string mId;
    std::vector <std::string> mTypes;
    std::string mPrimaryType;
    std::string mProvider;
    int mFloor;
    double mAlarmRadius;
    bool mIsNew;
    bool mFavorite;
    bool mTransitStop;
    long long mModTime;
    bool mFetchedDetails;
    std::string mScope;

    POI();//Default Constructor
    POI(const POI& poi);//Copy Constructor

    //Member Functions
    std::string primaryType();
    std::string getDescription();
    bool isEqualToPOI(POI other);
    bool isEqual(POI other);
    void makeId();
};


