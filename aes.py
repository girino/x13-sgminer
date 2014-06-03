#!/opt/local/bin/python2.7
#
# Copyright 2014 Girino Vey!
#
# This file is released as is, under Girino's Anarchist License
# http://www.girino.org/license
#

import sys
import base64
from Crypto import Random
from Crypto.Cipher import AES

class AESCipher:
    def __init__(self, key):
        self.bs = 32
        if len(key) >= 32:
            self.key = key[:32]
        else:
            self.key = self._pad(key)
    def encrypt(self, raw):
        raw = self._pad(raw)
        iv = Random.new().read(AES.block_size)
        cipher = AES.new(self.key, AES.MODE_CBC, iv)
        return iv + cipher.encrypt(raw)
        #return base64.b64encode(iv + cipher.encrypt(raw))
    def decrypt(self, enc):
        enc = base64.b64decode(enc)
        iv = enc[:AES.block_size]
        cipher = AES.new(self.key, AES.MODE_CBC, iv)
        return self._unpad(cipher.decrypt(enc[AES.block_size:]))
    def _pad(self, s):
        return s + (self.bs - len(s) % self.bs) * chr(self.bs - len(s) % self.bs)
    def _unpad(self, s):
        return s[:-ord(s[len(s)-1:])]

def print_as_C_hex(bytes):
	print "const int FILE_AS_ARRAY_SIZE = " + str(len(bytes)) + ";"
	print "const char FILE_AS_ARRAY[] = {"
	count = 0
	for byte in bytes:
		print "0x%02X," % ord(byte),
		count += 1
		if (count % 8 == 0):
			print ""
	print "};"


file = sys.argv[-1];
if file:
	aes = AESCipher("<INSERT_YOUR_PASSWORD_HERE>");
	f = open(file, "r");
	cypher = aes.encrypt("".join(f.readlines()))
	print_as_C_hex(cypher);
