import requests
API_KEY = "D331F1DEB5EDAE0D7CC90FA90B98852E"
#r = requests.get("http://api.steampowered.com/ISteamNews/GetNewsForApp/v0002/?appid=440&count=3&maxlength=300&format=json")
#r = requests.get("http://api.steampowered.com/ISteamApps/GetAppList/v2/?key=" + API_KEY)
#print(r.status_code)
#data = r.json()

"""with open('SteamScraper/id_mappings.txt', 'w', encoding='utf-8') as f:
    for app in data['applist']['apps']:
        f.write(str(app['name']) + ', ' + str(app['appid']) + '\n')
"""
r = requests.get("http://api.steampowered.com/ISteamEconomy/GetAssetPrices/v0001/?key=" + API_KEY + "&appid=240")

data = r.status_code
print(data)

