/*
Do NOT use the below MODIFIABLE relations in any query.
These will need to be updated by the website and shouldn't be used in querying.
    OutcomeResults
    Assessments
    Narratives
*/

--SELECT a.sectionId, a.weight, a.outcomeId, a.major
SELECT a.assessmentDescription, a.weight
FROM Assessments a
WHERE a.sectionId = 3
    AND a.outcomeId = 2
    AND a.major = 'CS'
ORDER BY a.weight DESC, a.assessmentDescription ASC;