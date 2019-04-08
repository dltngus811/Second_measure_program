#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "matdisplay.h"

// OpenCV 및 기타 Library 추가
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QString>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    static Ui::MainWindow *ui;

// 마우스 좌표를 추적하는 함수 SLOT List
public slots:
    void showMousePosition(QPoint& pos);
    void showMousePosition2(QPoint& pos);

// private slots에 정의된 함수는 mainwindows.cpp에서 connect 및 기능 구현
// 버튼 및 이미지 처리에 사용될 함수 List
private slots:
    void On_UpperSide();        // Upper Side image 불러오는 함수
    void On_APview();           // AP view image 불러오는 함수
    void Save_UpperSide();      // Upper Side image 저장하는 함수
    void Save_APview();         // AP view image 불러오는 함수

};

#endif // MAINWINDOW_H
