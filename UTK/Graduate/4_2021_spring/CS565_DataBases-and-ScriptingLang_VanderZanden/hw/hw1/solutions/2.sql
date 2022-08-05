SELECT *
FROM Room
WHERE (type IN ('double', 'family') AND price < 40.00)
ORDER BY price; 
--ORDER BY price DESC; 

-- SELECT *
-- FROM Hotel
-- WHERE city = 'London';

-- SELECT s.FirstName, s.LastName
--     FROM student s
--     JOIN studentcourses sc ON s.Id = sc.StudentId 
--     WHERE sc.courseId = 'CS140';