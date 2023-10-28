from PyQt5.QtCore import *
import requests
import time
import socket

class Client(QThread):
    received_data = pyqtSignal(str)

    def __init__(self, addr: str, port: int):
        super().__init__()

        self.running = False

        self.tic = time.perf_counter()
        
        self.socket = socket.socket()
        self.recv_data = []
        self.sendData = "p"
        try:
            self.socket.connect((addr, port))
            self.running = True
        except:
            raise

        self.timer = QTimer(self)
        self.timer.start(100)
        self.timer.timeout.connect(self.readNSend)

        
    def readNSend(self):
        if self.running:
            receive_data = self.socket.recv(1024)
            self.recv_data.extend(receive_data)

            if len(self.recv_data) != 0:
                print(bytes(self.recv_data).decode('utf-8'))
                self.recv_data = []
                # client_socket.send  
            
            s = "MOVE " + self.sendData + ",USER AA" + "\n"
            self.socket.send(s.encode())
            
            self.sendData = "p"

            toc = time.perf_counter()
            self.tic = toc
    
    def stop(self):
        self.running = False
        self.session.close()
        # self.exec_()

