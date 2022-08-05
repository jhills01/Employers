students =
[
	{
		lastname: "duck", 
		firstname: "donald",
		dateOfBirth: new Date("1993-03-17"), 
		email: "dduck@disney.com",
		grade: "C",
		major: "MATH",
		quizzes : [43, 71, 53, 100, 0, 32, 31, 41],
		report : { score: 71, ta: "Clark Richards" },
		exams : 
			[ 		
				{ score: 58, ta: "Nancy Hew", comments: "Need to step it up" },
				{ score: 69, ta: "Clark Richards", comments: "Still not passing" },
				{ score: 45, ta: "Nancy Hew" },
				{ score: 33, ta: "Nancy Hew" },
				{ score: 61, ta: "Clark Richards", comments: "Stop laying eggs on these exams" }
			]
	}
]

db.students.insertMany(students);

db.students.find({ email : "dduck@disney.com" }, { _id : 0 });
