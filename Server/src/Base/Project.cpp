#include "Project.h"

Project::Project(std::string group_description, Date *due_date)
{
    this->description = group_description;
    this->due_date = due_date;
}

Project::~Project()
{
    delete due_date;
    for (auto iter = submission_dates.begin(); iter != submission_dates.end(); ++iter)
    {
        delete *iter;
    }
}

std::string Project::getDescription()
{
    return this->description;
}

Date *Project::getDueDate()
{
    return this->due_date;
}

std::vector<Date *> Project::getSubmissionDateCopy()
{
    std::vector<Date *> date_list(this->submission_dates.size());
    for (size_t index = 0; index < this->submission_dates.size(); index++)
    {
        if (this->submission_dates[index] != nullptr)
        {
            date_list[index] = new Date(this->submission_dates[index]->getDay(), this->submission_dates[index]->getMonth(), this->submission_dates[index]->getYear());
        }
        else
        {
            date_list[index] = nullptr;
        }
    }

    return date_list;
}

//* SETTER
void Project::setDescription(std::string new_description)
{
    this->description = new_description;
}

void Project::setDueDate(Date *new_due_date)
{
    this->due_date = new_due_date;
};

//* UPDATE SUBMISSION DATE
void Project::addSubmissionDate(std::vector<Date *>::size_type groupID, Date *group_subbmit_date)
{
    if (groupID > this->submission_dates.size())
    {
        this->submission_dates.resize(groupID);
    }

    this->submission_dates[groupID - 1] = group_subbmit_date;
};

// FUNCTION TO RESIZE THE submission_dates VECTOR
void resizeSubmissionDates(Project *project, int newSize)
{
    project->submission_dates.resize(newSize);
}

// Print Out Project Object
std::ostream &operator<<(std::ostream &os, const Project &project)
{
    os << "DESCRIPTION: " << std::endl;
    os << project.description << std::endl;
    os << "DUE DATE: " << project.due_date << std::endl;
    os << "SUBMISSION DATE FOR EACH GROUP:" << std::endl;
    for (size_t index = 0; index < project.submission_dates.size(); index++)
    {
        os << "SUBMISSION DATE FOR GROUP " << index + 1 << ": " << project.submission_dates[index] << std::endl;
    }
    return os;
}

// Print Out Project Object
std::ostream &operator<<(std::ostream &os, const Project *project)
{
    os << "DESCRIPTION: " << std::endl;
    os << project->description << std::endl;
    os << "DUE DATE: " << project->due_date << std::endl;
    os << "SUBMISSION DATE FOR EACH GROUP:" << std::endl;
    for (size_t index = 0; index < project->submission_dates.size(); index++)
    {
        os << "SUBMISSION DATE FOR GROUP " << index + 1 << ": " << project->submission_dates[index] << std::endl;
    }
    return os;
}