// Copyright 2024 Jason Han
#pragma once

#include "Course.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>

class Department {
public:
    Department(std::string deptCode, std::map<std::string, std::shared_ptr<Course>> courses,
               std::string departmentChair, int numberOfMajors);

    Department();

    int getNumberOfMajors() const;
    std::string getDepartmentChair() const;
    const std::map<std::string, std::shared_ptr<Course>>& getCourseSelection() const;
    std::string display() const;

    void addPersonToMajor();
    void dropPersonFromMajor();

    void addCourse(std::string courseId, std::shared_ptr<Course> course);
    void createCourse(std::string courseId, std::string instructorName, std::string courseLocation,
                      std::string courseTimeSlot, int capacity);

    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);

private:
    int numberOfMajors;
    std::string deptCode;
    std::string departmentChair;
    std::map<std::string, std::shared_ptr<Course>> courses;
};
