/*
Do NOT use the below MODIFIABLE relations in any query.
These will need to be updated by the website and shouldn't be used in querying.
    OutcomeResults
    Assessments
    Narratives
*/

--SELECT ocr.*, pl.description
SELECT pl.description, ocr.numberOfStudents
FROM OutcomeResults ocr
NATURAL JOIN PerformanceLevels pl
WHERE ocr.outcomeId = 2
    AND ocr.major = 'CS'
    AND ocr.sectionId = 3
ORDER BY ocr.performanceLevel ASC;