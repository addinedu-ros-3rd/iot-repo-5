import sys, os
import numpy as np
from collections import deque
from datetime import datetime

from PyQt5 import uic
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

import serial.tools.list_ports

# from pyqtgraph import PlotWidget, plot
# import pyqtgraph as pg

from Controller import Controller



from_class = uic.loadUiType(os.getcwd() +\
                            "/segway.ui")[0]

class WindowClass(QMainWindow, from_class) :
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        
        self.controller = None

        self.resetPortDrop()
        self.resetBaudDrop()

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
        self.hold_type = None
        self.timer.timeout.connect(self.btnHoldEvent)

        self.btnConnect.pressed.connect(self.pressConnect)
        self.btnSend.pressed.connect(self.pressSend)
        self.btnReset.pressed.connect(self.pressReset)

    # def plot(self):
    #     for j in range(self.num_MPU_data):
    #         self.curve[j].setData(self.x_data, self.y_datas[j])
    #                             #   name=self.data_label[j])

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

    def updateRFID(self, data):
        if self.controller:
            # print(data)
            self.textInfo.appendPlainText(data)

    def updateMPU(self, datas):
        if self.controller:
            self.textInfo.appendPlainText(datas)

            # self.x_data.append(self.i)
            # for j, data in enumerate(datas.split(",")[:self.num_MPU_data]):
            #     self.y_datas[j][self.i] = float(data)

            # if len(self.x_data) < self.max_plot_size:
            #     self.i += 1

            # self.plot()
        


    def errorController(self):
        self.controller.stop()
        self.controller = None

    def pressReset(self):
        if self.controller:
            self.controller.stop()

        self.textInfo.clear()
        self.resetPortDrop()
        self.resetBaudDrop()

    def pressConnect(self):
        port_name = self.comboPort.currentText()
        baudrate = int(self.comboBaudrate.currentText())

        try:
            self.controller = Controller(port_name=port_name, baudrate=baudrate, timeout=(1/100))
            self.controller.daemon = True
            self.controller.rfid_data.connect(self.updateRFID)
            self.controller.mpu_data.connect(self.updateMPU)
            self.controller.error_flag.connect(self.errorController)
            self.controller.start()

        except:
            self.controller = None
            return

    def pressSend(self):
        if self.controller.connection:
            text = self.lineEdit.text()
            self.controller.sendData(text)

        
    def pressUp(self):
        if self.hold_type is None:
            self.controller.sendData("U")
        self.timer.start(100)
        self.hold_type = "U"
    
    def pressDown(self):
        if self.hold_type is None:
            self.controller.sendData("D")
        self.timer.start(100)
        self.hold_type = "D"
    
    def pressRight(self):
        if self.hold_type is None:
            self.controller.sendData("R")
        self.timer.start(100)
        self.hold_type = "R"
    
    def pressLeft(self):
        if self.hold_type is None:
            self.controller.sendData("L")
        self.timer.start(100)
        self.hold_type = "L"

    def releaseBtn(self):
        self.hold_type = None
        self.timer.stop()

    def btnHoldEvent(self):
        self.textInfo.appendPlainText(self.hold_type)
        self.controller.sendData(self.hold_type)

        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWindows = WindowClass()
    myWindows.show()
    sys.exit(app.exec_())