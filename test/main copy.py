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
        except:
            print("Connect Failed")
            sys.exit()

        # self.graphWidget = pg.GraphicsLayout()

        # self.i = 0
        # self.num_MPU_data = 6
        # self.max_plot_size = 1000
        # tmp = deque([0] * self.max_plot_size)
        # self.x_data = tmp
        # self.y_datas = [tmp for _ in range(self.num_MPU_data)]
        # self.data_label = ["accX", "accY", "accZ", "gyroX", "gyroY", "gyroZ"]
        
        # self.graphWidget.showGrid(x=True, y=True)
        # self.graphWidget.addLegend()
        # self.graphWidget.setYRange(-10, 10, padding=0)
        # self.plot = None
        # self.curve = [None] * self.num_MPU_data

        # self.colorList = ["#803723", "#1ff2ed", "#00fa5c",
        #                 "#aff0ed", "#f1af00", "#803723"]

        # for j in range(self.num_MPU_data):
        #     self.curve[j] = self.graphWidget.plot(self.x_data, self.y_datas[j])


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

    # def plot(self):
    #     for j in range(self.num_MPU_data):
    #         self.curve[j].setData(self.x_data, self.y_datas[j])
    #                             #   name=self.data_label[j])

    def updateRFID(self, data):
        if self.client:
            # print(data)
            self.textInfo.appendPlainText(data)

    def updateMPU(self, datas):
        if self.client:
            self.textInfo.appendPlainText(datas)

            # self.x_data.append(self.i)
            # for j, data in enumerate(datas.split(",")[:self.num_MPU_data]):
            #     self.y_datas[j][self.i] = float(data)

            # if len(self.x_data) < self.max_plot_size:
            #     self.i += 1

            # self.plot()
        

        self.textInfo.clear()
        self.resetPortDrop()
        self.resetBaudDrop()

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