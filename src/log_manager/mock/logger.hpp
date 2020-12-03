#ifndef LOGGER_MOCK_HPP
#define LOGGER_MOCK_HPP

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../interf/logger.hpp"
namespace app
{
namespace logger
{
namespace mock
{
/**
  *@brief logger class that describe the mock implementation of interf::logger
  */
class logger : public interf::logger
{
/**
  *@brief - Mock implementation for log method
  */
  MOCK_METHOD(bool, log, (const std::string&));
};
} //namespace mock
} //namespace logger
} //namespace app

#endif
