-- Active: 1701942902692@@127.0.0.1@3306

-- Find the Group Name
SELECT group_name
FROM "groups"
WHERE id = 1;

-- Find all the students with the same group_id
SELECT id, student_name
FROM students
WHERE "group_ID" = 1;

