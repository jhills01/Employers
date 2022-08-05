-- REFERENCE: pdf page 37/43 from "Ch07-SQL-DDL.pdf"

-- (a) Valid:: hotelNo and roomNo are correctly selected columns in the View. Lastly, "hotelNo = 1"...
--             ...in WHERE clause is valid b/c hotelNo is not a column based on a aggregate function.
--             If bookingCount had been used in WHERE, then it would have been invalid. 

-- (b) Invalid: Columns in Views based on aggregate functions are not allowed to be used as arguments...
--              ... to any aggregate function within any query on the corresponding View. 

-- (c) Valid: Columns in Views based on aggregate functions are only allowed in SELECT and ORDER BY...
--            ...clauses. Thus, this query is valid since it adheres to this rule. 







-- PLEASE IGNORE EVERYTHING BELOW THIS LINE =======================================================

--TEST CODE for kicks
-- DROP VIEW IF EXISTS RoomBookingCount;

-- CREATE VIEW RoomBookingCount (hotelNo, roomNo, bookingCount)
--     AS SELECT b.hotelNo, r.roomNo, COUNT(*)
--     FROM Room r, Booking b
--     WHERE r.roomNo = b.roomNo AND r.hotelNo = b.hotelNo
--     GROUP BY b.hotelNo, r.roomNo;

-- SELECT b.hotelNo, r.roomNo
-- FROM Room r, Booking b
-- WHERE r.roomNo = b.roomNo AND r.hotelNo = b.hotelNo;

-- --A: This first GROUP BY seems to only pick the first hotelNo/roomNo pair it finds and uses that
-- SELECT b.hotelNo, r.roomNo
-- FROM Room r, Booking b
-- WHERE r.roomNo = b.roomNo AND r.hotelNo = b.hotelNo
-- GROUP BY b.hotelNo;

-- --B: This GROUP BY actually groups by hotelNo first, then it looks inside each of those groups and...
-- --...groups by roomNo. Good stuff.
-- SELECT b.hotelNo, r.roomNo
-- FROM Room r, Booking b
-- WHERE r.roomNo = b.roomNo AND r.hotelNo = b.hotelNo
-- GROUP BY b.hotelNo, r.roomNo;

-- --C: This GROUP BY behaves the same as A:, but groups by roomNo instead. 
-- SELECT b.hotelNo, r.roomNo
-- FROM Room r, Booking b
-- WHERE r.roomNo = b.roomNo AND r.hotelNo = b.hotelNo
-- GROUP BY r.roomNo;


-- SELECT hotelNo, roomNo
-- FROM RoomBookingCount
-- WHERE hotelNo = 1;

-- SELECT hotelNo, SUM(bookingCount)
-- FROM RoomBookingCount
-- GROUP BY hotelNo;

-- SELECT *
-- FROM RoomBookingCount
-- ORDER BY bookingCount;