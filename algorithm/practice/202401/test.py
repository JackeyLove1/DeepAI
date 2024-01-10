import time
import uuid
import requests
import json
import subprocess
import re
chat_id = "oc_691aa46c9c59f6c2ec37c5d5fd790c68"
app_id = "cli_a51484a160f9d00d"
app_secret = "mBi87qOsHjuFp5XUtd2JjdCeiUf8zoBr"


def generate_token() -> str:
    url = "https://open.feishu.cn/open-apis/auth/v3/tenant_access_token/internal"
    headers = {
        'Content-Type': 'application/json'
    }
    data = {
        "app_id": app_id,
        "app_secret": app_secret
    }
    resp = requests.post(url, headers=headers, data=json.dumps(data))
    resp.raise_for_status()
    token = resp.json().get('tenant_access_token')
    return token


def lark_alarm(content: str):
    random_uuid = str(uuid.uuid4())
    hook = "https://fsopen.bytedance.net/open-apis/im/v1/messages?receive_id_type=chat_id"
    token = generate_token()
    headers = {
        'Content-Type': 'application/json',
        'Authorization': 'Bearer {}'.format(token),
    }
    data = {
        "content": json.dumps({"text": content}),
        "msg_type": "text",
        "receive_id": chat_id,
        "uuid": random_uuid
    }
    resp = requests.post(hook, headers=headers, data=json.dumps(data))
    print(resp.text)
    return resp.status_code


import socket


def get_local_ipv6():
    output = subprocess.check_output(['ip', 'addr'])
    match = re.search('inet6 ([\w:]+)/\d+ scope global', output.decode())
    ipv6_address = match.group(1)
    return ipv6_address


print(time.time())
