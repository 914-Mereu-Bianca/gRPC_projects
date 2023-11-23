
cmake_minimum_required(VERSION 3.8)

project(gRPCServer)

set(CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH "/root/.local")

find_package(Threads REQUIRED)

# Includes the Server header directory
include_directories(${CMAKE_SOURCE_DIR}/server/include)
include_directories(~/grpc)

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)

message(STATUS "Using protobuf ${Protobuf_VERSION}")
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)

if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
get_target_property(_GRPC_GRPC_INCLUDE_DIR gRPC::grpc INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(_GRPC_GRPCPP_INCLUDE_DIR gRPC::grpc++ INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)

if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# include protobuf
include_directories(${Protobuf_INCLUDE_DIRS} ${_GRPC_GRPCPP_INCLUDE_DIR} ${_GRPC_GRPC_INCLUDE_DIR})
 
# get path to the protos files
get_filename_component(PROTO_FILE "./proto/data.proto" ABSOLUTE)
get_filename_component(PROTO_FILE_PATH "${PROTO_FILE}" PATH)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/proto_generated/)

# setting the location for generated files
set(
  PROTO_SOURCES
  "${CMAKE_SOURCE_DIR}/build/proto_generated/data.pb.cc"
)

set(
  PROTO_HEADERS
  "${CMAKE_SOURCE_DIR}/build/proto_generated/data.pb.h"
)

set(
  GRPC_SOURCES
  "${CMAKE_SOURCE_DIR}/build/proto_generated/data.grpc.pb.cc"
)

set(
  GRPC_HEADERS
  "${CMAKE_SOURCE_DIR}/build/proto_generated/data.grpc.pb.h"
)

# generate *.pb* and *.qrpc* files from .proto file
add_custom_command(
  OUTPUT "${PROTO_SOURCES}" "${PROTO_HEADERS}" "${GRPC_SOURCES}" "${GRPC_HEADERS}"
  COMMAND ${_PROTOBUF_PROTOC}
  ARGS --grpc_out "${CMAKE_SOURCE_DIR}/build/proto_generated"
       --cpp_out "${CMAKE_SOURCE_DIR}/build/proto_generated"
       -I "${PROTO_FILE_PATH}"
       --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
       "${PROTO_FILE}"
  DEPENDS "${PROTO_FILE}"
)

# add genearated files to a library
add_library(GrpcProtoLib
  ${PROTO_SOURCES}
  ${PROTO_HEADERS}
  ${GRPC_SOURCES}
  ${GRPC_HEADERS}
)

target_link_libraries(GrpcProtoLib

  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# sets sources for server
set(
  SERVER_SOURCES
  server/main.cpp
  server/src/server_impl.cpp
)

add_executable(
  ${CMAKE_PROJECT_NAME}
  ${SERVER_SOURCES}
)

# links against the necessary depdendencies
target_link_libraries(${CMAKE_PROJECT_NAME}
  GrpcProtoLib
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

# Sets sources for Client
set(CLIENT_SOURCES
  client/main.cpp
  client/src/client_impl.cpp
)

# Sets executable for Client
add_executable(gRPCClient
  ${CLIENT_SOURCES}
)

target_link_libraries(gRPCClient
  GrpcProtoLib
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)