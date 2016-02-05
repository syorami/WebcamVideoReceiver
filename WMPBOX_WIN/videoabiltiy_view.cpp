#include "videoabiltiy_view.h"
#include "ui_videoabiltiy_view.h"

VideoAbiltiy_view::VideoAbiltiy_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoAbiltiy_view)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::Tool);
    setFixedSize(width(), height());

    ui->plainTextEdit_videoability->setReadOnly(true);

    lower();
}

VideoAbiltiy_view::~VideoAbiltiy_view()
{
    delete ui;
}

void VideoAbiltiy_view::Active(CHD_WMP_T handler)
{
    this->handler = handler;
    show();
    videoabilty_fill();
}

void VideoAbiltiy_view::Inactive()
{
    this->hide();
}

void VideoAbiltiy_view::videoabilty_fill()
{
    CHD_RET_E ret;
    char buf[1024];
    unsigned int w, h;
    CHD_VIDEO_ABILITY_S ability;

    if(isVisible() == false) return;

    char * fmtstr[10] = {NULL, (char*)"YUYV", (char*)"MJPEG", (char*)"H264"};

    ui->plainTextEdit_videoability->clear();

    ret = CHD_WMP_Video_GetAbility(handler, &ability);
    if(ret == CHD_RET_SUCCESS){
        for(int f = 0; f < FORMAT_MAX_NUM; f++){
            if(ability.format[f].format == 0){
                break;
            }

            sprintf(buf, "No.%-2d : %s\n", f+1, fmtstr[ability.format[f].format]);
            ui->plainTextEdit_videoability->insertPlainText(buf);

            for(int r = 0; r < RESOLU_MAX_NUM; r++){
               if(ability.format[f].resolu[r].resolu == 0){
                   break;
               }

               CHD_Video_AnalysisResolu(ability.format[f].resolu[r].resolu, &w, &h);
               sprintf(buf, "\t%-2d %4dx%-4d : maxfps : %-4dfps\n", r+1, w,h, ability.format[f].resolu[r].maxfps);

               ui->plainTextEdit_videoability->insertPlainText(buf);
            }
        }
    }

}

void VideoAbiltiy_view::closeEvent(QCloseEvent *e)
{
    Inactive();
}


