// NAME:Tom Hills
// NETID: jhills

// CS 360: Lab7 - Jsh

// DESCRIPTION:
// A command line interpreter that lets you execute commands and redirects their input/output. It uses the same syntax as the bourne/c shell,
// and implements the operations < , >, >>, & and |.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "fields.h"
#include "jrb.h"

//cleans up leftover memory
void 
free_all(IS is, JRB tree)
{
	jettison_inputstruct(is);
	jrb_free_tree(tree);
}

//returns 1 if field string is a recognized operator
//0 otherwise
int
is_operator(char *field)
{
	if ( strcmp(field,"<") == 0 || strcmp(field,">>") == 0 || strcmp(field,">") == 0 || strcmp(field,"|") == 0 || strcmp(field,"&") == 0 ) return 1;
	else return 0;
}

//returns next operator in command line chain as referenced from the current operator 
//returns NULL if there is NOT a subsequent operator in command chain
char *
next_operator(IS is, int i)
{
	for ( ++i; i < is->NF; ++i)
	{
		if ( is_operator(is->fields[i]) ) return is->fields[i];
	}

	return NULL;
}

//returns 1 if command line has at least one operator, 0 otherwise 
int
operator_present(IS is)
{
	int i;
	
	//doesn't count & operator in this check
	if ( strcmp(is->fields[is->NF - 1], "&") == 0 ) return 0;

	for ( i = 0; i < is->NF; ++i)
	{
		if ( is_operator(is->fields[i]) ) return 1;
	}
	return 0;
}

//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
void
wait_helper(IS is, JRB processes, int exe_pid)
{
	int ret_pid;
	JRB node;

	if ( strcmp(is->fields[is->NF - 1],"&") == 0 ) jrb_insert_int(processes, exe_pid, JNULL);
	else 
	{
		while (1)
		{
			ret_pid = wait(NULL);
			if ( (ret_pid == -1) || (ret_pid == exe_pid) ) return;
			node = jrb_find_int(processes, ret_pid);
			if (node != NULL) jrb_delete_node(node);
		}
	}
}

int
main(int argc, char **argv, char **envp)
{
	IS is;
	JRB processes;
	char *prompt;
	char *next_op;
	char *newargv[100]; //100 size array is arbitrary. 
	int pipefd1[2], pipefd2[2];
	int last_pipe_written_to; //0 if none written yet. Otherwise, 1 == pipefd1 and 2 == pipefd2
	int exe_pid;
	int newargv_i, i;
	int file_fd1, file_fd2;

	//establish j-shell prompt
	if (argc > 2) {fprintf(stderr, "argc > 2\nreturn(1)\n"); return(1);}
	else if (argc == 2) prompt = argv[1];
	else prompt = "jsh: ";

	//establish inputstruct for parsing file lines and a JRB tree for tracking background processes
	is = new_inputstruct(NULL);
	if (is == NULL) 
	{
		perror("new_inputstruct(NULL)");
		return 1;
	}
	processes = make_jrb();

	//JSHELL LOOP
	for ( ; ; )
	{
		//display prompt
		if ( strcmp(prompt, "-") == 0 ) {/*don't print a prompt*/} 
		else printf("%s ", prompt);
		
		//get single command line from stdin
		//if "exit" or CTRL-D, free leftover memory and return
		if ( (get_line(is) == -1) || ((is->fields[0] != NULL) && (strcmp(is->fields[0], "exit") == 0)) )
		{
			free_all(is, processes);
			return 0; 
		}

		//initialize new parent process
		last_pipe_written_to = 0; 
	
		//INPUTSTRUCT FOR LOOP START
		//loops through IS struct identifying operators and building command arrays for execvp calls in child processes	
		for ( i = 0, newargv_i = 0; i < is->NF; ++i, ++newargv_i )
		{
			//FINISHING COMMAND LINE PROCESS CHAIN--------------------------------------------------------------------------------------------------------
			if ( i == (is->NF - 1) )
			{
				// build newargv based on last item in commmand line
				if ( !is_operator(is->fields[i]) ) 
				{
					newargv[newargv_i] = is->fields[i];  	
					++newargv_i;
					newargv[newargv_i] = NULL;  	
				}
				else newargv[newargv_i] = NULL;  

				//no operator (excluding &) found in command line	
				//if (last_pipe_written_to == 0 && !operator_present(is) )
				if ( !operator_present(is) )
				{
					//execute next process
					exe_pid = fork();
					if ( exe_pid == 0 )
					{
						execvp(newargv[0], newargv);
						perror(newargv[0]);
						exit(1);
					}

					else
					{
						//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
						wait_helper(is, processes, exe_pid);
					}
				}

				else if (last_pipe_written_to == 1) 
				{
					//fork child process
					exe_pid = fork();
					if ( exe_pid == 0 )
					{
						//set up child process
						dup2(pipefd1[0], 0);
						close(pipefd1[0]);

						//execute child process
						execvp(newargv[0], newargv);
						perror(newargv[0]);
						exit(1);
					}

					else
					{
						//clean up FDs before calling wait
						close(pipefd1[0]);
						
						//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
						wait_helper(is, processes, exe_pid);
					}
				}
				
				else if (last_pipe_written_to == 2) 
				{
					//execute child process
					exe_pid = fork();
					if ( exe_pid == 0 )
					{
						//set up next process
						dup2(pipefd2[0], 0);
						close(pipefd2[0]);

						//execute child process
						execvp(newargv[0], newargv);
						perror(newargv[0]);
						exit(1);
					}

					else
					{
						//clean up FDs before calling wait
						close(pipefd2[0]);
						
						//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
						wait_helper(is, processes, exe_pid);
					}
				}
			}
			
			//BUILDING NEWARGV-----------------------------------------------------------------------------------------------------------------------
			else if ( !is_operator(is->fields[i]) ) newargv[newargv_i] = is->fields[i]; 
			
			//< OPERATOR------------------------------------------------------------------------------------------------------------------------------
			else if ( strcmp(is->fields[i], "<") == 0 )
			{
				//null terminate newargv
				newargv[newargv_i] = NULL;
				
				//determine what next operator is going to be
				next_op = next_operator(is, i); //returns NULL if there is NOT a subsequent operator in command chain

				//open file that needs redirection into stdin
				++i;
				file_fd1 = open(is->fields[i], O_RDONLY, 0777);
				if (file_fd1 < 0) { fprintf(stdout, "opening file_fd1 for STDIN failed. < OPERATOR. Exiting..."); exit(1); }
				
				//set up process based on next_op
				if (next_op == NULL) {/*do nothing*/} 
				else if ( strcmp(next_op,">") == 0 )
				{
					i += 2; //move "i" one element past ">" operator --- this effectively ends the IS FOR loop
					file_fd2 = open(is->fields[i], O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0777); 
					if (file_fd2 < 0) { fprintf(stdout, "opening file_fd1 for STDOUT failed. < OPERATOR. Exiting..."); exit(1); }
				}
				else if ( strcmp(next_op,">>") == 0 )
				{
					i += 2; //move "i" one element past ">>" operator --- this effectively ends the IS FOR loop
					file_fd2 = open(is->fields[i], O_WRONLY | O_CREAT | O_APPEND | O_SYNC, 0777); 
					if (file_fd2 < 0) { fprintf(stdout, "opening file_fd1 for STDOUT failed. < OPERATOR. Exiting..."); exit(1); }
				}
				else if ( strcmp(next_op,"|") == 0 )
				{
					//set up pipes
					pipe(pipefd1);
					i += 1; //move "i" to postion of the "|" operator of next_op so that loop will start one element after that
				}
				
				//start child process
				exe_pid = fork();
				if ( exe_pid == 0 )
				{
					//open file that needs redirection into stdin
					dup2(file_fd1, 0);
					close(file_fd1);

					//set up process based on next_op
					if (next_op == NULL) {/*do nothing*/} 
					else if ( strcmp(next_op,">") == 0 )
					{
						dup2(file_fd2, 1);
						close(file_fd2);
					}
					else if ( strcmp(next_op,">>") == 0 )
					{
						dup2(file_fd2, 1);
						close(file_fd2);
					}
					else if ( strcmp(next_op,"|") == 0 )
					{
						dup2(pipefd1[1], 1);
						close(pipefd1[1]);
					}
					
					//execute child process
					execvp(newargv[0], newargv);
					perror(newargv[0]);
					exit(1);
				}
				
				//wait for child process to return
				else 
				{
					//clean up FDs before calling wait
					close(file_fd1);
					if ( (strcmp(next_op,">") == 0) || (strcmp(next_op,">>") == 0) ) close(file_fd2);
				
					//if | is next, close write end of pipe and set last pipe written to.
					if ( strcmp(next_op,"|") == 0 ) { close(pipefd1[1]); last_pipe_written_to = 1; } 
					
					//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
					wait_helper(is, processes, exe_pid);
					newargv_i = -1; //b/c newarv_i will get incremented by 1 at the start of next loop
				}
			}
		
			//> OR >> OPERATORS----------------------------------------------------------------------------------------------------------------------------
			else if ( (strcmp(is->fields[i], ">") == 0) || (strcmp(is->fields[i], ">>") == 0) )
			{
				//null terminate newargv
				newargv[newargv_i] = NULL;

				//open file that needs redirection into stdin
				if ( strcmp(is->fields[i],">") == 0 )
				{
					++i;
					file_fd2 = open(is->fields[i], O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0777); 
					if (file_fd2 < 0) { fprintf(stdout, "opening file_fd1 for STDOUT failed. < OPERATOR. Exiting..."); exit(1); }
				}
				else if ( strcmp(is->fields[i],">>") == 0 )
				{
					++i;
					file_fd2 = open(is->fields[i], O_WRONLY | O_CREAT | O_APPEND | O_SYNC, 0777); 
					if (file_fd2 < 0) { fprintf(stdout, "opening file_fd1 for STDOUT failed. < OPERATOR. Exiting..."); exit(1); }
				}

				//start child process
				exe_pid = fork();
				if ( exe_pid == 0 )
				{
					//dup file that needs redirection into stdin
					dup2(file_fd2, 1);
					close(file_fd2);

					//set up next process
					if (last_pipe_written_to == 1) 
					{
							dup2(pipefd1[0], 0);
							close(pipefd1[0]);
					}
					
					else if (last_pipe_written_to == 2) 
					{
							dup2(pipefd2[0], 0);
							close(pipefd2[0]);
					}

					//execute child process
					execvp(newargv[0], newargv);
					perror(newargv[0]);
					exit(1);
				}
				
				//wait for child process to return
				else 
				{
					//clean up FDs before calling wait
					close(file_fd2);
					
					if (last_pipe_written_to == 1) close(pipefd1[0]);
					else if (last_pipe_written_to == 2) close(pipefd2[0]);
					
					//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
					wait_helper(is, processes, exe_pid);
					newargv_i = -1; //b/c newarv_i will get incremented by 1 at the start of next loop

					//break IS FOR loop
					break;
				}
			}

			//PIPE OPERATOR--------------------------------------------------------------------------------------------------------------------------
			else if ( strcmp(is->fields[i], "|") == 0 )
			{
				//null terminate newargv
				newargv[newargv_i] = NULL;

				//determine what next operator is going to be
				next_op = next_operator(is, i); //returns NULL if there is NOT a subsequent operator in command chain
				
				//neither pipe written to --- should execute only when | is seen for the first time
				if ( last_pipe_written_to == 0 ) 
				{
					//set up next pipe
					pipe(pipefd1);

					//start child process
					exe_pid = fork();
					if ( exe_pid == 0 )
					{
						//setup child process
						dup2(pipefd1[1], 1);
						close(pipefd1[1]);
					
						//execute child process
						execvp(newargv[0], newargv);
						perror(newargv[0]);
						exit(1);
					}
					
					else 
					{
						//clean up FDs before calling wait
						close(pipefd1[1]);
						
						//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
						wait_helper(is, processes, exe_pid);
						last_pipe_written_to = 1;
						newargv_i = -1; //b/c newarv_i will get incremented by 1 at the start of next loop
					}
				}
				
				//pipefd1 last pipe written to
				else if (last_pipe_written_to == 1 ) 
				{
					//create new pipe
					pipe(pipefd2); 
					
					//start child process
					exe_pid = fork();
					if ( exe_pid == 0 )
					{
						//setup child process 
						dup2(pipefd1[0], 0);
						close(pipefd1[0]); //pipefd1 now completely closed
						dup2(pipefd2[1], 1);
						close(pipefd2[1]); 
					
						//execute child process
						execvp(newargv[0], newargv);
						perror(newargv[0]);
						exit(1);
					}
					
					else 
					{
						//clean up FDs before calling wait
						close(pipefd1[0]); //pipefd1 now completely closed
						close(pipefd2[1]); 
						
						//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
						wait_helper(is, processes, exe_pid);
						last_pipe_written_to = 2;
						newargv_i = -1; //b/c newarv_i will get incremented by 1 at the start of next loop
					}
				}
				
				//pipefd2 last pipe written to
				else if (last_pipe_written_to == 2) 
				{
					//create needed pipe
					pipe(pipefd1);

					//start child process
					exe_pid = fork();
					if ( exe_pid == 0 )
					{
						//setup child process
						dup2(pipefd2[0], 0);
						close(pipefd2[0]); //pipefd2 now completely closed
						dup2(pipefd1[1], 1);
						close(pipefd1[1]); 
					
						//execute child process
						execvp(newargv[0], newargv);
						perror(newargv[0]);
						exit(1);
					}
					
					else 
					{
						close(pipefd2[0]); //pipefd2 now completely closed
						close(pipefd1[1]); 

						//doesn't call "wait" if command line has &. Otherwise, calls "wait" and compares against background processes, if present
						wait_helper(is, processes, exe_pid);
						last_pipe_written_to = 1;
						newargv_i = -1; //b/c newarv_i will get incremented by 1 at the start of next loop
					}
				}
			}
		}
	}

	return 0;
}
