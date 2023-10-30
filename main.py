import sys, os

from PyQt5 import uic
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

# from pyqtgraph import PlotWidget, plot
# import pyqtgraph as pg

from Client import Client


from_class = uic.loadUiType(os.getcwd() +\
                            "/segway.ui")[0]

class WindowClass(QMainWindow, from_class) :
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        try:
            self.client = Client("192.168.4.1", 80)
            print("Connect Success")
        except :
            print("Connect Failed")
            sys.exit()

        # self.client.rfid_user.connect(self.emitUser)  


        self.btnUp.pressed.connect(self.pressUp)
        self.btnDown.pressed.connect(self.pressDown)
        self.btnRight.pressed.connect(self.pressRight)
        self.btnLeft.pressed.connect(self.pressLeft)

        self.btnUp.released.connect(self.releaseBtn)
        self.btnDown.released.connect(self.releaseBtn)
        self.btnRight.released.connect(self.releaseBtn)
        self.btnLeft.released.connect(self.releaseBtn)

        self.timer = QTimer()
        self.hold_type = 'p'
        self.timer.timeout.connect(self.btnHoldEvent)


    def updateRFID(self, data):
        if self.client:
            # print(data)
            self.textInfo.appendPlainText(data)

    def updateMPU(self, datas):
        if self.client:
            self.textInfo.appendPlainText(datas)

        self.textInfo.clear()

    def pressSend(self):
        if self.client.running:
            text = self.lineEdit.text()
            self.client.moveType = text

        
    def pressUp(self):
        if self.client:
            self.client.moveType = "w"
        self.timer.start(50)
        self.hold_type = "w"
    
    def pressDown(self):
        if self.client:
            self.client.moveType = "s"
        self.timer.start(50)
        self.hold_type = "s"
    
    def pressRight(self):
        if self.client:
            self.client.moveType = "d"
        self.timer.start(50)
        self.hold_type = "d"
    
    def pressLeft(self):
        if self.client:
            self.client.moveType = "a"
        self.timer.start(50)
        self.hold_type = "a"

    def releaseBtn(self):
        self.hold_type = "p"
        self.timer.stop()

    def btnHoldEvent(self):
        self.textInfo.appendPlainText(self.hold_type)
        if self.client:
            self.client.moveType = self.hold_type
    
    def emitUser(self, userName):
        self.textInfo.appendPlainText("RFID tagged!")
        self.textInfo.appendPlainText("User Name :", userName)
        print(userName)

        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWindows = WindowClass()
    myWindows.show()
    sys.exit(app.exec_())