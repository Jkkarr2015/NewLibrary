//
// Created by jkarr on 11/27/2017.
//

#include "POI.h"
#include <algorithm>
#include "OSMPOISearch.h"
#include <math.h>
#include "Location.h"
#include "OSMPBFread.h"
#include "BeaconSearchResult.h"
#include <vector>
#include <regex>
#include <sstream>
#include <android/log.h>

using namespace std;

 OSMPOISearch::OSMPOISearch() {
     rgx = "([0-9.])-";
 }
//bool POITextDisplayCompare(POI a, POI b)
//{
//    if(a.mName.compare(b.mName) == 0){
//        if(a.mAddress.compare(b.mAddress) == 0)
//            return true;
//    }
//    return false;
//}

bool OSMPOISearch::isPOISimilarToPOI(POI a, POI b) {
    bool bRes = false;
    if (a.isEqualToPOI(b))
        bRes = true;
    else if (!a.mName.empty() && !b.mName.empty() && a.mName.compare(b.mName) == 0) {
        if (a.mAddress.empty() && b.mAddress.empty())
            bRes = true; //no address and names match, similar
        else if (!a.mAddress.empty() && !b.mAddress.empty() && a.mAddress.compare(b.mAddress) == 0)
            bRes = true; //Address and name matches

    }

return bRes;
}

double GetDistanceMilesHaversineApprox(::google::protobuf::int64 ilat1, ::google::protobuf::int64 ilon1, ::google::protobuf::int64 ilat2, ::google::protobuf::int64 ilon2)
{

    double flat1 = ilat1;
    double flat2 = ilat2;

    double flon1 = ilon1;
    double flon2 = ilon2;

    flat1 = flat1/10000000.0;
    flat2 = flat2/10000000.0;
    flon1 = flon1/10000000.0;
    flon2 = flon2/10000000.0;


    double ilatd = flat1 - flat2;
    double ilond = flon1 - flon2;


    double flatMiles = ilatd/0.0144984587616;

    double fLatRad = flat1/180.0*M_PI;

    double fCos = cos(fLatRad);
    double flonMiles = fCos*ilond/(0.0144984587616);

    double fDist = sqrt(flatMiles*flatMiles+flonMiles*flonMiles);

    return fDist;
}

bool OSMPOISearch::isLevelinRange(double level, string strRange) {
    int iLevel = floor(level);
    bool bFound = false;

    string modRange = strRange;
    //Problem: range delimiter is "-", while negative sign is also "-". Convert "[0-9]-" to "[0-9]_", making delimiter "_"
    //[regex replaceMatchesInString:modRange options:0
    //range:NSMakeRange(0, modRange.length) withTemplate:@"$1_"];

    //std::regex_replace(modRange, 0, modRange.length(), rgx, "$1_");
    char delimitter = ';';//";" used for single values i.e., "3;5;6"
    size_t index, previous =0;
    index = modRange.find(delimitter);
    vector<string> ranges;
    while(index != std::string::npos){
        ranges.push_back(modRange.substr(previous, index - previous));
        previous = index + 1;
        index = modRange.find(delimitter);
    }//end while
    ranges.push_back(modRange.substr(previous, index - previous));


    for (string range : ranges) {//for-each range in ranges
        if (range.find("_") == std::string::npos) {
        //not a true range -- just a single value
            if (std::atoi(range.c_str()) == iLevel) {
                bFound = true;
                break;
            }
        } //single value specification
        else {
        //true range -- get the bounds
            delimitter = '_';
            index = modRange.find(delimitter);
            vector<string> bounds;
            while(index != std::string::npos){
                bounds.push_back(modRange.substr(previous, index - previous));
                previous = index + 1;
                index = modRange.find(delimitter);
            }//end while
            bounds.push_back(modRange.substr(previous, index - previous));
            if (bounds.size() == 2) {
                int iLowerBound = std::atoi(bounds[0].c_str());
                int iUpperBound = std::atoi(bounds[1].c_str());
                if ((iLevel >= iLowerBound) && (iLevel <= iUpperBound)) {
                    bFound = true;
                    break;
                }
            } //if got 2 bounding values
        } //else true range
    } //for range

    return bFound;
}


//this one searches a pre processed list of POIs, more efficient
vector<POI> OSMPOISearch::searchPOIpreproc (Location loc ,string searchString ,double radius, int limit ,double level ,bool sortResults) {
    vector <POI> pois; //add results to this vector

    double aclldlat = loc.latitude;
    double aclldlon = loc.longitude;

    ::google::protobuf::int64 ig64lat = aclldlat* 10000000.0;
    ::google::protobuf::int64 ig64lon = aclldlon* 10000000.0;



    OSMPBFread aopr;
    vector <OSMnode> vOSMnode;


    aopr.open("path goes here", aclldlat, aclldlon, radius, &vOSMnode);


    if(!searchString.empty()) {
        std::string sSearch = searchString;
        std::transform(sSearch.begin(), sSearch.end(), sSearch.begin(), ::tolower);

        vector <OSMnode> vOSMnodeTemp;

        for(int i=0;i<vOSMnode.size();i++){
            string sNNL = vOSMnode.at(i).GetName();
            std::transform(sNNL.begin(), sNNL.end(), sNNL.begin(), ::tolower);

            if(sNNL.find(sSearch)!= std::string::npos) {
                OSMnode temp = vOSMnode.at(i);
                vOSMnodeTemp.push_back(temp);

            }

        }

        vOSMnode = vOSMnodeTemp;
    }


    for(int i=0;i<vOSMnode.size();i++){
        vOSMnode.at(i).fAux =  GetDistanceMilesHaversineApprox(vOSMnode.at(i).lat ,vOSMnode.at(i).lon ,ig64lat ,ig64lon);
    }


    //sort by distance
    struct sort_by_distance {
        bool operator()(const OSMnode &left, OSMnode &right) {
        return left.fAux < right.fAux;
        }
    };
    sort(vOSMnode.begin(),vOSMnode.end(),sort_by_distance());

    for(int i=0;i<vOSMnode.size();i++){
        if (pois.size() == limit)
        break; //done

        POI poi;

        //floor (level)
        if(vOSMnode.at(i).HasTag("level")){
            int iLevel = std::atoi(vOSMnode.at(i).GetTag("level").c_str());
            poi.mFloor = iLevel;
        }

        string levelRepeatRange = "";
        if(vOSMnode.at(i).HasTag("repeat_on"))
            levelRepeatRange = vOSMnode.at(i).GetTag("repeat_on").c_str();

        if (level && (!poi.mFloor || !level == poi.mFloor)) {
            if (!levelRepeatRange.empty())
                continue; //if caller specified level, it must match

            if (!isLevelinRange(level, levelRepeatRange))
                continue;

        }
        else if (!level && poi.mFloor)
            continue; //skip POIs with a level if no level specified

        string address;
        if(vOSMnode.at(i).HasTag("addr:housenumber")){
            address.append(vOSMnode.at(i).GetTag("addr:housenumber").c_str());
        }

        if(vOSMnode.at(i).HasTag("addr:street")){
            address.append(" ");
            address.append(vOSMnode.at(i).GetTag("addr:street").c_str());
        }

        if(vOSMnode.at(i).HasTag("addr:city")){
            address.append(", ");
            address.append(vOSMnode.at(i).GetTag("addr:city").c_str());
        }

        if(vOSMnode.at(i).HasTag("addr:state")){
            address.append(", ");
            address.append(vOSMnode.at(i).GetTag("addr:state").c_str());
        }

        poi.mAddress = address;


        if(vOSMnode.at(i).HasTag("website")){
            poi.mWebsite= vOSMnode.at(i).GetTag("website").c_str();
        }

        if(vOSMnode.at(i).HasTag("phone")){
            poi.mPhone = vOSMnode.at(i).GetTag("phone").c_str();
        }

        //filter out rooms
        if(vOSMnode.at(i).bIsIndoorRoom())
            continue;

        //filter out indoor areas
        if(vOSMnode.at(i).bIsIndoorArea())
            continue;

        double dLat=vOSMnode.at(i).lat;
        dLat=dLat/10000000;
        double dLon=vOSMnode.at(i).lon;
        dLon=dLon/10000000;

        string anss = vOSMnode.at(i).GetTag("name").c_str();
        poi.mName = anss;

        poi.mLocation.latitude = dLat;
        poi.mLocation.longitude = dLon;
        poi.mBearing = loc.bearingTo(poi.mLocation);
        poi.mDistance = loc.distanceTo(poi.mLocation);

        poi.mId = vOSMnode.at(i).id ;
        if(poi.mId.size() == 0)
            poi.makeId();

        //see if already have this POI (or one very much like it)
        bool bUnique = true;
        vector<POI> tempPois = pois;
        std::reverse(tempPois.begin(), tempPois.end());
        //do it in reverse, assuming that duplicates are more likely to be closer than further
        for (POI tempPoi : tempPois ) {
            if (isPOISimilarToPOI(poi, tempPoi )) {
                bUnique = false;
                break;
            }
        } //for tempPoi

        if (bUnique)
            pois.push_back(poi);

    }

//results are already sorted, regardless of caller's preference

    return pois;
}//end searchPOIpreproc



void OSMPOISearch::search(Location loc, string searchString, double radius, int limit, double level,
                          bool sortResults)
{

double dmiles = 2.0 * radius / 1760.0;
vector<POI> pois = searchPOIpreproc(loc, searchString, dmiles, limit, level, sortResults);

}

void OSMPOISearch::beaconSearch(Location *loc, double radius, int limit, bool sortResults)
{
    //Radius 150 meters
    //Limit: what ever I want
    std::vector<Beacon> beaconResults;
    double aclldlat = loc->latitude;
    double aclldlon = loc->longitude;

    ::google::protobuf::int64 ig64lat = ((::google::protobuf::int64) (aclldlat* 10000000.0));
    ::google::protobuf::int64 ig64lon = ((::google::protobuf::int64) (aclldlon* 10000000.0));

    OSMPBFread aopr;

    vector <OSMnode> vOSMnode;

    double dmiles = 2.0 * radius / 1760.0;

    const char * fileName = "/storage/emulated/0/Android/data/com.example.jkarr.hello_cmake/files/kentucky-latest_Beacon.osm.pbf";

    sortResults = aopr.open(fileName, aclldlat,aclldlon, dmiles, &vOSMnode);

    for(int i = 0; i < vOSMnode.size();i++)
        vOSMnode.at(i).fAux = GetDistanceMilesHaversineApprox(vOSMnode.at(i).lat, vOSMnode.at(i).lon, ig64lat, ig64lon);

    //sort by distance
    struct sort_by_distance {
        bool operator()(const OSMnode &left, OSMnode &right) {
            return left.fAux < right.fAux;
        }
    };

    sort(vOSMnode.begin(),vOSMnode.end(),sort_by_distance());

    for(int i=0;i<vOSMnode.size();i++){

        if (beaconResults.size() == limit)
            break; //done

        if(!vOSMnode.at(i).IsBeacon())
            continue;
        Beacon aBeacon;

        double dlon = vOSMnode.at(i).lon;
        double dlat = vOSMnode.at(i).lat;
        dlon/=10000000.0;
        dlat/=10000000.0;
        aBeacon.lon = dlon;
        aBeacon.lat = dlat;

        aBeacon.level = atoi(vOSMnode.at(i).GetTag("level").c_str());
        aBeacon.major = atoi(vOSMnode.at(i).GetTag("beacon:major").c_str());
        aBeacon.minor = atoi(vOSMnode.at(i).GetTag("beacon:minor").c_str());
        aBeacon.name = atoi(vOSMnode.at(i).GetTag("name").c_str());
        aBeacon.type = atoi(vOSMnode.at(i).GetTag("beacon:type").c_str());
        aBeacon.UUID = atoi(vOSMnode.at(i).GetTag("beacon:uuid").c_str());

       // __android_log_print(ANDROID_LOG_DEBUG, "JDK", "Level: %s",vOSMnode.at(i).GetTag("level").c_str());
       // __android_log_print(ANDROID_LOG_DEBUG, "JDK", "Beacon-Major: %s",vOSMnode.at(i).GetTag("beacon:major").c_str() );
      //  __android_log_print(ANDROID_LOG_DEBUG, "JDK", "Beacon-Minor: %s ",vOSMnode.at(i).GetTag("beacon:minor").c_str() );
       // __android_log_print(ANDROID_LOG_DEBUG, "JDK", "Name: %s",vOSMnode.at(i).GetTag("name").c_str() );
       // __android_log_print(ANDROID_LOG_DEBUG, "JDK", "Type: %s",vOSMnode.at(i).GetTag("beacon:type").c_str() );
       // __android_log_print(ANDROID_LOG_DEBUG, "JDK", "UUID: %s",vOSMnode.at(i).GetTag("beacon:uuid").c_str() );
        beaconResults.push_back(aBeacon);


    }//end beaconSearch

}