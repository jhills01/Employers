/*
Do NOT use the below MODIFIABLE relations in any query.
These will need to be updated by the website and shouldn't be used in querying.
    OutcomeResults
    Assessments
    Narratives
*/

SELECT a.sectionId, i.email, a.outcomeId, a.major, SUM(a.weight) AS weightTotal
FROM Assessments a
NATURAL JOIN Sections s
NATURAL JOIN Instructors i
GROUP BY a.major, a.outcomeId, a.sectionId
HAVING weightTotal != 100
ORDER BY i.email ASC, a.major ASC, a.outcomeId ASC;