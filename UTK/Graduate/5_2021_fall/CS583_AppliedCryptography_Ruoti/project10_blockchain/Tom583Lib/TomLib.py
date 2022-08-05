# IMPORTANT NOTES: 
# 1) Python environment for Visual Studio (Python 3.7) is different than Ubuntu's.
#    Some imports will show as "unresolved" here, but they are NOT b/c I installed them in Ubuntu.
#    If I want to get rid of "unresolved", then I install lib in "Python Environment" tab here. 

import sys;
import math;
from Crypto.Util import number;
from random import randrange;

debug = False;
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
        # print(f"\nCONVERT-APINT-BYTE_ARR:\n\ninteger:\n {integer}");
        
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
    
    ## NOTE: Not using "int.to_bytes()" b/c it wants "length" of byte string.
    ##       By using "hex", I avoid have to figure this out.
    #hexStr = hex(apint)[2:]; 
    #byteStr = bytes.fromhex(hexStr);
    #return byteStr.decode("utf-8");

    # WAIT!! Turns out using int.to_bytes() is the way Dr. Ruoti
    # ..., or at least the passoff server, writes these projects. 
    # It is possible to get different results using hex() as opposed to 
    # ... to_bytes() depending on steps taken, so I'm changing 
    # ... this to use int.to_bytes instead since I have getByteSize
    # ... already to go :)

    byteStr = apint.to_bytes(getByteSize(apint), "big");
    return str(byteStr, "utf-8");

    # "correct" approach
    print(f"YAY. I CHANGED TO TO_BYTES()");
    byteStr = apint.to_bytes(getByteSize(apint), "big");
    print(f"\n{byteStr}");
    
    hexStr = hex(apint)[2:]; 
    byteStr2 = bytes.fromhex(hexStr);
    print(f"\n{byteStr2}\n");

    if byteStr == byteStr2: print(f"\nTHEY EQUAL!! WTF!!!")
    
    return str(byteStr, "utf-8");

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

def getByteSize(integer):
    integerBitSize = int.bit_length(integer);
    if (integerBitSize % 8 != 0): 
        # bit size is NOT a multiple of 8. Add 1 byte to compensate for bits no comprising a full byte
        integerByteSize = integerBitSize // 8 + 1;
    else:
        # bit size is a multiple of 8. No need to add additional byte as above. 
        integerByteSize = integerBitSize // 8;

    return integerByteSize;

# NOTE: This function is not meant to be called. 
#       It's just here for reference on how to quickly convert string to byte string/array. 
def convertStrToByteStr(s):
    return bytes(s, 'utf-8');