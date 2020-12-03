#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <cvs_util/network/tcp/interf/acceptor.hpp>
#include <cvs_util/file/interf/file.hpp>
#include <cvs_util/timer/interf/timer.hpp>
#include "../db_manager/interf/db_manager.hpp"
#include "../log_manager/interf/logger.hpp"
#include <boost/atomic.hpp>
#include <vector>
#include <mutex>

/**
 * @brief The server_manager class The server layer implementation
 */
namespace app {
namespace server_manager{

class server_manager
{
private:
  /**
   * @brief acceptor - Is used for accepting new socket connections
   */
  cvs_util::network::tcp::interf::acceptor& acceptor;
  /**
   * @brief logger - Refference to logger class object used by dependency
   * injection used to perform logger functionalities
   */
  logger::interf::logger& logger;
  /*
   * /**
   * @brief log_file - Is used for working with files with the purpose to log
   * any important things into a file, like new client connecter/disconnected,
   * client's commands or server's answers

  cvs_util::file::interf::file& log_file;
  */
  /**
   * @brief duration - Is used to determine all query execution time
   */
  cvs_util::timer::interf::timer& duration;
  /**
   * @brief should_accept_connections - Is used to manage moments when server
   * will not accept new connections or server will close all connections.
   */
  boost::atomic<bool> should_accept_connections {true};
  /**
   * @brief client_threads - Is used to store threads for each client socket
   */
  std::vector<std::thread> client_threads;
  /**
   * @brief client_sockets - Is used to store all connected client's sockets
   */
  std::vector<cvs_util::network::socket_tcp_ptr> client_sockets;
  /**
   * @brief db_manager - Is used to maintain database communication trough
   * dependency injection
   */
  db_manager::interf::db_manager& db_manager;
  std::mutex client_socket_mtx, logger_mtx;
public:
  /**
   * @brief server_manager - Constructor with all dependency injections
   * @param _acceptor - Is used to inject an acceptor object
   * @param _db_manager - Is used to inject an db_manager object
   */
  server_manager(cvs_util::network::tcp::interf::acceptor& _acceptor,
                 db_manager::interf::db_manager& _db_manager,
                 logger::interf::logger& _logger,
                 cvs_util::timer::interf::timer& _duration);
  /**
   * @brief server_writes This function will run on a separate thread and will
   * wait for a message from server. e.g. "Exit"
   */
  void server_writes();
  /**
   * @brief accept_connections This function will initiate server socket, wil bind and
   * set all neceseries socket's options and will put server in listening state.
   */
  void accept_connections(cvs_util::network::tcp::interf::acceptor&);
  /**
   * @brief start_server This function will initiate server on a new thread
   */
  void start_server( );
  /**
   * @brief log - This function will log into a file all important things that
   * will happen on the server
   * @param message - This parameter will contain info to be logged into file
   * e.g. client's commands, server's answers
   */
  void log(const std::string& message);
  /**
   * @brief client_handler - This function will maintain communication between
   * server and each client on separated thread
   * @param client - Pointer that holds address of client's socket object
   */
  void client_handler(cvs_util::network::socket_tcp_ptr client);
  /*/**
   * @brief check_client_command check_client_command This function will verify
   * if client's command is one of corrects variants or not
   * @param message_to_client - Is used to send database result to client
   * @param message_from_client - Is full message from client
   * @param client_command_split - Used for store the result of boost::split
   *
  void check_client_command(
      std::string& message_to_client,
      std::string& message_from_client,
      std::vector<std::string>& client_command_split); */
  /**
   * @brief format_categories_result This function will format the result for
   * get_categories_of_category in a table format
   * @param rows - Is used to reference a vector of strings that will be used
   * to store all rows of database query result
   * @return - This function will return a formated string that contain
   * database query result
   */
  std::string format_categories_result(
      const std::vector<std::string>& rows);
  /**
   * @brief format_products_result This function will format the result for
   * get_products_of_category in a table format
   * @param rows - Is used to reference a vector of strings that will be used
   * to store all rows of database query result
   * @return This function will return a formated string that contain
   * database query result
   */
  std::string format_products_result(
      const std::vector<std::string>& rows);

  ~server_manager();
};
} //namespace server_manager
} //namespace app
#endif // SERVER_MANAGER_HPP
