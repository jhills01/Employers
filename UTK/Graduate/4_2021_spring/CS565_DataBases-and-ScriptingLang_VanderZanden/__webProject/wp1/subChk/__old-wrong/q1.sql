/*
Do NOT use the below MODIFIABLE relations in any query.
These will need to be updated by the website and shouldn't be used in querying.
    OutcomeResults
    Assessments
    Narratives
*/

SELECT DISTINCT s.instructorId, s.sectionId, s.courseId, o.major, s.semester, s.year
FROM Instructors i
NATURAL JOIN Sections s
NATURAL JOIN Outcomes o
WHERE i.email = 'coyote@utk.edu'
    AND i.password = PASSWORD('password')
ORDER BY s.year DESC, s.semester ASC;
