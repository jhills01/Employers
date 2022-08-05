
DROP TABLE IF EXISTS apartmentInspections;
DROP TABLE IF EXISTS apartmentPlaces;
DROP TABLE IF EXISTS hallPlaces;
DROP TABLE IF EXISTS halls;
DROP TABLE IF EXISTS Invoice;
DROP TABLE IF EXISTS nextOfKin;
DROP TABLE IF EXISTS Lease;

/*Had to drop Student (parent table) before Adviser (child table with foreign key)*/
DROP TABLE IF EXISTS Student;

DROP TABLE IF EXISTS Adviser;
DROP TABLE IF EXISTS residencePlaces;
DROP TABLE IF EXISTS residenceStaff;
DROP TABLE IF EXISTS apartmentBuildings;

/*IMPORTANT NOTE: List tables WITHOUT foreign keys first. Then create other tables with child tables below
                  their corresonding parent tables as relating to foreign keys*/

--TABLES WITHOUT FOREIGN KEYS!====================================================================================
--Adviser------------------------------------------------------------------------------------------------
CREATE TABLE Adviser
(
    adviserNo int,
    firstname varchar(500) NOT NULL,
    lastname varchar(500) NOT NULL,
    position varchar (500) NOT NULL,
    department varchar (500) NOT NULL,
    phone varchar (50) NOT NULL, /*XXX-XXX-XXXX*/
    email varchar (500) NOT NULL,
    officeRoomNo int NOT NULL,
	
    PRIMARY KEY (adviserNo)
);

--residencePlaces------------------------------------------------------------------------------------------------
CREATE TABLE residencePlaces
(
    placeNo int,
    monthlyRent decimal(15,2) NOT NULL,
    roomNo int NOT NULL,
	
    PRIMARY KEY (placeNo)
);

--residenceStaff------------------------------------------------------------------------------------------------
CREATE TABLE residenceStaff
(
    staffNo int,
    firstname varchar(500) NOT NULL,
    lastname varchar(500) NOT NULL,
    email varchar (500) NOT NULL,
    addrStreet varchar (500) NOT NULL,
    addrCity varchar (500) NOT NULL,
    addrPostalCode int NOT NULL,
    birthDate date NOT NULL,
    gender varchar (10) NOT NULL
        CHECK (gender IN ('male','female')),
    position varchar (500) NOT NULL, /*e.g., Hall Manager, Administrative Assistant, Cleaner, etc*/
    location varchar (500) NOT NULL, /*e.g., Residence Office, Hall, etc*/

    PRIMARY KEY (staffNo)
);

--apartmentBuildings----------------------------------------------------------------------------------------------
CREATE TABLE apartmentBuildings
(
    apartmentBldgNo int,
    addrStreet varchar (500) NOT NULL,
    addrCity varchar (500) NOT NULL,
    addrPostalCode int NOT NULL,
    numOfRooms int NOT NULL,

    PRIMARY KEY (apartmentBldgNo)
);


--TABLES WITH FOREIGN KEYS!=======================================================================================
--Student------------------------------------------------------------------------------------------------
CREATE TABLE Student
(
    /*Student address and phone info are related to HOME which sounds like parent's info*/
    bannerNo int,
    firstname varchar(500) NOT NULL,
    lastname varchar(500) NOT NULL,
    addrStreet varchar (500) NOT NULL,
    addrCity varchar (500) NOT NULL,
    addrPostalCode int NOT NULL,
    phone varchar (50) NOT NULL, /*XXX-XXX-XXXX*/
    email varchar (500) NOT NULL,
    birthDate date NOT NULL,
    gender varchar (10) NOT NULL
        CHECK (gender IN ('male','female')),
    category varchar (500) NOT NULL, /*book doesn't give enough info for me to do a CHECK*/
    nationality varchar (500) NOT NULL, /*Not allowing this to be NULL. Put "none" if not applicable.*/
    specialNeeds varchar (500) NOT NULL,/*Not allowing this to be NULL. Put "none" if not applicable.*/
    comments varchar (5000), /*optional field. Will allow NULL*/
    status varchar(10) NOT NULL
        CHECK (status IN ('placed','waiting')), /*Used to determine if student has placeNo or not*/
    major varchar(500) NOT NULL,
    minor varchar(500) NOT NULL,
    adviserNo int,
	
    PRIMARY KEY (bannerNo),

    FOREIGN KEY(adviserNo) REFERENCES Adviser(adviserNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

--nextOfKin------------------------------------------------------------------------------------------------
CREATE TABLE nextOfKin
(
    kinNo int,
    bannerNo int,
    firstname varchar(500) NOT NULL,
    lastname varchar(500) NOT NULL,
    relationship varchar(500) NOT NULL,
    addrStreet varchar (500) NOT NULL,
    addrCity varchar (500) NOT NULL,
    addrPostalCode int NOT NULL,
    phone varchar (50) NOT NULL, /*XXX-XXX-XXXX*/
	
    PRIMARY KEY (kinNo),

    FOREIGN KEY(bannerNo) REFERENCES Student(bannerNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

--Lease------------------------------------------------------------------------------------------------
CREATE TABLE Lease
(
    leaseNo int,
    bannerNo int,
    placeNo int,
    startDate date NOT NULL, /*startDate and endDate must align with semester calendar dates*/
    endDate date NOT NULL,
    duration int NOT NULL
        CHECK (duration IN (1,2,3)), /*duration is in terms of semesters
                                       1 == 1 semester duration
                                       2 == 2 semester duration (e.g., fall through spring)
                                       3 == 3 semester duration/full year (e.g. fall -> spring -> summer semesters)
                                     */
    fallSem varchar(5)
            CHECK (fallSem IN ('yes','no')),
    springSem varchar(5)
            CHECK (springSem IN ('yes','no')),
    summerSem varchar(5)
            CHECK (summerSem IN ('yes','no')),

    
    PRIMARY KEY (leaseNo),

    FOREIGN KEY(bannerNo) REFERENCES Student(bannerNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(placeNo) REFERENCES residencePlaces(placeNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

--Invoice------------------------------------------------------------------------------------------------
CREATE TABLE Invoice
(
    /*primary key*/
    invoiceNo int,
    
    /*foreign keys*/
    leaseNo int,
    bannerNo int,
    placeNo int,

    semester varchar(20) NOT NULL
        CHECK (semester IN ('fall','spring','summer')), /*semester the payment was issued in*/
    paymentDue decimal(15,2) NOT NULL,
    payment decimal(15,2) NOT NULL,
    paymentDate date, /*It's likely an invoice will be inserted without a payment. paymentDate can be NULL*/
    paymentMethod varchar(100),/*Same reasoning as with paymentDate. paymentMethod can be NULL*/
    firstReminderDate date, /*might not be need a 1st reminder. Can be NULL.*/
    secondReminderDate date, /*might not be need a 2nd reminder. Can be NULL.*/
    
    PRIMARY KEY (invoiceNo),
    
    FOREIGN KEY(leaseNo) REFERENCES Lease(leaseNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(bannerNo) REFERENCES Student(bannerNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(placeNo) REFERENCES residencePlaces(placeNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

--halls------------------------------------------------------------------------------------------------
CREATE TABLE halls
(
    hallNo int,
    name varchar(500) NOT NULL,
    addrStreet varchar (500) NOT NULL,
    addrCity varchar (500) NOT NULL,
    addrPostalCode int NOT NULL,
    phone varchar (50) NOT NULL, /*XXX-XXX-XXXX*/
    managerNo int,
	
    PRIMARY KEY (hallNo),

    FOREIGN KEY(managerNo) REFERENCES residenceStaff(staffNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

--hallPlaces------------------------------------------------------------------------------------------------
CREATE TABLE hallPlaces
(
    hallNo int,
    placeNo int,

    FOREIGN KEY(hallNo) REFERENCES halls(hallNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(placeNo) REFERENCES residencePlaces(placeNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

--apartmentPlaces------------------------------------------------------------------------------------------------
CREATE TABLE apartmentPlaces
(
    apartmentNo int,
    apartmentBldgNo int,
    placeNo int,
    
    FOREIGN KEY(apartmentBldgNo) REFERENCES apartmentBuildings(apartmentBldgNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(placeNo) REFERENCES residencePlaces(placeNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);

--apartmentInspections------------------------------------------------------------------------------------------------
CREATE TABLE apartmentInspections
(
    inspectionNo int, /*added this primary key to eliminate any outlying cases such as an inspector inspecting 
                        the same apartmentNo on the same date.*/
    
    apartmentNo int, /*From the book's description, I'm assuming only student apartments, not halls, are inspected*/
    apartmentBldgNo int,
    inspectorNo int,

    inspectionDate date NOT NULL,
    inspectionSatisfied varchar(5) NOT NULL,
        CHECK (inspectionSatisfied IN ('yes','no')),
    comments varchar(5000), /*optional field. Can be NULL*/

    PRIMARY KEY (inspectionNo),

    FOREIGN KEY(inspectorNo) REFERENCES residenceStaff(staffNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(apartmentBldgNo) REFERENCES apartmentBuildings(apartmentBldgNo)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);


--DATA INSERTING===================================================================================================

-- ==========================================================================================================================
INSERT INTO `residencePlaces` (`placeNo`, `monthlyRent`, `roomNo`)
VALUES
/*HALLS------------------------------------------*/
/*hallNo 1*/
(1, 1.00, 1),
(2, 2.00, 2),
(3, 3.00, 3),

/*hallNo 2*/
(4, 4.00, 1),
(5, 5.00, 2),
(6, 6.00, 3),

/*APARTMENTS--------------------------------------*/
/*apartmentBldgNo 1, apartmentNo 1*/
(7, 1.00, 1),
(8, 1.00, 2),
(9, 1.00, 3),
/*apartmentBldgNo 1, apartmentNo 2*/
(10, 1.00, 1),
(11, 1.00, 2),
(12, 1.00, 3),

/*apartmentBldgNo 2, apartmentNo 1*/
(13, 1.00, 1),
(14, 1.00, 2),
(15, 1.00, 3),
/*apartmentBldgNo 2, apartmentNo 2*/
(16, 1.00, 1),
(17, 1.00, 2),
(18, 1.00, 3);


-- ==========================================================================================================================
INSERT INTO `residenceStaff` (`staffNo`, `firstname`, `lastname`, `email`, `addrStreet`, `addrCity`, `addrPostalCode`, `birthDate`, `gender`, `position`, `location`)
VALUES
(1, 'staff1', 'staff1', 'staff1@staff.com', '123 Staff street', 'staff1City', 00001, '1960-01-01', 'male', 'position1', 'location1'),
(2, 'staff2', 'staff2', 'staff2@staff.com', '223 Staff street', 'staff2City', 00002, '2002-02-02', 'male', 'position2', 'location2'),
(3, 'staff3', 'staff3', 'staff3@staff.com', '323 Staff street', 'staff3City', 00003, '2003-03-03', 'male', 'position3', 'location3'),
(4, 'staff4', 'staff4', 'staff4@staff.com', '423 Staff street', 'staff4City', 00004, '2004-04-04', 'male', 'position4', 'location4'),
(5, 'staff5', 'staff5', 'staff5@staff.com', '523 Staff street', 'staff5City', 00005, '2005-05-05', 'male', 'position5', 'location5');


-- ==========================================================================================================================
INSERT INTO `halls` (`hallNo`, `name`, `addrStreet`, `addrCity`, `addrPostalCode`, `phone`, `managerNo`)
VALUES
(1, 'hall1', '123 Hall street', 'hall1City', 00001, '123-456-7890', 1),
(2, 'hall2', '223 Hall street', 'hall2City', 00002, '223-456-7890', 2);


-- ==========================================================================================================================
INSERT INTO `hallPlaces` (`hallNo`, `placeNo`)
VALUES
(1, 1),
(1, 2),
(1, 3),
(2, 4),
(2, 5),
(2, 6);


-- ==========================================================================================================================
INSERT INTO `Adviser` (`adviserNo`, `firstname`, `lastname`, `position`, `department`, `phone`, `email`, `officeRoomNo`)
VALUES
(1, 'adviser1', 'adviser1', 'adviser1', 'advising', '123-456-7890', 'adviser1@adivser.com', 1),
(2, 'adviser2', 'adviser2', 'adviser2', 'advising', '223-456-7890', 'adviser2@adivser.com', 2),
(3, 'adviser3', 'adviser3', 'adviser3', 'advising', '323-456-7890', 'adviser3@adivser.com', 3),
(4, 'adviser4', 'adviser4', 'adviser4', 'advising', '423-456-7890', 'adviser4@adivser.com', 4),
(5, 'adviser5', 'adviser5', 'adviser5', 'advising', '523-456-7890', 'adviser5@adivser.com', 5),
(6, 'adviser6', 'adviser6', 'adviser6', 'advising', '623-456-7890', 'adviser6@adivser.com', 6),
(7, 'adviser7', 'adviser7', 'adviser7', 'advising', '723-456-7890', 'adviser7@adivser.com', 7),
(8, 'adviser8', 'adviser8', 'adviser8', 'advising', '823-456-7890', 'adviser8@adivser.com', 8),
(9, 'adviser9', 'adviser9', 'adviser9', 'advising', '923-456-7890', 'adviser9@adivser.com', 9),
(11, 'adviser11', 'adviser11', 'adviser11', 'advising', '113-456-7890', 'adviser11@adivser.com', 11),
(13, 'adviser13', 'adviser13', 'adviser13', 'advising', '133-456-7890', 'adviser13@adivser.com', 13),
(15, 'adviser15', 'adviser15', 'adviser15', 'advising', '153-456-7890', 'adviser15@adivser.com', 15),
(17, 'adviser17', 'adviser17', 'adviser17', 'advising', '173-456-7890', 'adviser17@adivser.com', 17);


-- ==========================================================================================================================
/*odd numbered students are placed, even number students are waiting*/
INSERT INTO `Student` (`bannerNo`, `firstname`, `lastname`, `addrStreet`, `addrCity`, `addrPostalCode`, `phone`, `email`, `birthDate`, `gender`, `category`, `nationality`, `specialNeeds`, `comments`, `status`, `major`, `minor`, `adviserNo`)
VALUES
(1, 'student1', 'student1', 'student1 street', 'student1 city', 00001, '123-456-7890', 'student1@student.com', '2001-01-01', 'male', 'student odd category', 'student1 nationality', 'student1 special needs', 'student1 comments', 'placed', 'student1 major', 'student1 minor', 1),
(2, 'student2', 'student2', 'student2 street', 'student2 city', 00002, '223-456-7890', 'student2@student.com', '2002-02-02', 'male', 'student even category', 'student2 nationality', 'student2 special needs', 'student2 comments', 'waiting', 'student2 major', 'student2 minor', 2),
(3, 'student3', 'student3', 'student3 street', 'student3 city', 00003, '323-456-7890', 'student3@student.com', '2003-03-03', 'male', 'student odd category', 'student3 nationality', 'student3 special needs', 'student3 comments', 'placed', 'student3 major', 'student3 minor', 3),
(4, 'student4', 'student4', 'student4 street', 'student4 city', 00004, '423-456-7890', 'student4@student.com', '2004-04-04', 'male', 'student even category', 'student4 nationality', 'student4 special needs', 'student4 comments', 'waiting', 'student4 major', 'student4 minor', 4),
(5, 'student5', 'student5', 'student5 street', 'student5 city', 00005, '523-456-7890', 'student5@student.com', '2005-05-05', 'male', 'student odd category', 'student5 nationality', 'student5 special needs', 'student5 comments', 'placed', 'student5 major', 'student5 minor', 5),
(6, 'student6', 'student6', 'student6 street', 'student6 city', 00006, '623-456-7890', 'student6@student.com', '2006-06-06', 'male', 'student even category', 'student6 nationality', 'student6 special needs', 'student6 comments', 'waiting', 'student6 major', 'student6 minor', 6),
(7, 'student7', 'student7', 'student7 street', 'student7 city', 00007, '723-456-7890', 'student7@student.com', '2007-07-07', 'male', 'student odd category', 'student7 nationality', 'student7 special needs', 'student7 comments', 'placed', 'student7 major', 'student7 minor', 7),
(8, 'student8', 'student8', 'student8 street', 'student8 city', 00008, '823-456-7890', 'student8@student.com', '2008-08-08', 'male', 'student even category', 'student8 nationality', 'student8 special needs', 'student8 comments', 'waiting', 'student8 major', 'student8 minor', 8),
(9, 'student9', 'student9', 'student9 street', 'student9 city', 00009, '923-456-7890', 'student9@student.com', '2009-09-09', 'male', 'student odd category', 'student9 nationality', 'student9 special needs', 'student9 comments', 'placed', 'student9 major', 'student9 minor', 9),
(11, 'student11', 'student11', 'student11 street', 'student11 city', 00011, '113-456-7890', 'student11@student.com', '2011-11-11', 'male', 'student odd category', 'student11 nationality', 'student11 special needs', 'student11 comments', 'placed', 'student11 major', 'student11 minor', 11),
(13, 'student13', 'student13', 'student13 street', 'student13 city', 00013, '133-456-7890', 'student13@student.com', '2013-12-13', 'male', 'student odd category', 'student13 nationality', 'student13 special needs', 'student13 comments', 'placed', 'student13 major', 'student13 minor', 13),
(15, 'student15', 'student15', 'student15 street', 'student15 city', 00015, '153-456-7890', 'student15@student.com', '2015-12-15', 'male', 'student odd category', 'student15 nationality', 'student15 special needs', 'student15 comments', 'placed', 'student15 major', 'student15 minor', 15),
(17, 'student17', 'student17', 'student17 street', 'student17 city', 00017, '173-456-7890', 'student17@student.com', '2017-12-17', 'male', 'student odd category', 'student17 nationality', 'student17 special needs', 'student17 comments', 'placed', 'student17 major', 'student17 minor', 17);


-- ==========================================================================================================================
INSERT INTO `nextOfKin` (`kinNo`, `bannerNo`, `firstname`, `lastname`, `relationship`, `addrStreet`, `addrCity`, `addrPostalCode`, `phone`)
VALUES
(1, 1, 'student1 kin', 'student1 kin', 'student1 kin', 'student1 kin street', 'student1 kin city', 00001, '123-456-7890'),
(2, 2, 'student2 kin', 'student2 kin', 'student2 kin', 'student2 kin street', 'student2 kin city', 00002, '223-456-7890'),
(3, 3, 'student3 kin', 'student3 kin', 'student3 kin', 'student3 kin street', 'student3 kin city', 00003, '323-456-7890'),
(4, 4, 'student4 kin', 'student4 kin', 'student4 kin', 'student4 kin street', 'student4 kin city', 00004, '423-456-7890'),
(5, 5, 'student5 kin', 'student5 kin', 'student5 kin', 'student5 kin street', 'student5 kin city', 00005, '523-456-7890'),
(6, 6, 'student6 kin', 'student6 kin', 'student6 kin', 'student6 kin street', 'student6 kin city', 00006, '623-456-7890'),
(7, 7, 'student7 kin', 'student7 kin', 'student7 kin', 'student7 kin street', 'student7 kin city', 00007, '723-456-7890'),
(8, 8, 'student8 kin', 'student8 kin', 'student8 kin', 'student8 kin street', 'student8 kin city', 00008, '823-456-7890'),
(9, 9, 'student9 kin', 'student9 kin', 'student9 kin', 'student9 kin street', 'student9 kin city', 00009, '923-456-7890'),
/*(11, 11, 'student11 kin', 'student11 kin', 'student11 kin', 'student11 kin street', 'student11 kin city', 00011, '113-456-7890'),
(13, 13, 'student13 kin', 'student13 kin', 'student13 kin', 'student13 kin street', 'student13 kin city', 00013, '133-456-7890'),
(15, 15, 'student15 kin', 'student15 kin', 'student15 kin', 'student15 kin street', 'student15 kin city', 00015, '153-456-7890'),*/
(17, 17, 'student17 kin', 'student17 kin', 'student17 kin', 'student17 kin street', 'student17 kin city', 00017, '173-456-7890');


-- ==========================================================================================================================
/*odd numbered students are placed, even number students are waiting*/
INSERT INTO `Lease` (`leaseNo`, `bannerNo`, `placeNo`, `startDate`, `endDate`, `duration`, `fallSem`, `springSem`, `summerSem`)
VALUES
(1, 1, 1, '2021-08-01', '2022-01-01', 1, 'yes', 'no', 'no'),
(3, 3, 3, '2021-08-01', '2022-03-31', 2, 'yes', 'yes', 'no'),
(5, 5, 5, '2021-01-01', '2022-01-01', 3, 'yes', 'yes', 'yes'),
(7, 7, 7, '2021-01-01', '2022-01-01', 3, 'yes', 'yes', 'yes'),
(9, 9, 9, '2021-01-01', '2022-01-01', 3, 'yes', 'yes', 'yes'),
(11, 11, 11, '2021-01-01', '2022-01-01', 3, 'yes', 'yes', 'yes'),
(13, 13, 13, '2021-01-01', '2022-01-01', 3, 'yes', 'yes', 'yes'),
(15, 15, 15, '2021-01-01', '2022-01-01', 3, 'yes', 'yes', 'yes'),
(17, 17, 17, '2021-01-01', '2022-01-01', 3, 'yes', 'yes', 'yes');


-- ==========================================================================================================================
/*odd numbered students are placed, even number students are waiting*/
/*each "paymentDue" is 3.00 b/c monthly rent is 1.00 and there are 3 months in a semester*/
INSERT INTO `Invoice` (`invoiceNo`, `leaseNo`,  `bannerNo`, `placeNo`, `semester`, `paymentDue`, `payment`, `paymentDate`, `paymentMethod`, `firstReminderDate`, `secondReminderDate`)
VALUES
(1, 1, 1, 1, 'fall', 3.00, 3.00, '2021-12-01', 'cash', null, null),
(2, 1, 1, 1, 'spring', 3.00, 3.00, '2021-12-01', 'cash', null, null),

(3, 3, 3, 3, 'fall', 3.00, 3.00, '2021-12-03', 'cash', null, null),
(5, 5, 5, 5, 'fall', 3.00, 3.00, '2021-12-05', 'cash', null, null),
(7, 7, 7, 7, 'fall', 3.00, 3.00, '2021-12-07', 'cash', null, null),
(9, 9, 9, 9, 'fall', 3.00, 3.00, '2021-12-09', 'cash', null, null),
(11, 11, 11, 11, 'fall', 3.00, 3.00, '2021-12-11', 'cash', null, null),
(13, 13, 13, 13, 'fall', 3.00, 3.00, '2021-12-13', 'cash', null, null),
(15, 15, 15, 15, 'fall', 3.00, 3.00, '2021-12-15', 'cash', null, null),
(17, 17, 17, 17, 'fall', 3.00, 0.00, null, null, null, null);


-- ==========================================================================================================================
INSERT INTO `apartmentBuildings` (`apartmentBldgNo`, `addrStreet`, `addrCity`, `addrPostalCode`, `numOfRooms`)
VALUES
-- (1, 'apartmentBldgNo1 street', 'apartmentBldgNo1 city', 00001, 6),
(1, '1678 Cardiff Rd', 'apartmentBldgNo1 city', 00001, 6),

(2, 'apartmentBldgNo2 street', 'apartmentBldgNo2 city', 00002, 6);
-- (2, '1678 Cardiff Rd', 'apartmentBldgNo2 city', 00002, 6);


-- ==========================================================================================================================
INSERT INTO `apartmentPlaces` (`apartmentNo`, `apartmentBldgNo`, `placeNo`)
VALUES
(1, 1, 7),
(1, 1, 8),
(1, 1, 9),

(2, 1, 10),
(2, 1, 11),
(2, 1, 12),

(1, 2, 13),
(1, 2, 14),
(1, 2, 15),

(2, 2, 16),
(2, 2, 17),
(2, 2, 18);


-- ==========================================================================================================================
INSERT INTO `apartmentInspections` (`inspectionNo`, `apartmentNo`, `apartmentBldgNo`, `inspectorNo`, `inspectionDate`, `inspectionSatisfied`, `comments`)
VALUES
(1, 1, 1, 1, '2021-01-01', 'no', 'Common kitchen area was covered in feces. Called exocist!'),
(2, 2, 2, 2, '2021-02-02', 'yes', null);