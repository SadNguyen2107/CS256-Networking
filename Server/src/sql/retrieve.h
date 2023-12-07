//? This Place is the Place to hold all the needed Retrieve Function

#ifndef RETRIEVEFUNC_H
#define RETRIEVEFUNC_H

#include "../Base/Group.h"
#include "../Base/Project.h"
#include <sqlite3.h>

/**
 * @brief Get the Student Info and the Group that the Student Belong to
 *
 * @param DB The DB object
 * @param student_ID HUST-ID to check
 * @param ppStudent the memory address of the pointer student-Object
 * @return int return -1 if that Student Have No Group Belong To else > 0
 */
int getStudentInfo(sqlite3 *DB, size_t student_ID, Student **ppStudent)
{
    // Return group-id
    int group_id = -1;

    // Check For memory Leak occur
    if (*ppStudent != nullptr)
    {
        delete *ppStudent;
        *ppStudent = nullptr;
    }

    // SELECT * FROM students
    const char *selectSQL = "SELECT id, student_name, group_ID FROM students WHERE id = :id;";
    sqlite3_stmt *statement;

    // Prepare the SQL statement
    int result = sqlite3_prepare_v2(DB, selectSQL, -1, &statement, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return group_id;
    }

    // Bind Values to the placeholders
    sqlite3_bind_int(statement, 1, static_cast<int>(student_ID));

    // Excute the prepared statement and iterate through the results
    result = sqlite3_step(statement);
    if (result == SQLITE_ROW)
    {
        // Fetch data from the current row
        int student_id = sqlite3_column_int(statement, 0);
        const char *student_name = reinterpret_cast<const char *>(sqlite3_column_text(statement, 1));
        std::string student_name_str = student_name;

        // Assign To the Return Result
        group_id = sqlite3_column_int(statement, 2);

        // Assign to the ppStudent
        *ppStudent = newStudent(student_name_str, student_id);
    }
    else if (result != SQLITE_DONE)
    {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(DB) << std::endl;
        return group_id;
    }

    // Finalize The Prepared Statement
    sqlite3_finalize(statement);

    return group_id;
}

#endif