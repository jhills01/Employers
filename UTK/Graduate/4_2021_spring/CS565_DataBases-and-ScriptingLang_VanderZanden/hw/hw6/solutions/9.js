db.students.aggregate(
	[	
		{
			$project: {
						_id: 0, firstname: 1, lastname: 1,  
						min : {$min : "$quizzes"},
						max 	: {$max : "$quizzes"}, 
						avgQuiz : {$avg : "$quizzes"}  
					  }
		}
	
		// { 	$unwind: "$quizzes"},
			
		// {
			// $group: {
						// _id: "$email", 
						// lastname: {$first: "$lastname"},
						// firstname: {$first: "$firstname"},
						// min : {$min : "$quizzes"},
						// max 	: {$max : "$quizzes"}, 
						// avgQuiz : {$avg : "$quizzes"}  
					// }
		// },		
	]
);

// print("\n\n");
// db.students.find(
	// {},
	// {_id: 0, lastname: 1, firstname: 1, quizzes : 1}
// );
