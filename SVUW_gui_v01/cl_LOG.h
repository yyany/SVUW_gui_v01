#ifndef CL_LOG_H
#define CL_LOG_H


// function
// =======
#include "func_main.h"
// ======

// Подключение классов
//#include <QMainWindow>
#include <QWidget>
#include <QObject>

// Подключение классов
#include <QTime>
#include <QDateTime>
// Поключение класса для вектора для сокетов
#include <QVector>
// Подключение стандартных библиотек
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdlib.h>




#if defined(_WIN64)


// ================
// Only Windows
// ================
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <windows.h>
#include <direct.h>
#include <time.h>
#include <ppl.h>

// ================
// Only Windows END
// ================

#else

// ================
// Only Linux
// ================

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <stdlib.h>

// ================
// Only Linux END
// ================

#endif








using namespace std;

    // Создаём класс TCPClient
   class cl_LOG: public QObject
//    class cl_LOG: public QWidget
{

    Q_OBJECT // используем сигнально-слотовые связи поэтому прописываем Q_OBJECT

        public:
                 cl_LOG(); // создание конструктора


public slots:

    void slot_MainFormToLOG(ADS::cl_DenseStereo* obj_ptr, str_Settings* settings_ptr); // Слот для принятия данных с главной формы
    void slot_MainFormToLOG_AirCraftData(            // Слот для принятия данных с AirCraft
        int counter_AirCraft,
        double x_AirCraft,
        double y_AirCraft,
        double z_AirCraft,
        double x_GlidePath,
        double y_GlidePath,
        double z_GlidePath,
        double dx_AirCraft,
        double dy_AirCraft,
        double dz_AirCraft,
        double ro_AirCraft,
        double theta_AirCraft,
        double phi_AirCraft,
        double ro_GlidePath,
        double theta_GlidePath,
        double phi_GlidePath,
        double dro_AirCraft,
        double dtheta_AirCraft,
        double dphi_AirCraft);
    void WiriteLogAirCraft3DCoordinates(); // создание и запись лог-файла



private:

       // AirCraft
        int counter_AirCraft;        // переменная для номера самёта (если по z произошел большой разрыв)
        double x_AirCraft;          // Координата самолёта по x
        double y_AirCraft;          // Координата самолёта по y
        double z_AirCraft;          // Координата самолёта по z
        double x_GlidePath;         // Координата глисcады по x
        double y_GlidePath;         // Координата глиcсады по y
        double z_GlidePath;         // Координата глисcады по z
        double dx_AirCraft;         // Отклонение самолёто от глисcады по x
        double dy_AirCraft;         // Отклонение самолёто от глисcады по y
        double dz_AirCraft;         // Отклонение самолёто от глисcады по z
        double ro_AirCraft;         // радиус AirCraft
        double theta_AirCraft;      // полярный угол AirCraft
        double phi_AirCraft;        // азимут  AirCraft
        double ro_GlidePath;        // радиус глисcады
        double theta_GlidePath;     // полярный угол глисcады
        double phi_GlidePath;       // азимут глисcады
        double dro_AirCraft;        // Отклонение самолёто от глисcады по радиусу
        double dtheta_AirCraft;     // Отклонение самолёто от глисcады по полярному углу
        double dphi_AirCraft;       // Отклонение самолёто от глисcады по азимуту



};




#endif // CL_LOG_H
