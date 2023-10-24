import sys, os
import numpy as np
from datetime import datetime
from PyQt5 import uic
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
import serial.tools.list_ports
from Controller import Controller

from_class = uic.loadUiType(os.getcwd() +\
                            "/segway.ui")[0]

class WindowClass(QMainWindow, from_class) :
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        
        # self.controller = Controller("/dev/ttyACM0", 9600, timeout=1)
        # self.controller.daemon = True

        # self.controller.update.connect(self.updateController)

        self.resetPortDrop()
        self.resetBaudDrop()
        
        # self.btnUp.pressed.connect(self.pressUp)
        # self.btnDown.pressed.connect(self.pressDown)
        # self.btnRight.pressed.connect(self.pressRight)
        # self.btnLeft.pressed.connect(self.pressLeft)
        self.btnConnect.pressed.connect(self.pressConnect)
        self.btnSend.pressed.connect(self.pressSend)

    def resetPortDrop(self):
        self.comboPort.clear()
        ports = serial.tools.list_ports.comports()

        available_ports = [p.device for p in ports]
        if len(available_ports) > 0:
            available_ports.sort()
            for p in available_ports:
                self.comboPort.addItem(p)

    def resetBaudDrop(self):
        self.comboBaudrate.clear()
        self.comboBaudrate.addItem("9600")
        self.comboBaudrate.addItem("115200")

    def updateController(self, data):
        print(data)
        self.textInfo.appendPlainText(data)

    def pressConnect(self):
        port_name = self.comboPort.currentText()
        baudrate = int(self.comboBaudrate.currentText())

        try:
            self.controller = Controller(port_name=port_name, baudrate=baudrate, timeout=(1/100))
            self.controller.daemon = True
            self.controller.received_data.connect(self.updateController)

            self.controller.start()

        except:
            return

    def pressSend(self):
        if self.controller.connection:
            text = self.lineEdit.text()
            self.controller.sendData(text)

        
        
    # def pressUp(self):
    #     return
    
    # def pressDown(self):
    #     return
    
    # def pressRight(self):
    #     return
    
    # def pressLeft(self):
    #     return
        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWindows = WindowClass()
    myWindows.show()
    sys.exit(app.exec_())