// print("BEFORE\n");
// db.students.find(
	// {lastname: "vander zanden", firstname: "smiley"},
	// {_id: 0, lastname: 1, firstname: 1, exams : 1}
// ).pretty();
// print("\n\n");


db.students.updateMany(	
	{
		email: "smiley@utk.edu"
		//lastname: "vander zanden", firstname: "smiley"		
	},

	{
		$push: {exams: {score: 95, ta: "Jeff Hardy"} }
	},
);

// print("\nAFTER\n");
// db.students.find(
	// {lastname: "vander zanden", firstname: "smiley"},
	// {_id: 0, lastname: 1, firstname: 1, exams : 1}
// ).pretty();

db.students.find(
	{email: "smiley@utk.edu"},
	//{lastname: "vander zanden", firstname: "smiley"},
	{_id: 0 }
);
