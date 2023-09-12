import http.server
import socketserver
import os
import sys
import re

class MyRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_HEAD(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

        self.wfile.write("<html><head><title>TP1 Lab Sisop</title></head>".encode())
        self.wfile.write("<body><h2>Target info:</h2>".encode())

        datetime = os.popen('date +%a" "%D" "%T').read()
        self.wfile.write(f"<p> - DateTime: {datetime}</p>".encode())

        uptime = os.popen("uptime | awk -F ',' '{print $1}' | sed 's/.*up/Up/'").read()
        self.wfile.write(f"<p> - Uptime: {uptime}</p>".encode())

        model = os.popen("grep -m1 'model name' /proc/cpuinfo | cut -d' ' -f3-").read()
        self.wfile.write(f"<p> - CPU model: {model}</p>".encode())

        vel = os.popen("grep -m1 'MHz' /proc/cpuinfo | awk '{print $4 \" MHz\"}'").read()
        self.wfile.write(f"<p> - CPU Frequency: {vel}</p>".encode())

        cpuUsage = os.popen("top -bn1 | grep CPU: | awk '{printf \"%.2f %\", $(NF-2)}' | awk -F '%' '{print $1 \" %\"}'").read()
        self.wfile.write(f"<p> - CPU usage: {cpuUsage}</p>".encode())

        ram = os.popen("free -h | grep \"Mem:\" | awk '{print $2}'").read()
        self.wfile.write(f"<p> - Total RAM: {ram}</p>".encode())

        ramUsage = os.popen("free -h | grep \"Mem:\" | awk '{print $3}'").read()
        self.wfile.write(f"<p> - RAM usage: {ramUsage}</p>".encode())

        sys = os.popen("cat /etc/os-release | grep \"PRETTY_NAME=\" | awk -F '\"' '{print $2}'").read()
        self.wfile.write(f"<p> - System version: {sys}</p>".encode())

        procs = os.popen("ps -o pid,comm").read()
        procs_lines = procs.strip().split('\n')
        html_table = "<table><tr><th>PID</th><th>Command</th></tr>"
        for line in procs_lines[1:]:  # Ignorar o cabeçalho da saída do comando ps
            matches = re.split(r'\s+', line.strip())
            pid = matches[0]
            process_name = " ".join(matches[1:])
            html_table += f"<tr><td>{pid}</td><td>{process_name}</td></tr>"
        html_table += "</table>"
        self.wfile.write(f"<p> - Processes list:</p>{html_table}".encode())
        
        self.wfile.write("</body></html>".encode())

Handler = MyRequestHandler
with socketserver.TCPServer(('0.0.0.0', 8080), Handler) as server:
    print("Server TP1 OK")
    server.serve_forever()