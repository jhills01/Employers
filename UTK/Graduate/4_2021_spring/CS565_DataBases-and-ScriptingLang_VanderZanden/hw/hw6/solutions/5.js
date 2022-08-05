db.students.find(
	{ exams: { $elemMatch: {ta: "Jeff Hardy"} } },	
	{ _id: 0, firstname: 1, lastname: 1, exams: 1 }
).sort( 
	{ lastname: 1, firstname: -1 }
);

