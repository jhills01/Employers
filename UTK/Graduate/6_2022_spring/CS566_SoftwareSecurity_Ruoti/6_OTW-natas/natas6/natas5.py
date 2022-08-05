import requests # ignore the error. Requests module is installed on school computers. 

website = "http://natas5.natas.labs.overthewire.org/"

sess = requests.Session()

sess.auth = ('natas5', 'iX6IOfmpN7AYOQGPwtn3fXpbaJVJcHfq')
# sess.headers.update({'Set-Cookie': "loggedin=1"})
resp = sess.get(website)
resp.headers.update({'Set-Cookie': "loggedin=1"})

print(resp.text)
print(resp.headers)
