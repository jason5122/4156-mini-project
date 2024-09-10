# Bugs

## General

- Includes sometimes come before header guards.
- There are lots of duplicate imports.
  - `Course.{h,cpp}`
  - `Department.{h,cpp}`

## `class Course`

- `Course(...)` constructor initializes `enrolledStudentCount` to 500.
- `enrollStudent()` doesn't correctly return true/false.
- `dropStudent()` doesn't correctly return true/false.
- `getInstructorName()` and `getCourseTimeSlot()` are mixed up.
- `isCourseFull()`'s conditional is reversed.

## `class Department`

- `getDepartmentChair()` returns a fixed string instead of the member variable.
- `dropPersonFromMajor()` doesn't check if greater than 0 before decrementing.

## `class RouteController`

- All endpoints segfault if the required URL parameters aren't sent with the request.
- dropStudentFromCourse() doesn't have an endpoint.
- Some methods don't match their endpoint names.
  - `setCourseLocation()` -> /changeCourseLocation
  - `setCourseInstructor()` -> /changeCourseTeacher
  - `setCourseTime()` -> /changeCourseTime
- Unnecessary back and forth `std::stoi()`/`std::to_string()` casts for `courseCode`.

## `CourseUnitTests.cpp`

- `testCourse` is a global raw pointer which uses `new`/`delete`, which is very bad!
  This can easily be a variable on the stack instead.
