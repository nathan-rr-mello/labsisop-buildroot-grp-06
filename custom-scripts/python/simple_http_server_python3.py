import os
from http.server import BaseHTTPRequestHandler,HTTPServer


HOST_NAME = '0.0.0.0' # !!!REMEMBER TO CHANGE THIS!!!
PORT_NUMBER = 8000

def get_proc_list(array: [str]):
    res = []
    for line in array:
        filtered = line.strip().split()
        res.append((filtered[0], filtered[11]))
    return res

class MyHandler(BaseHTTPRequestHandler):
    def do_HEAD(s):
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
    def do_GET(s):
        """Respond to a GET request."""
        s.send_response(200)
        s.send_header("Content-type", "text/html")
        s.end_headers()
        #print(topInfo[4:])
        
        date = os.popen('date').read()
        uptime = os.popen("uptime -p").read()
        cpuspeed = os.popen("cat /proc/cpuinfo | grep -m1 -e MHz").read().split(":")[1].strip()
        cpumodel = os.popen("cat /proc/cpuinfo | grep -m1 -e 'model name'").read().split(":")[1].strip()
        topInfo = os.popen("top -n 1 -b").read().splitlines()
        cpuusage = topInfo[2].split(":")[1].strip()
        memusage = topInfo[3].split(":")[1].strip()
        procs = get_proc_list(topInfo[7:])

        

        s.wfile.write("<html><head><title>Title goes here.</title></head>".encode())
        s.wfile.write(f"<body><p>Current time: {date}</p>".encode())
        s.wfile.write(f"<p>Uptime: {uptime}".encode())
        s.wfile.write(f"<p>CPU Model: {cpumodel}</p>".encode())
        s.wfile.write(f"<p>CPU Speed (MHz): {cpuspeed}</p>".encode())
        s.wfile.write(f"<p>CPU Usage (%): {cpuusage}</p>".encode())
        s.wfile.write(f"<p>Memory Usage (MiB): {memusage}</p>".encode())
        s.wfile.write("<p>Processes List:</p>".encode())
        s.wfile.write("<ul>".encode())

        for proc in procs:
            s.wfile.write(f"<li>PID: {proc[0]} NAME: {proc[1]}</li>".encode())

        s.wfile.write("</ul>".encode())
        s.wfile.write("</body></html>".encode())

if __name__ == '__main__':
    httpd = HTTPServer((HOST_NAME, PORT_NUMBER), MyHandler)
    print("Server Starts - %s:%s" % (HOST_NAME, PORT_NUMBER))
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print("Server Stops - %s:%s" % (HOST_NAME, PORT_NUMBER))

