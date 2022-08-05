drop table if exists Booking;
drop table if exists Hotel;
drop table if exists Room;
drop table if exists Guest;

create table Hotel(
    hotelNo int(4),
    hotelName varchar(30),
    city varchar(20));

create table Room(
       roomNo int(3),
       hotelNo int(4),
       type varchar(9),
       price dec(5,2));

create table Booking(
       hotelNo int(4),
       guestNo int(7),
       dateFrom date,
       dateTo date,
       roomNo int(3));

create table Guest(
       guestNo int(7),
       guestName varchar(30),
       guestAddress varchar(50));

insert into Hotel values(1, 'Grosvenor Hotel', 'London');
insert into Hotel values(2, 'Holiday Inn', 'Knoxville');
insert into Hotel values(3, 'Hilton', 'Knoxville');
insert into Hotel values(4, 'Marriott', 'Memphis');

-- --ADDED
-- insert into Hotel values(5, 'Marriott', 'London');
-- --ADDED

insert into Room values(100, 1, 'double', 89.99);
insert into Room values(200, 1, 'family', 109.99);
insert into Room values(300, 1, 'king', 135.50);
insert into Room values(500, 2, 'king', 205);
insert into Room values(100, 2, 'penthouse', 249.99);
insert into Room values(200, 2, 'double', 99.99);
insert into Room values(300, 2, 'double', 109.99);
insert into Room values(400, 2, 'family', 149.99);
insert into Room values(400, 1, 'penthouse', 899.99);
insert into Room values(450, 2, 'double', 27.95);
insert into Room values(250, 2, 'family', 32.50);
insert into Room values(110, 1, 'double', 35.00);

--ADDED
-- insert into Room values(110, 5, 'double', 35.00);
-- insert into Room values(110, 5, 'double', 35.00);
--ADDED

insert into Booking values(1, 33, '2021-01-11', '2021-02-01', 100);
insert into Booking values(1, 31, '2021-01-21', '2021-01-30', 200);
insert into Booking values(1, 20, '2021-01-30', '2021-02-03', 200);
insert into Booking values(1, 20, '2021-02-11', '2021-02-14', 100);
insert into Booking values(1, 21, '2021-02-18', '2021-02-19', 100);
insert into Booking values(1, 22, '2021-02-23', '2021-10-28', 100);
insert into Booking values(1, 21, '2021-02-02', '2021-10-01', 100);

--ADDED
--insert into Booking values(1, 21, '2021-02-03', '2021-10-01', 400);
--ADDED

insert into Booking values(1, 30, '2021-02-11', '2021-02-14', 200);
insert into Booking values(1, 31, '2021-02-15', '2021-02-20', 200);
insert into Booking values(1, 32, '2021-02-27', '2021-02-28', 200);
insert into Booking values(1, 33, '2021-02-01', '2021-02-02', 200);

insert into Booking values(1, 41, '2021-02-05', '2021-02-06', 300);
insert into Booking values(1, 42, '2021-02-15', '2021-02-20', 300);
insert into Booking values(1, 33, '2021-02-20', '2021-02-25', 300);
insert into Booking values(1, 21, '2021-02-25', '2021-10-02', 300);
insert into Booking values(1, 33, '2021-02-06', '2021-02-10', 300);

insert into Booking values(2, 21, '2021-02-18', '2021-02-25', 100);
insert into Booking values(2, 42, '2021-02-05', '2021-02-28', 500);
insert into Booking values(2, 31, '2021-02-09', '2021-12-11', 300);
insert into Booking values(2, 31, '2021-02-11', '2021-02-12', 300);

insert into Guest values(20, 'Brad Vander Zanden', 'Knoxville, TN');
insert into Guest values(21, 'Minnie Mouse', 'Orlando, FL');
insert into Guest values(22, 'Wily Coyote', 'Phoenix, AZ');

insert into Guest values(30, 'Daffy Duck', 'Knoxville, TN');
insert into Guest values(31, 'Winnie The Pooh', 'Phoenix, AZ');
insert into Guest values(32, 'Pluto', 'Phoenix, AZ');
insert into Guest values(33, 'Snow White', 'Orlando, FL');

insert into Guest values(40, 'Bugs Bunny', 'Ithaca, NY');
insert into Guest values(41, 'Quickdraw McGraw', 'Townsend, TN');
insert into Guest values(42, 'Cinderella', 'Orlando, FL');

--my tests
insert into Guest values(666, 'SATAN!!!', 'My Flesh Palace, HELL');
insert into Booking values(1, 41, '2021-02-12', '2021-02-13', 300);
insert into Booking values(1, 41, '2021-02-12', '2021-02-14', 400);
insert into Booking values(1, 666, '2021-02-10', '2021-02-13', 100);


-- insert into Booking values(1, 666, '2021-02-09', '2021-02-11', 400);
-- insert into Guest values(666, 'Satan', 'My Flesh Palace, HELL');

-- insert into Booking values(1, 666, '2002-02-11', '2021-02-13', 666);
-- insert into Booking values(1, 32, '2002-02-10', '2021-02-13', 667);
-- insert into Booking values(1, 33, '2002-05-11', '2021-02-13', 668);

-- insert into Booking values(3, 34, '2021-02-02', '2021-11-12', 669);
-- insert into Booking values(3, 35, '2021-02-03', '2021-02-13', 670);

-- insert into Guest values(666, 'SATAN!!!', 'My Flesh Palace, HELL');

-- insert into Room values(400, 3, 'b1house', 899.99);
-- insert into Booking values(1, 666, '2021-02-01', '2021-02-11', 400);
-- insert into Booking values(2, 666, '2021-02-02', '2021-02-11', 400);
-- insert into Booking values(3, 666, '2021-02-03', '2021-02-11', 400);

-- insert into Booking values(1, 35, '2021-02-03', '2021-02-13', 670);
-- insert into Room values(300, 1, 'king', 135.50);
