/*
Do NOT use the below MODIFIABLE relations in any query.
These will need to be updated by the website and shouldn't be used in querying.
    OutcomeResults
    Assessments
    Narratives
*/

SELECT DISTINCT o.outcomeId, o.outcomeDescription
FROM Outcomes o
NATURAL JOIN CourseOutcomeMapping com
NATURAL JOIN Sections s
WHERE s.sectionId = 3
    AND com.major = 'CS'
ORDER BY o.outcomeId ASC;
