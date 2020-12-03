#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "db_manager_tests.hpp"
#include "logger_tests.hpp"

int main(int argc, char *argv[])
{
  std::cout << "Running all test..." << std::endl;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
