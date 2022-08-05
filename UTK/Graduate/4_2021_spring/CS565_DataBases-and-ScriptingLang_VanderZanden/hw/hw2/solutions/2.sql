--NOTE: Since "Booking" contained foreign keys to other parent tables, I had to DROP it first!!!
drop table if exists Booking;
drop table if exists Hotel;
drop table if exists Room;
drop table if exists Guest;

create table Hotel
(
    hotelNo int(4),
    hotelName varchar(30),
    city varchar(20),
    
    PRIMARY KEY(hotelNo)
);

create table Room
(
    roomNo int(3)
        CHECK (roomNo BETWEEN 1 AND 100),
    hotelNo int(4),
    type varchar(9),
        CHECK (type IN ('Single', 'Double', 'Family')),
    price dec(5,2)
        CHECK (price BETWEEN 10.00 AND 100.00),

    PRIMARY KEY (roomNo)
);

create table Guest
(
    guestNo int(7),
    guestName varchar(30),
    guestAddress varchar(50),
    
    PRIMARY KEY (guestNo)
);

create table Booking
(
    hotelNo int(4),
    guestNo int(7),
    dateFrom date,
        /*CHECK (dateFrom > CURDATE()),*/
    dateTo date,
        /*CHECK (dateTo > CURDATE()),*/
    roomNo int(3)
        CHECK (roomNo BETWEEN 1 AND 100),
    
    /*
    NOTE: In order to get foreign keys to work, I had to
            1) Put all parent tables ABOVE Booking.
            2) Make referenced attributes primary keys in parent tables
    */
    FOREIGN KEY(hotelNo) REFERENCES Hotel(hotelNo)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY(guestNo) REFERENCES Guest(guestNo)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY(roomNo) REFERENCES Room(roomNo)
        ON DELETE CASCADE
        ON UPDATE CASCADE
    
    /*
    CONSTRAINT e
    CHECK 
    (
        NOT EXISTS
        (
            SELECT b1.*, COUNT(*) AS numOfConflictsWithOtherBookings
            FROM Booking b1, Booking b2
            WHERE b1.hotelNo = b2.hotelNo
                AND b1.roomNo = b2.roomNo
                AND
                (
                    (b1.dateFrom > b2.dateFrom AND b1.dateFrom < b2.dateTo)
                    OR 
                    (b2.dateFrom > b1.dateFrom AND b2.dateFrom < b1.dateTo)
                )
            GROUP BY b1.dateFrom, b1.dateTo
            ORDER BY b1.dateFrom, b1.dateTo;
        )
    )

    CONSTRAINT f
    CHECK 
    (
        NOT EXISTS
        (
            SELECT b1.*, COUNT(*) AS numOfOverlappingBookingsByGuests
            FROM Booking b1, Booking b2
            WHERE b1.guestNo = b2.guestNo
                AND
                (
                    (b1.dateFrom > b2.dateFrom AND b1.dateFrom < b2.dateTo)
                    OR 
                    (b2.dateFrom > b1.dateFrom AND b2.dateFrom < b1.dateTo)
                )
            GROUP BY b1.guestNo, b1.dateFrom, b1.dateTo
            ORDER BY b1.dateFrom, b1.dateTo;
        )
    )
    */
);






--TEST CODEs =================================================================================
--NOTE: Inserts below WORK with constraints AND foreign key dependencies
-- insert into Hotel values(1, 'Grosvenor Hotel', 'London');
-- insert into Hotel values(2, 'Holiday Inn', 'Knoxville');
-- insert into Hotel values(3, 'Hilton', 'Knoxville');
-- insert into Hotel values(4, 'Marriott', 'Memphis');

-- insert into Room values(100, 2, 'Double', 99.99);
-- insert into Room values(99, 2, 'Double', 99.99);

-- insert into Guest values(33, 'Cinderella', 'Orlando, FL');
-- insert into Guest values(666, 'Satan', 'My Flesh Palace, HELL');

-- insert into Booking values(1, 33, '2021-02-17', '2021-02-22', 100);
-- insert into Booking values(1, 33, '2021-02-15', '2021-02-18', 100);
-- insert into Booking values(1, 33, '2021-02-02', '2021-02-28', 100);
-- insert into Booking values(1, 33, '2021-02-01', '2021-02-03', 100);


--NOTE: With foreign keys added to Bookings, these fail b/c I need to establish parent tables first!
--      Correction above
-- insert into Booking values(1, 33, '2021-02-17', '2021-02-22', 100);
-- insert into Booking values(1, 33, '2021-02-15', '2021-02-18', 100);
-- insert into Booking values(1, 33, '2021-02-02', '2021-02-28', 100);
-- insert into Booking values(1, 33, '2021-02-01', '2021-02-03', 100);

-- --(e)
-- SELECT b1.*, COUNT(*) AS numOfConflictsWithOtherBookings
-- FROM Booking b1, Booking b2
-- WHERE b1.hotelNo = b2.hotelNo
--     AND b1.roomNo = b2.roomNo
--     AND
--     (
--         (b1.dateFrom > b2.dateFrom AND b1.dateFrom < b2.dateTo)
--         OR 
--         (b2.dateFrom > b1.dateFrom AND b2.dateFrom < b1.dateTo)
--     )
-- GROUP BY b1.dateFrom, b1.dateTo
-- ORDER BY b1.dateFrom, b1.dateTo;

-- --(f)
-- SELECT b1.*, COUNT(*) AS numOfOverlappingBookingsByGuests
-- FROM Booking b1, Booking b2
-- WHERE b1.guestNo = b2.guestNo
--     AND
--     (
--         (b1.dateFrom > b2.dateFrom AND b1.dateFrom < b2.dateTo)
--         OR 
--         (b2.dateFrom > b1.dateFrom AND b2.dateFrom < b1.dateTo)
--     )
-- GROUP BY b1.guestNo, b1.dateFrom, b1.dateTo
-- ORDER BY b1.dateFrom, b1.dateTo;


-- insert into Room values(100, 2, 'penthouse', 249.99); --'penthouse' not valid type. Error success.
-- insert into Room values(100, 2, 'Family', 249.99); --249.99 not BETWEEN 10.00 AND 100.00. Error success.

-- insert into Room values(101, 2, 'Family', 15.00); --room 101 not BETWEEN 1 AND 100. Error success.
-- insert into Booking values(1, 21, '2021-02-02', '2021-10-01', 101);--room 101 not BETWEEN 1 AND 100. Error success.

-- insert into Booking values(1, 21, '2021-02-02', '2021-02-11', 99);--room 101 not BETWEEN 1 AND 100. Error success.


--IGNORE CODE BELOW THIS LINE =========================================================================

-- -- SELECT b1.*
-- SELECT b1.*, COUNT(*) as numOfConflictingBooks
-- FROM Booking b1, Booking b2
-- WHERE   
--     /*(b1.dateFrom BETWEEN b2.dateFrom AND b2.dateTo)*/
--     (b1.dateFrom > b2.dateFrom AND b1.dateFrom < b2.dateTo)
--     OR 
--     /*(b2.dateFrom BETWEEN b1.dateFrom AND b1.dateTo)*/
--     (b2.dateFrom > b1.dateFrom AND b2.dateFrom < b1.dateTo)
-- GROUP BY b1.roomNo;
-- --HAVING numOfSimultaneousBooksIncludingSelf > 1;

