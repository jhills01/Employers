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

# NOTE: Use this later. 
# hashlib.sha256

# randomly generate a cryptographically "strong" prime, p, s.t. (p-1)/2 is also prime.
# it must also be at least 1024 bits 
# REFERENCE: https://datatracker.ietf.org/doc/rfc3526/?include_text=1
strongPrimeHex = "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF"
p = int(strongPrimeHex, 16);
print(f"\n\np:(size == {number.size(p)} bits)\n{p}\n");

# assign g, which is referenced from passOff website 
g = 5;

# select private key, "a", between p/2 and p to ensure it's large enough
a = random.randint(p//2+1, p);
#print(f"\na:(size == {int.bit_length(a)} bits)\n{a}\n");

# public key
myPublicKey = fastModExpModN(g, a, p);
print(f"\nmyPublicKey:\n{myPublicKey}");

# passoff server public key ####################################################################### 
serverPublicKey = input("\nEnter public key from 'passoff' server:\n");

# calculate shared key (g**b == base, a == exponent, p == n to mod by)
sharedKey = fastModExpModN( int(serverPublicKey), a, p );
print(f"\nsharedKey:\n{sharedKey}");

# determine byte size of "sharedKey". 
sharedKeyBitSize = int.bit_length(sharedKey);
if (sharedKeyBitSize % 8 != 0): 
    # bit size is NOT a multiple of 8. Add 1 byte to compensate for bits no comprising a full byte
    sharedKeyByteSize = sharedKeyBitSize // 8 + 1;
else:
    # bit size is a multiple of 8. No need to add additional byte as above. 
    sharedKeyByteSize = sharedKeyBitSize // 8;
#print(f"\nsharedKeyByteSize: {sharedKeyByteSize}, sharedKeyBitSize: {int.bit_length(sharedKey)}");

# convert "sharedKey" integer to byte string for SHA256
sharedKeyByteString = sharedKey.to_bytes(sharedKeyByteSize, "big");
#print(f"\nsharedKeyByteString:\n{sharedKeyByteString}");

# get sha256 hash from sharedKey and convert to int for bit shifting  
sha256HexDigestStr = SHA256.new(sharedKeyByteString).hexdigest(); 
#print(f"\sha256HexDigestStr:\n{sha256HexDigestStr}");
sha256HexDigestInt = int(sha256HexDigestStr, 16);
#print(f"\nsha256HexDigestInt:\n{sha256HexDigestInt}");

# bit shift to obtain symmetricKey from the higher order 128 bit (16 bytes) of sha256 hash
symmetricKey = sha256HexDigestInt >> 128;
symmetricKeyBytes = symmetricKey.to_bytes(16, "big");
#print(f"\nhex(symmetricKey):\n{hex(symmetricKey)}");
#print(f"\nsymmetricKeyBytes: {symmetricKeyBytes}");

# passoff server IV ##############################################################################
iv = input("\nEnter IV from 'passoff' server:\n");
#iv = int(iv, 16).to_bytes(16, "big"); # BETTER WAY ON NEXT LINE
iv = bytes.fromhex(iv);
#print(f"IV: {iv}");

# passoff server Ciphertext#########################################################################
ciphertext = input("\nEnter ciphertext from 'passoff' server:\n");
#ciphertext = int(ciphertext, 16).to_bytes(len(ciphertext.encode('utf-8')), "big");
ciphertext = bytes.fromhex(ciphertext);
#print(f"\nciphertext:\n{ciphertext}");

# decrypt ciphertext ###############################################################################
aes = AES.new(symmetricKeyBytes, AES.MODE_CBC, iv);
plaintext = aes.decrypt(ciphertext);
#print(f"plaintext:\n{plaintext}");
plaintext = plaintext.decode("utf-8");
print(f"plaintext:\n{plaintext}");
