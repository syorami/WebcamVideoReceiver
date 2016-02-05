#include "video_ctrl.h"
#include "ui_video_ctrl.h"

video_ctrl::video_ctrl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::video_ctrl)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::Tool);
    setFixedSize(width(), height());
    vctrl_init();

    for(int i = 0; i < CHD_VCTRL_MAX; i++){
        connect(QHS_list[i], SIGNAL(sliderMoved(int)), QSpinBox_list[i], SLOT(setValue(int)));

        connect(QHS_list[i], SIGNAL(sliderReleased()), this, SLOT(vctrl_set()));
        connect(QCheckBox_list[i], SIGNAL(clicked()), this,  SLOT(vctrl_set()));
    }

    lower();
}

video_ctrl::~video_ctrl()
{
    delete ui;
}

void video_ctrl::Active(CHD_WMP_T handler)
{
    this->handler = handler;
    show();
    vctrl_fill();

}



void video_ctrl::Inactive()
{
    this->hide();
}

void video_ctrl::vctrl_init()
{
    //vctrl_info.name
    QString_List    << QString("亮度 ") << QString("对比度 ")
                    << QString("饱和度 ")
                    << QString("色调 ")
                    << QString("白平衡 ")
                    << QString("伽马 ")
                    << QString("增益 ")
                    << QString("背光补偿 ")
                    << QString("清晰度 ")
                    << QString("曝光 ");

    QLabel_list     <<ui->label_vctrl_1
                    <<ui->label_vctrl_2
                    <<ui->label_vctrl_3
                    <<ui->label_vctrl_4
                    <<ui->label_vctrl_5
                    <<ui->label_vctrl_6
                    <<ui->label_vctrl_7
                    <<ui->label_vctrl_8
                    <<ui->label_vctrl_9
                    <<ui->label_vctrl_10;


    QHS_list        <<ui->hs_vctrl_1
                    <<ui->hs_vctrl_2
                    <<ui->hs_vctrl_3
                    <<ui->hs_vctrl_4
                    <<ui->hs_vctrl_5
                    <<ui->hs_vctrl_6
                    <<ui->hs_vctrl_7
                    <<ui->hs_vctrl_8
                    <<ui->hs_vctrl_9
                    <<ui->hs_vctrl_10;

     QSpinBox_list  <<ui->spinBox_vctrl_1
                    <<ui->spinBox_vctrl_2
                    <<ui->spinBox_vctrl_3
                    <<ui->spinBox_vctrl_4
                    <<ui->spinBox_vctrl_5
                    <<ui->spinBox_vctrl_6
                    <<ui->spinBox_vctrl_7
                    <<ui->spinBox_vctrl_8
                    <<ui->spinBox_vctrl_9
                    <<ui->spinBox_vctrl_10;

     QCheckBox_list <<ui->checkBox_vctrl_1
                    <<ui->checkBox_vctrl_2
                    <<ui->checkBox_vctrl_3
                    <<ui->checkBox_vctrl_4
                    <<ui->checkBox_vctrl_5
                    <<ui->checkBox_vctrl_6
                    <<ui->checkBox_vctrl_7
                    <<ui->checkBox_vctrl_8
                    <<ui->checkBox_vctrl_9
                    <<ui->checkBox_vctrl_10;

}

void video_ctrl::vctrl_fill()
{
    if(isVisible() == false){
        return ;
    }

    for(int i = 0; i < CHD_VCTRL_MAX; i++){

         if(CHD_WMP_Video_GetVideoCtrl(handler, (CHD_VCTRL_E)i, &vctrl[i]) != CHD_RET_SUCCESS){
             break;
         }

         if(vctrl[i].val_valid == 1){

            QCheckBox_list[i]->setChecked(vctrl[i].autoval);
            QCheckBox_list[i]->setEnabled(vctrl[i].auto_valid);

            QHS_list[i]->setEnabled(!vctrl[i].autoval);

            QHS_list[i]->setMinimum(vctrl[i].minval);
            QSpinBox_list[i]->setMinimum(vctrl[i].minval);

            QSpinBox_list[i]->setMaximum(vctrl[i].maxval);
            QHS_list[i]->setMaximum(vctrl[i].maxval);

            QSpinBox_list[i]->setValue(vctrl[i].curval);
            QHS_list[i]->setValue(vctrl[i].curval);
            QHS_list[i]->setSingleStep(vctrl[i].stepval);
            QHS_list[i]->setPageStep(vctrl[i].stepval);
        }
        else{
            QHS_list[i]->setEnabled(false);
            QSpinBox_list[i]->setEnabled(false);
            QCheckBox_list[i]->setEnabled(false);
        }
    }
}

void video_ctrl::on_pushButton_vctrl_exit_clicked()
{
     Inactive();
}

void video_ctrl::vctrl_set()
{
    int i;
    int ret;
    for(i = 0; i < CHD_VCTRL_MAX; i++){
        if(sender() == QHS_list[i]
           || sender() == QCheckBox_list[i]){
            break;
        }
    }

    temp_vctrl.curval = QSpinBox_list[i]->value();
    temp_vctrl.autoval = QCheckBox_list[i]->isChecked();

    ret = CHD_WMP_Video_SetVideoCtrl(handler, (CHD_VCTRL_E)i, &temp_vctrl);
    if(ret == CHD_RET_SUCCESS){
        vctrl_fill();
    }
}

void video_ctrl::on_pushButton_vctrl_rest_clicked()
{
    int ret = CHD_WMP_Video_ResetVCtrl(handler);
    if(ret == CHD_RET_SUCCESS){
        vctrl_fill();
     }
}

void video_ctrl::closeEvent(QCloseEvent *e)
{
    Inactive();
}
