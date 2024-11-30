cmake_minimum_required(VERSION 3.10)
project(geometry VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
	geometry.cpp
	vertex.cpp
)
set(HEADERS 
	geometry.h
	vertex.h
)

add_library(
	${PROJECT_NAME} 
	STATIC 
	${SOURCES}
	${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_include_directories(${PROJECT_NAME} PUBLIC .)

