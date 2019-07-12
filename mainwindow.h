#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qstring.h>

#include <motor.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void logMsg(QString);


private:
    Ui::MainWindow *ui;

    Motor motor1_;
    Motor motor2_;
    Motor motor3_;
    Motor motor4_;
    Motor motor5_;
    Motor motor6_;


};

#endif // MAINWINDOW_H
