// Copyright 2024 Jason Han
#include "Course.h"
#include <gtest/gtest.h>

TEST(CourseUnitTests, DefaultConstructorTest) {
    Course coms4156{};
    EXPECT_EQ(coms4156.getCourseLocation(), "");
    EXPECT_EQ(coms4156.getInstructorName(), "");
    EXPECT_EQ(coms4156.getCourseTimeSlot(), "");

    // Course should be empty.
    EXPECT_FALSE(coms4156.dropStudent());
}

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
    EXPECT_EQ(testCourse.display(), expectedResult);
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

TEST(CourseUnitTests, ReassignTest) {
    Course coms1004{10, "Gail Kaiser", "501 NWC", "10:10-11:25"};

    coms1004.reassignLocation("417 IAB");
    coms1004.reassignInstructor("Adam Cannon");
    coms1004.reassignTime("11:40-12:55");

    EXPECT_EQ(coms1004.getCourseLocation(), "417 IAB");
    EXPECT_EQ(coms1004.getInstructorName(), "Adam Cannon");
    EXPECT_EQ(coms1004.getCourseTimeSlot(), "11:40-12:55");
}

TEST(CourseUnitTests, EqualityTest) {
    Course c1{10, "Gail Kaiser", "501 NWC", "10:10-11:25"};
    Course c2{10, "Gail Kaiser", "501 NWC", "10:10-11:25"};
    EXPECT_EQ(c1, c2);
}

TEST(CourseUnitTests, InequalityTest) {
    Course c1{10, "Gail Kaiser", "501 NWC", "10:10-11:25"};
    Course c2{11, "Gail Kaiser", "501 NWC", "10:10-11:25"};
    EXPECT_NE(c1, c2);
}
