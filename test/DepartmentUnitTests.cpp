// Copyright 2024 Jason Han
#include "Department.h"
#include <gtest/gtest.h>

TEST(DepartmentUnitTests, GetterTest) {
    std::string times[] = {"11:40-12:55", "4:10-5:25", "10:10-11:25", "2:40-3:55"};

    std::map<std::string, std::shared_ptr<Course>> courses;
    auto ieor2500 = std::make_shared<Course>(50, "Uday Menon", "627 MUDD", times[0]);
    ieor2500->setEnrolledStudentCount(52);
    auto ieor3404 = std::make_shared<Course>(73, "Christopher J Dolan", "303 MUDD", times[2]);
    ieor3404->setEnrolledStudentCount(80);
    auto ieor3658 = std::make_shared<Course>(96, "Daniel Lacker", "310 FAY", times[2]);
    courses["2500"] = ieor2500;
    courses["3404"] = ieor3404;
    courses["3658"] = ieor3658;

    Department ieor("IEOR", courses, "Jay Sethuraman", 67);
    EXPECT_EQ(ieor.getNumberOfMajors(), 67);
    EXPECT_EQ(ieor.getDepartmentChair(), "Jay Sethuraman");
    EXPECT_EQ(ieor.getCourseSelection(), courses);
}

TEST(DepartmentUnitTests, DisplayTest) {
    std::string times[] = {"11:40-12:55", "4:10-5:25", "10:10-11:25", "2:40-3:55"};

    std::map<std::string, std::shared_ptr<Course>> courses;
    auto ieor2500 = std::make_shared<Course>(50, "Uday Menon", "627 MUDD", times[0]);
    ieor2500->setEnrolledStudentCount(52);
    auto ieor3404 = std::make_shared<Course>(73, "Christopher J Dolan", "303 MUDD", times[2]);
    ieor3404->setEnrolledStudentCount(80);
    auto ieor3658 = std::make_shared<Course>(96, "Daniel Lacker", "310 FAY", times[2]);
    courses["2500"] = ieor2500;
    courses["3404"] = ieor3404;
    courses["3658"] = ieor3658;

    Department ieor("IEOR", courses, "Jay Sethuraman", 67);

    std::string expectedResult =
        "IEOR 2500: \nInstructor: Uday Menon; Location: 627 MUDD; Time: 11:40-12:55\n"
        "IEOR 3404: \nInstructor: Christopher J Dolan; Location: 303 MUDD; Time: 10:10-11:25\n"
        "IEOR 3658: \nInstructor: Daniel Lacker; Location: 310 FAY; Time: 10:10-11:25\n";
    EXPECT_EQ(ieor.display(), expectedResult);
}

TEST(DepartmentUnitTests, AddDropTest) {
    std::map<std::string, std::shared_ptr<Course>> courses;
    Department ieor("IEOR", courses, "Jay Sethuraman", 20);
    EXPECT_EQ(ieor.getNumberOfMajors(), 20);

    for (int i = 1; i <= 10; i++) {
        ieor.addPersonToMajor();
        EXPECT_EQ(ieor.getNumberOfMajors(), 20 + i);
    }
    for (int i = 30; i >= 1; i--) {
        EXPECT_EQ(ieor.getNumberOfMajors(), i);
        ieor.dropPersonFromMajor();
    }
    EXPECT_EQ(ieor.getNumberOfMajors(), 0);

    // Ensure number of majors doesn't go below 0.
    ieor.dropPersonFromMajor();
    EXPECT_EQ(ieor.getNumberOfMajors(), 0);
}
