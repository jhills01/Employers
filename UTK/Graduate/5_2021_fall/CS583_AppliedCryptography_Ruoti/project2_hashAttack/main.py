from definitions import *;
import numpy;

# cd '/mnt/f/Computer_Science/Back2School/UTK/GraduateSchool/_classes/5_2021_fall/CS583_AppliedCryptography_Ruoti/projects/project2'

# determines how many times we perform attack and track attempts for a particular bit size
sampleSize = 50; 
# array of all hash bit sizes to be attacked
bitSizes = [8, 10, 12, 14, 16, 18, 20, 22];

# size is unknown since we don't know when our attack will be successful. KEY == hash, VALUE == input string that created hash
collisionAttack_allHashes = {}; 

# Stores all 
allAttemptsPerBitSize = {
                8: [0] * sampleSize, 
               10: [0] * sampleSize, 
               12: [0] * sampleSize, 
               14: [0] * sampleSize, 
               16: [0] * sampleSize, 
               18: [0] * sampleSize, 
               20: [0] * sampleSize, 
               22: [0] * sampleSize
           };

# COLLISION ATTACKS -----------------------------------------------------------------------------------------------------------------------------------
for bs in bitSizes:
    
    # set initial values to prepare for sample collection of next bit size
    attempts = 0;
    
    currentString = "a"; # here is the only time we initialize "currentString" to starting string of sequence
    hash = sha256Wrapper(currentString, bs);
    
    collisionAttack_allHashes.clear();
    collisionAttack_allHashes[hash] = currentString; # if "hash" isn't in dictionary, it's added automatically. If it is already there, value is overwritten.

    # gather samples for a particular bit size
    # IMPORTANT NOTE: we can't reset "currentString" to the initial value, "a", when gathering next sample. Otherwise, we'll run into the same string pairs over and over again.  
    for j in range(sampleSize):

        # loop until last created hash is found to have a duplicate in "samples"
        while ( True ):

            # get next string in sequence and associated hash for another collision attempt
            currentString = stringSequenceGenerator();
            hash = sha256Wrapper(currentString, bs);
            attempts += 1; # python doens't support "++" operator

            if (hash in collisionAttack_allHashes):
                #  do something before breaking while loop
                #print(f"SAMPLE {j + 1}: attempts = {attempts} ==> " + "Collsion found for hash: " + hash + ". Strings: " + currentString + ", " + collisionAttack_allHashes[hash]);
                allAttemptsPerBitSize[bs][j] = attempts;
                attempts = 0;
                collisionAttack_allHashes.clear();
                break;
            else: 
                collisionAttack_allHashes[hash] = currentString;

printDictionary(allAttemptsPerBitSize);
# print median values
for key, value in allAttemptsPerBitSize.items():
    median = numpy.median(value);
    print(key, " : ", median);

# PRE-IMAGE ATTACKS -----------------------------------------------------------------------------------------------------------------------------------
# NOTE: Using "allAttemptsPerBitSize.clear()" doesn't work b/c it empties the object. Ooops
allAttemptsPerBitSize = {
                8: [0] * sampleSize, 
               10: [0] * sampleSize, 
               12: [0] * sampleSize, 
               14: [0] * sampleSize, 
               16: [0] * sampleSize, 
               18: [0] * sampleSize, 
               20: [0] * sampleSize, 
               22: [0] * sampleSize
           };

preImageString = "a"; # purposely choose string that cannot match any string in generated sequence since sequence never produces string with numerical characters

for bs in bitSizes:
    
    # get pre-image hash using same pre-image string, but different bit size to match iteration
    preImageHash = sha256Wrapper(preImageString, bs);

    # set initial values to prepare for sample collection of next bit size
    attempts = 0;
    
    # gather samples for a particular bit size
    for j in range(sampleSize):

        # loop until last created hash is found to have a duplicate in "samples"
        while ( True ):

            # get next string in sequence and associated hash for another collision attempt
            currentString = stringSequenceGenerator();
            hash = sha256Wrapper(currentString, bs);
            attempts += 1; # python doens't support "++" operator

            #if (hash in collisionAttack_allHashes):
            if (hash == preImageHash):
                #  do something before breaking while loop
                #print(f"SAMPLE {j + 1}: attempts = {attempts} ==> " + "Collsion found for hash: " + hash + ". Strings: " + currentString + ", " + collisionAttack_allHashes[hash]);
                allAttemptsPerBitSize[bs][j] = attempts;
                attempts = 0;
                break;

printDictionary(allAttemptsPerBitSize);

# print median values
print("\nPRE-IMAGE ATTACK");
for key, value in allAttemptsPerBitSize.items():
    median = numpy.median(value);
    print(key, " : ", median);
