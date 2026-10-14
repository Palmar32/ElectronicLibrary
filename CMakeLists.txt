cmake_minimum_required(VERSION 3.20)
project(ElectronicLibrary)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

add_executable(ElectronicLibrary
        main.cpp
        models/Book.cpp
        models/Reader.cpp
        models/Loan.cpp
        services/Library.cpp
        storage/Database.cpp
)

target_link_libraries(ElectronicLibrary PRIVATE SQLite::SQLite3)