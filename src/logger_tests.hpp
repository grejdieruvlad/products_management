#ifndef LOGGER_TEST_HPP
#define LOGGER_TEST_HPP
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <cvs_util/file/mock/file.hpp>
#include <cvs_util/timer/mock/timer.hpp>
#include "log_manager/real/logger.hpp"

TEST(logger, unsuccesful_log)
{
  cvs_util::file::mock::file test_file;
  cvs_util::timer::mock::timer test_timer;
  app::logger::real::logger logger(test_file, test_timer);
  EXPECT_CALL(test_file,open(testing::_,testing::_)).Times(1);
  EXPECT_CALL(test_file,is_open()).Times(1).WillOnce(testing::Return(false));
  EXPECT_FALSE(logger.log(""));
}
TEST(logger, succesful_log)
{
  cvs_util::file::mock::file test_file;
  cvs_util::timer::mock::timer test_timer;
  app::logger::real::logger logger(test_file, test_timer);
  EXPECT_CALL(test_file, open(testing::_, testing::_)).Times(1);
  EXPECT_CALL(test_file, is_open()).Times(1).WillOnce(testing::Return(true));
  EXPECT_CALL(test_file, write(testing::_, testing::_)).Times(2).
      WillRepeatedly(testing::ReturnRef(test_file));
  EXPECT_CALL(test_file, put(testing::_)).Times(1).
      WillOnce(testing::ReturnRef(test_file));
  EXPECT_CALL(test_file, close()).Times(1);
  EXPECT_TRUE(logger.log(""));
}
#endif // LOGGER_TEST_HPP
