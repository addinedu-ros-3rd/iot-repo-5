from PyQt5.QtCore import *
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
import time

class Uno(QThread):
    received_data = pyqtSignal(QByteArray, name="receiveData")

    def __init__(self, sec=0, parent=None):
        super().__init__()
        self.cond = QWaitCondition()
        self._status = False
        self.mutex = QMutex()

    def run(self):
        count = 0
        while self.running == True:
            self.update.emit()
            time.sleep(0.1)

    def stop(self):
        self.running = False