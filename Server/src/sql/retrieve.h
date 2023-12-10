//? This Place is the Place to hold all the needed Retrieve Function

#ifndef RETRIEVEFUNC_H
#define RETRIEVEFUNC_H

#include "database_messages.h"

enum GroupStatus
{
    GROUP_NOT_EXIST = 0,
    GROUP_EXIST = 1
};

int getStudentInfo(sqlite3 *DB, size_t student_ID, Student **ppStudent);
GroupStatus getGroupInfo(sqlite3 *DB, size_t group_id, Group **ppGroup);

#endif