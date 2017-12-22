//
// Created by jkarr on 11/22/2017.
//

#include "POI.h"
#include "Location.h"


POI::POI(){
    mIsNew = true;
    mDbId = -1;//flag no database ID

}

POI::POI(const POI& poi):Geopoint(){//Copy Constructor
    mAddress = poi.mAddress;
    mPhone = poi.mPhone;
    mWebsite = poi.mWebsite;
    mId = poi.mId;
    mTypes = poi.mTypes;
    mProvider = poi.mProvider;
    mFloor = poi.mFloor;
    mIsNew = poi.mIsNew;
    mDbId = poi.mDbId;
    mFavorite = poi.mFavorite;
    mTransitStop = poi.mTransitStop;
    mAlarmRadius = poi.mAlarmRadius;
    mModTime = poi.mModTime;
    mFetchedDetails = poi.mFetchedDetails;
    mScope = poi.mScope;
    mPrimaryType = poi.mPrimaryType;
}

 std::string POI:: primaryType() {
    std::string res = NULL;
    if (mTypes.size() != 0) {
        if (mTypes.size() > 1) {
            //res = findDesirableType(poi);
            if (!res.empty()) {
                //find most desirable by passing over undesirables
                unsigned int x;
                //assume lower position types are more preferred
                for (x = 0; x < mTypes.size(); ++x) {
                    //if (![UNDESIRABLE_POI_TYPES containsObject:self.types[x]])//Don't know how to convert this
                        break; //this one not undesirable, use it

                }

                if (x == mTypes.size())
                    x = 0; //no undesirables, use first type

                res = mTypes[x];
            }
        }
        else //just one
            res = mTypes[0];

        //if ([REMOVE_POI_TYPES containsObject:res])//Don't know how to convert
        //res = NULL;

    }

    return res;
}

std::string POI::getDescription() {
    std::string str;
    if (!mName.empty())
        str += mName;

    std::string primaryType = mPrimaryType; //save calculating twice
    if (primaryType.empty())
 //   [str appendFormat:@", %@", primaryType];//Don't know how to convert this line

    //if (this.location)
    //[str appendFormat:@"\n%@", [Funcs relativeDescriptionWithDistance:self.distance direction:self.bearing]];

    //if (poi.address && !poi.address.isEqualToString(this->name)) {
        std::string address = mAddress;
        //if ([Funcs getHub].expandAbbreviations)//Don't know how to convert this
        //address = [Funcs subst:self.address usingDict:EXC_DICT_STREET];
        //[str appendFormat:@"\n%@", address];
    //}

    return str;
}

bool POI::isEqualToPOI(POI other) {
    //compares ID only
    bool res = false;
   // if (other && (other.mId.compare(mId) == 0))//Need to ask about this one
        res = true;

    return res;
}

bool POI::isEqual(POI other) {
    return isEqualToPOI(other);
}

void POI::makeId() {
        //only use when provider does not offer a unique ID

        std::string str;

        if (!mName.empty())
        str += mName;

        std::string type = mPrimaryType;
        if (!type.empty())
        str += type;

       // if (this -> location) {
            //std::string strLocation = [NSString stringWithFormat:@"%@%@", @(self.location.coordinate.latitude), @(self.location.coordinate.longitude)];
            //str += strLocation;
        //}

        //mId = [NSString stringWithFormat:@"%llu", (long long) str.hash];
}