#include "mainwindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //a.setStyle("windowsvista"); // метод установки стилей

     MainWindow w;

    // =================================
    // Установка палитры цветов
    // =================================
     QPalette m_pal; // создаём объект палитры



//     cblue = [0, 0.4470, 0.7410];
//     corange = [0.8500, 0.3250, 0.0980];
//     cyellow = [0.9290, 0.6940, 0.1250];
//     cmagenta = [0.4940, 0.1840, 0.5560];
//     cgreen = [0.4660, 0.6740, 0.1880];
//     cblue = [0.3010, 0.7450, 0.9330];
//     cred = [0.6350, 0.0780, 0.1840];

//     clrA[0] = {47,      20,     162};    //  красный
//     clrA[1] = { 25,     83,     216};    // оранжевый
//     clrA[2] = { 32,     177,    237};    //  желтый
//     clrA[3] = { 48,     172,    119};    // зеленый
//     clrA[4] = { 238,	  190,     77};    // голубой
//     clrA[5] = { 189,    114,      0};    // синий
//     clrA[6] = { 142,    47,     126};    // фиолетовый
//     clrA[7] = { 0,       0,       0};    // чернота



     // Создаём метод и в него передаём: (группу, роль, цвет)  Qt::red или QColor(216, 83, 25)

     //m_pal.setColor(QPalette::Active, QPalette::Window, Qt::red  );
     m_pal.setBrush(QPalette::Active, QPalette::Window, QBrush(QPixmap("./files/Logo/fone.jpg")));

     m_pal.setColor(QPalette::Active, QPalette::Button, Qt::green );

//     m_pal.setColor(QPalette::Active, QPalette::ButtonText, QColor(112, 40, 7) );
////     m_pal.setColor(QPalette::Active, QPalette::HighlightedText, QColor(162, 20, 47) );
////     m_pal.setColor(QPalette::Active, QPalette::WindowText, QColor(216, 83, 25) );

//     m_pal.setColor(QPalette::Active, QPalette::Text, QColor(30, 92, 14) );
     m_pal.setColor(QPalette::Active, QPalette::Base, QColor(240, 240, 240) );




     w.setPalette(m_pal);  // Установка палитры в виджет
     w.setAutoFillBackground(true);  // Если не сработало то ...
//     w.update();




    w.show();
    //w.hide();
    return a.exec();
}
