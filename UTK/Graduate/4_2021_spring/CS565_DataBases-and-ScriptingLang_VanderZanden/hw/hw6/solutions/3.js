db.students.find(
	{ dateOfBirth: {$gte: new Date("2000-01-01"), $lt: new Date("2001-01-01")} },
	{ _id: 0, firstname: 1, lastname: 1, dateOfBirth: 1, grade: 1 }
);
