// print("BEFORE\n");
// db.students.find(
	// {email: "mickey@disney.com"},
	// // {lastname: "mouse", firstname: "mickey"},
	// {_id: 0, lastname: 1, firstname: 1, "grade" : 1, "report.ta": 1}
// );
// print("\n\n");

db.students.updateMany(	
	{
		email: "mickey@disney.com"
		//lastname: "mouse", firstname: "mickey"		
	},

	{
		$set: {grade: "A", "report.ta": "Clark Richards"}
	},
);

// print("\nAFTER\n");
// db.students.find(
	// {email: "mickey@disney.com"},
	// // {lastname: "mouse", firstname: "mickey"},
	// {_id: 0, lastname: 1, firstname: 1, "grade" : 1, "report.ta": 1}
// );

db.students.find(
	{email: "mickey@disney.com"},
	{_id: 0 }
);
