cmake_minimum_required(VERSION 3.10)
project(clientHTTP CXX)

# Set standard C++ 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type (Debug/Release)
set(CMAKE_BUILD_TYPE Debug)

# Include directory for libcurl
include_directories(${CURL_INCLUDE_DIR})

# Sources for your executable
set(SOURCES
    main.cpp
    http_client.cpp  # Include your http_client.cpp file here
)

# Find libcurl package
find_package(CURL REQUIRED)

# Add the executable
add_executable(clientHTTP ${SOURCES})

# Link necessary libraries (libcurl)
target_link_libraries(clientHTTP CURL::libcurl)
