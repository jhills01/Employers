DROP VIEW IF EXISTS CurrentGuestCount;

CREATE VIEW CurrentGuestCount
    AS SELECT hotelNo, COUNT(*) as guestCount
    FROM Booking
    WHERE dateFrom <= CURDATE() AND dateTo > CURDATE()
    GROUP BY hotelNo;

-- print result
SELECT *
FROM CurrentGuestCount;

-- test code --
-- SELECT *
-- FROM Booking
-- ORDER BY dateTo;