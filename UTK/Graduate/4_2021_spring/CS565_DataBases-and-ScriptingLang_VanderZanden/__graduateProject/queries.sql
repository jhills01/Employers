
--FROM BOOK========================================================================================
--(a)
SELECT h.hallNo, rs.firstname, rs.lastname, h.phone
FROM residenceStaff rs
JOIN halls h ON rs.staffNo = h.managerNo;

--(b)
SELECT s.bannerNo, s.firstname, s.lastname, l.*
FROM Student s
NATURAL JOIN Lease l;

--(c)
SELECT *
FROM Lease
WHERE summerSem = 'yes';
      
--(d)
SELECT s.bannerNo, s.firstname, s.lastname, SUM(payment) as TotalRentPaid
FROM Student s
JOIN Invoice i ON s.bannerNo = i.bannerNo
GROUP BY s.bannerNo;

--(e)
SELECT s.bannerNo, s.firstname, s.lastname 
FROM Student s
JOIN Invoice i ON s.bannerNo = i.bannerNo
WHERE i.paymentDate < '2021-12-11';

--(f)
SELECT *
FROM apartmentInspections
WHERE inspectionSatisfied = 'no';

--(g)
SELECT s.bannerNo, s.firstname, s.lastname, hp.hallNo, rp.roomNo, rp.placeNo
FROM Student s
JOIN Lease l ON s.bannerNo = l.bannerNo
JOIN hallPlaces hp ON l.placeNo = hp.placeNo
JOIN residencePlaces rp ON hp.placeNo = rp.placeNo
WHERE hp.hallNo = 1;

--(h)
--NOTE: There's way too much info on students, so I'm only choosing bannerNo, name, and status
SELECT s.bannerNo, s.firstname, s.lastname, s.status
FROM Student s
WHERE s.status = 'waiting';

--(i)
SELECT s.category, COUNT(*) AS studentCategoryCount
FROM Student s
GROUP BY s.category;

--(j)
SELECT s.bannerNo, s.firstname, s.lastname 
FROM Student s
WHERE s.bannerNo NOT IN
(
    SELECT s.bannerNo
    FROM Student s
    JOIN nextOfKin nok ON s.bannerNo = nok.bannerNo
);

--(k)
--NOTE: I'm assuming bannerNo of the "particular student" would be known for the query
SELECT a.firstname, a.lastname, a.phone
FROM Adviser a
WHERE a.adviserNo = 
(
    SELECT s.adviserNo
    FROM Student s
    WHERE s.bannerNo = 17
);

--(l)
SELECT MIN(monthlyRent) AS minimum, MAX(monthlyRent) AS maximum, AVG(monthlyRent) AS average
FROM residencePlaces rp
WHERE rp.placeNo IN
(
    SELECT hp.placeNo
    FROM hallPlaces hp
);

--(m)
SELECT hp.hallNo, COUNT(*) AS totalNumberOfPlaces
FROM hallPlaces hp
GROUP BY hp.hallNo;

--(n)
SELECT rs.staffNo, rs.firstname, rs.lastname, DATEDIFF(CURDATE(), rs.birthDate)/365 AS age, rs.location
FROM residenceStaff rs
WHERE DATEDIFF(CURDATE(), rs.birthDate)/365 > 60.0;

--FROM BVZ========================================================================================
SELECT s.firstname, s.lastname, ap.apartmentNo, rp.roomNo AS bedroomNo
FROM Student s
JOIN Lease l ON s.bannerNo = l.bannerNo
JOIN apartmentPlaces ap ON l.placeNo = ap.placeNo
JOIN residencePlaces rp ON ap.placeNo = rp.placeNo
WHERE ap.apartmentBldgNo IN 
(
    SELECT ab.apartmentBldgNo
    FROM apartmentBuildings ab
    WHERE ab.addrStreet = '1678 Cardiff Rd'
);