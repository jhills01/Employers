db.students.find(
	{ grade: { $in: [ "A", "A+", "A-" ] }, quizzes: { $elemMatch: {$lt: 50} } },	
	{ _id: 0, firstname: 1, lastname: 1, quizzes: 1 }
);
