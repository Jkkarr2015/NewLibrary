//
// Created by jkarr on 11/22/2017.
//

#include "Location.h"
#include <regex>
#include "POI.h"
using namespace std;

class OSMPOISearch {
public:
    bool filterUndesirables;
    bool isRunning;
    double radius;
    int limit;
    bool searchResult;
    std::regex rgx;
    



    //Member Functions
    OSMPOISearch();
    bool isLevelinRange(double level, string strRange);
    bool isPOISimilarToPOI(POI a, POI b);
    vector<POI> searchPOIpreproc (Location loc ,string searchString ,double radius, int limit ,double level ,bool sortResults);
    void search(Location loc, string searchString, double radius, int limit, double level, bool sortResults);
    void beaconSearch(Location *loc, double radius, int limit, bool sortResults);

};