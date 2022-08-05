import base64
import json

def xor_decrypt(plaintext, ciphertext):
    s = ""

    for i in range(len(plaintext)):
        s += str(chr(ciphertext[i] ^ plaintext[i % len(plaintext)]))

    return s

# Original cookie ('document.cookie' in dev console) == ClVLIh4ASCsCBE8lAxMacFMZV2hdVVotEhhUJQNVAmhSEV4sFxFeaAw%3D where "%3D" substring is URL encoded '='.
# Thus, our cipher text is as follows:  
ciphertext = b"ClVLIh4ASCsCBE8lAxMacFMZV2hdVVotEhhUJQNVAmhSEV4sFxFeaAw="
ciphertext = base64.decodebytes(ciphertext)
plaintext = {"showpassword":"no", "bgcolor":"#ffffff"}
# Here, we remove the space as JSON implementation in Python is different from PHP
plaintext = json.dumps(plaintext).encode('utf-8').replace(b" ", b"")

secret = xor_decrypt(ciphertext, plaintext)
print(secret)