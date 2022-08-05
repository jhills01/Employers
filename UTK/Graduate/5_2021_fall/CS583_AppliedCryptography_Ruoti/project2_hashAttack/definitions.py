import hashlib;
#from string import ascii_letters;
import string;
import random;

def printDictionary(d):
    for key, value in d.items():
        print(key, " : ", value);

# produces random strings of lower case letters and digits 0 - 9 with size "size"
def stringSequenceGenerator(size=24, chars=string.ascii_lowercase + string.digits):
   return ''.join(random.choice(chars) for _ in range(size))

# creates simple string sequence "a", "b", ..., "z", "aa", "bb", ..., "zz", "aaa", .. etc
# ... based on currrentString value
def obsolete_stringSequenceGenerator(currentString):
    
    stringLength = len(currentString);
    currentStringCharacter = currentString[0];
    newString = "";

    # if string characters aren't "z", we move to the next SAME LENGTH string in sequence
    if currentString[0] != "z":
        for char in currentString:
            newString += ascii_letters[(ascii_letters.index(char)+1)%len(ascii_letters)];

        return newString;

    # otherwise, we move on to the next sequence string of all "a" characters that has length increased by 1
    else:
        return "a" * (stringLength + 1);
        
def sha256Wrapper(string2Hash, hashBits):
    
    h = hashlib.sha256( bytes(string2Hash, 'utf-8') );
    #print("sha256 hash: " + h.hexdigest()); # test print
    return hex( int("0x" + h.hexdigest() , 16 ) >> (256 - hashBits) );
