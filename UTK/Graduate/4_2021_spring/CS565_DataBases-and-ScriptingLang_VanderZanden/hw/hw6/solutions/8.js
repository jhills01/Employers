db.students.aggregate(
	[	
		{ 
			$group: { 
						_id : "$major", 
						count : {$sum : 1} 
					} 
		}
	]
);

// print("\n\n");
// db.students.find(
	// {},
	// {lastname: 1, firstname: 1, major : 1}
// ).sort({major : 1});