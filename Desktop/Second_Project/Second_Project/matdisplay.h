#ifndef MATDISPLAY_H
#define MATDISPLAY_H

#include "mainwindow.h"
#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QMouseEvent>
#include <QtCore/qmath.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// 이미지 클릭 수 및 클릭한 위치를 저장하는 배열
extern int L_cnt;
extern int R_cnt;

// Upper image 좌표값 저장 배
extern int Value[8];
extern int Value_real[8];

// AP view image 좌표값 저장 배열
extern int ValueAP[8];
extern int ValueAP_real[8];

// 불러온 이미지의 폭, 너비
extern int img_w;
extern int img_h;

// 이미지의 임시 좌표?
extern int arr_x;
extern int arr_y;

// 이미지와 Label의 상대좌표
extern int ptx;
extern int pty;

// 실제 이미지에 찍힐 좌표
extern int real_x;
extern int read_y;

// 좌,우측 쇄골의 가로,세로 길이
extern double Lt_width;
extern double Lt_height;

extern double Rt_width;
extern double Rt_height;

class matDisplay : public QLabel
{
    Q_OBJECT

public:
    matDisplay(QWidget *parent = 0);
    ~matDisplay();

public:
    void mouseMoveEvent(QMouseEvent *mouse_event);
    void mousePressEvent(QMouseEvent *mouse_event);

signals:
    void sendMousePosition(QPoint&);


};

#endif // MATDISPLAY_H
