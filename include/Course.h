// Copyright 2024 Jason Han
#pragma once

#include <string>

class Course {
public:
    Course(int count, const std::string& instructorName, const std::string& courseLocation,
           const std::string& timeSlot);
    Course();

    std::string getCourseLocation() const;
    std::string getInstructorName() const;
    std::string getCourseTimeSlot() const;
    std::string display() const;

    bool isCourseFull() const;
    void setEnrolledStudentCount(int count);
    bool enrollStudent();
    bool dropStudent();

    void reassignLocation(const std::string& newLocation);
    void reassignInstructor(const std::string& newInstructorName);
    void reassignTime(const std::string& newTime);

    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);

    bool operator==(const Course& rhs) const;
    bool operator!=(const Course& rhs) const;

private:
    int enrollmentCapacity;
    int enrolledStudentCount;
    std::string courseLocation;
    std::string instructorName;
    std::string courseTimeSlot;
};
