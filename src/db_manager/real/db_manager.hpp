#ifndef DB_MANAGER_REAL_HPP
#define DB_MANAGER_REAL_HPP

#include "../interf/db_manager.hpp"

/**
 * @brief The db_manager class describe the real implementation of db_manager
 * interface
 */
namespace app
{
namespace db_manager
{
namespace real
{
class db_manager: public interf::db_manager
{
private:
  /**
   * @brief mysql_connection_info - Is a structure that hold database connection
   * info
   */
  cvs_query::mysql_connection_info& mysql_connection_info;
  /**
   * @brief my_system - Used to send SQL commands (queries) to a MySQL server
   * or to get a new connection object
   */
  cvs_query::interf::mysql_system& my_system;
public:
  /**
   * @brief db_manager - Constructor
   * @param _mysql_connection_info - Is used to hold database connection info
   * @param _my_mysystem - Is used to inject an mysql_system object
   */
  db_manager(cvs_query::mysql_connection_info& _mysql_connection_info,
             cvs_query::interf::mysql_system& _my_system);
  /**
   * @brief ~db_manager - virtual destructor
   */
  virtual ~db_manager() override;
  /**
   * @brief get_all_categories This function will select all categories from db
   * @return This function return database query result that may containt
   * a set of rows or a "Empty set" message or just an exception message
   */
  const std::string get_all_categories() override;
  /**
   * @brief get_categories_of_category This function will select all categories
   * that have HEAD_CATEGORY_ID equal with the client given one.
   * @param category_id Parent category id given by client to server
   * @return This function will return a set of categories or an "Empty set"
   * message or an exceprtion message
   */
  const std::string get_categories_of_category(
      const std::string &category_id) override;
  /**
   * @brief get_products_of_category This function will select all products
   * that have CATEGORY_ID equal with the client given one.
   * @param category_id The category_id for wich the client ask for products
   * @return This function return a set of products for respective category_id
   */
  const std::string  get_products_of_category(
      const std::string &category_id) override;
  /**
   * @brief add_category  -  This function will add a new category
   *  in categories
   * tabel from database
   * @param category_name New category's name to add into table categories
   * @param category_desc New category's description to add into categories table
   * @param head_category_id Parent category from wich will derive new category
   * @return This function will return a succesfull message or an
   * message that will contain an exception given by cvs_query
   */
  const std::string add_category(
      const std::string& category_name,
      const std::string& category_desc,
      const std::string& head_category_id) override;
  /**
   * @brief add_product - This function will add a new product in products
   * tabel from database
   * @param product_name - Product name of new record into table products
   * @param product_price - Product price of new record into table products
   * @param product_desc - Product description of new record into table products
   * @param category_id - Product category of new record into table products
   * @return This function will return a succesfull message or an
   * message that will contain an exception given by cvs_query
   */
  const std::string add_product(
      const std::string& product_name,
      const std::string& product_price,
      const std::string& product_desc,
      const std::string& category_id) override;
  /**
   * @brief remove_category This function will remove a category specified by
   * client
   * @param category_id - Is the category_id that will be removed from db
   * @return This function will return a succesfull message in case of removing
   * or an message that will contain an exception given by cvs_query
   */
  const std::string remove_category(const std::string &category_id) override;
  /**
   * @brief remove_product This function will remove a category specified by
   * client
   * @param product_id - Is the category_id that will be removed from db
   * @return  This function will return a succesfull message in case of removing
   * or an message that will contain an exception given by cvs_query
   */
  const std::string remove_product(const std::string &product_id) override;
};
} //namespace real
} //namespace db_manager
} // namespace app
#endif
