DROP VIEW IF EXISTS HotelRoom;
DROP VIEW IF EXISTS BookingGuest;

CREATE VIEW HotelRoom
    AS SELECT r.*
    FROM Hotel h, Room r
    WHERE h.hotelNo = r.hotelNo AND h.hotelName = 'Grosvenor Hotel';

CREATE VIEW BookingGuest
    AS SELECT b.roomNo, g.guestName
    FROM Booking b, Guest g
    WHERE b.guestNo = g.guestNo
        AND ( b.dateFrom <= CURDATE() AND b.dateTo > CURDATE() )
        AND b.hotelNo = 
            (SELECT h.hotelNo
             FROM Hotel h
             WHERE h.hotelName = 'Grosvenor Hotel'
            ); --NOTE:  "b.hotelNo = (...)" works b/c only 1 row is returned!! There is only one hotel with that name!
               --       Otherwise, I'd have to use "b.hotelNo IN (...)" if more than one row returned. Else, I get error

SELECT hr.*, bg.guestName
FROM HotelRoom hr
LEFT JOIN BookingGuest bg ON hr.roomNo = bg.roomNo;
--LEFT JOIN BookingGuest bg; --NOTE: This was giving me errors without the ON clause above. 
                             --      Error message wasn't very insightful (syntax is bad, mmkay?)

-- SELECT *
-- FROM HotelRoom;

-- SELECT *
-- FROM BookingGuest;
