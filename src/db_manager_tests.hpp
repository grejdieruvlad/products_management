#ifndef DB_MANAGER_TESTS_HPP
#define DB_MANAGER_TESTS_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "db_manager/real/db_manager.hpp"
#include "cvs_query/mock/mysql_result_set.hpp"
#include "cvs_query/fake/mysql_result_set.hpp"
#include "cvs_query/mock/mysql_system.hpp"
#include "cvs_query/mock/quick_connection.hpp"
#include "cvs_query/exception.hpp"
#include "cvs_query/utils.hpp"
/*
//get_all_category
TEST(db_manager, get_all_categories_failed)
{

  cvs_query::mysql_connection_info test_conn;
  ::std::shared_ptr<::cvs_query::fake::mysql_result_set> spm(
        new ::cvs_query::fake::mysql_result_set());
  ::std::shared_ptr<::cvs_query::mock::quick_connection> quick_connection =
      std::make_shared<::cvs_query::mock::quick_connection>();
  spm->initialize({{"1", "Auto","Category","NULL"},
                   {"2", "Drinks","Category","NULL"}});
  cvs_query::mock::mysql_system test_system;
  app::db_manager::real::db_manager test_db_manager(test_conn, test_system);
  test_db_manager.get_all_categories(test_system);

}

TEST(db_manager, remove_category_failed)
{
  cvs_query::mock::mysql_system test_system;
  cvs_query::mysql_connection_info test_conn;
  cvs_query::ptr_quick_connection test_quick_con;
  std::string result = "The product  was not added because an exception in "
                       "database\n";
  app::db_manager::real::db_manager test_db_manager(test_conn, test_system);
  EXPECT_CALL(test_system, get_new_connection(testing::_)).Times(1);
  ON_CALL(test_system, query(test_quick_con, testing::_)).
      WillByDefault(testing::Throw(*test_quick_con->error()));
  EXPECT_EQ(result, test_db_manager.remove_category("100"));

}
TEST(db_manager, remove_category_succesed)
{
  cvs_query::mock::mysql_system test_system;
  cvs_query::mysql_connection_info test_conn;
  cvs_query::mock::ptr_quick_connection test_quick_con;
  std::string result ="The category  was deleted successful"
                      " with all subcateories;" ;
  app::db_manager::real::db_manager test_db_manager(test_conn, test_system);
  EXPECT_CALL(test_system, get_new_connection(testing::_)).Times(1);
  EXPECT_CALL(test_quick_con.get(), query(testing::_)).Times(1);
  EXPECT_EQ(result,test_db_manager.remove_category("20"));
  //EXPECT_EQ(result, "The category  was deleted successful"
  //        " with all subcateories;" );
*/
#endif // DB_MANAGER_TESTS_HPP

