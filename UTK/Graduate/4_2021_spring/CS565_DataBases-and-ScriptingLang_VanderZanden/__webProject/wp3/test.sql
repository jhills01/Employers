-- SELECT *
-- FROM OutcomeResults
-- WHERE sectionId > 8;

-- SELECT *
-- FROM Assessments;

-- SELECT *
-- FROM Narratives
-- WHERE sectionId > 3;

SELECT instructorId, email, password
FROM Instructors
WHERE email = "coyote@utk.edu";