//
// Created by jkarr on 12/11/2017.
//

#ifndef HELLOCMAKE_OSMPBF_H
#define HELLOCMAKE_OSMPBF_H

#include <iostream>
#include <fstream>
#include <sstream>
#import <string>
#include "osmformat.pb.h"
#include "fileformat.pb.h"
#include "zlib.h"
using namespace std;

#define MAX_BLOCK_HEADER_SIZE (64*1024)
#define MAX_BLOB_SIZE         (32*1024*024)
#define NANO                  (1000.0*1000.0*1000.0)
class osmpbf {

        long lBufferPosition; //where in the data the buffer we are at
        const long lHeaderLength=4;
public:
    osmpbf();
};


#endif //HELLOCMAKE_OSMPBF_H
