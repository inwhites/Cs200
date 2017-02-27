#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cell.h"
#include "init.h"



#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QByteArray>
#include <QTime>
#include <QProgressDialog>
#include <QScrollBar>
#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QAxObject>




//***********************************************************
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_setting();

               //测量
    connect(ui->measure,SIGNAL(clicked(bool)),
          this,SLOT(startORcancel()));
    connect(ui->isconnected,SIGNAL(clicked()),
            this,SLOT(remote_state()));           //连接
    connect(ui->clear,SIGNAL(clicked()),this,SLOT(clearAll_clicked()));
    connect(ui->exportExcel,SIGNAL(clicked()),this,SLOT(export_excel()));
    connect(ui->speed,SIGNAL(currentIndexChanged(int)),this,SLOT(speed_changed(int)));
    connect(ui->progressBar,SIGNAL(valueChanged(int)),this,SLOT(proTextDisplay()));

    connect(ui->FindCom,SIGNAL(clicked(bool)),this,SLOT(init_serial()));

    connect(ui->OpenComBut,SIGNAL(clicked(bool)),this,SLOT(open_serial()));
    connect(ui->XScrollBar,SIGNAL(valueChanged(int)),this,SLOT(x_chg()));
    connect(ui->YScrollBar,SIGNAL(valueChanged(int)),this,SLOT(y_chg()));
    connect(ui->ZScrollBar,SIGNAL(valueChanged(int)),this,SLOT(z_chg()));
    connect(ui->RecordPos,SIGNAL(clicked(bool)),this,SLOT(lcd_record()));
    connect(ui->SetPos,SIGNAL(clicked(bool)),this,SLOT(loc_pos()));
    connect(ui->PosReset,SIGNAL(clicked(bool)),this,SLOT(reset()));


}



//***********************************************************
MainWindow::~MainWindow()
{
    delete ui;
}





//***********************************************************
//初始化设置
void MainWindow::init_setting(){

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->measure->setEnabled(false);
    ui->measure->setToolTip("start OR stop");
    ui->isconnected->setToolTip("Connect");
    ui->clear->setToolTip("clear Table");
    ui->exportExcel->setToolTip("Save");
    ui->color->setToolTip("the color be measured");
    ui->distance->setToolTip("the diatance between display and device");
    ui->progressText->setText("0");

    ui->XScrollBar->setEnabled(false);
    ui->YScrollBar->setEnabled(false);
    ui->ZScrollBar->setEnabled(false);

    ui->RecordPos->setEnabled(false);
    ui->SetPos->setEnabled(false);
    ui->PosReset->setEnabled(false);
    ui->SetX->setValidator(new QDoubleValidator());
    ui->SetY->setValidator(new QDoubleValidator());
    ui->SetZ->setValidator(new QDoubleValidator());
    createActions();
    createContextMenu();
    init_serial();
}



//***************************电机控制部分************************************//
//***********************************************************
void MainWindow::init_serial(){  //查找可用串口
    ui->comNum->clear();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        ui->comNum->addItem(info.portName());
    }


}



void MainWindow::read_serial()//读取串口数据
{

}

void MainWindow::write_serial() //写串口
{

}

void MainWindow::x_chg() //x位置改变
{
    if(break_loop)
        return;
    Xpulse_cnt = (ui->XScrollBar->value()-x_coor)*10;
    if(Xpulse_cnt > 0){ //生成发送字符
        Xpulse_str = "X+" + QString::number(Xpulse_cnt);
    }else if (Xpulse_cnt < 0){
        Xpulse_str = "X-" + QString::number((0-Xpulse_cnt));
    }else
        return;

    qDebug()<<Xpulse_str;
    loc_flag = false;
    loc_pos();
//    if(serial->write(Xpulse_str.toLatin1()) != -1){ //串口是否写成功
//        qDebug()<< "x串口写成功";
//        x_coor = ui->XScrollBar->value();
//        x_str = QString::number(((double)x_coor)/100.0,'f',2);
//        ui->lcd_x->display(x_str);
//    }else{// 没成功scrollbar恢复原位
//        ui->XScrollBar->setValue(x_coor);
//    }



}

void MainWindow::y_chg()//y位置改变
{
    if(break_loop)
        return;
    Ypulse_cnt = (ui->YScrollBar->value()-y_coor)*10;

    if(Ypulse_cnt > 0){
        Ypulse_str = "Y+" + QString::number(Ypulse_cnt);
    }else if (Ypulse_cnt < 0){
        Ypulse_str = "Y-" + QString::number((0-Ypulse_cnt));
    }else
        return;

    qDebug()<<Ypulse_str;
    loc_flag = false;
    loc_pos();
//    if(serial->write(Ypulse_str.toLatin1()) != -1){
//        qDebug()<< "y串口写成功";
//        y_coor = ui->YScrollBar->value();
//        y_str = QString::number(((double)y_coor)/100.0,'f',2);
//        qDebug() << y_str;
//        ui->lcd_y->display(y_str);
//    }else{
//        ui->YScrollBar->setValue(y_coor);
//    }
}

void MainWindow::z_chg()//z位置改变

{
    if(break_loop){

        return;}

    Zpulse_cnt = (ui->ZScrollBar->value()-z_coor)*100;
    if(Zpulse_cnt > 0){
        Zpulse_str = "Z+" + QString::number(Zpulse_cnt);
    }else if (Zpulse_cnt < 0){
        Zpulse_str = "Z-" + QString::number((0-Zpulse_cnt));
    }else
        return;

    qDebug()<<Zpulse_str;
    loc_flag = false;
    loc_pos();
//    if(serial->write(pulse_str.toLatin1()) != -1){
//        qDebug()<< "z串口写成功"<<Zpulse_str.toLatin1();
//        z_coor = ui->ZScrollBar->value();
//        z_str = QString::number(((double)z_coor)/100.0,'f',2);
//        ui->lcd_z->display(z_str);
//    }else{
//        ui->ZScrollBar->setValue(z_coor);
//    }

}

void MainWindow::lcd_record()
{
    ui->RecordX->display(x_str);
    ui->RecordY->display(y_str);
    ui->RecordZ->display(z_str);
}

void MainWindow::loc_pos()
{   if(loc_flag){
        int x_pos = QString::number(ui->SetX->text().toDouble()*100,'f',0).toInt();
        int y_pos = QString::number(ui->SetY->text().toDouble()*100,'f',0).toInt();
        int z_pos = QString::number(ui->SetZ->text().toDouble()*100,'f',0).toInt();
        if(x_pos>=0 && x_pos <=36000 &&y_pos >= 0 && y_pos <= 30000
                && z_pos >=0 &&z_pos <= 20000){

            Xpulse_cnt = (x_pos-x_coor)*10;
            if(Xpulse_cnt > 0){ //生成发送字符
                Xpulse_str = "X+" + QString::number(Xpulse_cnt);
            }else if (Xpulse_cnt < 0){
                Xpulse_str = "X-" + QString::number((0-Xpulse_cnt));
            }else
                 Xpulse_str = "X+0";


            Ypulse_cnt = (y_pos-y_coor)*10;

            if(Ypulse_cnt > 0){
                Ypulse_str = "Y+" + QString::number(Ypulse_cnt);
            }else if (Ypulse_cnt < 0){
                Ypulse_str = "Y-" + QString::number((0-Ypulse_cnt));
            }else
                Ypulse_str = "Y+0";

            Zpulse_cnt = (z_pos-z_coor)*100;
            if(Zpulse_cnt > 0){
                Zpulse_str = "Z+" + QString::number(Zpulse_cnt);
            }else if (Zpulse_cnt < 0){
                Zpulse_str = "Z-" + QString::number((0-Zpulse_cnt));
            }else
               Zpulse_str = "Z+0" ;

            XYZ_str = "B"+Xpulse_str+Ypulse_str+Zpulse_str+"FF";


            if(serial->write(XYZ_str.toLatin1()) != -1){
                    qDebug()<< "三口写成功"<<XYZ_str.toLatin1();
                    break_loop = true;
                    ui->XScrollBar->setValue(x_pos);
                    ui->YScrollBar->setValue(y_pos);
                    ui->ZScrollBar->setValue(z_pos);

                    break_loop = false;
                    x_coor = ui->XScrollBar->value();
                    y_coor = ui->YScrollBar->value();
                    z_coor = ui->ZScrollBar->value();

                    x_str = QString::number(((double)x_coor)/100.0,'f',2);
                    ui->lcd_x->display(x_str);
                    y_str = QString::number(((double)y_coor)/100.0,'f',2);
                    ui->lcd_y->display(y_str);
                    z_str = QString::number(((double)z_coor)/100.0,'f',2);
                    ui->lcd_z->display(z_str);
               }else{QMessageBox::critical(this,"Critical Error",
                                           "Serial Error "
                                          ,QMessageBox::Ok);

            }

        }else{
            QMessageBox::critical(this,"Critical Error",
                                   "Set data error "
                                  ,QMessageBox::Ok);
        }

    }else{
        XYZ_str = "B"+Xpulse_str+Ypulse_str+Zpulse_str+"FF";

        if(serial->write(XYZ_str.toLatin1()) != -1){
                qDebug()<< "单口写成功"<<XYZ_str.toLatin1();
                x_coor = ui->XScrollBar->value();
                y_coor = ui->YScrollBar->value();
                z_coor = ui->ZScrollBar->value();

                x_str = QString::number(((double)x_coor)/100.0,'f',2);
                ui->lcd_x->display(x_str);
                y_str = QString::number(((double)y_coor)/100.0,'f',2);
                ui->lcd_y->display(y_str);
                z_str = QString::number(((double)z_coor)/100.0,'f',2);
                ui->lcd_z->display(z_str);
           }else{
                break_loop = true;
                ui->XScrollBar->setValue(x_coor);
                ui->YScrollBar->setValue(y_coor);
                ui->ZScrollBar->setValue(z_coor);
                break_loop = false;
            }



    }
    Xpulse_str = "X+0";
    Ypulse_str = "Y+0";
    Zpulse_str = "Z+0";
    loc_flag = true;


}

void MainWindow::reset()
{
    ui->SetX->setText("0");
    ui->SetY->setText("0");
    ui->SetZ->setText("0");
    loc_pos();
}





void MainWindow::open_serial() //打开串口
{
    if(openCom){
        if(set_serial()){
            ui->OpenComBut->setStyleSheet(
                        "background-image: url(:/new/background/resource/close.png);");
            ui->FindCom->setEnabled(false);
            ui->comNum->setEnabled(false);
            ui->XScrollBar->setEnabled(true);
            ui->YScrollBar->setEnabled(true);
            ui->ZScrollBar->setEnabled(true);
            ui->RecordPos->setEnabled(true);
            ui->SetPos->setEnabled(true);
            ui->PosReset->setEnabled(true);

        }

    }else{
        ui->OpenComBut->setStyleSheet(
                    "background-image: url(:/new/background/resource/open.png);");
        ui->FindCom->setEnabled(true);
        ui->comNum->setEnabled(true);
        ui->XScrollBar->setEnabled(false);
        ui->YScrollBar->setEnabled(false);
        ui->ZScrollBar->setEnabled(false);
        ui->RecordPos->setEnabled(false);
        ui->SetPos->setEnabled(false);
        ui->PosReset->setEnabled(false);
        close_serial();

    }

    openCom = !openCom;

}



void MainWindow::close_serial(){//关闭串口
    serial->close();
}

bool MainWindow::set_serial()//设置串口参数
{
    serial = new QSerialPort(ui->comNum->currentText());
    if(serial->open(QIODevice::ReadWrite)
            && serial->setBaudRate(QSerialPort::Baud9600)
            && serial->setDataBits(QSerialPort::Data8)
            && serial->setParity(QSerialPort::NoParity)
            && serial->setStopBits(QSerialPort::OneStop)
            && serial->setFlowControl(QSerialPort::NoFlowControl))
        return true;

    return false;


}








//***************************电机控制部分************************************//



//****************************(@*@)无敌分割线**********************************************//

//****************************(@*@)无敌分割线**********************************************//


//**********************************Cs200控制**********************************************开始//

//***********************************************************
//测量数据
void MainWindow::measure_start()
{

    if(!cancelFlag){
        speed_set();
        progress_display();
    }



}






//***********************************************************
//测量进度条
void MainWindow::progress_display(){
    int times = ui->times->value();
//    QProgressDialog process(this);
//    process.setWindowTitle("Start");
//    process.setLabelText("Measuring...");
//    process.setRange(0,times);
//    process.setModal(true);
//    process.setCancelButtonText("Cancel");
//    process.setVisible(true);
//    process.frameGeometry();
//    process.setValue(0);
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0,times);
    if(cancelFlag){

        return;
    }
    for(int i=1;i<=times;i++){
            if(cancelFlag){
                return;
            }

            if(currentRow >= 1001){
                QMessageBox::about(this,"About",
                            "No space to save data,Please clear table and restart!");
                break;
            }

            int result=single_measure();
            if(result !=0){
                 i--;
                 if(result == 2){
                     return;
                  }
            }
            if(cancelFlag) return;
            ui->progressBar->setValue(i);






    }

     ui->clear->setEnabled(true);
     ui->exportExcel->setEnabled(true);
     ui->isconnected->setEnabled(true);
     ui->times->setEnabled(true);
     ui->speed->setEnabled(true);
     ui->measure->setStyleSheet("background-image: url(:/new/background/resource/start.png);");
     cancelFlag = !cancelFlag;

}







//***********************************************************
//状态检测
void MainWindow::remote_state()

{
    if(openPipe!=0){//判断通道是否连接成功
        QMessageBox::critical(this,"Critical Error",
                               "Device is not connected"
                              ,QMessageBox::Ok);
        end_usb(0);
        openPipe=int_usb(0);
        return;


    }else{
        static bool state = true;
        if(state){
            if(int_usb(0)== -2){
                end_usb(0);
                openPipe=-1;
                QMessageBox::critical(this,"Critical Error",
                                  "Device is not connected"
                                  ,QMessageBox::Ok);
                return;
            }
            remote_connected();              //远程连接模式
            ui->isconnected->setStyleSheet("background-image: url(:/new/background/resource/disconnect.png);");
            ui->isconnected->setToolTip("Disconnect");

            state=!state;
        }else{
            remote_off();           //关闭远程模式
            ui->isconnected->setStyleSheet("background-image: url(:/new/background/resource/connect.png);");
            ui->isconnected->setToolTip("Connect");
            state = !state;
        }
    }

}








//***********************************************************
//远程模式连接
void  MainWindow::remote_connected()
{


        char cRemote[] = { "RMT,1\r\n" };
        char cBuf[250];

        write64_usb(0, cRemote, 1, 7);
        read64_usb(0, cBuf, 1, 250);



        ui->measure->setEnabled(true);




}







//***********************************************************
//断开远程模式
void MainWindow::remote_off()
{



    char cRemoteEnd[] = { "RMT,0\r\n" };//结束远程
    char cBuf[250];
    write64_usb(0, cRemoteEnd, 1, 7);
    read64_usb(0, cBuf, 1, 250);





    ui->measure->setEnabled(false);


}

//***********************************************************
//手动测试间隔设置

void MainWindow::speed_changed(int sec)
{
    if(sec==5){
        ui->manuset->setEnabled(true);

    }else
        ui->manuset->setEnabled(false);


}




//***********************************************************
//测量速度设定
void MainWindow::speed_set()
{
    char cBuf[250];
    int speed = ui->speed->currentIndex();
    QString speedStr =  QString::number(speed);
    int sec = ui->manuset->value();
    QString str="SPS,";
    str.append(speedStr);

    str.append(",");

    if(speed == 5){
        QString secStr = QString::number(sec);
        if(sec<10){
            str.append("0");
        }
        str.append(secStr);

    }else{
        str.append("00");
    }


    str.append("\r\n");

    char *cSps=str.toLatin1().data();




    write64_usb(0,cSps,1,10);
    read64_usb(0,cBuf,1,250);


    QString tmp(cBuf);
    if(tmp.left(4)=="OK00")
        return;
    else
        speed_set();

}








//***********************************************************
//非阻塞延时
void MainWindow::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

        while( QTime::currentTime() < dieTime )

        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

}









//***********************************************************
//单次测量设置
int MainWindow::single_measure()
{
    char cBuf[250];
    char cMes[] = { "MES,1\r\n" };
    char cMdrLvxy[] = { "MDR,0\r\n" };
    char cMdrLvuv[] = { "MDR,1\r\n" };
    char cMdrLvXYZ[] = { "MDR,3\r\n" };

    char cLvuv[250];
    char cLvxy[250];
    char cLvXYZ[250];
    QString strxy;
    QString struv;
    QString strXYZ;

    write64_usb(0, cMes, 1, 7);
    read64_usb(0, cBuf, 1, 250);
    QString strTime(cBuf);

    if(strTime.left(4)!="OK00")  //判断测量是否写入成功
        return 1;


    int nTime =strTime.mid(5, 2).toInt();
    int dwTime = (nTime * 1000 - 500);
    sleep(dwTime);

    int times=1;

    while(times--){

        write64_usb(0, cMdrLvxy, 1, 7); //Lv ,x,y
        read64_usb(0, cLvxy, 1, 250);
        strxy = QString(cLvxy);



        write64_usb(0, cMdrLvuv, 1, 7); //Lv,u,v
        read64_usb(0, cLvuv, 1, 250);
        struv = QString(cLvuv);



        write64_usb(0, cMdrLvXYZ, 1, 7); //X,Y,Z
        read64_usb(0, cLvXYZ, 1, 250);
        strXYZ = QString(cLvXYZ);

        if(struv.left(4) == "ER21"){
            QMessageBox::about(this,"Error",
            "Low Luminance");
            return 2;

        }




        if (struv.left(4) == "ER02"||
                strXYZ.left(4) =="ER02"|| strxy.left(4) =="ER02")
        {

            sleep(300);
            times++;
            continue;
        }




    }

    QString Lv = strxy.mid(27,11);
    QString x = strxy.mid(39,11);
    QString y = strxy.mid(51,11);
    QString u = struv.mid(39,11);
    QString v = struv.mid(51,11);
    QString X = strXYZ.mid(27,11);
    QString  Y = strXYZ.mid(39,11);
    QString  Z = strXYZ.mid(51,11);


    QString value[11] ={x_str,y_str,z_str,Lv,x,y,u,v,X,Y,Z};


    table_set(value);







    char cMesEnd[] = {"MES,0\r\n" };//测量结束
     write64_usb(0, cMesEnd, 1, 7);
     read64_usb(0, cBuf, 1, 250);
     return 0;
}



//**************************************************************************//
//*********************************************表格*************************//




//***********************************************************
//数据表格
void MainWindow::table_set(QString value[])

{
    QScrollBar *verticalScroll = ui->tableWidget->verticalScrollBar();



    if(currentRow>25&&currentRow<1001)
    verticalScroll->setValue(currentRow-25);
    else
        verticalScroll->setValue(0);

    ui->tableWidget->setItem(currentRow,0,new QTableWidgetItem(QString::number(currentRow+1)));
    for(int i=1;i<12;i++){
    ui->tableWidget->setItem(currentRow,i,new QTableWidgetItem(value[i-1]));

    }

    currentRow++;





}



//***********************************************************
//清除表格所有数据

void MainWindow::clearAll_clicked()
{

    ui->tableWidget->clear();
    currentRow=0;
    QStringList header;
    header<<"DataName"<<"X_Coor"<<"Y_Coor"<<"Z_Coor"<<"Lv"<<"x"<<"y"<<"u'"<<"v'"<<"X"<<"Y"<<"Z";
    ui->tableWidget->setHorizontalHeaderLabels(header);


}







//保存为Excel
void MainWindow::export_excel()
{

    table_excel(ui->tableWidget);

}



//***********************************************************
//导出excel
void MainWindow::table_excel(QTableWidget *table )
{
    QDateTime current_Date_Time = QDateTime::currentDateTime();
    QString title;
    title.append("Color=");
    title.append(ui->color->text());
    title.append("  Note=");
    title.append(ui->distance->text());
    title.append(" ");
    title.append(current_Date_Time.toString("yyyy-MM-dd hh:mm:ss"));




    qDebug()<<title<<endl;

    QString fileName = QFileDialog::getSaveFileName(table, "Save",
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
            "Excel(*.xlsx *.xls)");

    if (fileName!="")
    {
        QAxObject *excel = new QAxObject;
        if (excel->setControl("Excel.Application")) //连接Excel控件
        {
            excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
            excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
            QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
            workbooks->dynamicCall("Add");//新建工作簿
            QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);
            int i,j,colcount=table->columnCount();

            QAxObject *cell,*col;
            //标题行
            cell=worksheet->querySubObject("Cells(int,int)", 1, 1);
            cell->dynamicCall("SetValue(const QString&)", title);
            cell->querySubObject("Font")->setProperty("Size", 18);
            //调整行高
            worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
            //合并标题行
            QString cellTitle;
            cellTitle.append("A1:");
            cellTitle.append(QChar(colcount - 1 + 'A'));
            cellTitle.append(QString::number(1));
            QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);
            range->setProperty("WrapText", true);
            range->setProperty("MergeCells", true);
            range->setProperty("HorizontalAlignment", -4108);//xlCenter
            range->setProperty("VerticalAlignment", -4108);//xlCenter
            //列标题
            for(i=0;i<colcount;i++)
            {
                QString columnName;
                columnName.append(QChar(i  + 'A'));
                columnName.append(":");
                columnName.append(QChar(i + 'A'));
                col = worksheet->querySubObject("Columns(const QString&)", columnName);
                col->setProperty("ColumnWidth", table->columnWidth(i)/6);
                cell=worksheet->querySubObject("Cells(int,int)", 2, i+1);
                columnName=table->horizontalHeaderItem(i)->text();
                cell->dynamicCall("SetValue(const QString&)", columnName);
                cell->querySubObject("Font")->setProperty("Bold", true);
                cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                cell->setProperty("VerticalAlignment", -4108);//xlCenter
            }
            //数据区
           // for(i=0;i<table->rowCount();i++){
           for(i=0;i<currentRow+1;i++){
                for (j=0;j<colcount;j++)
                {
                    worksheet->querySubObject("Cells(int,int)", i+3, j+1)->dynamicCall("SetValue(const QString&)", table->item(i,j)?table->item(i,j)->text():"");
                }
            }
            //画框线
            QString lrange;
            lrange.append("A2:");
            lrange.append(colcount - 1 + 'A');
          //lrange.append(QString::number(table->rowCount() + 2));
            lrange.append(QString::number(currentRow+ 2));
            range = worksheet->querySubObject("Range(const QString&)", lrange);
            range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
            range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
            //调整数据区行高
            QString rowsName;
            rowsName.append("2:");
            //rowsName.append(QString::number(table->rowCount() + 2));
            rowsName.append(QString::number(currentRow+ 2));
            range = worksheet->querySubObject("Range(const QString&)", rowsName);
            range->setProperty("RowHeight", 20);
            workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至fileName
            workbook->dynamicCall("Close()");//关闭工作簿
            excel->dynamicCall("Quit()");//关闭excel
            delete excel;
            excel=NULL;
            if (QMessageBox::question(NULL,"Save","Save successful,Open it now?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            {
                QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
            }
        }
        else
        {
            QMessageBox::warning(NULL,"Error","Save failed,Please sure Office Excel be installed!",QMessageBox::Apply);
        }
    }
}


//***********************************************************
//加入右键选项
void MainWindow::createContextMenu()
{
    ui->tableWidget->addAction(copyAction);
    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::startORcancel()
{
    cancelFlag = !cancelFlag;
    if(!cancelFlag){
        ui->clear->setEnabled(false);
        ui->exportExcel->setEnabled(false);
        ui->isconnected->setEnabled(false);
        ui->times->setEnabled(false);
        ui->speed->setEnabled(false);
        ui->measure->setStyleSheet("background-image: url(:/new/background/resource/cancel.png);");
        measure_start();
    }else{
        ui->clear->setEnabled(true);
        ui->exportExcel->setEnabled(true);
         ui->isconnected->setEnabled(true);
         ui->times->setEnabled(true);
         ui->speed->setEnabled(true);
         ui->measure->setStyleSheet("background-image: url(:/new/background/resource/start.png);");
    }

}

void MainWindow::proTextDisplay()
{   int p =((ui->progressBar->value()*1.0)/(ui->times->value()*1.0))*100;
    qDebug()<<p;
    ui->progressText->setNum(p);
}



//***********************************************************
//增加copyAction
void MainWindow::createActions()
{
    copyAction = new QAction("&Copy",this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction,SIGNAL(triggered()),this,SLOT(copy()));
}

//***********************************************************
//获取选择范围
QTableWidgetSelectionRange MainWindow::selectedRange() const
{
    QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();

    if(ranges.isEmpty())
       return QTableWidgetSelectionRange();
   return ranges.first();

}





//***********************************************************
//复制数据到剪贴板

void MainWindow::copy()
{
    QTableWidgetSelectionRange range = selectedRange();
   QString str;

   for(int i=0;i<range.rowCount();++i){
        if(i>0)
          str += "\n";
        for(int j = 0;j<range.columnCount();++j){
            if(j > 0)
               str += "\t";
            str += formula(range.topRow() +i, range.leftColumn() + j);
        }


    }
    QApplication::clipboard()->setText(str);

}



Cell *MainWindow::cell(int row, int column) const
{
    return static_cast<Cell *>(ui->tableWidget->item(row, column));
}


//***********************************************************
//返回一个tableWidget的Item
QString MainWindow::formula(int row, int column) const
{
    Cell *c = cell(row, column);
    if (c) {
        return c->formula();
    } else {
        return "";
    }
}


//**********************************Cs200控制**********************************************结束//




//***********************************************************
//GUI关闭事件
void MainWindow::closeEvent(QCloseEvent *event)  //关闭Gui
{


      char cRemote[] = { "RMT,0\r\n" };
      char cBuf[250];

      write64_usb(0, cRemote, 1, 7);
      read64_usb(0, cBuf, 1, 250);

      char cMesEnd[] = { "MES,0\r\n" };//测量结束
      write64_usb(0, cMesEnd, 1, 7);
      read64_usb(0, cBuf, 1, 250);
      end_usb(0) ;
      qDebug() << "关闭窗口，程序结束" << endl;
    // FreeLibraryAndExitThread(hDll,0);
       FreeLibrary(hDll);

}



