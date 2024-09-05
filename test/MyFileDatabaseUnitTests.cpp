// Copyright 2024 Jason Han
#include "MyFileDatabase.h"
#include <gtest/gtest.h>

TEST(MyFileDatabaseUnitTests, SerializeDeserializeTest) {
    // Serialize.
    MyFileDatabase serialize_db{1, "testfile.bin"};

    std::map<std::string, Department> mapping;
    auto coms1004 = std::make_shared<Course>(400, "Adam Cannon", "417 IAB", "11:40-12:55");
    coms1004->setEnrolledStudentCount(249);
    std::map<std::string, std::shared_ptr<Course>> courses;
    courses["1004"] = coms1004;
    Department coms("COMS", courses, "Luca Carloni", 2700);
    mapping["COMS"] = coms;

    serialize_db.setMapping(mapping);
    serialize_db.saveContentsToFile();

    // Deserialize.
    MyFileDatabase deserialize_db{0, "testfile.bin"};
    auto deserialized_mapping = deserialize_db.getDepartmentMapping();

    EXPECT_EQ(deserialized_mapping, mapping);
}

TEST(MyFileDatabaseUnitTests, DisplayTest) {
    MyFileDatabase db{1, "testfile.bin"};

    std::map<std::string, Department> mapping;
    auto coms1004 = std::make_shared<Course>(400, "Adam Cannon", "417 IAB", "11:40-12:55");
    coms1004->setEnrolledStudentCount(249);
    std::map<std::string, std::shared_ptr<Course>> courses;
    courses["1004"] = coms1004;
    Department coms("COMS", courses, "Luca Carloni", 2700);
    mapping["COMS"] = coms;

    db.setMapping(mapping);

    EXPECT_EQ(db.display(), "For the COMS department:\nCOMS 1004: \nInstructor: Adam Cannon; "
                            "Location: 417 IAB; Time: 11:40-12:55\n\n");
}
