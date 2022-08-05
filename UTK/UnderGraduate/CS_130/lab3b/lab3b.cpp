//John Hills
//lab3b

#include <iostream>
#include <cstring>//for strcopy below

using namespace std;

struct PERSON {
	     char name[51];
		 int age;
		 char gender;
};


extern "C"
{

PERSON *Search(PERSON people[], int num_people, const char *name, int age, char gender);	//3

int Sum(const int array[], int array_size);													//1

int Median(const int array[], int array_size, int *buffer);									//3

void MinMax(const int array[], int size, int *min, int *max);								//2

}

void print_person(PERSON * p)
{
	//changed " " to ' ' because all string literals, strcpy, etc automatically terminate string with '/0'.
	cout << p->name << ' ' << p->age << ' ' << p->gender << endl;
}


//int main(int argc, char * argv[])
int main()
{

//***************************************************************************************************************************************
//	const int num [] = {10, 5, 5, 5, 5, 5, 10}; //const because I don't want function calls to change this array.
//	int size = sizeof(num)/sizeof(num[0]);
////	cout << "number of elements: " << size << endl;
//	int sum = Sum(num, size);
//
//	cout << "Sum from assembly returns: " << sum << "\n";
//***************************************************************************************************************************************
////SEARCH:	
////array of people
//	PERSON peeps [4];
//	//person 1
////		peeps[0].name[50] = '\0';
////		const char * name = "Tom";
////		strcpy(peeps[0].name, name);
//		strcpy(peeps[0].name, "Tom"); //GIVING peeps[0] A STRING LITERAL AUTOMATICALLY PUTS '\0' (terminate) AT THE END!!!!!
//		peeps[0].age = 37;
//		peeps[0].gender = 'm';
//	//person 2
////		peeps[1].name[50] = '\0';
////		name = "Amy";
////		strcpy(peeps[1].name, name);
//		strcpy(peeps[1].name, "Amy"); //GIVING peeps[0] A STRING LITERAL AUTOMATICALLY PUTS '\0' (terminate) AT THE END!!!!!
//		peeps[1].age = 35;
//		peeps[1].gender = 'f';
//	//person 3
//	//	peeps[2].name[50] = '\0';
//	//	name = "Sean";
//	//	strcpy(peeps[2].name, name);
//		strcpy(peeps[2].name, "Sean"); //GIVING peeps[0] A STRING LITERAL AUTOMATICALLY PUTS '\0' (terminate) AT THE END!!!!!
//		peeps[2].age = 22;
//		peeps[2].gender = 'm';//NOTE: This is printing out as P for some reason. All else is good.
//	//person 4
//	//	peeps[2].name[50] = '\0';
//	//	name = "Sean";
//	//	strcpy(peeps[2].name, name);
//		strcpy(peeps[3].name, "Seanless"); //GIVING peeps[0] A STRING LITERAL AUTOMATICALLY PUTS '\0' (terminate) AT THE END!!!!!
//		peeps[3].age = -11;
//		peeps[3].gender = 'Q';//NOTE: This is printing out as P for some reason. All else is good.
//		
////	PERSON * p;
////	
////	*p = peeps[2]; //should be Sean
////
////	print_person(p);
//		
//	//person I'm searching for
//	
////	const char* name1 = "Tom"; //String literal automatically has '\0' at the end!!!!
////	//ALL OF BELOW IS UN-NEEDED. above takes care of everything.
////	//char name2 [51];
////	//name2 [50] = '\0';
////	//strcpy(name2, name1);	
////	int age = 37;
////	char gender = 'm'; //IMPORTANT: make sure to account for 'm' and 'M' when searching genders.
//	
////	const char* name1 = "Amy"; //String literal automatically has '\0' at the end!!!!
////	//ALL OF BELOW IS UN-NEEDED. above takes care of everything.
////	//char name2 [51];
////	//name2 [50] = '\0';
////	//strcpy(name2, name1);	
////	int age = 35;
////	char gender = 'f'; //IMPORTANT: make sure to account for 'm' and 'M' when searching genders.
//
////	const char* name1 = "Sean"; //String literal automatically has '\0' at the end!!!!
////	int age = 22;
////	char gender = 'm'; //IMPORTANT: make sure to account for 'm' and 'M' when searching genders.
//
////	const char* name1 = "Seanless"; //String literal automatically has '\0' at the end!!!!
////	int age = -11;
////	char gender = 'Q'; //IMPORTANT: make sure to account for 'm' and 'M' when searching genders.
//	
//	//this should return nullptr (i.e., 64 bits of only zeros!)
//	const char* name1 = "Seanlesi"; //String literal automatically has '\0' at the end!!!!
//	int age = -10;
//	char gender = 'q'; //IMPORTANT: make sure to account for 'm' and 'M' when searching genders.
//	
//	//Results
//	int num_people = 4;
//	PERSON * result;
//	result = Search(peeps, num_people, name1, age, gender); 
//
////	int blah = sizeof(PERSON);
////	cout << "sizeof(PERSON): " << blah << endl;
//	
//	if(result == nullptr) {cout << "Result was 0!" << endl; return 0;}
//	
//	print_person(result);

//***************************************************************************************************************************************
////median
//
//	const int arrayC[] = {22, 1, 3, 15, -6, 45, -13};
//	int array[] =        {22, 1, 3, 15, -6, 45, -13};
////	const int arrayC[] = {39, 40, -12, 11, 17, 0};
////	int array[] =        {39, 40, -12, 11, 17, 0};
//	int array_size = sizeof(array)/sizeof(array[0]); //confirmed to calculate correctly
//	int buffer[7]; //have to change this manually since you can't have a variable length array. stupid shit.
//
//	int median = 0;
//	median++;
//
//	median = Median(arrayC, array_size, buffer);
//	
//	//bubblesort logic
//
//	int temp = 0;
//	bool notSorted = true;
//
//	while (notSorted)
//	{
//		notSorted = false;
//
//		for (int i = 0; i < (array_size - 1); i++)
//		{
//			if (array[i]>array[i+1])
//			{
//				temp = array[i];
//				array[i] = array[i+1];
//				array[i+1] = temp;
//				notSorted = true;
//			}
//		}
//	}
//
//	for (int i = 0; i < array_size; i++)
//	{
//		cout << "array[" << i << "]: " << array[i] << "\tbuffer[" << i << "]: " << buffer[i] << endl;
//	}
//		
//	cout << "median(assembly): " << median << endl; 
	
//***************************************************************************************************************************************
//MinMax

		
	const int arrayC2[] = {22, 1, 300, -76, 91, 152, 46, 32, 401, 0, -11, 400};
	int array_size2 = sizeof(arrayC2)/sizeof(arrayC2[0]); //confirmed to calculate correctly
	int * min = new int;
	int * max = new int;

	MinMax(arrayC2, array_size2, min, max);

	cout << "\nmin: " << *min;
	cout << "\tmax: " << *max << '\n';  

	delete min;
	delete max;

	return 0;

}
