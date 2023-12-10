#include "Group.h"

Student::Student() {}

Student::~Student()
{
    this->student_name = "";
    this->student_id = 0;
}

// For Print Out Student Object
std::ostream &operator<<(std::ostream &os, const Student &student)
{
    os << "Name: " << student.student_name << " | ID: " << student.student_id;
    return os;
}

// For Print Out Student Object
std::ostream &operator<<(std::ostream &os, const Student *student)
{
    os << "Name: " << student->student_name << " | ID: " << student->student_id;
    return os;
}

// Function For Creating new Student Object
Student *newStudent(std::string name, size_t id)
{
    Student *s = new Student();
    s->student_name = name;
    s->student_id = id;
    return s;
}

// =====================================================================
// Group Area
Group::Group(std::string group_name, size_t group_id)
{
    if (group_id <= 0)
    {
        throw Exception("Group ID Cannot Below or Equal to 0");
    }

    this->group_name = group_name;
    this->group_id = group_id;
}

Group::~Group()
{
    Node<Student *> *head = this->group_students.getHead();
    while (head != nullptr)
    {
        Node<Student *> *temp = head;
        head = head->next;

        delete temp->data;
        temp->data = nullptr;

        // delete temp; // Deallocate the Node object
        // temp = nullptr;
    }
}

// Properties
// Get Properties
std::string Group::getGroupName()
{
    return this->group_name;
}

size_t Group::getGroupID()
{
    return this->group_id;
}

std::vector<Student> Group::getGroupStudentCopy()
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

// Setter
void Group::addStudent(Student *s)
{
    this->group_students.add(s);
}

void Group::changeGroupName(std::string group_name)
{
    this->group_name = group_name;
}

void Group::deleteStudent(unsigned int id)
{
    Student *tempStudent = new Student();
    tempStudent->student_id = id;

    this->group_students.remove([](Student *student_a, Student *student_b) -> bool
                                { return student_a->student_id == student_b->student_id; },
                                tempStudent);
}

void Group::changeStudentInfo(unsigned int id, std::string new_name, unsigned int new_id)
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

// Print Out Group 
std::ostream &operator<<(std::ostream &os, const Group &group)
{
    os << "ID: " << group.group_id << std::endl;
    os << "NAME: " << group.group_name << std::endl;
    os << "GROUP MEMBERS: " << std::endl;
    os << group.group_students << std::endl;
    return os;
}

std::ostream &operator<<(std::ostream &os, const Group *group)
{
    os << "ID: " << group->group_id << std::endl;
    os << "NAME: " << group->group_name << std::endl;
    os << "GROUP MEMBERS: " << std::endl;
    os << group->group_students << std::endl;
    return os;
}