#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTableWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "cell.h"

class Cell;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
     void closeEvent(QCloseEvent *event);
    ~MainWindow();

     void close_serial();
private slots:

    void clearAll_clicked();
    void measure_start();

    void remote_state();
    void startORcancel();
    void proTextDisplay();
    void export_excel();
    void copy();
    void speed_changed(int sec);
    void init_serial();
    void open_serial();
    void read_serial();
    void write_serial();
    void x_chg();
    void y_chg();
    void z_chg();
    void lcd_record();
    void loc_pos();//定位
    void reset();




private:
    Ui::MainWindow *ui;


    void init_setting();//初始化设置
    void remote_connected();//远程模式连接
    void remote_off(); //远程模式断开
    void speed_set();  //测量速度设定
    void sleep(int msec); //暂停
    int single_measure(); //单次测量
    void table_set(QString value[]);//表格设置
    void progress_display();//进度条
    void table_excel(QTableWidget *table);//导出excel
    void createContextMenu();



    void createActions();
    QTableWidgetSelectionRange selectedRange() const;
    QAction *copyAction;








private:
    Cell *cell(int row, int column) const;
    QString formula(int row, int column) const;
    QSerialPort *serial; //串口

    bool set_serial();
};




#endif // MAINWINDOW_H
