db.students.find(
	{$or: 
		[
			{ "report.score" : {$gte: 60, $lte: 80}, "report.ta" : "Nancy Hew" },
			{ "report.score" : {$gte: 80, $lte: 100}, "report.ta" : "Nancy Hew" }
		]
	},
	
	{ _id: 0, firstname: 1,  lastname: 1, "report.score": 1 }
	//{ _id: 0, lastname: 1, firstname: 1, "report.score": 1 , "report.ta": 1}
);
