db.students.find(
	//{grade: "A"},
	
	{grade: { $in: [ "A", "A+", "A-" ] } },
	{_id: 0, firstname: 1, lastname: 1}
);

// print("\n\n");
// db.students.find(
	// {},
	// {_id: 0, lastname: 1, firstname: 1, grade: 1}
// );