#include <iostream>
#include "db_manager.hpp"
#include <iomanip>
#include <cvs_query/exception.hpp>

app::db_manager::real::db_manager::db_manager(
    cvs_query::mysql_connection_info& _my_connection_info,
    cvs_query::interf::mysql_system& _my_system) :
  mysql_connection_info(_my_connection_info), my_system(_my_system){ }


const std::string app::db_manager::real::db_manager::get_all_categories()
{
  std::string query_result {"+-------------+---------------"
                            "+-----------------"
                            "------+------------------+\n| CATEGORY_ID | "
                            "CATEGORY_NAME | CATEGORY_DESC         | "
                            "HEAD_CATEGORY_ID |\n+-------------+-------------"
                            "--+-----------------------+------------------+\n"
                           };
  cvs_query::interf::ptr_mysql_result_set res;
  try
  {
    res = my_system.query_with_result(mysql_connection_info,
                                      "Select * from categories");
    cvs_query::result::row row;
    std::vector<std::string> tmp_vect;
    while(true)
    {
      row = res->get_row();
      if(res.get()->num_rows() ==  0)
      {
        query_result = "There are no categories records into table!";
        break;
      }
      if(row.is_null())
      {
        break;
      }
      else
      {
        query_result.append(row[0]);
        query_result.append("        ");
        query_result.append(row[1]);
        query_result.append("        ");
        query_result.append(row[2]);
        query_result.append("        ");
        query_result.append(row[3]);
        query_result.append("        ");
        query_result.append("\n");
      }
    }
  }
  catch (cvs_query::exception e)
  {
    query_result = "Error while executing database select query!\n";
  }
  return query_result;
}

const std::string app::db_manager::real::db_manager::
get_categories_of_category(const std::string &category_id)
{
  cvs_query::interf::ptr_mysql_result_set res;
  std::string sqlQuery = "SELECT CATEGORY_ID, CATEGORY_NAME, CATEGORY_DESC,"
                         "HEAD_CATEGORY_ID FROM categories WHERE "
                         "HEAD_CATEGORY_ID = " + category_id + ";" ;
  std::string query_result {"+-------------+---------------+-----------------"
                            "------+------------------+\n| CATEGORY_ID | "
                            "CATEGORY_NAME | CATEGORY_DESC         | "
                            "HEAD_CATEGORY_ID |\n+-------------+-------------"
                            "--+-----------------------+------------------+\n"
                           };
  try
  {
    res = my_system.query_with_result(mysql_connection_info, sqlQuery);
    cvs_query::result::row row;
    std::vector<std::string> tmp_vect;
    while(true)
    {
      row = res->get_row();
      if(res.get()->num_rows() ==  0)
      {
        query_result = "There are no categories records into table for this "
                       "Parent category!\n";
        break;
      }
      if(row.is_null())
      {
        break;
      }
      else
      {
        query_result.append(row[0]);
        query_result.append("        ");
        query_result.append(row[1]);
        query_result.append("        ");
        query_result.append(row[2]);
        query_result.append("        ");
        query_result.append(row[3]);
        query_result.append("        ");
        query_result.append("\n");
      }
    }
  }
  catch (cvs_query::exception e)
  {
    query_result = "Error while executing database select query!\n";
  }
  return query_result;
}

const std::string app::db_manager::real::db_manager::
get_products_of_category(const std::string &category_id)
{
  cvs_query::interf::ptr_mysql_result_set res;
  std::string sqlQuery = "SELECT * FROM products WHERE "
                         " products.CATEGORY_ID = " + category_id +" ;";
  std::string query_result {"+-------------+---------------"
                            "+---------------+-----------------"
                            "------+------------------+\n| PRODUCT_ID "
                            " | PRODUCT_NAME  | PRODUCT_PRICE | CATEGORY_DESC"
                            "         | HEAD_CATEGORY_ID |\n+-------------+--"
                            "-------------+---------------+--------------------"
                            "---+------------------+\n"};
  try
  {
    res = my_system.query_with_result(mysql_connection_info, sqlQuery);
    cvs_query::result::row row;
    std::vector<std::string> tmp_vect;
    while(true)
    {
      row = res->get_row();
      if(res.get()->num_rows() ==  0)
      {
        query_result = "There are no products from category: " + category_id+
            "\n";
        break;
      }
      if(row.is_null())
      {
        break;
      }
      else
      {
        query_result.append(row[0]);
        query_result.append("              ");
        query_result.append(row[1]);
        query_result.append("        ");
        query_result.append(row[2]);
        query_result.append("        ");
        query_result.append(row[3]);
        query_result.append("        ");
        query_result.append(row[4]);
        query_result.append("        ");
        query_result.append("\n");
      }
    }
  }
  catch (cvs_query::exception e)
  {
    query_result = "Error while executing database select query!\n";
  }
  return query_result;

}
const std::string app::db_manager::real::db_manager::add_category(
    const std::string& category_name,
    const std::string& category_desc,
    const std::string& head_category_id)
{
  cvs_query::ptr_quick_connection quick_conn;
  quick_conn = my_system.get_new_connection(mysql_connection_info);
  std::string sqlQuery = "INSERT INTO categories(CATEGORY_NAME, "
                         "CATEGORY_DESC, HEAD_CATEGORY_ID) VALUES ('"
      + category_name + "','"
      + category_desc + "',"
      + head_category_id + ");";
  std::string query_result;
  quick_conn->query(sqlQuery.c_str());
  const char * s {"\0\0\0\0\0\0\0\0"};
  if(*quick_conn->error() != *s)
  {
    query_result = "The category '" + category_name + "' was not added because"
                                                      " an exception in database"
                                                      "\n";
  }
  else
  {
    query_result = "The category  '" + category_name + "' was added successful"
                                                       "\n" ;
  }
  return query_result;
}
const std::string app::db_manager::real::db_manager::add_product(
    const std::string& product_name,
    const std::string& product_price,
    const std::string& product_desc,
    const std::string& category_id)
{
  cvs_query::ptr_quick_connection quick_conn;
  quick_conn =my_system.get_new_connection(mysql_connection_info);
  std::string sqlQuery = "INSERT INTO products(PRODUCT_NAME, PRODUCT_PRICE, "
                         "PRODUCT_DESC, CATEGORY_ID ) VALUES ('"
      + product_name + "',"
      + product_price + ",'"
      + product_desc+"','"
      + category_id+"');";
  quick_conn->query(sqlQuery.c_str());
  std::string query_result ;
  const char * s {"\0\0\0\0\0\0\0\0"};
  if(*quick_conn->error() != *s)
  {
    query_result = "The product  was not added because an exception in database"
                   "\n";
  }
  else
  {
    query_result = "The product  was added successful\n" ;
  }
  return query_result;

}
const std::string app::db_manager::real::db_manager::remove_category(
    const std::string &category_id)
{
  cvs_query::ptr_quick_connection quick_conn;
  quick_conn = my_system.get_new_connection(mysql_connection_info);
  std::string query_result ;
  const std::string sqlQuery  = "DELETE FROM categories WHERE CATEGORY_ID = " +
      category_id + ";";
  quick_conn->query(sqlQuery.c_str());
  const char * s {"\0\0\0\0\0\0\0\0"};
  if(*quick_conn->error() != *s)
  {
    query_result = "The category was not deleted because an"
                   " exception in database\n";
  }
  else
  {
    if(quick_conn->affected_rows() == 1)
    {
      query_result = "The category  was deleted successful"
                     " with all subcateories;" ;
    }
    else
    {
      query_result = "The category with  does'n exist!" ;
    }
  }
  quick_conn->close();
  return query_result;
}

const std::string app::db_manager::real::db_manager::remove_product(
    const std::string &product_id)
{
  cvs_query::ptr_quick_connection quick_conn;
  quick_conn = my_system.get_new_connection(mysql_connection_info);
  std::string query_result;
  const std::string sqlQuery  = "DELETE FROM products WHERE PRODUCT_ID = " +
      product_id + ";";
  quick_conn->query(sqlQuery.c_str());
  const char * s {"\0\0\0\0\0\0\0\0"};
  if(*quick_conn->error() != *s)
  {
    query_result = "The product " + product_id + " was not deleted because an"
                                                 " exception in database\n";
  }
  else
  {
    if(quick_conn->affected_rows() == 1)
    {
      query_result = "The product  " + product_id + " was deleted successful" ;
    }
    else
    {
      query_result = "The product with " + product_id + " does'n exist!" ;
    }
  }
  quick_conn->close();
  return query_result;
}
app::db_manager::real::db_manager::~db_manager(){}
