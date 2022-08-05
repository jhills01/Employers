DROP TABLE IF EXISTS OutcomeResults;
DROP TABLE IF EXISTS Assessments;
DROP TABLE IF EXISTS Narratives;
DROP TABLE IF EXISTS CourseOutcomeMapping;

DROP TABLE IF EXISTS PerformanceLevels;

DROP TABLE IF EXISTS Sections;
DROP TABLE IF EXISTS Instructors;
DROP TABLE IF EXISTS Courses;
DROP TABLE IF EXISTS Outcomes;

--Instructors------------------------------------------------------------------------------------------------
--"email" == instructor's username
CREATE TABLE Instructors
(
    instructorId int,
    firstname varchar(1000),
	lastname varchar(1000),
	email varchar(1000),
	password varchar(1000),
	
    PRIMARY KEY (instructorId)
);

--Courses------------------------------------------------------------------------------------------------
CREATE TABLE Courses
(
    courseId varchar(1000),
    courseTitle varchar(1000) NOT NULL,
	
    PRIMARY KEY (courseId)
);

--Sections------------------------------------------------------------------------------------------------
CREATE TABLE Sections
(
    sectionId int,
    courseId varchar(1000),
    instructorId int,
    semester varchar(1000) NOT NULL,
    year int NOT NULL,
	
    PRIMARY KEY (sectionId),

    FOREIGN KEY(courseId) REFERENCES Courses(courseId)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY(instructorId) REFERENCES Instructors(instructorId)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

--Outcomes------------------------------------------------------------------------------------------------
CREATE TABLE Outcomes
(
    outcomeId int,
    outcomeDescription varchar(1000) NOT NULL,
    major ENUM('CS','CpE','EE'),
	
    PRIMARY KEY (outcomeId, major)
);

--PerformanceLevels------------------------------------------------------------------------------------------------
CREATE TABLE PerformanceLevels
(
    performanceLevel int AUTO_INCREMENT,
    description varchar(1000) NOT NULL
        CHECK (description IN ('Not Meets Expectations', 'Meets Expectations', 'Exceeds Expectations')),
	
    PRIMARY KEY (performanceLevel)
);

--OutcomeResults------------------------------------------------------------------------------------------------
CREATE TABLE OutcomeResults
(
    sectionId int,
    outcomeId int,
    major ENUM('CS','CpE','EE'),
    performanceLevel int,
    numberOfStudents int NOT NULL,
    
    FOREIGN KEY(sectionId) REFERENCES Sections(sectionId)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY(outcomeId, major) REFERENCES Outcomes(outcomeId, major)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY(performanceLevel) REFERENCES PerformanceLevels(performanceLevel)
        ON DELETE CASCADE
        ON UPDATE CASCADE    
);

--Assessments------------------------------------------------------------------------------------------------
CREATE TABLE Assessments
(
    assessmentId int AUTO_INCREMENT,
    sectionId int,
    assessmentDescription varchar(1000) NOT NULL,
    major ENUM('CS','CpE','EE'),
    outcomeId int,
    weight int NOT NULL
        CHECK (weight BETWEEN 0 AND 100),

    PRIMARY KEY (assessmentId),

    FOREIGN KEY(sectionId) REFERENCES Sections(sectionId)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY(outcomeId, major) REFERENCES Outcomes(outcomeId, major)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

--Narratives------------------------------------------------------------------------------------------------
CREATE TABLE Narratives
(
    sectionId int,
    major ENUM('CS','CpE','EE'),
    outcomeId int,
    strengths varchar(10000) NOT NULL,
    weaknesses varchar(10000) NOT NULL,
    actions varchar(10000), /*the only 'optional' attribute that could be NULL in entire database*/

    FOREIGN KEY(sectionId) REFERENCES Sections(sectionId)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY(outcomeId, major) REFERENCES Outcomes(outcomeId, major)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);

--CourseOutcomeMapping------------------------------------------------------------------------------------------------
CREATE TABLE CourseOutcomeMapping
(
    courseId varchar(1000),
    outcomeId int,
    major ENUM('CS','CpE','EE'),
    semester varchar(1000) NOT NULL,
    year int NOT NULL,
    
    FOREIGN KEY(courseId) REFERENCES Courses(courseId)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
    FOREIGN KEY(outcomeId, major) REFERENCES Outcomes(outcomeId, major)
        ON DELETE CASCADE
        ON UPDATE CASCADE
);
