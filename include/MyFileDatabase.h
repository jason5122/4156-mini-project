// Copyright 2024 Jason Han
#ifndef MYFILEDATABASE_H
#define MYFILEDATABASE_H

#include "Department.h"
#include <map>
#include <string>

class MyFileDatabase {
public:
    MyFileDatabase(int flag, const std::string& filePath);

    void setMapping(const std::map<std::string, Department>& mapping);
    void saveContentsToFile() const;
    void deSerializeObjectFromFile();

    std::map<std::string, Department> getDepartmentMapping() const;
    std::string display() const;

private:
    std::map<std::string, Department> departmentMapping;
    std::string filePath;
};

#endif
