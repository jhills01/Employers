DROP TABLE IF EXISTS BookingOld;

-- a.
CREATE TABLE BookingOld
    ( hotelNo int(4),
      guestNo int(7),
      dateFrom date,
      dateTo date,
      roomNo int(3) 
    );

-- b.
INSERT INTO BookingOld 
    ( SELECT *
      FROM Booking
      WHERE dateTo < '2003-1-1'
    );


-- c.
DELETE FROM Booking
WHERE dateTo < '2003-1-1';


-- NOTE: Not sure about this print. Check Piazza for Dr. VZ's answer.
SELECT *
FROM BookingOld;

SELECT *
FROM Booking;
