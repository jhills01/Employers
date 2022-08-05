# IMPORTANT NOTES: 
# 1) Python environment for Visual Studio (Python 3.7) is different than Ubuntu's.
#    Some imports will show as "unresolved" here, but they are NOT b/c I installed them in Ubuntu.
#    If I want to get rid of "unresolved", then I install lib in "Python Environment" tab here. 

import sys;
import math;
from Crypto.Util import number;
from random import randrange;

debug = True;
debugSize = True;

# converts byte array (e.g., b'\x80\x01') to arbitrary precision integer
def convertByteArrToAPInt(byteArr):
    if (debug):
        print(f"\nCONVERT-BYTE_ARR-APINT:\n\nbyteArr:\n {byteArr}");
        
        integer = int.from_bytes(byteArr, "big");
        print(f"\nint.from_bytes(byteArr, \"big\"):\n{integer}");
    else:
        integer = int.from_bytes(byteArr, "big");

    return integer;

# converts arbitrary precision integer to byte array (e.g., b'\x80\x01')
def convertAPIntToByteArr(integer, byteArrSize):
    if (debug):
        print(f"\nCONVERT-APINT-BYTE_ARR:\n\ninteger:\n {integer}");
        
        byteArr = integer.to_bytes(byteArrSize, "big");
        print(f"\nint.to_bytes(integer, \"big\"):\n{byteArr}");

    else:
        byteArr = integer.to_bytes(byteArrSize, "big");

    return byteArr;

# b == base, e == exponent
def fastModExpModN(b, e, n):
    if (debug):
        # get bit size of e
        eBits = number.size(e);
        runningBits = eBits;
        runningBitShift = e;
        product = 1;

        while (runningBits != 0):
            if (runningBitShift & 0x1):
                product = (product*b) % n;
        
            b = (b*b) % n;
            
            # adjust running variables
            runningBitShift >>= 1;    
            runningBits -= 1;

    else:
        # get bit size of e
        eBits = number.size(e);
        runningBits = eBits;
        runningBitShift = e;
        product = 1;

        while (runningBits != 0):
            if (runningBitShift & 0x1):
                product = (product*b) % n;
        
            b = (b*b) % n;
            
            # adjust running variables
            runningBitShift >>= 1;    
            runningBits -= 1;
            1
    return product;

def modAddModN(a, b, n):
    return ((a%n) + (b%n)) % n;

def modMultModN(a, b, n):
    product = ((a%n)*(b%n))%n;
    return product;

# apint == arbitrary percision integer
def convertStrToAPInt(string):
    # check if "string" is already a byte string
    try:
        byteStr = string.encode('utf-8');
    except (UnicodeDecodeError, AttributeError):
        byteStr = string;

    return int.from_bytes(byteStr, "big");

# apint == arbitrary percision integer
def convertAPIntToStr(apint):
    #byteStr = int.to_bytes(apint);
    
    # NOTE: Not using "int.to_bytes()" b/c it wants "length" of byte string.
    #       By using "hex", I avoid have to figure this out.
    hexStr = hex(apint)[2:]; 
    byteStr = bytes.fromhex(hexStr);
    return byteStr.decode("utf-8");

def gcd(a,b):
    
    while(b != 0):
        a,b = b, a%b;
    
    return a;

def extendedEuclidian(a, b):

	#Set default values for the quotient, remainder, 
	#s-variables and t-variables
	q, r = 0, 1;
	s1, s2, s3 = 1, 0, 1; 
	t1, t2, t3 = 0, 1, 0; 
    
    # while remainder, r, is greater than 0, we continue 
    # ... calculating quotient, remainder, and 
	while(r > 0):
		# get quotient, q, remainder, r, and  s/t-variables
		#q = math.floor(a/b) #NOTE: math.floor overflows with large numbers
		q = a // b;
		r = a - q * b;
		s3 = s1 - q * s2
		t3 = t1 - q * t2
		
        # if remainder, r, greater than 0 at this point, 
        # we update values in preparation for next iteration
		if(r > 0):
			a = b
			b = r
			s1 = s2
			s2 = s3
			t1 = t2
			t2 = t3

	return abs(b), s2, t2
