DROP VIEW IF EXISTS CheckingOutToday;

CREATE VIEW CheckingOutToday
AS SELECT g.*, b.roomNo, DATEDIFF(b.dateTo, b.dateFrom) as numDays, 
    DATEDIFF(b.dateTo, b.dateFrom) * r.price as totalCost
FROM Booking b, Guest g, Room r
WHERE b.guestNo = g.guestNo 
    AND b.roomNo = r.roomNo
    AND b.hotelNo = r.hotelNo
    AND b.hotelNo = 
        (
            SELECT h.hotelNo
            FROM Hotel h
            WHERE h.hotelName = 'Grosvenor Hotel'
        )
    AND b.dateTo = CURDATE();

-- print final result
SELECT * 
FROM CheckingOutToday; 
