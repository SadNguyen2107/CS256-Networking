//? This Place is the Place to hold all the needed Retrieve Function

#ifndef RETRIEVEFUNC_H
#define RETRIEVEFUNC_H

#include "database_messages.h"

enum GroupStatus
{
    GROUP_NOT_EXIST = 0,
    GROUP_EXIST = 1
};

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
        BOOST_LOG_TRIVIAL(error) << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
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

        // Assign To the Return Result
        group_id = sqlite3_column_int(statement, 2);

        // Assign to the ppStudent
        *ppStudent = newStudent(student_name, student_id);
    }
    else if (result != SQLITE_DONE)
    {
        BOOST_LOG_TRIVIAL(error) << "Error executing statement: " << sqlite3_errmsg(DB) << std::endl;
        return group_id;
    }

    // Finalize The Prepared Statement
    sqlite3_finalize(statement);

    return group_id;
}

/**
 * @brief Get The Group Object and the all related Student in this group
 * 
 * @param DB The Pointer To database after open the database
 * @param group_id group-id To get
 * @param ppGroup The memory address of the Group Object
 * @return GROUP_EXIST if that group-info is EXIST else GROUP_NOT_EXIST
 */
GroupStatus getGroupInfo(sqlite3 *DB, size_t group_id, Group **ppGroup)
{
    // Return Status
    GroupStatus groupStatus = GROUP_NOT_EXIST;

    // Check If valid group_id
    if (group_id <= 0)
    {
        BOOST_LOG_TRIVIAL(error) << "Group ID cannot be negative!" << std::endl;
        return groupStatus;
    }

    // Check if memory leak
    if (*ppGroup != nullptr)
    {
        delete *ppGroup;
        *ppGroup = nullptr;
    }

    // SQL Statment To Reuse
    sqlite3_stmt *selectGroupStatement;

    // Find The Group with group_id
    //==============================================================================================
    // SELECT SQL Statement
    const char *selectGroup = "SELECT group_name FROM groups WHERE id = :id";

    // Prepare The SQL statement
    int result = sqlite3_prepare_v2(DB, selectGroup, -1, &selectGroupStatement, nullptr);
    if (result != SQLITE_OK)
    {
        BOOST_LOG_TRIVIAL(error) << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return groupStatus;
    }

    // Bind Value to the Placeholder
    sqlite3_bind_int(selectGroupStatement, 1, static_cast<int>(group_id));

    // Execute The Prepared Statement and iterate through the result
    result = sqlite3_step(selectGroupStatement);
    if (result == SQLITE_ROW)
    {
        // Fetch Data From The Current Row
        const char *group_name = reinterpret_cast<const char *>(sqlite3_column_text(selectGroupStatement, 0));

        // Asssign To The ppGroup
        *ppGroup = new Group(group_name, group_id);
        groupStatus = GROUP_EXIST;
    }
    else if (result != SQLITE_DONE)
    {
        BOOST_LOG_TRIVIAL(error) << "Error executing statement: " << sqlite3_errmsg(DB) << std::endl;

        // Finalize The Prepared Statement
        sqlite3_finalize(selectGroupStatement);
        return groupStatus;
    }

    // Finalize The Prepared Statement
    sqlite3_finalize(selectGroupStatement);

    // Find All The Group Members
    //========================================================================================================
    // SELECT * FROM students
    const char *selectStudentSQL = "SELECT id, student_name FROM students WHERE group_id = :group_id;";
    sqlite3_stmt *selectStudentStatement;

    // Prepare the SQL statement
    result = sqlite3_prepare_v2(DB, selectStudentSQL, -1, &selectStudentStatement, nullptr);
    if (result != SQLITE_OK)
    {
        BOOST_LOG_TRIVIAL(error) << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return groupStatus;
    }

    // Bind Values to the placeholders
    sqlite3_bind_int(selectStudentStatement, 1, static_cast<int>(group_id));

    // Excute the prepared statement and iterate through the results
    while ((result = sqlite3_step(selectStudentStatement)) == SQLITE_ROW)
    {
        // Fetch data from the current row
        int student_id = sqlite3_column_int(selectStudentStatement, 0);
        const char *student_name = reinterpret_cast<const char *>(sqlite3_column_text(selectStudentStatement, 1));

        // Assign To the Return Result
        group_id = sqlite3_column_int(selectStudentStatement, 2);

        // Assign to the ppGroup
        Student *student = newStudent(student_name, student_id);
        (*ppGroup)->addStudent(student);
    }
    if (result != SQLITE_DONE)
    {
        BOOST_LOG_TRIVIAL(error) << "Error executing statement: " << sqlite3_errmsg(DB) << std::endl;

        // Finalize The Prepared Statement
        sqlite3_finalize(selectStudentStatement);
        return groupStatus;
    }

    // Finalize The Prepared Statement
    sqlite3_finalize(selectStudentStatement);

    return groupStatus;
}

#endif