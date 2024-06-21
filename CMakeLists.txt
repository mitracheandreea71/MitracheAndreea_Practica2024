cmake_minimum_required(VERSION 3.10)
project(clientHTTP)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

# Adăugați fișierul sursă principal
add_executable(clientHTTP main.cpp)

# Includeți biblioteca curl
find_package(CURL REQUIRED)
target_link_libraries(clientHTTP CURL::libcurl)
