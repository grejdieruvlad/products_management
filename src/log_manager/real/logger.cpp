#include "logger.hpp"
#include <cvs_util/timer/interf/timer.hpp>
#include <cvs_util/file/interf/file.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

app::logger::real::logger::logger(cvs_util::file::interf::file& _log_file,
                             cvs_util::timer::interf::timer& _log_time):
  log_file(_log_file), log_time(_log_time)
{ }
bool app::logger::real::logger::log(const std::string& message)
{
  log_file.open("../src/logs.txt", std::ios_base::out | std::ios_base::app);
  if(log_file.is_open())
  {
    current_time = log_time.time(&current_time);
    log_time.localtime(&current_time,&current_time_tm);
    std::string log_locatime = "[";
    std::stringstream transTime;
    transTime << std::put_time(&current_time_tm,"%c");
    log_locatime.append(transTime.str());
    log_locatime.append("]: ");
    log_file.write(log_locatime.c_str(),log_locatime.size());
    log_file.write(message.c_str(), message.size());
    log_file.put('\n');
  }
  else
  {
    return false;
  }
  log_file.close();
  return true;
}
app::logger::real::logger::~logger(){}
