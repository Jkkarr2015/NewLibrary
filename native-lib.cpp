#include <jni.h>
#include <string>
#include "Location.h"
#include "OSMPOISearch.h"
#include <android/log.h>
#include "google/include/protobuf/stubs/common.h"

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_example_jkarr_hello_1cmake_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_jkarr_hello_1cmake_MainActivity_testBeaconSearch(JNIEnv *env, jobject instance) {

    Location loc("test");
    loc.setLatitude(38.2572978);
    loc.setLongitude(-85.7142547);

    double radius = 150.0;

    int limit = 10;
    OSMPOISearch searchTest;

    searchTest.beaconSearch(&loc,radius,limit,false);


}extern "C"
JNIEXPORT void JNICALL
Java_com_example_jkarr_hello_1cmake_MainActivity_testPOISearch(JNIEnv *env, jobject instance) {



}