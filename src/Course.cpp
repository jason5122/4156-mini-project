// Copyright 2024 Jason Han
#include "Course.h"
#include <iostream>
#include <string>

/**
 * Constructs a new Course object with the given parameters. Initial count starts at 0.
 *
 * @param instructorName     The name of the instructor teaching the course.
 * @param courseLocation     The location where the course is held.
 * @param timeSlot           The time slot of the course.
 * @param capacity           The maximum number of students that can enroll in the course.
 */
Course::Course(int capacity,
               const std::string& instructorName,
               const std::string& courseLocation,
               const std::string& timeSlot)
    : enrollmentCapacity(capacity),
      enrolledStudentCount(0),
      courseLocation(courseLocation),
      instructorName(instructorName),
      courseTimeSlot(timeSlot) {}

/**
 * Constructs a default Course object with the default parameters.
 */
Course::Course()
    : enrollmentCapacity(0),
      enrolledStudentCount(0),
      courseLocation(""),
      instructorName(""),
      courseTimeSlot("") {}

/**
 * Returns the course's location.
 *
 * @return The location as a string.
 */
std::string Course::getCourseLocation() const {
    return courseLocation;
}

/**
 * Returns the course's instructor.
 *
 * @return The instructor as a string.
 */
std::string Course::getInstructorName() const {
    return instructorName;
}

/**
 * Returns the course's time slot.
 *
 * @return The time slot as a string.
 */
std::string Course::getCourseTimeSlot() const {
    return courseTimeSlot;
}

/**
 * Returns the course info as a human-readable string.
 *
 * @return The display string.
 */
std::string Course::display() const {
    std::string str;
    str += "\nInstructor: " + instructorName;
    str += "; Location: " + courseLocation;
    str += "; Time: " + courseTimeSlot;
    return str;
}

/**
 * Returns whether or not the course is full.
 *
 * @return true if the course is full, false otherwise.
 */
bool Course::isCourseFull() const {
    return enrolledStudentCount >= enrollmentCapacity;
}

/**
 * Sets the enrollment count.
 *
 * @param count              The new count.
 */
void Course::setEnrolledStudentCount(int count) {
    enrolledStudentCount = count;
}

/**
 * Enrolls a student in the course if there is space available.
 *
 * @return true if the student is successfully enrolled, false otherwise.
 */
bool Course::enrollStudent() {
    if (!isCourseFull()) {
        enrolledStudentCount++;
        return true;
    } else {
        return false;
    }
}

/**
 * Drops a student from the course if a student is enrolled.
 *
 * @return true if the student is successfully dropped, false otherwise.
 */
bool Course::dropStudent() {
    if (enrolledStudentCount > 0) {
        enrolledStudentCount--;
        return true;
    } else {
        return false;
    }
}

/**
 * Assigns the course to a new location.
 *
 * @param newLocation        The new location.
 */
void Course::reassignLocation(const std::string& newLocation) {
    courseLocation = newLocation;
}

/**
 * Assigns the course to a new instructor.
 *
 * @param newInstructorName  The new instructor name.
 */
void Course::reassignInstructor(const std::string& newInstructorName) {
    instructorName = newInstructorName;
}

/**
 * Assigns the course to a new time slot.
 *
 * @param newTime            The new time slot.
 */
void Course::reassignTime(const std::string& newTime) {
    courseTimeSlot = newTime;
}

/**
 * Serializes the course as a binary format.
 *
 * @param out                The out stream to write to.
 */
void Course::serialize(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&enrollmentCapacity), sizeof(enrollmentCapacity));
    out.write(reinterpret_cast<const char*>(&enrolledStudentCount), sizeof(enrolledStudentCount));

    size_t locationLen = courseLocation.length();
    out.write(reinterpret_cast<const char*>(&locationLen), sizeof(locationLen));
    out.write(courseLocation.c_str(), locationLen);

    size_t instructorLen = instructorName.length();
    out.write(reinterpret_cast<const char*>(&instructorLen), sizeof(instructorLen));
    out.write(instructorName.c_str(), instructorLen);

    size_t timeSlotLen = courseTimeSlot.length();
    out.write(reinterpret_cast<const char*>(&timeSlotLen), sizeof(timeSlotLen));
    out.write(courseTimeSlot.c_str(), timeSlotLen);
}

/**
 * De-serializes the course from a binary format, setting this object's members accordingly.
 *
 * @param in                 The in stream to read from.
 */
void Course::deserialize(std::istream& in) {
    in.read(reinterpret_cast<char*>(&enrollmentCapacity), sizeof(enrollmentCapacity));
    in.read(reinterpret_cast<char*>(&enrolledStudentCount), sizeof(enrolledStudentCount));

    size_t locationLen;
    in.read(reinterpret_cast<char*>(&locationLen), sizeof(locationLen));
    courseLocation.resize(locationLen);
    in.read(&courseLocation[0], locationLen);

    size_t instructorLen;
    in.read(reinterpret_cast<char*>(&instructorLen), sizeof(instructorLen));
    instructorName.resize(instructorLen);
    in.read(&instructorName[0], instructorLen);

    size_t timeSlotLen;
    in.read(reinterpret_cast<char*>(&timeSlotLen), sizeof(timeSlotLen));
    courseTimeSlot.resize(timeSlotLen);
    in.read(&courseTimeSlot[0], timeSlotLen);
}

/**
 * Checks if this course is equal to another course.
 *
 * @param rhs                The right hand side Course object to compare to.
 */
bool Course::operator==(const Course& rhs) const {
    return enrollmentCapacity == rhs.enrollmentCapacity &&
           enrolledStudentCount == rhs.enrolledStudentCount &&
           courseLocation == rhs.courseLocation && instructorName == rhs.instructorName &&
           courseTimeSlot == rhs.courseTimeSlot;
}

/**
 * Checks if this course is *not* equal to another course.
 *
 * @param rhs                The right hand side Course object to compare to.
 */
bool Course::operator!=(const Course& rhs) const {
    return !operator==(rhs);
}
