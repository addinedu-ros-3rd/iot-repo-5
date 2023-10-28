import struct
import time
import socket
import sys
from _thread import *

HOST = "192.168.4.1"
PORT = 80

data = []

client_socket = socket.socket()
client_socket.connect((HOST, PORT))

def rev_data(client_socket):
    while True:
        receive_data = client_socket.recv(1024)
        data.extend(receive_data)
        
start_new_thread(rev_data, (client_socket, ))

while True:
   if len(data) != 0:
        print(bytes(data).decode('utf-8'))
        data = []
    # client_socket.send
    # time.sleep(0.1)

client_socket.close()