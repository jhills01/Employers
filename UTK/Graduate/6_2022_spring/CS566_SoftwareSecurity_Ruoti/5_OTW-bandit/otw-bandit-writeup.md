# OverTheWire Bandit Project

## Student Information

* Student name: Tom Hills
* NetId: jhills

## Project Description

In this project, you will be complete the Bandit challenges at <https://overthewire.org/wargames/bandit/>. For each challenge, you will complete the following worksheet describing:
1. The credentials found in the challenge.
2. How you solved the challenge. Include any JavaScript or other scripts necessary to retrieve the credentials.
3. What mitigations should be used to address the exploit.

In the workshop, the challenges are named based on the username used to login to the shell for that challenge. For example, the challenge labeled "Bandit 10" will be logged into with the username bandit10, and the solution will be the credentials for the user bandit11.

You can find a basic guide for markdown formatting [here](https://www.markdownguide.org/basic-syntax/). In particular, note the [code section](https://www.markdownguide.org/basic-syntax/#code) which should be used to annotate any code or commands used in your writeup.

## Hints
There are hints for each challenge on the [Bandit](https://overthewire.org/wargames/bandit/) webpage.

Below are several additional hints:
* Bandit 24: The netcat server dies if sent too much input. Limit how much content you send per connection.
* Bandit 25: This level is only finished once you have the password for bandit26. The ssh key along is insufficient. Also, how does `vi` factor into all of this?
* Bandit 28–30: Core `git` features include commits, branches, and tags.
* Bandit 32: `sh` has access to argc and argv.


## Challenges

---
### Bandit 00

#### Credentials
Username: bandit1
Password: boJ9jbbUNNfktd78OOpsqOltutMc3MY1

#### How you passed the challenge
Simply use `cat readme` command to read password from file

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext



---
### Bandit 01

#### Credentials
Username: bandit2
Password: CV1DtqXWVFXTvM2F0k09SHz0YwRINYA9

#### How you passed the challenge
used `cat < -` command to print password to stdout

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext

---
### Bandit 02

#### Credentials
Username: bandit3
Password: UmHadQclWmgdLOKQ3YNgjWxGoRMb5luK

#### How you passed the challenge
used `cat < spaces\ in\ this\ filename` command to print password to stdout

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 03

#### Credentials
Username: bandit4
Password: pIwrPrtPN36QITSp3EQaw936yaFoFgAB

#### How you passed the challenge
used `cat < inhere/.hidden` command to print password to stdout

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 04

#### Credentials
Username: bandit5
Password: koReBOKuIDDepwhWk7jZC0RTdopnAYKh

#### How you passed the challenge
First, I used `for x in {0..9}; do file ./inhere/-file0$x; done` which told me -file07 was the only ASCII text file. Then, I simply ran `cat < inhere/-file07` command to print password to stdout

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 05

#### Credentials
Username: bandit6
Password: DXjZPULLxYr17uwoI01bNLQbtFemEgo7

#### How you passed the challenge
As suggested by overthewire.org, I used `find` command to search for a file with the required properties (`find -type f -size 1033c ! -executable`). The output from this command was `./inhere/maybehere07/.file2` where `cat < inhere/maybehere07/.file2` command printed password to stdout.

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 06

#### Credentials
Username: bandit7
Password: HKBPTKQnIay4Fw76bEy8PVxKEDQRKTzs

#### How you passed the challenge
As suggested by overthewire.org, I used `find` command to search for a file with the required properties while dumping errors to dev/null (`find / -user bandit7 -group bandit6 -size 33c 2>../../dev/null`). The output from this command was `/var/lib/dpkg/info/bandit7.password` where `cat < /var/lib/dpkg/info/bandit7.password` command printed password to stdout.

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 07

#### Credentials
Username: bandit8
Password: cvX2JJa4CFALtqS87jk27qwqGhBM9plV

#### How you passed the challenge
Used `cat data.txt | grep millionth` to find proper line in file with the password.

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 08

#### Credentials
Username: bandit9
Password: UsvVyFSfZZWbi6wgC7dAFyFuR6jQQUhR

#### How you passed the challenge
As suggested by overthewire.org, I used `uniq` and `sort` commands to find unqiue lines in file.

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 09

#### Credentials
Username: bandit10
Password: truKLdjsbJ5g7yyJ2X2R0o3a5HQJFuLk

#### How you passed the challenge
As suggested by overthewire.org, I used `strings` and `grep` commands to readable lines in file (`strings data.txt | grep =`). From the output, I identified the password.

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 10

#### Credentials
Username: bandit11
Password: IFukwKGsFW8MOq3IRFqrxE1hxTNEbUPR

#### How you passed the challenge
As suggested by overthewire.org, I used `base64` command to decode data in file (`base64 -d data.txt`).

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as with easy to break encoding.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 11

#### Credentials
Username: bandit12
Password: 5Te8Y4drgCRfCx8ugdwuEX8KFC6k2EUu

#### How you passed the challenge
As suggested by overthewire.org, I used `tr` command to decode data in file revealing the password (`cat data.txt | tr '[A-MN-Za-mn-z]' '[N-ZA-Mn-za-m]'`).

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as with easy to break encoding.

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext


---
### Bandit 12

#### Credentials
Username: bandit13
Password: 8ZjyCRiBWFYkneahHwxCv3wb2a1ORpYL

#### How you passed the challenge
After two decodings using `xxd` and a ridiculous number of decompressions using `gzip`, `bzip2`, and `tar` (over 10 decompressions), I was finally able to arrive at plaintext file with plaintext password.

#### What sins are evidenced in this challenge
password file does not have proper permissions set. Also, password is stored as plaintext even with so much compression and encoding.  

#### How could those sins be mitigated
change password file permissions to not be readable by group, and also don't store password as plaintext with commonly available compression and encoding techniques. 


---
### Bandit 13

#### Credentials
Username: bandit14
Password: 4wcYUJFw0k0XLShlDzztnTBHiqxU3b3e

#### How you passed the challenge
Performed `ssh -i sshkey.private bandit14@localhost` command to log in as bandit14 which allowed me to run `cat /etc/bandit_pass/bandit14` to obtain the password for bandit14.

#### What sins are evidenced in this challenge
Leaving private key for bandit14 accessible/usable to bandit 13. 

#### How could those sins be mitigated
don't share private keys with unauthorized users!


---
### Bandit 14

#### Credentials
Username: bandit15
Password: BfMYroe26WYalil77FoDi9qh59eK5xNr

#### How you passed the challenge
Used netcat and ran `nc localhost 30000` command which gave me a prompt where I pasted bandit14 password. My response was the password for bandit15.

#### What sins are evidenced in this challenge
Having password for bandit15 being returned over port 30000 with bandit14 password, unless it's intended. 

#### How could those sins be mitigated
Don't leak bandit15's password through port 30000 unless it's intended.


---
### Bandit 15

#### Credentials
Username: bandit16
Password: cluFn7wTiGryunymYOu4RcffSxQluehd

#### How you passed the challenge
Ran `openssl s_client -connect localhost:30001` command which gave me a prompt where I pasted bandit15 password. My response was the password for bandit16.

#### What sins are evidenced in this challenge
Having password for bandit16 being returned over port 30001 using ssl encryption with bandit15 password, unless it's intended. 

#### How could those sins be mitigated
Don't leak bandit16's encrypted password through port 30001 unless it's intended.


---
### Bandit 16

#### Credentials
Username: bandit17
Password: xLYVMN9WE5zQ5vHacb0sZEVqbrp7nBTn

#### How you passed the challenge
Ran `nmap -v -A -T4 -p 31000-32000 localhost` command to discover open ports and their associated services. *31790/tcp open  ssl/unknown* and *31518/tcp open  ssl/echo* were reported. I ran `openssl s_client -connect localhost:31790` command and got an RSA private key after pasting bandit16's password. After putting RSA private key in file, I connected to bandit17 using `ssh -i bandit17.key bandit17@localhost` where `cat /etc/bandit_pass/bandit17` command gave me the password. 

#### What sins are evidenced in this challenge
Having bandit17's private key being returned over port 31790 using ssl encryption with bandit16 password, unless it's intended. 

#### How could those sins be mitigated
Don't leak bandit17's encrypted private key through port 31790 unless it's intended.


---
### Bandit 17

#### Credentials
Username: bandit18
Password: kfBf3eYk5BPBRzwjqutbbfE887SVc5Yd

#### How you passed the challenge
Ran `diff passwords.new passwords.old ` command which provided password directly. 

#### What sins are evidenced in this challenge
Having bandit18's password in plaintext and readable. 

#### How could those sins be mitigated
Don't have bandit18's password stored as plaintext. 


---
### Bandit 18

#### Credentials
Username: bandit19
Password: IueksS7Ubh8G3DCwVzrTd8rAVOwq3M5x

#### How you passed the challenge
Ran `ssh bandit18@bandit.labs.overthewire.org -p 2220 "cat ~/readme"` command which printed the password directly to console. 

#### What sins are evidenced in this challenge
Having bandit18's password in plaintext and readable. 

#### How could those sins be mitigated
Don't have bandit18's password stored as plaintext. 


---
### Bandit 19

#### Credentials
Username: bandit20
Password: GbKksEFF4yrVs6il55v6gwY5aVje5f0j

#### How you passed the challenge
Ran `./bandit20-do cat /etc/bandit_pass/bandit20` command which printed the password directly to console. 

#### What sins are evidenced in this challenge
Arbitrarily leaving a program that can execute commands as another user. 

#### How could those sins be mitigated
Don't arbitrarily leave a program that can execute commands as another user. 



---
### Bandit 20

#### Credentials
Username: bandit21
Password: gE269g2h3mw3pwgrj0Ha9Uoqen1c9DGr

#### How you passed the challenge
Ran `echo "GbKksEFF4yrVs6il55v6gwY5aVje5f0j" | nc -l localhost -p 61666 &` and then `./suconnect 61666` commands which printed the password directly to console. 

#### What sins are evidenced in this challenge
Created a program that sent bandit21's password over given port if bandit20 password was supplied.

#### How could those sins be mitigated
Don't create a program that sends bandit21's password over given port if bandit20 password is supplied.


---
### Bandit 21

#### Credentials
Username: bandit22
Password: Yk7owGAcWjwMVRwrTesJEwB7WVOiILLI

#### How you passed the challenge
As prescribed from writeup, I ran `ls -la /etc/cron.d` and found *cronjob_bandit22*, *cronjob_bandit23*, and *cronjob_bandit24*. Ran `cat /etc/cron.d/cronjob_bandit22` and saw */usr/bin/cronjob_bandit22.sh* script being ran. Ran `cat /usr/bin/cronjob_bandit22.sh` and saw that the script was putting bandit22 password in */tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv* file. So I simply ran `cat /tmp/t7O6lds9S0RqQh9aMcz6ShpAoZKF7fgv` which outputted bandit22's password to stdout.

#### What sins are evidenced in this challenge
Having a script which put bandit22's password in file with chmod 644 permissions. 

#### How could those sins be mitigated
Don't have a script which puts bandit22's password in file with chmod 644 permissions unless that's what you want to happen...yeah.


---
### Bandit 22

#### Credentials
Username: bandit23
Password: jc1udXuA1tiHqjIsL8yaapX5XIAI6i0n

#### How you passed the challenge
Started off by running `cat /etc/cron.d/cronjob_bandit23` to see that */usr/bin/cronjob_bandit23.sh* script was being run. Ran `cat /usr/bin/cronjob_bandit23.sh` and discovered that the script placed the password of a specified user (specified in the script from *whoami*) in */tmp/$mytarget* where *$mytarget* is determined by `echo I am user $myname | md5sum | cut -d ' ' -f 1` command. *$myname* is specified  by `whoami` command. After studying the script, I realized I could run `echo I am user bandit23 | md5sum | cut -d ' ' -f 1` (replacing *$myname* with *bandit23*) in the console to get the md5 value for bandit23 which was *8ca319486bfbbc3663ea0fbe81326349*. Then I simply ran `cat /tmp/8ca319486bfbbc3663ea0fbe81326349` which printed the stored password to stdout.

#### What sins are evidenced in this challenge
Having a script which was able to specify that bandit23's password be put in plaintext file. 

#### How could those sins be mitigated
Don't have a script which is able to specify that bandit23's password be put in plaintext file.


---
### Bandit 23

#### Credentials
Username: bandit24
Password: UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ

#### How you passed the challenge
First, I created a working directory: */tmp/tom_bandit23*. Within that directory, I created shell script:

```js
#!/bin/bash
cat /etc/bandit_pass/bandit24 > /tmp/tom_bandit23/password
```

I also created */tmp/tom_bandit23/password* file and changed permissions to chmod 777. Finally, I ran `cp getBandit24PW.sh /var/spool/bandit24/` and waited for the *password* file to update. Once that occured, I ran `cat password` which printed bandit24's password to stdout.  

#### What sins are evidenced in this challenge
Having an accessible script which ran other scripts in protected directory. 

#### How could those sins be mitigated
Don't have an accessible script which runs other scripts in protected directory.


---
### Bandit 24

#### Credentials
Username: bandit25
Password: uNG9O58gUE7snukf3bvZ0rxhtnjzSGzG

#### How you passed the challenge
I simply ran `nc localhost 30002 <<< $(for i in {0000..9999};do echo “UoMYTrfrBFHyQXmg6gzctqAwOmw1IohZ $i”;done)` in command line and waited for output to finally give me the password.

#### What sins are evidenced in this challenge
Having small pin number that allowed brute force guessing attacks. 

#### How could those sins be mitigated
Increase bit size of pin number to a size easily brute forced. 



---
### Bandit 25

#### Credentials
Username: bandit26
Password: 5czgV9L3Xx8JPOyRbXh6lQbmIOWvPT6Z

#### How you passed the challenge
Ran `ssh -i bandit26.sshkey bandit26@localhost` which printed out text and ascii art before exiting. In order to find out which shell bandit26 was using, I ran `cat /etc/passwd | grep bandit26` which told me it was */usr/bin/showtext*. Next, I ran `cat /usr/bin/showtext` and discovered that *more ~text.txt* was being called in the script. After minimizing my console window to trigger *more*, I pressed v to open vim editor and entered `:e /etc/bandit_pass/bandit26` which gave me the password.

#### What sins are evidenced in this challenge
Calling *more* in shell script allowing another shell to be run. 

#### How could those sins be mitigated
Don't call *more* in shells scripts.



---
### Bandit 26

#### Credentials
Username: bandit27
Password: 3ba3118a22e93127a4ed485be72ef5ea

#### How you passed the challenge
Did all steps in bandit25 to get back to the vim editor using *more*. Now, instead of `:e /etc/bandit_pass/bandit26`, I entered `:set shell=/bin/bash` to reset our shell allowing access to bandit26 shell. Once that was accomplished, I found "bandit-do" file which allowed me to run a command as bandit27. Thus, I just ran `./bandit27-do cat /etc/bandit_pass/bandit27` to print bandit27's password to console.

#### What sins are evidenced in this challenge
Same as for Bandit 25.

#### How could those sins be mitigated
Same as for Bandit 25.



---
### Bandit 27

#### Credentials
Username: bandit28
Password: 0ef186ac70e04ea33b4c1853d2526fa2

#### How you passed the challenge
Created */tmp/tom27* directory and then cloned the git repository in there using `git clone ssh://bandit27-git@localhost/home/bandit27-git/repo`. Then I simply ran `cat README` to print out the password. 

#### What sins are evidenced in this challenge
Storing plaintext password for another user in a git repo file which inherits permissions of directory containing clone. 

#### How could those sins be mitigated
Don't ever store passwords as plaintext. 



---
### Bandit 28

#### Credentials
Username: bandit29
Password: bbc96594b4e001778eee9975372716b2

#### How you passed the challenge
Performed all the same steps from Bandit 27 to clone the repo. However, the README.md showed that the password had been replaced with x's. By using `git log`, I was able to see that the inital commit of the README.md contained the password. Thus, using `git log -p -1` which showed me the diff of the commit of the last entry in the log revealing the password. 

#### What sins are evidenced in this challenge
Not being careful to clean the logs of a git repo even though an attempt was made to protect the password

#### How could those sins be mitigated
Be careful of keeping log information when dealing with sensitive information like passwords. 



---
### Bandit 29

#### Credentials
Username: bandit30
Password: 5b90576bedb2cc04c86a9e924ce42faf

#### How you passed the challenge
Performed all the same steps from Bandit 28 to clone the repo. Read README.md and saw *no passwords in production!* which I took to mean that the password might be in a different branch of the repo. Ran `git branch -r` and saw a *dev* repo. Ran `git checkout dev` which switched me to dev branch. Running `git log` showed me where the initial commit of the README.md file. Ran `git log -p -1` which showed me the diff giving me the password. 

#### What sins are evidenced in this challenge
Not being careful to clean the logs of a git repo even though an attempt was made to protect the password

#### How could those sins be mitigated
Be careful of keeping log information when dealing with sensitive information like passwords. 



---
### Bandit 30

#### Credentials
Username: bandit31
Password: 47e603bb428404d265f59c42920d81e5

#### How you passed the challenge
Performed all the same steps from Bandit 29 to clone the repo and look at the log and branches. Unfortunately, the README.md was blank and there were no helpful log entries or other branches. Luckily, git allows tagging which tags specific points in the repo's history as being important. Running `git tag` showed one tag with the name *secret* where `git show secret` revealed that the tag contained the password for bandit31.

#### What sins are evidenced in this challenge
Not being careful to make sure sensitive information isn't stored in git tags. 

#### How could those sins be mitigated
Don't store sensitive information in git tags. 



---
### Bandit 31

#### Credentials
Username: bandit32
Password: 56a9bf19c63d650ce78e6ec0354ee45e

#### How you passed the challenge
Performed all the same steps from Bandit 30 to clone the repo. This time, the README.md instructed me to created a *key.txt* file with "May I come in?" as it's contents and push it to the master branch. After doing this, the server responded with the password. 

#### What sins are evidenced in this challenge
Not sure exactly b/c the repo gave explicit instructions on how to retrieve the password from the server. Doesn't feel like a mistake was made. 

#### How could those sins be mitigated
Nothing b/c I'm not sure if a sin was actually performed here. 



---
### Bandit 32

#### Credentials
Username: bandit33
Password: c9c3199ddf4121b10cf581a98d51caee

#### How you passed the challenge
After logging in, I was greeted with the "UPPERCASE SHELL" interactive shell script. I quickly discovered that all my shell commands were being converted to uppercase not allowing normal access to commands. Luckily, *$0* can be inputted to come out of interactive mode and stop the constant uppercase conversions. Running `whoami` command showed me that I was bandit33, so I simply ran `cat /etc/bandit_pass/bandit33` to give me the password.

#### What sins are evidenced in this challenge
Password was stored in plaintext.

#### How could those sins be mitigated
Don't store passwords or other sensitive information in plaintext. 


