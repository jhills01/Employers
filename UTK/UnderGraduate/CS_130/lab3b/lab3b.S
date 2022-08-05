//John Hills
//lab3b

.section .text
.global Sum, Search, Median, MinMax

Search:	
//PERSON *Search(PERSON people[], int num_people, const char *name, int age, char gender);
//a0		     a0				, a1		    , a2			  , a3	   , a4

//NOTE: sizeof(PERSON) == 60 bytes

//FOR loop to compares each PERSON with arguments a2 thru a4 
	li		t0, 0			//i == 0, initializer.
	addi	sp, sp, -16		//creating room to store and load a0 and a2 memory addresses

loop:

	bge		t0, a1, null_ret		//if i >= num_people, then exit Search and return NULL to cpp because nobody was found.

	//store memory addresses for easy reset back to the first address of PERSON being compared. 
	sd		a0, 0(sp)
	sd		a2, 8(sp)

//WHILE loop to compare char by char, and break when not equal or if  null terminator, '\0', is reached in both on the same index.
1:
	lb		t3, 0(a0)		//loads single char from name of a0
	lb		t4, 0(a2)		//loads single char from name of a2

	//Char to char comparison of a0 and a2
	beq		t3, t4, chars_equal		//if not equal, switch to next PERSON and start for loop over again.
	
	//adds 1 to loop incrementor
	addi	t0, t0, 1
	
	//resets pointers of a0 and a2 to point back to the first address of PERSON being compared.
	ld		a0, 0(sp)
	ld		a2, 8(sp)
	//moves pointer to next PERSON
	addi	a0, a0, 60

	j		loop

chars_equal:
	
	beqz	t3, continue	//since t3 equals t4 at this point, and if t3 is null terminator, then both strings are at their end and we continue.

	//add 1 byte to both memory addresses to access next char in both
	addi	a0, a0, 1
	addi	a2, a2, 1
	
	j		1b

continue:
	
	//reset pointer of a0 and a2 to point back to the first address of PERSON being compared.
	ld		a0, 0(sp)
	ld		a2, 8(sp)
	
	//loads information from people[] for comparision with other arguments
	lw		t1, 52(a0)		//loads age from a0.
	lb		t2, 56(a0)		//loads gender from a0.
	
	//comparisons. 
1:
	beq		t1, a3, 1f	
	addi	t0, t0, 1
	addi	a0, a0, 60
	
	j		loop
1:
	beq		t2, a4, 1f	
	addi	t0, t0, 1
	addi	a0, a0, 60
	
	j		loop	

//getting here means a match was found and should be returned.
1:

	addi	sp, sp, 16

	ret

null_ret://if no match was found, return nullptr
	
	addi	sp, sp, 16
	mv		a0, zero

	ret		

//*********************************************************************************************************************************************************************

Sum:
//int Sum(const int array[], int array_size);
//a0	  a0			   , a1	

	//Need for loop to sum all elements. 
	lw		t1, 0(a0)	//sum == first value in array[]
	li		t0, 1		//i == 1, initializer. Initialized to 1 because I've already loaded the first value of array into sum (i.e., don't need i == 0 iteration)

1: //condition check of FOR loop	

	bge		t0, a1, 2f  // if i >= array_size, break loop and return sum 

	addi	a0, a0, 4	//move memory address in a0 down 4 bytes
	lw		t3, 0(a0)	//loads a0 value from new memory address
				
	add		t1, t1, t3	//adds new value to sum
	addi	t0, t0, 1	//i++

	j		1b			//go back and perform conditon check of FOR loop

2: //loop finished. returning sum

	mv		a0, t1
	ret

//*********************************************************************************************************************************************************************
Median:

	//NOTE: Have to copy array into buffer, and then sort buffer. array is const and therefore is read-only and can't be overwritten.

	//int Median(const int array[], int array_size, int *buffer);	
	//a0		 a0				  , a1			  , a2

	//FOR loop to copy array to buffer
	li		t0, 0			//i == 0, initializer
	addi	sp, sp, -16		//creating room to store and load a0 and a2 initial memory addresses for easy reset after moving pointers.
	
	//storing initial memory addresses for quick reset of pointers
	sd		a0, 0(sp)
	sd		a2, 8(sp)

1:	
	bge		t0, a1, bubble	//if i >= array_size, then FOR loop is done and move on to bubble sort algorithm
	
	//copy individual elements from array to buffer, move pointers for array and buffer to next element, increment i, and go back to check loop condition
	lw		t1, 0(a0)
	sw		t1, 0(a2)
	addi	a0, a0, 4
	addi	a2, a2, 4
	addi	t0, t0, 1
	j		1b

bubble:
	//reset pointers of array and buffer to first element.
	ld		a0, 0(sp)
	ld		a2, 8(sp)
	
	li		t0, 0			//reset i, loop initializer
	li		t1, 1			//bool value where 1 == not sorted(bubble sort loop continues), and 0 == sorted (break bubble loop).
//	li		t2, 0			//temporary storage during swap --- PROBABLY NOT NEEDED AND CAN DELETE WHEN FINISHED.		
	addi	t3, a1, -1		//inner for loop of bubble is checked against "array_size - 1" b/c I compare [i] and [i+1] elements of buffer.

while://Outer WHILE loop

	beqz	t1, break_bubble		//if t1 == 0, then goto break_loop because buffer is sorted

	ld		a2, 8(sp)		//reset a2 pointer to original memory location in preparation for another FOR loop
	li		t0, 0			//reset i, loop initializer
	li		t1, 0			//sets bool value to false, a condition that would break_loop if nothing is swapped

for://inner FOR loop

	bge		t0, t3, while	//if i >= "array_size - 1", then break FOR loop and check WHILE condition again.
	
	//load adjacent elements of buffer for comparison	
	lw		t4, 0(a2)		//load buffer[i]
	lw		t5, 4(a2)		//load buffer[i+1]
	
	
	bgt		t4, t5, if_true	//if buffer[i] > buffer[i+1], the if statement is true	

	addi	t0, t0, 1
	addi	a2, a2, 4		//prepares FOR loop for next set of adjacent elements to be compared
	
	j		for

if_true:

	sw		t5, 0(a2)		//stores buffer[i+1] into memory location previously containing buffer[i] 
	sw		t4, 4(a2)		//stores buffer[i] into memory location previously containing buffer[i+1] 
	li		t1, 1			//change bool value to true
	addi	a2, a2, 4		//prepares FOR loop for next set of adjacent elements to be compared
	addi	t0, t0, 1		//i++

	j		for

break_bubble:

	ld		a2, 8(sp)		//reset a2 pointer to original memory location

	//checking to see if array_size is odd or even
	li		t0, 2			//a literal that will be used in all calculations
	rem		t1, a1, t0 		
	
	beqz	t1, even
	
	//t1 is odd
	div		t2, a1, t0		//perform integer division by dividing array_size by 2 and use the result as the array index of the median value
	slli	t3, t2, 2		//multiply median index by 4 to get the number of bytes needed to reach median value by adding to buffer starting address
	add		a2, a2, t3		//add bits to starting buffer address
	lw		a0, 0(a2)		//load median value into return value a0

	j		ret

even:
	
	//For even numbers of elements, the average of the middle two will be needed. Of those two, lower index == LI and upper index == UI
	div		t2, a1, t0		//integer division gives me UI
	addi	t2, t2, -1		//change to LI
	slli	t3, t2, 2		//multiplies LI by 4 bytes
	add		a2, a2, t3
	//get average and return it
	lw		a0, 0(a2)
	lw		a1, 4(a2)
	add		a0, a0, a1
	div		a0, a0, t0

ret:

	addi	sp, sp, 16

	ret

////*********************************************************************************************************************************************************************

MinMax:

	//void MinMax(const int array[], int size, int *min, int *max);		
	//a0		  a0			   , a1		 , a2	   , a3

	addi	sp, sp, -8
	sd		a0, 0(sp)			//save starting memory address of array 

find_min:

		li		t0, 0				//loop iterator, i == 0
		lw		t1, 0(a0)			//min value. will need to build loop to compare this value to others in array and swap if other value is less than currently loaded min value.
	
	min_for:
		
		lw		t2, 0(a0)			//comparison value. need to start at 0 offset to make sure we check every element in array

		bge		t0, a1, find_max	//if i >= array size, then we finished comparing min value to every element in array and didn't need to swap. We have the min! Need max now.
		bgt		t1, t2, min_swap	//if current min value is > comparing value, jump to swap in preparation to restart FOR loop process with new min value 
		
		addi	t0, t0, 1			//i++
		addi	a0, a0, 4			//move pointer to next int in array

		j		min_for

	min_swap://if we end up here, our currently loaded "min value" was found to be greater than another element and we need to swap and restart ENTIRE FOR loop process
		
		mv		t1, t2				//exchange new min value
		
		li		t0, 0				//reset iterator to 0 because we need to start FOR loop from the beginning
		ld		a0, 0(sp)			//resets a0 pointer
		
		j		min_for

find_max:
		
		//store min int at a2 memory address
		sw		t1, 0(a2)

		ld		a0, 0(sp)			//Need to reset a0 pointer since it was last pointing to last element of array when exiting min_for.
		li		t0, 0				//loop iteratpr, i == 0
		lw		t1, 0(a0)			//max value. will need to build loop to compare this value to others in array and swap if other value is greater than currently loaded max.

	max_for:

		lw		t2, 0(a0)			//comparison value. need to start at 0 offset to make sure we check every element in array

		bge		t0, a1, min_max_ret	//if i >= array size, then we finished comparing max value to every element in array and didn't need to swap. We have the max and min! Done.
		blt		t1, t2, max_swap	//if current max value is < comparing value, jump to swap in preparation to restart FOR loop process with new max value 

		addi	t0, t0, 1			//i++
		addi	a0, a0, 4			//move pointer to next int in array

		j		max_for

	max_swap://if we end up here, our currently loaded "max value" was found to be less than another element and we need to swap and restart ENTIRE FOR loop process

		mv		t1, t2				//exchange new max value
			
		li		t0, 0				//reset iterator to 0 because we need to start FOR loop from the beginning
		ld		a0, 0(sp)			//resets a0 pointer
		
		j		max_for

min_max_ret:
	
	//store max int at a3 memory address
	sw		t1, 0(a3)

	addi	sp, sp, 8
	mv		a0, zero

	ret

//*********************************************************************************************************************************************************************
