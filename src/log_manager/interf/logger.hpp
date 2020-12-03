#ifndef LOGGER_INTERF_HPP
#define LOGGER_INTERF_HPP
#include <iostream>

namespace app
{
namespace logger
{
namespace interf
{
/**
 * @brief The logger interface class used to log all important events into a file
 */
class logger
{
public:
  /**
    *@brief ~logger virtual destructor
    */
  virtual ~logger() = default;
  /**
   * @brief log This function will write all important events into specified file
   * @param message - Used to pass event to log function to be writed into file
   * @return True in case log function wrote successful into file and False if not
 */
  virtual bool  log(const std::string& message) = 0;
};
} //namespace interf
} //namespace logger
} //namespace app

#endif
