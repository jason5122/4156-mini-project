// Copyright 2024 Jason Han
#include "Course.h"
#include <gtest/gtest.h>

TEST(CourseUnitTests, GetterTest) {
    Course coms4156{120, "Gail Kaiser", "501 NWC", "10:10-11:25"};
    EXPECT_EQ(coms4156.getCourseLocation(), "501 NWC");
    EXPECT_EQ(coms4156.getInstructorName(), "Gail Kaiser");
    EXPECT_EQ(coms4156.getCourseTimeSlot(), "10:10-11:25");
}

TEST(CourseUnitTests, DisplayTest) {
    Course testCourse{250, "Griffin Newbold", "417 IAB", "11:40-12:55"};
    std::string expectedResult =
        "\nInstructor: Griffin Newbold; Location: 417 IAB; Time: 11:40-12:55";
    EXPECT_EQ(expectedResult, testCourse.display());
}
