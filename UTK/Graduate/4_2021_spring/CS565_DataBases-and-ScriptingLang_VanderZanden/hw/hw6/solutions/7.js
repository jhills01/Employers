db.students.find(
	{ "report.comments" : { $exists : false } },
	{ _id: 0, firstname: 1, lastname: 1, report: 1 }
);

// print("\n\n");
// db.students.find(
	// {},
	// {_id: 0, lastname: 1, firstname: 1, report : 1}
// );