from PyQt5.QtCore import *
import time
import socket
from datetime import datetime

from DBConnector import DBConnector

class Client(QThread):
    rfid_user = pyqtSignal(str)

    def __init__(self, addr: str, port: int):
        super().__init__()

        self.running = False

        self.tic = time.perf_counter()
        
        self.socket = socket.socket()
        self.recv_data = []
        self.moveType = "p"
        self.userName = "None"

        self.dbconn = DBConnector()

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
                data = bytes(self.recv_data).decode('utf-8')
                self.splitInput(data)
                self.recv_data = []
            
            s = "MOVE " + self.moveType
            if self.userName != "None":
                s += ",USER " + self.userName
            s += "\n"
            self.socket.send(s.encode())
            
            self.moveType = "p"


            toc = time.perf_counter()
            print("Time interval : ", self.tic-toc)
            self.tic = toc

    def splitInput(self, data):
        timestamp = datetime.strftime(datetime.now(), "%Y-%m-%d %H:%M:%S")

        try:
            moveLine, rfidLine, mpuLine = data.split(',')
            
            moveOper, moveType = moveLine.split(' ')
            rfidOper, rfidUID = rfidLine.split(' ')
            mpuOper, mpuValue = mpuLine.split('/')
        except:
            print("Wrong input from ESP32")

        if moveOper == "MOVE" and moveType not in ["w", "a", "s", "d"]:
            self.dbconn.insert_to_motor(timestamp, moveType)

        if rfidOper == "RFID" and rfidUID != "None":
            self.dbconn.insert_to_rfid_tag_log(timestamp, rfidUID)
            # RFID 조회해서 emit 하는 코드
        
        if mpuOper == "MPU" and mpuValue != "0/0/0/0/0/0":
            try:
                acx, acy, acz, gyx, gyy, gyz = mpuValue.split("/")
                self.dbconn.insert_to_mpu6050(timestamp, acx, acy, acz, gyx, gyy, gyz)
            except:
                print("MPU Value Wrong")

    
    def stop(self):
        self.running = False
        self.session.close()
        # self.exec_()

