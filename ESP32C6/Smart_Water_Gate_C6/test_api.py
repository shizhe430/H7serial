import urllib.request
import json
import os

deepseek_key = os.environ.get("DeepSeek__ApiKey")
if not deepseek_key:
    raise RuntimeError("DeepSeek__ApiKey is not set")

data = {
    "model": "deepseek-chat",
    "messages": [
        {"role": "user", "content": "你好，回复一句话即可"}
    ],
    "max_tokens": 50
}
req = urllib.request.Request(
    "https://api.deepseek.com/chat/completions",
    data=json.dumps(data).encode(),
    headers={
        "Content-Type": "application/json",
        "Authorization": f"Bearer {deepseek_key}"
    }
)
try:
    resp = urllib.request.urlopen(req, timeout=15)
    result = json.loads(resp.read())
    print("状态码:", resp.status)
    print("回复:", result['choices'][0]['message']['content'])
except Exception as e:
    print("失败:", e)
