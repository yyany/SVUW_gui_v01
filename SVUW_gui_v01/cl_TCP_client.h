#ifndef CL_TCP_CLIENT_H
#define CL_TCP_CLIENT_H

// Подключение классов
//#include <QMainWindow>
#include <QTime>
#include <QImage>
#include <QTcpSocket>
#include <QTcpServer>
#include <QBuffer>


#include <cl_TCP_server.h>
#include <vector>


//#include <QtNetwork>



// Подключение ...
//#include <func_DenseStereo.h>

// Создаём класс TCPClient
//class TCPClient: public QTcpSocket
class TCPClient: public TCPServer
{

            Q_OBJECT // используем сигнально-слотовые связи поэтому прописываем Q_OBJECT

        public:
            TCPClient(); // создание конструктора

            void StartTcpClient(); //
            void CloseTcpClient(); //


            void SendToServer_ObjectDetectionCompressedImg(QImage); // Отправка сжатого изображения по TCP


        public slots:
            void slotReadyRead_C(); // Слот для сигнала - это обработчик для полученных от клиента сообшений
            bool ConnectToServer(QString hostIP_C, quint16 hostPort_C); // соединение с сервером
            bool TcpClientConnectToServerIsOpen();
            bool TcpClientConnectToServerisValid();
            void SendToServer(QString str_С); // Функции отправки сообщений на сервер
            void SendToServer_StateInformation(quint8, quint8, quint8, quint8, quint8);
            void SendToServer_StatusStateInformation(quint8);
            void SendToServer_MomentObjectDetection(quint8, double, std::vector<float> , float, std::vector<float> , quint8, QImage img);

            void SendToServer_ObjectDetectionImgFile(QString); // Отправка файла по TCP





        private:
            QTcpSocket *socket_C;

            QByteArray Data_C; // Все данные передаются через QByteArray. Создаём объект Data
            QByteArray Data_C_StateInfo_G; // Все данные передаются через QByteArray. Создаём объект Data
            QByteArray Data_C_StatusStateInfo_G; // Все данные передаются через QByteArray. Создаём объект Data
            QByteArray Data_C_MomentObjDet_G; // Все данные передаются через QByteArray. Создаём объект Data
            QByteArray Data_C_ObjDetImgFile_G; // Все данные передаются через QByteArray. Создаём объект Data
            QByteArray Data_C_ObjDetCompresImg_G; // Все данные передаются через QByteArray. Создаём объект Data


            quint16 nextBlockSize_C = 0;          // Переменная для храниения размера блока для передачи больших по размеру данных
            QString hostIP_C = "192.168.56.1";   // Переменная для храниения ip
            quint16 hostPort_C = 10099;        // Переменная для храниения port  10099, 2323


        signals:

            void signal_sendText_plainTextEdit_StateInfo(QString); // сигнал для отправки текста на главную форму


};

#endif // CL_TCP_CLIENT_H
