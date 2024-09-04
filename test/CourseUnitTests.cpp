// Copyright 2024 Jason Han
#include "Course.h"
#include <gtest/gtest.h>

TEST(CourseUnitTests, ToStringTest) {
    Course testCourse{250, "Griffin Newbold", "417 IAB", "11:40-12:55"};
    std::string expectedResult =
        "\nInstructor: Griffin Newbold; Location: 417 IAB; Time: 11:40-12:55";
    ASSERT_EQ(expectedResult, testCourse.display());
}
