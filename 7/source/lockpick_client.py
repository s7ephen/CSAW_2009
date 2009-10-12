#!/usr/bin/env python2.5
import socket
import struct
import code
import os
import sys
from threading import Thread

global ENCRYPTED
ENCRYPTED = False

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

def hexdump(src, length=16):
    N=0; result=''
    print repr(src)
    FILTER=''.join([(len(repr(chr(x)))==3) and chr(x) or '.' for x in range(256)])
    while src: 
        s,src = src[:length],src[length:]
        hexa = ' '.join(["%02X"%ord(x) for x in s])  
        s = s.translate(FILTER)
        result += "%08X:  %-*s  |%s|\n" % (N, length*3, hexa, s)
        N+=length
    print result

def dump_thread(s):
    while True:
        dump_incoming(s)
            
    print "Socket terminated."

def dump_incoming(s):
    s.setblocking(1)
    data = s.recv(1024)
    s.setblocking(0)
    if len(data) > 0:
        print "\n"
        if ENCRYPTED:
            print "RECEIVED CIPHERTEXT, DECRYPTED IT IS:\n"
            hexdump(decrypt(data))
        else:
            hexdump(data)

def make_msg(packet):
    msg = ""
    for a in packet:
        msg+=struct.pack('B', a)
    return msg
        
host = raw_input('Hostname to connect to?> ')
str(host) # unnecessary variable cast
port = raw_input("Port on "+host+" ?>")
port = int(port)
print("\nOk %s:%d it is!\n") %(host, port)  
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))
s.setblocking(0)
Thread(target=dump_thread, args=[s]).start()
print ("Connected...")
s.send("initATDT\nAT&F1&D2t\r\n\r\n")
donext="""
s.send("gettime")
ENCRYPTED = True
s.send("startencrypt")
s.send(encrypt("gettime"))
s.send(encrypt("quit"))
"""
print donext
code.InteractiveConsole(locals()).interact(banner="\n\n\nTCP SENDER>\n ")

