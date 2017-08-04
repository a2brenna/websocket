#!/usr/bin/env python

from SimpleWebSocketServer import SimpleWebSocketServer, WebSocket

class SimpleEcho(WebSocket):

    def handleMessage(self):
        # echo message back to client
        self.sendMessage(self.data)

    def handleConnected(self):
        print(self.address, 'connected')
        self.sendMessage("CONNECTED")

    def handleClose(self):
        print(self.address, 'closed')

server = SimpleWebSocketServer('', 10001, SimpleEcho)
server.serveforever()
