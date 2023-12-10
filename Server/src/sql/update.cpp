#include "update.h"

// INSERT INTO students table in projects.db
UpdateStatus insertStudentInfo(sqlite3 *DB, Student *student, int group_id)
{
    // SET FLAG
    UpdateStatus success = UPDATE_SUCCESS;

    // The prepared statment object
    sqlite3_stmt *stmt = nullptr;
    const char *sql_statement = "UPDATE students SET group_id = :group_id WHERE id = :student_id";

    // Prepare the SQL Statement
    // The sql_statement -> byte-code
    int rc = sqlite3_prepare_v2(DB, sql_statement, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        BOOST_LOG_TRIVIAL(error) << "SQL error: " << sqlite3_errmsg(DB) << std::endl;
    }

    int student_id = student->student_id;

    // Bind the paramenter
    sqlite3_bind_int(stmt, 1, student_id);
    sqlite3_bind_int(stmt, 2, group_id);

    // Execute the prepared statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        // ERROR OCCUR
        BOOST_LOG_TRIVIAL(error) << "Error: " << sqlite3_errmsg(DB) << std::endl;
        success = UPDATE_FAIL;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);

    return success;
}