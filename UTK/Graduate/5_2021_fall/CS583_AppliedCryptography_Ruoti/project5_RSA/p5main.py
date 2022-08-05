# standard libraries
import sys, os
import random;
from Crypto.Util import number;

# libraries in subfolders
sys.path.append(os.path.realpath('.') + '/Tom583Lib'); # "...realpath('.') returns path string for current directory

from TomLib import *;

# cycle until we get (p-1)*(q-1) that is relatively prime to e = 65537
# To be relatively prime, 
e = 65537;
phi_n = e; # this ensures that GCD == e, not 1 the first time
gcd, s, t = extendedEuclidian(phi_n, e);
while ( gcd != 1 ):
    p = number.getPrime(1024);
    q = number.getPrime(1024);
    n = p*q;
    phi_n = (p-1)*(q-1);
    gcd, s, t = extendedEuclidian(phi_n, e);

# get final secret key, d, value
d = t % phi_n;

print(f"p: {p},\n\nq: {q}\n\nn: {n}, \n\nphi_n: {phi_n}, \n\nd: {d}");

# message to encrypt given by passoff website's 2nd page
msg = input(f"\nEnter message to encrypt:\n");
m = convertStrToAPInt(msg);
c = fastModExpModN(m, e, n);
print(f"\nc:\n{c}\n");

# chipher given by passoff website 3rd page
c = input(f"Enter encrypted integer:\n");
c = int(c);

decrypted = fastModExpModN(c, d, n);
decryptedStr = convertAPIntToStr(decrypted);
print(f"\ndecryptedStr: {decryptedStr}\n");
