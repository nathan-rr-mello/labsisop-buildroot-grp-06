import http.server as HTTPServer
import socketserver
import os.path
import sys

class MyRequestHandler(HTTPServer.SimpleHTTPRequestHandler): 
    def end_headers (self):
        self.send_header('Access-Control-Allow-Origin', '*')
        HTTPServer.SimpleHTTPRequestHandler.end_headers(self)
        
    def do_GET(self):
        self.path = '/index.html'
        return HTTPServer.SimpleHTTPRequestHandler.do_GET(self)
    
Handler = MyRequestHandler

server = socketserver.TCPServer(('0.0.0.0', 8080), Handler) 
server.serve_forever()