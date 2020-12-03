#ifndef DB_MANAGER_MOCK_HPP
#define DB_MANAGER_MOCK_HPP
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../interf/db_manager.hpp"
namespace app
{
namespace db_manager
{
namespace mock
{
/**
 * @brief The db_manager class describe the mock implementation of db_manager
 * interface
 */
class db_manager: public interf::db_manager
{
public:
  /**
   * @brief MOCK_METHOD - Mock implementation for get_all_categories method
   */
  MOCK_METHOD(const std::string, get_all_categories, ());
  /**
   * @brief MOCK_METHOD - Mock implementation for get_categories_of_category
   *  method
   */
  MOCK_METHOD( const std::string, get_categories_of_category,
               (const std::string& ));
  /**
    *@brief  MOCK_METHOD - Mock implementation for get_products_of_category
    *  method
    */
  MOCK_METHOD( const std::string , get_products_of_category,
               (const std::string& ));
  /**
    *@brief  MOCK_METHOD - Mock implementation for add_category method
    */
  MOCK_METHOD(const std::string, add_category, (
                const std::string &,const std::string&, const std::string&));
  /**
    *@brief  MOCK_METHOD - Mock implementation for add_product method
    */
  MOCK_METHOD(const std::string, add_product,(
                const std::string&, const std::string& , const std::string&,
                const std::string&));
  /**
    *@brief MOCK_METHOD - Mock implementation for remove_category method
    */
  MOCK_METHOD( const std::string, remove_category, ());
  /**
    *@brief  MOCK_METHOD - Mock implementation for remove_product method
    */
  MOCK_METHOD(const std::string ,remove_product, ());
};
} // namespace mock
} // namespace db_manager
} // namespace app
#endif
