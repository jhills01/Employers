#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "fields.h"
#include "dllist.h"

typedef struct {
	char *name;
	char sex; 
	char *mother;
	char *father;
	Dllist children;
	int visited;
} Person;

//concatenates the strings inside IS->fields[] array starting at the ith index which are separated by SPACE and ends with NULL TERMINATOR. . 
char *
make_name(IS is, int i)
{
	char *name;
	int str_size, i_original;
	
	//referenced code from Plank's JRB notes
	
	//initialize variables
	str_size = 0;
	i_original = i;
	
	//get byte size needed for name
	for (; i < is->NF; i++) 
	{
		str_size += (strlen(is->fields[i])+1);
	}

	//reset i 
	i = i_original;

	//allocate total memory for "name", copy 1st word into "name", set "str_size to correct value for following FOR loop
	name = (char *) malloc(sizeof(char)*(str_size)); 
	strcpy(name, is->fields[i]); 
	str_size = strlen(is->fields[i]); 

	//replaces all null terminators separating "name" words with ' ' except the final null terminator
	for (i = i + 1; i < is->NF; ++i)
	{
		name[str_size] = ' ';
		strcpy(name + str_size + 1, is->fields[i]);
		str_size += strlen(name + str_size); 
	}
	
	return name;	
}

//allocates memory for Person object and assigns default values to all fields except name which is given by "person"
Person * 
construct_person(Person *p, char *person)
{		
	p = malloc(sizeof(Person));
	p->name = person;
	p->father = "Unknown";
	p->mother = "Unknown";
	p->sex = '?';
	p->visited = 0;
	p->children = new_dllist();

	return p;
}

//compares p->sex against "ts" (i.e., test sex) and returns 1 if they are the same or if p->sex is unassigned
//returns 0 otherwise 
int
same_sex_check(Person *p, char ts)
{
	if (p->sex == '?') return 1;
	else if (p->sex != ts) return 0;
	else return 1;
}

//searches parent->children (Dllist object) for a Person object with name "child".
//Returns 1 if "child" name was found, 0 otherwise. 
int 
search_children(Person *parent, char *child)
{	
	Dllist tmpDLL;
	Person *p3;

	dll_traverse(tmpDLL, parent->children)
	{
		p3 = (Person *)tmpDLL->val.v;
		if ( strcmp(p3->name, child) == 0 ) return 1;
	}

	return 0;
}

//recursive method to check for cycles in "people" JRB tree
int
cycle_checker(JRB people)
{
	JRB tmpJRB;
	Person *p;

	jrb_traverse (tmpJRB, people)
	{
		p = (Person *) tmpJRB->val.v;
		//returns 1 if cycle was found for given Person p.
		if ( is_descendant(p) ) return 1; 
	}

	//reset all visited fields of Persons in "people" back to 0
	jrb_traverse (tmpJRB, people)
	{
		p = (Person *) tmpJRB->val.v;
		p->visited = 0;
	}

	return 0;
}

//recursive helper method to check Person p's children for cycles
int
is_descendant(Person *p)
{
	Dllist tmpDLL;
	Person *p2;

	//BASE CASES:
	//person was processed but is ok
	if (p->visited == 1) return 0; 
	//graph has cycle
	if (p->visited == 2) return 1;  

	p->visited = 2;
	
	//RECURSION: its important to ensure p->children is NOT empty before trying to access its "val" memeber
	if ( !dll_empty(p->children) )
	{
		dll_traverse(tmpDLL, p->children)
		{
			p2 = (Person *)tmpDLL->val.v;
			if ( is_descendant(p2) ) return 1;
		}
	}
	
	p->visited = 1;
	return 0;
}

//final print helper function for Person's sex
void
print_sex(Person *p)
{
	if (p->sex == '?') printf ("  Sex: Unknown\n");
	else if (p->sex == 'F') printf ("  Sex: Female\n");
	else if (p->sex == 'M') printf ("  Sex: Male\n");
	else
	{
		printf ("  Sex: WE HAVE A PROBLEM!!!!! p->sex != what it should!!! Exiting...\n");
		exit(1);
	}
}

//final print helper function for Person's children
void
print_children(Person *p)
{
	Dllist tmpDLL;
	Person *p2;

	if ( dll_empty(p->children) ) printf("  Children: None\n");
	else
	{
		printf("  Children:\n");

		dll_traverse(tmpDLL, p->children)
		{
			p2 = (Person *) tmpDLL->val.v;
			printf("    %s\n", p2->name);
		}
	}
	
	printf("\n");
}

//cleans up all memory before program exits
void
free_all(JRB people, Dllist print_queue, IS is)
{
	JRB tmpJRB;
	Person *p;
	char *person;
	
	jrb_traverse(tmpJRB, people)
	{
		p = (Person *) tmpJRB->val.v;
		person = tmpJRB->key.s;

		//free all pointer members of Person p since they were allocated with malloc
		free_dllist(p->children); 
		
		//free val and key of JRB node 
		free(p);
		free(person); //this is the same as freeing p->name 
	}

	//Finally, free leftover structs 
	jrb_free_tree(people);
	free_dllist(print_queue);
	jettison_inputstruct(is);
}

int 
main()
{
	JRB people, tmpJRB;
	Dllist tmpDLL, tmpDLL2, print_queue;
	IS is;
	//p == Person from "PERSON" label of input file, p2 == PERSON with either FATHER_OF, FATHER, MOTHER_OF, or MOTHER labels
	Person *p, *p2;
	Person *p_father, *p_mother;
	char *person, *father, *mother, *father_of, *mother_of;
	char sex; 

	//construct objects
	is = new_inputstruct(NULL);
	people = make_jrb();
	print_queue = new_dllist();

	if (people == NULL) 
	{
		printf("people JRB failed to be created!\n"); 
		free_all(people, print_queue, is);
		exit(1);
	}

	//reading input from file
	while (get_line(is) >= 0) 
	{	
		//avoids do a bunch of IF checks for a blank line
		if (is->NF > 1)
		{
			//check for "PERSON" label
			if (strcmp(is->fields[0],"PERSON") == 0)
			{
				//allocates memory for "person" which is concatenated from IS->fields[] starting at index 1
				person = make_name(is, 1);

				//check if "person" is already in tree. If not, constuct Person and insert into JRB "people"
				tmpJRB = jrb_find_str(people, person);
				
				//"person" NOT found in "people"
				if ( tmpJRB == NULL ) 
				{
					//allocates memory for new Person with name and default values and insert into JRB "people"
					p = construct_person(p, person);
					jrb_insert_str(people, person, new_jval_v( (void *)p ));	
				}
				
				//"person" found in "people"
				else
				{
					p = (Person *) tmpJRB->val.v;
					//free allocated "person" b/c it wasn't used
					free(person);
				}


			}
			
			//check for "FATHER_OF" label 
			else if (strcmp(is->fields[0],"FATHER_OF") == 0)
			{
				//allocates memory for "father_of" name which is concatenated from IS->fields[] starting at index 1
				father_of = make_name(is, 1);

				//search for JRB node in "people" with key == "father_of"
				tmpJRB = jrb_find_str(people, father_of);

				//"father_of" NOT found in "people"
				if ( tmpJRB == NULL ) 
				{
					//allocates memory for new Person and assigns name, assigns father, and inserts into JRB "people"
					p2 = construct_person(p2, father_of);
					p2->father = p->name; 
					jrb_insert_str(people, father_of, new_jval_v( (void *)p2 ));		
				
					//check if father (Person p) has previous sex assignment. If it does and it's not the same, trigger error and exit
					if ( same_sex_check(p, 'M') ) p->sex = 'M';
					else 
					{
						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1);
					}
				
					//update father's (Person p) children 
					dll_append(p->children, new_jval_v ( (void *)p2 ));
				}
			
				//"father_of" found in "people"
				else 
				{
					//extract Person with "father_of" name
					p2 = (Person *) tmpJRB->val.v;

					//check if father (Person p) has previous sex assignment. If it does and it's not the same, trigger error and exit
					if ( same_sex_check(p, 'M') ) p->sex = 'M';
					else 
					{
						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1);
					}
					
					//check to see if Person p2 already has a previous father assigned. If so and it's different, throw error!
					if ( strcmp(p2->father, "Unknown") == 0 ) p2->father = p->name;
					else if ( strcmp(p2->father, p->name) != 0 ) 
					{
						fprintf(stderr, "Bad input -- child with two fathers on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1); 
					}

					//check if Person p has "father_of" child in children. If not, add it.  
					if (!search_children(p, p2->name)  ) //returns 0 if child NOT found 
					{
						dll_append(p->children, new_jval_v ( (void *)p2 ));
					}

					//free allocated "father_of" b/c it wasn't used
					free(father_of);
				}
			}
			
			//check for "MOTHER_OF" label 
			else if (strcmp(is->fields[0],"MOTHER_OF") == 0)
			{
				//allocates memory for "mother_of" name which is concatenated from IS->fields[] starting at index 1
				mother_of = make_name(is, 1);

				//search for JRB node in "people" with key == "mother_of"
				tmpJRB = jrb_find_str(people, mother_of);

				//"mother_of" NOT found in "people"
				if ( tmpJRB == NULL ) 
				{
					//allocates memory for new Person and assigns name, assigns mother, and inserts into JRB "people"
					p2 = construct_person(p2, mother_of);
					p2->mother = p->name; 
					jrb_insert_str(people, mother_of, new_jval_v( (void *)p2 ));		
				
					//check if mother (Person p) has previous sex assignment. If it does and it's not the same, trigger error and exit
					if ( same_sex_check(p, 'F') ) p->sex = 'F';
					else 
					{
						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1);
					}
				
					//update mother's (Person p) children 
					dll_append(p->children, new_jval_v ( (void *)p2 ));
				}
			
				//"mother_of" found in "people"
				else 
				{
					//extract Person with "mother_of" name
					p2 = (Person *) tmpJRB->val.v;
					
					//check if mother (Person p) has previous sex assignment. If it does and it's not the same, trigger error and exit
					if ( same_sex_check(p, 'F') ) p->sex = 'F';
					else 
					{
						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1);
					}
					
					//check to see if Person p2 already has a previous mother assigned. If so, throw error!
					if ( strcmp(p2->mother, "Unknown") == 0 ) p2->mother = p->name;
					else if ( strcmp(p2->mother, p->name) != 0 ) 
					{
						fprintf(stderr, "Bad input -- child with two mothers on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1); 
					}

					//check if Person p has "mother_of" child in children. If not, add it.  
					if (!search_children(p, p2->name)  ) //returns 0 if child NOT found 
					{
						dll_append(p->children, new_jval_v ( (void *)p2 ));
					}

					//free allocated "mother_of" b/c it wasn't used
					free(mother_of);
				}
			}
		
			//check for "FATHER" label 
			else if (strcmp(is->fields[0],"FATHER") == 0)
			{
				//allocates memory for "father" name which is concatenated from IS->fields[] starting at index 1
				father = make_name(is, 1);

				//search for JRB node in "people" with key == "father"
				tmpJRB = jrb_find_str(people, father);

				//"father" NOT found in "people"
				if ( tmpJRB == NULL ) 
				{
					//allocates memory for new Person 
					p2 = construct_person(p2, father);
					//no need to error check for p2 sex since he's newly created 
					p2->sex = 'M'; 
					
					//check if p already has father assignment that ISN'T named "father"
					if ( strcmp(p->father, "Unknown") == 0 ) p->father = father; 
					else if ( strcmp(p->father, father) != 0 )
					{
						fprintf(stderr, "Bad input -- child with two fathers on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1);
					}
					
					jrb_insert_str(people, father, new_jval_v( (void *)p2 ));		
			
					//update father's (Person p) children 
					dll_append(p2->children, new_jval_v ( (void *)p ));
				}
			
				//"father" found in "people"
				else 
				{
					//extract Person with "father" name
					p2 = (Person *) tmpJRB->val.v;
					
					//check if father (Person p2) has previous sex assignment. If it does and it's not the same, trigger error and exit
					if ( same_sex_check(p2, 'M') ) p2->sex = 'M';
					else 
					{
						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1);
					}
					
					//check to see if Person p already has a previous father assigned. If so, throw error!
					if ( strcmp(p->father, "Unknown") == 0 ) p->father = p2->name;
					else if ( strcmp(p->father, p2->name) != 0 ) 
					{
						fprintf(stderr, "Bad input -- child with two fathers on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1); 
					}

					//check if Person p2 has Person p as child in children. If not, add it.  
					if (!search_children(p2, p->name)  ) //returns 0 if p->name NOT found in p2->children 
					{
						dll_append(p2->children, new_jval_v ( (void *)p ));
					}

					//free allocated "father" b/c it wasn't used
					free(father);
				}
			}
		
			//check for "MOTHER" label 
			else if (strcmp(is->fields[0],"MOTHER") == 0)
			{
				//allocates memory for "mother" name which is concatenated from IS->fields[] starting at index 1
				mother = make_name(is, 1);

				//search for JRB node in "people" with key == "mother"
				tmpJRB = jrb_find_str(people, mother);

				//"mother" NOT found in "people"
				if ( tmpJRB == NULL ) 
				{
					//allocates memory for new Person 
					p2 = construct_person(p2, mother);
					//no need to error check for p2 sex since he's newly created 
					p2->sex = 'F'; 
					
					//check if p already has mother assignment that ISN'T named "mother"
					if ( strcmp(p->mother, "Unknown") == 0 ) p->mother = mother; 
					else if ( strcmp(p->mother, mother) != 0 )
					{
						fprintf(stderr, "Bad input -- child with two mothers on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1);
					}
					
					jrb_insert_str(people, mother, new_jval_v( (void *)p2 ));		
			
					//update mother's (Person p) children 
					dll_append(p2->children, new_jval_v ( (void *)p ));
				}
			
				//"mother" found in "people"
				else 
				{
					//extract Person with "mother" name
					p2 = (Person *) tmpJRB->val.v;
					
					//check if mother (Person p2) has previous sex assignment. If it does and it's not the same, trigger error and exit
					if ( same_sex_check(p2, 'F') ) p2->sex = 'F';
					else 
					{
						fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1);
					}
					
					//check to see if Person p already has a previous mother assigned. If so, throw error!
					if ( strcmp(p->mother, "Unknown") == 0 ) p->mother = p2->name;
					else if ( strcmp(p->mother, p2->name) != 0 ) 
					{
						fprintf(stderr, "Bad input -- child with two mothers on line %d\n", is->line);
						free_all(people, print_queue, is);
						exit(1); 
					}

					//check if Person p2 has Person p as child in children. If not, add it.  
					if (!search_children(p2, p->name)  ) //returns 0 if p->name NOT found in p2->children 
					{
						dll_append(p2->children, new_jval_v ( (void *)p ));
					}

					//free allocated "mother" b/c it wasn't used
					free(mother);
				}
			}

			//check for "SEX" label 
			else if (strcmp(is->fields[0],"SEX") == 0)
			{
				//check if Person p has previous sex assignment. If it does and it's not the same, trigger error and exit
				sex = is->fields[1][0];
				
				if ( same_sex_check(p, sex) ) p->sex = sex;
				else 
				{
					fprintf(stderr, "Bad input - sex mismatch on line %d\n", is->line);
					free_all(people, print_queue, is);
					exit(1);
				}
			}
		}
	}

	//cycle check
	if ( cycle_checker(people) ) //cycle_checker also resets all "visited" memebers of Persons in "people" back to 0
	{
		fprintf(stderr, "Bad input -- cycle in specification\n");
		free_all(people, print_queue, is);
		exit(1);
	}

	//final print if cycle check passed
	else
	{
		//load up "print_queue" with people who don't have a designated father AND mother
		jrb_traverse(tmpJRB, people)
		{
			p = (Person *) tmpJRB->val.v;
			
			if ( (strcmp(p->father, "Unknown") == 0) && (strcmp(p->mother, "Unknown") == 0) )
			{
				dll_append( print_queue, new_jval_v( (void *)p ) );
			}
		}

		//final print
		while ( !dll_empty(print_queue) )  //ensures tmpDLL below won't be the sentinel node
		{
			//take node from the front of queue
			tmpDLL = print_queue->flink;
			
			//extract Person object from node and delete node from queue
			p = (Person *) tmpDLL->val.v;
			dll_delete_node(tmpDLL); 
	
			//not visited == not printed
			if ( !p->visited )
			{
				//find father of p
				tmpJRB = jrb_find_str(people, p->father);
				if (tmpJRB != NULL) p_father = (Person *) tmpJRB->val.v;
				else p_father = NULL;
				
				//find mother of p
				tmpJRB = jrb_find_str(people, p->mother);
				if (tmpJRB != NULL) p_mother = (Person *) tmpJRB->val.v;
				else p_mother = NULL;

				//have to check every permutation of non-existing parent versus existing parent that's already been printed
				//shouldn't have to check if p_mother/father->visited == 0 b/c my tree has no cycles from earlier check
				if ( (p_father == NULL && p_mother == NULL) || 
					 (p_father == NULL && p_mother->visited == 1) ||
					 (p_mother == NULL && p_father->visited == 1) ||
					 (p_father->visited == 1 && p_mother->visited == 1)
				   )
				{
					//set Person p as visited since we are printing them
					p->visited = 1;
			
					//final print of Person p	
					printf("%s\n", p->name);
					print_sex(p);
					printf("  Father: %s\n", p->father);
					printf("  Mother: %s\n", p->mother);
					print_children(p); 

					//put p's children onto the queue
					dll_traverse(tmpDLL, p->children)
					{
						p2 = (Person *) tmpDLL->val.v;
						dll_append( print_queue, new_jval_v( (void *)p2 ) );
					}
				}
			}
		}
	}

	//freeing all allocated memory
	free_all(people, print_queue, is);
	
	exit(0);
} 
