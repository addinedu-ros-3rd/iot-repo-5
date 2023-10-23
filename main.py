import sys, os
import numpy as np
from datetime import datetime
from PyQt5 import uic
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from Controller import Controller

# 미리 video 폴더 생성 안해두면 안됨
from_class = uic.loadUiType(os.getcwd() +\
                            "/segway.ui")[0]



class WindowClass(QMainWindow, from_class) :
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        
        self.controller = Controller("COM10", 9600)
        
        self.controller.update.connect(self.updateController)
        
    #     self.btnUp.pressed.connect(self.pressUp)
    #     self.btnDown.pressed.connect(self.pressDown)
    #     self.btnRight.pressed.connect(self.pressRight)
    #     self.btnLeft.pressed.connect(self.pressLeft)
        
    def updateController(self, data):
        print(data)
        
        
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