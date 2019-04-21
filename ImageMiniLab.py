import cv2 as cv
import numpy as np
from PyQt5.QtWidgets import QMainWindow, QFileDialog, QMessageBox
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt
from ImageMiniLabUI import *


# 图像信息
def get_image_info(image):
    print("图像类型：",type(image))
    print("图像长x宽x通道数：",image.shape)
    print("图像长宽通道数相乘所得值：",image.size)
    print("图像像素值类型：",image.dtype)
    pixel_data = np.array(image)  # 将图像转换成数组
    print("像素大小：", pixel_data)


class ImageMiniLab(QMainWindow, Ui_ImageMiniLabUI):
    def __init__(self, parent=None):
        super(ImageMiniLab, self).__init__(parent)
        self.setupUi(self)

        self.SrcImgShowLabel.clear()
        self.DstImgShowLabel.clear()
        # self.SrcImgShowLabel.setScaledContents(True)
        # self.DstImgShowLabel.setScaledContents(True)

        self.src_file = ""  # 图像原始路径，cv处理用
        self.src_pix = QPixmap()  # 未处理像素，显示用
        self.dst_pix = QPixmap()  # 处理后像素，显示用

        # 实验内容，接口定义，实验入口
        self.exp_type = {"选择实验类型":self.no_exp_type,
                         "灰度化":self.to_gray,
                         "反转": self.bitwise_not}
        self.ExpTypeComboBox.addItems(self.exp_type)

    # 载入图像（初次）
    def load_exp_img(self, img_name):
        self.ExpImgLineEdit.setText(img_name)
        if self.src_pix.load(img_name) is False:
            QMessageBox.warning(self, "打开图片失败", "请更换图片")
            return
        else:
            self.src_file = img_name
            self.dst_pix = self.src_pix.copy(self.src_pix.rect())
            self.resizeEvent(None)
            self.show_exp_pix()

    # 显示图像
    def show_exp_pix(self):
        if self.src_pix.width() > 0:
            w = self.SrcImgShowLabel.width()
            h = self.SrcImgShowLabel.height()
            self.SrcImgShowLabel.setPixmap(self.src_pix.scaled(w, h, Qt.KeepAspectRatio))
        if self.dst_pix.width() > 0:
            w = self.DstImgShowLabel.width()
            h = self.DstImgShowLabel.height()
            self.DstImgShowLabel.setPixmap(self.dst_pix.scaled(w, h, Qt.KeepAspectRatio))

    # 窗口大小变化，使显示内容适应窗口大小
    def resizeEvent(self, event):
        w = self.ImgShowWidget.width()
        h = self.ImgShowWidget.height()
        self.SrcImgShowLabel.resize(w/2, h)
        self.SrcImgShowLabel.move(0, 0)
        self.DstImgShowLabel.resize(w/2, h)
        self.DstImgShowLabel.move(w/2, 0)
        self.show_exp_pix()

    # 装饰器，必须注明，不然槽会调用两次
    @QtCore.pyqtSlot()
    def on_SelectImgPushButton_clicked(self):
        # 设置文件扩展名过滤,注意用双分号间隔
        img_name, file_type = QFileDialog.getOpenFileName(self, "选取实验图片", ".", "All Files (*);;Images (*.png *.jpg *.bmp)")
        print(img_name)
        if len(img_name) == 0:
            return
        self.load_exp_img(img_name)

    @QtCore.pyqtSlot()
    def on_LoadTestDataPushButton_clicked(self):
        # 测试参数
        self.ExpTypeComboBox.setCurrentIndex(1)
        test_img = './lena.jpg'
        self.ExpImgLineEdit.setText(test_img)
        self.load_exp_img(test_img)

    @QtCore.pyqtSlot()
    def on_GoExpPushButton_clicked(self):
        cur_exp_type = self.ExpTypeComboBox.currentText()
        print("实验类型：", cur_exp_type)
        if cur_exp_type not in self.exp_type:
            QMessageBox.warning(self, "实验类型出错", "实验类型不存在，或无实验处理步骤，请联系技术支持。")
        # print("类型", type(self.exp_type[cur_exp_type]))
        self.exp_type[cur_exp_type]()

    # 未选择实验类型
    def no_exp_type(self):
        QMessageBox.warning(self, "未选择实验类型", "请先选择实验类型。")

    # cv读取图片
    def cv_read_img(self, img):
        src = cv.imread(img)
        if src is None:
            QMessageBox.warning(self, "载入出错", "图片读取失败。\n（可能原因：无图片、无正确权限、不受支持或未知的格式）")
            return None
        return src

    def decode_and_show_dst(self, dst):
        ret, img_buf = cv.imencode(".jpg", dst)
        # print(ret, img_buf)
        if ret is True:
            ret = self.dst_pix.loadFromData(img_buf)
            if ret is True:
                self.show_exp_pix()

    # 灰度化
    def to_gray(self):
        src = self.cv_read_img(self.src_file)
        if src is None:
            return
        gray = cv.cvtColor(src, cv.COLOR_BGR2GRAY)
        # print("类型", type(gray))
        # get_image_info(gray)
        self.decode_and_show_dst(gray)

    # 反转
    def bitwise_not(self):
        src = self.cv_read_img(self.src_file)
        if src is None:
            return
        dst = cv.bitwise_not(src)  # 按位取反，白变黑，黑变白
        self.decode_and_show_dst(dst)
