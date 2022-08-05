//Lab 1A
//COSC130
//Lab synopsis
//John Hills
//07/15/2018
//List any TAs/students that you worked with here: Hudson (student) and Sean (TA).
#include <string>
#include <stdio.h>
using namespace std;

//Prototypes for the functions that you will write
int CharToInt(char v)
{
	//changed from int 48 to char '0' for portability. Dr. Marz explained that the compiler would then make the conversion from '0' to whatever system-specific integer.
	return (v-'0');
}

char IntToChar(int v)
{
	return (v+'0');
}

string IntToString(int val)
{
	string str = "";
    bool valIsNegative = 0;

	//a negative val produces incorrect characters in str during while loop.
	if (val < 0){val = -1*val; valIsNegative = 1;}

    //increases the number of loops in the next while by 1.
    int valCopy = 10*val;

    //stores each digit of val in str as a character in reverse order.
    while (valCopy/=10)
    {
        str.push_back(IntToChar(val%10));
        val = val/10;
    }

	//valIsNegative used instead of (val < 0) b/c val and valCopy == 0 now.
	if (valIsNegative) {str.push_back('-');}

    //Reverses the character order of str into strReverse.
    int strMaxIndex = str.length()-1;//Storing str's length in variable to avoid excessive invocation of .length() function.
    string strReverse = str;

    for (int i = 0; i <= strMaxIndex; i++)
    {
        strReverse[strMaxIndex-i] = str[i];
    }

    return strReverse;
}

int StringToInt(string val)
{
    int iVal = 0;
    int m = 1;
    bool valIsNegative = 0;

    //erases '-' from val for proper calculation of iVal in for loop.
    if (val[0]== '-') {val.erase(0,1); valIsNegative = 1;}

    for (int i = val.length()-1; i >= 0; i--)
    {
        iVal += m*CharToInt(val[i]);
        m *= 10;
    }

    if (valIsNegative) {iVal = -1*iVal;}

    return iVal;
}

int main(int argc, char *argv[])
{
    string sresult;
    int left;
    int right;
    char op;

    if (4 != argc) {
            printf("Usage: %s <left> <op> <right>\n", argv[0]);
            return -1;
    }
    //Notice that this calls your StringToInt. So, make sure you debug
    //StringToInt() to make sure that left "cArray[0] == " << and right get a proper
    //value.
    left = StringToInt(argv[1]);
    right = StringToInt(argv[3]);
    op = argv[2][0];
    //so debug your IntToString() function to make sure that sresult
    //is given the proper result. This assumes your StringToInt already
    //works.
    switch (op)
    {
            case 'x':
                    sresult = IntToString(left * right);
                    break;
            case '/':
                    sresult = IntToString(left / right);
                    break;
            case '+':
                    sresult = IntToString(left + right);
                    break;
            case '-':
                    sresult = IntToString(left - right);
                    break;
            case '%':
                    sresult = IntToString(left % right);
                    break;
            default:
                    sresult = IntToString(left);
                    break;
    }

    //Remember, printf is the only output function you may use for this lab!
    //The format string is %d %c %d = %s. This means that the first argument
    //is %d (decimal / integer), %c (character), %d (decimal /integer),
    //%s (string). Notice that because printf() is a C-style function, you
    //must pass strings as character arrays. We can convert a C++ string
    //to a character array (C-style string) by using the c_str() member function.
    printf("%d %c %d = %s\n", left, op, right, sresult.c_str());

  return 0;
}

