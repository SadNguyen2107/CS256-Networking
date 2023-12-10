#ifndef PROJECT_H
#define PROJECT_H

#include "Date.h"
#include <vector>

// Use Vector to hold the number of Groups
class Project
{
private:
    std::string description;
    Date *due_date = nullptr;
    std::vector<Date *> submission_dates;

public:
    Project(std::string group_description, Date *due_date);
    ~Project();

    //* GETTER
    std::string getDescription();
    Date *getDueDate();
    std::vector<Date *> getSubmissionDateCopy();

    //* SETTER
    void setDescription(std::string new_description);
    void setDueDate(Date *new_due_date);

    //* UPDATE SUBMISSION DATE
    void addSubmissionDate(std::vector<Date *>::size_type groupID, Date *group_subbmit_date);

    //* DEBUG PURPOSE
    friend std::ostream &operator<<(std::ostream &os, const Project &project);
    friend std::ostream &operator<<(std::ostream &os, const Project *project);

    // Resize Method
    friend void resizeSubmissionDates(Project *project, int newSize);
};

void resizeSubmissionDates(Project *project, int newSize);
std::ostream &operator<<(std::ostream &os, const Project &project);
std::ostream &operator<<(std::ostream &os, const Project *project);

#endif