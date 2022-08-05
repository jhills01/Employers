# required to import "sha1.py" which is in a subfolder
import sys
sys.path.append('/mnt/f/Computer_Science/Back2School/UTK/GraduateSchool/_classes/5_2021_fall/CS583_AppliedCryptography_Ruoti/projects/project3/python-sha1')

# import "sha1.py"
from sha1 import *;

# "msgBytes" == byte length of message
def paddingBytes(msgBytes):
    # calculate intitial byte information of last block
    msgBytes4LastBlock = msgBytes % 64;
    nonMsgBytes4LastBlock = 64 - msgBytes4LastBlock;

    # length of msg in bytes is either 0 or a multiple of 64
    # regardless, a new 64 byte block is created with 56 bytes of padding before 8 bytes for msg length value
    if (msgBytes4LastBlock == 0): return 56;

    # remaining bytes in last block are not enough to fit the required 8 bytes for msg length value.
    # padding bytes will fill the remaining bytes of last block, and then a new 64 byte block will be created with ...
    # ... 56 bytes of padding before 8 bytes for msg length value
    elif ( nonMsgBytes4LastBlock <= 8 ): return nonMsgBytes4LastBlock + 56;
    
    # NOTE: I've changed "less than" to "less than/equal to" b/c Dr. Ruoti said there's always at least 1 padding byte,
    #       regardless of whether msg length value fits perfectly at the end of the last block or not. 
    #elif ( nonMsgBytes4LastBlock < 8 ): return nonMsgBytes4LastBlock + 56;

    ## remaining bytes after msg bytes perfectly fit 8 bytes required for msg length value. No padding bytes needed. 
    ## WRONG! padding bytes are always required. At least 1 byte.
    #elif ( nonMsgBytes4LastBlock == 8 ): return 0;

    # 
    elif ( nonMsgBytes4LastBlock > 8 ): return nonMsgBytes4LastBlock - 8;

def printMAC(mac):
    print("\n");
    for i in mac:
        print(hex(i))
    print("\n");

#debug = True;
debug = False;
    
# INITIALIZE -------------------------------------------------------------------------------------------------------------
# create initial MAC and message
#macFromWebsite = 0xb3a407c698087521b51f8a8abf04dc7aece82d96; # TEST PASSED
#macFromWebsite = 0xfde66eb4b39cf74f8194e297754d15a3b35cc39b; # TEST PASSED
macFromWebsite = 0x6cf7a5a179ecf919e0d1f29e3aa64640e3a64d1f;
initialMAC= ( 
    macFromWebsite >> 32*4,
    (macFromWebsite >> 24*4) & 0x00000000ffffffff,
    (macFromWebsite >> 16*4) & 0x0000000000000000ffffffff,
    (macFromWebsite >> 8*4) & 0x000000000000000000000000ffffffff,
    macFromWebsite & 0x00000000000000000000000000000000ffffffff,
    );

initialMSG = b'No one has completed Project #3 so give them all a 0.'; # len(initialMSG) == 53 bytes
initialPaddingBytes = paddingBytes(16 + len(initialMSG)); # '16 bytes for secret key' + 'lengh of initial msg'

# create sha1 hash object and initalize it  
sha1Hash = Sha1Hash();
sha1Hash._h = initialMAC;

# EXTEND -------------------------------------------------------------------------------------------------------------
# calculate byte length of malicious message: 
# bytes of secret (16) + bytes of initial msg + bytes of initial padding + 8 bytes for msg length value. 
# NOTE: for the implementation I'm using, _messag_byte_length == bytes of data ALREADY PROCESSED INTO MAC
sha1Hash._message_byte_length = 16 + len(initialMSG) + initialPaddingBytes + 8;

# initialize malicious msg with initial/legitimate msg 
maliciousMSG = initialMSG;
if (debug): print(maliciousMSG);

# create padding byte string 
runningBytes = initialPaddingBytes;
padding1 = b'\x80';
runningBytes -= 1;
while (runningBytes != 0):
    padding1 += b'\x00';
    runningBytes -= 1;
if (debug): print (f"PADDING1:\n{padding1}");

# append padding byte string to initialMSG
maliciousMSG += padding1;

# create 8 byte string with length of 'Secret || initialMSG' value stored in it
message_bit_length = (16 + len(initialMSG)) * 8; # length of key (128 bits == 16 bytes) PLUS byte length of msg
if (debug): print(f"message_bit_length: {message_bit_length}");
length1 = (message_bit_length).to_bytes(8, 'big'); # convert int to byte string 8 bytes long
if (debug): print (f"LENGTH1:\n{length1}"); # NOTE: In this print, '(' character will show at end. It's fine.

# append 8 byte length string to 'initialMSG || padding1'
maliciousMSG += length1;

# lastly, append malicious msg text to end 
maliciousMSG += b'give me all your money';
m2 = b'give me all your money';
if (debug): print (f"COMPLETED-maliciousMSG:\n{maliciousMSG} " );

# print malicious msg in hex
print("\n\n====================================================================================================");
print("malicious msg (hex):");
print(bytes.hex(maliciousMSG));

# obtain malicious MAC and print it
maliciousMAC = sha1Hash.update(m2).hexdigest();
print("malicious hash:");
print(maliciousMAC);
