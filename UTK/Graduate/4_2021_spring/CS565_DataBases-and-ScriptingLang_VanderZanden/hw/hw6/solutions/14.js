// print("BEFORE\n");
// db.students.find(
	// {lastname: "mouse", firstname: "minnie"},
	// {_id: 0, lastname: 1, firstname: 1, exams : 1}
// ).pretty();
// print("\n\n");

db.students.updateMany(	
	{
		email: "mmouse@disney.com"
		//lastname: "mouse", firstname: "minnie"		
	},

	{
		$set: 
		{ 
			"exams.0.score": 86,
			"exams.2.score": 100,
			"exams.2.ta": "Nancy Hew"
			
			// //This actually replaced the entire object instead of the specific object members. Ooops
			// "exams.0": {score: 86},
			// "exams.2": {score: 100, ta: "Nancy Hew"} 
		}
	},
);

// print("\nAFTER\n");
// db.students.find(
	// {lastname: "mouse", firstname: "minnie"},
	// {_id: 0, lastname: 1, firstname: 1, exams : 1}
// ).pretty();

db.students.find(
	{email: "mmouse@disney.com"},
	//{lastname: "mouse", firstname: "minnie"},
	{_id: 0 }
);
