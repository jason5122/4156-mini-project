// Copyright 2024 Jason Han
#include "Department.h"
#include <sstream>

/**
 * Constructs a new Department object with the given parameters.
 *
 * @param deptCode         The code of the department.
 * @param courses          A HashMap containing courses offered by the department.
 * @param departmentChair  The name of the department chair.
 * @param numberOfMajors   The number of majors in the department.
 */
Department::Department(std::string deptCode,
                       std::map<std::string, std::shared_ptr<Course>> courses,
                       std::string departmentChair,
                       int numberOfMajors)
    : numberOfMajors(numberOfMajors),
      deptCode(deptCode),
      departmentChair(departmentChair),
      courses(courses) {}

Department::Department() : numberOfMajors(0) {}

/**
 * Gets the number of majors in the department.
 *
 * @return The number of majors.
 */
int Department::getNumberOfMajors() const {
    return numberOfMajors;
}

/**
 * Gets the name of the department chair.
 *
 * @return The name of the department chair.
 */
std::string Department::getDepartmentChair() const {
    return departmentChair;
}

/**
 * Gets the courses offered by the department.
 *
 * @return A HashMap containing courses offered by the department.
 */
const std::map<std::string, std::shared_ptr<Course>>& Department::getCourseSelection() const {
    return courses;
}

/**
 * Returns a string representation of the department, including its code and the courses offered.
 *
 * @return A string representing the department.
 */
std::string Department::display() const {
    std::ostringstream result;
    for (const auto& it : courses) {
        result << deptCode << " " << it.first << ": " << it.second->display() << "\n";
    }
    return result.str();
}

/**
 * Increases the number of majors in the department by one.
 */
void Department::addPersonToMajor() {
    numberOfMajors++;
}

/**
 * Decreases the number of majors in the department by one if it's greater than zero.
 */
void Department::dropPersonFromMajor() {
    if (numberOfMajors > 0) {
        numberOfMajors--;
    }
}

/**
 * Adds a new course to the department's course selection.
 *
 * @param courseId The ID of the course to add.
 * @param course   The Course object to add.
 */
void Department::addCourse(std::string courseId, std::shared_ptr<Course> course) {
    courses[courseId] = course;
}

/**
 * Creates and adds a new course to the department's course selection.
 *
 * @param courseId           The ID of the new course.
 * @param instructorName     The name of the instructor teaching the course.
 * @param courseLocation     The location where the course is held.
 * @param courseTimeSlot     The time slot of the course.
 * @param capacity           The maximum number of students that can enroll in the course.
 */
void Department::createCourse(std::string courseId,
                              std::string instructorName,
                              std::string courseLocation,
                              std::string courseTimeSlot,
                              int capacity) {
    std::shared_ptr<Course> newCourse =
        std::make_shared<Course>(capacity, instructorName, courseLocation, courseTimeSlot);
    addCourse(courseId, newCourse);
}

/**
 * Serializes the department as a binary format.
 *
 * @param out                The out stream to write to.
 */
void Department::serialize(std::ostream& out) const {
    size_t deptCodeLen = deptCode.length();
    out.write(reinterpret_cast<const char*>(&deptCodeLen), sizeof(deptCodeLen));
    out.write(deptCode.c_str(), deptCodeLen);

    size_t chairLen = departmentChair.length();
    out.write(reinterpret_cast<const char*>(&chairLen), sizeof(chairLen));
    out.write(departmentChair.c_str(), chairLen);

    out.write(reinterpret_cast<const char*>(&numberOfMajors), sizeof(numberOfMajors));

    size_t mapSize = courses.size();
    out.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    for (const auto& it : courses) {
        size_t courseIdLen = it.first.length();
        out.write(reinterpret_cast<const char*>(&courseIdLen), sizeof(courseIdLen));
        out.write(it.first.c_str(), courseIdLen);
        it.second->serialize(out);
    }
}

/**
 * De-serializes the department from a binary format, setting this object's members accordingly.
 *
 * @param in                 The in stream to read from.
 */
void Department::deserialize(std::istream& in) {
    size_t deptCodeLen;
    in.read(reinterpret_cast<char*>(&deptCodeLen), sizeof(deptCodeLen));
    deptCode.resize(deptCodeLen);
    in.read(&deptCode[0], deptCodeLen);

    size_t chairLen;
    in.read(reinterpret_cast<char*>(&chairLen), sizeof(chairLen));
    departmentChair.resize(chairLen);
    in.read(&departmentChair[0], chairLen);

    in.read(reinterpret_cast<char*>(&numberOfMajors), sizeof(numberOfMajors));

    size_t mapSize;
    in.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    for (size_t i = 0; i < mapSize; ++i) {
        size_t courseIdLen;
        in.read(reinterpret_cast<char*>(&courseIdLen), sizeof(courseIdLen));
        std::string courseId(courseIdLen, ' ');
        in.read(&courseId[0], courseIdLen);
        std::shared_ptr<Course> course = std::make_shared<Course>();
        course->deserialize(in);
        courses[courseId] = course;
    }
}

/**
 * Checks if this department is equal to another department. We perform deep equality here,
 * comparing each course in `courses` by value.
 *
 * @param rhs                The right hand side Department object to compare to.
 */
bool Department::operator==(const Department& rhs) const {
    // Compare courses by value, not by pointer address.
    for (const auto& [courseId, course] : courses) {
        if (!rhs.courses.count(courseId)) {
            return false;
        }
        if (*course.get() != *rhs.courses.at(courseId).get()) {
            return false;
        }
    }
    return numberOfMajors == rhs.numberOfMajors && deptCode == rhs.deptCode &&
           departmentChair == rhs.departmentChair;
}

/**
 * Checks if this department is *not* equal to another department. We perform deep equality here,
 * comparing each course in `courses` by value.
 *
 * @param rhs                The right hand side Department object to compare to.
 */
bool Department::operator!=(const Department& rhs) const {
    return !operator==(rhs);
}
