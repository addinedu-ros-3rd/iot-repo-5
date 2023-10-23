from PyQt5.QtCore import *
import serial
import time

class Controller(QThread):
    received_data = pyqtSignal(str)

    def __init__(self, port, baudrate):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.connection = False
        self.exitThread = False
        
        while True:
            try:
                self.ser = serial.Serial(self.port, baudrate=self.baudrate)
            
            except serial.SerialException:
                continue
            else:
                print("connect", self.ser)
                self.connection = True
                break
        

    def run(self):
        count = 0
        while self.connection == True:
            try:
                res = self.ser.readline()
                data = res.decode("utf-8")
                
                self.received_data.emit(data)
                
            except ValueError:
                print("Value Error")
            except serial.SerialException:
                print("disconnected")
                self.ser.close()
                self.connection = False
            except UnicodeDecodeError:
                print("Unicode Decode Error")

    def stop(self):
        self.connection = False
