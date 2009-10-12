#!/usr/bin/env python2.5
import random
import base64
from binascii import hexlify
import sys

def genasckey(length):
    randstr = ""
    while len(randstr) < length:
        randstr+=chr(random.randint(0x21, 0x7e))
    return randstr 

def genkey(length):
    randstr = ""
    while len(randstr) < length:
        randstr+=chr(random.randint(0x01, 0x7f))
    return randstr 

def sa7crypt(data, key):
    x = 0
    box = range(256)
    for i in range(256):
        x = (x + box[i] + ord(key[i % len(key)])) % 256
        box[i], box[x] = box[x], box[i]
    x = 0
    y = 0
    out = []
    for char in data:
        x = (x + 1) % 256
        y = (y + box[x]) % 256
        box[x], box[y] = box[y], box[x]
        out.append(chr(ord(char) ^ box[(box[x] + box[y]) % 256]))
    return ''.join(out)

def hexstr(thing):
    tmp_buf = ""
    nullfree = True
    for byte in thing:
        if byte == "\x00":
            nullfree = False
        tmp_buf+='\\'+'x'+hexlify(byte)
    if not nullfree:
        print "Sorry, the string: ",tmp_buf," was not nullfree, you will need another one."
        sys.exit(1)
    return tmp_buf

def printasm(ciphertext, key):
    c_emit = ""
    k_emit = ""
    for char in ciphertext:
        c_emit += "_emit "+hex(ord(char))+"\n"
    c_emit += "_emit 0x00" #null terminate.
    for char in key:
        k_emit += "_emit "+hex(ord(char))+"\n"
    k_emit += "_emit 0x00" #null terminate.
    inline_asm = """
void *ct;
void *key;
__asm {
lea eax, ciphertext
lea ebx, thekey
jmp done
ciphertext:
"""+c_emit+"""
thekey:
"""+k_emit+"""
done:
mov ct, eax
mov key, ebx
}
printf("%s\\n",ct);
printf("%s\\n",key);
"""
    return inline_asm

px = raw_input("Enter the plaintext> ")
if len(px) >= 255:
    print "Your plaintext needs to be less than 255 characters."
key = genkey(len(px))
cipher = sa7crypt(px, key)
print "The resultant ciphertext is: ", hexstr(cipher)
print "The randomly generated key is: ", hexstr(key)
print "The base64'd ciphertext is: ", base64.b64encode(cipher)
print "The inline asm stub you can use is:\n", printasm(cipher, key)
#print repr(sa7crypt(cipher, "Stephen"))

