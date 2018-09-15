#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

////////////////////////////////////////////////////////////
///////////////////////CRYPT BUTTON/////////////////////////
////////////////////////////////////////////////////////////

void MainWindow::on_cryptButton_clicked()
{  
    ui->cryptedText->clear();
    if(ui->langBox->currentText()=="English"){
        std::vector<QString> englang = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
        crypt(englang); }
    if(ui->langBox->currentText()=="Russian"){
        std::vector<QString> ruslang = {"а","б","в","г","д","е","ё","ж","з","и","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ъ","ы","ь","э","ю","я"};
        crypt(ruslang);
    }
}
void MainWindow::on_decryptButton_clicked()
{
    ui->cryptedText->clear();
    if(ui->langBox->currentText()=="English"){
        std::vector<QString> englang = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
        decrypt(englang); }
    if(ui->langBox->currentText()=="Russian"){
        std::vector<QString> ruslang = {"а","б","в","г","д","е","ё","ж","з","и","й","к","л","м","н","о","п","р","с","т","у","ф","х","ц","ч","ш","щ","ъ","ы","ь","э","ю","я"};
        decrypt(ruslang);
    }
}

///////////////////////////////////////////////////////////
////////////////////////CRYPT FUNCS////////////////////////
///////////////////////////////////////////////////////////

void MainWindow::crypt(std::vector<QString>& lang)          //шифровка
{
    for (QChar origSymbol : ui->originalText->toPlainText())
    {
        unsigned int index = 0;
        for(QString cryptSymbol: lang)
        {
            if(origSymbol==cryptSymbol)
            {
                unsigned int shift = index + unsigned(ui->key->text().toInt());
                while(shift>=lang.size())
                    shift-=lang.size();
                ui->cryptedText->insertPlainText(lang[shift]);
                break;
            }
            if(index==lang.size()-1)                            //случай для заглавных букв
                {
                    index=0;
                    QLocale locale;
                    QString capitalSymbol=locale.toLower(origSymbol);
                    for(QString cryptCapitalSymbol: lang)
                    {
                        if(cryptCapitalSymbol==capitalSymbol){
                            unsigned int shift = index + unsigned(ui->key->text().toInt());
                            while(shift>=lang.size())
                                shift-=lang.size();
                            ui->cryptedText->insertPlainText(locale.toUpper(lang[shift]));
                            break;
                        }
                        if(index==lang.size()-1)
                        {
                            ui->cryptedText->insertPlainText(origSymbol);
                            break;
                        }
                        index++;
                    }
                }
            index++;
        }
    }
}
void MainWindow::decrypt(std::vector<QString>& lang)        //дешифровка
{
    for (QChar origSymbol : ui->originalText->toPlainText())
    {
        int index = 0;
        for(QString cryptSymbol: lang)
        {
            if(origSymbol==cryptSymbol)
            {
                int shift = index - ui->key->text().toInt();
                while(shift<0)
                    shift+=lang.size();
                ui->cryptedText->insertPlainText(lang[unsigned(shift)]);
                break;
            }
            if(index==int(lang.size()-1))                            //случай для заглавных букв
                {
                    index=0;
                    QLocale locale;
                    QString capitalSymbol=locale.toLower(origSymbol);
                    for(QString cryptCapitalSymbol: lang)
                    {
                        if(cryptCapitalSymbol==capitalSymbol){
                            int shift = index - ui->key->text().toInt();
                            while(shift<0)
                                shift+=lang.size();
                            ui->cryptedText->insertPlainText(locale.toUpper(lang[unsigned(shift)]));
                            break;
                        }
                        if(index==int(lang.size())-1)
                        {
                            ui->cryptedText->insertPlainText(origSymbol);
                            break;
                        }
                        index++;
                    }
                }
            index++;
        }
    }
}
void MainWindow::on_invertBox_stateChanged(int arg1)        //инверт чекбокс
{
    QString str = "";
    for(QChar a:ui->cryptedText->toPlainText())             //парсинг по textEdit
        {
        str.push_front(a);                                  //в str каждый раз буква "a" записывается в начало строки(инверт)
        }
    ui->cryptedText->setPlainText(str);                     //заменяется текст в textEdit на str
}
void MainWindow::on_largeBox_stateChanged(int arg1)         //заглавных букв чекбокс
{
    QLocale locale;
    if(arg1)
        ui->cryptedText->setText(locale.toUpper(ui->cryptedText->toPlainText()));
    else
        ui->cryptedText->setText(locale.toLower(ui->cryptedText->toPlainText()));
}
void MainWindow::on_binBox_stateChanged(int arg1)           //перевод в и из двоич. с.с.
{
    if (arg1 == 2 && ui->langBox->currentText()=="English")
        {
            QString str1 = ui->originalText->toPlainText().toLocal8Bit();
            QString result = "";

            QByteArray ba = str1.toLatin1();
            const char *str2 = ba.data();

            for(int i = 0; i < str1.count(); i++)
            {
                static const int mask[] = { 1, 2, 4, 8, 16, 32, 64, 128 };
                int j = 8;
                while( j-- )
                {
                    result.insert(result.count(),(int(str2[i]) & mask[j] ? '1' : '0'));
                }
                result.insert(result.count()," ");
            }
            ui->cryptedText->setText(result);
        }
    else if (arg1 == 0 && ui->langBox->currentText()=="English")
        {
        QString str1 = ui->originalText->toPlainText(); //получаем исходный текст в str1
        str1.remove(QChar(' '), Qt::CaseInsensitive);   //убирает пробелы из полученного текста str1
        QString eightBits = "";
        QString result = "";                            //строка с результатом перевода из дв. сс. в строку
        while(str1.count()) {

        if(str1.count()>=8) {
            eightBits = str1.left(8);                   //отрезает 8 битов слева
            str1.remove(0,8); }                         //удаляет 8 битов
        if(str1.count()<8)
        {
            for(;str1.count();){
               eightBits = str1.left(1);
               str1.remove(0,1);
            }
        }

        eightBits=eightBits.toLocal8Bit();              //конв.
        QByteArray ba = eightBits.toLatin1();           //конв.

        const char *str2 = ba.data();                   //char строка


        char digits[8];
        digits[0] = str2[0];
        digits[1] = str2[1];
        digits[2] = str2[2];
        digits[3] = str2[3];
        digits[4] = str2[4];
        digits[5] = str2[5];
        digits[6] = str2[6];
        digits[7] = str2[7];

        int mask[8] = {1, 2, 4, 8, 16, 32, 64, 128};
        char ch = 0;

        for(int i=0; i<8; i++)
            {
                ch += (digits[7-i]-48) * mask[i];
            }
        result.insert(result.count(),ch);
        }
        ui->cryptedText->setText(result);
    }
    else
        QMessageBox::warning(this, "Error #zero", "You can not use languages other than English with binary conversion.");
}
void MainWindow::on_swapButton_clicked()                    //поменять местами текст
{
    QString str1 = ui->cryptedText->toPlainText();
    ui->cryptedText->setText(ui->originalText->toPlainText());
    ui->originalText->setText(str1);
}

void MainWindow::on_openButton_clicked()
{
    QFile file( QFileDialog::getOpenFileName(this,tr("Open Text"), "file:///C:/", tr("Text file (*.txt)") ));  //Создает файл с урл, которое выберет пользователь в диалоговом окне, первый параметр заголовок, второй нач. директория, третий доступные типы.
    ui->originalText->clear();                                                                                  //очищает левый textEdit
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))                                                       //???(открыть файл для чтения и получить текст?)
    {
        QTextStream stream (&file);                                                                     //передает текст в файле в поток
        while(!stream.atEnd())                                                                          //пока поток не закончился
            ui->originalText->setText(ui->originalText->toPlainText()+stream.readLine()+"\n");          //берет строку из потока и вписывает ее в левый textEdit
    }
}

void MainWindow::on_exportButton_clicked()
{
    QFile file( QFileDialog::getSaveFileName(this,tr("Save Text"), "file:///C:/untitled.txt", tr("Text file (*.txt)") ));  //Создает файл с урл, которое выберет пользователь в диалоговом окне, первый параметр заголовок, второй нач. директория, третий доступные типы.                                                                                 //очищает левый textEdit
    file.open(QIODevice::ReadOnly | QIODevice::Text);                                                       //???(открыть файл для чтения и получить текст?)

        QTextStream expStream (&file);                                                                     //передает текст в файле в поток
        QString str = ui->cryptedText->toPlainText();
        QTextStream origStream (&str);
        while(!origStream.atEnd())
            expStream<<origStream.readLine()+"\n";

}
