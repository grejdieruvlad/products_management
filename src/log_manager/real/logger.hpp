#ifndef LOGGER_REAL_HPP
#define LOGGER_REAL_HPP
#include <iostream>
#include "../interf/logger.hpp"
#include <cvs_util/timer/interf/timer.hpp>
#include <cvs_util/file/interf/file.hpp>

namespace app
{
namespace logger
{
namespace real
{
/**
 * @brief The logger class describe real implementation of interf::logger
 */
class logger : public interf::logger
{
private:
  /**
   * @brief current_time - used to store number of second from 1970 until now
   */
  time_t current_time;
  /**
   * @brief current_time_tm - used to store current_time as broken_time
   */
  tm current_time_tm;
  /**
   * @brief log_file Is used for working with files with the purpose to log
   * any important things into a file, like new client connecter/disconnected,
   * client's commands or server's answers
   */
  cvs_util::file::interf::file& log_file;
  /**
   * @brief log_time Is used to determine all query execution time
   */
  cvs_util::timer::interf::timer& log_time;
public:
  /**
   * @brief logger::logger - Constructor for logger class
   * @param log_file - Reference to a file object used by dependency injection
   * for taking a file into wich we will  log all events
   * @param log_time - Reference to a timer object used by dependency injection
   * for taking time when wich message was logged
   */
  logger(cvs_util::file::interf::file& log_file,
         cvs_util::timer::interf::timer& log_time);
  /**
   * @brief log This function will write all important events into specified file
   * @param message - Used to pass event to log function to be writed into file
   * @return True in case log function wrote successful into file and False if not
 */
  bool log(const std::string & message) override;
  /**
    *@brief Overriden destructor
    */
  ~logger()  override;
};
} //namespace real
} //namespace logger
} //namespace app

#endif
