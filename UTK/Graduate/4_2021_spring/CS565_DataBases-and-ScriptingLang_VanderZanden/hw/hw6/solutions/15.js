// print("BEFORE\n");
// db.students.find(
	// {lastname: "ape", firstname: "great"},
	// {_id: 0, lastname: 1, firstname: 1}
// ).pretty();
// print("\n\n");

db.students.deleteMany(	
	{
		email: "gape@warner.com"
		//lastname: "ape", firstname: "great"		
	}
);

// print("\nAFTER\n");
// db.students.find(
	// {lastname: "ape", firstname: "great"},
	// {_id: 0, lastname: 1, firstname: 1}
// ).pretty();

db.students.find(
	{email: "gape@warner.com"},
	//{lastname: "ape", firstname: "great"},
	{_id: 0 }
);
