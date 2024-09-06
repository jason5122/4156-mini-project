// Copyright 2024 Jason Han
#include "RouteController.h"
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/lexical_cast.hpp>
#include <gtest/gtest.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// NOTE: Make sure the server is running before executing these tests!

namespace {

std::string MakeRequest(const std::string& endpoint, http::verb verb) {
    net::io_context ioc;
    tcp::resolver resolver{ioc};
    beast::tcp_stream stream{ioc};

    const auto host = "localhost";
    const auto port = "8080";

    const auto results = resolver.resolve(host, port);
    stream.connect(results);

    http::request<http::string_body> req{verb, endpoint, 10};
    req.set(http::field::host, host);

    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream, buffer, res);
    auto body = boost::lexical_cast<std::string>(res.body());

    beast::error_code ec;
    auto error_code [[maybe_unused]] = stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    return body;
}

std::string Fetch(const std::string& endpoint) {
    return MakeRequest(endpoint, http::verb::get);
}

std::string Patch(const std::string& endpoint) {
    return MakeRequest(endpoint, http::verb::patch);
}

}

TEST(RouteControllerUnitTests, RetrieveDepartmentTest) {
    const auto endpoint = "/retrieveDept?deptCode=COMS";
    auto body = Fetch(endpoint);
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
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, RetrieveCourseTest) {
    const auto endpoint = "/retrieveCourse?deptCode=COMS&courseCode=3134";
    auto body = Fetch(endpoint);
    auto expected = "\nInstructor: Brian Borowski; Location: 301 URIS; Time: 4:10-5:25";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, IsCourseFullTest) {
    const auto endpoint1 = "/isCourseFull?deptCode=ECON&courseCode=3211";
    auto body1 = Fetch(endpoint1);
    auto expected1 = "false";
    EXPECT_EQ(body1, expected1);

    const auto endpoint2 = "/isCourseFull?deptCode=IEOR&courseCode=2500";
    auto body2 = Fetch(endpoint2);
    auto expected2 = "true";
    EXPECT_EQ(body2, expected2);
}

TEST(RouteControllerUnitTests, GetMajorCountFromDeptTest) {
    const auto endpoint = "/getMajorCountFromDept?deptCode=COMS";
    auto body = Fetch(endpoint);
    auto expected = "There are: 2700 majors in the department";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, IdentifyDeptChairTest) {
    const auto endpoint = "/idDeptChair?deptCode=COMS";
    auto body = Fetch(endpoint);
    auto expected = "Luca Carloni is the department chair.";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, FindCourseLocationTest) {
    const auto endpoint = "/findCourseLocation?deptCode=COMS&courseCode=3203";
    auto body = Fetch(endpoint);
    auto expected = "301 URIS is where the course is located.";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, FindCourseInstructorTest) {
    const auto endpoint = "/findCourseInstructor?deptCode=COMS&courseCode=3203";
    auto body = Fetch(endpoint);
    auto expected = "Ansaf Salleb-Aouissi is the instructor for the course.";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, FindCourseTimeTest) {
    const auto endpoint = "/findCourseTime?deptCode=COMS&courseCode=3203";
    auto body = Fetch(endpoint);
    auto expected = "The course meets at: 10:10-11:25";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, AddMajorToDeptTest) {
    const auto endpoint = "/addMajorToDept?deptCode=COMS";
    auto body = Fetch(endpoint);
    auto expected = "Attribute was updated successfully";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, RemoveMajorFromDeptTest) {
    const auto endpoint = "/removeMajorFromDept?deptCode=COMS";
    auto body = Fetch(endpoint);
    auto expected = "Attribute was updated successfully";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, SetEnrollmentCountTest) {
    const auto endpoint = "/setEnrollmentCount?deptCode=COMS&courseCode=3203&count=42";
    auto body = Patch(endpoint);
    auto expected = "Attribute was updated successfully.";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, SetCourseLocationTest) {
    const auto endpoint =
        "/changeCourseLocation?deptCode=CHEM&courseCode=1500&location=402%20CHANDLER";
    auto body = Patch(endpoint);
    auto expected = "Attribute was updated successfully.";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, SetCourseInstructorTest) {
    const auto endpoint =
        "/changeCourseTeacher?deptCode=CHEM&courseCode=1500&instructor=Jae%20Lee";
    auto body = Patch(endpoint);
    auto expected = "Attribute was updated successfully.";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, SetCourseTimeTest) {
    const auto endpoint = "/changeCourseTime?deptCode=CHEM&courseCode=1500&time=10:10-11:25";
    auto body = Patch(endpoint);
    auto expected = "Attribute was updated successfully.";
    EXPECT_EQ(body, expected);
}

TEST(RouteControllerUnitTests, DropStudentFromCourseTest) {
    const auto endpoint = "/dropStudentFromCourse?deptCode=CHEM&courseCode=1500";
    auto body = Fetch(endpoint);
    auto expected = "Student has been dropped";
    EXPECT_EQ(body, expected);
}
