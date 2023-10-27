import requests
import time
import logging


logging.basicConfig(level=logging.DEBUG)

url = "http://192.168.1.132:80/"

with requests.session() as s:
    while True:
        start = time.perf_counter()
        data = s.get(url+"H")
        # res = requests.put(url, {"move": "L"})
        print(data.text)
        # res = requests.put(url, {"move": "H"})
        end = time.perf_counter()- start
        print(end)