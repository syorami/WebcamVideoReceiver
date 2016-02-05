#include "i2c_ctrl.h"
#include "ui_i2c_ctrl.h"

I2C_Ctrl::I2C_Ctrl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::I2C_Ctrl)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()|Qt::Tool);
    setFixedSize(285, 255);

    ui->radioButton_havestop->setChecked(true);

    lower();
}

I2C_Ctrl::~I2C_Ctrl()
{
    delete ui;
}


void I2C_Ctrl::Active(CHD_WMP_T handler)
{
    this->handler = handler;
    ui->lineEdit_address->setText("0");
    ui->lineEdit_subaddress->setText("0");
    ui->lineEdit_readbits->setText("0");
    show();
}

void I2C_Ctrl::Inactive()
{
    hide();
}

void I2C_Ctrl::closeEvent(QCloseEvent *e)
{
    Inactive();
}


void I2C_Ctrl::on_pushButton_read_clicked()
{
    CHD_IIC_DATA_S i2cdata;
    CHD_RET_E ret;
    int len;
    ui->plainTextEdit_Read->clear();

    if(ui->lineEdit_readbits->text().toInt() == 0){
        QMessageBox::information(this,this->windowTitle(),
                                      QString("请指定读取字节长度 "),
                                      QMessageBox::Yes);
        return ;
    }

    i2cdata.address     = atohex(ui->lineEdit_address->text().toLocal8Bit().data());
    i2cdata.subaddress  = atohex(ui->lineEdit_subaddress->text().toLocal8Bit().data());
    i2cdata.readmode    = ui->radioButton_havestop->isChecked();
    i2cdata.datasize    = ui->lineEdit_readbits->text().toInt();

    ret = CHD_WMP_I2C_GetValue(handler, &i2cdata);

    len = i2cdata.datasize;
    if(ret == CHD_RET_SUCCESS){
        unsigned char * b = new unsigned char[i2cdata.datasize*5];
        unsigned char * p = b;
        for(int i = 0 ;i < len; i++){
            sprintf((char*)p, "%02x ", (unsigned char)i2cdata.data[i]);
            p += 3;
        }
        ui->plainTextEdit_Read->insertPlainText((char*)b);
        delete b;
    }
    else{
        QMessageBox::information(this,this->windowTitle(),
                                      QString("I2C操作失败,没有该设备 "),
                                      QMessageBox::Yes);
    }
}

void I2C_Ctrl::on_pushButton_write_clicked()
{
    CHD_IIC_DATA_S i2cdata;
    CHD_RET_E ret;

    i2cdata.address = atohex(ui->lineEdit_address->text().toLocal8Bit().data());
    i2cdata.subaddress = atohex(ui->lineEdit_subaddress->text().toLocal8Bit().data());

    unsigned char * buffer = new unsigned char[ui->plainTextEdit_Write->toPlainText().length()];
    QStringList list;
    list.append(ui->plainTextEdit_Write->toPlainText().split(" "));

    for(int i = 0 ;i < list.length(); i++){
         buffer[i] = atohex(list.at(i).toLocal8Bit().data());
    }

    i2cdata.datasize = list.length();
    memcpy(i2cdata.data, buffer, list.length());
    ret = CHD_WMP_I2C_SetValue(handler, &i2cdata);
    if(ret == CHD_RET_SUCCESS){
        QMessageBox::information(this,this->windowTitle(),
                                      QString("I2C写入成功 "),
                                      QMessageBox::Yes);
    }
    else{
        QMessageBox::information(this,this->windowTitle(),
                                      QString("I2C操作失败,没有该设备 "),
                                      QMessageBox::Yes);
    }


}

/*a 的 b 次方*/
static int powint(int a, int b)
{
    int value = 1;

    if(a == 0)
        return 0;

    for(;b > 0; b--){
        value *= a;
    }

    return value;
}

/* 字符串转十六进制 自适应*/
unsigned int atohex(char *hexValue)
{
    int value = 0, tmpval = 0, atoc = 0;
    int len;
    char *p = NULL, c;

    if(NULL == hexValue){
        printf("empty data...\n");
        return -1;
    }
    /*Adaptive Hex*/
    p = ('x' == hexValue[1]) ? (hexValue + 2) : hexValue;
    len = strlen(p);

    for(c = *p; *p && len > 0; p++, c = *p){
            /*0 - 9*/
        if(c >= 48 && c <= 57){
            atoc = c - '0';
        }
            /*A - F*/
        else if(c >= 65 && c <= 70){
            atoc = c - 55;
        }
            /*a - f*/
        else if(c >= 97 && c <= 102){
            atoc = c - 87;
        }
        else{
            return -1;
        }
        tmpval = atoc * powint(16, --len);
        value += tmpval;
    }

    return value;
}







