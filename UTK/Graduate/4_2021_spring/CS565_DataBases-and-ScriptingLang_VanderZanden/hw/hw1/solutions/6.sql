
--NOTE: I got 3 points off b/c I didn't consider roomNo = roomNo basically. Look at key.

--SELECT *
SELECT SUM(price)
FROM Hotel h
JOIN Room r ON h.hotelNo = r.hotelNo AND h.hotelName = 'Grosvenor Hotel'
WHERE (h.hotelNo, r.roomNo) IN (SELECT hotelNo, roomNo
                  FROM Booking
                  WHERE dateFrom <= CURDATE() AND dateTo > CURDATE());  



--NOTE: This works too, but I was originally aiming for the above
--      ...BUT, I screwed up the "(h.hotelNo, r.roomNo) IN" part

-- -- SELECT *
-- SELECT SUM(price)
-- FROM Hotel h
-- JOIN Room r ON h.hotelName = 'Grosvenor Hotel' AND h.hotelNo = r.hotelNo
-- JOIN Booking b
-- WHERE b.dateFrom <= CURDATE() AND b.dateTo > CURDATE() AND r.roomNo = b.roomNo;

