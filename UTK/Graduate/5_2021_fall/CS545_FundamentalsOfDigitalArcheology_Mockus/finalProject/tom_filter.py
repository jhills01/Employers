#Filter for game titles to remove NSFW content
def filter_titles(fn):
    FILTER_LIST = ['18+', 'sex', 'fuck', 'hentai', 'dick', 'shit', 'cock', 'cum']
    with open(fn, 'r', encoding='utf-8') as f:
        data = f.readlines()
    
    for i in range(len(data)):
        data[i] = data[i].lower()
    
    l = []
    for d in data:
        i = 1
        for f in FILTER_LIST:
            # if bad word found, we break and move on to next data item
            if f in d:
                break
            # if we made it to last bad word and haven't broke free of loop, then data item doesn't contain bad words
            if i == len(FILTER_LIST):
                l.append([d]) 
            i += 1

    with open('tom_filtered.txt', 'w', encoding='utf-8') as f:
        for j in l:
            f.write(j[0])


# run it!
filter_titles("../id_mappings.txt")