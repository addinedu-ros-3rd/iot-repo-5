from PyQt5.QtCore import *
import serial

class Logger(QThread):
    received_data = pyqtSignal(str)

    def __init__(self, port_name, baudrate, timeout):
        super().__init__()
        self.ser = serial.Serial(port_name, baudrate=baudrate, timeout=timeout)

        self.connection = False
        self.count = 0
        
        if not self.ser.is_open:
            print("connection failed")
            raise

        else:
            print("connect success")
            print(self.ser)
            self.connection = True
        

    def run(self):
        while self.connection:
            if self.ser.readable():
                try:
                    res = self.ser.readline().strip()
                    data = res.decode()
                    
                    if data != "":
                        self.received_data.emit(data)

                except ValueError:
                    print("Value Error")
                except serial.SerialException:
                    print("disconnected")
                    self.ser.close()
                    self.connection = False
                except UnicodeDecodeError:
                    print("Unicode Decode Error")

    def sendData(self, data):
        if self.connection:
            data += "\n"
            self.ser.write(data.encode())

    def stop(self):
        self.connection = False