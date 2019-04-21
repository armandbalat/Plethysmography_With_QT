#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/objdetect.hpp"

#include <QTimer>
#include <QFile>
#include <iostream>
#include <fstream>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void calculatePixelArea(std::vector<Rect> front);
    void CaptureImage();
    void WriteOnTxt(double moyenne);
    Mat m_frame;
    QTimer *m_timer;

    VideoCapture m_cap;
    QImage m_qtImage;

private slots :
    void openCapture();
    void closeCapture();
    void update_window();
    void detectAndDisplay(Mat l_frame);

public slots :

};

#endif // MAINWINDOW_H
