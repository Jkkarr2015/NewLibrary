//
// Created by jkarr on 11/22/2017.
//
//
//THIS IS A BRAND NEW COMMIT 1:44pm!!!!
//THIS IS A NEW COMMENT!!!
//EVEN NEWER COMMIT 1:56pm!!!
#import "Geopoint.h"

Geopoint::Geopoint(){}

Geopoint::Geopoint(Geopoint& point){
    mName = point.mName;
    mDistance = point.mDistance;
    mBearing = point.mBearing;
    mLocation = point.mLocation;
}


