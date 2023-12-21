cmake_minimum_required(VERSION 3.26.4)
project(zLog)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(${PROJECT_NAME} main.c   
                               src/zLog.h        ) 
