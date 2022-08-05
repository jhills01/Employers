from bs4 import BeautifulSoup
import requests
import csv

# builds/returns array of line offset values for given "file" to use with file.seek() 
def fileLineOffsets(file):
    
    # Read in the file once and build a list of line offsets
    lineOffsets = [];
    offset = 0;
    for line in file:
        lineOffsets.append(offset);
        offset += len(line);
    file.seek(0);

    return lineOffsets;

    ## Now, to skip to line n (with the first line being line 0), just do
    #file.seek(line_offset[n])

# creates file containing seek offsets for every line in given "file"
def fileLineOffsets4Seek(f, fileOffsetName):
    # open given file and get offsets for all lines
    file = open(f, 'r', encoding='utf-8');
    lineOffsets = fileLineOffsets(file);
    file.close();

    # create file to contain all offsets
    file = open(fileOffsetName, 'w+', encoding='utf-8');
    for i in lineOffsets:
        file.write(f"{i}\n");
    file.close();

def getGenre(soup):
    genre = soup.find( "div", {"id" : 'genresAndManufacturer'} );
    
    if genre is None: return None;

    nextElementNeeded = False;
    for element in genre:
        if element.name is not None and nextElementNeeded:
            return element.text.strip();

        if element.name == "b" and element.text == "Genre:":
            #printElement(element);                
            nextElementNeeded = True;

def printElement(element):
    print(f"----------------------------------------------------------------------");
    print(f"type(element): {type(element)}");
    print(f"element.name: {element.name}");
    # if "element.name" is None, then it's NOT a tag element!!!
    if element.name is not None: print(f"element.text: {element.text}");
    print(f"    element: {element}");

# extracts data from steam game pages and stores it in csv file
# "scrapeList" contains game names and ids to be scraped
# "scrapeResults" references the csv file to store results
# "scrapeResume" is the file containing the "rowNumResume, lineNumResume"
# "rowNumResume" is the row number in the csv file to start writing  --- NOT NEEDED!! Just open in 'a+' mode
# "lineNumResume" is the line number in scrapeList file to start reading from
def steamScrape(scrapeList, scrapeResults, scrapeResume, lastGameID):
#def steamScrape(scrapeList, scrapeResults, scrapeResume, lineNumResume):
    
    # setup variables
    lineNum = 0;
    continueScrapping = False;

    for line in scrapeList:
        
        lineNum += 1;
        
        # test code
        # if lineNum == 201: return;

        # process line: separating game name from game id, and tracking file line number
        if line == '':
            continue
        elif line == '+':
            continue
        
        game_name = line.split(',')[0].strip();
        game_id = line.split(',')[1].strip();
        
        # resume code: 
        if game_id != lastGameID and not continueScrapping: 
            #print(f"line {lineNum}: {game_id} != {lastGameID} and not {continueScrapping}");
            continue;
        elif game_id == lastGameID: 
            #print(f"line {lineNum}: {game_id} == {lastGameID}");
            continueScrapping = True; continue;
        
        # 
        #https://store.steampowered.com/app/1690670
        URL = 'https://store.steampowered.com/app/' + game_id
        
        try:
            page = requests.get(URL)
        except: 
            print(f"REQUEST ERROR. Continuing....");
            continue;

        soup = BeautifulSoup(page.content, 'html.parser')
        
        ## TEST CODE
        ## getting 'none' as price even though I see discounted price on store page
        #if game_name == 'synthwaifu: neon space fighter':
        #    f = open("synthwaifu:_neon_space_fighter.html", "w+");
        #    f.write(str(soup));
        
        # GENRE ---------------------------------------------------------------------------- 
        genre = getGenre(soup);
        
        # PRICE ---------------------------------------------------------------------------- 
        # <div class="game_purchase_price price" data-price-final="1499">$14.99</div>
        # <div class="discount_original_price">$0.99</div>
        price_noDiscount = soup.find(class_='game_purchase_price price');
        price_discount = soup.find(class_='discount_original_price');
        if price_noDiscount is not None:
            price = price_noDiscount.text.strip();
            #print(f"    {game_name} ==> price_noDiscount: ${price}");
        elif price_discount is not None:
            price = price_discount.text.strip();
            #print(f"    {game_name} ==> price_discount: ${price}");
        else: price = "Bug: enter price manually"
        
        # TAGS ---------------------------------------------------------------------------- 
        tags = soup.find_all(class_='app_tag')
        if tags is None:
            print(f"    no tags");
            continue

        # perform final write to csv file
        for tag in tags:
            #if tag == '' or tag.text.strip() == '+':
            if tag == '':
                continue
            scrapeResults.writerow([lineNum, game_id, game_name, tag.text.strip(), genre, price])

        # print to show what was written to file
        print(f"line {lineNum}: {game_name}, {game_id}");

        # record last game ID that was written to csv
        lastGameID = game_id;
        scrapeResume.seek(0);
        scrapeResume.write(f"{lastGameID}");
        scrapeResume.truncate();


            
        ## at this point, we're ready for next line in file to be processed
        #lineNumResume += 1

        ## track latest line processed by overwriting previous lineNumResume value
        #scrapeResume.seek(0);
        #scrapeResume.write(f"{lineNumResume}");
        #scrapeResume.truncate();
