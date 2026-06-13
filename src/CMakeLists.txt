include("${_build_cmake_local}")

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)

set(VIDEO_TELEMETRY_PROTO "${CMAKE_CURRENT_SOURCE_DIR}/PiSubmarine/Video/Telemetry/Protobuf/Video.proto")
protobuf_generate_cpp(VIDEO_TELEMETRY_PROTO_SRCS VIDEO_TELEMETRY_PROTO_HDRS "${VIDEO_TELEMETRY_PROTO}")

message("Adding library: ${PISUBMARINE_MODULE_NAME}")
add_library(${PISUBMARINE_MODULE_NAME}
        PiSubmarine/Video/Telemetry/Protobuf/Deserializer.cpp
        PiSubmarine/Video/Telemetry/Protobuf/ErrorCode.cpp
        PiSubmarine/Video/Telemetry/Protobuf/Serializer.cpp
        "../public/PiSubmarine/Video/Telemetry/Protobuf/Deserializer.h"
        "../public/PiSubmarine/Video/Telemetry/Protobuf/ErrorCode.h"
        "../public/PiSubmarine/Video/Telemetry/Protobuf/Serializer.h"
        ${VIDEO_TELEMETRY_PROTO_SRCS}
        ${VIDEO_TELEMETRY_PROTO_HDRS})
target_include_directories(${PISUBMARINE_MODULE_NAME} PUBLIC "../public" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
PiSubmarineInitTarget(${PISUBMARINE_MODULE_NAME})

PiSubmarineAddDependency("https://github.com/PiSubmarine/Video.Telemetry.Api.git" "")
target_link_libraries(${PISUBMARINE_MODULE_NAME} PUBLIC PiSubmarine.Video.Telemetry.Api)

PiSubmarineAddDependency("https://github.com/PiSubmarine/Telemetry.Api.git" "")
target_link_libraries(${PISUBMARINE_MODULE_NAME} PUBLIC PiSubmarine.Telemetry.Api)

target_link_libraries(${PISUBMARINE_MODULE_NAME} PRIVATE protobuf::libprotobuf)
