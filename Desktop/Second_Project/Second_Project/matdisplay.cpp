#include "mainwindow.h"
#include "matdisplay.h"
#include "ui_mainwindow.h"
#include "math.h"           // 두 점 사이의 거리를 계산하기 위한 헤더 선언

// 이미지 클릭 수 및 클릭한 위치를 저장하는 배열
int L_cnt = 0;
int R_cnt = 0;

// Upper image 좌표값 저장 배열
int Value[8] = {0};
int Value_real[8] = {0};

// AP view image 좌표값 저장 배열
int ValueAP[8] = {0};
int ValueAP_real[8] = {0};

// 불러온 이미지의 폭, 너비
int img_w =0;
int img_h = 0;

// 이미지의 임시 좌표?
int arr_x = 0;
int arr_y = 0;

// 이미지와 Label의 상대좌표
int ptx = 0;
int pty = 0;

// 실제 이미지에 찍힐 좌표
int real_x = 0;
int read_y = 0;

// 좌,우측 쇄골의 가로,세로 길이
double Lt_width = 0;
double Lt_height = 0;

double Rt_width = 0;
double Rt_height = 0;

matDisplay::matDisplay(QWidget *parent) :
    QLabel(parent)
{
    this->setMouseTracking(true);
}

matDisplay::~matDisplay()
{

}

void matDisplay::mouseMoveEvent(QMouseEvent *mouse_event)
{
    QPoint  mouse_pos = mouse_event->pos();

    if(mouse_pos.x() <= this->size().width() && mouse_pos.y() <= this->size().height())
    {
        if(mouse_pos.x() >= 0 && mouse_pos.y() >= 0)
        {
            emit sendMousePosition(mouse_pos);
        }
    }
}

void matDisplay::mousePressEvent(QMouseEvent *mouse_event)
{
    QMessageBox msg;
    if(mouse_event->button() == Qt::LeftButton)
    {
        // Upper,AP Img에 대한 Img Processing 결과를 저장할 변수
        cv::Mat Upper_RstImg;
        cv::Mat AP_RstImg;

        // Variable of click amount, L_cnt = 1 이기 때문에 if(L_cnt == 1) 조건으로 이동
        L_cnt++;

        // 클릭한 위치를 변수에 저장
        arr_x = mouse_event->pos().x();
        arr_y = mouse_event->pos().y();

        // 이미지가 단일 Spec이 아닐 경우를 대비하여 상대좌표값 계산
        ptx = img_w / MainWindow::ui->Lt_Image->width();
        pty = img_h / MainWindow::ui->Lt_Image->height();

        // 클릭 수에 따라 클릭 오더를 내리는 기능
        if(L_cnt == 1) {

            // 미리 다음 클릭 오더를 띄움
            MainWindow::ui->Click_Order->setText("(Superior view) 우측 쇄골 내측단 클릭");

            // 클릭한 좌표를 배열에 저장
            Value[0] = mouse_event->pos().x() * ptx;
            Value[1] = mouse_event->pos().y() * pty;

            // 클릭한 원좌표를 배열에 저장
            Value_real[0] = mouse_event->pos().x();
            Value_real[1] = mouse_event->pos().y();

            // 클릭 지점을 나타내기 위해 이미지 Load 및 전처리
            Upper_RstImg = cv::imread("tmp_UpperImg.jpg");
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_BGR2RGB);

            // 클릭 지점에 원을 그리기
            cv::circle(Upper_RstImg, cvPoint(Value[0], Value[1]), 3, CV_RGB(0,0,255), 2, 8, 0);

            // 점 근처에 좌표값 출력 Test
            // 1. putText 함수에서 두번째 인자값은 str* 이므로 char배열 선언 후 sprintf 해줘야 함
            // 2. sprintf의 %d는 실제 이미지의 좌표가 아닌 512x512 size에 매핑된 주소이기 때문에
            // 마우스 좌표(우측 상단) 수치와 일치해야 함. 따라서 mouse_event->pos().x~y()
            char str[50];
            sprintf(str, "(%d,%d)", mouse_event->pos().x(), mouse_event->pos().y());
            //cv::putText(Upper_RstImg, str, cvPoint(Value[0],Value[1]-20), CV_FONT_HERSHEY_PLAIN, 2, cvScalar(255,0,0),4);

            // Label에 Image 뿌리기
            MainWindow::ui->Lt_Image->setPixmap(QPixmap::fromImage(QImage(Upper_RstImg.data, Upper_RstImg.cols, Upper_RstImg.rows, Upper_RstImg.step, QImage::Format_RGB888)));
            MainWindow::ui->Lt_Image->setScaledContents(true);
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_RGB2BGR);

            // Image 저장
            cv::imwrite("tmp_UpperImg.jpg", Upper_RstImg);
        }

        else if(L_cnt == 2) {

            // 미리 다음 클릭 오더를 띄움
            MainWindow::ui->Click_Order->setText("(Superior view) 좌측 쇄골 외측단 클릭");

            // 클릭한 좌표를 배열에 저장
            Value[2] = mouse_event->pos().x() * ptx;
            Value[3] = mouse_event->pos().y() * pty;

            // 클릭한 원좌표를 배열에 저장
            Value_real[2] = mouse_event->pos().x();
            Value_real[3] = mouse_event->pos().y();

            // 클릭 지점을 나타내기 위해 이미지 Load 및 전처리
            Upper_RstImg = cv::imread("tmp_UpperImg.jpg");
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_BGR2RGB);

            // 클릭 지점에 원을 그리기
            cv::circle(Upper_RstImg, cvPoint(Value[2], Value[3]), 3, CV_RGB(0,0,255), 2, 8, 0);

            // 점 근처에 좌표값 출력 Test
            // 1. putText 함수에서 두번째 인자값은 str* 이므로 char배열 선언 후 sprintf 해줘야 함
            // 2. sprintf의 %d는 실제 이미지의 좌표가 아닌 512x512 size에 매핑된 주소이기 때문에
            // 마우스 좌표(우측 상단) 수치와 일치해야 함. 따라서 mouse_event->pos().x~y()
            char str[50];
            sprintf(str, "(%d,%d)", mouse_event->pos().x(), mouse_event->pos().y());
            //cv::putText(Upper_RstImg, str, cvPoint(Value[2],Value[3]-20), CV_FONT_HERSHEY_PLAIN, 2, cvScalar(255,0,0),4);

            // 두 점 사이에 선 긋기
            cv::line(Upper_RstImg, cvPoint(Value[0],Value[1]), cvPoint(Value[2],Value[3]), CV_RGB(0,0,255), 2);

            // Label에 Image 뿌리기
            MainWindow::ui->Lt_Image->setPixmap(QPixmap::fromImage(QImage(Upper_RstImg.data, Upper_RstImg.cols, Upper_RstImg.rows, Upper_RstImg.step, QImage::Format_RGB888)));
            MainWindow::ui->Lt_Image->setScaledContents(true);

            // 두 점 사이의 거리 계산 후 출력 및 변수에 저장
            double Distance = sqrt(pow(Value_real[0]-Value_real[2],2) + pow(Value_real[1]-Value_real[3],2));
            Rt_width = Distance;

            QMessageBox msg;
            msg.setText("Distance -> "+QString::number(Distance));
            msg.exec();

            // Image 저장
            cv::imwrite("tmp_UpperImg.jpg", Upper_RstImg);
        }

        else if(L_cnt == 3) {

            // 미리 다음 클릭 오더를 띄움
            MainWindow::ui->Click_Order->setText("(Superior view) 좌측 쇄골 내측단 클릭");

            // 클릭한 좌표를 배열에 저장
            Value[4] = mouse_event->pos().x() * ptx;
            Value[5] = mouse_event->pos().y() * pty;

            // 클릭한 원좌표를 배열에 저장
            Value_real[4] = mouse_event->pos().x();
            Value_real[5] = mouse_event->pos().y();

            // 클릭 지점을 나타내기 위해 이미지 Load 및 전처리
            Upper_RstImg = cv::imread("tmp_UpperImg.jpg");
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_BGR2RGB);

            // 클릭 지점에 원을 그리기
            cv::circle(Upper_RstImg, cvPoint(Value[4], Value[5]), 3, CV_RGB(0,0,255), 2, 8, 0);

            // 점 근처에 좌표값 출력 Test
            // 1. putText 함수에서 두번째 인자값은 str* 이므로 char배열 선언 후 sprintf 해줘야 함
            // 2. sprintf의 %d는 실제 이미지의 좌표가 아닌 512x512 size에 매핑된 주소이기 때문에
            // 마우스 좌표(우측 상단) 수치와 일치해야 함. 따라서 mouse_event->pos().x~y()
            char str[50];
            sprintf(str, "(%d,%d)", mouse_event->pos().x(), mouse_event->pos().y());
            //cv::putText(Upper_RstImg, str, cvPoint(Value[4],Value[5]-20), CV_FONT_HERSHEY_PLAIN, 2, cvScalar(255,0,0),4);

            // Label에 Image 뿌리기
            MainWindow::ui->Lt_Image->setPixmap(QPixmap::fromImage(QImage(Upper_RstImg.data, Upper_RstImg.cols, Upper_RstImg.rows, Upper_RstImg.step, QImage::Format_RGB888)));
            MainWindow::ui->Lt_Image->setScaledContents(true);

            // Image 저장
            cv::imwrite("tmp_UpperImg.jpg", Upper_RstImg);
        }

        else if(L_cnt == 4) {

            // 미리 다음 클릭 오더를 띄움
            MainWindow::ui->Click_Order->setText("(Anterior view) 우측 쇄골 외측단 클릭");

            // 클릭한 좌표를 배열에 저장
            Value[6] = mouse_event->pos().x() * ptx;
            Value[7] = mouse_event->pos().y() * pty;

            // 클릭한 원좌표를 배열에 저장
            Value_real[6] = mouse_event->pos().x();
            Value_real[7] = mouse_event->pos().y();

            // 클릭 지점을 나타내기 위해 이미지 Load 및 전처리
            Upper_RstImg = cv::imread("tmp_UpperImg.jpg");
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_BGR2RGB);

            // 클릭 지점에 원을 그리기
            cv::circle(Upper_RstImg, cvPoint(Value[6], Value[7]), 3, CV_RGB(0,0,255), 2, 8, 0);

            // 점 근처에 좌표값 출력 Test
            // 1. putText 함수에서 두번째 인자값은 str* 이므로 char배열 선언 후 sprintf 해줘야 함
            // 2. sprintf의 %d는 실제 이미지의 좌표가 아닌 512x512 size에 매핑된 주소이기 때문에
            // 마우스 좌표(우측 상단) 수치와 일치해야 함. 따라서 mouse_event->pos().x~y()
            char str[50];
            sprintf(str, "(%d,%d)", mouse_event->pos().x(), mouse_event->pos().y());
            //cv::putText(Upper_RstImg, str, cvPoint(Value[6],Value[7]-20), CV_FONT_HERSHEY_PLAIN, 2, cvScalar(255,0,0),4);

            // 두 점 사이에 선 긋기
            cv::line(Upper_RstImg, cvPoint(Value[4],Value[5]), cvPoint(Value[6],Value[7]), CV_RGB(0,0,255), 2);

            // Label에 Image 뿌리기
            MainWindow::ui->Lt_Image->setPixmap(QPixmap::fromImage(QImage(Upper_RstImg.data, Upper_RstImg.cols, Upper_RstImg.rows, Upper_RstImg.step, QImage::Format_RGB888)));
            MainWindow::ui->Lt_Image->setScaledContents(true);

            // 두 점 사이의 거리 계산 후 출력
            double Distance = sqrt(pow(Value_real[4]-Value_real[6],2) + pow(Value_real[5]-Value_real[7],2));
            Lt_width = Distance;

            QMessageBox msg;
            msg.setText("Distance -> "+QString::number(Distance));
            msg.exec();

            // Image 저장
            cv::imwrite("tmp_UpperImg.jpg", Upper_RstImg);
        }

        else if(L_cnt == 5) {

            // 미리 다음 클릭 오더를 띄움
            MainWindow::ui->Click_Order->setText("(Anterior view) 우측 쇄골 내측단 클릭");

            // 클릭한 좌표를 배열에 저장
            ValueAP[0] = mouse_event->pos().x() * ptx;
            ValueAP[1] = mouse_event->pos().y() * pty;

            // 클릭한 원좌표를 배열에 저장
            ValueAP_real[0] = mouse_event->pos().x();
            ValueAP_real[1] = mouse_event->pos().y();

            // 클릭 지점을 나타내기 위해 이미지 Load 및 전처리
            AP_RstImg = cv::imread("tmp_APImage.jpg");
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_BGR2RGB);

            // 클릭 지점에 원을 그리기
            cv::circle(AP_RstImg, cvPoint(ValueAP[0], ValueAP[1]), 3, CV_RGB(0,0,255), 2, 8, 0);

            // 점 근처에 좌표값 출력 Test
            // 1. putText 함수에서 두번째 인자값은 str* 이므로 char배열 선언 후 sprintf 해줘야 함
            // 2. sprintf의 %d는 실제 이미지의 좌표가 아닌 512x512 size에 매핑된 주소이기 때문에
            // 마우스 좌표(우측 상단) 수치와 일치해야 함. 따라서 mouse_event->pos().x~y()
            char str[50];
            sprintf(str, "(%d,%d)", mouse_event->pos().x(), mouse_event->pos().y());
            //cv::putText(AP_RstImg, str, cvPoint(ValueAP[0],ValueAP[1]-20), CV_FONT_HERSHEY_PLAIN, 2, cvScalar(255,0,0),4);

            // Label에 Image 뿌리기
            MainWindow::ui->Rt_Image->setPixmap(QPixmap::fromImage(QImage(AP_RstImg.data, AP_RstImg.cols, AP_RstImg.rows, AP_RstImg.step, QImage::Format_RGB888)));
            MainWindow::ui->Rt_Image->setScaledContents(true);

            // Image 저장
            cv::imwrite("tmp_APImage.jpg", AP_RstImg);
        }

        else if(L_cnt == 6) {

            // 미리 다음 클릭 오더를 띄움
            MainWindow::ui->Click_Order->setText("(Anterior view) 좌측 쇄골 외측단 클릭");

            // 클릭한 좌표를 배열에 저장
            ValueAP[2] = mouse_event->pos().x() * ptx;
            ValueAP[3] = mouse_event->pos().y() * pty;

            // 클릭한 원좌표를 배열에 저장
            ValueAP_real[2] = mouse_event->pos().x();
            ValueAP_real[3] = mouse_event->pos().y();

            // 클릭 지점을 나타내기 위해 이미지 Load 및 전처리
            AP_RstImg = cv::imread("tmp_APImage.jpg");
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_BGR2RGB);

            // 클릭 지점에 원을 그리기
            cv::circle(AP_RstImg, cvPoint(ValueAP[2], ValueAP[3]), 3, CV_RGB(0,0,255), 2, 8, 0);

            // 점 근처에 좌표값 출력 Test
            // 1. putText 함수에서 두번째 인자값은 str* 이므로 char배열 선언 후 sprintf 해줘야 함
            // 2. sprintf의 %d는 실제 이미지의 좌표가 아닌 512x512 size에 매핑된 주소이기 때문에
            // 마우스 좌표(우측 상단) 수치와 일치해야 함. 따라서 mouse_event->pos().x~y()
            char str[50];
            sprintf(str, "(%d,%d)", mouse_event->pos().x(), mouse_event->pos().y());
            //cv::putText(AP_RstImg, str, cvPoint(ValueAP[2],ValueAP[3]-20), CV_FONT_HERSHEY_PLAIN, 2, cvScalar(255,0,0),4);

            // 두 점 사이에 선 긋기
            cv::line(AP_RstImg, cvPoint(ValueAP[0],ValueAP[1]), cvPoint(ValueAP[2],ValueAP[3]), CV_RGB(0,0,255), 2);

            // AP view Image에서 바닥선 긋기
            cv::line(AP_RstImg, cvPoint(ValueAP[2],ValueAP[3]), cvPoint(ValueAP[0],ValueAP[3]), CV_RGB(0,255,0), 2);

            // AP view imagem에서 구하고자 하는 높이 선 긋기
            cv::line(AP_RstImg, cvPoint(ValueAP[0],ValueAP[1]), cvPoint(ValueAP[0],ValueAP[3]), CV_RGB(255,0,0), 2);

            // Label에 Image 뿌리기
            MainWindow::ui->Rt_Image->setPixmap(QPixmap::fromImage(QImage(AP_RstImg.data, AP_RstImg.cols, AP_RstImg.rows, AP_RstImg.step, QImage::Format_RGB888)));
            MainWindow::ui->Rt_Image->setScaledContents(true);

            // 두 점 사이의 거리 계산 후 출력
            Rt_height = ValueAP_real[3] - ValueAP_real[1];
            QMessageBox msg;
            msg.setText("Distance -> "+QString::number(Rt_height));
            msg.exec();

            // Image 저장
            cv::imwrite("tmp_APImage.jpg", AP_RstImg);
        }

        else if(L_cnt == 7) {

            // 미리 다음 클릭 오더를 띄움
            MainWindow::ui->Click_Order->setText("(Anterior view) 우측 쇄골 내측단 클릭");

            // 클릭한 좌표를 배열에 저장
            ValueAP[4] = mouse_event->pos().x() * ptx;
            ValueAP[5] = mouse_event->pos().y() * pty;

            // 클릭한 원좌표를 배열에 저장
            ValueAP_real[4] = mouse_event->pos().x();
            ValueAP_real[5] = mouse_event->pos().y();

            // 클릭 지점을 나타내기 위해 이미지 Load 및 전처리
            AP_RstImg = cv::imread("tmp_APImage.jpg");
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_BGR2RGB);

            // 클릭 지점에 원을 그리기
            cv::circle(AP_RstImg, cvPoint(ValueAP[4], ValueAP[5]), 3, CV_RGB(0,0,255), 2, 8, 0);

            // 점 근처에 좌표값 출력 Test
            // 1. putText 함수에서 두번째 인자값은 str* 이므로 char배열 선언 후 sprintf 해줘야 함
            // 2. sprintf의 %d는 실제 이미지의 좌표가 아닌 512x512 size에 매핑된 주소이기 때문에
            // 마우스 좌표(우측 상단) 수치와 일치해야 함. 따라서 mouse_event->pos().x~y()
            char str[50];
            sprintf(str, "(%d,%d)", mouse_event->pos().x(), mouse_event->pos().y());
            //cv::putText(AP_RstImg, str, cvPoint(ValueAP[4],ValueAP[5]-20), CV_FONT_HERSHEY_PLAIN, 2, cvScalar(255,0,0),4);

            // Label에 Image 뿌리기
            MainWindow::ui->Rt_Image->setPixmap(QPixmap::fromImage(QImage(AP_RstImg.data, AP_RstImg.cols, AP_RstImg.rows, AP_RstImg.step, QImage::Format_RGB888)));
            MainWindow::ui->Rt_Image->setScaledContents(true);

            // Image 저장
            cv::imwrite("tmp_APImage.jpg", AP_RstImg);
        }

        else if(L_cnt == 8) {

            // 클릭한 좌표를 배열에 저장
            ValueAP[6] = mouse_event->pos().x() * ptx;
            ValueAP[7] = mouse_event->pos().y() * pty;

            // 클릭한 원좌표를 배열에 저장
            ValueAP_real[6] = mouse_event->pos().x();
            ValueAP_real[7] = mouse_event->pos().y();

            // 클릭 지점을 나타내기 위해 이미지 Load 및 전처리
            AP_RstImg = cv::imread("tmp_APImage.jpg");
            //cv::cvtColor(Upper_RstImg, Upper_RstImg, CV_BGR2RGB);

            // 클릭 지점에 원을 그리기
            cv::circle(AP_RstImg, cvPoint(ValueAP[6], ValueAP[7]), 3, CV_RGB(0,0,255), 2, 8, 0);

            // 점 근처에 좌표값 출력 Test
            // 1. putText 함수에서 두번째 인자값은 str* 이므로 char배열 선언 후 sprintf 해줘야 함
            // 2. sprintf의 %d는 실제 이미지의 좌표가 아닌 512x512 size에 매핑된 주소이기 때문에
            // 마우스 좌표(우측 상단) 수치와 일치해야 함. 따라서 mouse_event->pos().x~y()
            char str[50];
            sprintf(str, "(%d,%d)", mouse_event->pos().x(), mouse_event->pos().y());
            //cv::putText(AP_RstImg, str, cvPoint(ValueAP[6],ValueAP[7]-20), CV_FONT_HERSHEY_PLAIN, 2, cvScalar(255,0,0),4);

            // 두 점 사이에 선 긋기
            cv::line(AP_RstImg, cvPoint(ValueAP[4],ValueAP[5]), cvPoint(ValueAP[6],ValueAP[7]), CV_RGB(0,0,255), 2);

            // AP view Image에서 바닥선 긋기
            cv::line(AP_RstImg, cvPoint(ValueAP[6],ValueAP[7]), cvPoint(ValueAP[4],ValueAP[7]), CV_RGB(0,255,0), 2);

            // AP view imagem에서 구하고자 하는 높이 선 긋기
            cv::line(AP_RstImg, cvPoint(ValueAP[4],ValueAP[7]), cvPoint(ValueAP[4],ValueAP[5]), CV_RGB(255,0,0), 2);

            // Label에 Image 뿌리기
            MainWindow::ui->Rt_Image->setPixmap(QPixmap::fromImage(QImage(AP_RstImg.data, AP_RstImg.cols, AP_RstImg.rows, AP_RstImg.step, QImage::Format_RGB888)));
            MainWindow::ui->Rt_Image->setScaledContents(true);

            // 두 점 사이의 거리 계산 후 출력
            Lt_height = ValueAP_real[7] - ValueAP_real[5];
            QMessageBox msg;
            msg.setText("Distance -> "+QString::number(Lt_height));
            msg.exec();

            // Image 저장
            cv::imwrite("tmp_APImage.jpg", AP_RstImg);

            // 양측 쇄골 길이값을 구한 후 Label에 출력
            double Rt_length = sqrt(pow(Rt_height,2) + pow(Rt_width,2));
            double Lt_length = sqrt(pow(Lt_height,2) + pow(Lt_width,2));
            MainWindow::ui->ClavicleLength->setText("우측 쇄골 길이 : "+QString::number(Lt_length)+"\n좌측 쇄골 길이 : "+QString::number(Rt_length));
            MainWindow::ui->ClavicleRatio->setText("좌/우측 비율 -> 1 : "+QString::number(Rt_length/Lt_length)+"\n우/좌측 비율 -> 1 : "+QString::number(Lt_length/Rt_length));

        }
    }
}

