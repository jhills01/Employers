SELECT h.hotelName, COUNT(*) AS count
FROM Hotel h
JOIN Room r ON h.hotelNo = r.hotelNo AND h.city = 'London'
GROUP BY h.hotelNo;