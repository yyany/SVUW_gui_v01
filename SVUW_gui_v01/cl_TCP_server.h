#ifndef CL_TCP_SERVER_H
#define CL_TCP_SERVER_H





// Подключение классов
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>


// Поключение класса для вектора для сокетов
#include <QVector>
#include <QFileInfo>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>




#if defined(_WIN64)


// ================
// Only Windows
// ================
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
//#include <windows.h>  // **** возможно лишнее ****
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





//#include "func_main.h"

// Подключение ...
//#include <func_DenseStereo.h>

// Создаём класс сервер и унаследуем его от класса QTcpServer
class TCPServer: public  QTcpServer
{

    Q_OBJECT // используем сигнально-слотовые связи поэтому прописываем Q_OBJECT

        public:
            TCPServer(); // создание конструктора
            void StartTcpServer();
            void StopTcpServer();



            // ============================
            // Переменные для хранения полученных данных
            // ============================

            std::vector<double> GeographicCoordinates_S {0.0, 0.0}; // Текущие географические координаты АНПА (широта, долгота)
            double HardwareTime_S;                              // Аппаратное время*
            float FloorDistance_S;                              // Отстояние от дна
            float RollANPA_S;                                   // Крен АНПА
            float TrimANPA_S;                                   // Дифферент
            std::vector<float> AngularVelocity_S {0.0f, 0.0f, 0.0f};     // Угловая скорость АНПА
            std::vector<float> ApparentAcceleration_S {0.0f, 0.0f, 0.0f};// Кажущееся ускорение АНПА
            float AbsoluteVelocity_S;                           // Абсолютная скорость

            quint8 G_StartCommand_S = 0;  // Строка с командой на старт программы




            QTcpSocket *socket_S; // создаём объект класса QTcpSocket
    // Сервер будет создавать сокет для каждого нового подключения
    // Все сокеты будут записаны в вектор
        private:
            QVector <QTcpSocket*> Sockets_S; // создаём ветор для сокетов
            QByteArray Data_S; // Все данные передаются через QByteArray. Создаём объект Data
            quint32 nextBlockSize_S = 0;          // Переменная для храниения размера блока

            // QString hostIP_S = "192.168.56.1";   // Переменная для храниения ip
           // QString hostIP_S = QHostAddress::Any;   // Переменная для храниения ip
            quint16 hostPort_S = 10098;        // Переменная для храниения port  10099, 2323


            // Фунция для передачи данных клиенту
            void SendToClient_S(QString str);


        public slots:
            void incomingConnection(qintptr socketDescriptor); // Слот для подключения
            void slotReadyRead_S(); // Слот для сигнала - это обработчик для полученных от клиента сообшений

            void setHostPort_S(int); // назначение порта, который прослушивает сервер

            // Слот получения с form01_mainSettings ссылок на объект класса и ссылки на стуктуру параметров настройки
           // void slot_Form01ToTcpServer(ADS::cl_DenseStereo, str_Settings);



        signals:


            // Сигнал на форму об получении данных
             void signal_TcpServetToForm_Receive();

            // Сигнал от TCP сервера к главной форме для старта программы по команде от стороннего TCP клиента
            void signal_SendStartCommand();
             // Сигнал для отправки TCP сообщения о готовности к работе
            void signal_Send_InitialStateInformation();



            void signal_sendText_plainTextEdit_StateInfo(QString text); // сигнал для отправки текста на главную форму

};

#endif // CL_TCP_SERVER_H
