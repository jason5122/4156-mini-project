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

namespace {

std::string Fetch(const std::string& endpoint) {
    net::io_context ioc;
    tcp::resolver resolver{ioc};
    beast::tcp_stream stream{ioc};

    const auto host = "localhost";
    const auto port = "8080";

    const auto results = resolver.resolve(host, port);
    stream.connect(results);

    http::request<http::string_body> req{http::verb::get, endpoint, 10};
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
