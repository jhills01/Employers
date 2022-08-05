DROP TABLE IF EXISTS OutcomeResults;
DROP TABLE IF EXISTS Assessments;
DROP TABLE IF EXISTS Narratives;
DROP TABLE IF EXISTS CourseOutcomeMapping;

DROP TABLE IF EXISTS PerformanceLevels;

DROP TABLE IF EXISTS Sections;
DROP TABLE IF EXISTS Instructors;
DROP TABLE IF EXISTS Courses;
DROP TABLE IF EXISTS Outcomes;

/* IMPORTANT NOTE: (https://www.informit.com/articles/article.aspx?p=377068&seqNum=12)
ON DELETE enables you to specify what happens to the child table when parent table records are deleted. The 
default if no ON DELETE clause is present is to reject any attempt to delete rows in the parent table that 
have child rows pointing to them. To specify an action value explicitly, use one of the following clauses:

    ON DELETE NO ACTION and ON DELETE RESTRICT are the same as omitting the ON DELETE clause. (Some database 
    systems have deferred checks, and NO ACTION is a deferred check. In MySQL, foreign key constraints are 
    checked immediately, so NO ACTION and RESTRICT are the same.)

    ON DELETE CASCADE causes matching child records to be deleted when the corresponding parent record is 
    deleted. In essence, the effect of the delete is cascaded from the parent to the child. This allows you to 
    perform multiple-table deletes by deleting rows only from the parent table and letting InnoDB take care of 
    deleting rows from the child table.

    ON DELETE SET NULL causes index columns in matching child records to be set to NULL when the parent record 
    is deleted. If you use this option, all the indexed child table columns named in the foreign key 
    definition must be defined to allow NULL values. (One implication of using this action is that you cannot 
    define the foreign key to be a PRIMARY KEY because primary keys do not allow NULL values.)

    ON DELETE SET DEFAULT is recognized but unimplemented and InnoDB issues an error.

ON UPDATE enables you to specify what happens to the child table when parent table records are updated. The 
default if no ON UPDATE clause is present is to reject any inserts or updates in the child table that result 
in foreign key values that don't have any match in the parent table index, and to prevent updates to parent 
table index values to which child rows point. The possible action values are the same as for ON DELETE and 
have similar effects.
*/

--Instructors------------------------------------------------------------------------------------------------
--"email" == instructor's username
CREATE TABLE Instructors
(
    instructorId int,
    firstname varchar(1000) NOT NULL,
	lastname varchar(1000) NOT NULL,
	email varchar(1000) NOT NULL,
	password varchar(1000) NOT NULL,
	
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

    /*Important: (see detailed explanation above)
    1) ON DELETE/ON UPDATE == whether referenced primary key in parent table is DELETED/UPDATED
    2) NO ACTION/CASCADE/SET NULL/SET DEFAULT == what happens to primary key's referenced children 
       in this table (i.e., foreign key matches)! Note, NO ACTION prevents referenced  */

    FOREIGN KEY(courseId) REFERENCES Courses(courseId)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(instructorId) REFERENCES Instructors(instructorId)
        ON DELETE NO ACTION
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
    
    /*Important: (see detailed explanation above)
    1) ON DELETE/ON UPDATE == whether referenced primary key in parent table is DELETED/UPDATED
    2) NO ACTION/CASCADE/SET NULL/SET DEFAULT == what happens to primary key's referenced children 
       in this table (i.e., foreign key matches)! Note, NO ACTION prevents referenced PK */

    FOREIGN KEY(sectionId) REFERENCES Sections(sectionId)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(outcomeId, major) REFERENCES Outcomes(outcomeId, major)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(performanceLevel) REFERENCES PerformanceLevels(performanceLevel)
        ON DELETE NO ACTION
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

    /*Important: (see detailed explanation above)
    1) ON DELETE/ON UPDATE == whether referenced primary key in parent table is DELETED/UPDATED
    2) NO ACTION/CASCADE/SET NULL/SET DEFAULT == what happens to primary key's referenced children 
       in this table (i.e., foreign key matches)! Note, NO ACTION prevents referenced  */

    FOREIGN KEY(sectionId) REFERENCES Sections(sectionId)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(outcomeId, major) REFERENCES Outcomes(outcomeId, major)
        ON DELETE NO ACTION
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

    /*Important: (see detailed explanation above)
    1) ON DELETE/ON UPDATE == whether referenced primary key in parent table is DELETED/UPDATED
    2) NO ACTION/CASCADE/SET NULL/SET DEFAULT == what happens to primary key's referenced children 
       in this table (i.e., foreign key matches)! Note, NO ACTION prevents referenced  */

    FOREIGN KEY(sectionId) REFERENCES Sections(sectionId)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(outcomeId, major) REFERENCES Outcomes(outcomeId, major)
        ON DELETE NO ACTION
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
    
    /*Important: (see detailed explanation above)
    1) ON DELETE/ON UPDATE == whether referenced primary key in parent table is DELETED/UPDATED
    2) NO ACTION/CASCADE/SET NULL/SET DEFAULT == what happens to primary key's referenced children 
       in this table (i.e., foreign key matches)! Note, NO ACTION prevents referenced  */
    
    FOREIGN KEY(courseId) REFERENCES Courses(courseId)
        ON DELETE NO ACTION
        ON UPDATE CASCADE,
    FOREIGN KEY(outcomeId, major) REFERENCES Outcomes(outcomeId, major)
        ON DELETE NO ACTION
        ON UPDATE CASCADE
);
