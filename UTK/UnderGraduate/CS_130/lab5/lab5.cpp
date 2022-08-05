//John (Tom) Hills
//lab5 - dissassembler
//8/6/2018

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>

using namespace std;

class DECODER
{
	private:
		//PRIVATE MEMBER VARIABLES.
		
		//RISC V register array. Eliminates the need for large switch/if statements for converting register numbers from BIN file to corresponding ABI names.
		//NOTE: The integers in rd, rs1, and rs2 are the indices to elements within ABI array that correspond to the associated  ABI name for that register number.
		const string ABI [32]
		{
			//This models the layout of the RISC V registers
			"zero",
			"ra",
			"sp",
			"gp",
			"tp",
			"t0",
			"t1", "t2",
			"s0",
			"s1",
			"a0", "a1",
			"a2", "a3", "a4", "a5", "a6", "a7",
			"s2","s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
			"t3", "t4", "t5", "t6"

		};

		const string func3_imm12_op19[8]
		//	 0      1      2        3       4      5      6       7
		{"addi", "slli","slti", "sltiu", "xori", "inv", "ori", "andi"};

		const string func3_imm12_op3[7]
		// 0     1    2     3      4      5      6    
		{"lb", "lh","lw", "ld", "lbu", "lhu", "lwu"};
	
		const string func3_imm12_op35[4]
		// 0     1    2     3     
		{"sb", "sh","sw", "sd"};

		const string func7_0_op51[8]
		//	0      1     2       3      4      5     6      7
		{"add", "sll","slt", "sltu", "xor", "srl", "or", "and"};

		const string func7_32_op51[6]
		//  0      1     2      3      4      5         
		{"sub", "inv","inv", "inv", "inv", "sra"};
		
		FILE *fout; //so I can tell where to output "processed" instruction (see process function)
		
		//PRIVATE MEMBER FUNCTIONS
		//NOTE: all private functions being called through process_opcode()

		//19 == 0b0010011 opcode.
		void decode_op19(int instruction, unsigned char opcode, bool argv3_x);//call this from my public "process" function, so make it private
		//55 == 0b0110111 opcode
		void decode_op55(int instruction, unsigned char opcode, bool argv3_x);
		//111 == 0b1101111 opcode
		void decode_op111(int instruction, unsigned char opcode, bool argv3_x);
		//103 == 0b1100111 opcode
		void decode_op103(int instruction, unsigned char opcode, bool argv3_x);
		//3 == 0b0000011 opcode
		void decode_op3(int instruction, unsigned char opcode, bool argv3_x);
		//35 == 0b0100011 opcode
		void decode_op35(int instruction, unsigned char opcode, bool argv3_x);
		//51 == 0b0110011 opcode
		void decode_op51(int instruction, unsigned char opcode, bool argv3_x);
		
		//print function to stream or file for all decoded instructions
		void print_decoded_instruction(unsigned char opcode, string operation_name, int i, int j, int k, bool argv3_x, int instruction);

	public:
		//Constructor
		DECODER(FILE *output_filestream)
		{
			//fout is a private member variable of FILE pointer type pointing to a "processed instruction" (see "process" function below)
			fout = output_filestream;
		}
		
		//public member function
		void process_opcode(int instruction, bool argv3_x); //might not be void. Send it 32 bits (one instruction) and print to filestream the assembly version of that instruction.

};

//19 == 0b0010011 op code. 
void DECODER::decode_op19(int instruction, unsigned char opcode, bool argv3_x)
{
	int rd = -1, func3 = -1, rs1 = -1, imm12 = -1, func7 = -1, shamt = -1;
	string operation_name = "NULL";
	
	rd = (instruction >> 7) & 0b11111;
	func3 = (instruction >> 12) & 0b111;
	rs1 = (instruction >> 15) & 0b11111;

	if (func3 == 0 || func3 == 2 || func3 == 3 || func3 == 4 || func3 == 6 || func3 == 7)
	{
		imm12 = ((signed)instruction) >> 20;

		operation_name = func3_imm12_op19[func3];

		//print decoded instruction
		print_decoded_instruction(opcode, operation_name, rd, rs1, imm12, argv3_x, instruction);
	}
	
	else if (func3 == 1 || func3 == 5)
	{
		//Since we have either RV32 or RV64 instruction sets depending on the size of shamt (RV32: shamt_MAX == 31, RV53: shamt_max == 63), we must branch dependent on shamt. 
		if (((instruction >> 20) & 0b111111) <= 31)	
		{
			shamt = ((signed)instruction >> 20) & 0b11111;
			func7 = (instruction >> 25) & 0b1111111;
		}
		else
		{
			shamt = ((signed)instruction >> 20) & 0b111111;
			func7 = (instruction >> 26) & 0b111111;
		}
		
		//Assigning the remaining operation names 
		if (func3 == 1) {operation_name = func3_imm12_op19[func3];}
		if (func7 == 0 && func3 == 5) {operation_name = "srli";}
		if (func7 == 32) {operation_name = "srai";}
		
		//print decoded instruction
		print_decoded_instruction(opcode, operation_name, rd, rs1, shamt, argv3_x, instruction);
	}
}

//55 == 0b0110111 op code. 
void DECODER::decode_op55(int instruction, unsigned char opcode, bool argv3_x)
{
	string operation_name = "lui";
	int rd = (instruction >> 7) & 0b11111;
	int imm20 = ((signed)instruction) & 0b11111111111111111111000000000000;

	//                        char    string          i   j       k  bool 
	print_decoded_instruction(opcode, operation_name, rd, imm20, -1, argv3_x, instruction);
}

//111 == 0b1101111 op code. 
void DECODER::decode_op111(int instruction, unsigned char opcode, bool argv3_x)
{
	string operation_name = "jal";
	int rd = (instruction >> 7) & 0b11111;
	//isolating each section of the immediate in prepartion for reorganzing bits into correct integer.
	//
	int imm19_12 = ((signed)instruction >> 12) & -1;//right shifting past func7 and rd to 8 bit imm19:12
	int imm11 = ((signed)instruction >> 20) & -1;//right shifting 12 + 8 places to 1 bit imm11
	int imm10_1 = ((signed)instruction >> 21) & -1;//right shifting 20 + 1 places to 10 bit imm10:1
	int imm20 = ((signed)instruction >> 31) & -1;//right shifting 21 + 10 places to 1 bit imm20
	
	//puting immediate's bits into proper order to obtain the correct integer
	int imm32 = (imm10_1 << 1) | (imm11 << 10) | (imm19_12 << 11) | (imm20 << 12);

	//                        char    string          i   j       k  bool 
	print_decoded_instruction(opcode, operation_name, rd, imm32, -1, argv3_x, instruction);
}

//103 == 0b1100111 op code. 
void DECODER::decode_op103(int instruction, unsigned char opcode, bool argv3_x)
{
	string operation_name = "jalr";
	int rd = (instruction >> 7) & 0b11111;
	int rs1 = (instruction >> 15) & 0b11111;
	int imm12 = ((signed)instruction >> 20);

	//                        char    string          i   j    k      bool 
	print_decoded_instruction(opcode, operation_name, rd, rs1, imm12, argv3_x, instruction);
}

//3 == 0b0000011 op code. 
void DECODER::decode_op3(int instruction, unsigned char opcode, bool argv3_x)
{
	int rd = -1, func3 = -1, rs1 = -1, imm12 = -1;
	string operation_name = "NULL";
	
	//isolate instruction sections
	rd = (instruction >> 7) & 0b11111;
	func3 = (instruction >> 12) & 0b111;
	rs1 = (instruction >> 15) & 0b11111;
	imm12 = ((signed)instruction) >> 20;
	operation_name = func3_imm12_op3[func3];

	//print decoded instruction
	print_decoded_instruction(opcode, operation_name, rd, rs1, imm12, argv3_x, instruction);
}

//35 == 0b0100011 op code. 
void DECODER::decode_op35(int instruction, unsigned char opcode, bool argv3_x)
{
	//isolating sections of instruction
	int imm4_0 = (instruction >> 7) & 0b11111;
	int func3 = (instruction >> 12) & 0b111;
	int rs1 = (instruction >> 15) & 0b11111;
	int rs2 = (instruction >> 20) & 0b11111;
	int imm11_5 = ((signed)instruction >> 25) & -1;
	string operation_name = func3_imm12_op35[func3];
	
	//assembling the correct immediate
	int imm12 = imm4_0 | (imm11_5 << 5);
	
	//print decoded instruction
	print_decoded_instruction(opcode, operation_name, rs1, rs2, imm12, argv3_x, instruction);
}

//51 == 0b0110011 op code. 
void DECODER::decode_op51(int instruction, unsigned char opcode, bool argv3_x)
{
	int rd = (instruction >> 7) & 0b11111;
	int func3 = (instruction >> 12) & 0b111;
	int rs1 = (instruction >> 15) & 0b11111;
	int rs2 = (instruction >> 20) & 0b11111;
	int func7 = (instruction >> 25) & 0b1111111;
	string operation_name = "NULL";
	
	//assign correct operation name
	if (func7 == 0){operation_name = func7_0_op51[func3];}
	if (func7 == 32){operation_name = func7_32_op51[func3];}
	
	//print decoded instruction
	print_decoded_instruction(opcode, operation_name, rd, rs1, rs2, argv3_x, instruction);
}

void DECODER::print_decoded_instruction (unsigned char opcode, string operation_name, int i, int j, int k, bool argv3_x, int instruction)
{
	
	switch (opcode)
	{
		case 0b0010011:
			//fprintf instruction using register names
			if (argv3_x)
			{
				fprintf(fout,"    %-7.7sx%-d, x%-d, %-d\t\t//0x%-.8x\n", operation_name.c_str(), i, j, k, instruction);
			}
			
			//fprintf instruction using ABI names
			else 
			{
				fprintf(fout,"    %-7.7s%-2.4s, %-2.4s, %-d\t\t//0x%-.8x\n",operation_name.c_str(), ABI[i].c_str(),ABI[j].c_str(), k, instruction);
			}
			break;

		case 0b0110111:
			//fprintf instruction using register names
			if (argv3_x)
			{
				fprintf(fout,"    %-7.7sx%-d, %-d\t//0x%-.8x\n", operation_name.c_str(), i, j, instruction);
			}
			
			//fprintf instruction using ABI names
			else 
			{
				fprintf(fout,"    %-7.7s%-2.4s, %-d\t//0x%-.8x\n",operation_name.c_str(), ABI[i].c_str(), j, instruction);
			}
			break;
	
		case 0b1101111:
			//fprintf instruction using register names
			if (argv3_x)
			{
				fprintf(fout,"    %-7.7sx%-d, %-d\t\t//0x%-.8x\n", operation_name.c_str(), i, j, instruction);
			}
			
			//fprintf instruction using ABI names
			else 
			{
				fprintf(fout,"    %-7.7s%-2.4s, %-d\t\t//0x%-.8x\n",operation_name.c_str(), ABI[i].c_str(), j, instruction);
			}
			break;

		case 0b1100111:
			//fprintf instruction using register names
			if (argv3_x)
			{
				fprintf(fout,"    %-7.7sx%-d, x%-d, %-d\t\t//0x%-.8x\n", operation_name.c_str(), i, j, k, instruction);
			}
			
			//fprintf instruction using ABI names
			else 
			{
				fprintf(fout,"    %-7.7s%-2.4s, %-2.4s, %-d\t\t//0x%-.8x\n",operation_name.c_str(), ABI[i].c_str(),ABI[j].c_str(), k, instruction);
			}
			break;

		case 0b0000011:
			//fprintf instruction using register names
			if (argv3_x)
			{
			    if(operation_name == "lh" || operation_name == "lb" || operation_name == "lw"){fprintf(fout,"    %-7.7sx%-d, %-d(x%-d)\t//0x%-.8x\n", operation_name.c_str(), i, k, j, instruction);}
				else {fprintf(fout,"    %-7.7sx%-d, %-d(x%-d)\t\t//0x%-.8x\n", operation_name.c_str(), i, k, j, instruction);}
			}
			
			//fprintf instruction using ABI names
			else 
			{
				if(operation_name == "lh" || operation_name == "lb"){fprintf(fout,"    %-7.7s%-2.4s, %-d(%-2.4s)\t//0x%-.8x\n",operation_name.c_str(), ABI[i].c_str(), k, ABI[j].c_str(), instruction);}
				else {fprintf(fout,"    %-7.7s%-2.4s, %-d(%-2.4s)\t\t//0x%-.8x\n",operation_name.c_str(), ABI[i].c_str(), k, ABI[j].c_str(), instruction);}
			}
			break;

		case 0b0100011:
			//fprintf instruction using register names
			if (argv3_x)
			{
			    fprintf(fout,"    %-7.7sx%-d, %-d(x%-d)\t\t//0x%-.8x\n", operation_name.c_str(), j, k, i, instruction);
			}
			
			//fprintf instruction using ABI names
			else 
			{
				fprintf(fout,"    %-7.7s%-2.4s, %-d(%-2.4s)\t\t//0x%-.8x\n",operation_name.c_str(), ABI[j].c_str(), k, ABI[i].c_str(), instruction);
			}
			break;
	
		case 0b0110011:
			//fprintf instruction using register names
			if (argv3_x)
			{
				
				if(operation_name == "or" || operation_name == "srl" || (operation_name == "sub" && k == 11)) {fprintf(fout,"    %-7.7sx%-d, x%-d, x%-d\t//0x%-.8x\n", operation_name.c_str(), i, j, k, instruction);}
				else {fprintf(fout,"    %-7.7sx%-d, x%-d, x%-d\t\t//0x%-.8x\n", operation_name.c_str(), i, j, k, instruction);}
			}
			
			//fprintf instruction using ABI names
			else 
			{
				fprintf(fout,"    %-7.7s%-2.4s, %-2.4s, %-2.4s\t\t//0x%-.8x\n",operation_name.c_str(), ABI[i].c_str(),ABI[j].c_str(),ABI[k].c_str(), instruction);
			}
			break;
	}
	
}

void DECODER::process_opcode(int instruction, bool argv3_x)
{
	unsigned char opcode;

	opcode = instruction & 0b1111111; //need to filter out unwanted bits. Just use 7 1's to filter out that unwanted bit.

	switch(opcode)
	{
		case 0b0010011: 
			decode_op19(instruction, opcode, argv3_x);
			break;
		
		case 0b0110111: 
			decode_op55(instruction, opcode, argv3_x);
			break;
		
		case 0b1101111:
			decode_op111(instruction, opcode, argv3_x);
			break;

		case 0b1100111:
			decode_op103(instruction, opcode, argv3_x);
			break;
		
		case 0b0000011: 
			decode_op3(instruction, opcode, argv3_x);
			break;
		
		case 0b0100011: 
			decode_op35(instruction, opcode, argv3_x);
			break;
		
		case 0b0110011: 
			decode_op51(instruction, opcode, argv3_x);
			break;
		
		default:
			//if an invalid instruction is found, use a function to handle this, or just type it here
			fprintf(fout, "    inv    invalid\t\t//0x%-.8x\n", instruction); //format this later. This prints to a FILE!!
			break;
	}

	//prints hex address of each instruction AFTER instruction is printed (i.e., "print_decoded_instruction" function was called)
//	fprintf(fout, "//0x%-.8x\n", instruction);//printing hex stuff. Adjust leading spaces as needed. Use a combination of \t (tab) and spaces to get right. ".8" makes 0x 8 digits long!! Sean got this from man command. LOOK AT FPRINTF DOCUMENTATION for how to use left/right justification, length, etc. EX. fprintf(//0x%-30.8x\n).
}


//NOTES:
//1. Don't forget to TWO'S COMPLEMENT negative immediates
//2. argv[1] = infile.bin, argv[2] = output, argv[3] = optional a or x

int main(int argc, char *argv[])
{

	//check num of arguments (should be at least 3, but no greater than 4
	if ((argc < 3)||(argc > 4))
	{
		printf("Invalid number of arguments (%d arguments identified).", argc);
	}

//	cout << "\n\n\nargv[3]: " << argv[3] << "\n\n\n";
	
	FILE *fin, *fout;
	int filesize;
	int *instructions; //array to store instructions
	bool argv3_x = false;

	if ((argc == 4) && (*(argv[3]) == 'x')) {argv3_x = true;}

	fin = fopen(argv[1], "rb");

	//error check for opening file -- fin != NULL

	//fseek to end of file
	fseek(fin, 0, SEEK_END);
	filesize = ftell(fin);
	rewind(fin); //gets us back to the end of file fin

	//allocate instructions
	instructions = new int [filesize/4];//creates an array where each element == one instruction from file. "filesize/4" should be 24 for our particular file.

	fread(instructions, 1, filesize, fin); //not sure about "filesize" in that 3rd argument.

	//make sure fread returned "filesize" number of bytes. If not, send an error message.

	fclose(fin);

//	//Test code	
//	cout << "instructions[1] = " << instructions[1] << endl;

	if (argv[2][0] == '-')
	{
		fout = stdout;//stdout is a filestream. output to terminal. This is in writeup in 2nd user argument section
	}
	else
	{
		fout = fopen(argv[2], "w");
	}

	DECODER D(fout);

	for (int i = 0; i < filesize/4; i++)
	{
		//decode process to identify full instruction starting with the opcode.
		D.process_opcode(instructions[i], argv3_x);
	}




	delete [] instructions;


	return 0;
}
