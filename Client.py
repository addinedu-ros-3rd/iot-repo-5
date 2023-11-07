from PyQt5.QtCore import QThread, pyqtSignal, QTimer
import time
import socket
from datetime import datetime

from DBConnector import DBConnector

class Client(QThread):
    rfid_user = pyqtSignal(str)

    def __init__(self, addr: str, port: int):
        super().__init__()

        self.running = False

        
        self.socket = socket.socket()
        self.recv_data = []
        self.moveType = "p"
        self.userName = "None"

        self.dbconn = DBConnector()
        self.dbconn.connect()
        # self.dbconn.rfid_user.connect(self.emitUser)

        try:
            self.socket.connect((addr, port))
            self.running = True
            print("Connect Success")
        except:
            print("Connect Failed")
            raise

        self.timer = QTimer(self)
        self.timer.start(100)
        self.timer.timeout.connect(self.readNSend)

        
    def readNSend(self):
        if self.running:
            tic = time.perf_counter()
            receive_data = self.socket.recv(64)
            data = bytes(receive_data).decode('utf-8').strip("\r\n")

            self.recv_data.append(data)
            
            try:
                self.splitInput(data)
            except: 
                self.recv_data = []

            self.recv_data = []
            

            s = "MOVE " + self.moveType
            s += ",USER " + self.userName
            s += "\n"
            self.socket.send(s.encode())
            
            self.moveType = "p"
            self.userName = "None"

            toc = time.perf_counter()
            print("Time interval : ", toc - tic)
            

    def splitInput(self, data):
        timestamp = datetime.strftime(datetime.now(), "%Y-%m-%d %H:%M:%S")
        print(data)
        try:
            moveLine, rfidLine, mpuLine = data.split(',')
            
            # print(moveLine)
            # print(rfidLine)
            # print(mpuLine)
            moveOper, moveType = moveLine.split()
            rfidOper, rfidUID = rfidLine.split()
            mpuOper, mpuValue = mpuLine.split()
        except:
            print("Wrong input from ESP32")
            print("data", data, "\n----------------")
            raise

        if (moveOper == "MOVE") and (moveType in ["w", "a", "s", "d"]):
            self.dbconn.insert_to_motor(timestamp, moveType)

        if rfidOper == "RFID" and rfidUID != "None":
            self.dbconn.insert_to_rfid_tag_log(timestamp, rfidUID)
            self.rfid_user.emit(self.dbconn.fetch_uid_from_user(rfidUID))
        
        if mpuOper == "MPU" and mpuValue != "0/0/0/0/0/0" and mpuValue != "-1/-1/-1/-1/-1/-1":
            try:
                acx, acy, acz, gyx, gyy, gyz = mpuValue.split("/")
                self.dbconn.insert_to_mpu6050(timestamp, acx, acy, acz, gyx, gyy, gyz)
            except:
                print("MPU Value Wrong")
    
    def stop(self):
        self.running = False
        self.session.close()
        # self.exec_()