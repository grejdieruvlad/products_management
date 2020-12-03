cmake_minimum_required(VERSION 3.5)
project(project LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsanitize=thread")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_BUILD_TYPE "Debug")
include_directories(src)

add_executable(project
   src/main.cpp
  src/server_manager/server_manager.hpp
  src/server_manager/server_manager.cpp
  src/db_manager/real/db_manager.hpp
  src/db_manager/real/db_manager.cpp
  src/db_manager/interf/db_manager.hpp
  src/db_manager/mock/db_manager.hpp
  src/log_manager/interf/logger.hpp
  src/log_manager/real/logger.hpp
  src/log_manager/real/logger.cpp
  src/log_manager/mock/logger.hpp)
add_executable("project_test"
  src/run_tests.cpp
  src/db_manager_tests.hpp
  src/logger_tests.hpp
  src/server_manager/server_manager.hpp
  src/server_manager/server_manager.cpp
  src/db_manager/real/db_manager.hpp
  src/db_manager/real/db_manager.cpp
  src/db_manager/interf/db_manager.hpp
  src/db_manager/mock/db_manager.hpp
  src/log_manager/interf/logger.hpp
  src/log_manager/real/logger.hpp
  src/log_manager/real/logger.cpp
  src/log_manager/mock/logger.hpp)

FILE(GLOB_RECURSE conan_files "conanfile.py")
add_custom_target(conan SOURCES ${conan_files})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP(TARGETS)
target_link_libraries("project_test" pthread ${CONAN_TARGETS} ${CMAKE_DL_LIBS})
target_link_libraries(${PROJECT_NAME} pthread ${CONAN_TARGETS} ${CMAKE_DL_LIBS})
