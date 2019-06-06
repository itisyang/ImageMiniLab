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


def clamp(pv):
    if pv > 255:
        return 255
    elif pv < 0:
        return 0
    else:
        return pv


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
                         "反转": self.bitwise_not,
                         "通道分离": self.channels_split,
                         "噪声、滤波": self.noise_and_blur,
                         "高斯双边滤波": self.bilateral_filter,
                         "均值偏移滤波": self.mean_shift_filter,
                         "图像二值化": self.threshold,
                         "Canny边缘检测": self.canny_edge,
                         "直线检测": self.hough_line,
                         "圆检测": self.hough_circles,
                         "轮廓发现": self.find_contours}
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

    @QtCore.pyqtSlot()
    def on_ExpTypeComboBox_currentTextChanged(self, text):
        if text == "通道分离":
            # 显示对应的处理参数界面
            pass

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

    # 通道分离
    def channels_split(self):
        src = self.cv_read_img(self.src_file)
        if src is None:
            return
        b, g, r = cv.split(src)
        merge_image = cv.merge([b, g, r])
        """创建三维数组，0维为B，1维为G，2维为R"""
        height, width, channels = src.shape
        img = np.zeros([height*2, width*2, channels], np.uint8)
        img[0:height, 0:width] = np.expand_dims(b, axis=2)
        img[0:height, width:width*2] = np.expand_dims(g, axis=2)
        img[height:height*2, 0:width] = np.expand_dims(r, axis=2)
        img[height:height*2, width:width*2] = merge_image

        self.decode_and_show_dst(img)

    '''
    一些图像知识：
    1. 噪声：主要有三种：
    椒盐噪声（Salt & Pepper）：含有随机出现的黑白亮度值。
    脉冲噪声：只含有随机的正脉冲和负脉冲噪声。
    高斯噪声：含有亮度服从高斯或正态分布的噪声。高斯噪声是很多传感器噪声的模型，如摄像机的电子干扰噪声。
    2. 滤波器：主要两类：线性和非线性
    线性滤波器：使用连续窗函数内像素加权和来实现滤波，同一模式的权重因子可以作用在每一个窗口内，即线性滤波器是空间不变的。
    如果图像的不同部分使用不同的滤波权重因子，线性滤波器是空间可变的。因此可以使用卷积模板来实现滤波。
    线性滤波器对去除高斯噪声有很好的效果。常用的线性滤波器有均值滤波器和高斯平滑滤波器。
    (1) 均值滤波器：最简单均值滤波器是局部均值运算，即每一个像素只用其局部邻域内所有值的平均值来置换.
    (2) 高斯平滑滤波器是一类根据高斯函数的形状来选择权值的线性滤波器。 高斯平滑滤波器对去除服从正态分布的噪声是很有效的。
    非线性滤波器:
    (1) 中值滤波器:均值滤波和高斯滤波运算主要问题是有可能模糊图像中尖锐不连续的部分。
    中值滤波器的基本思想使用像素点邻域灰度值的中值来代替该像素点的灰度值，它可以去除脉冲噪声、椒盐噪声同时保留图像边缘细节。
    中值滤波不依赖于邻域内与典型值差别很大的值，处理过程不进行加权运算。
    中值滤波在一定条件下可以克服线性滤波器所造成的图像细节模糊，而对滤除脉冲干扰很有效。
    (2) 边缘保持滤波器:由于均值滤波：平滑图像外还可能导致图像边缘模糊和中值滤波：去除脉冲噪声的同时可能将图像中的线条细节滤除。
    边缘保持滤波器是在综合考虑了均值滤波器和中值滤波器的优缺点后发展起来的，它的特点是：
    滤波器在除噪声脉冲的同时，又不至于使图像边缘十分模糊。
    过程：分别计算[i，j]的左上角子邻域、左下角子邻域、右上角子邻域、右下角子邻域的灰度分布均匀度V；
    然后取最小均匀度对应区域的均值作为该像素点的新灰度值。分布越均匀，均匀度V值越小。v=<(f(x, y) - f_(x, y))^2

    '''
    def noise_and_blur(self):
        src = self.cv_read_img(self.src_file)
        if src is None:
            return

        # 加高斯噪声
        h, w, c = src.shape
        for row in range(h):
            for col in range(w):
                s = np.random.normal(0, 20, 3)  # normal(loc=0.0, scale=1.0, size=None),均值，标准差，大小

                b = src[row, col, 0]
                g = src[row, col, 1]
                r = src[row, col, 2]

                src[row, col, 0] = clamp(b + s[0])
                src[row, col, 1] = clamp(g + s[1])
                src[row, col, 2] = clamp(r + s[2])

        img = np.zeros([h * 2, w * 2, c], np.uint8)
        img[0:h, 0:w] = src

        # GaussianBlur(src, ksize, sigmaX, dst=None, sigmaY=None, borderType=None)
        # ksize表示卷积核大小，sigmaX，Y表示x，y方向上的标准差，这两者只需一个即可，并且ksize为大于0的奇数
        dst = cv.GaussianBlur(src, (5, 5), 0)  # 高斯模糊，sigmaX与ksize一个为0
        img[0:h, w:w*2] = dst

        self.decode_and_show_dst(img)

    """
    同时考虑空间与信息和灰度相似性，达到保边去噪的目的
    双边滤波的核函数是空间域核与像素范围域核的综合结果：
    在图像的平坦区域，像素值变化很小，对应的像素范围域权重接近于1，此时空间域权重起主要作用，相当于进行高斯模糊；
    在图像的边缘区域，像素值变化很大，像素范围域权重变大，从而保持了边缘的信息。
    """
    def bilateral_filter(self):
        src = self.cv_read_img(self.src_file)
        if src is None:
            return

        dst = cv.bilateralFilter(src, 0, 100, 15)  # 高斯双边滤波
        self.decode_and_show_dst(dst)

    def mean_shift_filter(self):
        src = self.cv_read_img(self.src_file)
        if src is None:
            return

        dst = cv.pyrMeanShiftFiltering(src, 10, 50)  # 均值偏移滤波
        self.decode_and_show_dst(dst)

    def threshold(self):
        src = self.cv_read_img(self.src_file)
        if src is None:
            return

        gray = cv.cvtColor(src, cv.COLOR_BGR2GRAY)

        # 这个函数的第一个参数就是原图像，原图像应该是灰度图。
        # 第二个参数就是用来对像素值进行分类的阈值。
        # 第三个参数就是当像素值高于（有时是小于）阈值时应该被赋予的新的像素值
        # 第四个参数来决定阈值方法，见threshold_simple()
        # ret, binary = cv.threshold(gray, 127, 255, cv.THRESH_BINARY)
        ret, dst = cv.threshold(gray, 127, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
        self.decode_and_show_dst(dst)

    def canny_edge(self):
        src = self.cv_read_img(self.src_file)
        if src is None:
            return

        blurred = cv.GaussianBlur(src, (3, 3), 0)
        gray = cv.cvtColor(blurred, cv.COLOR_BGR2GRAY)

        grad_x = cv.Sobel(gray, cv.CV_16SC1, 1, 0)
        grad_y = cv.Sobel(gray, cv.CV_16SC1, 0, 1)

        dst = cv.Canny(grad_x, grad_y, 30, 150)
        # dst = cv.Canny(gray, 50, 150)
        self.decode_and_show_dst(dst)

    # 直线检测
    def hough_line(self):
        pass

    # 圆检测
    def hough_circles(self):
        pass

    # 轮廓发现
    def find_contours(self):
        pass

