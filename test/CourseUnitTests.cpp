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

TEST(CourseUnitTests, EnrollTest) {
    Course coms4156{10, "Gail Kaiser", "501 NWC", "10:10-11:25"};

    for (int i = 0; i < 10; i++) {
        EXPECT_FALSE(coms4156.isCourseFull());
        EXPECT_TRUE(coms4156.enrollStudent());
    }

    // Course is now full.
    EXPECT_TRUE(coms4156.isCourseFull());
    EXPECT_FALSE(coms4156.enrollStudent());
}

TEST(CourseUnitTests, DropTest) {
    Course coms4156{10, "Gail Kaiser", "501 NWC", "10:10-11:25"};

    EXPECT_FALSE(coms4156.isCourseFull());
    coms4156.setEnrolledStudentCount(10);
    EXPECT_TRUE(coms4156.isCourseFull());

    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(coms4156.dropStudent());
    }

    // Course is now empty.
    EXPECT_FALSE(coms4156.dropStudent());
}
