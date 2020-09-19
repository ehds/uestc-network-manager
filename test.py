import requests
import time
import re
import json
import hmac
def get_challange():
    url = "http://aaa.uestc.edu.cn/cgi-bin/get_challenge"
    data = {
        "callback":"jQuery11240889396928485537_1600002013738",
        "username":"sdf@dx-uestc",
        "_":str(int(time.time())),
        "ip":"113.54.201.245"
    }
    data_url = '&'.join([k+"="+v for k,v in data.items()])
    url += '?'+data_url
    res = requests.get(url)
    res = re.search(r'\{.*?\}', res.content.decode()).group()
    return json.loads(res)

def login():
    import hashlib
    passwd = "value"
    a = "key"
    cc = hmac.new(a.encode(),passwd.encode(), digestmod=hashlib.md5)
    print(cc.hexdigest())
    
login()