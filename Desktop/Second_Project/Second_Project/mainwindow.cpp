#include "mainwindow.h"
#include "matdisplay.h"
#include "ui_mainwindow.h"
#include "QFileInfo"        // 이미지 저장 시 File Dialog에서 Esc 눌렀을 때 에러 발생 없애기 위한 선언

// 서로 다른 함수에서 이미지를 불러와야 하기 때문에 Public cv::Mat 변수 선언
cv::Mat Upper_Img;
cv::Mat AP_Img;

Ui::MainWindow* MainWindow::ui = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    MainWindow::ui = new Ui::MainWindow;
    MainWindow::ui->setupUi(this);

    connect(ui->Open_Upper, SIGNAL(triggered()), this, SLOT(On_UpperSide()));
    connect(ui->Open_AP, SIGNAL(triggered()), this, SLOT(On_APview()));
    connect(ui->Save_Upper, SIGNAL(triggered()), this, SLOT(Save_UpperSide()));
    connect(ui->Save_AP, SIGNAL(triggered()), this, SLOT(Save_APview()));
    connect(ui->Lt_Image, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePosition(QPoint&)));
    connect(ui->Rt_Image, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePosition2(QPoint&)));

}

MainWindow::~MainWindow()
{
    delete MainWindow::ui;
}


// Upper Side (좌측) Image 좌표 출력 / 19.02.25 이수현
void MainWindow::showMousePosition(QPoint &pos)
{
    ui->Lt_Location->setText("x: "+ QString::number(pos.x()) + " y: "+ QString::number(pos.y()));
}

void MainWindow::showMousePosition2(QPoint &pos)
{
    ui->Rt_Location->setText("x: "+ QString::number(pos.x()) + " y: "+ QString::number(pos.y()));
}

// Upper Side (좌측) Image 불러오는 버튼 (Menu -> Open -> Upper Side)
void MainWindow::On_UpperSide()
{
    L_cnt = 0;
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open Upper side image"),
                       "../",
                       tr("Image Files(*.png *jpg *.bmp)"));

    Upper_Img = cv::imread(filename.toLocal8Bit().constData());

    /*
     * if문 없이 이미지를 불러오지 않고 File Dialog 창을 닫으면 cv::cvtColor() 함수 오류 발생
     * 원인 : 데이터가 없는 껍데기 이미지에 컬러모델 변환을 하면 에러로 판단 추정
     * 조치 : 조건문에 Image Data가 존재하면 cvtColor함수 및 Label에 Image를 뿌리도록 구성
     */
    if(Upper_Img.data) {
        cv::cvtColor(Upper_Img, Upper_Img, CV_BGR2RGB);
        ui->Lt_Image->setPixmap(QPixmap::fromImage(QImage(Upper_Img.data, Upper_Img.cols, Upper_Img.rows, Upper_Img.step, QImage::Format_RGB888)));
        ui->Lt_Image->setScaledContents(true);
        cv::imwrite("tmp_UpperImg.jpg",Upper_Img);

        // 이미지의 rows, cols 값을 전역변수에 저장
        // 동일환자의 Upper, Ap Img는 스펙이 동일하는 전제 하에 Upper Img Open시에만 이미지값 저장
        img_w = Upper_Img.rows;
        img_h = Upper_Img.cols;

        // Upper Img 불러올 시 AP Img Open order 생성
        MainWindow::ui->Click_Order->setText("Open AP view image");
    }

    else {
        QMessageBox msg;
        msg.setWindowTitle("Alert");
        msg.setText("Image Open Fail\t");
        msg.exec();
    }
}

// AP view (우측) Image 불러오는 버튼 (Menu -> Open -> AP view)
void MainWindow::On_APview()
{
    L_cnt = 0;
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open AP view image"),
                       "../",
                       tr("Image Files(*.png *jpg *.bmp)"));

    AP_Img = cv::imread(filename.toLocal8Bit().constData());

    /*
     * 19. 02. 25 이수현
     * if문 없이 이미지를 불러오지 않고 File Dialog 창을 닫으면 cv::cvtColor() 함수 오류 발생
     * 원인 : 데이터가 없는 껍데기 이미지에 컬러모델 변환을 하면 에러로 판단 추정
     * 조치 : 조건문에 Image Data가 존재하면 cvtColor함수 및 Label에 Image를 뿌리도록 구성
     */
    if(AP_Img.data) {
        cv::cvtColor(AP_Img, AP_Img, CV_BGR2RGB);
        ui->Rt_Image->setPixmap(QPixmap::fromImage(QImage(AP_Img.data, AP_Img.cols, AP_Img.rows, AP_Img.step, QImage::Format_RGB888)));
        ui->Rt_Image->setScaledContents(true);
        cv::imwrite("tmp_APImage.jpg",AP_Img);

        // AP Img 불러올 시 Upper Img Click order 생성
        MainWindow::ui->Click_Order->setText("(Superior view) 우측 쇄골 외측단 클릭");
    }

    else {
        QMessageBox msg;
        msg.setWindowTitle("Alert");
        msg.setText("Image Open Fail\t");
        msg.exec();
    }
}

// Upper Side (좌측) Image 저장하는 버튼 (Menu -> Save -> Upper Side)
void MainWindow::Save_UpperSide()
{
    QString filename = QFileDialog::getSaveFileName(this,
                       tr("Save Upper Side Image"),
                       "../",
                       tr("Image Files(*.png *jpg *.bmp)"));

    // On_UpperSide() 함수에서 이미지를 불러왔어도 사용자가 File Dialog에서 Esc 키 누를 수 있기 때문에
    // 파일 경로가 지정된 유무를 조건문에서 체크
    if(Upper_Img.data) {
        if(filename.toUtf8().constData()) {
            cv::cvtColor(Upper_Img, Upper_Img, CV_BGR2RGB);
            cv::imwrite(filename.toStdString(),Upper_Img);
        }
    }

    else {
        QMessageBox msg;
        msg.setWindowTitle("Alert");
        msg.setText("Upper Side Image Save Fail\t");
        msg.exec();
    }
}

// AP View (우측) Image 저장하는 버튼 (Menu -> Save -> AP View)
void MainWindow::Save_APview()
{
    QString filename = QFileDialog::getSaveFileName(this,
                       tr("Save AP View Image"),
                       "../",
                       tr("Image Files(*.png *jpg *.bmp)"));

    // On_APview() 함수에서 이미지를 불러왔어도 사용자가 File Dialog에서 Esc 키 누를 수 있기 때문에
    // 파일 경로가 지정된 유무를 조건문에서 체크
    if(QFileInfo(filename).exists()) {
        cv::cvtColor(AP_Img, AP_Img, CV_BGR2RGB);
        cv::imwrite(filename.toStdString(),AP_Img);
    }


    else {
        QMessageBox msg;
        msg.setWindowTitle("Alert");
        msg.setText("AP View Image Save Fail\t");
        msg.exec();
    }
}
