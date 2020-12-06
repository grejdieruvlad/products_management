#include <iostream>
#include <cvs_util/network/tcp/real/acceptor.hpp>
#include <cvs_util/file/real/file.hpp>
#include <cvs_util/timer/real/timer.hpp>
#include <cvs_query/mysql_connection_info.hpp>
#include <cvs_query/real/mysql_system.hpp>
#include "server_manager/server_manager.hpp"
#include "db_manager/real/db_manager.hpp"
#include "log_manager/real/logger.hpp"
#include <thread>

using namespace std;

int main()
{
  int n = 9, div = 0;
  div = n/0;
  std::cout << div << std::endl;
  cout << "Hello World!" << endl;
  cvs_util::network::tcp::real::acceptor my_acceptor;
  cvs_util::file::real::file my_file;
  cvs_util::timer::real::timer log_time;
  cvs_util::timer::real::timer my_duration;
  cvs_query::mysql_connection_info my_conn;
  my_conn.user = "vgrejdieru";
  my_conn.passwd = "vgrejdieru_cvs";
  my_conn.db = "training_db";
  my_conn.host = "127.0.0.1";
  cvs_query::real::mysql_system my_system;
  app::db_manager::real::db_manager db(my_conn,my_system);
  app::logger::real::logger logger(my_file,log_time);
  app::server_manager::server_manager *server =
      new  app::server_manager::server_manager(
        my_acceptor, db, logger,my_duration);
  std::thread server_thread(
        &app::server_manager::server_manager::start_server, server);
  std::cout << "Type \"exit\" to stop:" << std::endl;
  std::thread server_writes_th(
        &app::server_manager::server_manager::server_writes, server);
  server_writes_th.detach();
  server_thread.join();
  delete (server);
  std::cout << "Server is closed!" << std::endl;
  return 0;
}
