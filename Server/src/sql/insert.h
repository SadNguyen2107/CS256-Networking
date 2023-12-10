#ifndef INSERTFUNC_H
#define INSERTFUNC_H

#include "database_messages.h"

enum InsertStatus
{
    INSERT_FAIL = 0,
    INSERT_SUCCESS = 1
};

InsertStatus insertStudentInfo(sqlite3 *DB, Student *student, size_t group_id);
InsertStatus insertGroupInfo(sqlite3 *DB, Group *group, size_t group_id);
InsertStatus insertProjectInfo(sqlite3 *DB, Project *project, size_t project_id);
InsertStatus insertSubmitDate(sqlite3 *DB, Date *submitDate, size_t group_id, size_t project_id);

#endif