//
// Created by jkarr on 12/11/2017.
//

#include <string>


struct Beacon{
    std::string UUID;
    unsigned short major;
    unsigned short minor;
    double lat;
    double lon;
    std::string name;
    std::string type;
    float level;
    double distance;
};
