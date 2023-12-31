#ifndef GROUP_H
#define GROUP_H

#include "DataStructures/LinkedList.h"
#include "Exception.h"
#include <vector>
#include <string>

struct Student
{
    size_t student_id;
    std::string student_name;

    ~Student()
    {
        this->student_name = "";
        this->student_id = 0;
    }

    //* FOR DEBUG PURPOSE
    friend std::ostream &operator<<(std::ostream &os, const Student &student)
    {
        os << "Name: " << student.student_name << " | ID: " << student.student_id;

        return os;
    }

    //* FOR DEBUG PURPOSE
    friend std::ostream &
    operator<<(std::ostream &os, const Student *student)
    {
        os << "Name: " << student->student_name
           << " | ID: " << student->student_id;

        return os;
    }
};
Student *newStudent(std::string name, size_t id)
{
    Student *s = new Student();
    s->student_name = name;
    s->student_id = id;
    return s;
}
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
    std::string getGroupName() { return this->group_name; }
    size_t getGroupID() { return this->group_id; }
    std::vector<Student> getGroupStudentCopy()
    {
        std::vector<Student> listStudent = std::vector<Student>();
        Node<Student *> *head = this->group_students.getHead();

        while (head != nullptr)
        {
            unsigned int id = head->data->student_id;
            std::string name = head->data->student_name;

            Student student = Student();
            student.student_id = id;
            student.student_name = name;

            listStudent.push_back(student);

            head = head->next;
        }

        return listStudent;
    }

    //* SETTER
    void addStudent(Student *s) { this->group_students.add(s); }
    void changeGroupName(std::string group_name) { this->group_name = group_name; }
    void deleteStudent(unsigned int id)
    {
        Student *tempStudent = new Student();
        tempStudent->student_id = id;

        this->group_students.remove([](Student *student_a, Student *student_b) -> bool { 
            return student_a->student_id == student_b->student_id; 
            }, tempStudent);
    }

    void changeStudentInfo(unsigned int id, std::string new_name, unsigned int new_id)
    {
        Node<Student *> *head = this->group_students.getHead();

        while (head != nullptr)
        {
            if (head->data->student_id == id)
            {
                head->data->student_id = new_id;
                head->data->student_name = new_name;
                return;
            }
            head = head->next;
        }
    }

    //* DEBUG PURPOSE
    friend std::ostream &operator<<(std::ostream &os, const Group &group)
    {
        os << "ID: " << group.group_id << std::endl;
        os << "NAME: " << group.group_name << std::endl;
        os << "GROUP MEMBERS: " << std::endl;
        os << group.group_students << std::endl;
        return os;
    }

    friend std::ostream &operator<<(std::ostream &os, const Group *group)
    {
        os << "ID: " << group->group_id << std::endl;
        os << "NAME: " << group->group_name << std::endl;
        os << "GROUP MEMBERS: " << std::endl;
        os << group->group_students << std::endl;
        return os;
    }
};

Group::Group(std::string group_name, size_t group_id)
{
    if (group_id <= 0)
    {
        throw Exception("Group ID Cannot Below or Equal to 0");
    }

    this->group_name = group_name;
    this->group_id = group_id;
};

Group::~Group()
{
    Node<Student *> *head = this->group_students.getHead();
    while (head != nullptr)
    {
        Node<Student *> *temp = head;
        head = head->next;

        delete temp->data;
        temp->data = nullptr;

        delete temp; // Deallocate the Node object
        temp = nullptr;
    }
};

#endif