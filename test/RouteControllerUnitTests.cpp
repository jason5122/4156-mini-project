// Copyright 2024 Jason Han
#include "MyApp.h"
#include "RouteController.h"
#include <gtest/gtest.h>

namespace {

inline void SetUpDatabase(RouteController& routeController) {
    MyApp::run("setup");
    MyApp::onTermination();
    MyApp::run("run");

    crow::SimpleApp app;
    routeController.initRoutes(app);
    routeController.setDatabase(MyApp::getDatabase());
}

}

TEST(RouteControllerUnitTests, RetrieveDepartmentMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    auto expected = R"(COMS 1004: 
Instructor: Adam Cannon; Location: 417 IAB; Time: 11:40-12:55
COMS 3134: 
Instructor: Brian Borowski; Location: 301 URIS; Time: 4:10-5:25
COMS 3157: 
Instructor: Jae Lee; Location: 417 IAB; Time: 4:10-5:25
COMS 3203: 
Instructor: Ansaf Salleb-Aouissi; Location: 301 URIS; Time: 10:10-11:25
COMS 3251: 
Instructor: Tony Dear; Location: 402 CHANDLER; Time: 1:10-3:40
COMS 3261: 
Instructor: Josh Alman; Location: 417 IAB; Time: 2:40-3:55
COMS 3827: 
Instructor: Daniel Rubenstein; Location: 207 Math; Time: 10:10-11:25
COMS 4156: 
Instructor: Gail Kaiser; Location: 501 NWC; Time: 10:10-11:25
)";

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.retrieveDepartment(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, expected);

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT"};
    routeController.retrieveDepartment(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=42"};
    routeController.retrieveDepartment(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, RetrieveCourseMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS&courseCode=3134"};
    routeController.retrieveCourse(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "\nInstructor: Brian Borowski; Location: 301 URIS; Time: 4:10-5:25");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=COMS&courseCode=9999"};
    routeController.retrieveCourse(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.retrieveCourse(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");
}

TEST(RouteControllerUnitTests, IsCourseFullMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=ECON&courseCode=3211"};
    routeController.isCourseFull(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "false");

    res200.body = "";
    req200.url_params = crow::query_string{"?deptCode=IEOR&courseCode=2500"};
    routeController.isCourseFull(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "true");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT&courseCode=3211"};
    routeController.isCourseFull(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params = crow::query_string{"?deptCode=IEOR&courseCode=9999"};
    routeController.isCourseFull(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=10"};
    routeController.isCourseFull(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?deptCode=IEOR"};
    routeController.isCourseFull(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");
}

TEST(RouteControllerUnitTests, GetMajorCountFromDeptMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.getMajorCountFromDept(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "There are: 2700 majors in the department");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT"};
    routeController.getMajorCountFromDept(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=10"};
    routeController.getMajorCountFromDept(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, IdentifyDeptChairMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.identifyDeptChair(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Luca Carloni is the department chair.");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT"};
    routeController.identifyDeptChair(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=10"};
    routeController.identifyDeptChair(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, FindCourseLocationMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS&courseCode=3203"};
    routeController.findCourseLocation(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "301 URIS is where the course is located.");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT&courseCode=3203"};
    routeController.findCourseLocation(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params = crow::query_string{"?deptCode=COMS&courseCode=9999"};
    routeController.findCourseLocation(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=10"};
    routeController.findCourseLocation(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.findCourseLocation(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");
}

TEST(RouteControllerUnitTests, FindCourseInstructorMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS&courseCode=3203"};
    routeController.findCourseInstructor(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Ansaf Salleb-Aouissi is the instructor for the course.");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT&courseCode=3203"};
    routeController.findCourseInstructor(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params = crow::query_string{"?deptCode=COMS&courseCode=9999"};
    routeController.findCourseInstructor(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=10"};
    routeController.findCourseInstructor(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.findCourseInstructor(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");
}

TEST(RouteControllerUnitTests, FindCourseTimeMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS&courseCode=3203"};
    routeController.findCourseTime(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "The course meets at: 10:10-11:25");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT&courseCode=3203"};
    routeController.findCourseTime(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params = crow::query_string{"?deptCode=COMS&courseCode=9999"};
    routeController.findCourseTime(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=10"};
    routeController.findCourseTime(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.findCourseTime(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");
}

TEST(RouteControllerUnitTests, AddMajorToDeptMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.addMajorToDept(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Attribute was updated successfully");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT"};
    routeController.addMajorToDept(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=10"};
    routeController.addMajorToDept(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, RemoveMajorFromDeptMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.removeMajorFromDept(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Attribute was updated successfully");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT"};
    routeController.removeMajorFromDept(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?x=10"};
    routeController.removeMajorFromDept(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, SetEnrollmentCountMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=COMS&courseCode=3203&count=42"};
    routeController.setEnrollmentCount(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Attribute was updated successfully.");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT&courseCode=3203&count=42"};
    routeController.setEnrollmentCount(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params = crow::query_string{"?deptCode=COMS&courseCode=9999&count=42"};
    routeController.setEnrollmentCount(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?deptCode=COMS&courseCode=3203"};
    routeController.setEnrollmentCount(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include count");

    res400.body = "";
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.setEnrollmentCount(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?x=10"};
    routeController.setEnrollmentCount(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, SetCourseLocationMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params =
        crow::query_string{"?deptCode=CHEM&courseCode=1500&location=402%20CHANDLER"};
    routeController.setCourseLocation(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Attribute was updated successfully.");

    crow::request req404{};
    crow::response res404{};
    req404.url_params =
        crow::query_string{"?deptCode=NONEXISTENT&courseCode=3203&location=402%20CHANDLER"};
    routeController.setCourseLocation(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params =
        crow::query_string{"?deptCode=COMS&courseCode=9999&location=402%20CHANDLER"};
    routeController.setCourseLocation(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?deptCode=COMS&courseCode=3203"};
    routeController.setCourseLocation(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include location");

    res400.body = "";
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.setCourseLocation(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?x=10"};
    routeController.setCourseLocation(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, SetCourseInstructorMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=CHEM&courseCode=1500&instructor=Jae%20Lee"};
    routeController.setCourseInstructor(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Attribute was updated successfully.");

    crow::request req404{};
    crow::response res404{};
    req404.url_params =
        crow::query_string{"?deptCode=NONEXISTENT&courseCode=3203&instructor=Jae%20Lee"};
    routeController.setCourseInstructor(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params = crow::query_string{"?deptCode=COMS&courseCode=9999&instructor=Jae%20Lee"};
    routeController.setCourseInstructor(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?deptCode=COMS&courseCode=3203"};
    routeController.setCourseInstructor(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include instructor");

    res400.body = "";
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.setCourseInstructor(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?x=10"};
    routeController.setCourseInstructor(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, SetCourseTimeMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=CHEM&courseCode=1500&time=10:10-11:25"};
    routeController.setCourseTime(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Attribute was updated successfully.");

    crow::request req404{};
    crow::response res404{};
    req404.url_params =
        crow::query_string{"?deptCode=NONEXISTENT&courseCode=3203&time=10:10-11:25"};
    routeController.setCourseTime(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params = crow::query_string{"?deptCode=COMS&courseCode=9999&time=10:10-11:25"};
    routeController.setCourseTime(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?deptCode=COMS&courseCode=3203"};
    routeController.setCourseTime(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include time");

    res400.body = "";
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.setCourseTime(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?x=10"};
    routeController.setCourseTime(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}

TEST(RouteControllerUnitTests, DropStudentFromCourseMockTest) {
    RouteController routeController;
    SetUpDatabase(routeController);

    crow::request req200{};
    crow::response res200{};
    req200.url_params = crow::query_string{"?deptCode=CHEM&courseCode=1500"};
    routeController.dropStudentFromCourse(req200, res200);
    EXPECT_EQ(res200.code, 200);
    EXPECT_EQ(res200.body, "Student has been dropped");

    // Set enrollment to 0 for testing.
    crow::request req{};
    crow::response res{};
    req.url_params = crow::query_string{"?deptCode=CHEM&courseCode=1500&count=0"};
    routeController.setEnrollmentCount(req, res);

    res200.body = "";
    req200.url_params = crow::query_string{"?deptCode=CHEM&courseCode=1500"};
    routeController.dropStudentFromCourse(req200, res200);
    EXPECT_EQ(res200.code, 400);
    EXPECT_EQ(res200.body, "Student has not been dropped");

    crow::request req404{};
    crow::response res404{};
    req404.url_params = crow::query_string{"?deptCode=NONEXISTENT&courseCode=3203"};
    routeController.dropStudentFromCourse(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Department Not Found");

    res404.body = "";
    req404.url_params = crow::query_string{"?deptCode=COMS&courseCode=9999"};
    routeController.dropStudentFromCourse(req404, res404);
    EXPECT_EQ(res404.code, 404);
    EXPECT_EQ(res404.body, "Course Not Found");

    crow::request req400{};
    crow::response res400{};
    req400.url_params = crow::query_string{"?deptCode=COMS"};
    routeController.dropStudentFromCourse(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include courseCode");

    res400.body = "";
    req400.url_params = crow::query_string{"?x=10"};
    routeController.dropStudentFromCourse(req400, res400);
    EXPECT_EQ(res400.code, 400);
    EXPECT_EQ(res400.body, "URL parameters must include deptCode");
}
