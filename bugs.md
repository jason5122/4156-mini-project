# Bugs

## General

- Includes sometimes come before header guards. (clang-tidy: `llvm-header-guard`)
- There are lots of duplicate imports. (clang-tidy: `readability-duplicate-include`)
  - `Course.{h,cpp}`
  - `Department.{h,cpp}`

### `class Course`

- `Course(...)` constructor initializes `enrolledStudentCount` to 500.
- `enrollStudent()` doesn't correctly return true/false.
- `dropStudent()` doesn't correctly return true/false.
- `getInstructorName()` and `getCourseTimeSlot()` are mixed up.
- `isCourseFull()`'s conditional is reversed.

### `class Department`

- `getDepartmentChair()` returns a fixed string instead of the member variable.
- `dropPersonFromMajor()` doesn't check if greater than 0 before decrementing.

### `class RouteController`

- All endpoints segfault if the required URL parameters aren't sent with the request.
- `dropStudentFromCourse()` doesn't have an endpoint.
- Some methods don't match their endpoint names.
  - `setCourseLocation()` -> /changeCourseLocation
  - `setCourseInstructor()` -> /changeCourseTeacher
  - `setCourseTime()` -> /changeCourseTime
- Unnecessary back and forth `std::stoi()`/`std::to_string()` casts for `courseCode`.

## Warnings, Style, Readability, etc.

- Method declaration and definition have different parameter names. (clang-tidy: `readability-inconsistent-declaration-parameter-name`)
  - `Course(int count, ...)` versus `Course::Course(int capacity, ...)`
- Constructor initialization lists are sometimes out of order. (`-Wreorder`)

### `CourseUnitTests.cpp`

- `testCourse` is a global raw pointer which uses `new`/`delete`, which is very bad!
  This can easily be a variable on the stack instead.
