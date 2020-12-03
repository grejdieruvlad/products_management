#ifndef DB_MANAGER_INTERF_HPP
#define DB_MANAGER_INTERF_HPP

#include <cvs_query/mysql_connection_info.hpp>
#include <cvs_query/interf/mysql_system.hpp>
#include <vector>
/**
 * @brief The db_manager class - implements database layer
 */
namespace app
{
namespace db_manager
{
namespace interf
{
class db_manager
{
public:
  /**
   * @brief ~db_manager virtual desctructor
   */
  virtual ~db_manager() = default;
  /**
   * @brief get_all_categories This function will select all categories from db
   * @return This function return database query result that may containt
   * a set of rows or a "Empty set" message or just an exception message
   */
  virtual const std::string get_all_categories() = 0;
  /**
   * @brief get_categories_of_category This function will select all categories
   * that have HEAD_CATEGORY_ID equal with the client given one.
   * @param category_id Parent category id given by client to server
   * @return  This function will return a set of categories or an "Empty set"
   * message or an exceprtion message
   */
  virtual const std::string get_categories_of_category(
      const std::string& category_id) = 0 ;
  /**
   * @brief get_products_of_category This function will select all products
   * that have CATEGORY_ID equal with the client given one.
   * @param category_id The category_id for wich the client ask for products
   * @return This function return a set of products for respective category_id
   */
  virtual const std::string get_products_of_category(
      const std::string& category_id) = 0 ;
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
  virtual const std::string add_category(
      const std::string& category_name,
      const std::string& category_desc,
      const std::string& head_category_id) = 0;
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
  virtual const std::string add_product(
      const std::string& product_name,
      const std::string& product_price,
      const std::string& product_desc,
      const std::string& category_id) = 0;
  /**
   * @brief remove_category This function will remove a category specified by
   * client
   * @param category_id - Is the category_id that will be removed from db
   * @return  This function will return a succesfull message in case of removing
   * or an message that will contain an exception given by cvs_query
   */
  virtual const std::string remove_category(
      const std::string& category_id) = 0;
  /**
   * @brief remove_product This function will remove a category specified by
   * client
   * @param product_id - Is the category_id that will be removed from db
   * @return This function will return a succesfull message in case of removing
   * or an message that will contain an exception given by cvs_query
   */
  virtual const std::string remove_product(
      const std::string& product_id) = 0;
};
} // namespace interf
} // namespace db_manager
} // namespace app
#endif
