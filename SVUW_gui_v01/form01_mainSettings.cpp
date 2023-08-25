#include "form01_mainSettings.h"
#include "ui_form01_mainSettings.h"


ADS::cl_DenseStereo *obj_ptrf; // указатель оъект класса
str_Settings *settings_ptrf;  // указатель на структуру настроек


form01_mainSettings::form01_mainSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form01_mainSettings)
{
    ui->setupUi(this);

    // =================================
    // Таймер
    // =================================
    timer_value = 0;            // обнуление счётчика тайм
    timer = new QTimer(this);   // инициализация
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot_f01())); // соединение сигнала со слотом для таймера
    timer->setInterval(1000);  // Задаем интервал таймера

    timer->start();            // запуск таймера (время в милисикундах)


    // =================================
    // Тайммер TCP (по сигналу таймера происходит отправка TCP пакета)
    // =================================
    timer_value_TCP = 0;            // обнуление счётчика тайм
    timer_TCP = new QTimer(this);   // инициализация

    // Соединение сигнал-слот для TCP сообщений по таймеру
    connect(timer_TCP, SIGNAL(timeout()), this, SLOT( on_checkBox_Send_StatusStateInformation_clicked() )); // соединение сигнала со слотом для таймера
    connect(timer_TCP, SIGNAL(timeout()), this, SLOT( on_checkBox_Send_MomentOjectDetection_clicked() ));   // соединение сигнала со слотом для таймера

    // Соединение сигнал-слот для отправки изображения по таймеру
    //connect(timer_TCP, SIGNAL(timeout()), this, SLOT( on_checkBox_Send_ObjectDetectionImgFile_clicked() ));   // соединение сигнала со слотом для таймера
    //connect(timer_TCP, SIGNAL(timeout()), this, SLOT( on_checkBox_Send_ObjectDetectionCompressedImg_clicked() ));   // соединение сигнала со слотом для таймера

    // !!! Замена предыдущих для отправки изображений, так как внутри  on_checkBox_Send_MomentOjectDetection_clicked - происходит обнуление переменных обнаружения
    // Соединение сигнал-слот для запуска отправки изображения по сигналу, только в случаи обнаружения объекта
    connect(this, &form01_mainSettings::signal_Send_ObjectDetectionCompressedImg,
            this, &form01_mainSettings::on_checkBox_Send_ObjectDetectionCompressedImg_clicked);



    //    int interval = settings_ptrf->TCP.TCPsetTimerInterval;
    int interval = 1000;
    timer_TCP->setInterval(interval);  // Задаем интервал таймера (мс)


    // =================================
    // Для получения сиглала от TCP сервера о примёме данных
    // =================================

    //     connect(obj_ptrf, &ADS::cl_DenseStereo::signal_TcpServetToForm_Receive,
    //             this, &form01_mainSettings::slot_TcpServetToForm_Receive);

    //     connect(obj_ptrf, &TCPServer::signal_TcpServetToForm_Receive,
    //                 this, &form01_mainSettings::slot_TcpServetToForm_Receive);

    //    connect(obj_ptrf, SIGNAL(signal_TcpServetToForm_Receive()),
    //            this, SLOT(slot_TcpServetToForm_Receive() ) );
    // =================================
    // альтернатива, так как не получилось отослать или принять сигнал
    // =================================
    connect(timer_TCP, SIGNAL(timeout()), this, SLOT( slot_TcpServetToForm_Receive() ));
    // =================================



    // =================================
    // Автоматический запуск TCP (по задержке)
    // =================================
    QTimer::singleShot( 2000, this, SLOT( on_checkBox_StartTcpServer_clicked()  ));
    QTimer::singleShot( 3000, this, SLOT( on_checkBox_StartTcpClient_clicked() ) );


    // =================================
    // Автоматический отправка состояния готовности к работе (по задержке)
    // =================================
    QTimer::singleShot( 5000, this, SLOT( on_checkBox_Send_InitialStateInformation_clicked() ) );
    // =================================
    // Автоматическая отправка TCP  (по задержке)
    // =================================
    // !!! Закоментированно, так как запускается по сигналу на старт программы
    // QTimer::singleShot( 9000, this, SLOT( slot_Initiate_timerTCP() ) );

    // !!! по сигналу не подкодит так как не будет работать в ручном режиме, Записанов в  slot_ApplyStartCommand
    //    connect(obj_ptrf, &TCPServer::signal_SendStartCommand,
//            this, &form01_mainSettings::slot_Initiate_timerTCP);







     // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     emit signal_sendText_plainTextEdit_StateInfo( " Запуск окна доп. настроек"); // отправка текста в информационное окно
     // >>>>>>>>>>>>>>>>>>>>


}

form01_mainSettings::~form01_mainSettings()
{
    delete ui;
}



// Функция дублирования
void form01_mainSettings::ChangeHSVFindColor()
{



    double h1, s1, v1, h2, s2, v2;
    h1 = ui->spinBox_LowHSV_h->value();
    s1 = ui->spinBox_LowHSV_s->value();
    v1 = ui->spinBox_LowHSV_v->value();
    h2 = ui->spinBox_HieghHSV_h->value();
    s2 = ui->spinBox_HieghHSV_s->value();
    v2 = ui->spinBox_HieghHSV_v->value();

    settings_ptrf->LowHSV = {h1, s1, v1};
    settings_ptrf->HieghHSV = {h2, s2, v2};



}

// ===================================
// Слот для принятия данных с главной формы по сигналу от неё
// ===================================
void form01_mainSettings::slot_MainFormToForm01(ADS::cl_DenseStereo *obj_ptr_slot, str_Settings *settings_ptr_slot)
{
    obj_ptrf = obj_ptr_slot;
    settings_ptrf = settings_ptr_slot;


    // ======================
    // Инициализация параметры на окне приложения из файла настроек
    // ======================
    // инициализация элементов на форме

    double h1, s1, v1, h2, s2, v2;

    // =================================
    // Установка значений в spinBox поиск по цвету HSV
    // =================================
    h1 = settings_ptrf->LowHSV[0];
    s1 = settings_ptrf->LowHSV[1];
    v1 = settings_ptrf->LowHSV[2];
    h2 = settings_ptrf->HieghHSV[0];
    s2 = settings_ptrf->HieghHSV[1];
    v2 = settings_ptrf->HieghHSV[2];

    ui->spinBox_LowHSV_h->setValue(h1);
    ui->spinBox_LowHSV_s->setValue(s1);
    ui->spinBox_LowHSV_v->setValue(v1);

    ui->spinBox_HieghHSV_h->setValue(h2);
    ui->spinBox_HieghHSV_s->setValue(s2);
    ui->spinBox_HieghHSV_v->setValue(v2);


    // =================================
    // Установка значений в spinBox Disparities
    // =================================
    ui->spinBox_smbNumDisparities->setValue(settings_ptrf->smbNumDisparities);
    ui->spinBox_smbNumDisparities->setValue(settings_ptrf->smbNumDisparities);
    ui->spinBox_smbBlockSize->setValue(settings_ptrf->smbBlockSize);
    ui->spinBox_smbPreFilterCap->setValue(settings_ptrf->smbPreFilterCap);

    ui->spinBox_smbMinDisparity->setValue(settings_ptrf->smbMinDisparity);
    ui->spinBox_smbTextureThreshold->setValue(settings_ptrf->smbTextureThreshold);
    ui->spinBox_smbUniquenessRatio->setValue(settings_ptrf->smbUniquenessRatio);

    ui->spinBox_smbSpeckleWindowSize->setValue(settings_ptrf->smbSpeckleWindowSize);
    ui->spinBox_smbSpeckleRange->setValue(settings_ptrf->smbSpeckleRange);
    ui->spinBox_smbDisp12MaxDiff->setValue(settings_ptrf->smbDisp12MaxDiff);


    ui->spinBox_dbsRad->setValue(settings_ptrf->rad);
    ui->spinBox_dbsNeighbors->setValue(settings_ptrf->neighbors);
    ui->spinBox_filter_ClusterPoints->setValue(settings_ptrf->filter_ClusterPoints);

    ui->doubleSpinBox_coef_base_dbs->setValue(settings_ptrf->coef_base_dbs);
    ui->doubleSpinBox_coef_pow_dbs->setValue(settings_ptrf->coef_pow_dbs);

    ui->spinBox_limit_points->setValue(settings_ptrf->limit_points);


    // =================================
    // Установка значений в spinBox WorkingArea
    // =================================
    ui->spinBox_X_Low->setValue(settings_ptrf->limit_WorkingArea[0]);
    ui->spinBox_Y_Low->setValue(settings_ptrf->limit_WorkingArea[1]);
    ui->spinBox_Z_Low->setValue(settings_ptrf->limit_WorkingArea[2]);
    ui->spinBox_X_Up->setValue(settings_ptrf->limit_WorkingArea[3]);
    ui->spinBox_Y_Up->setValue(settings_ptrf->limit_WorkingArea[4]);
    ui->spinBox_Z_Up->setValue(settings_ptrf->limit_WorkingArea[5]);


    // ======================
    //
    // ======================

    settings_ptrf->metod_Disparity = ui->comboBox_MetodDisparity->currentIndex();




    // ===========================
    // Установка значений  адреса и порта сервера для клиета  в строку на ui
    // ===========================

    ui->lineEdit_TCPhostIP_C->setText(QString::fromStdString(settings_ptrf->TCP.TCPhostIP_C));
    ui->lineEdit_TCPhostPort_C->setText(QString::number(settings_ptrf->TCP.TCPhostPort_C));

    ui->lineEdit_AuroraLogPath->setText(QString::fromStdString(settings_ptrf->Aurora.Aurora_AuroraLogPath));
    ui->lineEdit_AuroraFilesPath->setText(QString::fromStdString(settings_ptrf->Aurora.Aurora_AuroraFilesPath));

    // ===========================
    // Инициализация значения яркости фонаря подаваемое на SerialPort
    // ===========================
    G_AuroraLantern_SerialPort_value = settings_ptrf->SerialPort.value;
    ui->spinBox_SwitchAuroraLantern->setValue( G_AuroraLantern_SerialPort_value ); //  вывод значения по умолчаниию
    ui->label_AuroraLanternSerialPortCOMport->setText(QString::fromStdString(settings_ptrf->SerialPort.COMport)); // вывод ззначения COM порта



    // ===================================
    // Инициализация виджетов вывода информации на экран (РТК)
    // ===================================

    if (settings_ptrf->mode_AddResults.at(0) == 0 )  ui->checkBox_ShowMarkersImgRTK_00->setChecked(false);
    if (settings_ptrf->mode_AddResults.at(0) == 1 )  ui->checkBox_ShowMarkersImgRTK_00->setChecked(true);

    if (settings_ptrf->mode_AddResults.at(1) == 0 )  ui->checkBox_ShowMarkersImgRTK_01->setChecked(false);
    if (settings_ptrf->mode_AddResults.at(1) == 1 )  ui->checkBox_ShowMarkersImgRTK_01->setChecked(true);

    if (settings_ptrf->mode_AddResults.at(2) == 0 )
    {
        ui->checkBox_ShowMarkersImgRTK_02->setChecked(false);
        ui->radioButton_ShowMarkersImgRTK_02_1->setChecked(false);
        ui->radioButton_ShowMarkersImgRTK_02_1->setChecked(false);
    }
    if (settings_ptrf->mode_AddResults.at(2) == 1 )
    {
        ui->checkBox_ShowMarkersImgRTK_02->setChecked(true);
        ui->radioButton_ShowMarkersImgRTK_02_1->setChecked(true);
    }
    if (settings_ptrf->mode_AddResults.at(2) == 2 )
    {
        ui->checkBox_ShowMarkersImgRTK_02->setChecked(true);
        ui->radioButton_ShowMarkersImgRTK_02_2->setChecked(true);
    }

    if (settings_ptrf->mode_AddResults.at(3) == 0 )
    {
        ui->checkBox_ShowMarkersImgRTK_03->setChecked(false);
        ui->radioButton_ShowMarkersImgRTK_03_1->setChecked(false);
        ui->radioButton_ShowMarkersImgRTK_03_1->setChecked(false);
    }
    if (settings_ptrf->mode_AddResults.at(3) == 1 )
    {
        ui->checkBox_ShowMarkersImgRTK_03->setChecked(true);
        ui->radioButton_ShowMarkersImgRTK_03_1->setChecked(true);
    }
    if (settings_ptrf->mode_AddResults.at(3) == 2 )
    {
        ui->checkBox_ShowMarkersImgRTK_03->setChecked(true);
        ui->radioButton_ShowMarkersImgRTK_03_2->setChecked(true);
    }

    if (settings_ptrf->mode_AddResults.at(4) == 0 )  ui->checkBox_ShowMarkersImgRTK_04->setChecked(false);
    if (settings_ptrf->mode_AddResults.at(4) == 1 )  ui->checkBox_ShowMarkersImgRTK_04->setChecked(true);

    if (settings_ptrf->mode_AddResults.at(5) == 0 )  ui->checkBox_ShowMarkersImgRTK_05->setChecked(false);
    if (settings_ptrf->mode_AddResults.at(5) == 1 )  ui->checkBox_ShowMarkersImgRTK_05->setChecked(true);

    if (settings_ptrf->mode_AddResults.at(6) == 0 )  ui->checkBox_ShowMarkersImgRTK_06->setChecked(false);
    if (settings_ptrf->mode_AddResults.at(6) == 1 )  ui->checkBox_ShowMarkersImgRTK_06->setChecked(true);

    if (settings_ptrf->mode_AddResults.at(7) == 0 )  ui->checkBox_ShowMarkersImgRTK_07->setChecked(false);
    if (settings_ptrf->mode_AddResults.at(7) == 1 )  ui->checkBox_ShowMarkersImgRTK_07->setChecked(true);


    // ===================================





}

// =================================
// Слот для обработки сигнала от TCP Server
// =================================

void form01_mainSettings::slot_TcpServetToForm_Receive()
{



    // Условия не выполнения отправки
    if (!ui->checkBox_StartTcpServer->isChecked()) return;
    if (!ui->checkBox_Receive_OutsideInformationANPA->isChecked()) return;



    // =================================
    // Заносим полученные данные в общие данные
    // =================================
    // Текущие географические координаты АНПА (широта, долгота)
    settings_ptrf->Aurora.OutsideInformationANPA.GeographicCoordinates[0] = obj_ptrf->GeographicCoordinates_S.at(0);
    settings_ptrf->Aurora.OutsideInformationANPA.GeographicCoordinates[1] = obj_ptrf->GeographicCoordinates_S.at(1);
        // Аппаратное время*
    settings_ptrf->Aurora.OutsideInformationANPA.HardwareTime =  obj_ptrf->HardwareTime_S;
    // Отстояние от дна
    // Крен АНПА
    // Дифферент
    settings_ptrf->Aurora.OutsideInformationANPA.FloorDistance =  obj_ptrf->FloorDistance_S;
    settings_ptrf->Aurora.OutsideInformationANPA.RollANPA =  obj_ptrf->RollANPA_S;
    settings_ptrf->Aurora.OutsideInformationANPA.TrimANPA =  obj_ptrf->TrimANPA_S;
        // Угловая скорость АНПА
    settings_ptrf->Aurora.OutsideInformationANPA.AngularVelocity[0] = obj_ptrf->AngularVelocity_S.at(0);
    settings_ptrf->Aurora.OutsideInformationANPA.AngularVelocity[1] = obj_ptrf->AngularVelocity_S.at(1);
    settings_ptrf->Aurora.OutsideInformationANPA.AngularVelocity[2] = obj_ptrf->AngularVelocity_S.at(2);
    // Кажущееся ускорение АНПА
    settings_ptrf->Aurora.OutsideInformationANPA.ApparentAcceleration[0] = obj_ptrf->ApparentAcceleration_S.at(0);
    settings_ptrf->Aurora.OutsideInformationANPA.ApparentAcceleration[1] = obj_ptrf->ApparentAcceleration_S.at(1);
    settings_ptrf->Aurora.OutsideInformationANPA.ApparentAcceleration[2] = obj_ptrf->ApparentAcceleration_S.at(2);
    //Абсолютная скорость
    settings_ptrf->Aurora.OutsideInformationANPA.AbsoluteVelocity =  obj_ptrf->AbsoluteVelocity_S;



    QDateTime DateTime;
    DateTime.setMSecsSinceEpoch(qint64(settings_ptrf->Aurora.OutsideInformationANPA.HardwareTime));




//    // =================================
//    // Вывод полученных данных в терминал
//    // =================================

//    qDebug() << "============== slotReadyRead_S ===============";

//    qDebug() << "timer_value_TCP :: "<< timer_value_TCP;

//    qDebug() << "GeographicCoordinates.at(0) = " << settings_ptrf->Aurora.OutsideInformationANPA.GeographicCoordinates.at(0);
//    qDebug() << "GeographicCoordinates.at(1) = " <<  settings_ptrf->Aurora.OutsideInformationANPA.GeographicCoordinates.at(1);
//    qDebug() << "HardwareTime = "  <<  QString::number(settings_ptrf->Aurora.OutsideInformationANPA.HardwareTime, 'f', 2);
//    qDebug() << "HardwareTime = "  << DateTime.toString("yyyy-MM-dd hh_mm_ss");
//    qDebug() << "FloorDistance = " <<  settings_ptrf->Aurora.OutsideInformationANPA.FloorDistance;
//    qDebug() << "RollANPA = " <<  settings_ptrf->Aurora.OutsideInformationANPA.RollANPA;
//    qDebug() << "TrimANPA = " <<  settings_ptrf->Aurora.OutsideInformationANPA.TrimANPA;
//    qDebug() << "AngularVelocity.at(0) = " <<  settings_ptrf->Aurora.OutsideInformationANPA.AngularVelocity.at(0);
//    qDebug() << "AngularVelocity.at(1) = " <<  settings_ptrf->Aurora.OutsideInformationANPA.AngularVelocity.at(1);
//    qDebug() << "AngularVelocity.at(2) = " <<  settings_ptrf->Aurora.OutsideInformationANPA.AngularVelocity.at(2);
//    qDebug() << "ApparentAcceleration.at(0) = " <<  settings_ptrf->Aurora.OutsideInformationANPA.ApparentAcceleration.at(0);
//    qDebug() << "ApparentAcceleration.at(1) = " <<  settings_ptrf->Aurora.OutsideInformationANPA.ApparentAcceleration.at(1);
//    qDebug() << "ApparentAcceleration.at(2) = " <<  settings_ptrf->Aurora.OutsideInformationANPA.ApparentAcceleration.at(2);
//    qDebug() << "AbsoluteVelocity = " <<  settings_ptrf->Aurora.OutsideInformationANPA.AbsoluteVelocity;



//    qDebug() << "============================================";









}



void form01_mainSettings::on_spinBox_LowHSV_h_valueChanged(int arg1)
{
    ChangeHSVFindColor();

}


void form01_mainSettings::on_spinBox_LowHSV_s_valueChanged(int arg1)
{
    // ChangeHSVFindColor();

    double s1 = ui->spinBox_LowHSV_s->value();
    settings_ptrf->LowHSV[1] = s1;

}


void form01_mainSettings::on_spinBox_LowHSV_v_valueChanged(int arg1)
{
    ChangeHSVFindColor();
}


void form01_mainSettings::on_spinBox_HieghHSV_h_valueChanged(int arg1)
{
    ChangeHSVFindColor();
}


void form01_mainSettings::on_spinBox_HieghHSV_s_valueChanged(int arg1)
{
    ChangeHSVFindColor();
}


void form01_mainSettings::on_spinBox_HieghHSV_v_valueChanged(int arg1)
{
    ChangeHSVFindColor();
}


void form01_mainSettings::on_spinBox_smbNumDisparities_valueChanged(int arg1)
{
    settings_ptrf->smbNumDisparities = ui->spinBox_smbNumDisparities->value();
}


void form01_mainSettings::on_checkBox_Send_InitialStateInformation_clicked()
{




    if (!ui->checkBox_StartTcpClient->isChecked()) return;
    if (!ui->checkBox_Send_InitialStateInformation->isChecked()) return;







    if ( !settings_ptrf->TCP.flag_StartProgram) // Выполниться, если прокграмма не начала работать
    {

        // =================================
        // Отправка сигнала на сериал порт Arduino для проверки работоспособности для заполнения информационных статусов
        // =================================
        int value =  0;
        string COMport = settings_ptrf->SerialPort.COMport; // номер компорта

        form01_mainSettings::SwitchAuroraLantern(value, COMport);




        // =====================================
        // !!! Одиночный запуск камер
        // =====================================


        // Запуск потока с #01, который запускает камеры
        Start_certainThread(obj_ptrf, settings_ptrf, 1);

#if defined(_WIN64)
        // ################################
        Sleep(1000);
        // ################################
#else
        // ################################
        usleep(1000*1000);
        // ################################
#endif

        // и остановка потока
        settings_ptrf->exitCode[1] = false;

    }
    // =====================================


    // +++++++++++++++++++++++++++
    // Получение сформированных данных о состоянии для отправки по TCP
    // +++++++++++++++++++++++++++

    settings_ptrf->Aurora.InitialStateInformation.SecondaryPowerSupply =  obj_ptrf->SecondaryPowerSupply_Cam;
    settings_ptrf->Aurora.InitialStateInformation.ConnectToCams = obj_ptrf->ConnectToCams_Cam;
    settings_ptrf->Aurora.InitialStateInformation.ImageAquisition = obj_ptrf->ImageAquisition_Cam;
    settings_ptrf->Aurora.InitialStateInformation.ReadyForOperationCompartmentLamp = obj_ptrf->ReadyForOperationCompartmentLamp_Cam;
    settings_ptrf->Aurora.InitialStateInformation.FuctSuccessfulOperation = obj_ptrf->FuctSuccessfulOperation_Cam;


    // =============================
    // данные для отправки
    // =============================
    quint8 q01 = settings_ptrf->Aurora.InitialStateInformation.SecondaryPowerSupply;
    quint8 q02 = settings_ptrf->Aurora.InitialStateInformation.ConnectToCams;
    quint8 q03 = settings_ptrf->Aurora.InitialStateInformation.ImageAquisition;
    quint8 q04 = settings_ptrf->Aurora.InitialStateInformation.ReadyForOperationCompartmentLamp;
    quint8 q05 = settings_ptrf->Aurora.InitialStateInformation.FuctSuccessfulOperation;



    //obj_ptrf->StartTcpClient();
    // ===========================
    // Подключение клиента к серверу
    // ===========================
    QString TCPhostIP_C = QString::fromStdString(settings_ptrf->TCP.TCPhostIP_C);
    quint16 TCPhostPort_C = settings_ptrf->TCP.TCPhostPort_C;


    obj_ptrf->ConnectToServer(TCPhostIP_C, TCPhostPort_C);


    // ===========================
    // Отправка пакета
    // ===========================
    obj_ptrf->SendToServer_StateInformation(q01, q02, q03, q04, q05);



    // =====================================
    // Сохранение информация о начальном состоянии в txt Log файл
    // =====================================


    // Создание папки
//    string path_Log =   "./files/Aurora/Log/";
    string path_Log =   settings_ptrf->Aurora.Aurora_AuroraLogPath;

#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    int result01 = _mkdir(path_Log.c_str());
    if (result01==0 )
        cout << "Aurora::Log:: Folder WriteOneStereoPair is made \n";
    else
        cout << "Aurora::Log:: Don't made Folder WriteOneStereoPair \n";


// ================
// Only Windows END
// ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(path_Log.c_str(), 0777) == -1  )
        cout << "Aurora::Log:: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "Aurora::Log:: Don't made Folder WriteOneStereoPair \n";

// ================
// Only Linux END
// ================

#endif


    // Пути к файлам
    stringstream filename_Log;
    filename_Log << path_Log  << "Log_InitialStateInformation.txt";

    // =====================================
    //  Сохранение данных о начальном состоянии в текстовый файл
    // =====================================

    std::ofstream out3;          // поток для записи
    out3.open(filename_Log.str(), ios_base::app); // окрываем файл для записи




    if (out3.is_open() )
    {

        out3
            << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString() <<  "; "  << endl
            << "SecondaryPowerSupply: " << settings_ptrf->Aurora.InitialStateInformation.SecondaryPowerSupply <<  "; "  << endl
            << "ConnectToCams: " << settings_ptrf->Aurora.InitialStateInformation.ConnectToCams <<  "; "  << endl
            << "ImageAquisition: " << settings_ptrf->Aurora.InitialStateInformation.ImageAquisition <<  "; "  << endl
            << "ReadyForOperationCompartmentLamp: " << settings_ptrf->Aurora.InitialStateInformation.ReadyForOperationCompartmentLamp <<  "; "  << endl
            << "FuctSuccessfulOperation: " << settings_ptrf->Aurora.InitialStateInformation.FuctSuccessfulOperation <<  "; "  << endl
            << " ========================= " << endl;

    }

    out3.close();




    // =====================================
    // Сохранение информация о начальном состоянии в csv Log файл
    // =====================================

    // Пути к файлам
    stringstream filename_Log_CSV;
    filename_Log_CSV << path_Log  << "Log_InitialStateInformation.csv";



    // QFileInfo для проверки файла на существование
    bool chechFileExist = QFileInfo::exists( filename_Log_CSV.str().c_str() );


    std::ofstream out4;          // поток для записи
    out4.open(filename_Log_CSV.str(), ios_base::app); // окрываем файл для записи

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
                << "Secondary Power Supply; "
                << "Connect To Cams; "
                << "Image Aquisition; "
                << "Ready ForOperation Compartment Lamp; "
                << "Fuct Successful Operation; "
                << endl;

        }

        qDebug() << "Log_InitialStateInformation.csv:: Created ";

    }





    if (out4.is_open() )
    {

        out4
//            << "current Date; " << "current Time; "
//            << "Secondary Power Supply; "
//            << "Connect To Cams; "
//            << "Image Aquisition; "
//            << "Ready ForOperation Compartment Lamp; "
//            << "Fuct Successful Operation; " << endl

            << QDateTime::currentDateTime().toString("yyyy-MM-dd; ").toStdString()
            << QDateTime::currentDateTime().toString("hh:mm:ss; ").toStdString()
            << settings_ptrf->Aurora.InitialStateInformation.SecondaryPowerSupply <<  "; "
            << settings_ptrf->Aurora.InitialStateInformation.ConnectToCams <<  "; "
            << settings_ptrf->Aurora.InitialStateInformation.ImageAquisition <<  "; "
            << settings_ptrf->Aurora.InitialStateInformation.ReadyForOperationCompartmentLamp <<  "; "
            << settings_ptrf->Aurora.InitialStateInformation.FuctSuccessfulOperation  << endl;


    }

    out4.close();



    // +++++++++++++++++++++++++++
    // Приведение в исходное состояние статусов состояния
    // +++++++++++++++++++++++++++
    // Реализация так себе. Как сделать по другому не понятно

    settings_ptrf->Aurora.InitialStateInformation.SecondaryPowerSupply = 1;             // Наличие вторичного электропитания отсека СТЗ
    settings_ptrf->Aurora.InitialStateInformation.ConnectToCams = 0;                    // Наличие соединения с камерами
    settings_ptrf->Aurora.InitialStateInformation.ImageAquisition = 0;                  // Получение изображения камерами
    settings_ptrf->Aurora.InitialStateInformation.ReadyForOperationCompartmentLamp = 1; // Готовность к работе регулятора яркости светильника отсека
    settings_ptrf->Aurora.InitialStateInformation.FuctSuccessfulOperation = 0;          // факт успешной работы



    // +++++++++++++++++++++++++++
    //
    // +++++++++++++++++++++++++++
    obj_ptrf->SecondaryPowerSupply_Cam = 1;             // Наличие вторичного электропитания отсека СТЗ
    obj_ptrf->ConnectToCams_Cam = 0;                    // Наличие соединения с камерами
    obj_ptrf->ImageAquisition_Cam = 0;                  // Получение изображения камерами
    obj_ptrf->ReadyForOperationCompartmentLamp_Cam = 1; // Готовность к работе регулятора яркости светильника отсека
    obj_ptrf->FuctSuccessfulOperation_Cam = 0;          // факт успешной работы



}

void form01_mainSettings::on_checkBox_Send_StatusStateInformation_clicked()
{


    // timer_value_TCP ++;  // нет необходимости посчитывать


    // Условия не выполнения отправки
    if (!ui->checkBox_StartTcpClient->isChecked()) return;
    if (!ui->checkBox_Send_StatusStateInformation->isChecked()) return;


    //if (!obj_ptrf->TcpClientConnectToServerisValid()) return; // проверка сокета на дееспособность socket_C->isValid()




   // qDebug() << "on_checkBox_StatusStateInformation_pressed::BIGIN";


    // +++++++++++++++++++++++++++
    // информация о состоянии (в текущий момент времени)
    // +++++++++++++++++++++++++++
    settings_ptrf->Aurora.StatusStateInformation.StatusSuccessfulOperation = obj_ptrf->StatusSuccessfulOperation_Cam;



    // ===========================
    // занесение сформированных данных о состоянии (в текущий момент времени)
    // ===========================

    quint8 q01 = settings_ptrf->Aurora.StatusStateInformation.StatusSuccessfulOperation;          // отчёт о завершении оконтуривания и готовности передать контур объекта в БПУ АНПА


    //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::2222";

    //    //obj_ptrf->StartTcpClient();

    // ===========================
    // Подключение к серверу
    // ===========================
    QString TCPhostIP_C = QString::fromStdString(settings_ptrf->TCP.TCPhostIP_C);
    quint16 TCPhostPort_C = settings_ptrf->TCP.TCPhostPort_C;


    obj_ptrf->ConnectToServer(TCPhostIP_C, TCPhostPort_C);

    //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::3333";
    // ===========================
    // Отправка пакета
    // ===========================
    obj_ptrf->SendToServer_StatusStateInformation(q01);


    //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::END";


//    // ===========================
//    // в другом месте slot_TcpServetToForm_Receive
//    // ===========================

//    settings_ptrf->Aurora.OutsideInformationANPA.GeographicCoordinates = obj_ptrf->GeographicCoordinates_S;

//    settings_ptrf->Aurora.OutsideInformationANPA.HardwareTime = obj_ptrf->HardwareTime_S;
//    settings_ptrf->Aurora.OutsideInformationANPA.FloorDistance = obj_ptrf-> FloorDistance_S;
//    settings_ptrf->Aurora.OutsideInformationANPA.RollANPA = obj_ptrf-> RollANPA_S;
//    settings_ptrf->Aurora.OutsideInformationANPA.TrimANPA = obj_ptrf-> TrimANPA_S;

//    settings_ptrf->Aurora.OutsideInformationANPA.AngularVelocity = obj_ptrf-> AngularVelocity_S;

//    settings_ptrf->Aurora.OutsideInformationANPA.ApparentAcceleration = obj_ptrf-> ApparentAcceleration_S;

//    settings_ptrf->Aurora.OutsideInformationANPA.AbsoluteVelocity = obj_ptrf-> AbsoluteVelocity_S;












    // =====================================
    // Создание папки
    // =====================================



    // string path_Log =   "./files/Aurora/Log/";
    string path_Log =   settings_ptrf->Aurora.Aurora_AuroraLogPath;


#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    int result01 = _mkdir(path_Log.c_str());
    if (result01==0 )
        cout << "Aurora::Log:: Folder WriteOneStereoPair is made \n";
    else
        cout << "Aurora::Log:: Don't made Folder WriteOneStereoPair \n";


// ================
// Only Windows END
// ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(path_Log.c_str(), 0777) == -1  )
        cout << "Aurora::Log:: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "Aurora::Log:: Don't made Folder WriteOneStereoPair \n";

// ================
// Only Linux END
// ================

#endif


    // 03 Test TCP
    // time_t t = std::time(NULL);
    QDateTime DateTime = QDateTime::currentDateTime();
    qint64 t_ms = DateTime.toMSecsSinceEpoch();



    // =====================================
    // Пути к файлам
    // =====================================
    stringstream filename_Log_OjectDetection;

    filename_Log_OjectDetection << path_Log  << "Log_StatusSuccessfulOperation.txt";

    // =====================================
    //  Сохранение данных об обнаруженном объекте в текстовый файл
    // =====================================

    std::ofstream out3;          // поток для записи
    out3.open(filename_Log_OjectDetection.str(), ios_base::app); // окрываем файл для записи


    if (out3.is_open() )
    {

        out3
            << DateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString() <<  ";"
            << " Status Successful Operation:: " << settings_ptrf->Aurora.StatusStateInformation.StatusSuccessfulOperation
            << endl;

    }

    out3.close();




    // +++++++++++++++++++++++++++
    // Приведение в исходное состояние статусов состояния
   // +++++++++++++++++++++++++++
    // Реализация так себе. Как сделать по другому не понятно

    settings_ptrf->Aurora.StatusStateInformation.StatusSuccessfulOperation = 0; // информация о состоянии работоспособности системы


    // +++++++++++++++++++++++++++
    // информация о состоянии (в текущий момент времени)
    // +++++++++++++++++++++++++++
    obj_ptrf->StatusSuccessfulOperation_Cam = 0;


}


void form01_mainSettings::on_checkBox_Send_MomentOjectDetection_clicked()
{

     //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::BIGIN";
     // timer_value_TCP ++; // нет необходимости посчитывать

//    cout << " M (!ui->checkBox_StartTcpClient->isChecked() )" << (!ui->checkBox_StartTcpClient->isChecked());
//    cout << " M (!ui->checkBox_Send_MomentOjectDetection->isChecked()) " << (!ui->checkBox_Send_MomentOjectDetection->isChecked());

     // Условия не выполнения отправки
      if (!ui->checkBox_StartTcpClient->isChecked()) return;
     if (!ui->checkBox_Send_MomentOjectDetection->isChecked()) return;


     // Если объект не обнаружен то пропуск
     //if (!settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection) return;



    // if ( settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection !=1) return;

    // if (!obj_ptrf->TcpClientConnectToServerisValid()) return;|| проверка сокета на дееспособность socket_C->isValid()




    // 03 Test TCP
     // time_t t = std::time(NULL);
     QDateTime DateTime = QDateTime::currentDateTime();
     qint64 t_ms = DateTime.toMSecsSinceEpoch();



     // ===========================
     // занесение сформированных данных о факте обнаружения объкта (в момент обнаружения)
     // ===========================
    settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection = obj_ptrf->FuctOjectDetection_DS;
    settings_ptrf->Aurora.MomentOjectDetection.Time = t_ms;

    settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates[0] = settings_ptrf->Aurora.OutsideInformationANPA.GeographicCoordinates[0];
    settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates[1] = settings_ptrf->Aurora.OutsideInformationANPA.GeographicCoordinates[1];


    settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[0] = obj_ptrf->OjectDetection_x_DS;
    settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[1] = obj_ptrf->OjectDetection_y_DS;
    settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[2] = obj_ptrf->OjectDetection_z_DS;
    settings_ptrf->Aurora.MomentOjectDetection.ObjectDistance =  obj_ptrf->ObjectDistance_DS;

    settings_ptrf->Aurora.MomentOjectDetection.ContouringCompletion = obj_ptrf->ContouringCompletion_DS;





    settings_ptrf->Aurora.MomentOjectDetection.DetectObjSize.ObjLength = obj_ptrf->get_vuxyzrgbColorObj().ObjSize_ObjLength;
    settings_ptrf->Aurora.MomentOjectDetection.DetectObjSize.ObjWidth = obj_ptrf->get_vuxyzrgbColorObj().ObjSize_ObjWidth;




    // ===========================
    // данные для отправки по TCP (в момент обнаружения)
    // ===========================
    quint8 q00 = settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection;             // факт обнаружения объекта
    double q01 = settings_ptrf->Aurora.MomentOjectDetection.Time;                           // время обнаружения объекта с фиксацией на кадре
    std::vector<float> q02 = settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates;   // географические координаты объекта с фиксацией на кадре
    float q03 = settings_ptrf->Aurora.MomentOjectDetection.ObjectDistance;                 // дальность до обнаруженного объекта
    std::vector<float> q04 = settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates; // линейные координаты обнаруженного объекта в системе координат СТЗ
    quint8 q05 = settings_ptrf->Aurora.MomentOjectDetection.ContouringCompletion;          // отчёт о завершении оконтуривания и готовности передать контур объекта в БПУ АНПА


     //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::2222";

//    //obj_ptrf->StartTcpClient();



    // ===========================
    // Отпавка изображения через буфер
    cv::Mat img = obj_ptrf->getImgFindColorObj();
    //if (img.empty()) return;

    QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    // ===========================





     // ===========================
     // Подключение к серверу
     // ===========================
    QString TCPhostIP_C = QString::fromStdString(settings_ptrf->TCP.TCPhostIP_C);
    quint16 TCPhostPort_C = settings_ptrf->TCP.TCPhostPort_C;


    obj_ptrf->ConnectToServer(TCPhostIP_C, TCPhostPort_C);

    //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::3333";
    // ===========================
    // Отправка пакета
    // ===========================
    obj_ptrf->SendToServer_MomentObjectDetection(q00, q01, q02, q03, q04, q05, qimg);


    //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::END";






     // =====================================
     // =====================================


    // =====================================
    // Создание папки
    // =====================================



//     string path_Log =   "./files/Aurora/Log/";
    string path_Log =   settings_ptrf->Aurora.Aurora_AuroraLogPath;


#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    int result01 = _mkdir(path_Log.c_str());
    if (result01==0 )
        cout << "Aurora::Log:: Folder WriteOneStereoPair is made \n";
    else
        cout << "Aurora::Log:: Don't made Folder WriteOneStereoPair \n";


// ================
// Only Windows END
// ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(path_Log.c_str(), 0777) == -1  )
        cout << "Aurora::Log:: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "Aurora::Log:: Don't made Folder WriteOneStereoPair \n";

// ================
// Only Linux END
// ================

#endif



    // ===============================
    // Перевод времени на виде (сек) в (мин:сек)
    //================================
    double time1_d = settings_ptrf->CurrentTime_WriteResultVideo;
    //    int min = (int) time/60;
    //    int sec = (int) time - min;

    string time1 = QDateTime::fromMSecsSinceEpoch( time1_d * 1000, Qt::UTC ).toUTC().toString("hh:mm:ss,zzz").toStdString();

    double time2_d = time1_d + settings_ptrf->TCP.TCPsetTimerInterval/1000;
    string time2 =  QDateTime::fromMSecsSinceEpoch( time2_d * 1000, Qt::UTC ).toUTC().toString("hh:mm:ss,zzz").toStdString();



    // =====================================
    // Пути к файлам
    // =====================================
    stringstream filename_Log_OjectDetection;

    filename_Log_OjectDetection << path_Log  << "Log_OjectDetection.txt";



    // =====================================
    //  Сохранение данных об обнаруженном объекте в текстовый файл
    // =====================================

    std::ofstream out3;          // поток для записи
    out3.open(filename_Log_OjectDetection.str(), ios_base::app); // окрываем файл для записи


    if (out3.is_open() )
    {

        out3
            << DateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString() <<  "; "
            << " MomentOjectDetection: " << settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection
            << endl

            << "GeographicalCoordinates: "
            << settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates[0] <<  " с.ш. "
            << settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates[1] <<  " в.д. "
            << endl

            << "LinearCoordinates: x = "
            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[0] << ", y = "
            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[1] << ", z = "
            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[2] << "; "
            << endl

            << "ObjectDistance: "
            << settings_ptrf->Aurora.MomentOjectDetection.ObjectDistance << "; " << endl

            << "ContouringCompletion: "
            << settings_ptrf->Aurora.MomentOjectDetection.ContouringCompletion << "; "
            << endl

            << "File Name: "
            << settings_ptrf->fileName_WriteSourceVideo.at(0) << "; "
            << endl

            << "Time Detection in file: "
            << time1 << "; "

            << endl


            << " ====================== " << endl;

    }

    out3.close();


    // =====================================

    // =====================================
    // Сохранение информация об обнаруженном объекте в csv Log файл
    // =====================================

    // Пути к файлам
    stringstream filename_Log_CSV;
    filename_Log_CSV << path_Log  << "Log_OjectDetection.csv";


    // QFileInfo для проверки файла на существование
    bool chechFileExist = QFileInfo::exists( QString::fromStdString(filename_Log_CSV.str()) );


    std::ofstream out4;          // поток для записи
    out4.open(filename_Log_CSV.str(), ios_base::app); // окрываем файл для записи

    // ===============================
    // Пропись заголовков столбцов в новом csv-файле, когда он не существовал до этого
    //================================

    if(  !chechFileExist  ) // проверка файла на существование
    {

        if (out4.is_open() )
        {

            out4
                << "current Date; "
                << "current Time; "
                << "Moment Oject Detection; "
                << "Geographical Coordinates (latitude); "
                << "Geographical Coordinates (longitude); "
                << "Linear Coordinates (x); "
                << "Linear Coordinates (y); "
                << "Linear Coordinates (z); "
                << "Object Distance; "
                << "Contouring Completion; "
                << "File Name; "
                << "Time Detection in file; "

                << "Obj Length ; "
                << "Obj Width ; "

                << endl;

        }

        qDebug() << "Log_OjectDetection.csv:: Created ";
    }






    if (out4.is_open() )
    {

        out4
            //            << "current Date; " << "current Time; "
            //            << "Moment Oject Detection; "
            //            << "Geographical Coordinates (latitude); "
            //            << "Geographical Coordinates (longitude); "
            //            << "Linear Coordinates; "
            //            << "Linear Coordinates (x); "
            //            << "Linear Coordinates (y); "
            //            << "Linear Coordinates (z); "
            //            << "Object Distance; "
            //            << "Contouring Completion; " << endl



            << QDateTime::currentDateTime().toString("yyyy-MM-dd; ").toStdString()
            << QDateTime::currentDateTime().toString("hh:mm:ss; ").toStdString()

            << settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection  <<  "; "

            << settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates[0] <<  "; "
            << settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates[1] <<  "; "

            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[0] <<  "; "
            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[1] <<  "; "
            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[2] <<  "; "

            << settings_ptrf->Aurora.MomentOjectDetection.ObjectDistance << "; "

            << settings_ptrf->Aurora.MomentOjectDetection.ContouringCompletion << "; "

            << settings_ptrf->fileName_WriteSourceVideo.at(0) << "; "
            << time1 << "; "


            << settings_ptrf->Aurora.MomentOjectDetection.DetectObjSize.ObjLength << "; "
            << settings_ptrf->Aurora.MomentOjectDetection.DetectObjSize.ObjWidth << "; "

            << endl;


    }

    out4.close();





    // =====================================
    // Сохранение информация об обнаруженном объекте в субтитры str для виде файлов
    // =====================================

    // Пути к файлам
    stringstream filename_Subtitle_SRT;
    string path_Subtitle = settings_ptrf->fileName_WriteResultVideo;
    // Возращаемся в строке path_Subtitle на 3 позиции назад с конца
    if (!path_Subtitle.empty() && path_Subtitle.size() > 4)
    {
        for(int qc = 0; qc < 3; qc++) path_Subtitle.pop_back();
    }


    filename_Subtitle_SRT << path_Subtitle  << "srt";


    std::ofstream out5;          // поток для записи
    out5.open(filename_Subtitle_SRT.str(), ios_base::app); // окрываем файл для записи


    if (out5.is_open() )
    {

        out5
            << endl

            << countStrSubtitle
            << endl
                << time1 << "\t --> \t" << time2
            << endl


            << DateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString() <<  "; "
            << endl

            << "GeographicalCoordinates: "
            << settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates[0] <<  " с.ш. "
            << settings_ptrf->Aurora.MomentOjectDetection.GeographicalCoordinates[1] <<  " в.д. "
            << endl

            << "LinearCoordinates: x = "
            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[0] << ", y = "
            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[1] << ", z = "
            << settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[2] << "; "
            << endl

            << " Object Dimensions :  "
                << time1_d << "  " << time2_d

            << endl ;


    }

    out5.close();

    countStrSubtitle++; // Номер записи в субтитрах для видео файла



//    settings_ptrf->TCP.TCPsetTimerInterval




    // Переделано под сигнал для запуска отправки изображения из on_checkBox_Send_MomentOjectDetection_clicked
    if (settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection > 0)
    {
        // сигнал для запуска отправки изображения по сигналу, только в случаи обнаружения объекта
        // ++++++++++++++++++++++++++++++++
        emit signal_Send_ObjectDetectionCompressedImg();
        // ++++++++++++++++
    }



    // +++++++++++++++++++++++++++
    // Приведение в исходное состояние статусов состояния о факте обнаружения объкта (в момент обнаружения)
    // +++++++++++++++++++++++++++

    settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection = 0;      // факт обнаружения объекта

    settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[0] = -1;   // x: линейные координаты обнаруженного объекта в системе координат СТЗ
    settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[1] = -1;   // y:
    settings_ptrf->Aurora.MomentOjectDetection.LinearCoordinates[2] = -1;   // z:
    settings_ptrf->Aurora.MomentOjectDetection.ObjectDistance = -1;         // дальность до обнаруженного объекта

    settings_ptrf->Aurora.MomentOjectDetection.ContouringCompletion = 0;    // отчёт о завершении оконтуривания и готовности передать контур объекта в БПУ АНПА

    // +++++++++++++++++++++++++++
    //
    // +++++++++++++++++++++++++++
    obj_ptrf->FuctOjectDetection_DS = 0; // факт обнаружения объекта

    obj_ptrf->OjectDetection_x_DS = -1;     // x: линейные координаты обнаруженного объекта в системе координат СТЗ
    obj_ptrf->OjectDetection_y_DS = -1;     // y:
    obj_ptrf->OjectDetection_z_DS = -1;     // z:
    obj_ptrf->ObjectDistance_DS = -1;       // дальность до обнаруженного объекта

    obj_ptrf->ContouringCompletion_DS = 0;  // отчёт о завершении оконтуривания и готовности передать контур объекта в БПУ АНПА




}

// ============================
// Функция старта клиента
// ============================
void form01_mainSettings::on_checkBox_StartTcpClient_clicked()
{



    // ===========================
    // Для Подключение к серверу
    // ===========================

    QString TCPhostIP_C = QString::fromStdString(settings_ptrf->TCP.TCPhostIP_C);
    quint16 TCPhostPort_C = settings_ptrf->TCP.TCPhostPort_C;



//    qDebug() << " S1 (ui->checkBox_StartTcpClient->isChecked())  ::" << ui->checkBox_StartTcpClient->isChecked();



    if (ui->checkBox_StartTcpClient->isChecked())
    {
        obj_ptrf->StartTcpClient();
         qDebug() << "on_checkBox_StartTcpClient_clicked()::obj_ptrf->StartTcpClient():: ... ";

        // ===================================
        // !!! уже есть внутри методов при каждой отправки сообщений
        // ===================================
        obj_ptrf->ConnectToServer(TCPhostIP_C, TCPhostPort_C);
        // ===================================

    }

    if (!ui->checkBox_StartTcpClient->isChecked() )
    {
        obj_ptrf->CloseTcpClient();
        qDebug() << "on_checkBox_StartTcpClient_clicked()::obj_ptrf->CloseTcpClient():: ... ";
    }


    // ===================================
    // Отправка сигнала запуск клиента TCP и запуск таймера на переодическую отправку сообщений
    // ===================================
    // form01_mainSettings::slot_Initiate_timerTCP(); // отключено так как старт не автоматически, а по нажатию старта программы
    // ===================================


  //  qDebug() <<  " S2 (ui->checkBox_StartTcpClient->isChecked()  :: " << ui->checkBox_StartTcpClient->isChecked();


}


// ============================
//
// ============================
void form01_mainSettings::TimerSlot_f01()
{
    timer_value ++;
    ui->label_ShowTimer->setText(QDate::currentDate().toString("yyyy.MM.dd") + "  " +
                                 QTime::currentTime().toString("hh:mm:ss") + "  " +
                                 QString::number(timer_value));


}

// ============================
//
// ============================
void form01_mainSettings::slot_Initiate_timerTCP()
{


    // ============================
    // Функция старта клиента
    // ============================
    // obj_ptrf->StartTcpClient();

    // ===========================
    // Для Подключение к серверу
    // ===========================

    QString TCPhostIP_C = QString::fromStdString(settings_ptrf->TCP.TCPhostIP_C);
    quint16 TCPhostPort_C = settings_ptrf->TCP.TCPhostPort_C;




//  // вариант 01
//    obj_ptrf->ConnectToServer(TCPhostIP_C, TCPhostPort_C);
//    // =================================
//    // Запуск циклической отправки по TCP по сигналу от таймера
//    // =================================
//    timer_TCP->start();            // запуск таймера (время в милисикундах)


//  // вариант 02
    if (ui->checkBox_StartTcpClient->isChecked() )
        {

           // obj_ptrf->StartTcpClient(); // запуск клиента TCP

            // Запуск циклической отправки по TCP по сигналу от таймера
            timer_TCP->start();            // запуск таймера (время в милисикундах)


            qDebug() << "form01_mainSettings::TimerSlot_SendTCP():: START timer_TCP ";


            // уже есть внутри методов при каждой отправки сообщений
          //  obj_ptrf->ConnectToServer(TCPhostIP_C, TCPhostPort_C);

        }


      if  (!ui->checkBox_StartTcpClient->isChecked() )
        {

          timer_TCP->stop();            // остановка таймера (время в милисикундах)
          // obj_ptrf->CloseTcpClient();  // остановка клиента TCP


            qDebug() << "form01_mainSettings::TimerSlot_SendTCP():: STOP timer_TCP ";
        }








//    // вариант 03
//    if (ui->checkBox_StartTcpServer->isChecked() &&
//            ui->checkBox_Receive_OutsideInformationANPA->isChecked())
//    {


//        // =================================
//        // Запуск циклической отправки по TCP по сигналу от таймера
//        // =================================
//        timer_TCP->start();            // запуск таймера (время в милисикундах)

//        qDebug() << "form01_mainSettings::TimerSlot_SendTCP():: START timer_TCP";
//    }


//    else
//    {
//        timer_TCP->stop();            // остановка таймера (время в милисикундах)



//        qDebug() << "form01_mainSettings::TimerSlot_SendTCP():: CLOSE timer_TCP";
//    }


}





void form01_mainSettings::slot_StopAllTCP()
{

    timer_TCP->stop(); // остановка таймера TCP для прекращения отправки и получения данных по TCP

}



void form01_mainSettings::slot_StartAllTCP()
{

    timer_TCP->start(); // запуск таймера TCP для прекращения отправки и получения данных по TCP

}

void form01_mainSettings::slot_StartAuroraLantern()
{

    bool checked = ui->checkBox_SwitchAuroraLantern->isChecked();

    if (!checked)
    {
        ui->checkBox_SwitchAuroraLantern->setChecked(!checked);
        form01_mainSettings::on_checkBox_SwitchAuroraLantern_clicked(!checked);

    }






}

void form01_mainSettings::slot_StopAuroraLantern()
{


    bool checked = ui->checkBox_SwitchAuroraLantern->isChecked();

    if (checked)
    {
        ui->checkBox_SwitchAuroraLantern->setChecked(!checked);
        form01_mainSettings::on_checkBox_SwitchAuroraLantern_clicked(!checked);

    }


}


void form01_mainSettings::on_checkBox_StartTcpServer_clicked()
{

    // назначение порта, который прослушивает сервер
    obj_ptrf->setHostPort_S(settings_ptrf->TCP.TCPhostPort_S);


//    cout << " ui->checkBox_StartTcpServer->isChecked() (1) =  " << ui->checkBox_StartTcpServer->isChecked();


    if (ui->checkBox_StartTcpServer->isChecked())
    {

        qDebug() << "StartTcpServer :: Begin ... ";
        obj_ptrf->StartTcpServer();
        qDebug() << "StartTcpServer :: OK ";

    }
    else
    {
        qDebug() << "StartTcpServer :: DISCONNECT ";

        obj_ptrf->StopTcpServer();

//        cout << " ui->checkBox_StartTcpServer->isChecked() (2) = " << ui->checkBox_StartTcpServer->isChecked();
    }


    //if (!ui->checkBox_StartTcpServer->isChecked()) obj_ptrf->CloseTcpServer();





//    // Соеденение сигнала и слота для Окна Настроек
//    connect(this, &form01_mainSettings::signal_Form01ToTcpServer,
//            obj_ptrf->StartTcpServer(), &TCPServer::slot_Form01ToTcpServer);

//    // Отсылаем также для Окна Настроек через сигнал-слот
//    emit signal_Form01ToTcpServer(obj_ptrf, settings_ptrf);
//    // =================================



}



void form01_mainSettings::on_checkBox_Receive_OutsideInformationANPA_clicked()
{




}





void form01_mainSettings::on_spinBox_X_Low_valueChanged(int arg1)
{
    settings_ptrf->limit_WorkingArea[0] = ui->spinBox_X_Low->value();
}


void form01_mainSettings::on_spinBox_Y_Low_valueChanged(int arg1)
{
    settings_ptrf->limit_WorkingArea[1] = ui->spinBox_Y_Low->value();
}


void form01_mainSettings::on_spinBox_Z_Low_valueChanged(int arg1)
{
    settings_ptrf->limit_WorkingArea[2] = ui->spinBox_Z_Low->value();
}


void form01_mainSettings::on_spinBox_X_Up_valueChanged(int arg1)
{
    settings_ptrf->limit_WorkingArea[3] = ui->spinBox_X_Up->value();
}


void form01_mainSettings::on_spinBox_Y_Up_valueChanged(int arg1)
{
    settings_ptrf->limit_WorkingArea[4] = ui->spinBox_Y_Up->value();
}


void form01_mainSettings::on_spinBox_Z_Up_valueChanged(int arg1)
{
    settings_ptrf->limit_WorkingArea[5] = ui->spinBox_Z_Up->value();
}


void form01_mainSettings::on_spinBox_smbBlockSize_valueChanged(int arg1)
{
    settings_ptrf->smbBlockSize = ui->spinBox_smbBlockSize->value();
}


void form01_mainSettings::on_spinBox_smbPreFilterCap_valueChanged(int arg1)
{
    settings_ptrf->smbPreFilterCap = ui->spinBox_smbPreFilterCap->value();
}


void form01_mainSettings::on_spinBox_smbMinDisparity_valueChanged(int arg1)
{
    settings_ptrf->smbMinDisparity = ui->spinBox_smbMinDisparity->value();
}


void form01_mainSettings::on_spinBox_smbTextureThreshold_valueChanged(int arg1)
{
    settings_ptrf->smbTextureThreshold = ui->spinBox_smbTextureThreshold->value();
}


void form01_mainSettings::on_doubleSpinBox_coef_pow_dbs_valueChanged(double arg1)
{
    settings_ptrf->coef_pow_dbs = ui->doubleSpinBox_coef_pow_dbs->value();
}


void form01_mainSettings::on_doubleSpinBox_coef_base_dbs_valueChanged(double arg1)
{
    settings_ptrf->coef_base_dbs = ui->doubleSpinBox_coef_base_dbs->value();
}


void form01_mainSettings::on_spinBox_smbUniquenessRatio_valueChanged(int arg1)
{
    settings_ptrf->smbUniquenessRatio = ui->spinBox_smbUniquenessRatio->value();
}


void form01_mainSettings::on_spinBox_smbSpeckleWindowSize_valueChanged(int arg1)
{
    settings_ptrf->smbSpeckleWindowSize = ui->spinBox_smbSpeckleWindowSize->value();
}


void form01_mainSettings::on_spinBox_smbSpeckleRange_valueChanged(int arg1)
{
    settings_ptrf->smbSpeckleRange = ui->spinBox_smbSpeckleRange->value();
}


void form01_mainSettings::on_spinBox_smbDisp12MaxDiff_valueChanged(int arg1)
{
    settings_ptrf->smbDisp12MaxDiff = ui->spinBox_smbDisp12MaxDiff->value();
}


void form01_mainSettings::on_spinBox_filter_ClusterPoints_valueChanged(int arg1)
{
    settings_ptrf->filter_ClusterPoints = ui->spinBox_filter_ClusterPoints->value();
}


void form01_mainSettings::on_spinBox_dbsNeighbors_valueChanged(int arg1)
{
    settings_ptrf->neighbors = ui->spinBox_dbsNeighbors->value();
}


void form01_mainSettings::on_spinBox_dbsRad_valueChanged(int arg1)
{
    settings_ptrf->rad = ui->spinBox_dbsRad->value();
}


void form01_mainSettings::on_comboBox_MetodDisparity_currentIndexChanged(int index)
{
    //  Метод получения карты диспаратности:
    //  0 - BM;
    //  1 - MODE_SGBM;
    //  2 - MODE_HH;
    //  3 - MODE_SGBM_3WAY;
    //  4 - MODE_HH4;

    settings_ptrf->metod_Disparity = index;
}


void form01_mainSettings::on_checkBox_PointCorrectinForClastering_stateChanged(int arg1)
{
//    settings_ptrf->metod_PointCorrectinForClastering = bool(arg1);
}


void form01_mainSettings::on_comboBox_ImagePreProcessing_currentIndexChanged(int index)
{
    //==============================
    // Параметры для включения фильтров предобработки
    //==============================
    //
    //    0 - без обработки;
    //    1 - equalizeHist (Color RGB);
    //    2 - CLAHE (Color RGB);
    //    3 - CLAHE;
    //    4 - equalizeHist;
    switch (index)
    {
    case 0:

        settings_ptrf->metod_ImagePerProcessing[1] = 0;
        settings_ptrf->metod_ImagePerProcessing[2] = 0;
        settings_ptrf->metod_ImagePerProcessing[3] = 0;
        settings_ptrf->metod_ImagePerProcessing[4] = 0;
        settings_ptrf->metod_ImagePerProcessing[5] = 0;

        break;

    case 1:
        // equalizeHist (Color RGB)
        settings_ptrf->metod_ImagePerProcessing[1] = 1;
        settings_ptrf->metod_ImagePerProcessing[2] = 0;
        settings_ptrf->metod_ImagePerProcessing[3] = 0;
        settings_ptrf->metod_ImagePerProcessing[4] = 0;
        settings_ptrf->metod_ImagePerProcessing[5] = 0;

        break;

    case 2:
        // CLAHE
        settings_ptrf->metod_ImagePerProcessing[1] = 2;
        settings_ptrf->metod_ImagePerProcessing[2] = 0;
        settings_ptrf->metod_ImagePerProcessing[3] = 0;
        settings_ptrf->metod_ImagePerProcessing[4] = 0;
        settings_ptrf->metod_ImagePerProcessing[5] = 0;

        break;

    case 3:
        // equalizeHist
        settings_ptrf->metod_ImagePerProcessing[1] = 3;
        settings_ptrf->metod_ImagePerProcessing[2] = 0;
        settings_ptrf->metod_ImagePerProcessing[3] = 0;
        settings_ptrf->metod_ImagePerProcessing[4] = 0;
        settings_ptrf->metod_ImagePerProcessing[5] = 0;

        break;


    case 4:
        // equalizeHist
        settings_ptrf->metod_ImagePerProcessing[1] = 4;
        settings_ptrf->metod_ImagePerProcessing[2] = 0;
        settings_ptrf->metod_ImagePerProcessing[3] = 0;
        settings_ptrf->metod_ImagePerProcessing[4] = 0;
        settings_ptrf->metod_ImagePerProcessing[5] = 0;

        break;

    case 5:
        // equalizeHist
        settings_ptrf->metod_ImagePerProcessing[1] = 3;
        settings_ptrf->metod_ImagePerProcessing[2] = 4;
        settings_ptrf->metod_ImagePerProcessing[3] = 0;
        settings_ptrf->metod_ImagePerProcessing[4] = 0;
        settings_ptrf->metod_ImagePerProcessing[5] = 0;

        break;

    case 6:
        // equalizeHist
        settings_ptrf->metod_ImagePerProcessing[1] = 0;
        settings_ptrf->metod_ImagePerProcessing[2] = 0;
        settings_ptrf->metod_ImagePerProcessing[3] = 0;
        settings_ptrf->metod_ImagePerProcessing[4] = 0;
        settings_ptrf->metod_ImagePerProcessing[5] = 0;

        break;


    }

}


void form01_mainSettings::on_checkBox_PointCorrectinForClastering_pressed()
{


    // =============================
    // без ! так как стоит внутри этого же слота и isChecked поменяется только после выполнения всего кода слота
    // =============================
    if (ui->checkBox_PointCorrectinForClastering->isChecked())
    {
        settings_ptrf->metod_PointCorrectinForClastering = false;
    }
    else
    {
        settings_ptrf->metod_PointCorrectinForClastering = true;
    }
    // =============================



}


void form01_mainSettings::on_spinBox_limit_points_valueChanged(int arg1)
{
    settings_ptrf->limit_points = arg1;
}




void form01_mainSettings::on_lineEdit_TCPhostIP_C_editingFinished()
{
    settings_ptrf->TCP.TCPhostIP_C = ui->lineEdit_TCPhostIP_C->text().toStdString();
}


void form01_mainSettings::on_lineEdit_TCPhostPort_C_editingFinished()
{
    settings_ptrf->TCP.TCPhostPort_C = ui->lineEdit_TCPhostPort_C->text().toInt();
}


void form01_mainSettings::on_lineEdit_AuroraLogPath_editingFinished()
{
    settings_ptrf->Aurora.Aurora_AuroraLogPath = ui->lineEdit_AuroraLogPath->text().toStdString();
}


void form01_mainSettings::on_lineEdit_AuroraFilesPath_editingFinished()
{
     settings_ptrf->Aurora.Aurora_AuroraFilesPath = ui->lineEdit_AuroraFilesPath->text().toStdString();
}


void form01_mainSettings::on_checkBox_Send_ObjectDetectionImgFile_clicked()
{

    // timer_value_TCP ++;  // нет необходимости посчитывать


    // Условия не выполнения отправки
    if (!ui->checkBox_StartTcpClient->isChecked()) return;
    if (!ui->checkBox_Send_ObjectDetectionImgFile->isChecked()) return;


    //if (!obj_ptrf->TcpClientConnectToServerisValid()) return; // проверка сокета на дееспособность socket_C->isValid()




    // +++++++++++++++++++++++++++
    // информация о состоянии (в текущий момент времени)
    // +++++++++++++++++++++++++++
   // settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection = obj_ptrf->FuctOjectDetection_DS;

    if (settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection > 0)

    {


        qDebug() << "on_checkBox_Send_ObjectDetectionImgFile_clicked::BIGIN";

        // ===========================
        // Подключение к серверу
        // ===========================
        QString TCPhostIP_C = QString::fromStdString(settings_ptrf->TCP.TCPhostIP_C);
        quint16 TCPhostPort_C = settings_ptrf->TCP.TCPhostPort_C;


        obj_ptrf->ConnectToServer(TCPhostIP_C, TCPhostPort_C);

        //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::3333";
        // ===========================
        // Отправка пакета
        // ===========================

        // Отпавка через файл

        QString filePathName = "./files/Aurora/Aurora_Send/imgFindColorObj_1012.jpg";
        obj_ptrf->SendToServer_ObjectDetectionImgFile(filePathName);





        qDebug() << "on_checkBox_Send_ObjectDetectionImgFile_clicked::END";

    }


}





void form01_mainSettings::on_checkBox_Send_ObjectDetectionCompressedImg_clicked()
{
    // timer_value_TCP ++;  // нет необходимости посчитывать


    // Условия не выполнения отправки
    if (!ui->checkBox_StartTcpClient->isChecked()) return;
    if (!ui->checkBox_Send_ObjectDetectionCompressedImg->isChecked()) return;


    // if (!obj_ptrf->TcpClientConnectToServerisValid()) return; // проверка сокета на дееспособность socket_C->isValid()




    // +++++++++++++++++++++++++++
    // информация о состоянии (в текущий момент времени)
    // +++++++++++++++++++++++++++
    // settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection = obj_ptrf->FuctOjectDetection_DS;



    // Это условие не работает, так как внутри  on_checkBox_Send_MomentOjectDetection_clicked - происходит обнуление переменных обнаружения
    // Переделано под сигнал  для запуска отправки изображения из on_checkBox_Send_MomentOjectDetection_clicked
    // if (settings_ptrf->Aurora.MomentOjectDetection.FuctOjectDetection > 0)

    {


        qDebug() << "on_checkBox_Send_ObjectDetectionCompressedImg_clicked::BIGIN";

        // ===========================
        // Подключение к серверу
        // ===========================
        QString TCPhostIP_C = QString::fromStdString(settings_ptrf->TCP.TCPhostIP_C);
        quint16 TCPhostPort_C = settings_ptrf->TCP.TCPhostPort_C;


        obj_ptrf->ConnectToServer(TCPhostIP_C, TCPhostPort_C);

        //qDebug() << "on_checkBox_Send_MomentOjectDetection_pressed::3333";



        // Отпавка через буфер
        cv::Mat img = obj_ptrf->getImgFindColorObj();
        //if (img.empty()) return;

        QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

        obj_ptrf->SendToServer_ObjectDetectionCompressedImg(qimg);



        qDebug() << "on_checkBox_Send_ObjectDetectionCompressedImg_clicked::END";

    }
}


void form01_mainSettings::on_checkBox_SwitchAuroraLantern_clicked(bool checked)
{
     string COMport = settings_ptrf->SerialPort.COMport; // номер компорта
     int value =  ui->spinBox_SwitchAuroraLantern->value();

    if (checked)
    {
        form01_mainSettings::SwitchAuroraLantern(value, COMport);
    }
    else
    {
        form01_mainSettings::SwitchAuroraLantern(0, COMport);
    }

}





void form01_mainSettings::SwitchAuroraLantern(int value, std::string COMport)
{
    QSerialPortInfo info("usb");

    qDebug() << "Name: " << info.portName();
    qDebug() << "Manufacturer: " << info.manufacturer();
    qDebug() << "Busy: " << info.isNull() << Qt::endl;

    QSerialPort *port = new QSerialPort;
    port->setPortName( QString::fromStdString(COMport) );
    port->open(QIODevice::WriteOnly);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);

    qDebug() << "Port named " << port->portName() << " is initted!";


    // Перевод строки в QByteArray
    QByteArray value_ba = QByteArray::number(value);


    qDebug() << "value:: " << value;
    qDebug() << "QByteArray::number(value) " << QByteArray::number(value);
    qDebug() << "ba:: " << value_ba;

    qDebug() << "ba[0]:: " << value_ba[0];
    qDebug() << "ba[0].toInt():: " << value_ba.toInt();
    qDebug() << "ba[1]:: " << value_ba[1];
    qDebug() << "ba[2]:: " << value_ba[2];



    if(port->isOpen(), port->isWritable())
    {
        QTest::qWait(1700);
        port->write( QByteArray::number(value) );
        port->waitForBytesWritten();
        qDebug() << "data has been send" << Qt::endl;



        // =====================================================
        // Готовность к работе регулятора яркости светильника отсека
        // ===================================================
        settings_ptrf->Aurora.InitialStateInformation.ReadyForOperationCompartmentLamp = 1;
        obj_ptrf->ReadyForOperationCompartmentLamp_Cam = 1;




    }
    else
    {
      qDebug() << "An error occured" << Qt::endl;


      // =====================================================
      // Готовность к работе регулятора яркости светильника отсека
      // ===================================================
      settings_ptrf->Aurora.InitialStateInformation.ReadyForOperationCompartmentLamp = 0;
      obj_ptrf->ReadyForOperationCompartmentLamp_Cam = 0;


    }

        QTest::qWait(30);
    port->close();




#if defined(_WIN64)

    // ################################
    Sleep(1000);
    // ################################

#else

    // ################################
    usleep(1000*1000);
    // ################################

#endif




}


void form01_mainSettings::on_spinBox_SwitchAuroraLantern_editingFinished()
{

    string COMport = settings_ptrf->SerialPort.COMport; // номер компорта
    int value = ui->spinBox_SwitchAuroraLantern->value() ;
    form01_mainSettings::SwitchAuroraLantern(value, COMport);

}


void form01_mainSettings::on_checkBox_ShowMarkersImgRTK_00_clicked()
{

    if (!ui->checkBox_ShowMarkersImgRTK_00->isChecked())  settings_ptrf->mode_AddResults.at(0) = 0;
    if ( ui->checkBox_ShowMarkersImgRTK_00->isChecked())  settings_ptrf->mode_AddResults.at(0) = 1;

}


void form01_mainSettings::on_checkBox_ShowMarkersImgRTK_01_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_01->isChecked())  settings_ptrf->mode_AddResults.at(1) = 0;
    if ( ui->checkBox_ShowMarkersImgRTK_01->isChecked())  settings_ptrf->mode_AddResults.at(1) = 1;
}


void form01_mainSettings::on_checkBox_ShowMarkersImgRTK_04_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_04->isChecked())  settings_ptrf->mode_AddResults.at(4) = 0;
    if ( ui->checkBox_ShowMarkersImgRTK_04->isChecked())  settings_ptrf->mode_AddResults.at(4) = 1;
}


void form01_mainSettings::on_checkBox_ShowMarkersImgRTK_05_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_05->isChecked())  settings_ptrf->mode_AddResults.at(5) = 0;
    if ( ui->checkBox_ShowMarkersImgRTK_05->isChecked())  settings_ptrf->mode_AddResults.at(5) = 1;
}


void form01_mainSettings::on_checkBox_ShowMarkersImgRTK_06_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_06->isChecked())  settings_ptrf->mode_AddResults.at(6) = 0;
    if ( ui->checkBox_ShowMarkersImgRTK_06->isChecked())  settings_ptrf->mode_AddResults.at(6) = 1;
}


void form01_mainSettings::on_checkBox_ShowMarkersImgRTK_07_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_07->isChecked())  settings_ptrf->mode_AddResults.at(7) = 0;
    if ( ui->checkBox_ShowMarkersImgRTK_07->isChecked())  settings_ptrf->mode_AddResults.at(7) = 1;
}


void form01_mainSettings::on_checkBox_ShowMarkersImgRTK_02_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_02->isChecked())
    {
        ui->radioButton_ShowMarkersImgRTK_02_1->setChecked(false);
        ui->radioButton_ShowMarkersImgRTK_02_2->setChecked(false);
        settings_ptrf->mode_AddResults.at(2) = 0;
    }

    if ( ui->checkBox_ShowMarkersImgRTK_02->isChecked() &&
        !ui->radioButton_ShowMarkersImgRTK_02_1->isChecked() &&
        !ui->radioButton_ShowMarkersImgRTK_02_2->isChecked())
    {
        ui->radioButton_ShowMarkersImgRTK_02_1->setChecked(true);
        settings_ptrf->mode_AddResults.at(2) = 1;
    }


    if ( ui->checkBox_ShowMarkersImgRTK_02->isChecked() &&
         ui->radioButton_ShowMarkersImgRTK_02_1->isChecked())  settings_ptrf->mode_AddResults.at(2) = 1;

    if ( ui->checkBox_ShowMarkersImgRTK_02->isChecked() &&
         ui->radioButton_ShowMarkersImgRTK_02_2->isChecked())  settings_ptrf->mode_AddResults.at(2) = 2;




}


void form01_mainSettings::on_radioButton_ShowMarkersImgRTK_02_1_clicked()
{

    if (!ui->checkBox_ShowMarkersImgRTK_02->isChecked())   ui->radioButton_ShowMarkersImgRTK_02_1->setChecked(false);

    if ( ui->checkBox_ShowMarkersImgRTK_02->isChecked() &&
        ui->radioButton_ShowMarkersImgRTK_02_1->isChecked())  settings_ptrf->mode_AddResults.at(2) = 1;


}


void form01_mainSettings::on_radioButton_ShowMarkersImgRTK_02_2_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_02->isChecked())   ui->radioButton_ShowMarkersImgRTK_02_2->setChecked(false);

    if ( ui->checkBox_ShowMarkersImgRTK_02->isChecked() &&
        ui->radioButton_ShowMarkersImgRTK_02_2->isChecked())  settings_ptrf->mode_AddResults.at(2) = 2;

}


void form01_mainSettings::on_checkBox_ShowMarkersImgRTK_03_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_03->isChecked())
    {

        ui->radioButton_ShowMarkersImgRTK_03_1->setChecked(false);
        ui->radioButton_ShowMarkersImgRTK_03_2->setChecked(false);
        settings_ptrf->mode_AddResults.at(3) = 0;

    }

    if ( ui->checkBox_ShowMarkersImgRTK_03->isChecked() &&
        !ui->radioButton_ShowMarkersImgRTK_03_1->isChecked() &&
        !ui->radioButton_ShowMarkersImgRTK_03_2->isChecked())
    {
        ui->radioButton_ShowMarkersImgRTK_03_1->setChecked(true);
        settings_ptrf->mode_AddResults.at(3) = 1;
    }


    if ( ui->checkBox_ShowMarkersImgRTK_03->isChecked() &&
        ui->radioButton_ShowMarkersImgRTK_03_1->isChecked())  settings_ptrf->mode_AddResults.at(3) = 1;

    if ( ui->checkBox_ShowMarkersImgRTK_03->isChecked() &&
        ui->radioButton_ShowMarkersImgRTK_03_2->isChecked())  settings_ptrf->mode_AddResults.at(3) = 2;



}


void form01_mainSettings::on_radioButton_ShowMarkersImgRTK_03_1_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_03->isChecked())   ui->radioButton_ShowMarkersImgRTK_03_1->setChecked(false);

    if ( ui->checkBox_ShowMarkersImgRTK_03->isChecked() &&
        ui->radioButton_ShowMarkersImgRTK_03_1->isChecked())  settings_ptrf->mode_AddResults.at(3) = 1;
}


void form01_mainSettings::on_radioButton_ShowMarkersImgRTK_03_2_clicked()
{
    if (!ui->checkBox_ShowMarkersImgRTK_03->isChecked())   ui->radioButton_ShowMarkersImgRTK_03_2->setChecked(false);

    if ( ui->checkBox_ShowMarkersImgRTK_03->isChecked() &&
        ui->radioButton_ShowMarkersImgRTK_03_2->isChecked())  settings_ptrf->mode_AddResults.at(3) = 2;
}

