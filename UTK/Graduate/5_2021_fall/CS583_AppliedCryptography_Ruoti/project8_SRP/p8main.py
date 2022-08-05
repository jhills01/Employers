# standard libraries
import sys, os
import random;

from Crypto.Util import number;
from Crypto.Hash import SHA256;
from Crypto.Cipher import AES;
from Crypto.Util.Padding import pad;

# libraries in subfolders
sys.path.append(os.path.realpath('.') + '/Tom583Lib'); # "...realpath('.') returns path string for current directory

from TomLib import *;

skipInput = False;

# assign p and g from 1st page of passOff website 
p = 233000556327543348946447470779219175150430130236907257523476085501968599658761371268535640963004707302492862642690597042148035540759198167263992070601617519279204228564031769469422146187139698860509698350226540759311033166697559129871348428777658832731699421786638279199926610332604408923157248859637890960407;
g = 5;

# select private key, "a", between p/2 and p to ensure it's large enough
a = random.randint(p//2+1, p);
print(f"a:\n{a}\n");
#print(f"\na:(size == {int.bit_length(a)} bits)\n{a}\n");

# public key
# g^a (mod p)
myPublicKey = fastModExpModN(g, a, p);
print(f"\nmyPublicKey:\n{myPublicKey}\n");

# Values from 2nd page of passoff server
# x=H(salt || password)^iterations
# NOTE: This isn't stated anywhere, but it turns out password must be transformed into hex string
username = "jhills";
# password = "lamentation";
if not skipInput: password = input(f"Please enter 'password' from passoff server:\n");
else: password = "blah"
# NOTE: Dr. Ruoti said NOT to use "hex()" as it's not the "correct approach"
#       Below for finding "k", I used int.to_bytes() instead and it gave me correct answer
#       ... right away without  all the "converting-to-hex" trouble I ran into with hex().
#       Read below for the details of the trouble I had. 
passwordHex = hex( convertStrToAPInt(password) )[2:];


## NOTE: Just changed my "convertAPIntToStr()" function to use int.to_bytes() instead. 
##       Testing that below. Although, sending myPublicKey as input errored out for both!!!
##       BUT, both approaches do produce the same byte string, so..........
#print(f"\n\nTEST:");
#test1 = convertAPIntToStr( myPublicKey );
#test2 = convertStrToAPInt (test1); 
#print(f"{test2}");
#print(f"END TEST\n\n");

if not skipInput: salt_hex = input(f"Please enter 'salt' from passoff server:\n");
else: salt_hex = "ae";
#salt_hex = "493ff196";
#print(f"{salt_hex} ::: {passwordHex}");

if not skipInput: 
    B = input(f"Please enter 'B' integer from passoff server:\n");
    B = int(B, 10);
else: B = 0;

pwHashIterations = 1000;

# calculate values for 2nd page of passoff server
saltPwHexStr = salt_hex + passwordHex; 
saltPwByteStr = bytes.fromhex(saltPwHexStr);

runningHashHex = SHA256.new(saltPwByteStr).hexdigest();
pwHashIterations -= 1;
for i in range(pwHashIterations):
    runningHashByteStr = bytes.fromhex( runningHashHex );
    runningHashHex = SHA256.new( runningHashByteStr ).hexdigest();

x = int(runningHashHex, 16);
print(f"x:\n{x}\n");

# k = H(p || g) as an integer
pgByteStr = p.to_bytes(getByteSize(p), "big") + g.to_bytes(getByteSize(g), "big");
#print(f"pgByteStr:\n{pgByteStr}\n");
kHexStr = SHA256.new( pgByteStr ).hexdigest();
#print(f"kHexStr:\n{kHexStr}\n");
kInt = int(kHexStr, 16);
k = kInt;
print(f"kInt:\n{kInt}\n");

# g^b ≡ B-k·v (mod p)
v = fastModExpModN(g, x, p)
# g^b (mod p)
serverPublicKey = (B - modMultModN(k,v,p)) % p;
print(f"serverPublicKey:\n{serverPublicKey}\n");

# u = H(g^a || g^b)
uByteStr = myPublicKey.to_bytes(getByteSize(myPublicKey), "big")\
   + serverPublicKey.to_bytes(getByteSize(serverPublicKey), "big");
#print(f"uByteStr:\n{uByteStr}\n");
uHexStr = SHA256.new( uByteStr ).hexdigest();
#print(f"uHexStr:\n{uHexStr}\n");
u = int(uHexStr, 16);
print(f"u:\n{u}\n");

# Shared key ( (g^b)^(a + x*u) )
exponent = ( a + modMultModN(x,u,p) ) % p;
sharedKey = fastModExpModN(serverPublicKey, exponent, p);
print(f"sharedKey:\n{sharedKey}\n");

#3RD PASSOFF SERVER PAGE
# building M1
# 1st term
pByteStr = p.to_bytes(getByteSize(p), "big");
gByteStr = g.to_bytes(getByteSize(g), "big");
pHashHex = SHA256.new( pByteStr ).hexdigest();
gHashHex = SHA256.new( gByteStr ).hexdigest();
pgXORint = int(pHashHex, 16) ^ int(gHashHex, 16);
pgXORintByteStr = pgXORint.to_bytes( getByteSize(pgXORint), "big" );

# 2nd term
# msg = "jhills";
msg = username;
msgInt = convertStrToAPInt(msg);
msgByteStr = msgInt.to_bytes( getByteSize(msgInt), "big" );
msgHashHex = SHA256.new( msgByteStr ).hexdigest();
msgHashInt = int(msgHashHex, 16);
msgHashByteStr = msgHashInt.to_bytes( getByteSize(msgHashInt), "big" );

# 3rd term
saltByteStr = bytes.fromhex(salt_hex);

# 4th term
gAByteStr = myPublicKey.to_bytes( getByteSize(myPublicKey), "big" );

# 5th term
gBByteStr = serverPublicKey.to_bytes( getByteSize(serverPublicKey), "big" );

# 6th term
sharedKeyByteStr = sharedKey.to_bytes( getByteSize(sharedKey), "big" );

M1 = SHA256.new( pgXORintByteStr + msgHashByteStr + saltByteStr + gAByteStr + gBByteStr + sharedKeyByteStr).hexdigest();
print(f"M1:\n{M1}\n");

# building M2
M1ByteStr = bytes.fromhex(M1);
M2 =  SHA256.new( gAByteStr + M1ByteStr + sharedKeyByteStr ).hexdigest();
print(f"M2:\n{M2}\n");
