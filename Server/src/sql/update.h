#ifndef UPDATEFUNC_H
#define UPDATEFUNC_H

#include "database_messages.h"

enum UpdateStatus {
    UPDATE_FAIL = 0,
    UPDATE_SUCCESS = 1
};

UpdateStatus insertStudentInfo(sqlite3 *DB, Student *student, int group_id);

#endif