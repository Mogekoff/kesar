#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow                   //объявление класса
{
    Q_OBJECT                                            //макрос для Qt

public:                                                 //public:
    ~MainWindow();                                      //прототип деструктора
    explicit MainWindow(QWidget *parent = nullptr);     //прототип явного конструктора
private slots:                                          //слоты

    void on_cryptButton_clicked();                      //по клику кнопки crypt

    void on_decryptButton_clicked();

    void on_invertBox_stateChanged(int arg1);

    void on_largeBox_stateChanged(int arg1);



    void on_binBox_stateChanged(int arg1);

    void on_swapButton_clicked();

    void on_openButton_clicked();

    void on_exportButton_clicked();

private:                                                //private
    void crypt(std::vector<QString>& lang);             //функция crypt
    void decrypt(std::vector<QString>& lang);           //функция decrypt
    Ui::MainWindow *ui;                                 //указатель на Ui

};

#endif // MAINWINDOW_H


