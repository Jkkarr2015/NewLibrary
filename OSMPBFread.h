//
// Created by jkarr on 12/11/2017.
//

#ifndef HELLOCMAKE_OSMPBFREAD_H
#define HELLOCMAKE_OSMPBFREAD_H
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "osmformat.pb.h"
#include "fileformat.pb.h"
#include <zlib.h>
#include <set>
#import <math.h>
#include "osmpbf.h"

using namespace std;

//a low memory dense way
struct sWayC{
    vector<::google::protobuf::int64> ipbNodeRefs;
    //vector<pair<string,string>> tags;
    vector<pair<int,int>> tags;
    ::google::protobuf::int64 id;
};

struct OSMnode{
    ::google::protobuf::int64 lat;
    ::google::protobuf::int64 lon;
    ::google::protobuf::int64 id;
    vector<pair<string,string>> tags;

    float fAux; //temp data used for sorting, etc
    string sAux;

    bool IsIndoorPOI(){

        return HasTag("level");

    }

    bool bIsIndoorRoom(){
        if(!IsIndoorPOI())
            return false;

        string sIndoorRoom=GetTag("indoor");

        if(sIndoorRoom.compare("room")==0)
            return true;

        return false;

    }

    bool bIsIndoorArea(){
        if(!IsIndoorPOI())
            return false;

        string sIndoorRoom=GetTag("area");

        if(sIndoorRoom.compare("yes")==0)
            return true;

        return false;

    }

    bool IsBeacon(){
        return HasTag("beacon:major");
    }

    bool bIsOutdoorPOI(){
        bool bPointOfInterest = false;

        bPointOfInterest= HasTag("name");

        if(HasTag("highway")){
            bPointOfInterest=false;
        }

        return bPointOfInterest;
    }


    string AllTags(){
        string sAT = "";
        for(int i =0;i<tags.size();i++){
            sAT+=tags.at(i).first + " " + tags.at(i).second + " ";
        }
        return sAT;
    }

    string GetName(){
        string sRetVal="";

        sRetVal = this->GetTag("name");

        return sRetVal;
    }
    string GetAddress(){
        string sRetVal="";

        sRetVal = this->GetTag("address");

        return sRetVal;
    }
    string GetTag(string sTagName){
        string sRetVal="";
        for(int i=0;i<tags.size();i++)
        {
            if(tags.at(i).first.compare(sTagName)==0){
                sRetVal=tags.at(i).second;
                return sRetVal;
            }
        }

        return sRetVal;
    }

    bool HasTag(string sTagName){
        bool bRetVal=false;

        for(int i=0;i<tags.size();i++)
        {
            if(tags.at(i).first.compare(sTagName)==0){
                bRetVal=true;
                return bRetVal;
            }
        }

        return bRetVal;

    }


};

struct rect
{
    ::google::protobuf::int64 x;
    ::google::protobuf::int64 y;
    ::google::protobuf::int64 width;
    ::google::protobuf::int64 height;
};

struct sBlockExtent
{
    rect r;
    long long llFilePos;
};

class OSMPBFread{

    vector <sBlockExtent> vrectPBFBlockExtents;
public:
    OSMPBFread();
    bool open(const char * sMapFile,double startLat,double startLon, double dRange,vector <OSMnode> *vOSMnode);


};

#endif //HELLOCMAKE_OSMPBFREAD_H
