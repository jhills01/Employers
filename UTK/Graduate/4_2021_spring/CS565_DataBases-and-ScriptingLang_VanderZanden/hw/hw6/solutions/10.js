db.students.aggregate(
	[	
		{
			$project: {
						_id: 0, firstname: 1, lastname: 1,  
						avgExam : {$avg : "$exams.score"}  
					  }
		},	

		// { $unwind: "$exams" },

		// {			
			// $group: {
						// _id: "$email", 
						// lastname: {$first: "$lastname"},
						// firstname: {$first: "$firstname"},
						// avgExam: {$avg : "$exams.score"}  
					// }
		// },	
		
		{ $sort: {avgExam: -1} }
	]
);

// print("\n\n");
// db.students.find(
	// {},
	// {_id: 0, lastname: 1, firstname: 1, "exams.score" : 1}
// );
