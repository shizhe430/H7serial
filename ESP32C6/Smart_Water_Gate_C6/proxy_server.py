#!/usr/bin/env python3
"""Simple HTTP proxy: ESP32 calls this via HTTP, this forwards to DeepSeek via HTTPS."""
import http.server
import urllib.request
import json
import os
import sys

DEEPSEEK_KEY = os.environ.get("DeepSeek__ApiKey")
if not DEEPSEEK_KEY:
    raise RuntimeError("DeepSeek__ApiKey is not set")
PORT = 8080

class ProxyHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path.startswith('/ask?q='):
            from urllib.parse import unquote
            q = unquote(self.path[7:])
            data = json.dumps({
                "model": "deepseek-chat",
                "messages": [{"role": "user", "content": q}],
                "max_tokens": 200,
                "temperature": 0
            }).encode()
            req = urllib.request.Request(
                "https://api.deepseek.com/chat/completions",
                data=data,
                headers={
                    "Content-Type": "application/json",
                    "Authorization": f"Bearer {DEEPSEEK_KEY}"
                }
            )
            try:
                resp = urllib.request.urlopen(req, timeout=30)
                result = json.loads(resp.read())
                reply = result['choices'][0]['message']['content']
                js = json.dumps({"ok": True, "reply": reply})
                self.send_response(200)
                self.send_header('Content-Type', 'application/json')
                self.end_headers()
                self.wfile.write(js.encode())
            except Exception as e:
                self.send_response(500)
                self.end_headers()
                self.wfile.write(json.dumps({"ok": False, "error": str(e)}).encode())
        else:
            self.send_response(404)
            self.end_headers()
    def log_message(self, *a):
        pass  # suppress logs

print(f"Proxy server running on http://0.0.0.0:{PORT}")
print(f"ESP32 can use: http://YOUR_IP:{PORT}/ask?q=你好")
http.server.HTTPServer(("0.0.0.0", PORT), ProxyHandler).serve_forever()
