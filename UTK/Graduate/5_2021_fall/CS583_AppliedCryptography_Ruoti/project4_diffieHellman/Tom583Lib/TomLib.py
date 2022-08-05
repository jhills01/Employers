# IMPORTANT NOTES: 
# 1) Python environment for Visual Studio (Python 3.7) is different than Ubuntu's.
#    Some imports will show as "unresolved" here, but they are NOT b/c I installed them in Ubuntu.
#    If I want to get rid of "unresolved", then I install lib in "Python Environment" tab here. 

import sys;
from Crypto.Util import number;

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






#################################### UN-NEEDED CODE ###########################################
# apint == arbitrary percision integer
def convertStrToAPInt(string):
    if (debug):
        print(f"\nCONVERT-STR-APINT:\n\nstring:\n {string}");
    
        # use 'utf-8' instead of 'ascii'. It's the same, but expanded
        # try/except is used in case "string" parameter is byte string
        try:
            byteStr = string.encode('utf-8');
        except (UnicodeDecodeError, AttributeError):
            byteStr = string;
     
        #byteStr = string.encode('utf-16'); # DON'T USE. Uses 2 bytes per character instead of 1
        print(f"\nstring.encode('utf-8'):\n {byteStr}");

        # NOTE: Don't need to add '0x' for int() to work!!
        #newByteStr = "0x" + bytes.hex(byteStr);
        hexByteStr = bytes.hex(byteStr); 
        print(f"\nbytes.hex(byteStr):\n {hexByteStr}");

        integer = int(hexByteStr, 16);
        print(f"\nint(hexByteStr, 16):\n {integer}")

    else:
        # use 'utf-8' instead of 'ascii'. It's the same, but expanded
        # try/except is used in case "string" parameter is byte string
        try:
            byteStr = string.encode('utf-8');
        except (UnicodeDecodeError, AttributeError):
            byteStr = string;
     
         # NOTE: Don't need to add '0x' for int() to work!!
        #newByteStr = "0x" + bytes.hex(byteStr);
        hexByteStr = bytes.hex(byteStr); 
        integer = int(hexByteStr, 16);

    # convert string to 
    return integer;

# apint == arbitrary percision integer
def convertAPIntToStr(apint):
    if (debug):
        print(f"\nCONVERT-APINT-STR:\n\napint:\n {apint}");
        if (debugSize): print(f"\nsys.getsizeof(apint):\n {sys.getsizeof(apint)} bytes");

        # "[2:]" skips "0x" portion of hex string to avoid Valueerror in fromhex()
        hexStr = hex(apint)[2:]; 
        print(f"\nhex(apint):\n {hexStr}");

        byteStr = bytes.fromhex(hexStr);
        print(f"\nbytes.fromhex(hexStr):\n {byteStr}");

        string = byteStr.decode("utf-8");
        print(f"\nbyteStr.decode(\"utf-8\"):\n {string}");

    else:
        # "[2:]" skips "0x" portion of hex string to avoid Valueerror in fromhex()
        hexStr = hex(apint)[2:]; 
        byteStr = bytes.fromhex(hexStr);
        string = byteStr.decode("utf-8");

    return string;
