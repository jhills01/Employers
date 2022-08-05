import base64
import json

def xor_encrypt(key, cookie):
    d = ""
    for i in range(len(key)):
        d += str(chr(cookie[i] ^ key[i % len(key)]))

    d = base64.encodebytes(d.encode('utf-8'))
    return d

# Added extra 'w' at the end b/c "yes" == 3 bytes and "no" == 2 bytes. 
# Need extra byte which is 'w' b/c it continues the 'qw8J' pattern of the encoding
key = b"qw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw8Jqw"
new_cookie = {"showpassword":"yes", "bgcolor":"#ffffff"}
new_cookie = json.dumps(new_cookie).encode('utf-8').replace(b" ", b"")

data = xor_encrypt(key, new_cookie)
print(data)