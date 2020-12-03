#include <iostream>
#include "server_manager.hpp"
#include <string>
#include <vector>
#include <thread>
#include <boost/algorithm/string.hpp>

std::string help();

app::server_manager::server_manager::server_manager(
    cvs_util::network::tcp::interf::acceptor& _acceptor,
    app::db_manager::interf::db_manager& _db_manager,
    app::logger::interf::logger& _logger,
    cvs_util::timer::interf::timer& _duration):
  acceptor(_acceptor), db_manager(_db_manager), logger(_logger),
  duration(_duration){}
void app::server_manager::server_manager::server_writes()
{
  boost::system::error_code errc;
  std::string command;
  while(should_accept_connections.load())
  {
    std::cout<<"Enter a exit command to close all connections:\n";
    getline(std::cin, command);
    if(command == "exit")
    {
      std::cout << "Shuting down the server!" << std::endl;
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  should_accept_connections.store(false);
  return ;
}
void app::server_manager::server_manager::start_server()
{
  try
  {
    boost::asio::ip::address_v4 ip_address;
    boost::asio::ip::tcp::endpoint endpoint(ip_address, 1234);
    boost::system::error_code errc;
    acceptor.open(boost::asio::ip::tcp::v4(),errc);
    if(errc)
    {
      std::cout << "Failed to open acceptor\n" <<std::endl;
      return;
    }
    errc.clear();
    acceptor.set_reuse_address(true,errc);
    if(errc)
    {
      std::cout << "Failed to set reuse option\n" << std::endl;
      return;
    }
    errc.clear();
    acceptor.bind(endpoint, errc);
    if(errc)
    {
      std::cout << "Failed to bind [" <<errc.message() << "]!\n" <<std::endl;
      return;
    }
    errc.clear();
    acceptor.listen(static_cast<int>(
                      boost::asio::socket_base::max_connections),errc);
    if(errc)
    {
      std::cout << "Failed to listen!\n" ;
      return;
    }
    errc.clear();
    std::cout << "Server has been started...\n";
    acceptor.non_blocking(true,errc);
    if(errc)
    {
      std::cout << "Failed to set the acceptor to be non blocking [" <<
                   errc.message() << "]\n";
      return;
    }
    errc.clear();
    std::cout << "Max listen connections: "
              << boost::asio::socket_base::max_listen_connections << std::endl;

    while(should_accept_connections.load())
    {
      cvs_util::network::socket_tcp_ptr client_socket = acceptor.accept(errc);
      if(errc)
      {
        if(errc == boost::asio::error::would_block)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          continue;
        }
        else
        {
          std::cout << "Error accepting connection [ "
                    << errc.message() << "]" << std::endl;
          break;
        }
      }
      {
        std::lock_guard<std::mutex> lock(client_socket_mtx);
        client_sockets.emplace_back(client_socket);
      }
      client_threads.emplace_back(
            &app::server_manager::server_manager::client_handler,this ,
            client_socket);
    }

    std::cout << "Waiting for client threads to stop...\n";
    for(auto & client_socket : client_sockets)
    {
      std::lock_guard<std::mutex> lock(client_socket_mtx);
      client_socket->shutdown(boost::asio::socket_base::shutdown_both, errc);
      if(errc)
      {
        std::cout << "Error while shutting down client!\n";
        return;
      }
      errc.clear();
      client_socket->close(errc);
      if(errc)
      {
        std::cout << "Error while closing client!\n";
        return;
      }
      errc.clear();
    }
    for(auto & client_thread : client_threads)
    {
      client_thread.join();
    }
    std::cout << "All client threads stoped!\n";
    return ;
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
}
void app::server_manager::server_manager::client_handler(
    cvs_util::network::socket_tcp_ptr client)
{
  std::string client_info;
  std::string message_to_client (1024, '\0');
  boost::system::error_code errc;
  if(!client->is_open())
  {
    std::cout << "Error in client handler thread. Socket is not open!"
              << std::endl;
    return ;
  }
  auto endpoint = client->remote_endpoint(errc);
  if(!errc)
  {
    client_info = endpoint.address().to_string();
  }
  errc.clear();

  std::cout << "New client conected from [ " << client_info << "]" <<std::endl;
  {
    std::lock_guard<std::mutex> lock(logger_mtx);
    logger.log("New client connected from : ["+client_info+"]");
  }
  client->write_some(help(), errc);
  if(errc)
  {
    std::cout<< "Error while sending message to client!\n";
  }
  errc.clear();
  while(should_accept_connections.load())
  {
    std::chrono::time_point< std::chrono::steady_clock > start_request,
        end_request, start_response, end_response;
    std::string message_from_client (1024, '\0');
    std::vector<std::string> client_command_split;
    start_request = duration.steady_clock_now();
    size_t no_byte_read = client->read_some(message_from_client, errc);
    if(no_byte_read == 0)
    {
      continue;
    }
    end_request = duration.steady_clock_now();
    if(errc)
    {
      std::cout << "Error while receiving message from client ["
                << client_info << "]" <<errc.message() << std::endl;
      break;
    }
    errc.clear();
    //-------------------------------------------------------------------------
    if (message_from_client.size () > 0)
    {
      message_from_client.resize(message_from_client.size () - 2);
    }
    boost::split(client_command_split, message_from_client,
                 boost::is_any_of("#"));
    if(client_command_split.size() == 1 )
    {
      if(client_command_split[0] == "get_all_categories")
      {
        start_response = duration.steady_clock_now();
        message_to_client = db_manager.get_all_categories();
        end_response = duration.steady_clock_now();
      }
      else if(client_command_split[0] == "")
      {
        message_to_client = "You have to enter a command for getting some "
                            "results!\n";
      }
      else if(client_command_split[0] == "exit")
      {
        {
          message_to_client = "You have been disconnected at your request!\n";
          break;
        }
      }
      else if(client_command_split[0] == "help")
      {
        {
          message_to_client = help();
        }
      }
      else
      {
        message_to_client = "Invalid command for getting all categories!\n";
      }
    }
    //-------------------------------------------------------------------------
    else if(client_command_split.size() == 2)
    {
      if(client_command_split[0] == "get_categories_of_category" )
      {
        try
        {
          stoi(client_command_split[1]);
          start_response = duration.steady_clock_now();
          message_to_client = db_manager.get_categories_of_category(
                client_command_split[1]);
          end_response = duration.steady_clock_now();
        }
        catch (std::exception e)
        {
          message_to_client = "Second parameter should be an interger! "
                              "In this case category_id should be a number!\n";
        }
      }
      else if(client_command_split[0] == "get_products_of_category")
      {
        try
        {
          stoi(client_command_split[1]);
          start_response = duration.steady_clock_now();
          message_to_client = db_manager.get_products_of_category(
                client_command_split[1]);
          end_response = duration.steady_clock_now();
        }
        catch (std::exception e)
        {
          message_to_client = "Second parameter should be an interger! "
                              "In this case category_id should be a number!\n";
        }
      }
      else if(client_command_split[0] == "remove_category")
      {
        try
        {
          stoi(client_command_split[1]);
          start_response = duration.steady_clock_now();
          message_to_client = db_manager.remove_category(
                client_command_split[1]);
          end_response = duration.steady_clock_now();
        }
        catch (std::exception e)
        {
          message_to_client = "Second parameter should be an interger! "
                              "In this case category_id should be a number!\n";
        }
      }
      else if(client_command_split[0] == "remove_product" )
      {
        try
        {
          stoi(client_command_split[1]);
          start_response = duration.steady_clock_now();
          message_to_client = db_manager.remove_product(
                client_command_split[1]);
          end_response = duration.steady_clock_now();
        }
        catch (std::exception e)
        {
          message_to_client = "Second parameter should be an interger! "
                              "In this product_id should be a number!\n";
        }
      }
      else
      {
        message_to_client = "Invalid command, check help section "
                            "(Type \'help\') to see all possible commands!\n";
      }
    }
    else if(client_command_split.size() == 4)
    {
      if(client_command_split[0] == "add_category")
      {
        if( client_command_split[1].size() > 0 &&
            client_command_split[1].size() < 50)
        {
          if(client_command_split[2].size() > 0 &&
             client_command_split[2].size() < 200)
          {
            if(client_command_split[3].size())
            {
              try {
                stoi(client_command_split[3]);
                start_response = duration.steady_clock_now();
                message_to_client = db_manager.add_category(
                      client_command_split[1],client_command_split[2],
                    client_command_split[3]);
                end_response = duration.steady_clock_now();
              }
              catch (std::exception e)
              {
                message_to_client = "Invalid command! Wrong parent category "
                                    "format! Head_category_id should be an "
                                    "integer number!\n";
              }
            }
            else
            {
              client_command_split[3] = "NULL";
              start_response = duration.steady_clock_now();
              message_to_client = db_manager.add_category(
                    client_command_split[1],client_command_split[2],
                  client_command_split[3]);
              end_response = duration.steady_clock_now();
            }
          }
          else
          {
            message_to_client = "Invalid command! Wrong category description"
                                " size!\n";
          }
        }
        else
        {
          message_to_client = "Invalid command! Wrong category name"
                              " size!\n";
        }
      }
    }
    else if(client_command_split.size() == 5)
    {
      try
      {
        stof(client_command_split[2]);
        try
        {
          stoi(client_command_split[4]);
          if(client_command_split[0] == "add_product")
          {
            if(client_command_split[1].size() > 0 &&
               client_command_split[1].size() < 50)
            {
              if(client_command_split[3].size() > 0 &&
                 client_command_split[3].size() < 200)
              {
                start_response = duration.steady_clock_now();
                message_to_client = db_manager.add_product(
                      client_command_split[1], client_command_split[2],
                    client_command_split[3], client_command_split[4]);
                end_response = duration.steady_clock_now();
              }
              else
              {
                message_to_client = "Invalid command! Wrong product description"
                                    " size\n";
              }
            }
            else
            {
              message_to_client = "Invalid command! Wrong product name size!\n";
            }
          }
          else
          {
            message_to_client = "Invalid category_id parameter! Category_id should"
                                "be a number!\n";
          }
        }
        catch (std::exception e)
        {
          message_to_client = "Invalid category_id parameter! Category_id should"
                              "be a number!\n";
        }
      }
      catch (std::exception e)
      {
        message_to_client = "Invalid price parameter! Product_price should"
                            "be a real number!\n";
      }
    }
    else
    {
      message_to_client = "Invalid command, check help section (Type \'help\')"
                          " to see all possible commands!\n";
    }
    std::chrono::duration<double,std::micro> exection_request = end_request -
        start_request;
    std::chrono::duration<double,std::micro> exection_response = end_response -
        start_response;
    {
      std::lock_guard<std::mutex> lock(logger_mtx);
      logger.log("[CLIENT]: " + message_from_client + " Execution time: " +
                 std::to_string(exection_request.count()) +"\n[SERVER]:\n"+
                 message_to_client + "\n Execution time:" +
                 std::to_string(exection_response.count()));
    }
    client->write_some(message_to_client, errc);
    client_command_split.clear();
    message_to_client.clear();
  }
  client->write_some("You have been disconnected at your request!\n", errc);
  {
    std::lock_guard<std::mutex> lock(logger_mtx);
    logger.log("Client["+ client_info + "] was disconnected\n" );
  }

  {
    std::lock_guard< std::mutex > guard(client_socket_mtx);
    client_sockets.erase(std::remove(client_sockets.begin(),
                                     client_sockets.end(), client),
                         client_sockets.end());
  }
}
std::string help()
{
  std::string help =
      "get_categories \nget_categories_of_category#head_category_id"
      "\nget_products_of_category#product_id"
      "\nadd_category#category_name#category_desc#head_category_id"
      "\nadd_product#product_name#price#product_desc#category_id"
      "\nremove_category#category_id""\nremove_product#product_id "
      "\nhelp - To see commands \nexit -To close end connection with server\n" ;
  return help;
}
app::server_manager::server_manager::~server_manager()
{ }

