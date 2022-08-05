--
-- Dumping data for table `Instructors`
--

-- INSERT INTO `Instructors` (`instructorId`, `firstname`, `lastname`, `email`, `password`) VALUES
-- (12, 'Brad', 'Vander Zanden', 'bvanderz@utk.edu', '*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19'),
-- (13, 'Wily', 'Coyote', 'coyote@utk.edu', '*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19'),
-- (14, 'Michael', 'Berry', 'mberry@utk.edu', '*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19'),
-- (15, 'Mia', 'Test User', 'testuser@utk.edu', '*2470C0C06DEE42FD1618BB99005ADCA2EC9D1E19');

-- --------------------------------------------------------

--
-- Dumping data for table `Sections`
--

INSERT INTO `Sections` (`sectionId`, `instructorId`, `courseId`, `semester`, `year`) VALUES
(10, 13, 'COSC402', 'spring', 2018),
(11, 13, 'COSC402', 'fall', 2018),
(12, 13, 'COSC402', 'fall', 2017),
(13, 13, 'COSC402', 'spring', 2017);
-- (1, 13, 'COSC402', 'spring', 2019),
-- (2, 13, 'COSC402', 'fall', 2019),
-- (3, 12, 'COSC365', 'spring', 2019),
-- (4, 14, 'COSC402', 'spring', 2019),
-- (5, 15, 'COSC402', 'spring', 2019),
-- (6, 12, 'ECE351', 'spring', 2019),
-- (7, 13, 'ECE336', 'spring', 2019),
-- (8, 12, 'COSC365', 'fall', 2019),
-- (9, 12, 'COSC402', 'spring', 2020);


