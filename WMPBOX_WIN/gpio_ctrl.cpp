#include "gpio_ctrl.h"
#include "ui_gpio_ctrl.h"

Gpio_Ctrl::Gpio_Ctrl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gpio_Ctrl)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::Tool);
    setFixedSize(380, 50);
    gpio_init();

    for(int i = 0; i < GPIO_MAX; i++){
        connect(QCheckBox_GPIO_list[i], SIGNAL(clicked()), this, SLOT(gpio_set()));
    }

    lower();
}

Gpio_Ctrl::~Gpio_Ctrl()
{
    delete ui;
}

void Gpio_Ctrl::Active(CHD_WMP_T handler)
{
    this->handler = handler;
    show();
    gpio_fill();
}

void Gpio_Ctrl::Inactive()
{
    this->hide();
}

void Gpio_Ctrl::gpio_init()
{
    for(int i = 0; i < GPIO_MAX; i++){
        QCheckBox_GPIO_list <<ui->checkBox_GPIO1
                       <<ui->checkBox_GPIO2
                       <<ui->checkBox_GPIO3
                       <<ui->checkBox_GPIO4
                       <<ui->checkBox_GPIO5;
    }
}

void Gpio_Ctrl::gpio_fill()
{
    if(isVisible() == false){
        return ;
    }

    for(int i = 0; i < GPIO_MAX; i++){
        CHD_WMP_Gpio_GetStatus(handler, (CHD_GPIO_E)i, (CHD_GPIO_STATE_E *)&state[i]);
        QCheckBox_GPIO_list[i]->setChecked(state[i]);
    }
}

void Gpio_Ctrl::gpio_set()
{
    int i;
    for(i = 0; i < GPIO_MAX; i++){
        if(sender() == QCheckBox_GPIO_list[i]){
           break;
        }
    }

    int state = QCheckBox_GPIO_list[i]->isChecked();
    int ret = CHD_WMP_Gpio_SetStatus(handler, (CHD_GPIO_E)i, (CHD_GPIO_STATE_E)state);
    if(ret != CHD_RET_SUCCESS){
         QCheckBox_GPIO_list[i]->setChecked(!state);
    }
}

void Gpio_Ctrl::closeEvent(QCloseEvent *e)
{
    Inactive();
}
