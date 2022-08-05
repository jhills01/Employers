import sys, os;
import random;

from Crypto.Util import number;
from Crypto.Hash import SHA256;
from Crypto.Cipher import AES;
from Crypto.Util.Padding import pad;

# libraries in subfolders
sys.path.append(os.path.realpath('.') + '/Tom583Lib'); # "...realpath('.') returns path string for current directory

from TomLib import *;

while(True): # changing to infinite loop so that I can keep going as long as I want and then ctrl-c out

    # VERY IMPORTANT NOTE: Using int(...) truncated leading 0s of the hash which lead to a byte string with those leading 0 bytes missing
    #                      Simply using byte.fromhex(...) corrected everything. 
    #                      The most confusing part was that I would correctly add first block everytime b/c Genesis block didn't have a hash with
    #                      ... leading 0s. However, b/c of the proof of work involving a hash with 1st 10 bits being 0, the problem would then crop up.
    gBlockHashStr = input("Paste previous block hash:\n");
    gBlockByteStr = bytes.fromhex(gBlockHashStr);

    quoteStr = input("Paste 'quote':\n");
    quoteByteStr = bytes(quoteStr, 'utf-8'); # NOTE: instructions say to encode with ASCII, but utf-8 should work too

    nonceInt = 0;
    nonceByteStr = nonceInt.to_bytes(getByteSize(nonceInt), "big");
    
    hashBlockHex = SHA256.new(gBlockByteStr + nonceByteStr + quoteByteStr).hexdigest();

    while( (int(hashBlockHex, 16) >> 246) != 0):
        nonceInt += 1;
        nonceByteStr = nonceInt.to_bytes(getByteSize(nonceInt), "big");
        
        hashBlockHex = SHA256.new(gBlockByteStr + nonceByteStr + quoteByteStr).hexdigest();

    print(f"\nSUCCESS!!!\n" + \
          f"nonceInt:\n{nonceInt}\n" + \
          f"hashBlockHex:\n{hashBlockHex}\n" + \
          f"-------------------------------------------------------------------------------\n");
