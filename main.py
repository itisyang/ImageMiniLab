import sys
from ImageMiniLab import *
from PyQt6.QtWidgets import QApplication

if __name__ == '__main__':
    app = QApplication(sys.argv)
    myWin = ImageMiniLab()
    myWin.show()
    sys.exit(app.exec())
