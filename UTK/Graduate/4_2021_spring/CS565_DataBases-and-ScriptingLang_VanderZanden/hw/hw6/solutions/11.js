db.students.aggregate(
	[	
		{
			$match: { 
						$or: 
						[
							//Probably should have written this in one large array...
							//...instead of OR'ing grade multiple times. But this looks better :)
							{grade: { $in: [ "C", "C+", "C-" ] }},
							{grade: { $in: [ "D", "D+", "D-" ] }},
							{grade: { $in: [ "F", "F+", "F-" ] }}
						]
					} 		
		},
	
		//The "count: {$sum: 1}" line is how you actually count items in each group!
		{
			$group: { 
						_id : null, 
						count : {$sum : 1} 
					} 	
		},

		{
			$project: {_id: 0, count: 1}
		}
	]
);

// print("\n\n");
// db.students.find(
	// { grade: { $in: [ "C", "C+", "C-", "D", "D+", "D-", "F", "F+", "F-" ] } },
	// {_id: 0, lastname: 1, firstname: 1, "grade" : 1}
// );

// //NOTE: This works good too. However, DON'T USE $group AND $count IN THE SAME PIPELINE.
// //	   It can lead to trouble.
// db.students.aggregate(
  // [
    // {$match: {grade: {$in: ["C", "D", "F"]}}},

    // {$count: "count"}
  // ]
// );
