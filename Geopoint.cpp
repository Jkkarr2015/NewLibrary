//
// Created by jkarr on 11/22/2017.
//


#import "Geopoint.h"

Geopoint::Geopoint(){}

Geopoint::Geopoint(Geopoint& point){
    mName = point.mName;
    mDistance = point.mDistance;
    mBearing = point.mBearing;
    mLocation = point.mLocation;
}


