import sys, os
import time

# libraries in subfolders
sys.path.append(os.path.realpath('.')); # "...realpath('.') returns path string for current directory

from steamScrape import *;

scrapeList = open('tom_filtered.txt', 'r', encoding='utf-8')
scrapeListLineOffsets = fileLineOffsets(scrapeList);

if len(sys.argv) != 2:
    print(f"Please use argument 'start' or 'resume'. Exiting...");
    exit();
elif sys.argv[1] == 'start':
    # 'w+' opens for reading/writing, overwrites file, and ptr @ beginning
    scrapeResults = open('scrapeResults.csv', 'w+') 
    scrapeResults = csv.writer(scrapeResults);
    scrapeResults.writerow(["lineNum", "game_id", "game_name", "tags", "genre", "price"])

    scrapeResume = open('resume.txt', 'w+'); 
    # gameID of the first item in "tom_filtered.txt"
    # REVIEW: Might want to consider reading first line from filtered to get ID
    #         ... just in case the file should change
    lastGameID = "216938"; 
    
    ## NOTE: see 'NOTE' below
    #lineNumResume = 0;
elif sys.argv[1] == 'resume':
    scrapeResults = open('scrapeResults.csv', 'a+') 
    scrapeResults = csv.writer(scrapeResults);
   
    # 'r+' opens for reading/writing, DOESN'T overwrite file, ptr @ beginning
    scrapeResume = open('resume.txt', 'r+'); 
    lastGameID = scrapeResume.readline().strip();
    print(f"RESUMING {lastGameID}:\n");
    # lastGameID = scrapeResume.readline();
    
    ## NOTE: --> OLD CODE. This doesn't work
    ##       I'm not 100% sure why, but I think it's b/c there are...
    ##       ... utf-9 encoded characters for game names. 
    ##       Regardless, my offsets for use with "seek" are not lining...
    ##       ... up properly. I think the best solution now is to just...
    ##       ... log the last written app_id and use that to skip entries...
    ##       ... until I find that app_id, and then continue scappring the next one
    #lineNumResume = scrapeResume.readline()
    #print(f"lineNumResume(after readline):{lineNumResume}");

    #if lineNumResume == '':
    #    lineNumResume = 0;
    #else: lineNumResume = int(lineNumResume);

    ## set file pointer to correct line number to start reading from.
    #print(f"lineNumResume: {lineNumResume}, offset: {scrapeListLineOffsets[lineNumResume]}\n")
    #scrapeList.seek( scrapeListLineOffsets[lineNumResume] )

else: 
    print(f"sys.argv[1] != '' or 'resume'. Please use correct arguments. Exiting...");
    exit();

# NOTE: do I multiprocess "steamScrape()"??
# If so: does multiprocess stuff in python lockout files properly while reading/writing them?
# NOTE: Doing multiprocess makes resuming VERY challenging

t0 = time.time();
# steamScrape(scrapeList, scrapeResults, scrapeResume, lineNumResume);
steamScrape(scrapeList, scrapeResults, scrapeResume, lastGameID);
t1 = time.time();

print(f"==============================\nTOTAL EXECUTION TIME: {t1 - t0}\n");

# scrapeResults.close()
scrapeList.close()