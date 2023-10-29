from collections import deque
from PyQt5.QtCore import *
from pyqtgraph import PlotWidget, plot
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg

app = QtGui.QGuiApplication([])
win = pg.GraphicsWidget()

class Plotter(QThread):
    def __init__(self):
        super().__init__()
        self.running = True

        self.graphWidget = pg.GraphicsLayout()

        self.i = 0
        self.num_MPU_data = 6
        self.max_plot_size = 1000
        tmp = deque([0] * self.max_plot_size)
        self.x_data = tmp
        self.y_datas = [tmp for _ in range(self.num_MPU_data)]
        self.data_label = ["accX", "accY", "accZ", "gyroX", "gyroY", "gyroZ"]
        
        self.graphWidget.showGrid(x=True, y=True)
        self.graphWidget.addLegend()
        self.graphWidget.setYRange(-10, 10, padding=0)
        self.plot = None
        self.curve = [None] * self.num_MPU_data

        self.color_list = ["#803723", "#1ff2ed", "#00fa5c",
                        "#aff0ed", "#f1af00", "#803723"]
        
        self.initPlotHandler()

        # for j in range(self.num_MPU_data):
        #     self.curve[j] = self.graphWidget.plot(self.x_data, self.y_datas[j])

    def initPlotHandler(self):
        for i in range(self.num_MPU_data):
            self.plot = self.win.addPlot()
            self.plot.setXRange(0, self.max_plot_size, padding=0)
            self.plot.setYRange(-10, 10, padding=0)
            self.win.nextRow()
            color = self.color_list[i]
            self.curve[i] = self.plot.plot(pen=pg.mkPen(color))

if __name__ == '__main__':
  try:
    a = QtGui.QGuiApplication.instance()
    T1 = Plotter()
    T1.daemon = True
    T1.start()
    a.exec_()
  except Exception as e:
    print(e)