#ifndef GROUP_H
#define GROUP_H

#include "./DataStructures/LinkedList.h"
#include "Exception.h"
#include <vector>
#include <string>

struct Student
{
    size_t student_id;
    std::string student_name;

    Student();
    ~Student();

    //* FOR DEBUG PURPOSE
    friend std::ostream &operator<<(std::ostream &os, const Student &student);
    friend std::ostream &operator<<(std::ostream &os, const Student *student);
};

std::ostream &operator<<(std::ostream &os, const Student &student);
std::ostream &operator<<(std::ostream &os, const Student *student);

Student *newStudent(std::string name, size_t id);
class Group
{
private:
    size_t group_id;
    std::string group_name;
    LinkedList<Student *> group_students;

public:
    Group(std::string group_name, size_t group_id);
    ~Group();

    //* GETTER
    std::string getGroupName();
    size_t getGroupID();
    std::vector<Student> getGroupStudentCopy();

    //* SETTER
    void addStudent(Student *s);
    void changeGroupName(std::string group_name);
    void deleteStudent(unsigned int id);
    void changeStudentInfo(unsigned int id, std::string new_name, unsigned int new_id);

    //* DEBUG PURPOSE
    friend std::ostream &operator<<(std::ostream &os, const Group &group);
    friend std::ostream &operator<<(std::ostream &os, const Group *group);
};

std::ostream &operator<<(std::ostream &os, const Group &group);
std::ostream &operator<<(std::ostream &os, const Group *group);

#endif