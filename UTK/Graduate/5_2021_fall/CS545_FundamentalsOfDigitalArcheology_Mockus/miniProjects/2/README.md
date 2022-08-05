# Data Discovery Project


1. Your topic is Pick a favorite topic is the
   course project you are working on  (or you can pick something
   else that you care about).
2. Find 20 datasets that related to that topic (use, for example,
   https://toolbox.google.com/datasetsearch). I for one, collect open
   source git repositories, so I searched for "git urls"
3. For each of the 20 datasets you chose determine if the underlying data can be accessed (some of these datasets do not provide public access)
4. Add records to a mongodb collection MP2 within the database fdac21
   where you store metadata for each of the 20 datasets:
   - YourTopic,
   - dataset title,
   - dataset license,
   - dataset description,
   - url(s) were the data may be retrieved
   
```
import pymongo, json
client = pymongo.MongoClient (host="da1.eecs.utk.edu")
db = client ['fdac21']
coll = db ['MP2']
# for each dataset
coll.insert_one ( { 'owner':'YourNetID', 'topic':'YourTopic', 'title': 'Data title', 'license': 'license', 'description': 'Brief data description', 'urls': [ 'url1', 'url2', ... ] } )
```


To check what is recorded:
```
import pprint
import pymongo, json
client = pymongo.MongoClient (host="da1.eecs.utk.edu")
db = client ['fdac21']
coll = db ['MP2']
pp = pprint.PrettyPrinter(indent=1,width=65)
for r in coll. find({'owner':'YourNetID'}):
  print(pp .pformat (r))  
```
