import requests
import json

SERVER_URL = 'http://<your_server_url>'

if __name__ == '__main__':
    with open('packet_send_proxies.json', 'r') as f:
        proxies_list = json.loads(f.read())
        f.close()

    proxies = [{
        'http' : f'http://{proxy_string}',
        'https' : f'http://{proxy_string}',
    } for proxy_string in proxies_list]

    for proxy_dict in [{} + proxies]:
        r = requests.get(
            url = SERVER_URL,
            proxies=proxy_dict
        )
        print(r.text, r.status_code)