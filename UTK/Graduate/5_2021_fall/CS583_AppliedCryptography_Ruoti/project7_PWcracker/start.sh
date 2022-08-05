wordlist="wordlist.txt"
shadow="shadow.txt"

# Apparently, this finished!!! I got 9 passwords finished, but 11 left. 
# ./JohnTheRipper/run/john --wordlist=$wordlist --rules:Jumbo --session:run1 $shadow &

#./JohnTheRipper/run/john --wordlist=$wordlist --rules:Jumbo --session:jumbo $shadow
#./JohnTheRipper/run/john --incremental:Lower -max-len=8 --session:iLower $shadow
#./JohnTheRipper/run/john --incremental:Alpha -max-len=8 --session:iAlpha $shadow

#./JohnTheRipper/run/john --wordlist=$wordlist --rules:hashcat -max-len=8 --format=md5crypt-long --session:rHashcat $shadow

./JohnTheRipper/run/john --incremental:Alnum -max-len=8 --format=md5crypt-long --session:iAlnum $shadow

