#include "videodisplay.h"
#include "ui_videodisplay.h"

VideoDisplay::VideoDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoDisplay)
{

    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::Tool);

    doubledisplay = false;

//#ifndef MULTI_DISPLAY
//    ui->pushButton->hide();
//#endif

    lower();
}

VideoDisplay::~VideoDisplay()
{
    delete ui;
}



void VideoDisplay::Display_draw(QImage * image, int index, int format, int w, int h, int fps)
{
    static QString qstr_fmt;

    if(doubledisplay == true){
        if(fullscreen_index == 0){

            ui->label_Display->setGeometry(0, 0, this->width()/2, this->height());
            ui->label_Display_2->setGeometry(this->width()/2, 0, this->width()/2, this->height());

            *image = image->scaled(this->width()/2, this->height());

            if(index == 1){
                ui->label_Display->setPixmap(QPixmap::fromImage(*image));
            }
            else if(index == 2){
                ui->label_Display_2->setPixmap(QPixmap::fromImage(*image));
            }
        }
        else if(fullscreen_index == 1 && index == 1){
            ui->label_Display->setGeometry(0, 0, this->width(), this->height());
            *image = image->scaled(this->width(), this->height());

            ui->label_Display->setPixmap(QPixmap::fromImage(*image));
        }
        else if(fullscreen_index == 2 && index == 2){
            ui->label_Display_2->setGeometry(0, 0, this->width(), this->height());
            *image = image->scaled(this->width(), this->height());

            ui->label_Display_2->setPixmap(QPixmap::fromImage(*image));
        }

    }
    else{
        if(index != 1) return ;

        ui->label_Display->setGeometry(0, 0, this->width(), this->height());
        *image = image->scaled(this->width(), this->height());

        ui->label_Display->setPixmap(QPixmap::fromImage(*image));
    }

    delete image;


    if(format == CHD_FMT_YUYV){
        qstr_fmt = "YUYV ";
    }
    else if(format == CHD_FMT_MJPEG){
        qstr_fmt = "MJPEG ";
    }
    else if(format == CHD_FMT_H264){
        qstr_fmt = "H264 ";
    }

    setWindowTitle(QString("视频显示:")
                                + qstr_fmt
                                + QString::number(w) + "x" + QString::number(h) + "  "
                                + QString::number(fps) + "fps");


}


void VideoDisplay::mousePressEvent(QMouseEvent *event)
{
     oldMousepot = event->globalPos();
     oldMainwpot = pos();

     isMousePressed = true;
}

void VideoDisplay::mouseMoveEvent(QMouseEvent *event)
{

    if(true == isMousePressed){
        move((oldMainwpot - (oldMousepot - event->globalPos())));
    }

}

void VideoDisplay::mouseReleaseEvent(QMouseEvent * event)
{
    isMousePressed = false;
}

void VideoDisplay::mouseDoubleClickEvent( QMouseEvent * e )
{
    if(e->button() == Qt::LeftButton){
        if(this->isFullScreen() != true && doubledisplay == true){
            if(ui->label_Display->rect().contains(e->pos())){
                ui->label_Display_2->hide();
                fullscreen_index = 1;
            }
            else{
                fullscreen_index = 2;
                ui->label_Display->hide();
            }
            showFullScreen();
        }
        else if(this->isFullScreen() != true && doubledisplay == false){
            showFullScreen();
        }
        else{
            fullscreen_index = 0;
            ui->label_Display->show();
            ui->label_Display_2->show();
            showNormal();
        }
    }
}

void VideoDisplay::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "double"){
        ui->pushButton->setText("single");
        doubledisplay = true;
        fullscreen_index = 0;
        resize(width() * 2, height());

    }
    else{
        doubledisplay = false;
        ui->pushButton->setText("double");
        resize(width() / 2, height());
    }
}
