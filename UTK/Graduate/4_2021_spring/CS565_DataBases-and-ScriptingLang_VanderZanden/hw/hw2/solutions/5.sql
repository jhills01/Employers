DROP VIEW IF EXISTS HotelData;

-- problem 5
CREATE VIEW HotelData
    AS SELECT g.guestName, h.hotelName
    FROM Hotel h, Guest g, Booking b
    WHERE h.hotelNo = b.hotelNo 
        AND b.guestNo = g.guestNo
        AND ( b.dateFrom <= CURDATE() AND b.dateTo > CURDATE() )
    ORDER BY h.hotelName;

-- TEST CODE
-- CREATE VIEW HotelData
--     AS SELECT g.guestName, h.hotelName, b.dateFrom, b.dateTo
--     FROM Hotel h, Guest g, Booking b
--     WHERE h.hotelNo = b.hotelNo 
--         AND b.guestNo = g.guestNo
--         AND ( b.dateFrom <= CURDATE() AND b.dateTo > CURDATE() )
--     ORDER BY h.hotelName;

-- print final result
SELECT *
FROM HotelData;

-- OLD CODE THAT DIDN'T WORK: KEEPING FOR REFERENCE ------------------------------------------------------
-- CREATE VIEW HotelData
--     AS SELECT g.guestName, h.hotelName
--     FROM Hotel h, Guest g, Booking b
--     WHERE h.hotelNo = b.hotelNo AND b.guestNo = g.guestNo;
    
    -- WHERE g.guestNo IN 
    --     (   
    --         SELECT b.guestNo
    --         FROM Booking b
    --         WHERE b.dateFrom <= CURDATE() AND b.dateTo > CURDATE()
    --     );

        -- AND
        
        -- h.hotelNo IN 
        -- ( 
        --     SELECT b.hotelNo
        --     FROM Booking b
        --     WHERE b.dateFrom <= CURDATE() AND b.dateTo > CURDATE()
        -- );
