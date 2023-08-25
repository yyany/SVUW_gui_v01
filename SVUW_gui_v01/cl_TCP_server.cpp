# include <cl_TCP_server.h>

// ============================
// Конструктор
// ============================
TCPServer::TCPServer()
{
    // В оригенале тут код метода StartServer
    nextBlockSize_S = 0;
}



void TCPServer::setHostPort_S(int port)
{

    hostPort_S = port;

}




void TCPServer::StartTcpServer() // >>>>> public:
{

    qDebug() << "TCPServer::StartTcpServer:: BIGIN ...";

    // Если сервер не запустится сообщение об ошибке
    // тут указано что сервер будет принимать сигналы с любого адреса и прослушивает порт 2323
    if(this->listen(QHostAddress::Any, hostPort_S))
    {

        qDebug() << "TCPServer::StartTcpServer:: start ";


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "StartTcpServer:: Host:: Any::" + QString::number(hostPort_S) ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>>>

    }
    else
    {
        qDebug() << "TCPServer::StartTcpServer:: error";

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "TCPServer::StartTcpServer:: error "  ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>>>

    }


}


void TCPServer::StopTcpServer() // >>>>> public:
{

    qDebug() << "TCPServer::StopTcpServer:: BIGIN ...";

    // Если сервер запущен то остановка
    if(this->isListening())
    {
        this->disconnect();
        this->close();

        qDebug() << "TCPServer::StopTcpServer:: stop ";

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "TCPServer::StopTcpServer:: stop "  ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>>>
    }
    else
    {
        qDebug() << "TCPServer::StopTcpServer:: error";


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "TCPServer::StopTcpServer:: error"  ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>>>
    }


}





// Обработка подключений
// ============================
// Создание сокета
// ============================
void TCPServer:: incomingConnection(qintptr socketDescriptor)  // >>>>> public slots:
{

    qDebug() << "TCPServer:: incomingConnection:: BIGIN ... " << socketDescriptor;




    socket_S = new QTcpSocket;    // содание нового сокета
    socket_S -> setSocketDescriptor(socketDescriptor); // установка в сокеке дискритора, который идентифицирует поток ввода-вывода
    QObject::connect(socket_S, &QTcpSocket::readyRead, this,  &TCPServer::slotReadyRead_S ); // объединение сигналов readyRead со слотом slotReadyRead
    QObject::connect(socket_S, &QTcpSocket::disconnected, socket_S, &QTcpSocket::deleteLater); // соединения сигнала disconnected с фукцией deleteLater
    // то есть при отключении клиента приложение удалить сокет сразу как это станет возможно


    // Помешение сокета в контейнер
    // Эта функция будет вызываться всегда при каждом новом подключении клиента
    // Для каждого клиента создаём новый сокет и сохраняе его в вектор
    Sockets_S.push_back(socket_S);





    // сообщение о полключении клиента
    qDebug() << "TCPServer:: incomingConnection:: client connected " << socketDescriptor;

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    emit signal_sendText_plainTextEdit_StateInfo( "TCPServer::incomingConnection::client connected " ); // отправка текста в информационное окно
    // >>>>>>>>>>>>>>>>>>>>>>

    // Вывод адреса поключившегося компа (не удачно)
    //    QHostAddress ip_C = this->nextPendingConnection()->peerAddress();
    //    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //    emit signal_sendText_plainTextEdit_StateInfo( "TCPServer:: incomingConnection:: client connected, ip: " + ip_C.toString() ); // отправка текста в информационное окно
    //    // >>>>>>>>>>>>>>>>>>>>>>

}
// ============================
// Слот для чтения сообшений
// ============================
void TCPServer::slotReadyRead_S()
{

    qDebug() << "TCPServer::slotReadyRead_S:: BIGIN ... ";

    // В переменную сокет нужно записать именно тот сокет, с которого пришёл запрос
    socket_S = (QTcpSocket*) sender();

    // QDataStream - класс для работы с потоковым вводом-выводом информации
    // Является мощным иструментом для работы с передачей данных
    QDataStream in_S(socket_S); // создаём объект in, кот. работаем с данными находящимися в сокете
    in_S.setVersion(QDataStream::Qt_6_2); // устанновка версии для избежания ошибок

    // Если ошибок нет продолжаем обработку, если ошибка то сообщение в консоль
    if (in_S.status() == QDataStream::Ok)
    {
        qDebug() << "TCPServer::slotReadyRead():: read...";


        // Передаём текс

        //        std::vector<double> GeographicCoordinates {0, 0}; // Текущие географические координаты АНПА (широта, долгота)
        //        double HardwareTime;                              // Аппаратное время*
        //        float FloorDistance;                              // Отстояние от дна
        //        float RollANPA;                                   // Крен АНПА
        //        float TrimANPA;                                   // Дифферент
        //        std::vector<float> AngularVelocity {0, 0, 0};     // Угловая скорость АНПА
        //        std::vector<float> ApparentAcceleration {0, 0, 0};// Кажущееся ускорение АНПА
        //        float AbsoluteVelocity;                           // Абсолютная скорость


        // Команда на начало работы программы
        quint8 StartCommand_S = G_StartCommand_S;


        quint8  b1;     // байты начала сообщения
        quint8  b2;
        quint8  id;     // код сообщения
        quint8  e1;
        quint8  e2;      // байты конца сообщения

        in_S >> b1 >> b2;        // байты начала сообщения
        // in_S >> id;             // код типа сообщения
        // in_S >> nextBlockSize_S; // размер сообщения



        switch( b2 )
        {

        case 111:
        {

            in_S  >>  StartCommand_S  // Команда на начало работы программы
                  >> e1 >> e2;

            qDebug() << "  >>>>>>>>> StartCommand_S = " <<  StartCommand_S;


            // отправление сигнала на форму об получении данных
            // emit TCPServer::signal_TcpServetToForm_Receive();


            // %%%%%%%%%%%%%%%%%%%%%%%%
            // Отправка сигнала начало работы
            // %%%%%%%%%%%%

            if (G_StartCommand_S != StartCommand_S)

            {


//                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//                emit signal_sendText_plainTextEdit_StateInfo( " G_StartCommand_S != StartCommand_S = " + QString::number(G_StartCommand_S != StartCommand_S)); // отправка текста в информационное окно
//                // >>>>>>>>>>>>>>>>>>>>

//                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//                emit signal_sendText_plainTextEdit_StateInfo( " G_StartCommand_S = " + QString::number(G_StartCommand_S)); // отправка текста в информационное окно
//                // >>>>>>>>>>>>>>>>>>>>

                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                emit signal_sendText_plainTextEdit_StateInfo( " StartCommand_S = " + QString::number(StartCommand_S)); // отправка текста в информационное окно
                // >>>>>>>>>>>>>>>>>>>>

                // Команда на начало работы программы
                G_StartCommand_S = StartCommand_S;

                qDebug() << "  >>>>>>>>> signal_SendStartCommand " ;




                //    // =================================
                //    // Автоматический отправка состояния готовности к работе (по задержке)
                //    // =================================
                //  QTimer::singleShot( 8000, this, SLOT( slot_SendStartCommand() ) );
                //     QTimer::singleShot( 8000, this, SIGNAL( signal_SendStartCommand() ) );


                // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                emit signal_SendStartCommand(); // Сигнал от TCP сервера к главной форме для старта программы по команде от стороннего TCP клиента
                // %%%%%%%%%%%%

                //    // =================================
                //    // Автоматический отправка состояния готовности к работе (по задержке)
                //    // =================================
                    QTimer::singleShot( 8000, this, SIGNAL( signal_Send_InitialStateInformation() ) );
//                // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                emit signal_Send_InitialStateInformation();  // Сигнал для отправки TCP сообщения о готовности к работе (только при первом запуске или в начале работы программы)
//                // %%%%%%%%%%%%




                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                emit signal_sendText_plainTextEdit_StateInfo( " Отправлен сигнал на автозапуск программы "); // отправка текста в информационное окно
                // >>>>>>>>>>>>>>>>>>>>

            }

            // =====================================
            // =====================================




            break ;
        }

        case 112:
        {


            in_S  >> GeographicCoordinates_S.at(0)      // Текущие географические координаты АНПА (широта, долгота)
                >> GeographicCoordinates_S.at(1)
                >> HardwareTime_S                       // Аппаратное время*
                >> FloorDistance_S                      // Отстояние от дна
                >> RollANPA_S                           // Крен АНПА
                >> TrimANPA_S                           // Дифферент
                >> AngularVelocity_S.at(0)              // Угловая скорость АНПА
                >> AngularVelocity_S.at(1)
                >> AngularVelocity_S.at(2)
                >> ApparentAcceleration_S.at(0)         // Кажущееся ускорение АНПА
                >> ApparentAcceleration_S.at(1)
                >> ApparentAcceleration_S.at(2)
                >> AbsoluteVelocity_S                   // Абсолютная скорость

                >> e1 >> e2;



            //        QDateTime DateTime;
            //        DateTime.setMSecsSinceEpoch(qint64(HardwareTime));

            //        qDebug() << "============== slotReadyRead_S ===============";
            //        qDebug() << "GeographicCoordinates.at(0) = " << GeographicCoordinates.at(0);
            //        qDebug() << "GeographicCoordinates.at(1) = " <<  GeographicCoordinates.at(1);
            //        qDebug() << "HardwareTime = "  <<  QString::number(HardwareTime, 'f', 2);
            //        qDebug() << "HardwareTime = "  << DateTime.toString("yyyy-MM-dd hh_mm_ss");
            //        qDebug() << "FloorDistance = " <<  FloorDistance;
            //        qDebug() << "RollANPA = " <<  RollANPA;
            //        qDebug() << "TrimANPA = " <<  TrimANPA;
            //        qDebug() << "AngularVelocity.at(0) = " <<  AngularVelocity.at(0);
            //        qDebug() << "AngularVelocity.at(1) = " <<  AngularVelocity.at(1);
            //        qDebug() << "AngularVelocity.at(2) = " <<  AngularVelocity.at(2);
            //        qDebug() << "ApparentAcceleration.at(0) = " <<  ApparentAcceleration.at(0);
            //        qDebug() << "ApparentAcceleration.at(1) = " <<  ApparentAcceleration.at(1);
            //        qDebug() << "ApparentAcceleration.at(2) = " <<  ApparentAcceleration.at(2);
            //        qDebug() << "AbsoluteVelocity = " <<  AbsoluteVelocity;
            //        qDebug() << "============================================";


            //        // ============================
            //         // Для много пакетной передачи
            //        // ============================
            //        // Так как не знаем придут нам данные целиком или по частям поэтому будем работать в бесконечном цикле
            //        for(;;)
            //        {
            //            // Первая задача считать размер блока
            //            // условие размер блока неизвестен
            //            if (nextBlockSize_S == 0)
            //            {
            //                // условие для чтение доступно не меньше 2-х байт
            //                if (socket_S->bytesAvailable() < 2)
            //                {
            //                    // если условие не выполнелось выход из цикла
            //                    break;
            //                }
            //                // если условие выполнелось выход из цикла считываем размер блока
            //                in_S >> nextBlockSize_S;
            //            }
            //            // Теперь когда известен размер блока сравниваем его с количесвом байт пришедших от сервера
            //            // Если размер блока больше то данные пришли не полностью
            //            if (socket_S->bytesAvailable() < nextBlockSize_S)
            //            {
            //                break;
            //            }

            //            QString str_S;
            //            in_S >> str_S;
            //            qDebug() << str_S;
            //            nextBlockSize_S = 0; // обнуляем размер блока, чтоб принемать новые сообщения
            //            SendToClient_S(str_S);

            //            // Принудительный выход из безконечного цикла
            //            break;

            //        }
            //        // ============================




            // =====================================
            // =====================================


            // =====================================
            // Создание папки
            // =====================================



            std::string path_Log =   "./files/Aurora/Log/";



#if defined(_WIN64)

            // ================
            // Only Windows
            // ================

            int result01 = _mkdir(path_Log.c_str());
            if (result01==0 )
                std::cout << "Aurora::Log:: Folder WriteOneStereoPair is made \n";
            else
                std::cout << "Aurora::Log:: Don't made Folder WriteOneStereoPair \n";


// ================
// Only Windows END
// ================

#else

            // ================
            // Only Linux
            // ================

            if (mkdir(path_Log.c_str(), 0777) == -1  )
                std::cout << "Aurora::Log:: Folder WriteOneStereoPair is made \n \n";
            else
                std::cout << "Aurora::Log:: Don't made Folder WriteOneStereoPair \n";

// ================
// Only Linux END
// ================

#endif





            // =====================================
            // Пути к файлам
            // =====================================
            std::stringstream filename_Log_OutsideInformationANPA;

            filename_Log_OutsideInformationANPA  << path_Log  << "Log_OutsideInformationANPA.txt";



            // =====================================
            //  Сохранение данных о 3D точках после кластеризации в текстовый файл
            // =====================================

            std::ofstream out3;          // поток для записи
            out3.open(filename_Log_OutsideInformationANPA.str(), std::ios_base::app); // окрываем файл для записи


            if (out3.is_open() )
            {

                out3
                    << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString()   << std::endl

                    << "GeographicalCoordinates: "
                    << GeographicCoordinates_S.at(0) <<  " с.ш. "
                    << GeographicCoordinates_S.at(1) <<  " в.д. "  << std::endl

                    << "FloorDistance: " << FloorDistance_S << "; " << std::endl
                    << "RollANPA: " << RollANPA_S << "; " << std::endl
                    << "TrimANPA: " << TrimANPA_S << "; " << std::endl

                    << "AngularVelocity : "
                    << AngularVelocity_S.at(0) << ", "
                    << AngularVelocity_S.at(1) << ", "
                    << AngularVelocity_S.at(2) << "; "  << std::endl

                    << "ApparentAcceleration: "
                    << ApparentAcceleration_S.at(0) << ", "
                    << ApparentAcceleration_S.at(1) << ", "
                    << ApparentAcceleration_S.at(2) << "; " << std::endl

                    << "AbsoluteVelocity: "<< AbsoluteVelocity_S  << std::endl
                    << " ===================== " << std::endl;

            }

            out3.close();




            // =====================================


            // =====================================
            // Сохранение информация об обнаруженном объекте в csv Log файл
            // =====================================

            // Пути к файлам
            std::stringstream filename_Log_CSV;
            filename_Log_CSV  << path_Log  << "Log_OutsideInformationANPA.csv";


            // QFileInfo для проверки файла на существование
            bool chechFileExist = QFileInfo::exists( filename_Log_CSV.str().c_str() );



            std::ofstream out4;          // поток для записи
            out4.open(filename_Log_CSV.str(), std::ios_base::app); // окрываем файл для записи



            // ===============================
            // Пропись заголовков столбцов в новом csv-файле, когда он не существовал до этого
            //================================


            if( !chechFileExist ) // проверка файла на существование
            {

                if (out4.is_open() )
                {

                    out4
                        << "current Date; "
                        << "current Time; "

                        << "Geographical Coordinates (latitude); "
                        << "Geographical Coordinates (longitude); "

                        << "FloorDistance; "
                        << "Roll ANPA; "
                        << "Trim ANPA; "

                        << "Angular Velocity (x); "
                        << "Angular Velocity (y); "
                        << "Angular Velocity (z); "

                        << "Apparent Acceleration (x); "
                        << "Apparent Acceleration (y); "
                        << "Apparent Acceleration (z); "

                        << "Absolute Velocity; "
                        << std::endl;

                }

                qDebug() << "Log_InitialStateInformation.csv:: Created ";

            }







            if (out4.is_open() )
            {

                out4

                    //            << "Geographical Coordinates (latitude); "
                    //            << "Geographical Coordinates (longitude); "
                    //            << "Floor Distance; "

                    //            << "FloorDistance; "
                    //            << "Roll ANPA; "
                    //            << "Trim ANPA; "

                    //            << "Angular Velocity (x); "
                    //            << "Angular Velocity (y); "
                    //            << "Angular Velocity (z); "

                    //            << "Apparent Acceleration (x); "
                    //            << "Apparent Acceleration (y); "
                    //            << "Apparent Acceleration (z); "

                    //            << "Absolute Velocity; "

                    << QDateTime::currentDateTime().toString("yyyy-MM-dd; ").toStdString()
                    << QDateTime::currentDateTime().toString("hh:mm:ss; ").toStdString()
                    << GeographicCoordinates_S.at(0) << "; "
                    << GeographicCoordinates_S.at(1) << "; "

                    << FloorDistance_S << "; "
                    << RollANPA_S << "; "
                    << TrimANPA_S << "; "

                    << AngularVelocity_S.at(0) << "; "
                    << AngularVelocity_S.at(1) << "; "
                    << AngularVelocity_S.at(2) << "; "

                    << ApparentAcceleration_S.at(0) << "; "
                    << ApparentAcceleration_S.at(1) << "; "
                    << ApparentAcceleration_S.at(2) << "; "

                    << AbsoluteVelocity_S  << std::endl;




            }

            out4.close();


            break ;
        } // Case
        } // switch




    }
    else
    {
        qDebug() << "TCPServer::slotReadyRead():: DataStream error";
    }






}


// ============================
// Функция отпавки сообщений клиенту
// ============================
void TCPServer:: SendToClient_S(QString str) // >>>>> public:
{
    // Подготовка сообщения для отправки
    Data_S.clear(); // Чистка массива Data так как при создании в нём случайные данные

    // объект out массив для вывода данных
    // в конструкторе пишем массив байтов и режим работы только для записи иначе ничего не получиться
    QDataStream out_S(&Data_S, QIODevice::WriteOnly);
    out_S.setVersion(QDataStream::Qt_6_2);


    // Записываем размер блока в самое начало quint16(0).
    // Присваеваем 0 так как не знаем размер блока.
    // Главное занимает размер 2 байта
    // out << str; // Записываем через объект out строку в массив Date
    out_S << quint16(0) << str;
    out_S.device()->seek(0);  // преход в начало сообщения
    out_S << quint16 (Data_S.size() - sizeof(quint16)); // ввод размера сообщения



    // socket->write(Data); // Записываем массив в сокет
    for (int i=0; i< Sockets_S.size(); i++)
    {
        Sockets_S[i]->write(Data_S);
    }



    // ДАННЫЕ ОТПРАВЛЕНЫ
    // остаётся только запустить сервер (в одном из потоков)


}







