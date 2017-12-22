//
// Created by jkarr on 12/11/2017.
//

#include "OSMPBFread.h"
#include <string>
#include <cstdlib>
#include <zconf.h>
#include <zlib.h>
#include <android/log.h>

//****** need to keep this one
const long lHeaderLength=4;

OSMPBFread::OSMPBFread(){

}



//this one is OK for short distance away from poles
//x,y are minimum at SW corner
rect GetRectHaverSineApprox(::google::protobuf::int64 iLatCenter, ::google::protobuf::int64 iLonCenter, double dmilesSquareLength){

    rect RetVal;

    double dLatCenter = iLatCenter;
    double dLonCenter = iLonCenter;

    dLatCenter/=10000000;
    dLonCenter/=10000000;

    double dHalfLengthofSquareInMiles = dmilesSquareLength/2.0;

    float fLatRad = dLatCenter/180.0*M_PI;

    float fCos = cos(fLatRad);

    if(fCos<0.1)
        fCos=0.1;

    double dLatN2 = dLatCenter + dHalfLengthofSquareInMiles * 0.0144984587616;
    double dLatS2 = dLatCenter - dHalfLengthofSquareInMiles * 0.0144984587616;
    double dLonE2 = dLonCenter + dHalfLengthofSquareInMiles * 0.0144984587616 / fCos;
    double dLonW2 = dLonCenter - dHalfLengthofSquareInMiles * 0.0144984587616 / fCos;


    ::google::protobuf::int64 iLonW = dLonW2*10000000;

    ::google::protobuf::int64 iLonE = dLonE2*10000000;

    ::google::protobuf::int64 iLatS = dLatS2*10000000;

    ::google::protobuf::int64 iLatN = dLatN2*10000000;

    RetVal.x = iLonW;
    RetVal.y = iLatS;
    RetVal.width = iLonE-iLonW;
    RetVal.height = iLatN-iLatS;


    return RetVal;
}


bool ValueInRange(::google::protobuf::int64 value, ::google::protobuf::int64 min, ::google::protobuf::int64 max)
{ return (value >= min) && (value <= max); }

bool RectOverlap(rect A, rect B)
{
    bool xOverlap = ValueInRange(A.x, B.x, B.x + B.width) ||
                    ValueInRange(B.x, A.x, A.x + A.width);

    bool yOverlap = ValueInRange(A.y, B.y, B.y + B.height) ||
                    ValueInRange(B.y, A.y, A.y + A.height);

    return xOverlap && yOverlap;
}



//return value is how many nodes are loaded. this is for saving memory so we dont have to save the string table
long ReadDenseNodes3(
        //sBlockWithNodesC * sBWN,
        vector <OSMnode> *pvOSMnode,
        OSMPBF::PrimitiveBlock *pblock,
        const OSMPBF::PrimitiveGroup& group,
        double startLat,double startLon,
        rect C)
{
    long lNumNodesLoaded=0;
    const OSMPBF::DenseNodes& dense=group.dense();

    //   Id                     dId=0;
    double                    lID=0;
    double                 dLat=0;
    double                 dLon=0;
    int                    t=0;




    for (int d=0; d<dense.id_size();d++) {
        //   dId+=dense.id(d);
        lID+=dense.id(d);
        dLat+=dense.lat(d);
        dLon+=dense.lon(d);





        OSMnode aNC;
        aNC.id=lID;
        aNC.lat=dLat;
        aNC.lon=dLon;



        bool bGetTags=true;




        //********** start get tags
        if(bGetTags) {
            //  tagMap.clear();
            //int ikv=0;

            while(dense.keys_vals().Get(t)!=0){
                int iKey = dense.keys_vals().Get(t);
                //string asKey = sBWN->tblock.stringtable().s(iKey) ;
                string asKey = pblock->stringtable().s(iKey) ;
                int iValue =dense.keys_vals().Get(t+1);
                string asValue = pblock->stringtable().s(iValue);
                t+=2;
                //pair<::google::protobuf::uint32,::google::protobuf::uint32> atag = pair<::google::protobuf::uint32,::google::protobuf::uint32> (iKey,iValue);
                pair<string,string> atag = pair<string,string> (asKey,asValue);
                aNC.tags.push_back(atag);

            }
            t++;


        }
        //*********** end get tags



        //**** this is kinda bad, referring to the size of the index find a better way
        //aNC.iBlockIndex=vsBlockWithNodesC.size();


        if(ValueInRange(aNC.lon,C.x,C.x+C.width) &&
           ValueInRange(aNC.lat,C.y,C.y+C.height))
            pvOSMnode->push_back(aNC);


        lNumNodesLoaded++;


    }
    return lNumNodesLoaded;
}



void ReadWays2(

        const OSMPBF::PrimitiveGroup& group)
{
    for (int w=0; w<group.ways_size(); w++) {
        const OSMPBF::Way &inputWay=group.ways(w);



        sWayC asWayC;


        for (int t=0; t<inputWay.keys_size(); t++) {

            ;


        }



        asWayC.id=inputWay.id();

        unsigned long ref=0;
        for (int r=0; r<inputWay.refs_size(); r++) {
            ref+=inputWay.refs(r);

            asWayC.ipbNodeRefs.push_back(ref);

        }

        //is one of the nodes in this way? if so store the way
        //bool bIsWayReferenced = true;

        if(asWayC.ipbNodeRefs.size()==0)
            continue;


    }
}


//************************************************************************************************************************
//bool ReadPrimitiveBlock(std::vector<char>buffer,const OSMPBF::BlobHeader& blockHeader, OSMPBF::PrimitiveBlock& primitiveBlock)
//bSkipProcess true = just advance the filepointer and skip over data, false = read in and process the block
bool readPrimitiveBlockSkip(fstream *input,const OSMPBF::BlobHeader& blockHeader, OSMPBF::PrimitiveBlock& primitiveBlock,bool bSkipProcess)
{

    OSMPBF::Blob               blob;

    google::protobuf::int32 length=blockHeader.datasize();


    if (length==0 || length>MAX_BLOB_SIZE) {
        //   progress.Error("Blob size invalid!");
        return false;
    }




    std::vector<char>buffer2(length);


    //skip ahead?
    if(bSkipProcess){
        input->seekg(length+input->tellg());
        return true;
    }
    else{
        input->read(&buffer2.at(0),length);
    }


    blob.ParseFromArray(&buffer2.at(0),length);

    if (blob.has_raw()) {
        length=(uint32_t)blob.raw().length();
        //    buffer = new char[length];
        buffer2.resize(length);

        std::copy(blob.raw().data(),blob.raw().data()+length,buffer2.begin());

    }
    else if (blob.has_zlib_data()){
        //#if defined(HAVE_LIB_ZLIB)
        length=blob.raw_size();

        //************** search for all these where we need to resize the buffer2 , and rename it as necessary
        buffer2.resize(length);

        z_stream compressedStream;

        compressedStream.next_in=(Bytef*)const_cast<char*>(blob.zlib_data().data());
        compressedStream.avail_in=(uint32_t)blob.zlib_data().size();
        //   compressedStream.next_out=(Bytef*)buffer;
        compressedStream.next_out=(Bytef*)&buffer2.at(0);

        compressedStream.avail_out=length;
        compressedStream.zalloc=Z_NULL;
        compressedStream.zfree=Z_NULL;
        compressedStream.opaque=Z_NULL;

        if (inflateInit( &compressedStream)!=Z_OK) {

            return false;
        }



        if (inflate(&compressedStream,Z_FINISH)!=Z_STREAM_END) {

            return false;
        }

        if (inflateEnd(&compressedStream)!=Z_OK) {

            return false;
        }


    }
    else if (blob.has_obsolete_bzip2_data()){
        //   progress.Error("Data is bzip2 encoded but bzip2 support is not enabled!");
        return false;
    }
    else if (blob.has_lzma_data()){
        //  progress.Error("Data is lzma encoded but lzma support is not enabled!");
        return false;
    }


    //********************************



    if (!primitiveBlock.ParseFromArray(&buffer2.at(0),length)) {
        //   progress.Error("Cannot parse primitive block!");
        return false;
    }

    return true;
}


vector<string> Split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}


bool readHeaderBlock(fstream *input,OSMPBF::BlobHeader& blockHeader,OSMPBF::HeaderBlock& headerBlock)

{
    OSMPBF::Blob blob;



    uint32_t length = blockHeader.datasize();

    if (length==0 || length>MAX_BLOB_SIZE) {
        //    progress.Error("Blob size invalid!");
        return false;
    }

    std::vector<char>buffer2(length);

    input->read(&buffer2.at(0) ,length);






    //**********************************
    blob.ParseFromArray(&buffer2.at(0),length);


    //bool bzlib = blob.has_zlib_data();

    if (blob.has_raw()) {
        length=(uint32_t)blob.raw().length();
        //    buffer = new char[length];
        buffer2.resize(length);

        std::copy(blob.raw().data(),blob.raw().data()+length,buffer2.begin());

    }
    else if (blob.has_zlib_data()){
        //#if defined(HAVE_LIB_ZLIB)
        length=blob.raw_size();
        //   buffer=new char[length];

        buffer2.resize(length);

        z_stream compressedStream;

        compressedStream.next_in=(Bytef*)const_cast<char*>(blob.zlib_data().data());
        compressedStream.avail_in=(uint32_t)blob.zlib_data().size();
        //   compressedStream.next_out=(Bytef*)buffer;
        compressedStream.next_out=(Bytef*)&buffer2.at(0);

        compressedStream.avail_out=length;
        compressedStream.zalloc=Z_NULL;
        compressedStream.zfree=Z_NULL;
        compressedStream.opaque=Z_NULL;

        if (inflateInit( &compressedStream)!=Z_OK) {
            //     progress.Error("Cannot decode zlib compressed blob data!");
            //     delete[] buffer;
            return false;
        }

        if (inflate(&compressedStream,Z_FINISH)!=Z_STREAM_END) {
            //     progress.Error("Cannot decode zlib compressed blob data!");
            //      delete[] buffer;
            return false;
        }

        if (inflateEnd(&compressedStream)!=Z_OK) {
            //      progress.Error("Cannot decode zlib compressed blob data!");
            //      delete[] buffer;
            return false;
        }


    }
    else if (blob.has_obsolete_bzip2_data()){
        //   progress.Error("Data is bzip2 encoded but bzip2 support is not enabled!");
        return false;
    }
    else if (blob.has_lzma_data()){
        //  progress.Error("Data is lzma encoded but lzma support is not enabled!");
        return false;
    }


    if(!headerBlock.ParseFromArray(&buffer2.at(0),length)){

        return false;
    }


    return true;
}





bool readBlobHeader(fstream *input,OSMPBF::BlobHeader& blobHeader,bool silent)
{

    std::vector<char>blockHeaderLength(lHeaderLength);



    input->read(&blockHeaderLength.at(0) ,lHeaderLength);



    uint32_t length = (blockHeaderLength[0]>>24) + (blockHeaderLength[1]>>16) + (blockHeaderLength[2]>>8) + (blockHeaderLength[3]);


    if (length==0 || length>MAX_BLOCK_HEADER_SIZE) {
        //     progress.Error("Block header size invalid!");
        return false;
    }

    //char *buffer=new char[length];

    std::vector<char>buffer2(length);



    input->read(&buffer2.at(0) ,length);


    //**********************

    OSMPBF::BlobHeader abh;

    //  abh.ParseFromArray(acp,length);
    abh.ParseFromArray(&buffer2.at(0),length);


    blobHeader = abh;



    return true;
}



bool OSMPBFread::open(const char * sMapFile,double startLat,double startLon, double dRange, vector <OSMnode> *vOSMnode)
{

    fstream input(sMapFile, ios::in | ios::binary);

    if(!input){
        cout<< "file not found." << endl;
        return false;
    }

    //input.seekg(0, std::ios::end);
    //long fileSize = input.tellg();
    input.seekg(0, std::ios::beg);



    //****************************** reading the entire file at once, change to reading partial
    // std::vector<char> buffer(   (std::istreambuf_iterator<char>(input)),(std::istreambuf_iterator<char>()));

    ::google::protobuf::int64 istartLat = ((::google::protobuf::int64) (startLat * 10000000));
    ::google::protobuf::int64 istartLon = ((::google::protobuf::int64) (startLon * 10000000));
    rect C = GetRectHaverSineApprox(istartLat,istartLon,dRange);

    bool bHaveExtents=false;


    long lNumBlocks=0;
    long lNumBlocksRead=0;
    long lNumBlocksSkipped=0;



    OSMPBF::HeaderBlock headerBlock;

    OSMPBF::BlobHeader abh;

    //ReadBlockHeader(buffer,abh,false);
    readBlobHeader(&input,abh,false);

    //ReadHeaderBlock(buffer,  abh, headerBlock);

    readHeaderBlock(&input,abh,headerBlock);


    //    NSLog(@"open4");

    for (int i=0; i<headerBlock.required_features_size(); i++) {
        std::string feature=headerBlock.required_features(i);
        if (feature!="OsmSchema-V0.6" &&
            feature!="DenseNodes") {

            return false;
        }
    }


    vector<OSMPBF::PrimitiveBlock> vblocks;
    long lBlobsRead=0;
    //**** read in the file data start
    long lCurrentBlockExtent=0;
    long lExtentsBlocksSkipped=0;
    long lTotalLoop=0;
    //vector <OSMnode> vOSMnode;

    //  NSLog(@"file open read blob");
    while (true) {
        //OSMPBF::HeaderBlock blockHeader;
        OSMPBF::BlobHeader ablobHeader;
        //        ReadBlockHeader(buffer,blockHeader,false);

        lTotalLoop++;
        lCurrentBlockExtent++;
        long long lFilePos = input.tellg();

        bool bOverlap=false;

        //figure out if we want to skip the block
        bool bSkipBlock = false;



        //if we already read the extents, just find the blocks we need
        if(bHaveExtents){
            if((lCurrentBlockExtent+1)<vrectPBFBlockExtents.size()){

                bOverlap = RectOverlap(vrectPBFBlockExtents.at(lCurrentBlockExtent).r,C);

                if(!bOverlap){
                    input.seekg(vrectPBFBlockExtents.at(lCurrentBlockExtent+1).llFilePos);
                    //lCurrentBlockExtent++;
                    lExtentsBlocksSkipped++;
                    continue;
                }
            }

        }

        readBlobHeader(&input,ablobHeader,false);
        // NSLog(@"file open read blob");


        if (ablobHeader.type()!="OSMData") {

            break;
        }
        lBlobsRead++;
        //        else
        //            continue;
        OSMPBF::PrimitiveBlock block;
        // if (!ReadPrimitiveBlock(buffer,ablobHeader, block))
        //if (!ReadPrimitiveBlock(&input,ablobHeader, block))




        //  double dMilesSquaredBox=8.0;    //how many miles do we cover? this is the length of one of the sides in the square
        std::string sBID = ablobHeader.indexdata();
        vector <string> vsBBox = Split(sBID," ");
        if(vsBBox.size()==5){
            if(vsBBox.at(0).compare("bbox")==0){

                ::google::protobuf::int64 iLatMin =  std::atoi(vsBBox.at(1).c_str());
                ::google::protobuf::int64 iLatMax =  std::atoi(vsBBox.at(2).c_str());
                ::google::protobuf::int64 iLonMin =  std::atoi(vsBBox.at(3).c_str());
                ::google::protobuf::int64 iLonMax =  std::atoi(vsBBox.at(4).c_str());



                //   double dMilesSquaredBoxRad=dMilesSquaredBox/2.0;

                rect A;A.x=iLonMin;A.width=iLonMax-iLonMin;A.y=iLatMin;A.height=iLatMax-iLatMin;
                //rect B;B.x=istartLon - 183865;B.width= 183865*2;B.y=istartLat- 144984;B.height= 144984*2;

                if(!bHaveExtents){

                    sBlockExtent abe;
                    abe.r=A;
                    abe.llFilePos=lFilePos;
                    vrectPBFBlockExtents.push_back(abe);
                }


                //rect C = GetRectHaversineApprox(istartLat,istartLon,dRange);

                lNumBlocks++;

                //bool bOverlap = rectOverlap(A,B);

                bOverlap = RectOverlap(A,C);

                if(bOverlap){
                    bSkipBlock=false;
                    lNumBlocksRead++;
                }
                else{
                    bSkipBlock=true;
                    lNumBlocksSkipped++;
                }

            }


        }





        if (!readPrimitiveBlockSkip(&input,ablobHeader, block,bSkipBlock))
        {
            //      fclose(file);
            return false;
        }

        for (int currentGroup=0;
             currentGroup<block.primitivegroup_size();
             currentGroup++) {

            const OSMPBF::PrimitiveGroup &group=block.primitivegroup(currentGroup);



            if (group.nodes_size()>0) {
                //                ReadNodes(//typeConfig,
                //                          block,
                //                          group);
                ;
            }
            else if (group.ways_size()>0) {
                //                sBlockWithWays aSBWW;
                //                aSBWW.tblock=block;
                //                ReadWays(//typeConfig,
                //                         &aSBWW,
                //                         group);
                //vsBlockWithWays.push_back(aSBWW);
                //sBlockWithWaysC asBWWC;
                //asBWWC.tblock=block;
                //ReadWays2(&asBWWC,group);

                //onyl need ways if we are routing
                //ReadWays2(group);


            }
            else if (group.relations_size()>0) {
                //                ReadRelations(//typeConfig,
                //                              block,
                //                              group);
            }
            else if (group.has_dense()) {

                ReadDenseNodes3(vOSMnode,&block,group,startLat, startLon,C);
            }

        }//end for

    }   //end while
    //*********** read in the file data end



    //NSLog(@"file open read blob end");

    return true;
}