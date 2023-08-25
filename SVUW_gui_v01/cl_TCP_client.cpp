# include <cl_TCP_client.h>

TCPClient::TCPClient()
{
   // В оригенале тут код метода StartTcpClient
   nextBlockSize_C = 0;



   // =================================
   // Соеденение сигнала и слота для передачи на ui из TCP_Server objTCPServer принятых сервером данных
   // =================================
   // код типа сообщения
   // код типа сообщения
   // 11 - код получения сообщения о готовности к работе
   // 22  - код получения сообщения о статусе успешной работы
   // 33 - код получения сообщения о моменте обнаружения объекта
   // 77 - код получения сообщения изображения (через файл)
   // 88  - код получения сообщения изображения (через бувер обмена)


   // 111  - код сообщения о команды управления включением работы
   // 122  - код сообщения с содержанием информации от АНПА



}

// ============================
// Функция старта клиента
// ============================
void TCPClient::StartTcpClient() // >>>>> public slots:
{


   qDebug() << "StartTcpClient():: START ..........  " ;



     socket_C = new QTcpSocket;    // содание нового сокета

    QObject::connect(socket_C, &QTcpSocket::readyRead, this, &TCPClient::slotReadyRead_C); // объединение сигналов readyRead со слотом slotReadyRead
    // QObject::connect(socket_C, SIGNAL(&QTcpSocket::connected()), this, SLOT(&TCPClient::SendToServer()));
    QObject::connect(socket_C, &QTcpSocket::disconnected, socket_C, &QTcpSocket::deleteLater); // соединения сигнала disconnected с фукцией deleteLater



        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "StartTcpClient:: Host:: " + hostIP_C + "::" + QString::number(hostPort_C) ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>>>


//   qDebug() << "StartTcpClient()::socket_C->state():: " << socket_C->state();
//   qDebug() << "StartTcpClient()::socket_C->isValid:: " << socket_C->isValid();
//   qDebug() << "StartTcpClient()::socket_C->isOpen():: " << socket_C->isOpen();


//   if (socket_C->isOpen() )
//{



//}


}

void TCPClient::CloseTcpClient()
{

    if ( socket_C->isValid() )
    {

        socket_C->close();              // закрыти соединения
        socket_C->destroyed(NULL);      // удаление сокета

    }

    qDebug() << "CloseTcpClient()::socket_C->state():: " << socket_C->state();;

}


// ============================
// Функция подключения к серверу
// ============================
bool TCPClient::ConnectToServer(QString hostIP_C, quint16 hostPort_C) // >>>>> public:
{


//    qDebug() << "TCPClient::ConnectToServer()::socket_C->isOpen()::(1):: " << socket_C->isOpen();
//    qDebug() << "TCPClient::ConnectToServer()::socket_C->isValid()::(1):: " << socket_C->isValid();
//    qDebug() << "TCPClient::ConnectToServer()::socket_C->state()::(1):: " << socket_C->state();


//    qDebug() << "TCPClient::ConnectToServer::hostIP_C:: " << hostIP_C;
//    qDebug() << "TCPClient::ConnectToServer::hostPort_C:: " << hostPort_C;



    socket_C->connectToHost(hostIP_C,hostPort_C);

//    qDebug() << "TCPClient::ConnectToServer():: ok !!!!!!!!!!";


//    qDebug() << "TCPClient::ConnectToServer()::socket_C->isOpen()::(2):: " << socket_C->isOpen();
//    qDebug() << "TCPClient::ConnectToServer()::socket_C->isValid()::(2):: " << socket_C->isValid();
//    qDebug() << "TCPClient::ConnectToServer()::socket_C->state()::(2):: " << socket_C->state();




//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//    emit signal_sendText_plainTextEdit_StateInfo( "TCPClient::connectToHost:: " + hostIP_C + "::" + QString::number(hostPort_C) ); // отправка текста в информационное окно
//    // >>>>>>>>>>>>>>>>>>>>>>


    return socket_C->isOpen();

}

// ============================
// Функция
// ============================
bool TCPClient::TcpClientConnectToServerIsOpen() // >>>>> public:
{

    if (!socket_C->isOpen())
    {
        qDebug() << "TCPClient::TcpClientConnectToServerIsOpen:: ok";
    }

    qDebug() << "TcpClientConnectToServerIsOpen()::socket_C->state():: " << socket_C->state();
    return socket_C->isOpen();

}



// ============================
// Функция
// ============================
bool TCPClient::TcpClientConnectToServerisValid() // >>>>> public:
{

// проверка сокета на дееспособность socket_C->isValid()
    qDebug() << "TcpClientConnectToServerisValid():: socket_C->isValid(...):: " << socket_C->isValid();
    qDebug() << "TcpClientConnectToServerisValid():: socket_C->state():: " << socket_C->state();

    return socket_C->isValid();

}


// ============================
// Функции отправки сообщений на сервер
// ============================
void TCPClient::SendToServer(QString str_C)    // >>>>> public:
{

    qDebug() << "TCPClient::SendToServer:: start ";

    Data_C.clear();
    QDataStream out_C(&Data_C, QIODevice::WriteOnly); // Только запись иначе ничего не получиться


    out_C.setVersion(QDataStream::Qt_6_2);

    //        // ============================
    //         // Для много пакетной передачи
    //        // ============================

    // Записываем размер блока в самое начало quint32(0).
    // Присваеваем 0 так как не знаем размер блока.
    // Главное занимает размер 2 байта
    // ============================
//    out_C << quint32(0) << str_C;
//    out_C.device()->seek(0);  // преход в начало сообщения
//    out_C << quint32 (Data_C.size() - sizeof(quint32)); // ввод размера сообщения
    // ============================

    out_C << str_C;
    // ============================


   qint64 status = socket_C->write(Data_C);
    //socket_C->flush();  // ?? обрыв сообшения


    qDebug() << "TCPClient::str_C:: " << str_C;
    qDebug() << "TCPClient::Data_C:: " << Data_C;
    qDebug() << "TCPClient::status:: " << status;
    qDebug() << "TCPClient::SendToServer:: end ";

}



// ==============================
// SendToServer_StateInformation
// ==============================

void TCPClient::SendToServer_StateInformation(quint8 q1,    // SecondaryPowerSupply,  // Наличие вторичного электропитания отсека СТЗ
                                              quint8 q2,    // ConnectToCams,         // Наличие соединения с камерами
                                              quint8 q3,    // ImageAquisition,       // Получение изображения камерами
                                              quint8 q4,    // SuccessfulWork         // Готовность к работе регулятора яркости светильника отсека
                                              quint8 q5)    // FuctSuccessfulWork     // факт успешной работы
{

    qDebug() << "TCPClient::SendToServer_StateInformation:: start ";

    Data_C_StateInfo_G.clear();
    QDataStream out_C(&Data_C_StateInfo_G, QIODevice::WriteOnly); // Только запись иначе ничего не получиться


    out_C.setVersion(QDataStream::Qt_6_2);

    //        // ============================
    //         // Для много пакетной передачи
    //        // ============================
    // Записываем размер блока в самое начало quint32(0).
    // Присваеваем 0 так как не знаем размер блока.
    // Главное занимает размер 2 байта

    // ============================
    //    out_C << quint32(0) << str_C;
    //    out_C.device()->seek(0);  // преход в начало сообщения
    //    out_C << quint32 (Data_C.size() - sizeof(quint32)); // ввод размера сообщения
    // ============================
    // Заголовки и концовки пакета
    quint8  b1 = 170;         // байты начала сообщения
    quint8  b2 = 11;    // новое значение - код типа сообщения  // <<<
    // quint8  b2 = 187; // старое значение   // <<<


    quint8  e1 = 0;
    quint8  e2 = 254;        // байты конца сообщения

// Bh (11d) – код сообщения о готовности к работе для пакета от СТЗ в БПУ
//    quint8  id = 11;     // код типа сообщения   // <<<
//    quint32 BlockSize =  0;  // Размер сообщения   // <<<


    out_C << b1 << b2
//          << id          // <<<
//          << quint32(0)  // <<<
          << q1 << q2 << q3 << q4 << q5
          << e1 << e2;


//    out_C.device()->seek(3);  // преход в начало сообщения  // <<<
//    out_C << quint32 (Data_C_StateInfo_G.size());  // Занесение размера сообщения   // <<<


    // ============================


    qint64 status = socket_C->write(Data_C_StateInfo_G);
//    socket_C->waitForDisconnected(10);
//   socket_C->close();
//    socket_C->flush();  // ?? обрыв сообшения
//    socket_C->disconnected();


//    qDebug() << "TCPClient::SendToServer_StateInformation::Data_C:: " << Data_C;
//    qDebug() << "TCPClient::SendToServer_StateInformation::status:: " << status;
//    qDebug() << "TCPClient::SendToServer_StateInformation::SendToServer:: end ";


}

// ==============================
// SendToServer_StatusStateInformation
// ==============================


void TCPClient::SendToServer_StatusStateInformation(quint8 q1)
{

   // qDebug() << "TCPClient::SendToServer_StatusStateInformation:: start ";

    Data_C_StatusStateInfo_G.clear();
    QDataStream out_C(&Data_C_StatusStateInfo_G, QIODevice::WriteOnly); // Только запись иначе ничего не получиться


    out_C.setVersion(QDataStream::Qt_6_2);

    //        // ============================
    //         // Для много пакетной передачи
    //        // ============================
    // Записываем размер блока в самое начало quint32(0).
    // Присваеваем 0 так как не знаем размер блока.
    // Главное занимает размер 2 байта

    // ============================
    //    out_C << quint32(0) << str_C;
    //    out_C.device()->seek(0);  // преход в начало сообщения
    //    out_C << quint32 (Data_C.size() - sizeof(quint32)); // ввод размера сообщения
    // ============================
    // Заголовки и концовки пакета
    quint8  b1 = 170;   // байты начала сообщения
    quint8  b2 = 12;    // новое значение - код типа сообщения  // <<<
    // quint8  b2 = 187; // старое значение   // <<<

    quint8  e1 = 0;
    quint8  e2 = 254;   // байты конца сообщения

    // Сh (12d) – код сообщения о статусе успешной работы для пакета от СТЗ в БПУ
    //    quint8  id = 12;     // код типа сообщения   // <<<
    //    quint32 BlockSize =  0;  // Размер сообщения   // <<<


    out_C << b1 << b2
          //          << id          // <<<
          //          << quint32(0)  // <<<
          << q1
          << e1 << e2;


//    out_C.device()->seek(3);  // преход в начало сообщения  // <<<
//    BlockSize = quint32 (Data_C_StatusStateInfo_G.size());  // <<<
//    out_C << BlockSize;  // Занесение размера сообщения      // <<<

    // ============================


    qint64 status = socket_C->write(Data_C_StatusStateInfo_G);
//    socket_C->waitForDisconnected(10);
    // socket_C->close();
    // socket_C->flush();  // ?? обрыв сообшения
    // socket_C->disconnected();


//    qDebug() << "TCPClient::SendToServer_StatusStateInformation::Data_C:: " << Data_C;
//    qDebug() << "TCPClient::SendToServer_StatusStateInformation::status:: " << status;
//    qDebug() << "TCPClient::SendToServer_StatusStateInformation::SendToServer:: end ";

}

// ==============================
// SendToServer_MomentOjectDetection
// ==============================
void TCPClient::SendToServer_MomentObjectDetection(
                                quint8 FuctOjectDetection,                    // факт обнаружения объекта
                                double Time,                                  // время
                                std::vector<float> GeographicalCoordinates,   // географические координаты
                                float ObjectDistance,                         // дальность до объекта
                                std::vector<float> LinearCoordinates,         // линейные координаты
                                quint8 ContouringCompletion,                   // отчёт о завершении оконтуривания
                                QImage img)                                             // кадр оъекта

{

    // qDebug() << "TCPClient::SendToServer::MomentOjectDetection:: start ";

    Data_C_MomentObjDet_G.clear();
    QDataStream out_C(&Data_C_MomentObjDet_G, QIODevice::WriteOnly); // Только запись иначе ничего не получиться


    out_C.setVersion(QDataStream::Qt_6_2);

    //        // ============================
    //         // Для много пакетной передачи
    //        // ============================
    // Записываем размер блока в самое начало quint32(0).
    // Присваеваем 0 так как не знаем размер блока.
    // Главное занимает размер 2 байта

    // ============================
    //    out_C << quint32(0) << str_C;
    //    out_C.device()->seek(0);  // преход в начало сообщения
    //    out_C << quint32 (Data_C.size() - sizeof(quint32)); // ввод размера сообщения
    // ============================

    // Заголовки и концовки пакета
    quint8  b1 = 170;  // байты начала сообщения
    quint8  b2 = 13;    // новое значение - код типа сообщения  // <<<
    // quint8  b2 = 187; // старое значение   // <<<

    quint8  e1 = 0;
    quint8  e2 = 254; // байты конца сообщения

    // Dh (13d) – код сообщения о моменте обнаружения объекта для пакета от СТЗ в БПУ
    //    quint8  id = 13;     // код типа сообщения   // <<<
    //    quint32 BlockSize =  0;  // Размер сообщения   // <<<

    QByteArray ba;                          // <<<
    QBuffer buffer(&ba);                    // <<<
    buffer.open(QIODevice::WriteOnly);      // <<<


    if (FuctOjectDetection != 0)
    {
        img.save(&buffer, "JPG", 10); // writes image into ba in PNG format // <<<
    }
    else
    {
        ba = 0;
    }



    out_C << b1 << b2
          //          << id          // <<<
          //          << quint32(0)  // <<<
          //<< QTime::currentTime()
          << FuctOjectDetection
          << Time
          << GeographicalCoordinates.at(0)
          << GeographicalCoordinates.at(1)
          << ObjectDistance
          << LinearCoordinates.at(0)
          << LinearCoordinates.at(1)
          << LinearCoordinates.at(2)
          << ContouringCompletion
          << ba   // <<<
          << e1 << e2;


//         out_C.device()->seek(3);  // преход в начало сообщения  // <<<
//         out_C << quint32 (Data_C_MomentObjDet_G.size());  // Занесение размера сообщения  // <<<

    // ============================


    qint64 status = socket_C->write(Data_C_MomentObjDet_G);
//    socket_C->waitForDisconnected(10);
   // socket_C->close();
    //    socket_C->flush();  // ?? обрыв сообшения
    //    socket_C->disconnected();


//    qDebug() << "TCPClient::MomentOjectDetection::Data_C:: " << Data_C;
//    qDebug() << "TCPClient::MomentOjectDetection::status:: " << status;
//    qDebug() << "TCPClient::MomentOjectDetection::SendToServer:: end ";


}



// ==============================
// SendToServer_StateInformation
// ==============================

void TCPClient::SendToServer_ObjectDetectionImgFile(QString filePathName)
{

    qDebug() << "TCPClient::SendToServer_OjectDetectionImg:: start ";


    QFile* file = new QFile(filePathName);

    if(file->open(QFile::ReadOnly))
    {

        QFileInfo fileInfo( filePathName) ;
//        quint64 sizeF = fileInfo.size();
//        QString nameF = QFileInfo(file->fileName()).fileName();


        Data_C_ObjDetImgFile_G.clear();
        QDataStream out_C(&Data_C_ObjDetImgFile_G, QIODevice::WriteOnly); // Только запись иначе ничего не получиться

        out_C.setVersion(QDataStream::Qt_6_2);


        // Заголовки и концовки пакета
        quint8  b1 = 170;
        quint8  b2 = 77;    // новое значение - код типа сообщения  // <<<
        // quint8  b2 = 187; // старое значение   // <<<

        quint8  id = 77;        // код типа сообщения // 77  - код получения изображения
        quint8  e1 = 0;
        quint8  e2 = 254;

        //        // ============================
        //         // Для много пакетной передачи
        //        // ============================
        // Записываем размер блока в самое начало quint32(0).
        // Присваеваем 0 так как не знаем размер блока.
        // Главное занимает размер 2 байта

        // ============================
        out_C << b1 << b2 << id << quint32(0) <<  file->readAll() << e1 << e2;
        out_C.device()->seek(3);  // преход в начало сообщения
        out_C << quint32 (Data_C_ObjDetImgFile_G.size() - sizeof(quint32)); // ввод размера сообщения

        // ============================


        qint64 status = socket_C->write(Data_C_ObjDetImgFile_G);
//        socket_C->waitForDisconnected(10);
        //   socket_C->close();
        //    socket_C->flush();  // ?? обрыв сообшения
        //    socket_C->disconnected();


//        qDebug() << "TCPClient::SendToServer_StateInformation::status:: " << status;
//        qDebug() << "TCPClient::SendToServer_StateInformation::SendToServer:: end ";



    }

    else
    {

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit this->signal_sendText_plainTextEdit_StateInfo( "TCPClient::OjectDetectionImg:: File can not be open for read"  ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>>>

        return;
    }




}

void TCPClient::SendToServer_ObjectDetectionCompressedImg(QImage img)
{



    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "JPG", 10); // writes image into ba in PNG format



    qDebug() << "TCPClient::SendToServer_ObjectDetectionCompressedImg:: start ";



        Data_C_ObjDetCompresImg_G.clear();
        QDataStream out_C(&Data_C_ObjDetCompresImg_G, QIODevice::WriteOnly); // Только запись иначе ничего не получиться

        out_C.setVersion(QDataStream::Qt_6_2);


        // Заголовки и концовки пакета
        quint8  b1 = 170; // байты начала сообщения
        quint8  b2 = 88;    // новое значение - код типа сообщения  // <<<
        // quint8  b2 = 187; // старое значение   // <<<

        quint8  id = 88;  // код типа сообщения // 88  - код получения изображения
        quint8  e1 = 0;
        quint8  e2 = 254; // байты конца сообщения

        //        // ============================
        //         // Для много пакетной передачи
        //        // ============================
        // Записываем размер блока в самое начало quint32(0).
        // Присваеваем 0 так как не знаем размер блока.
        // Главное занимает размер 2 байта

        // ============================
        out_C << b1 << b2 << id << quint32(0) <<  ba << e1 << e2;
        out_C.device()->seek(3);  // преход в начало сообщения
        out_C << quint32 (Data_C_ObjDetCompresImg_G.size() - sizeof(quint32)); // ввод размера сообщения




        // ============================
        // Для однопакетной пакетной передачи
        // ============================
//        out_C  << id <<  ba << e1 << e2;


        // ============================
        // Тест
        // ============================
//        out_C  << ba;


        // ============================


        qint64 status = socket_C->write(Data_C_ObjDetCompresImg_G);
//        socket_C->waitForDisconnected(10);

        //   socket_C->close();
        //    socket_C->flush();  // ?? обрыв сообшения
        //    socket_C->disconnected();


        //        qDebug() << "TCPClient::SendToServer_StateInformation::status:: " << status;
        //        qDebug() << "TCPClient::SendToServer_StateInformation::SendToServer:: end ";





//        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//        emit this->signal_sendText_plainTextEdit_StateInfo( "TCPClient::OjectDetectionImg:: File can not be open for read"  ); // отправка текста в информационное окно
//        // >>>>>>>>>>>>>>>>>>>>>>





}








// ============================
// Слот для чтения сообшений
// ============================
void TCPClient::slotReadyRead_C() // >>>>> public slots:
{


    // QDataStream - класс для работы с потоковым вводом-выводом информации
    // Является мощным иструментом для работы с передачей данных
    QDataStream in_C(socket_C); // создаём объект in, кот. работаем с данными находящимися в сокете
    in_C.setVersion(QDataStream::Qt_6_2); // устанновка версии для избежания ошибок

    // Если ошибок нет продолжаем обработку, если ошибка то сообщение в консоль
    if (in_C.status() == QDataStream::Ok)
    {
        qDebug() << "TCPClien::slotReadyRead_C():: read...";



        QString str_C;
        in_C >> str_C;
        qDebug() << str_C; // Вывод в консоль


//         // =====================
//         //  Цикл
//         // =====================
//        // Так как не знаем придут нам данные целиком или по частям поэтому будем работать в бесконечном цикле
//        for(;;)
//        {
//           // Первая задача считать размер блока
//           // условие размер блока неизвестен
//            if (nextBlockSize_C == 0)
//           {
//               // условие для чтение доступно не меньше 2-х байт
//                if (socket_C->bytesAvailable() < 2)
//               {
//                  // если условие не выполнелось выход из цикла
//                    break;
//               }
//               // если условие выполнелось выход из цикла считываем размер блока
//               in_C >> nextBlockSize_C;
//           }
//            // Теперь когда известен размер блока сравниваем его с количесвом байт пришедших от сервера
//            // Если размер блока больше то данные пришли не полностью
//           if (socket_C->bytesAvailable() < nextBlockSize_C)
//           {
//               break;
//           }

//           QString str_C;
//           in_C >> str_C;
//           qDebug() << str_C; // Вывод в консоль
//           nextBlockSize_C = 0; // обнуляем размер блока, чтоб принемать новые сообщения

//           // Принудительный выход из безконечного цикла
//            break;

//        }

//        // =====================


    }
    else
    {
        qDebug() << "TCPClien::slotReadyRead_C():: DataStream error";
    }


}
