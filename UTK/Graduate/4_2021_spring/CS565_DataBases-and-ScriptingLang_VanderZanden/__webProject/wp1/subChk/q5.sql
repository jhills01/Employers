/*
Do NOT use the below MODIFIABLE relations in any query.
These will need to be updated by the website and shouldn't be used in querying.
    OutcomeResults
    Assessments
    Narratives
*/

--SELECT n.sectionId, n.outcomeId, n.major
SELECT n.strengths, n.weaknesses, n.actions
FROM Narratives n
WHERE n.major = 'CS'
    AND n.outcomeId = 2
    AND n.sectionId = 3;