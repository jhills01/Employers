wordlist="wordlist.txt"
shadow="shadow.txt"

printf "SHOWING:\n"
./JohnTheRipper/run/john --show $shadow 
printf "\n\n"

printf "rJumbo:\n"
#./JohnTheRipper/run/john --status:rJumbo
#head -10 rJumbo.log
#tail -10 rJumbo.log
grep -i "c/s" rJumbo.log 
printf "\n"

printf "iLower:\n"
./JohnTheRipper/run/john --status:iLower
printf "\n"

printf "iAlpha:\n"
./JohnTheRipper/run/john --status:iAlpha
printf "\n"

#printf "rSingle:\n"
#./JohnTheRipper/run/john --status:rSingle
#printf "\n"

printf "iAlnum:\n"
./JohnTheRipper/run/john --status:iAlnum
printf "\n"

