LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := protobuf-lite
LOCAL_SRC_FILES := libprotobuf-lite.so 
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := native-lib
LOCAL_SHARED_LIBRARIES := libprotobuf-lite
LOCAL_CPP_EXTENSION := .cxx .cpp .cc
LOCAL_LDFLAGS := -lz -llog 
LOCAL_CPP_FEATURES := rtti
LOCAL_SRC_FILES := \
    native-lib.cpp \
	google/include/protobuf/io/coded_stream.cc \
	google/include/protobuf/wire_format_lite.cc \
	google/include/protobuf/arenastring.cc \
	google/include/protobuf/arena.cc \
	google/include/protobuf/stubs/int128.cc \
	google/include/protobuf/message_lite.cc \
	google/include/protobuf/io/zero_copy_stream_impl_lite.cc \
	google/include/protobuf/io/zero_copy_stream.cc \
	google/include/protobuf/generated_message_util.cc \
	google/include/protobuf/stubs/common.cc \
	BeaconSearchResult.cpp \
	Geopoint.cpp \
	fileformat.pb.cc \
	osmformat.pb.cc \
	Location.cpp \
	osmpbf.cpp \
	OSMPBFread.cpp \
	OSMPOISearch.cpp \
	POI.cpp \
LOCAL_CFLAGS    := -std=c++11
include $(BUILD_SHARED_LIBRARY)

