#!/usr/bin/python2.5
# The XOR values for Tumbler 1-5 is "NIAVU"
#   Tumbler1 XOR against N
#   Tumbler2 XOR against I
#   Tumbler3 XOR against A
#   etc...
#
#
#Python 3.1 (r31:73578, Jun 27 2009, 21:49:46) 
#[GCC 4.0.1 (Apple Inc. build 5493)] on darwin
#Type "help", "copyright", "credits" or "license" for more information.
#>>> from lockpick.py import *
#Traceback (most recent call last):
#  File "<stdin>", line 1, in <module>
#ImportError: No module named py
#>>> from lockpick import *
#>>> encrypt("Stephen")
#'\x161 5- +'
#>>> decrypt(encrypt("Stephen"))
#'Stephen'
#>>> 
import socket
import sys
import os
import time

def tumbler1(data):
    cipher = ""
    for byte in data:
        cipher+=chr(ord(byte) ^ ord('N'))   
    return cipher 

def tumbler2(data):
    cipher = ""
    for byte in data:
        cipher+=chr(ord(byte) ^ ord('I'))   
    return cipher 

def tumbler3(data):
    cipher = ""
    for byte in data:
        cipher+=chr(ord(byte) ^ ord('A'))   
    return cipher 

def tumbler4(data):
    cipher = ""
    for byte in data:
        cipher+=chr(ord(byte) ^ ord('V'))   
    return cipher 

def tumbler5(data):
    cipher = ""
    for byte in data:
        cipher+=chr(ord(byte) ^ ord('U'))   
    return cipher

def encrypt(data):
    return tumbler1(tumbler2(tumbler4(tumbler3(tumbler5(data)))))

def decrypt(data):
    return tumbler5(tumbler3(tumbler4(tumbler2(tumbler1(data))))) 

class Listener:
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.encrypted = False

    def listening_server(self):
        self.connectback_h = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.connectback_h.bind((self.ip, self.port))
        self.connectback_h.listen(5)
        while 1:
            (clientsocket, address) = self.connectback_h.accept() 
            self.hsend = clientsocket.send
            self.hrecv = clientsocket.recv
            print "Connect from ",  address, " ." 
            while 1:
                data = clientsocket.recv(1024)
                if self.encrypted:
                    print "received ciphertext: ", self.hexdump(data)
                    print "decrypted", self.hexdump(decrypt(data))
                    data = decrypt(data)
                if data.__contains__("startencrypt"):
                    self.encrypted = True
                    if self.encrypted:
                        clientsocket.send(encrypt("Encryption Started\nThe Key is: 'The time has come the walrus said.'"))
                if data.__contains__("endencrypt"):
                    clientsocket.send("\nDecryption Ended")
                if data.__contains__("initATDT\nAT&F1&D2"):
                    clientsocket.send("\n...oooOOO Welcome to Sa7ori Trojan OOOooo...\nuse 'startencrypt' to begin encrypted mode.")
                if data.__contains__("gettime"):
                    if self.encrypted:
                        clientsocket.send(encrypt(time.asctime()))
                    clientsocket.send(time.asctime())
                if data.__contains__("quit"):
                    if self.encrypted:
                        clientsocket.send(encrypt("Terminating Connection\nLaters..\n"))
                    clientsocket.send("quit")
                    clientsocket.close()
                print self.hexdump(data)
                if not data: break
        clientsocket.close() 

    def hexdump(self, src, length=16):
        N=0; result=''
        self.FILTER=''.join([(len(repr(chr(x)))==3) and chr(x) or '.' for x in range(256)])
        while src:
            s,src = src[:length],src[length:]
            hexa = ' '.join(["%02X"%ord(x) for x in s]) 
            s = s.translate(self.FILTER)
            result += "%08X:  %-*s  |%s|\n" % (N, length*3, hexa, s)
            N+=length
        return result

if __name__=="__main__":
    import optparse
    parser = optparse.OptionParser()
    parser.add_option(
        '-l','--local-ip',
        dest='local_ip',default='127.0.0.1',
        help='IP of interface to bind to')
    parser.add_option(
        '-p','--local-port',
        type='int',dest='local_port',default=80,
        help='Port to bind to')
    if len(sys.argv) == 1:
        sys.argv.append("--help")
    options, args = parser.parse_args()
    print "Listening on %s:%d..." % (options.local_ip, options.local_port)
    try:
        serv = Listener(options.local_ip, options.local_port)
        serv.listening_server() 
    except KeyboardInterrupt:
        print "Quitting..."
        sys.exit(0)

