#include "mainwindow.h"
#include "ui_mainwindow.h"

// ================
// Глобальные переменные
// ================
ADS::cl_DenseStereo *obj_ptr; // указатель оъект класса
str_Settings *settings_ptr;  // указатель на структуру настроек

// Копии
ADS::t_vuxyzrgb *vuxyzrgb_copy ; // указатель на переменную числовых результатов
ADS::type_arma_vuxyzrgb *vuxyzrgb_arma_copy;

// TCP
TCPServer* objTcpServer_ptr; // указатель оъект класса для TCP сервера
TCPClient* objTcpClient_ptr; // указатель оъект класса для TCP клиента

//QString TCPhostIP_C = "192.168.56.1";
//quint16 TCPhostPort_C = 10099;


// Глобальная переменная для выхода из цикла в потоках
//  true - работает
//  false - не работает
vector<bool> exitCode_global (20, false);
// запоминание потоков, которые были запущены до нажатия кнопки стоп
vector<bool> exitCode_global_Remember (20, false);

// Параметр выбора типа выдимого изображения
int selectorImage = 10;

// Парамет выбора способа ввода кординат клика мыши
//int MouseClickSelector = 1;


// Сохранение в переменную калибровки из config-файла файла настроек
QString FirstCalib_filename;


#if defined(_WIN64)
// ================
// Only Windows
// ================

// Задержа между запуском некоторых функций в мс
int delay =500;
// ================
// Only Windows END
// ================

#else

// ================
// Only Linux
// ================

// Задержа между запуском некоторых функций в мс
int delay =500;

// ================
// Only Linux END
// ================

#endif



// Счетчик для нумерации записываемых файлов
int filenameCount = 1001;


//===================================
// Конструктор главного окна
//===================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView01->setScene(new QGraphicsScene(this)); // Необходимо для графического окна




    // =================================
    // Таймер для строки статуса
    // =================================

    // const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();




    // установка фона формы
   // this->setStyleSheet("background-color:rgb(250, 250, 250);");




    // =================================
    // Таймер для строки статуса
    // =================================
    // обнуление счётчика тайм
    timer_Main_dsec = 0;
    timer_Main_sec = 0;
    timer_Main_min = 0;
    timer_Main_hour = 0;
    timer_Main = new QTimer(this);   // инициализация
    connect(timer_Main, SIGNAL(timeout()), this, SLOT(TimerSlot_fMain())); // соединение сигнала со слотом для таймера
    timer_Main->setInterval(100);  // Задаем интервал таймера
    timer_Main->start();            // запуск таймера (время в милисикундах)



//    // Логотип на форму
//    QPixmap myPixmap( "./file/Logo/logo5_100.bmp" );
//    ui->label_logo->setPixmap( myPixmap );


    //
    connect(ui->action_Hide, &QAction::triggered, this, &MainWindow::hideView);
    connect(ui->action_Show, &QAction::triggered, this, &MainWindow::showView);
    //setMouseTracking(true);





    //(num_Mode == 1) "Cameras Basler"
    //(num_Mode == 2) "IP-cams (simulator)"
    //(num_Mode == 3) "Load from File"
    //(num_Mode == 4) "Load from Video"
    //(num_Mode == 5) "old Module"
    int num_Mode = 1;

    // =================================
    // Создание обЪекта алгоритмической части программы
    // =================================
    obj_ptr = CreateObj();
    settings_ptr = Create_str_Settings();
    vuxyzrgb_copy = Create_copy_vuxyzrgb();

    // Initialize
    InitializeSettings(settings_ptr);
    InitializeObj(obj_ptr, settings_ptr, num_Mode);



    // =================================
    // Инициализация объекта Дополнительного Окна
    // =================================
    form01_mainSettings01 =  new form01_mainSettings;
    objp_f02_Ruler =  new f02_Ruler;

    // =================================
    // Инициализация объекта Дополнительного Окна
    // =================================
    cl_VideoStreamer* objp_VideoStreamer =  new cl_VideoStreamer;




    //===================================
    // Инициализация нейронной сети для распознания объектов на изображении
    //===================================
    const std::string model_path = settings_ptr->DarkNet.model_path;
    const std::string classes_path = settings_ptr->DarkNet.classes_path;
    ADS::NeuralNetDetector * detector = new ADS::NeuralNetDetector(model_path, classes_path, 640, 640);

    const std::string smodel_path = settings_ptr->DarkNetSeg.model_path;
    const std::string sclasses_path = settings_ptr->DarkNetSeg.classes_path;
    ADS::NeuralNetSegmentator * segmentator = new ADS::NeuralNetSegmentator(smodel_path, sclasses_path, 640, 640);


    // =================================
    // Занесение ссылок на используемые объекты классов в общие данные settings_ptr
    // =================================
    settings_ptr->Objs_ptr.obj_ptr = obj_ptr;
    settings_ptr->Objs_ptr.vuxyzrgb_copy = vuxyzrgb_copy;
    settings_ptr->Objs_ptr.objp_VideoStreamer = objp_VideoStreamer;
    settings_ptr->Objs_ptr.DarkNetDetector = detector;
    settings_ptr->Objs_ptr.DarkNetSegmentator = segmentator;


    //    settings_ptr->Objs_ptr.objTcpServer_ptr = objTcpServer_ptr;
    //    settings_ptr->Objs_ptr.objTcpClient_ptr = objTcpClient_ptr;

    // =================================
    // Соеденение сигнала и слота для отправки текста в информационное окно plainTextEdit_Status
    // =================================
    connect(this, &MainWindow::signal_sendText_plainTextEdit_StateInfo,
            this, &MainWindow::slot_writeText_plainTextEdit_StateInfo);

    connect(obj_ptr, &TCPServer::signal_sendText_plainTextEdit_StateInfo,
            this, &MainWindow::slot_writeText_plainTextEdit_StateInfo);

    connect(obj_ptr, &TCPClient::signal_sendText_plainTextEdit_StateInfo,
            this, &MainWindow::slot_writeText_plainTextEdit_StateInfo);

    connect(form01_mainSettings01, &form01_mainSettings::signal_sendText_plainTextEdit_StateInfo,
            this, &MainWindow::slot_writeText_plainTextEdit_StateInfo);

    connect(objp_f02_Ruler, &f02_Ruler::signal_sendText_plainTextEdit_StateInfo,
            this, &MainWindow::slot_writeText_plainTextEdit_StateInfo);

   // ======



  // =================================
  // ТУТ Реализован другой метод объекты класса создаются отдельно, сейчас все в одном классе obj_ptr, который наследует другие классы
  // =================================


//    // =================================
//    // Создание обЪекта TCP
//    // =================================
//    objTcpServer_ptr = CreateTcpServerObj();
//    objTcpClient_ptr = CreateTcpClientObj();


//    // Сделана альтернатива наследованию в obj_ptr, пока что не понятно, какой способ лучше
//    settings_ptr->Objs_ptr.objTcpServer_ptr = objTcpServer_ptr;
//    settings_ptr->Objs_ptr.objTcpClient_ptr = objTcpClient_ptr;





    // ============================
    // Соеденение сигнала и слота от TCP сервера к главной форме для старта программы по команде от стороннего TCP клиента
    // ============================
    connect(obj_ptr, &TCPServer::signal_SendStartCommand,
            this, &MainWindow::slot_ApplyStartCommand); // соединения сигнала disconnected с фукцией deleteLater


    // ============================
    // Соеденение сигнала и слота для отправки запуска измерений линейкой
    // ============================
    connect(this, &MainWindow::signal_SendStartRuler,
            objp_f02_Ruler, &f02_Ruler::slot_StartRuler); // соединения сигнала disconnected с фукцией deleteLater


    // !!! по сигналу не подкодит так как не будет работать в ручном режиме, Записанов в  slot_ApplyStartCommand
    // Отправка TCP сообщения после старта программы - отправка информация о готовности к работе (только при первом запуске или в начале работы программы)
//    connect(obj_ptr, &TCPServer::signal_Send_InitialStateInformation,
//            form01_mainSettings01, &form01_mainSettings::on_checkBox_Send_InitialStateInformation_clicked); // соединения сигнала disconnected с фукцией deleteLater



    // 01 Test TCP

    //    TCPClient* TCPclient_obj = new TCPClient;
    //    TCPclient_obj->StartTcpClient();
    //    TCPclient_obj->ConnectToServer("192.168.56.1", 10099);
    //    TCPclient_obj->SendToServer("111111");

    //    qDebug() << "mainwindow::TCPclient_obj:: " << TCPclient_obj;



    // 02 Test TCP

//        objTcpClient_ptr->StartTcpClient();
//        objTcpClient_ptr->ConnectToServer("192.168.56.1", 10099);
//        objTcpClient_ptr->SendToServer("FFh");

    // 03 Test TCP

//        obj_ptr->StartTcpClient();
//        obj_ptr->ConnectToServer("192.168.56.1", 10099);
//        obj_ptr->SendToServer("333333333");




    // ======
    // Test TCP
    // ======

    qDebug() << "mainwindow::this:: " << this;
    qDebug() << "mainwindow::obj_ptr:: " << obj_ptr;
    qDebug() << "mainwindow::objTcpServer_ptr:: " << objTcpServer_ptr;
    qDebug() << "mainwindow::objTcpClient_ptr:: " << objTcpClient_ptr;

    // ======


    // ++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++

    // =================================
    // Инициализация объекта Дополнительного Окна
    // =================================
    // выше  --  form01_mainSettings01 =  new form01_mainSettings;

    // Соеденение сигнала и слота для Окна Настроек
    connect(this, &MainWindow::signal_MainFormToForm01,
            form01_mainSettings01, &form01_mainSettings::slot_MainFormToForm01);

    // Соеденение сигнала и слота для Окна Настроек
    connect(this, &MainWindow::MainFormToF02_Ruler,
            objp_f02_Ruler, &f02_Ruler::slot_MainFormToF02_Ruler);


    // Отсылаем также для Окна Настроек через сигнал-слот
    emit signal_MainFormToForm01(obj_ptr, settings_ptr);
    // =================================


    // Отсылаем также для Окна Настроек через сигнал-слот
    emit MainFormToF02_Ruler(obj_ptr, settings_ptr);
    // =================================


    // <><><><><><><><><><><><><><><><><><><><><
    // =================================
    // Инициализация объекта класса LOG
    // =================================
    obj_LOG_01 =  new cl_LOG;

    // (01) Соеденение сигнала и слота для LOG
    connect(this, &MainWindow::signal_MainFormToLOG,
            obj_LOG_01, &cl_LOG::slot_MainFormToLOG);              // соединение сигнала со слотом LOG
    // (02) Соединение для передачи расчитанных данных с главной формы в LOG для записи
    connect(this, &MainWindow::signal_MainFormToLOG_AirCraftData,
            obj_LOG_01, &cl_LOG::slot_MainFormToLOG_AirCraftData); // соединение сигнала со слотом передачи данных AirCraft

    // Отсылаем также для LOG через сигнал-слот
    emit signal_MainFormToLOG(obj_ptr, settings_ptr);

    // =================================
    // Таймер (инициализация)
    // =================================
    timer_value_LOG_AirCraft = 0;            // обнуление счётчика тайм
    timer_LOG_AirCraft = new QTimer(this);   // инициализация
    connect(timer_LOG_AirCraft, SIGNAL(timeout()), obj_LOG_01, SLOT(WiriteLogAirCraft3DCoordinates())); // соединение сигнала со слотом для таймера
//    timer_LOG_AirCraft->setInterval(500);  // Задаем интервал таймера
    timer_LOG_AirCraft->setInterval(ui->spinBox_LogDataWriteSpeed_Aircraft->value());  // Задаем интервал таймера
     // =================================
    // Далее запуст Таймер в on_checkBox_WiriteLogAirCraft3DCoordinates_released()
    // =================================




    // =====================================
    // Остановка и запуска приёма и передачи данных по TCP
    // =====================================
    connect(this, &MainWindow::signal_StopAllTCP,
            form01_mainSettings01, &form01_mainSettings::slot_StopAllTCP);

    connect(this, &MainWindow::signal_StartAllTCP,
            form01_mainSettings01, &form01_mainSettings::slot_StartAllTCP);

    // Включение фонаря (при нажании кнопки старт программы)
    connect(this, &MainWindow::signal_StartAuroraLantern,
            form01_mainSettings01, &form01_mainSettings::slot_StartAuroraLantern);
    connect(this, &MainWindow::signal_StopAuroraLantern,
            form01_mainSettings01, &form01_mainSettings::slot_StopAuroraLantern);


    // ======================
    // Авто запуск записи лог файла Airfield
    // ======================

    if (autoStart_WiriteLogAirCraft3DCoordinates)
    {
        on_pushButton_WiriteLogAirCraft3DCoordinates_pressed();
    }


    // ++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++


    // ++++++++++++++++++++++++++++++++++++++
    // Вывод пути файла в строку на ui
    // ++++++++++++++++++++++++++++++++++++++

    FirstCalib_filename = settings_ptr->CurrentCalib_filename.c_str();  // Сохранение в переменную калибровки из config-файла файла настроек
    ui->lineEdit_OpenCalibr->setText(QString::fromUtf8(settings_ptr->CurrentCalib_filename.c_str()));

    switch (ui->comboBoxMain_SelectCam->currentIndex())
    {

    case 1:

        // IP камеры
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->videoStreamAddressLeft.c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->videoStreamAddressRight.c_str() ));

        break;

    case 2:

        // Видео файл
        // Заносим путь к видеофайлу  камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8(settings_ptr->video1Path.c_str()));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8(settings_ptr->video2Path.c_str()));



        break;


    case 3:

        // Изображение
        // Заносим путь к видеофайлу  камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8(settings_ptr->img1Path.c_str()));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8(settings_ptr->img2Path.c_str()));

        break;


    case 6:

        // ======================
        // IP камеры - параллено левая и правая камера (новый)
        // ======================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->videoStreamAddressLeft.c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->videoStreamAddressRight.c_str() ));

        break;


    }


    // ======================
    // расчет глиссады для tableWidget_AirCraft_xyz
    // ======================


    ui->doubleSpinBox_AirCraftParamEquation_ax->setValue(ax);
    ui->doubleSpinBox_AirCraftParamEquation_ay->setValue(ay);
    ui->doubleSpinBox_AirCraftParamEquation_az->setValue(az);
    ui->doubleSpinBox_AirCraftParamEquation_bx->setValue(bx);
    ui->doubleSpinBox_AirCraftParamEquation_by->setValue(by);
    ui->doubleSpinBox_AirCraftParamEquation_bz->setValue(bz);





    // ======================
    // Инициализация параметры на окне приложения из файла настроек
    // ======================

//    ui->spinBox_smbNumDisparities->setValue(settings_ptr->smbNumDisparities);
//    ui->spinBox_smbBlockSize->setValue(settings_ptr->smbBlockSize);
//    ui->spinBox_smbPreFilterCap->setValue(settings_ptr->smbPreFilterCap);

//    ui->spinBox_smbMinDisparity->setValue(settings_ptr->smbMinDisparity);
//    ui->spinBox_smbTextureThreshold->setValue(settings_ptr->smbTextureThreshold);
//    ui->spinBox_smbUniquenessRatio->setValue(settings_ptr->smbUniquenessRatio);

//    ui->spinBox_smbSpeckleWindowSize->setValue(settings_ptr->smbSpeckleWindowSize);
//    ui->spinBox_smbSpeckleRange->setValue(settings_ptr->smbSpeckleRange);
//    ui->spinBox_smbDisp12MaxDiff->setValue(settings_ptr->smbDisp12MaxDiff);


//    ui->spinBox_dbsRad->setValue(settings_ptr->rad);
//    ui->spinBox_dbsNeighbors->setValue(settings_ptr->neighbors);
//    ui->spinBox_filter_ClusterPoints->setValue(settings_ptr->filter_ClusterPoints);

//    ui->doubleSpinBox_coef_base_dbs->setValue(settings_ptr->coef_base_dbs);
//    ui->doubleSpinBox_coef_pow_dbs->setValue(settings_ptr->coef_pow_dbs);




    // ======================
    // Параметры автозапуса GUI
    // ======================

    ui->comboBoxMain_SelectCam->setCurrentIndex(settings_ptr->GuiSelectSorceImg);
    ui->comboBoxMain_ModeWork->setCurrentIndex(settings_ptr->GuiSelectWorkMode);
    ui->comboBoxMain_ImgType->setCurrentIndex(settings_ptr->GuiSelecttTipeShowWindow);
    ui->comboBox_CamType->setCurrentIndex(settings_ptr->GuiSelectCamType);

//    ui->comboBoxMain_ModeWork->activated(settings_ptr->GuiSelectWorkMode);
//    ui->comboBoxMain_SelectCam->activated(settings_ptr->GuiSelectSorceImg);
//    ui->comboBox_CamType->activated(settings_ptr->GuiSelectCamType);
//    ui->comboBoxMain_ImgType->activated(settings_ptr->GuiSelecttTipeShowWindow);

    // Важен порядок активации !!!! Можно потом пределать под произольный порядок
    // Не нашёл как вызвать событие по другому для автозапуса GUI
    MainWindow::on_comboBoxMain_ModeWork_currentIndexChanged(settings_ptr->GuiSelectWorkMode);
    MainWindow::on_comboBoxMain_SelectCam_currentIndexChanged(settings_ptr->GuiSelectSorceImg);
    MainWindow::on_comboBox_CamType_currentIndexChanged(settings_ptr->GuiSelectCamType);
    // ======================





    // ======================
    // Сворачивание окна
    // ======================
    if (settings_ptr->GuiWindowHide) this->showMinimized();





    // ======================
    // Установка галочек
    // ======================

    if ( ui->checkBox_WriteSourceVideo->isChecked() ) exitCode_global.at(18) = true;
    if ( ui->checkBox_WriteResultVideo->isChecked() ) exitCode_global.at(17) = true;
    if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= true;
    if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= true;
    if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= true;
    if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= true;
    if ( ui->checkBox_07->isChecked() ) exitCode_global.at(7)= true;
    if ( ui->checkBox_06->isChecked() ) exitCode_global.at(6)= true;
    if ( ui->checkBox_05->isChecked() ) exitCode_global.at(5)= true;
    if ( ui->checkBox_04->isChecked() ) exitCode_global.at(4)= true;
    if ( ui->checkBox_03->isChecked() ) exitCode_global.at(3)= true;
    if ( ui->checkBox_02->isChecked() ) exitCode_global.at(2)= true;
    if ( ui->checkBox_01->isChecked() ) exitCode_global.at(1)= true;
    if ( ui->checkBox_00->isChecked() ) exitCode_global.at(0)= true;

      settings_ptr->exitCode = exitCode_global;

    qDebug() << " exitCode_global.at(0):: " <<  settings_ptr->exitCode.at(0);
    qDebug() << " exitCode_global.at(1):: " <<  settings_ptr->exitCode .at(1);
    qDebug() << " exitCode_global.at(2):: " <<  settings_ptr->exitCode.at(2);
    qDebug() << " exitCode_global.at(3):: " <<  settings_ptr->exitCode.at(3);
    qDebug() << " exitCode_global.at(4):: " <<  settings_ptr->exitCode.at(4);
    qDebug() << " exitCode_global.at(5):: " <<  settings_ptr->exitCode.at(5);
    qDebug() << " exitCode_global.at(6):: " <<  settings_ptr->exitCode.at(6);
    qDebug() << " exitCode_global.at(7):: " <<  settings_ptr->exitCode.at(7);
    qDebug() << " exitCode_global.at(8):: " <<  settings_ptr->exitCode.at(8);
    qDebug() << " exitCode_global.at(9):: " <<  settings_ptr->exitCode.at(9);
    qDebug() << " exitCode_global.at(10):: " <<  settings_ptr->exitCode.at(10);
    qDebug() << " exitCode_global.at(11):: " <<  settings_ptr->exitCode.at(11);

    qDebug() << " exitCode_global.at(17):: " <<  settings_ptr->exitCode.at(17);
    qDebug() << " exitCode_global.at(18):: " <<  settings_ptr->exitCode.at(18);






    // ======================
    // Инициализация
    // ======================

    ui->spinBox_timeMinThreadDelay_00->setValue( settings_ptr->timeMsMinDelay.at(0) );
    ui->spinBox_timeMinThreadDelay_01->setValue( settings_ptr->timeMsMinDelay.at(1) );
    ui->spinBox_timeMinThreadDelay_02->setValue( settings_ptr->timeMsMinDelay.at(2) );
    ui->spinBox_timeMinThreadDelay_03->setValue( settings_ptr->timeMsMinDelay.at(3) );
    ui->spinBox_timeMinThreadDelay_04->setValue( settings_ptr->timeMsMinDelay.at(4) );

    ui->spinBox_timeMinThreadDelay_05->setValue( settings_ptr->timeMsMinDelay.at(5) );
    ui->spinBox_timeMinThreadDelay_06->setValue( settings_ptr->timeMsMinDelay.at(6) );
    ui->spinBox_timeMinThreadDelay_07->setValue( settings_ptr->timeMsMinDelay.at(7) );

    ui->spinBox_timeMinThreadDelay_08->setValue( settings_ptr->timeMsMinDelay.at(8) );
    ui->spinBox_timeMinThreadDelay_09->setValue( settings_ptr->timeMsMinDelay.at(9) );
    ui->spinBox_timeMinThreadDelay_10->setValue( settings_ptr->timeMsMinDelay.at(10) );
    ui->spinBox_timeMinThreadDelay_11->setValue( settings_ptr->timeMsMinDelay.at(11) );






//    // ======================
//    // Инициализация строк GUI с номерами алгоритмов
//    // ======================

//    // Режим "Управление потоками"

//    vector<int> line00= {-1};
//    //        settings_ptr->Algorithm_inThread_seq.at(1) = {111, 112, 11, 911, 912};
//    //        settings_ptr->Algorithm_inThread_seq.at(2) = {31};
//    //        settings_ptr->Algorithm_inThread_seq.at(3) = {41, 43};
//    //        settings_ptr->Algorithm_inThread_seq.at(4) = {51, 52, 53};
//    //        settings_ptr->Algorithm_inThread_seq.at(5) = {45};


//    // Запись из lineEdit
//    vector<string> xst(settings_ptr->maxNumThread);
//    xst.at(0) = ui->lineEdit_Alg_inThr00->setText();
//    xst.at(1) = ui->lineEdit_Alg_inThr01->text().toStdString();
//    xst.at(2) = ui->lineEdit_Alg_inThr02->text().toStdString();
//    xst.at(3) = ui->lineEdit_Alg_inThr03->text().toStdString();
//    xst.at(4) = ui->lineEdit_Alg_inThr04->text().toStdString();
//    xst.at(5) = ui->lineEdit_Alg_inThr05->text().toStdString();







   // ======================
   // Загрузка Config File в GUI
   // ======================

    QString fileName_ConfigFile = "./files/Config_Files/Config_001.xml";

    QFile file_ConfigFile(fileName_ConfigFile);

    file_ConfigFile.open(QIODevice::ReadOnly);

    QTextStream in1(&file_ConfigFile);

    ui->plainTextEdit_ConfigFile->setPlainText(in1.readAll());

    file_ConfigFile.close();




    // ======================
    // Загрузка Alg List в GUI
    // ======================

    QString fileName_AlgList = "./files/Config_Files/AlgList.txt";

    QFile file_AlgList(fileName_AlgList);

    file_AlgList.open(QIODevice::ReadOnly);

    QTextStream in2(&file_AlgList);

    ui->plainTextEdit_AlgList->setPlainText(in2.readAll());

    file_AlgList.close();





// ======================
// Автозапуск
// ======================
#if defined(_WIN64)
    // ################################
    Sleep(1000);
    // ################################
#else
    // ################################
    usleep(1000*1000);
    // ################################
#endif

    //if (settings_ptr->GuiAutoStart) on_pushButton_Start_released();
    if (settings_ptr->GuiAutoStart) ui->pushButton_Start->animateClick();



  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    emit signal_sendText_plainTextEdit_StateInfo( " Start Program "); // отправка текста в информационное окно
  // >>>>>>>>>>>>>>>>>>>>>>

    toolWindow = new ToolWindow(this);
}

MainWindow::~MainWindow()
{
    delete toolWindow;
    // остановка потоков
    fill(exitCode_global.begin(), exitCode_global.end(), false);


    settings_ptr->exitCode = exitCode_global;
 //   closeBaslerCam(obj_ptr);



    qDebug() << " exitCode_global.at(0):: " <<  settings_ptr->exitCode.at(0);
    qDebug() << " exitCode_global.at(1):: " <<  settings_ptr->exitCode .at(1);
    qDebug() << " exitCode_global.at(2):: " <<  settings_ptr->exitCode.at(2);
    qDebug() << " exitCode_global.at(3):: " <<  settings_ptr->exitCode.at(3);
    qDebug() << " exitCode_global.at(4):: " <<  settings_ptr->exitCode.at(4);
    qDebug() << " exitCode_global.at(5):: " <<  settings_ptr->exitCode.at(5);
    qDebug() << " exitCode_global.at(6):: " <<  settings_ptr->exitCode.at(6);
    qDebug() << " exitCode_global.at(7):: " <<  settings_ptr->exitCode.at(7);
    qDebug() << " exitCode_global.at(8):: " <<  settings_ptr->exitCode.at(8);
    qDebug() << " exitCode_global.at(9):: " <<  settings_ptr->exitCode.at(9);
    qDebug() << " exitCode_global.at(10):: " <<  settings_ptr->exitCode.at(10);
    qDebug() << " exitCode_global.at(11):: " <<  settings_ptr->exitCode.at(11);

    qDebug() << " exitCode_global.at(17):: " <<  settings_ptr->exitCode.at(17);
    qDebug() << " exitCode_global.at(18):: " <<  settings_ptr->exitCode.at(18);

#if defined(_WIN64)
    // ################################
    Sleep(1000);
    // ################################
#else
    // ################################
    usleep(1000*1000);
    // ################################
#endif



    delete obj_ptr;
    free(settings_ptr);
    free(vuxyzrgb_copy);

    delete objTcpServer_ptr;
    delete objTcpClient_ptr;

    delete obj_LOG_01;

    delete form01_mainSettings01;
    delete ui;

}

void MainWindow::hideView()
{

    ui->splitter->setSizes(QList<int>() << ui->splitter->width() << 0);
    //ui->splitter_2->setSizes(QList<int>() << ui->splitter_2->width() << 0);
    ui->splitter_2->moveHandleAt(ui->splitter_2->width());


}

void MainWindow::showView()
{

    double scale = 0.8;
    // Код: https://github.com/offs/geekschool/tree/master/ex1_promote_widgets

    ui->splitter->setSizes(QList<int>() << ui->splitter->width() * scale  << ui->splitter->width() * scale);
    // ui->splitter->setSizes(QList<int>() << ui->splitter->height() * scale  << ui->splitter->height() * scale);

    //ui->splitter_2->setSizes(QList<int>() << ui->splitter_2->width() * scale  << ui->splitter_2->width() * scale);
    ui->splitter_2->moveHandleAt(ui->splitter_2->width() * scale);

}

void MainWindow::FuncCam_Stat()
{

//    // =====================================
//    // Получение изображений
//    // =====================================
//    // Выбор источника изображений:
//    // 1 - Basler камеры
//    // 2 - симулятор IP-камеры
//    //int SelectImgSource = 1;




//    if ( ui->checkBox_01->isChecked() )
//    {

//        switch (ui->comboBoxMain_SelectCam->currentIndex())
//        {
//        case 0:

//            InitializeBaslerCam(obj_ptr);
//            openBaslerCam(obj_ptr);


//#if defined(_WIN64)

//            // ################################
//            Sleep(delay);
//            // ################################

//#else

//            // ################################
//            usleep(delay*1000);
//            // ################################

//#endif




//            CaptureBaslerCamImg_thread(obj_ptr, settings_ptr);


//#if defined(_WIN64)

//            // ################################
//            Sleep(delay);
//            // ################################

//#else

//            // ################################
//            usleep(delay*1000);
//            // ################################

//#endif

//            break;

//        case 1:

//            // Opening IP cams
//            OpenIPcam(obj_ptr, settings_ptr);

//            // Запуск потока
//            CaptureFromIP_thread(obj_ptr, settings_ptr);

//#if defined(_WIN64)

//            // ################################
//            Sleep(delay);
//            // ################################

//#else

//            // ################################
//            usleep(delay*1000);
//            // ################################

//#endif
//            break;

//        case 2:

//            // Opening Stereo cams
//            // OpenVideo(obj_ptr, settings_ptr);

//            // Запуск потока
//            CaptureFromVideo_thread(obj_ptr, settings_ptr);

//#if defined(_WIN64)

//            // ################################
//            Sleep(delay);
//            // ################################

//#else

//            // ################################
//            usleep(delay*1000);
//            // ################################

//#endif
//            break;

//        case 3:

//            СaptureFromImgFile(obj_ptr, settings_ptr);

//#if defined(_WIN64)

//                // ################################
//                Sleep(delay);
//            // ################################

//#else

//                // ################################
//                usleep(delay*1000);
//            // ################################

//#endif


//            break;



//        case 4:





//#if defined(_WIN64)

//                // ################################
//                Sleep(delay);
//            // ################################

//#else

//                // ################################
//                usleep(delay*1000);
//            // ################################

//#endif


//            break;





//        }

//    }

}

void MainWindow::FuncCam_Stop()
{

//    // =====================================
//    // Получение изображений
//    // =====================================
//    // Выбор источника изображений:
//    // 1 - Basler камеры
//    // 2 - симулятор IP-камеры
//    //int SelectImgSource = 1;

//    switch (ui->comboBoxMain_SelectCam->currentIndex())
//    {
//    case 0:

//        stopCaptureBaslerCam(obj_ptr);

//#if defined(_WIN64)

//        // ################################
//        Sleep(delay);
//        // ################################

//#else

//        // ################################
//        usleep(delay*1000);
//        // ################################

//#endif

//        closeBaslerCam(obj_ptr);

//#if defined(_WIN64)

//        // ################################
//        Sleep(delay);
//        // ################################

//#else

//        // ################################
//        usleep(delay*1000);
//        // ################################

//#endif


//        break;

//    case 1:

//#if defined(_WIN64)

//        // ################################
//        Sleep(delay);
//        // ################################

//#else

//        // ################################
//        usleep(delay*1000);
//        // ################################

//#endif
//        break;


//    }




}



void MainWindow::treadFunc_Stating()
{

    // =====================================
    // Получение изображений
    // =====================================
    // Выбор источника изображений:
    // 1 - Basler камеры
    // 2 - симулятор IP-камеры
    //int SelectImgSource = 1;




  // удалить
  //  if (ui->comboBoxMain_ModeWork->currentIndex() != 3) FuncCam_Stat();



    // считывание из lineEdit
    vector<string> xst(settings_ptr->maxNumThread);
    xst.at(0) = ui->lineEdit_Alg_inThr00->text().toStdString();
    xst.at(1) = ui->lineEdit_Alg_inThr01->text().toStdString();
    xst.at(2) = ui->lineEdit_Alg_inThr02->text().toStdString();
    xst.at(3) = ui->lineEdit_Alg_inThr03->text().toStdString();
    xst.at(4) = ui->lineEdit_Alg_inThr04->text().toStdString();
    xst.at(5) = ui->lineEdit_Alg_inThr05->text().toStdString();
    xst.at(6) = ui->lineEdit_Alg_inThr06->text().toStdString();
    xst.at(7) = ui->lineEdit_Alg_inThr07->text().toStdString();
    xst.at(8) = ui->lineEdit_Alg_inThr08->text().toStdString();
    xst.at(9) = ui->lineEdit_Alg_inThr09->text().toStdString();
    xst.at(10) = ui->lineEdit_Alg_inThr10->text().toStdString();
    xst.at(11) = ui->lineEdit_Alg_inThr11->text().toStdString();

    // преобразование string в vector<int>
    for (int q = 0; q < int(settings_ptr->Algorithm_inThread_seq.size()); q++ )
    {
        vector<int> data = string_to_vecint(xst.at(q));
        settings_ptr->Algorithm_inThread_seq.at(q) = data;
    }



    // Запуск потоков, которые были запущены до нажатия кнопки стоп

    if (exitCode_global_Remember.at(0)) {  ui->checkBox_00->setChecked(true); exitCode_global.at(0)= true;}
    if (exitCode_global_Remember.at(1)) {  ui->checkBox_01->setChecked(true); exitCode_global.at(1)= true;}
    if (exitCode_global_Remember.at(2)) {  ui->checkBox_02->setChecked(true); exitCode_global.at(2)= true;}
    if (exitCode_global_Remember.at(3)) {  ui->checkBox_03->setChecked(true); exitCode_global.at(3)= true;}
    if (exitCode_global_Remember.at(4)) {  ui->checkBox_04->setChecked(true); exitCode_global.at(4)= true;}
    if (exitCode_global_Remember.at(5)) {  ui->checkBox_05->setChecked(true); exitCode_global.at(5)= true;}
    if (exitCode_global_Remember.at(6)) {  ui->checkBox_06->setChecked(true); exitCode_global.at(6)= true;}
    if (exitCode_global_Remember.at(7)) {  ui->checkBox_07->setChecked(true); exitCode_global.at(7)= true;}
    if (exitCode_global_Remember.at(8)) {  ui->checkBox_08->setChecked(true); exitCode_global.at(8)= true;}
    if (exitCode_global_Remember.at(9)) {  ui->checkBox_09->setChecked(true); exitCode_global.at(9)= true;}
    if (exitCode_global_Remember.at(10)) { ui->checkBox_10->setChecked(true); exitCode_global.at(10)= true;}
    if (exitCode_global_Remember.at(11)) { ui->checkBox_11->setChecked(true); exitCode_global.at(11)= true;}


    if (exitCode_global_Remember.at(17)) { ui->checkBox_WriteResultVideo->setChecked(true); exitCode_global.at(17)= true;}
    if (exitCode_global_Remember.at(18)) { ui->checkBox_WriteSourceVideo->setChecked(true); exitCode_global.at(18)= true;}

    settings_ptr->exitCode = exitCode_global;




//    // Установка галочек в checkBox  (удалить??)
//    for (int q = 0; q < int(settings_ptr->Algorithm_inThread_seq.size()); q++ )
//    {
//        switch (q)
//        {
//        case 0:
//            if (settings_ptr->exitCode.at(0)) ui->checkBox_00->setChecked(true);
//            break;
//        case 1:
//            if (settings_ptr->exitCode.at(1)) ui->checkBox_01->setChecked(true);
//            break;
//        case 2:
//            if (settings_ptr->exitCode.at(2)) ui->checkBox_02->setChecked(true);
//            break;
//        case 3:
//            if (settings_ptr->exitCode.at(3)) ui->checkBox_03->setChecked(true);
//            break;
//        case 4:
//            if (settings_ptr->exitCode.at(4)) ui->checkBox_04->setChecked(true);
//            break;
//        case 5:
//            if (settings_ptr->exitCode.at(5)) ui->checkBox_05->setChecked(true);
//            break;
//        case 6:
//            if (settings_ptr->exitCode.at(6)) ui->checkBox_06->setChecked(true);
//            break;
//        case 7:
//            if (settings_ptr->exitCode.at(7)) ui->checkBox_07->setChecked(true);
//            break;
//        case 8:
//            if (settings_ptr->exitCode.at(8)) ui->checkBox_08->setChecked(true);
//            break;
//        case 9:
//            if (settings_ptr->exitCode.at(9)) ui->checkBox_09->setChecked(true);
//            break;
//        case 10:
//            if (settings_ptr->exitCode.at(10)) ui->checkBox_10->setChecked(true);
//            break;
//        case 11:
//            if (settings_ptr->exitCode.at(11)) ui->checkBox_11->setChecked(true);
//            break;

//        }


//    }



    // =====================================
    // Запуск потоков
    // =====================================

    for (int q = 0; q < int(settings_ptr->Algorithm_inThread_seq.size()); q++ )
    {

        Start_certainThread(obj_ptr, settings_ptr, q);

    }

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    emit signal_sendText_plainTextEdit_StateInfo( " treadFunc_Stating() "); // отправка текста в информационное окно
  // >>>>>>>>>>>>>>>>>>>>

    if (exitCode_global.at(17)) {MainWindow::on_checkBox_WriteResultVideo_released(); }
    if (exitCode_global.at(18)) {MainWindow::on_checkBox_WriteSourceVideo_released(); }



}


void MainWindow::showImg_Cycle()
{

    // =====================================
    //  Вывод изображений на экран
    // =====================================
    // idx - отображение окна:
    // 0 - откл.
    // 1 - вкл.
    // Альтернатива просто закоментировать


    // selectorImage
    // Получение изображения
    // первый входной аргумент функции - это номер камеры:

    // 0 - StereoCam00left     - левая камеры стереокамеры
    // 1 - StereoCam01right     - правая камеры стереокамеры
    // 2 - HandCam02left      - камера на левой руке манипулятора
    // 3 - HandCam03right     - камера на правой руке манипулятора
    // 9 - Стереопара         - левая + правая камеры стереокамеры

    // 10 - Изображение с результатами - левая камера стереокамеры: imgResult01r;
    // 11 - Изображение карты глубин в цвете: imgResultColorSearch
    // 12 - getDisparityMapImage02 - только карта диспаратности
    // 13 - getDisparityMapImage03  - нанесение всех точек на результирующие изображение

    // 14 - getImgRemapGrayPreProc_StereoPair() вывод чего-то

    // 15 - getImgRemap_StereoPair() вывод левого изображения стреопары перед поиском 3D точек
    // 16 - getImgRemap_StereoPair() вывод правого изображения стреопары перед поиском 3D точек
    // 17 - getImgRemap_DoubleStereoImg() вывод  стреопары перед поиском 3D точек



    // 20 - ОБЩЕЕ изображение с результатами построения примитивов объектов: imgPrimitive01r_general;
    // 21 - изображение отдельной проекции примитивов: imgPrimitivesXY;
    // 22 - изображение отдельной проекции примитивов: imgPrimitivesYZ;
    // 23 - изображение отдельной проекции примитивов: imgPrimitivesXZ;

    // 30 - Изображение результатов поиска по цвету (старое)

    // 40 - Изображение вывод контуров

    // 50 - визуализация 3D точек

    // 70 - результаты работы нейронной сети (изображение с рамками)


    QGraphicsPixmapItem pixmap;
    int idx = 1; // пояснение выше

    cv::Mat frame;

    do {
        // Окно вывода
        //ShowResults(obj_ptr, settings_ptr, idx, selectorImage);


        switch (ui->comboBoxMain_ImgType->currentIndex())
        {
        case 0:

            if (ui->radioButton_3D->isChecked())
            {
                selectorImage = 10;
                frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            }

            if (ui->radioButton_line->isChecked())
            {
                selectorImage = 13;
                frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            }
            break;

        case 1:
            selectorImage = 12;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;

        case 2:
            selectorImage = 25;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;

        case 3:
            selectorImage = 0;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;

        case 4:
            selectorImage = 1;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;

        case 5:
            selectorImage = 9;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;

        case 6:
            selectorImage = 40;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;

        case 7:
            selectorImage = 50;


            //    double angx; // угол поворота вокруг оси x центра вращения
            //    double angy; // угол поворота вокруг оси y центра вращения
            //    double angz; // угол поворота вокруг оси z центра вращения
            //    double tx;   // смещение вдоль оси x центра вращения
            //    double ty;   // смещение вдоль оси y центра вращения
            //    double tz;   // смещение вдоль оси z центра вращения
            //    double Scale; // Масштаб
            //    double Dz;    // смещение центра вращения от камер вдоль оси z

            settings_ptr->Rotation3Dpoints.angx = ui->horizontalSlider_Rx->value();
            settings_ptr->Rotation3Dpoints.angy = ui->horizontalSlider_Ry->value();
            settings_ptr->Rotation3Dpoints.angz = ui->horizontalSlider_Rz->value();
            settings_ptr->Rotation3Dpoints.tx = ui->horizontalSlider_Tx->value();
            settings_ptr->Rotation3Dpoints.ty = ui->horizontalSlider_Ty->value();
            settings_ptr->Rotation3Dpoints.tz = ui->horizontalSlider_Tz->value();
            settings_ptr->Rotation3Dpoints.Scale = ui->horizontalSlider_Scale->value();
            settings_ptr->Rotation3Dpoints.Dz = ui->horizontalSlider_Dz->value();


            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);


            break;

        case 8:
            selectorImage = 17;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;
        case 9:
            selectorImage = 22;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;

        case 10:
        {
            selectorImage = 14;
            cv::Mat tempImg =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            cvtColor(tempImg, frame, cv::COLOR_GRAY2RGB);
            break;
        }

        case 11:
            // getImgRemapGrayPreProc_StereoPair() вывод чего-то
            selectorImage = 70;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;


        case 12:
            selectorImage = 1000;
            frame =  getImg02(obj_ptr, settings_ptr, idx, selectorImage);
            break;



        default:
            cout<<"Error, bad input, quitting\n";
            break;
        }


        // Сохранение кадра с экрана
        obj_ptr->setShowImg( frame );





        // текущий индеск вывода результатов (вывод на экран или в файл)
        settings_ptr->currentIndex_selectorImage = selectorImage;


        // добавлено как альтернатива  qApp->processEvents(); можно убрать синнонимы
        QApplication::processEvents();


        QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
        // pixmap.setPos(-9, -37);



        //        QImage imgQI = ADS::Mat2QImage(img);
        //        QPixmap imgQP = ADS::QImage2QPixmap(imgQI);

        //        pixItem = QtGui.QGraphicsPixmapItem(imgQP);


        //ui->label_3->setPixmap(QPixmap::fromImage(qimg.rgbSwapped()).scaled(ui->label_3->width(),ui->label_3->height(),Qt::KeepAspectRatio));


        ui->graphicsView01->fitInView(&pixmap, Qt::KeepAspectRatio);
        ui->graphicsView01->scene()->addItem(&pixmap);

        // qApp->processEvents();






        // =====================================
        //  Вывод данных в таблицу(xyz) кординат выбранного кластера
        // =====================================


        // Копирование параметров из изменяемого в потоках объекта в неизменяемую копию для вывода резултата
        copy_vuxyzrgb(obj_ptr, vuxyzrgb_copy);


        // получение номера выделенного оператором кластера
        int SelectedClaster =  get_SelectedClaster(vuxyzrgb_copy);

        // получение количества обноруженных кластеров
        int num_cl = get_num_cl(vuxyzrgb_copy);

        //получение количества точек
        int num_p = get_num_p(vuxyzrgb_copy);

        // Количесво примитивов, должно совпадать с num_cl, введено так как иногда не совпадаес из-за рассинхронизации
        int num_prim = get_num_prim(vuxyzrgb_copy);


        // =====================================
        // Вывод в таблицу габаритов оъекта
        // =====================================

        ui->tableWidget_xyzDetObj->setItem(0,1, new QTableWidgetItem(QString::number(obj_ptr->OjectDetection_x_DS)));
        ui->tableWidget_xyzDetObj->setItem(0,2, new QTableWidgetItem(QString::number(obj_ptr->OjectDetection_y_DS)));
        ui->tableWidget_xyzDetObj->setItem(0,3, new QTableWidgetItem(QString::number(obj_ptr->OjectDetection_z_DS)));


        ADS::t_vuxyzrgb vuxyzrgb_temp = obj_ptr->get_vuxyzrgbColorObj();

        ui->tableWidget_DetectObjSize->setItem(0,0, new QTableWidgetItem(QString::number(vuxyzrgb_temp.ObjSize_circumscribedLength)));
        ui->tableWidget_DetectObjSize->setItem(0,1, new QTableWidgetItem(QString::number(vuxyzrgb_temp.ObjSize_circumscribedWidth)));
        ui->tableWidget_DetectObjSize->setItem(0,2, new QTableWidgetItem(QString::number(vuxyzrgb_temp.ObjSize_circumscribedHeight)));
        ui->tableWidget_DetectObjSize->setItem(0,3, new QTableWidgetItem(QString::number(vuxyzrgb_temp.ObjSize_ObjLength)));
        ui->tableWidget_DetectObjSize->setItem(0,4, new QTableWidgetItem(QString::number(vuxyzrgb_temp.ObjSize_ObjWidth)));
        ui->tableWidget_DetectObjSize->setItem(0,5, new QTableWidgetItem(QString::number(vuxyzrgb_temp.num_p)));




        //Фунция для передачи данных о 3D координатах найденых кластеров
        // numClaster - номер кластера
        // idxTypeData - тип данных:
        //0 - x_min
        // 1 - y_min
        // 2 - z_min

        // 3 - x_max
        // 4 - y_max
        // 5 - z_max

        // 6 - x_center
        // 7 - y_center
        // 8 - z_center

        // 9 - radius3D



        // =====================================
        // ДЕКАРТОВЫ КООРДИНАТЫ  AirCraft
        // =====================================
        // Старые но рабочие функции
        // Получение координат центра кластера и радиуса
        // SelectedClaster -номер выделенного кластера, но можно вставить любой др. номер кластера
        int x_center = get_x_center(vuxyzrgb_copy, SelectedClaster);
        int y_center = get_y_center(vuxyzrgb_copy, SelectedClaster);
        int  z_center = get_z_center(vuxyzrgb_copy, SelectedClaster);
        // int radius = get_radius(vuxyzrgb_copy, SelectedClaster);




        ui->tableWidget_xyz->setItem(0,0, new QTableWidgetItem(QString::number(SelectedClaster)));
        ui->tableWidget_xyz->setItem(0,1, new QTableWidgetItem(QString::number(x_center)));
        ui->tableWidget_xyz->setItem(0,2, new QTableWidgetItem(QString::number(y_center)));
        ui->tableWidget_xyz->setItem(0,3, new QTableWidgetItem(QString::number(z_center)));





        // =====================================
        // AirCraft_xyz
        // =====================================
        vector<double> ClasterCoordinate3D =  obj_ptr->getClasterCoordinate3D(0);

        double x_AirCraft = ClasterCoordinate3D.at(6); // Координата самолёта по x
        double y_AirCraft = ClasterCoordinate3D.at(7); // Координата самолёта по y
        double z_AirCraft = ClasterCoordinate3D.at(8); // Координата самолёта по z



        if ( z_AirCraft < -99770 )
        {
            // обновление предыдущей координаты
            z_AirCraft = z_AirCraft_prev;
            y_AirCraft = y_AirCraft_prev;
            x_AirCraft = x_AirCraft_prev;

        }



        // =====================================
        // расчет глиссады для tableWidget_AirCraft_xyz
        // =====================================
        ax = ui->doubleSpinBox_AirCraftParamEquation_ax->value();
        ay = ui->doubleSpinBox_AirCraftParamEquation_ay->value();
        az = ui->doubleSpinBox_AirCraftParamEquation_az->value();
        bx = ui->doubleSpinBox_AirCraftParamEquation_bx->value();
        by = ui->doubleSpinBox_AirCraftParamEquation_by->value();
        bz = ui->doubleSpinBox_AirCraftParamEquation_bz->value();

        double t = az * z_AirCraft + bz;  // параметр t

        double x_GlidePath = ax*t + bx; // Координата глисcады по x
        double y_GlidePath = ay*t + by; // Координата глиcсады по y
        double z_GlidePath = az*t + bz; // Координата глисcады по z

        double dx_AirCraft = x_AirCraft - x_GlidePath; // Отклонение самолёто от глисcады по x
        double dy_AirCraft = y_AirCraft - y_GlidePath; // Отклонение самолёто от глисcады по y
        double dz_AirCraft = z_AirCraft - z_GlidePath; // Отклонение самолёто от глисcады по z


        // Определение следующего самолёта

        if ( (z_AirCraft - z_AirCraft_prev) > 2000 )
        {
            counter_AirCraft++;
        }

        // обновление предыдущей координаты
        z_AirCraft_prev = z_AirCraft;
        y_AirCraft_prev = y_AirCraft;
        x_AirCraft_prev = x_AirCraft;

        // =====================================
        // Ввод данных в таблицу tableWidget_AirCraft_xyz
        // =====================================
        ui->tableWidget_AirCraft_xyz->setItem(0,0, new QTableWidgetItem(QString::number(counter_AirCraft)));
        ui->tableWidget_AirCraft_xyz->setItem(0,1, new QTableWidgetItem(QString::number( x_AirCraft )));
        ui->tableWidget_AirCraft_xyz->setItem(0,2, new QTableWidgetItem(QString::number( y_AirCraft )));
        ui->tableWidget_AirCraft_xyz->setItem(0,3, new QTableWidgetItem(QString::number( z_AirCraft )));

        ui->tableWidget_AirCraft_xyz->setItem(1,0, new QTableWidgetItem(QString::number(counter_AirCraft)));
        ui->tableWidget_AirCraft_xyz->setItem(1,1, new QTableWidgetItem(QString::number( x_GlidePath )));
        ui->tableWidget_AirCraft_xyz->setItem(1,2, new QTableWidgetItem(QString::number( y_GlidePath )));
        ui->tableWidget_AirCraft_xyz->setItem(1,3, new QTableWidgetItem(QString::number( z_GlidePath )));

        ui->tableWidget_AirCraft_xyz->setItem(2,0, new QTableWidgetItem(QString::number(counter_AirCraft)));
        ui->tableWidget_AirCraft_xyz->setItem(2,1, new QTableWidgetItem(QString::number( dx_AirCraft )));
        ui->tableWidget_AirCraft_xyz->setItem(2,2, new QTableWidgetItem(QString::number( dy_AirCraft )));
        ui->tableWidget_AirCraft_xyz->setItem(2,3, new QTableWidgetItem(QString::number( dz_AirCraft )));



        // =====================================



        // =====================================
        // СФЕРИЧЕСКИЕ КООРДИНАТЫ AirCraft
        // =====================================



        // =====================================
        // расчет координат самолёта для tableWidget_AirCraft_rtf
        // =====================================
        // =====================================
        // AirCraft_rtf
        // =====================================


        double ro_AirCraft = sqrt(pow(x_AirCraft,2) + pow(y_AirCraft,2) + pow(z_AirCraft,2));        // радиус AirCraft
        double theta_AirCraft = atan( sqrt(pow(x_AirCraft,2) + pow(y_AirCraft,2) ) / z_AirCraft );   // полярный угол AirCraft
        double phi_AirCraft = atan2l(y_AirCraft, x_AirCraft);                                       // азимут  AirCraft


        // =====================================
        // расчет глиссады для tableWidget_AirCraft_rtf
        // =====================================

        double ro_GlidePath = sqrt( pow(x_GlidePath,2) + pow(y_GlidePath,2) + pow(z_GlidePath,2) );           // радиус глисcады
        double theta_GlidePath = atan( sqrt(pow(x_GlidePath,2) + pow(y_GlidePath,2) ) / z_GlidePath );        // полярный угол глисcады
        double phi_GlidePath = atan2l( y_GlidePath, x_GlidePath );                                            // азимут глисcады




        // =====================================
        // отклонение глиссады для tableWidget_AirCraft_rtf
        // =====================================
        double dro_AirCraft = ro_AirCraft - ro_GlidePath;           // Отклонение самолёто от глисcады по радиусу
        double dtheta_AirCraft = theta_AirCraft - theta_GlidePath;  // Отклонение самолёто от глисcады по полярному углу
        double dphi_AirCraft = phi_AirCraft - phi_GlidePath;        // Отклонение самолёто от глисcады по азимуту

        // =====================================
        // Ввод данных в таблицу tableWidget_AirCraft_rtf
        // =====================================
        ui->tableWidget_AirCraft_rtf->setItem(0,0, new QTableWidgetItem(QString::number(counter_AirCraft)));
        ui->tableWidget_AirCraft_rtf->setItem(0,1, new QTableWidgetItem(QString::number( ro_AirCraft )));
        ui->tableWidget_AirCraft_rtf->setItem(0,2, new QTableWidgetItem(QString::number( theta_AirCraft )));
        ui->tableWidget_AirCraft_rtf->setItem(0,3, new QTableWidgetItem(QString::number( phi_AirCraft )));

        ui->tableWidget_AirCraft_rtf->setItem(1,0, new QTableWidgetItem(QString::number(counter_AirCraft)));
        ui->tableWidget_AirCraft_rtf->setItem(1,1, new QTableWidgetItem(QString::number( ro_GlidePath )));
        ui->tableWidget_AirCraft_rtf->setItem(1,2, new QTableWidgetItem(QString::number( theta_GlidePath )));
        ui->tableWidget_AirCraft_rtf->setItem(1,3, new QTableWidgetItem(QString::number( phi_GlidePath )));

        ui->tableWidget_AirCraft_rtf->setItem(2,0, new QTableWidgetItem(QString::number(counter_AirCraft)));
        ui->tableWidget_AirCraft_rtf->setItem(2,1, new QTableWidgetItem(QString::number( dro_AirCraft )));
        ui->tableWidget_AirCraft_rtf->setItem(2,2, new QTableWidgetItem(QString::number( dtheta_AirCraft)));
        ui->tableWidget_AirCraft_rtf->setItem(2,3, new QTableWidgetItem(QString::number( dphi_AirCraft )));





        // =====================================
        // AirCraft для LOG , сигнал для предачи данных для записи в лог файл
        // =====================================
        emit signal_MainFormToLOG_AirCraftData(
            counter_AirCraft,
            x_AirCraft,
            y_AirCraft,
            z_AirCraft,
            x_GlidePath,
            y_GlidePath,
            z_GlidePath,
            dx_AirCraft,
            dy_AirCraft,
            dz_AirCraft,
            ro_AirCraft,
            theta_AirCraft,
            phi_AirCraft,
            ro_GlidePath,
            theta_GlidePath,
            phi_GlidePath,
            dro_AirCraft,
            dtheta_AirCraft,
            dphi_AirCraft);





        // =====================================
        // Вывод в таблицу координат 2-х выбранных точек и рассояние между ними
        // =====================================
        if (!vuxyzrgb_copy->distance2Point3D.empty())
        {
                ui->tableWidget_TwoPoint->setItem(0,0, new QTableWidgetItem(QString::number(vuxyzrgb_copy->distance2Point3D.at(0))));
                ui->tableWidget_TwoPoint->setItem(0,1, new QTableWidgetItem(QString::number(vuxyzrgb_copy->distance2Point3D.at(1))));
                ui->tableWidget_TwoPoint->setItem(0,2, new QTableWidgetItem(QString::number(vuxyzrgb_copy->distance2Point3D.at(2))));
                ui->tableWidget_TwoPoint->setItem(1,0, new QTableWidgetItem(QString::number(vuxyzrgb_copy->distance2Point3D.at(3))));
                ui->tableWidget_TwoPoint->setItem(1,1, new QTableWidgetItem(QString::number(vuxyzrgb_copy->distance2Point3D.at(4))));
                ui->tableWidget_TwoPoint->setItem(1,2, new QTableWidgetItem(QString::number(vuxyzrgb_copy->distance2Point3D.at(5))));
                ui->tableWidget_TwoPoint->setItem(1,3, new QTableWidgetItem(QString::number(vuxyzrgb_copy->distance2Point3D.at(6))));
        }




        // =====================================
        // Вывод времени работы потока
        // =====================================


        ui->label_timeThreadRuntime_00->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(0), 0)));
        ui->label_timeThreadRuntime_01->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(1), 1)));
        ui->label_timeThreadRuntime_02->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(2), 2)));
        ui->label_timeThreadRuntime_03->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(3), 3)));
        ui->label_timeThreadRuntime_04->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(4), 4 )));
        ui->label_timeThreadRuntime_05->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(5), 5)));
        ui->label_timeThreadRuntime_06->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(6), 6)));
        ui->label_timeThreadRuntime_07->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(7), 7)));
        ui->label_timeThreadRuntime_08->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(8), 8)));
        ui->label_timeThreadRuntime_09->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(9), 9)));
        ui->label_timeThreadRuntime_10->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(10), 10)));
        ui->label_timeThreadRuntime_11->setText( QString::number( (int) ADS::expRunningAverage(settings_ptr->timeMsThreadRuntime.at(11), 11)));

        // =====================================
        // DarkNet
        // =====================================
        ui->label_DarkNet_Reply->setText(QString::number(settings_ptr->DarkNet.reply));

//        std::vector <std::string> textDarkNet(3," ");
//        textDarkNet.at(0) = " 111, \n";
//        textDarkNet.at(1) = " 222, \n";
//        textDarkNet.at(2) = " 333, \n";



         std::string textDarkNet = settings_ptr->DarkNet.replyAll;
         ui->textEdit_DarkNet_Reply2->setText( QString::fromStdString(textDarkNet) );


        // перенести в слоты чтоб изменялось не постоянно а только при изменении
        //settings_ptr->smbNumDisparities = ui->spinBox_smbNumDisparities->value();
//        settings_ptr->smbBlockSize = ui->spinBox_smbBlockSize->value();
//        settings_ptr->smbPreFilterCap = ui->spinBox_smbPreFilterCap->value();

//        settings_ptr->smbMinDisparity = ui->spinBox_smbMinDisparity->value();
//        settings_ptr->smbTextureThreshold = ui->spinBox_smbTextureThreshold->value();
//        settings_ptr->smbUniquenessRatio = ui->spinBox_smbUniquenessRatio->value();
//        settings_ptr->smbSpeckleWindowSize = ui->spinBox_smbSpeckleWindowSize->value();
//        settings_ptr->smbSpeckleRange = ui->spinBox_smbSpeckleRange->value();
//        settings_ptr->smbDisp12MaxDiff = ui->spinBox_smbDisp12MaxDiff->value();


//        settings_ptr->rad = ui->spinBox_dbsRad->value();
//        settings_ptr->neighbors = ui->spinBox_dbsNeighbors->value();
//        settings_ptr->filter_ClusterPoints = ui->spinBox_filter_ClusterPoints->value();

//        settings_ptr->coef_base_dbs = ui->doubleSpinBox_coef_base_dbs->value();
//        settings_ptr->coef_pow_dbs = ui->doubleSpinBox_coef_pow_dbs->value();





//        // =====================================
//        // ?? непонятно для чего тут поставил Скорее всего чтоб алгоритмы менялись в режиме онлайн
//        // =====================================
//            // считывание из lineEdit
//            vector<string> xst(settings_ptr->maxNumThread);
//            xst.at(0) = ui->lineEdit_Alg_inThr00->text().toStdString();
//            xst.at(1) = ui->lineEdit_Alg_inThr01->text().toStdString();
//            xst.at(2) = ui->lineEdit_Alg_inThr02->text().toStdString();
//            xst.at(3) = ui->lineEdit_Alg_inThr03->text().toStdString();
//            xst.at(4) = ui->lineEdit_Alg_inThr04->text().toStdString();
//            xst.at(5) = ui->lineEdit_Alg_inThr05->text().toStdString();
//            xst.at(6) = ui->lineEdit_Alg_inThr06->text().toStdString();
//            xst.at(7) = ui->lineEdit_Alg_inThr07->text().toStdString();
//            xst.at(8) = ui->lineEdit_Alg_inThr08->text().toStdString();
//            xst.at(9) = ui->lineEdit_Alg_inThr09->text().toStdString();
//            xst.at(10) = ui->lineEdit_Alg_inThr10->text().toStdString();
//            xst.at(11) = ui->lineEdit_Alg_inThr11->text().toStdString();


//            for (int q = 0; q < int(settings_ptr->Algorithm_inThread_seq.size()); q++ )
//            {


//               // преобрахование string в vector<int>
//               vector<int> data = string_to_vecint(xst.at(q));
//                settings_ptr->Algorithm_inThread_seq.at(q) = data;

//                // Авто рестарт потока
//                if (!settings_ptr->exitCode.at(q) &&
//                    settings_ptr->idxThreadAutoRestart.at(q))
//                    Start_certainThread(obj_ptr, settings_ptr, q);

//            }


//      // =====================================
//      //  способ старый 01 Алгоритм для рестарта потока
//      // =====================================


//        for (int q = 0; q < int(settings_ptr->Algorithm_inThread_seq.size()); q++ )
//        {


//            // Авто рестарт потока
//            if (!settings_ptr->exitCode.at(q) &&
//                settings_ptr->idxThreadAutoRestart.at(q))
//                Start_certainThread(obj_ptr, settings_ptr, q);


//        }







        //                // =====================================
        //                // способ новый 02 Авто рестарт потока
        //                // =====================================

        //                for (int q = 0; q < int(settings_ptr->Algorithm_inThread_seq.size()); q++ )
        //                {


        //                    // Авто рестарт потока
        //                    if (!settings_ptr->exitCode.at(q) &&
        //                            settings_ptr->idxThreadAutoRestart.at(q))
        //                    {

        //                        Start_certainThread(obj_ptr, settings_ptr, q);
        //                        settings_ptr->idxThreadAutoRestart.at(q) = false;


        //                        // Запуск потоков, которые были запущены до нажатия кнопки стоп

        //                        if ( q == 0 ) {  ui->checkBox_00->setChecked(true); exitCode_global.at(0)= true;}
        //                        if ( q == 1 ) {  ui->checkBox_01->setChecked(true); exitCode_global.at(1)= true;}
        //                        if ( q == 2 ) {  ui->checkBox_02->setChecked(true); exitCode_global.at(2)= true;}
        //                        if ( q == 3 ) {  ui->checkBox_03->setChecked(true); exitCode_global.at(3)= true;}
        //                        if ( q == 4 ) {  ui->checkBox_04->setChecked(true); exitCode_global.at(4)= true;}
        //                        if ( q == 5 ) {  ui->checkBox_05->setChecked(true); exitCode_global.at(5)= true;}
        //                        if ( q == 6 ) {  ui->checkBox_06->setChecked(true); exitCode_global.at(6)= true;}
        //                        if ( q == 7 ) {  ui->checkBox_07->setChecked(true); exitCode_global.at(7)= true;}
        //                        if ( q == 8 ) {  ui->checkBox_08->setChecked(true); exitCode_global.at(8)= true;}
        //                        if ( q == 9 ) {  ui->checkBox_09->setChecked(true); exitCode_global.at(9)= true;}
        //                        if ( q == 10 ) { ui->checkBox_10->setChecked(true); exitCode_global.at(10)= true;}
        //                        if ( q == 11 ) { ui->checkBox_11->setChecked(true); exitCode_global.at(11)= true;}
        //                        settings_ptr->exitCode = exitCode_global;

        //                    }

        //                }











        // =====================================
        // Код для отключение Basler камеры    ----- работае когда пропадает соединение с камерами
        // =====================================
        if (ui->comboBoxMain_SelectCam->currentIndex() == 0)
        {
            // проверка на подключение если камеры удалены
            if( !obj_ptr->checkConection() )
            {
                ui->checkBox_01->setChecked(0);
                exitCode_global.at(1)= false;
                settings_ptr->exitCode = exitCode_global;


                if (!settings_ptr->LossCamconnection)
                {
                    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    emit signal_sendText_plainTextEdit_StateInfo( " Basler Cam Loss Connection "); // отправка текста в информационное окно
                    emit signal_sendText_plainTextEdit_StateInfo( " Basler Cam Loss Connection "); // отправка текста в информационное окно
                    // >>>>>>>>>>>>>>>>>>>>
                }
                // занесение статуса о потере соединения с камерами
                settings_ptr->LossCamconnection = true;


#if defined(_WIN64)

                // ################################
                Sleep(delay);
                // ################################

#else

                // ################################
                usleep(delay*1000);
                // ################################

#endif

            }
        }


        // =====================================
        // Код для отключение HikRobot камеры
        // =====================================
        if (ui->comboBoxMain_SelectCam->currentIndex() == 4 || ui->comboBoxMain_SelectCam->currentIndex() == 5 )
        {
            // проверка на подключение если камеры удалены
            if( !obj_ptr->HRCam_IsDeviceConnected(settings_ptr->HikVision.HR_CamIndex[0]) ||
                !obj_ptr->HRCam_IsDeviceConnected(settings_ptr->HikVision.HR_CamIndex[1]) )
            {
                ui->checkBox_01->setChecked(0);
                ui->checkBox_09->setChecked(0);
                exitCode_global.at(1)= false;
                exitCode_global.at(9)= false;

                settings_ptr->exitCode = exitCode_global;


                if (!settings_ptr->LossCamconnection)
                {
                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                emit signal_sendText_plainTextEdit_StateInfo( " HikRobot Cam Loss Connection "); // отправка текста в информационное окно
                emit signal_sendText_plainTextEdit_StateInfo( " HikRobot Cam Loss Connection "); // отправка текста в информационное окно
                // >>>>>>>>>>>>>>>>>>>>
                }

                // занесение статуса о потере соединения с камераи
                settings_ptr->LossCamconnection = true;


#if defined(_WIN64)

                // ################################
                Sleep(delay);
                // ################################

#else

                // ################################
                usleep(delay*1000);
                // ################################

#endif

            }
        }



        // =====================================
        // =====================================
        // =====================================


// Неудачная попытка снять галочки с отключенного потока

//        if ( !exitCode_global.at(11)  ) ui->checkBox_11->setChecked(0);
//        if ( !exitCode_global.at(10)  ) ui->checkBox_10->setChecked(0);
//        if ( !exitCode_global.at(9)  ) ui->checkBox_09->setChecked(0);
//        if ( !exitCode_global.at(8)  ) ui->checkBox_08->setChecked(0);
//        if ( !exitCode_global.at(7)  ) ui->checkBox_07->setChecked(0);
//        if ( !exitCode_global.at(6)  ) ui->checkBox_06->setChecked(0);
//        if ( !exitCode_global.at(5)  ) ui->checkBox_05->setChecked(0);
//        if ( !exitCode_global.at(4)  ) ui->checkBox_04->setChecked(0);
//        if ( !exitCode_global.at(3)  ) ui->checkBox_03->setChecked(0);
//        if ( !exitCode_global.at(2)  ) ui->checkBox_02->setChecked(0);
//        if ( !exitCode_global.at(1)  ) ui->checkBox_01->setChecked(0);



    } while (exitCode_global[0]);

}




void MainWindow::mousePressEvent(QMouseEvent *event)
{

    // Detect if the click is in the view.
    // обработка клика мыши


    QPoint remapped = ui->graphicsView01->mapFromParent( event->pos() );
    QPointF mousePoint;
    if ( ui->graphicsView01->rect().contains( remapped ) )
    {
        mousePoint = ui->graphicsView01->mapToScene( remapped.x()-9, remapped.y()-37 );

        //        QTableWidgetItem *twi = new QTableWidgetItem;
        //        twi->setText(QString::number(mousePoint.x()));
        //        ui->tableWidget_TwoPoint->setItem(0,1, twi);


        if ( ui->comboBoxMain_ImgType->currentIndex() == 2 )
        {
            setMouseClickCoordinates(obj_ptr, mousePoint.x() * 2, mousePoint.y() * 2 );
        }
        else
        {
            setMouseClickCoordinates(obj_ptr, mousePoint.x(), mousePoint.y() );
        }



    }



    // Код для расчета размера выводимого на экран изображения
    //    int v = ui->graphicsView01->height();
    //    int u = ui->graphicsView01->width();

    //    if(v*1.6 < u)
    //    {
    //        u = v*1.6 - 8;
    //    }
    //    else
    //    {
    //        v = u/1.6 - 8;
    //    }







}

void MainWindow::on_checkBox_05_released()
{
    if ( ui->checkBox_05->isChecked() )
    {

        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(5)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

            //Чтение номеров алгоритмов из LineEdit
            st = ui->lineEdit_Alg_inThr05->text().toStdString();
            data = string_to_vecint(st);

            //cout << "\t\t\t  ######## Algorithm_inThread_seq.at(5)[0]: " << settings_ptr->Algorithm_inThread_seq.at(5)[0] << endl;
            settings_ptr->Algorithm_inThread_seq.at(5) = data;
            // Запуск потока
            Start_certainThread(obj_ptr, settings_ptr, 5);

    }
    else
    {
        exitCode_global.at(5)= false;
        settings_ptr->exitCode = exitCode_global;
    }
}

void MainWindow::on_checkBox_04_released()
{
    if ( ui->checkBox_04->isChecked() )
    {

        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(4)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

            //Чтение номеров алгоритмов из LineEdit
            st = ui->lineEdit_Alg_inThr04->text().toStdString();
            data = string_to_vecint(st);
            settings_ptr->Algorithm_inThread_seq.at(4) = data;
            // Запуск потока
            Start_certainThread(obj_ptr, settings_ptr, 4);

    }
    else
    {
        exitCode_global.at(4)= false;
        settings_ptr->exitCode = exitCode_global;
    }

}


void MainWindow::on_checkBox_03_released()
{
    if ( ui->checkBox_03->isChecked() )
    {
        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(3)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;


            //Чтение номеров алгоритмов из LineEdit
            st = ui->lineEdit_Alg_inThr03->text().toStdString();
            data = string_to_vecint(st);
            settings_ptr->Algorithm_inThread_seq.at(3) = data;
            // Запуск потока
            Start_certainThread(obj_ptr, settings_ptr, 3);


    }
    else
    {
        exitCode_global.at(3)= false;
        settings_ptr->exitCode = exitCode_global;
    }
}


void MainWindow::on_checkBox_02_released()
{
    if ( ui->checkBox_02->isChecked() )
    {
        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(2)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

            //Чтение номеров алгоритмов из LineEdit
            st = ui->lineEdit_Alg_inThr02->text().toStdString();
            data = string_to_vecint(st);
            settings_ptr->Algorithm_inThread_seq.at(2) = data;
            // Запуск потока
            Start_certainThread(obj_ptr, settings_ptr, 2);


    }
    else
    {
        exitCode_global.at(2)= false;
        settings_ptr->exitCode = exitCode_global;
    }
}

void MainWindow::on_checkBox_01_released()
{
    if ( ui->checkBox_01->isChecked() )
    {
        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");


        // запуск потоков
        exitCode_global.at(1)= true;
        settings_ptr->exitCode = exitCode_global;


        string st;
        vector<int> data;

            //Чтение номеров алгоритмов из LineEdit
            st = ui->lineEdit_Alg_inThr01->text().toStdString();
            data = string_to_vecint(st);
            settings_ptr->Algorithm_inThread_seq.at(1) = data;
            // Запуск потока
            Start_certainThread(obj_ptr, settings_ptr, 1);

    }
    else
    {
        exitCode_global.at(1)= false;
        settings_ptr->exitCode = exitCode_global;


#if defined(_WIN64)

        // ################################
        Sleep(delay);
        // ################################

#else

        // ################################
        usleep(delay*1000);
        // ################################

#endif





        // FuncCam_Stop();

    }

}


void MainWindow::on_checkBox_00_released()
{
    if ( ui->checkBox_00->isChecked() )
    {

        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // остановка потоков
        exitCode_global.at(0)= true;
        settings_ptr->exitCode = exitCode_global;

        showImg_Cycle();



    }
    else
    {
        exitCode_global.at(0)= false;
        settings_ptr->exitCode = exitCode_global;
    }
}





// Кнопка для открытия файла
void MainWindow::on_pushButton_OpenFile1L_released()
{
    QString fileName01;




    switch (ui->comboBoxMain_SelectCam->currentIndex())
    {
    case 0:

        // ======================
        // Камеры Basler
        // ======================

        settings_ptr->SerialNumber_StereoCam00left = ui->lineEdit_OpenFile1L->text().toInt();


        break;

    case 1:
        // ======================
        // IP камеры
        // ======================
        settings_ptr->videoStreamAddressLeft = ui->lineEdit_OpenFile1L->text().toStdString();

     break;

    case 2:
        // ======================
        // Видео файл
        // ======================

         fileName01 = QFileDialog::getOpenFileName(this,
                                                          QString::fromUtf8("Открыть файл"),
                                                          QDir::currentPath(),
                                                          "All files (*.*);;Video (*.mkv *.avi *.mp4)");
        // Выход если пустой или ничего не выбранно
        if (fileName01.isEmpty()) return;

         settings_ptr->video1Path = fileName01.toUtf8().constData();
         ui->lineEdit_OpenFile1L->setText(QString::fromUtf8(settings_ptr->video1Path.c_str()));



        break;

    case 3:
        // ======================
        // Изображение
        // ======================

         fileName01 = QFileDialog::getOpenFileName(this,
                                                          QString::fromUtf8("Открыть файл"),
                                                          QDir::currentPath(),
                                                          "All files (*.*);;Images (*.png *.bmp *.jpg)");
        // Выход если пустой или ничего не выбранно
        if (fileName01.isEmpty()) return;


        settings_ptr->img1Path = fileName01.toUtf8().constData();
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8(settings_ptr->img1Path.c_str()));


        break;

    case 4:
        // ======================
        // Камеры HikRobot
        // ======================
        settings_ptr->CamSN[0] =  ui->lineEdit_OpenFile1L->text().toStdString();


        break;

    case 5:
        // ======================
        // Камеры HikRobot - параллено левая и правая камера (новый)
        // ======================
        settings_ptr->CamSN[0] =  ui->lineEdit_OpenFile1L->text().toStdString();



        break;
    case 6:
        // ======================
        // IP камеры - параллено левая и правая камера (новый)
        // ======================
        settings_ptr->videoStreamAddressLeft =  ui->lineEdit_OpenFile1L->text().toStdString();


        break;

    case 7:
        // ======================
        // 7 Стерео камера HikRobot (by IP address)
        // ======================

        break;




    } //switch



}

void MainWindow::on_pushButton_OpenFile2R_released()
{

    QString fileName02;




    switch (ui->comboBoxMain_SelectCam->currentIndex())
    {
    case 0:

        // ======================
        // Камеры Basler
        // ======================

        settings_ptr->SerialNumber_StereoCam01right = ui->lineEdit_OpenFile2R->text().toInt();


        break;

    case 1:
        // ======================
        // IP камеры
        // ======================
        settings_ptr->videoStreamAddressRight = ui->lineEdit_OpenFile2R->text().toStdString();

        break;

    case 2:
        // ======================
        // Видео файл
        // ======================

        fileName02 = QFileDialog::getOpenFileName(this,
                                                  QString::fromUtf8("Открыть файл"),
                                                  QDir::currentPath(),
                                                  "All files (*.*);;Video (*.mkv *.avi *.mp4)");

        // Выход если пустой или ничего не выбранно
        if (fileName02.isEmpty()) return;



        settings_ptr->video2Path = fileName02.toUtf8().constData();
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8(settings_ptr->video2Path.c_str()));




        break;

    case 3:
        // ======================
        // Изображение
        // ======================

        fileName02 = QFileDialog::getOpenFileName(this,
                                                  QString::fromUtf8("Открыть файл"),
                                                  QDir::currentPath(),
                                                  "All files (*.*);;Images (*.png *.bmp *.jpg)");

        // Выход если пустой или ничего не выбранно
        if (fileName02.isEmpty()) return;

        settings_ptr->img2Path = fileName02.toUtf8().constData();
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8(settings_ptr->img2Path.c_str()));


        break;

    case 4:
        // ======================
        // Камеры HikRobot
        // ======================
        settings_ptr->CamSN[1] =  ui->lineEdit_OpenFile2R->text().toStdString();


        break;

    case 5:
        // ======================
        // Камеры HikRobot - параллено левая и правая камера (новый)
        // ======================
        settings_ptr->CamSN[1] =  ui->lineEdit_OpenFile2R->text().toStdString();



        break;
    case 6:
        // ======================
        // IP камеры - параллено левая и правая камера (новый)
        // ======================
        settings_ptr->videoStreamAddressRight =  ui->lineEdit_OpenFile2R->text().toStdString();


        break;

    case 7:
        // ======================
        // 7 Стерео камера HikRobot (by IP address)
        // ======================

        break;




    } //switch










}

void MainWindow::on_pushButton_OpenCalibr_released()
{

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QString::fromUtf8("Открыть файл"),
                                                    QDir::currentPath(),
                                                    "Calibration (*.xml);;All files (*.*)");
     // Выход если пустой или ничего не выбранно
    if (fileName.isEmpty()) return;



    settings_ptr->CurrentCalib_filename = fileName.toUtf8().constData();
    obj_ptr->setCalibration(fileName.toUtf8().constData() );
    ui->lineEdit_OpenCalibr->setText(QString::fromUtf8(settings_ptr->CurrentCalib_filename.c_str()));


}



void MainWindow::on_pushButton_WriteExp_released()
{


    // =====================================
    // Запись данных для эксперимента
    // =====================================

    // =====================================
    //  Вывод изображений на экран
    // =====================================
    // idx - отображение окна:
    // 0 - откл.
    // 1 - вкл.
    // Альтернатива просто закоментировать


    // selectorImage
    // Получение изображения
    // первый входной аргумент функции - это номер камеры:

    // 0 - StereoCam00left     - левая камеры стереокамеры
    // 1 - StereoCam01right     - правая камеры стереокамеры
    // 2 - HandCam02left      - камера на левой руке манипулятора
    // 3 - HandCam03right     - камера на правой руке манипулятора
    // 9 - Стереопара         - левая + правая камеры стереокамеры

    // 10 - Изображение с результатами - левая камера стереокамеры: imgResult01r;
    // 11 - Изображение карты глубин в цвете: imgResultColorSearch
    // 12 - getDisparityMapImage02 - только карта диспаратности
    // 13 - getDisparityMapImage03  - нанесение всех точек на результирующие изображение

    // 14 - getImgRemapGrayPreProc_StereoPair() вывод чего-то
    // 15 - getImgRemap_StereoPair() вывод левого изображения стреопары перед поиском 3D точек
    // 16 - getImgRemap_StereoPair() вывод правого изображения стреопары перед поиском 3D точек
    // 17 - getImgRemap_DoubleStereoImg() вывод  стреопары перед поиском 3D точек




    // 20 - ОБЩЕЕ изображение с результатами построения примитивов объектов: imgPrimitive01r_general;
    // 21 - изображение отдельной проекции примитивов: imgPrimitivesXY;
    // 22 - изображение отдельной проекции примитивов: imgPrimitivesYZ;
    // 23 - изображение отдельной проекции примитивов: imgPrimitivesXZ;

    // 30 - Изображение результатов поиска по цвету (старое)

    // 40 - Изображение вывод контуров

    // 50 - визуализация 3D точек


    int idx = 1; // пояснение выше
    cv::Mat frame_write;
    // =====================================
    // Создание папки
    // =====================================
    string path_Video =   "./files/WriteExp/";



#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    int result01 = _mkdir(path_Video.c_str());
    if (result01==0 )
        cout << "WriteExp: Folder WriteOneStereoPair is made \n";
    else
        cout << "WriteExp: Don't made Folder WriteOneStereoPair \n";

    // ================
    // Only Windows END
    // ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(path_Video.c_str(), 0777) == -1  )
        cout << "WriteExp: Folder WriteOneStereoPair is made \n";
    else
        cout << "WriteExp: Don't made Folder WriteOneStereoPair \n";

    // ================
    // Only Linux END
    // ================

#endif


    stringstream filename_IM_left, filename_IM_right,
        filename_IM_Result,
        filename_IM_Disparity,  filename_InfoFile,
        filename_pointsClaster, filename_pointsCloud;

    filename_IM_left  << path_Video << "imgLeft_" << filenameCount << ".bmp";
    filename_IM_right  << path_Video<< "imgRight_" << filenameCount << ".bmp";
    filename_IM_Result  << path_Video<< "imgLeftResult_" << filenameCount << ".bmp";
    filename_IM_Disparity  << path_Video << "imgLeftDisp_" << filenameCount << ".bmp";

    filename_InfoFile  << path_Video << "InfoFile_" << filenameCount << ".txt";
    filename_pointsClaster  << path_Video << "pointsClaster_" << filenameCount << ".txt";
    filename_pointsCloud  << path_Video << "pointsCloud_" << filenameCount << ".txt";

    cout << "WriteExp: Begin  \n" ;
    cout << filename_IM_left.str()  << "\n" ;
    cout << filename_IM_Result.str()  << "\n";
    cout << filename_IM_Disparity.str()  << "\n";




    //    // =====================================
    //    // Сохранение изображения с результатом
    //    // =====================================


    frame_write =  getImg02(obj_ptr, settings_ptr, idx, 0);
    if (!frame_write.empty()) cv::imwrite(filename_IM_left.str(), frame_write);


    frame_write =  getImg02(obj_ptr, settings_ptr, idx, 1);
    if (!frame_write.empty()) cv::imwrite(filename_IM_right.str(), frame_write);


    frame_write =  getImg02(obj_ptr, settings_ptr, idx, 10);
    if (!frame_write.empty()) cv::imwrite(filename_IM_Result.str(), frame_write);


    frame_write =  getImg02(obj_ptr, settings_ptr, idx, 40);
    if (!frame_write.empty()) cv::imwrite(filename_IM_left.str(), frame_write);

    frame_write =  getImg02(obj_ptr, settings_ptr, idx, 11);
    if (!frame_write.empty()) cv::imwrite(filename_IM_Disparity.str(), frame_write);

    cout << "WriteExp: 001  \n" ;



    ADS::t_vuxyzrgb vuxyzrgb_temp;

    switch (ui->comboBoxMain_ModeWork->currentIndex())
    {

    case 1:
    {

        // =====================================
        //  Aurora данных о 3D точках после сопоставления с контуром
        // =====================================
         vuxyzrgb_temp = * vuxyzrgb_copy;


        break;
    }

    case 2:
    {

        // =====================================
        //  Aurora данных о 3D точках после сопоставления с контуром
        // =====================================
         vuxyzrgb_temp = obj_ptr->get_vuxyzrgbColorObj();


        break;
    }

    } // switch


    // =====================================
    //  сохранение данных о 3D точках после кластеризации в текстовый файл
    // =====================================

    std::ofstream out3;          // поток для записи
    out3.open(filename_pointsClaster.str(), ios_base::out); // окрываем файл для записи

    if (out3.is_open())

        for (int q = 0; q <   vuxyzrgb_temp.num_p; q++)
        {

            out3 << vuxyzrgb_temp.vu.at(q)[0] << " \t "
                 << vuxyzrgb_temp.vu.at(q)[1] << " \t "
                 << vuxyzrgb_temp.xyz.at(q)[0] << " \t "
                 << vuxyzrgb_temp.xyz.at(q)[1] << " \t "
                 << vuxyzrgb_temp.xyz.at(q)[2] << " \t "
                 << vuxyzrgb_temp.rgb.at(q)[0] << " \t "
                 << vuxyzrgb_temp.rgb.at(q)[1] << " \t "
                 << vuxyzrgb_temp.rgb.at(q)[2] << " \t "
                 << vuxyzrgb_temp.cluster.at(q)<< endl;

        }


    out3.close();


    cout << "WriteExp: 002  \n" ;

    // =====================================
    //  сохранение данных о всех 3D точках в текстовый файл
    // =====================================

    Mat xyz_AllPoints =  vuxyzrgb_temp.xyz_Disparity_AllPoints;

    vector<double> limit_WorkingArea = settings_ptr->limit_WorkingArea;
    //vector<double> limit_WorkingArea {-4.0e3, -4.0e3, 250, 4.0e3, 4.0e3, 5.20e3};

    vector<vector<int>> vu;            // 2D координаты точки на изображении
    vector<vector<double>> xyz;        // 3D координаты точки на пространсве


    for(int v = 0; v < xyz_AllPoints.rows; v++)
    {
        for(int u = 0; u < xyz_AllPoints.cols; u++)
        {

            cv::Vec3f xyz3D = xyz_AllPoints.at<cv::Vec3f>(v, u);


            if( xyz3D[0] < limit_WorkingArea[0] ) continue;
            if( xyz3D[1] < limit_WorkingArea[1] ) continue;
            if( xyz3D[2] < limit_WorkingArea[2] ) continue;

            if( xyz3D[0] > limit_WorkingArea[3] ) continue;
            if( xyz3D[1] > limit_WorkingArea[4] ) continue;
            if( xyz3D[2] > limit_WorkingArea[5] ) continue;


            vu.push_back({v, u});
            xyz.push_back(vector<double> ({xyz3D[0], xyz3D[1], xyz3D[2]}));


        }
    }





    std::ofstream out2;          // поток для записи
    out2.open(filename_pointsCloud.str(), ios_base::out); // окрываем файл для записи

    if (out2.is_open())

        for (int q = 0; q <  int(xyz.size()); q++)
        {

            out2 << vu.at(q)[0] << " \t " << vu.at(q)[1] << " \t "
                 << xyz.at(q)[0] << " \t " << xyz.at(q)[1] << " \t " << xyz.at(q)[2] << " \t "
                 << endl;




        }
    out2.close();




    // =====================================
    // Сохранение характеристических данных
    // =====================================
    std::ofstream out4;          // поток для записи
    out4.open(filename_InfoFile.str(), ios_base::out); // окрываем файл для записи

    if (out4.is_open())
    {
        out4
                << "\n Количество точек : \n" <<  xyz.size() << endl
                << "\n Количество точек для кластеризации: \n" <<  vuxyzrgb_temp.num_p << endl
            << "\n Количество кластеров: \n" << vuxyzrgb_temp.num_cl << endl
            << endl;


        if (ui->comboBoxMain_ModeWork->currentIndex() == 2)
        {
            out4 << "ObjSize_circumscribedLength = " << vuxyzrgb_temp.ObjSize_circumscribedLength << endl
                 << "ObjSize_circumscribedWidth = " << vuxyzrgb_temp.ObjSize_circumscribedWidth << endl
                 << "ObjSize_circumscribedHeight = " << vuxyzrgb_temp.ObjSize_circumscribedHeight << endl
                 << "ObjSize_ObjLength = " << vuxyzrgb_temp.ObjSize_ObjLength << endl
                 << "ObjSize_ObjWidth = " << vuxyzrgb_temp.ObjSize_ObjWidth << endl
                 << "num_p = " << vuxyzrgb_temp.num_p << endl
                 << endl;

        }



    }
    out4.close();










    //        std::ofstream out2;          // поток для записи
    //        out2.open(filename_pointsCloud.str(), ios_base::out); // окрываем файл для записи

    //        if (out2.is_open())

    //            for (int v = 0; v < xyz_AllPoints.rows; v++)
    //            {
    //                for(int u = 0; u < xyz_AllPoints.cols ; u++)
    //                {
    //                    out2 << xyz_AllPoints.at<cv::Vec3f>(Point(v, u)) << " \t " ;
    //                }

    //                out2 << endl;
    //            }
    //        out2.close();



    //        cv::FileStorage fs(filename_pointsCloud.str(), cv::FileStorage::WRITE);
    //        if( fs.isOpened() )
    //        {

    //            fs << "xyz_AllPoints" << xyz_AllPoints;

    //            fs.release();
    //        }
    //        else cout << "Error: can not save parameters\n";







    filenameCount ++;

}






void MainWindow::on_checkBox_WriteSourceVideo_released()
{

    if ( ui->checkBox_WriteSourceVideo->isChecked() )
    {
        // Сообщение о влючении записи

        if (!settings_ptr->MissActions.MissWarnings) QMessageBox::warning(this, "Write Source Video", "Включена Запись видео !!");

        // контроль потоков
        exitCode_global.at(18)= true;
        settings_ptr->exitCode = exitCode_global;
        WriteSourceVideo_thread(obj_ptr, settings_ptr);


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " Start Write Source Video  "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

    }
    else
    {

        exitCode_global.at(18)= false;
        settings_ptr->exitCode = exitCode_global;

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " Stop Write Source Video  "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

    }



}


// ===========================
// on_pushButton_WriteOneStereoPair_released
// =========================

void MainWindow::on_pushButton_WriteOneStereoPair_released()
{

    WriteOneStereoPair(obj_ptr, settings_ptr);

}


// ===========================
//  on_pushButton_Start_released
// =========================
void MainWindow::on_pushButton_Start_released()
{

    // меняет флаг значения кнопки на противоположенный
    flag_StartButton = !flag_StartButton;

    settings_ptr->TCP.flag_StartProgram = flag_StartButton; // флаг указывает работает ли программа (или не работает)

    if (flag_StartButton)
    {

       ui->pushButton_Start->setText("Стоп"); // меняем название кнопки


       // =====================================
       // Запуск до  treadFunc_Stating();
       // =====================================
        switch (ui->comboBoxMain_ModeWork->currentIndex())
        {

        case 0:
        {


            break;
        }
        case 2:
        {
            // =============================
            //        Режим "Аврора"
            // =============================

            // =====================================
            // Запуск приёма и передачи данных по TCP
            // =====================================

            emit signal_StartAuroraLantern(); // сигнал на Включение фонаря


            break;
        }
        } // switch




        // <><><><><><><><><><><><><><><><>
        // Звпуск указанных потоков
        // <><><><><><><><><><><><><><><><>
        treadFunc_Stating();
        // <><><><><><><><><><><><><><><><>

        // =====================================
        // Запуск после treadFunc_Stating();
        // =====================================
        switch (ui->comboBoxMain_ModeWork->currentIndex())
        {

        case 0:
        {


            break;
        }
        case 2:
        {

            // =============================
            //        Режим "Аврора"
            // =============================

            // =====================================
            // Запуск приёма и передачи данных по TCP
            // =====================================

            form01_mainSettings01->on_checkBox_Send_InitialStateInformation_clicked(); // сообщения о готовности к работе для пакета от СТЗ в БПУ
            emit signal_StartAllTCP();      // сигнал на остановку отправки по TCP


            break;
        }
        } // switch




        // =====================================
        //  Вывод изображений на экран
        // =====================================

        if ( ui->checkBox_00->isChecked() )  // провека на отметку на запуск
        {
            showImg_Cycle();
        }




//        // =============================
//        //        Режим "Аврора"
//        // =============================

//        switch (ui->comboBoxMain_ModeWork->currentIndex())
//        {

//        case 2:
//        {
//            // =====================================
//            //  Аврора TCP
//            // =====================================
//            // Автоматическая отправка TCP сообщения о готовности к работе
//            form01_mainSettings01->on_checkBox_Send_InitialStateInformation_clicked();
//            // Запуск TCP таймера для отправки переодических сообщений
//            form01_mainSettings01->slot_Initiate_timerTCP();
//            break;
//        }

//        } // switch


    }
    else
    {

        ui->pushButton_Start->setText("Запуск"); // меняем название кнопки

        try {

            //        // остановка потоков
            //        fill(exitCode_global.begin(), exitCode_global.end(), false);
            //        settings_ptr->exitCode = exitCode_global;


            // Запоминание задействованных потоков во время нажатия кнопки стоп

            if ( ui->checkBox_WriteSourceVideo->isChecked() )  exitCode_global_Remember.at(18) = true; else exitCode_global_Remember.at(18) = false;
            if ( ui->checkBox_WriteResultVideo->isChecked() )  exitCode_global_Remember.at(17) = true; else exitCode_global_Remember.at(17) = false;

            if ( ui->checkBox_11->isChecked() )  exitCode_global_Remember.at(11) = true; else exitCode_global_Remember.at(11) = false;
            if ( ui->checkBox_10->isChecked() )  exitCode_global_Remember.at(10) = true; else exitCode_global_Remember.at(10) = false;
            if ( ui->checkBox_09->isChecked() )  exitCode_global_Remember.at(9) = true; else exitCode_global_Remember.at(9) = false;
            if ( ui->checkBox_08->isChecked() )  exitCode_global_Remember.at(8) = true; else exitCode_global_Remember.at(8) = false;
            if ( ui->checkBox_07->isChecked() )  exitCode_global_Remember.at(7) = true; else exitCode_global_Remember.at(7) = false;
            if ( ui->checkBox_06->isChecked() )  exitCode_global_Remember.at(6) = true; else exitCode_global_Remember.at(6) = false;
            if ( ui->checkBox_05->isChecked() )  exitCode_global_Remember.at(5) = true; else exitCode_global_Remember.at(5) = false;
            if ( ui->checkBox_04->isChecked() )  exitCode_global_Remember.at(4) = true; else exitCode_global_Remember.at(4) = false;
            if ( ui->checkBox_03->isChecked() )  exitCode_global_Remember.at(3) = true; else exitCode_global_Remember.at(3) = false;
            if ( ui->checkBox_02->isChecked() )  exitCode_global_Remember.at(2) = true; else exitCode_global_Remember.at(2) = false;
            if ( ui->checkBox_01->isChecked() )  exitCode_global_Remember.at(1) = true; else exitCode_global_Remember.at(1) = false;
            if ( ui->checkBox_00->isChecked() )  exitCode_global_Remember.at(0) = true; else exitCode_global_Remember.at(0) = false;


            // остановка потоков
            fill(exitCode_global.begin(), exitCode_global.end(), false);
            settings_ptr->exitCode = exitCode_global;


            // ?? Зачемто 2-а раза
            if ( ui->checkBox_WriteSourceVideo->isChecked() ) ui->checkBox_WriteSourceVideo->setChecked(false);
            if ( ui->checkBox_WriteResultVideo->isChecked() ) ui->checkBox_WriteResultVideo->setChecked(false);

            if ( ui->checkBox_11->isChecked() ) ui->checkBox_11->setChecked(false);
            if ( ui->checkBox_10->isChecked() ) ui->checkBox_10->setChecked(false);
            if ( ui->checkBox_09->isChecked() ) ui->checkBox_09->setChecked(false);
            if ( ui->checkBox_08->isChecked() ) ui->checkBox_08->setChecked(false);
            if ( ui->checkBox_07->isChecked() ) ui->checkBox_07->setChecked(false);
            if ( ui->checkBox_06->isChecked() ) ui->checkBox_06->setChecked(false);
            if ( ui->checkBox_05->isChecked() ) ui->checkBox_05->setChecked(false);
            if ( ui->checkBox_04->isChecked() ) ui->checkBox_04->setChecked(false);
            if ( ui->checkBox_03->isChecked() ) ui->checkBox_03->setChecked(false);
            if ( ui->checkBox_02->isChecked() ) ui->checkBox_02->setChecked(false);
            if ( ui->checkBox_01->isChecked() ) ui->checkBox_01->setChecked(false);
            if ( ui->checkBox_00->isChecked() ) ui->checkBox_00->setChecked(false);





            switch (ui->comboBoxMain_ModeWork->currentIndex())
            {

            case 0:
            {


                break;
            }
            case 2:
            {
                //        // =============================
                //        //        Режим "Аврора"
                //        // =============================
                // =====================================
                // Остановка приёма и передачи данных по TCP
                // =====================================
                //            connect(this, &MainWindow::signal_StopAllTCP,
                //                    form01_mainSettings01, &form01_mainSettings::slot_StopAllTCP);
                emit signal_StopAllTCP();      // сигнал на остановку отправки по TCP
                emit signal_StopAuroraLantern(); //  сигнал на Выключение фонаря


                break;
            }
            } // switch











#if defined(_WIN64)

            // ################################
            Sleep(delay);
            // ################################

#else

            // ################################
            usleep(delay*1000);
            // ################################

#endif




        }  catch (...)
        {

        }

    }









}


// =====================================
//  Функция преобразования строки в вектор (сделано для ввода алгоритмов в потоки)
// =====================================
vector<int> MainWindow::string_to_vecint(string xst)
{

    std::vector<int> data;

    // преобрахование string в vector<int>

        std::stringstream ss;
        double number;

        ss << xst;
        while (ss >> number) data.push_back(number);

     return data;
}



// =====================================
// Функция перевода вектора int в стороку
// =====================================
std::string MainWindow::vecint_to_string(vector<int> v)
{

    std::stringstream ss;
    for(size_t i = 0; i < v.size(); ++i)
    {
        if(i != 0) ss << " ";
        ss << v[i];
    }
    std::string s = ss.str();

    return s;

}

void MainWindow::slot_writeText_plainTextEdit_StateInfo(QString text)
{

    QTextCharFormat ff;

    ff.setFontPointSize(10);

    ff.setFontItalic(false);
    ff.setFontUnderline(false);
    ff.setFontFixedPitch(true);
    ff.setFontStrikeOut(false);
    ui->plainTextEdit_StateInfo->setCurrentCharFormat(ff);



    ui->plainTextEdit_StateInfo->appendPlainText(QTime::currentTime().toString("hh:mm:ss") + " ::  " + text);



    ui->statusbar->showMessage(QTime::currentTime().toString("hh:mm:ss") + " ::  " + text);


    //   ui->plainTextEdit_Status->setPlainText(ui->plainTextEdit_Status->toPlainText() +
    //                                            QTime::currentTime().toString("hh:mm:ss") + " ::  " +
    //                                             text);




}

void MainWindow::slot_ApplyStartCommand()
{


     ui->pushButton_Start->animateClick();

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    emit signal_sendText_plainTextEdit_StateInfo( " Применён автозапуск программы "); // отправка текста в информационное окно
    // >>>>>>>>>>>>>>>>>>>>>>



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



// ===========================
// on_pushButton_Stop_released
// =========================
void MainWindow::on_pushButton_Stop_released()
{


//    try {

//        //        // остановка потоков
//        //        fill(exitCode_global.begin(), exitCode_global.end(), false);
//        //        settings_ptr->exitCode = exitCode_global;





//        // остановка потоков
//        fill(exitCode_global.begin(), exitCode_global.end(), false);
//        settings_ptr->exitCode = exitCode_global;
//        if ( ui->checkBox_WriteSourceVideo->isChecked() ) ui->checkBox_WriteSourceVideo->setChecked(false);
//        if ( ui->checkBox_WriteResultVideo->isChecked() ) ui->checkBox_WriteResultVideo->setChecked(false);
//        if ( ui->checkBox_11->isChecked() ) ui->checkBox_11->setChecked(false);
//        if ( ui->checkBox_10->isChecked() ) ui->checkBox_10->setChecked(false);
//        if ( ui->checkBox_09->isChecked() ) ui->checkBox_09->setChecked(false);
//        if ( ui->checkBox_08->isChecked() ) ui->checkBox_08->setChecked(false);
//        if ( ui->checkBox_07->isChecked() ) ui->checkBox_07->setChecked(false);
//        if ( ui->checkBox_06->isChecked() ) ui->checkBox_06->setChecked(false);
//        if ( ui->checkBox_05->isChecked() ) ui->checkBox_05->setChecked(false);
//        if ( ui->checkBox_04->isChecked() ) ui->checkBox_04->setChecked(false);
//        if ( ui->checkBox_03->isChecked() ) ui->checkBox_03->setChecked(false);
//        if ( ui->checkBox_02->isChecked() ) ui->checkBox_02->setChecked(false);
//        if ( ui->checkBox_01->isChecked() ) ui->checkBox_01->setChecked(false);
//        if ( ui->checkBox_00->isChecked() ) ui->checkBox_00->setChecked(false);


//#if defined(_WIN64)

//        // ################################
//        Sleep(delay);
//        // ################################

//#else

//        // ################################
//        usleep(delay*1000);
//        // ################################

//#endif




//    }  catch (...)
//    {

//    }



}



// ===========================
//
// =========================
void MainWindow::on_comboBoxMain_ModeWork_currentIndexChanged(int index)
{


    //        Режим РТК
    //        Режим Калибровки
    //        Режим "Аврора"
    //        Режим "Тест TCP"




    int w, h;
    QPixmap *pix = new QPixmap; // для логотипа на форме
    string s00;


    switch (ui->comboBoxMain_ModeWork->currentIndex())
    {

    case 0:
    {
        // =============================
        //        Режим РТК
        // =============================

        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/RTK2.jpg" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_WorkMode->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================




        ui->lineEdit_Alg_inThr00->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set01_RTK_Algorithm_inThread_seq.at(0) )   ));

        // lineEdit_Alg_inThr01 Так как уже определено в comboBoxMain_SelectCam()
        //ui->lineEdit_Alg_inThr01->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set01_RTK_Algorithm_inThread_seq.at(1) )   ));

        ui->lineEdit_Alg_inThr02->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set01_RTK_Algorithm_inThread_seq.at(2) )   ));
        ui->lineEdit_Alg_inThr03->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set01_RTK_Algorithm_inThread_seq.at(3) )   ));
        ui->lineEdit_Alg_inThr04->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set01_RTK_Algorithm_inThread_seq.at(4) )   ));
        ui->lineEdit_Alg_inThr05->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set01_RTK_Algorithm_inThread_seq.at(5) )   ));
        ui->lineEdit_Alg_inThr06->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set01_RTK_Algorithm_inThread_seq.at(6) )   ));

        ui->lineEdit_Alg_inThr07->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set01_RTK_Algorithm_inThread_seq.at(7) )   ));



        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================
        ui->label_NoteThread00->setText(QString::fromUtf8( "UI экран" ));
        ui->label_NoteThread01->setText(QString::fromUtf8( "Камеры" ));
        ui->label_NoteThread02->setText(QString::fromUtf8( "Предобработка" ));
        ui->label_NoteThread03->setText(QString::fromUtf8( "3D точки" ));
        ui->label_NoteThread04->setText(QString::fromUtf8( "Кластеризация" ));
        ui->label_NoteThread05->setText(QString::fromUtf8( "Разметка" ));
        ui->label_NoteThread06->setText(QString::fromUtf8( "Примитивы" ));
        ui->label_NoteThread07->setText(QString::fromUtf8( "--" ));


        // ======================
        // Снятеие галоче с доп. потоков
        // ======================
        ui->checkBox_00->setChecked(true);
        ui->checkBox_01->setChecked(true);
        ui->checkBox_02->setChecked(true);
        ui->checkBox_03->setChecked(true);
        ui->checkBox_04->setChecked(true);
        ui->checkBox_05->setChecked(true);
        ui->checkBox_06->setChecked(false);
        ui->checkBox_07->setChecked(false);


        if ( ui->checkBox_00->isChecked() ) exitCode_global.at(0)= true;
        if ( ui->checkBox_01->isChecked() ) exitCode_global.at(1)= true;
        if ( ui->checkBox_02->isChecked() ) exitCode_global.at(2)= true;
        if ( ui->checkBox_03->isChecked() ) exitCode_global.at(3)= true;
        if ( ui->checkBox_04->isChecked() ) exitCode_global.at(4)= true;
        if ( ui->checkBox_05->isChecked() ) exitCode_global.at(5)= true;
        if ( ui->checkBox_06->isChecked() ) exitCode_global.at(6)= false;
        if ( ui->checkBox_07->isChecked() ) exitCode_global.at(7)= false;
        // settings_ptr->exitCode = exitCode_global;




        // =============================
        // Расстановква галочек для записи при выборе режима
        // =============================
        ui->checkBox_WriteResultVideo->setChecked(false);
        ui->checkBox_WriteSourceVideo->setChecked(false);
        exitCode_global.at(17) = false;
        exitCode_global.at(18) = false;
        settings_ptr->exitCode = exitCode_global;
        // =============================



        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "ModeWork:: Режим РТК  >> "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;

    }
    case 1:

        // =============================
        //        Режим Калибровки
        // =============================
        //        s00 = vecint_to_string( settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(0) );
        //        ui->lineEdit_Alg_inThr00->setText(QString::fromUtf8(s00));


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/logoCalib01_100.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_WorkMode->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================



        //
        ui->lineEdit_Alg_inThr00->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(0) )   ));

        // lineEdit_Alg_inThr01 Так как уже определено в comboBoxMain_SelectCam()
        // ui->lineEdit_Alg_inThr01->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(1) )   ));
        ui->lineEdit_Alg_inThr02->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(2) )   ));
        ui->lineEdit_Alg_inThr03->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(3) )   ));
        ui->lineEdit_Alg_inThr04->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(4) )   ));
        ui->lineEdit_Alg_inThr05->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(5) )   ));
        ui->lineEdit_Alg_inThr06->setText(QString::fromUtf8(  vecint_to_string( {} )   ));
        ui->lineEdit_Alg_inThr07->setText(QString::fromUtf8( vecint_to_string( {} )   ));




        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================
        ui->label_NoteThread00->setText(QString::fromUtf8( "UI экран" ));
        ui->label_NoteThread01->setText(QString::fromUtf8( "Камеры" ));
        ui->label_NoteThread02->setText(QString::fromUtf8( "Калибровка" ));
        ui->label_NoteThread03->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread04->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread05->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread06->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread07->setText(QString::fromUtf8( "-" ));


        // ======================
        // Снятеие галоче с доп. потоков
        // ======================
        ui->checkBox_00->setChecked(true);
        ui->checkBox_01->setChecked(true);
        ui->checkBox_02->setChecked(true);
        ui->checkBox_03->setChecked(false);
        ui->checkBox_04->setChecked(false);
        ui->checkBox_05->setChecked(false);
        ui->checkBox_06->setChecked(false);
        ui->checkBox_07->setChecked(false);


        if ( ui->checkBox_00->isChecked() ) exitCode_global.at(0)= true;
        if ( ui->checkBox_01->isChecked() ) exitCode_global.at(1)= true;
        if ( ui->checkBox_02->isChecked() ) exitCode_global.at(2)= true;
        if ( ui->checkBox_03->isChecked() ) exitCode_global.at(3)= false;
        if ( ui->checkBox_04->isChecked() ) exitCode_global.at(4)= false;
        if ( ui->checkBox_05->isChecked() ) exitCode_global.at(5)= false;
        if ( ui->checkBox_06->isChecked() ) exitCode_global.at(6)= false;
        if ( ui->checkBox_07->isChecked() ) exitCode_global.at(7)= false;
        // settings_ptr->exitCode = exitCode_global;


        // =============================
        // Расстановква галочек для записи при выборе режима
        // =============================
        ui->checkBox_WriteResultVideo->setChecked(false);
        ui->checkBox_WriteSourceVideo->setChecked(false);
        exitCode_global.at(17) = false;
        exitCode_global.at(18) = false;
        settings_ptr->exitCode = exitCode_global;
        // =============================

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "ModeWork:: Режим Калибровки  >> "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        break;
    case 2:

        // =============================
        //        Режим "Аврора"
        // =============================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/logoAurora02_100.jpg" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_WorkMode->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================


        //
        ui->lineEdit_Alg_inThr00->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(0) )   ));

        // lineEdit_Alg_inThr01 Так как уже определено в comboBoxMain_SelectCam()
        // ui->lineEdit_Alg_inThr01->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(1) )   ));
        ui->lineEdit_Alg_inThr02->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(2) )   ));
        ui->lineEdit_Alg_inThr03->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(3) )   ));
        ui->lineEdit_Alg_inThr04->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(4) )   ));
        ui->lineEdit_Alg_inThr05->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(5) )   ));
        ui->lineEdit_Alg_inThr06->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(6) )   ));
        ui->lineEdit_Alg_inThr07->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(7) )   ));




        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================
        ui->label_NoteThread00->setText(QString::fromUtf8( "UI экран" ));
        ui->label_NoteThread01->setText(QString::fromUtf8( "Камеры" ));
        ui->label_NoteThread02->setText(QString::fromUtf8( "Предобработка" ));
        ui->label_NoteThread03->setText(QString::fromUtf8( "3D точки" ));
        ui->label_NoteThread04->setText(QString::fromUtf8( "Поиск по цвету" ));
        ui->label_NoteThread05->setText(QString::fromUtf8( "Габариты" ));
        ui->label_NoteThread06->setText(QString::fromUtf8( "Примитивы" ));
        ui->label_NoteThread07->setText(QString::fromUtf8( "--" ));


        // ======================
        // Снятеие галоче с доп. потоков
        // ======================
        ui->checkBox_00->setChecked(true);
        ui->checkBox_01->setChecked(true);
        ui->checkBox_02->setChecked(true);
        ui->checkBox_03->setChecked(true);
        ui->checkBox_04->setChecked(true);
        ui->checkBox_05->setChecked(true);
        ui->checkBox_06->setChecked(false);
        ui->checkBox_07->setChecked(false);


        if ( ui->checkBox_00->isChecked() ) exitCode_global.at(0)= true;
        if ( ui->checkBox_01->isChecked() ) exitCode_global.at(1)= true;
        if ( ui->checkBox_02->isChecked() ) exitCode_global.at(2)= true;
        if ( ui->checkBox_03->isChecked() ) exitCode_global.at(3)= true;
        if ( ui->checkBox_04->isChecked() ) exitCode_global.at(4)= true;
        if ( ui->checkBox_05->isChecked() ) exitCode_global.at(5)= true;
        if ( ui->checkBox_06->isChecked() ) exitCode_global.at(6)= false;
        if ( ui->checkBox_07->isChecked() ) exitCode_global.at(7)= false;
        // settings_ptr->exitCode = exitCode_global;




        // =============================
        // Расстановква галочек для записи при выборе режима
        // =============================
        ui->checkBox_WriteResultVideo->setChecked(true);
        ui->checkBox_WriteSourceVideo->setChecked(true);
        exitCode_global.at(17) = true;
        exitCode_global.at(18) = true;
        settings_ptr->exitCode = exitCode_global;
        // =============================

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "ModeWork:: Режим Аврора  >> "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;

    case 3:

        // =============================
        //        Режим "Тест TCP"
        // =============================


        //    // Логотип на форму
//        pixmap->load( "./file/Logo/logoIPcam01_100.jpg" );
//        ui->label_logo_WorkMode->setPixmap( *pixmap );


        //
        ui->lineEdit_Alg_inThr00->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(0) )   ));

        // lineEdit_Alg_inThr01 Так как уже определено в comboBoxMain_SelectCam()
        // ui->lineEdit_Alg_inThr01->setText(QString::fromUtf8(  vecint_to_string( {171, -1} )   ));
        ui->lineEdit_Alg_inThr02->setText(QString::fromUtf8(  vecint_to_string( {172, -1} )   ));
        ui->lineEdit_Alg_inThr03->setText(QString::fromUtf8(  vecint_to_string( {173, -1} )   ));
        ui->lineEdit_Alg_inThr04->setText(QString::fromUtf8(  vecint_to_string( {-1} )   ));
        ui->lineEdit_Alg_inThr05->setText(QString::fromUtf8(  vecint_to_string( {-1} )   ));
        ui->lineEdit_Alg_inThr06->setText(QString::fromUtf8(  vecint_to_string( {} )   ));
        ui->lineEdit_Alg_inThr07->setText(QString::fromUtf8(  vecint_to_string( {} )   ));



        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================
        ui->label_NoteThread00->setText(QString::fromUtf8( "UI экран" ));
        ui->label_NoteThread01->setText(QString::fromUtf8( "Камера" ));
        ui->label_NoteThread02->setText(QString::fromUtf8( "TCP Клиент" ));
        ui->label_NoteThread03->setText(QString::fromUtf8( "TCP Сервер" ));
        ui->label_NoteThread04->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread05->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread06->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread07->setText(QString::fromUtf8( "--" ));


        // ======================
        // Снятеие галоче с доп. потоков
        // ======================
        ui->checkBox_00->setChecked(true);
        ui->checkBox_01->setChecked(false);
        ui->checkBox_02->setChecked(true);
        ui->checkBox_03->setChecked(true);
        ui->checkBox_04->setChecked(false);
        ui->checkBox_05->setChecked(false);
        ui->checkBox_06->setChecked(false);
        ui->checkBox_07->setChecked(false);


        if ( ui->checkBox_00->isChecked() ) exitCode_global.at(0)= true;
        if ( ui->checkBox_01->isChecked() ) exitCode_global.at(1)= true;
        if ( ui->checkBox_02->isChecked() ) exitCode_global.at(2)= true;
        if ( ui->checkBox_03->isChecked() ) exitCode_global.at(3)= false;
        if ( ui->checkBox_04->isChecked() ) exitCode_global.at(4)= false;
        if ( ui->checkBox_05->isChecked() ) exitCode_global.at(5)= false;
        if ( ui->checkBox_06->isChecked() ) exitCode_global.at(6)= false;
        if ( ui->checkBox_07->isChecked() ) exitCode_global.at(7)= false;
        // settings_ptr->exitCode = exitCode_global;

        // =============================
        // Расстановква галочек для записи при выборе режима
        // =============================
        ui->checkBox_WriteResultVideo->setChecked(false);
        ui->checkBox_WriteSourceVideo->setChecked(false);
        exitCode_global.at(17) = false;
        exitCode_global.at(18) = false;
        settings_ptr->exitCode = exitCode_global;


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "ModeWork:: Режим Тест TCP  >> "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;





    }



}




// ===========================
//
// =========================
void MainWindow::on_checkBox_06_released()
{

    if ( ui->checkBox_06->isChecked() )
    {
        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");


        // запуск потоков
        exitCode_global.at(6)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

        //Чтение номеров алгоритмов из LineEdit
        st = ui->lineEdit_Alg_inThr06->text().toStdString();
        data = string_to_vecint(st);

        //cout << "\t\t\t  ######## Algorithm_inThread_seq.at(5)[0]: " << settings_ptr->Algorithm_inThread_seq.at(5)[0] << endl;
        settings_ptr->Algorithm_inThread_seq.at(6) = data;
        // Запуск потока
        Start_certainThread(obj_ptr, settings_ptr, 6);

    }
    else
    {
        exitCode_global.at(6)= false;
        settings_ptr->exitCode = exitCode_global;
    }


}


void MainWindow::on_checkBox_07_released()
{
    if ( ui->checkBox_07->isChecked() )
    {

        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(7)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

        //Чтение номеров алгоритмов из LineEdit
        st = ui->lineEdit_Alg_inThr07->text().toStdString();
        data = string_to_vecint(st);

        //cout << "\t\t\t  ######## Algorithm_inThread_seq.at(5)[0]: " << settings_ptr->Algorithm_inThread_seq.at(5)[0] << endl;
        settings_ptr->Algorithm_inThread_seq.at(7) = data;
        // Запуск потока
        Start_certainThread(obj_ptr, settings_ptr, 7);

    }
    else
    {
        exitCode_global.at(7)= false;
        settings_ptr->exitCode = exitCode_global;
    }



}


void MainWindow::on_checkBox_08_released()
{
    if ( ui->checkBox_08->isChecked() )
    {
        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(8)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

        //Чтение номеров алгоритмов из LineEdit
        st = ui->lineEdit_Alg_inThr08->text().toStdString();
        data = string_to_vecint(st);

        //cout << "\t\t\t  ######## Algorithm_inThread_seq.at(5)[0]: " << settings_ptr->Algorithm_inThread_seq.at(5)[0] << endl;
        settings_ptr->Algorithm_inThread_seq.at(8) = data;
        // Запуск потока
        Start_certainThread(obj_ptr, settings_ptr, 8);

    }
    else
    {
        exitCode_global.at(8)= false;
        settings_ptr->exitCode = exitCode_global;
    }



}


void MainWindow::on_checkBox_09_released()
{

    if ( ui->checkBox_09->isChecked() )
    {

        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(9)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

        //Чтение номеров алгоритмов из LineEdit
        st = ui->lineEdit_Alg_inThr09->text().toStdString();
        data = string_to_vecint(st);

        //cout << "\t\t\t  ######## Algorithm_inThread_seq.at(5)[0]: " << settings_ptr->Algorithm_inThread_seq.at(5)[0] << endl;
        settings_ptr->Algorithm_inThread_seq.at(9) = data;
        // Запуск потока
        Start_certainThread(obj_ptr, settings_ptr, 9);

    }
    else
    {
        exitCode_global.at(9)= false;
        settings_ptr->exitCode = exitCode_global;
    }



}


void MainWindow::on_checkBox_10_released()
{
    if ( ui->checkBox_10->isChecked() )
    {
        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(10)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

        //Чтение номеров алгоритмов из LineEdit
        st = ui->lineEdit_Alg_inThr10->text().toStdString();
        data = string_to_vecint(st);

        //cout << "\t\t\t  ######## Algorithm_inThread_seq.at(5)[0]: " << settings_ptr->Algorithm_inThread_seq.at(5)[0] << endl;
        settings_ptr->Algorithm_inThread_seq.at(10) = data;
        // Запуск потока
        Start_certainThread(obj_ptr, settings_ptr, 10);

    }
    else
    {
        exitCode_global.at(10)= false;
        settings_ptr->exitCode = exitCode_global;
    }
}


void MainWindow::on_checkBox_11_released()
{
    if ( ui->checkBox_11->isChecked() )
    {

        // для кнопки запуска
        flag_StartButton =true;
        ui->pushButton_Start->setText("Стоп...");

        // запуск потоков
        exitCode_global.at(11)= true;
        settings_ptr->exitCode = exitCode_global;

        string st;
        vector<int> data;

        //Чтение номеров алгоритмов из LineEdit
        st = ui->lineEdit_Alg_inThr11->text().toStdString();
        data = string_to_vecint(st);

        //cout << "\t\t\t  ######## Algorithm_inThread_seq.at(5)[0]: " << settings_ptr->Algorithm_inThread_seq.at(5)[0] << endl;
        settings_ptr->Algorithm_inThread_seq.at(11) = data;
        // Запуск потока
        Start_certainThread(obj_ptr, settings_ptr, 11);

    }
    else
    {
        exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;
    }
}


void MainWindow::on_comboBoxMain_SelectCam_currentIndexChanged(int index)
{

    // ======================
    // Инициализация строк GUI с номерами алгоритмов
    // ======================
    int w, h;
    QPixmap *pix = new QPixmap; // для логотипа на форме
    string s00;

    switch (index)
    {
    case 0:
        // ======================
        // Камеры Basler
        // ======================

        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/basler_logo.jpg" );
        w = ui->label_logo_CamTypy->width();
        h = ui->label_logo_CamTypy->height();
        ui->label_logo_CamTypy->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================

        // ======================
        // Выбор алгоритмов
        // ======================

        settings_ptr->Algorithm_inThread_seq.at(1) = {111, 112, 11, 911, 912};


        settings_ptr->Algorithm_inThread_seq.at(8) = {};
        settings_ptr->Algorithm_inThread_seq.at(9) = {};
        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};


        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(false);
        ui->checkBox_09->setChecked(false);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= false;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= false;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;

        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================

        ui->label_NoteThread08->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));


        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::number( settings_ptr->SerialNumber_StereoCam00left ));
        ui->lineEdit_OpenFile2R->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: Камеры Basler "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        break;

    case 1:
        // ======================
        // IP камеры
        // ======================

        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/ip-camera.png" );
        w = ui->label_logo_CamTypy->width();
        h = ui->label_logo_CamTypy->height();
        ui->label_logo_CamTypy->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================

        // ======================
        // Выбор алгоритмов
        // ======================

        settings_ptr->Algorithm_inThread_seq.at(1) = {113, 12};

        settings_ptr->Algorithm_inThread_seq.at(8) = {};
        settings_ptr->Algorithm_inThread_seq.at(9) = {};
        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};



        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(false);
        ui->checkBox_09->setChecked(false);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= false;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= false;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;


        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================

        ui->label_NoteThread08->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));




        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->videoStreamAddressLeft.c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->videoStreamAddressRight.c_str() ));


        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply IP Cam01");
        ui->pushButton_OpenFile2R->setText("apply IP Cam02");



        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: IP камеры "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>


        break;

    case 2:
        // ======================
        // Видео файл
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/videot.png" );
        w = ui->label_logo_CamTypy->width();
        h = ui->label_logo_CamTypy->height();
        ui->label_logo_CamTypy->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================

        // ======================
        // Выбор алгоритмов
        // ======================

        settings_ptr->Algorithm_inThread_seq.at(1) = {13};


        settings_ptr->Algorithm_inThread_seq.at(8) = {};
        settings_ptr->Algorithm_inThread_seq.at(9) = {};
        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};



        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(false);
        ui->checkBox_09->setChecked(false);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= false;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= false;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;


        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================

        ui->label_NoteThread08->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));


        // Заносим путь к видеофайлу  камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8(settings_ptr->video1Path.c_str()));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8(settings_ptr->video2Path.c_str()));


        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("open Video 01");
        ui->pushButton_OpenFile2R->setText("open Video 02");

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: Видео файл "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;


    case 3:
        // ======================
        // Изображение
        // ======================

        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/image.png" );
        w = ui->label_logo_CamTypy->width();
        h = ui->label_logo_CamTypy->height();
        ui->label_logo_CamTypy->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================

        // ======================
        // Выбор алгоритмов
        // ======================

        settings_ptr->Algorithm_inThread_seq.at(1) = {14};

        settings_ptr->Algorithm_inThread_seq.at(8) = {};
        settings_ptr->Algorithm_inThread_seq.at(9) = {};
        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};



        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(false);
        ui->checkBox_09->setChecked(false);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= false;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= false;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;


        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================

        ui->label_NoteThread08->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));


        // Заносим путь к видеофайлу  камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8(settings_ptr->img1Path.c_str()));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8(settings_ptr->img2Path.c_str()));


        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("open Img 01");
        ui->pushButton_OpenFile2R->setText("open Img 02");

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: Изображение "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;


    case 4:
        // ======================
        // Камеры HikRobot
        // ======================

        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/HikVision.jpg" );
        w = ui->label_logo_CamTypy->width();
        h = ui->label_logo_CamTypy->height();
        ui->label_logo_CamTypy->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================

        // ======================
        // Выбор алгоритмов
        // ======================
        settings_ptr->Algorithm_inThread_seq.at(1) = {115, 116, 117, 17, 915, 916};

        settings_ptr->Algorithm_inThread_seq.at(8) = {};
        settings_ptr->Algorithm_inThread_seq.at(9) = {};
        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};



        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(false);
        ui->checkBox_09->setChecked(false);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= false;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= false;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;


        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================

        ui->label_NoteThread08->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));


        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->CamSN[0].c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->CamSN[1].c_str() ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: Камеры HikRobot "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;

    case 5:
        // ======================
        // Камеры HikRobot - параллено левая и правая камера (новый)
        // ======================

        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/HikVision.jpg" );
        w = ui->label_logo_CamTypy->width();
        h = ui->label_logo_CamTypy->height();
        ui->label_logo_CamTypy->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================



        settings_ptr->Algorithm_inThread_seq.at(1) = {115, 116, 117, 18, 915, 916};

        settings_ptr->Algorithm_inThread_seq.at(8) = {};
        settings_ptr->Algorithm_inThread_seq.at(9) = {19};
        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};



        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(false);
        ui->checkBox_09->setChecked(false);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= false;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= true;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;


        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================

        ui->label_NoteThread08->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "Камера 02" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));



        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->CamSN[0].c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->CamSN[1].c_str() ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");



        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: Камеры HikRobot - параллено левая и правая камера (новый) "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;

    case 6:

        // ======================
        // IP камеры - параллено левая и правая камера (новый)
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/IP.jpg" );
        w = ui->label_logo_CamTypy->width();
        h = ui->label_logo_CamTypy->height();
        ui->label_logo_CamTypy->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================

        // ======================
        // Выбор алгоритмов
        // ======================

        settings_ptr->Algorithm_inThread_seq.at(1) = {113};

        settings_ptr->Algorithm_inThread_seq.at(8) = {15};
        settings_ptr->Algorithm_inThread_seq.at(9) = {16};

        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};



        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(true);
        ui->checkBox_09->setChecked(true);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= true;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= true;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;


        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================


        ui->label_NoteThread08->setText(QString::fromUtf8( "IP Камера 01" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "IP Камера 02" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));


        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->videoStreamAddressLeft.c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->videoStreamAddressRight.c_str() ));


        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply IP Cam01");
        ui->pushButton_OpenFile2R->setText("apply IP Cam02");



        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: IP камеры - параллено левая и правая камера (новый) "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;

    case 7:

        // ======================
        // 7 Стерео камера HikRobot (by IP address)
        // ======================

        // ======================
        // Выбор алгоритмов
        // ======================

        settings_ptr->Algorithm_inThread_seq.at(1) = {};

        settings_ptr->Algorithm_inThread_seq.at(8) = {};
        settings_ptr->Algorithm_inThread_seq.at(9) = {};

        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};



        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(false);
        ui->checkBox_09->setChecked(false);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= false;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= false;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;


        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================


        ui->label_NoteThread08->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));

        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(" в разработке  " );
        ui->lineEdit_OpenFile2R->setText(" в разработке " );


        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("---");
        ui->pushButton_OpenFile2R->setText("---");


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: 7 Стерео камера HikRobot (by IP address) "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        break;

    case 8:

        // ======================
        // 8 Стерео камера Basler (by IP address)
        // ======================


        // ======================
        // Выбор алгоритмов
        // ======================

        settings_ptr->Algorithm_inThread_seq.at(1) = {};

        settings_ptr->Algorithm_inThread_seq.at(8) = {};
        settings_ptr->Algorithm_inThread_seq.at(9) = {};

        settings_ptr->Algorithm_inThread_seq.at(10) = {};
        settings_ptr->Algorithm_inThread_seq.at(11) = {};



        // ======================
        // Снятеие галоче с доп. потоков
        // ======================

        ui->checkBox_08->setChecked(false);
        ui->checkBox_09->setChecked(false);
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);

        if ( ui->checkBox_08->isChecked() ) exitCode_global.at(8)= false;
        if ( ui->checkBox_09->isChecked() ) exitCode_global.at(9)= false;
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;


        // =============================
        // Заполнение  пояснений для доп инфы
        // =============================


        ui->label_NoteThread08->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread09->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));
        ui->label_NoteThread11->setText(QString::fromUtf8( "-" ));

        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(" в разработке  " );
        ui->lineEdit_OpenFile2R->setText(" в разработке " );


        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("---");
        ui->pushButton_OpenFile2R->setText("---");


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " SelectCam:: 8 Стерео камера Basler (by IP address) "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        break;


    }

    // Запись в lineEdit

    ui->lineEdit_Alg_inThr01->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(1))   ));


//    ui->lineEdit_Alg_inThr06->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(6))   ));
//    ui->lineEdit_Alg_inThr07->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(7))   ));
    ui->lineEdit_Alg_inThr08->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(8))   ));
    ui->lineEdit_Alg_inThr09->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(9))   ));
    ui->lineEdit_Alg_inThr10->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(10))   ));
    ui->lineEdit_Alg_inThr11->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(11))   ));

}


void MainWindow::on_pushButton_Settings_released()
{
    // открываем окно
    form01_mainSettings01->show();
    form01_mainSettings01->activateWindow();
}




void MainWindow::on_pushButton_RulerStart_clicked()
{
    // открываем окно
    objp_f02_Ruler->show();
    objp_f02_Ruler->activateWindow();

    // ++++++++++++++++
    emit signal_SendStartRuler(); // отправка текста в информационное окно
    // ++++++++++++++++++

}





// ======================
// Таймер
// ======================

void MainWindow::TimerSlot_fMain()
{

    timer_Main_dsec ++;

    if (timer_Main_dsec >= 10)
    {
        timer_Main_dsec = 0;
        timer_Main_sec++;
    }
    if (timer_Main_sec >= 60)
    {
        timer_Main_sec = 0;
        timer_Main_min++;
    }

    if (timer_Main_min >= 60)
    {
        timer_Main_min = 0;
        timer_Main_hour++;
    }



    ui->label_StatusOnWindow->setText(QDate::currentDate().toString("yyyy.MM.dd") + "  " +
                                      QTime::currentTime().toString("hh:mm:ss") + " ||  " +
                                      QString::number(timer_Main_hour)+ ":" +
                                      QString::number(timer_Main_min)+ ":" +
                                      QString::number(timer_Main_sec)+ "," +
                                      QString::number(timer_Main_dsec));

}












void MainWindow::on_comboBox_CamType_currentIndexChanged(int index)
{

    int w, h;
    QPixmap *pix = new QPixmap; // для логотипа на форме
    string s00;

//    Basler :: acA1920-50gc :: 1920x1200 -> 960x600
//    Basler :: acA1920-50gc :: 1920x1200 -> 1440x900
//    Basler :: acA1920-50gc :: 1920x1200 -> 1920x1200


//    Basler :: acA1300-60gc :: 1280x1024  cam_dXY = 0 -> 960x600


cv::FileStorage fs;


    switch (index)
    {




    case 0:

        // ======================
        // (-1) >>> Произвольная камера (размер кадра не меняется)
        // ======================


        // ======================
        // (0) >>> Параметры камеры по умолчанию из Config.xml
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/png-transparent.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================

        // =============================
        // Параметры для обрезки изображений
        // =============================
        // Параметры настройки камер

        // Закоментированно так как не понятно как будет работать
//        settings_ptr->cam_CX = {0, 0};   // Центрирование изображения по горизонтали
//        settings_ptr->cam_CY = {0, 0};   // Центрирование изображения по вертикали
//        settings_ptr->cam_dX = {0, 0};         // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
//        settings_ptr->cam_dY = {0, 0};         // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
//        settings_ptr->cam_X = {0, 0};          // Ширина активной области (активной зоны) камеры в пикселях
//        settings_ptr->cam_Y = {0, 0};          // Высота активной области (активной зоны) камеры в пикселях.

        // Параметры сжатия изображения
        settings_ptr->img_X = -1; // Ширина сжатого изображения
        settings_ptr->img_Y = -1; // Высота сжатого изображения

        settings_ptr->img_coeff = -1;


        // ++++++++++++++++++++++++++++++++++++++
        // Вывод пути файла в строку на ui
        // ++++++++++++++++++++++++++++++++++++++

        // FirstCalib_filename - из config-файла файла настроек
        ui->lineEdit_OpenCalibr->setText(FirstCalib_filename);
        obj_ptr->setCalibration(FirstCalib_filename.toStdString().c_str() );

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "   (-1) >>> Произвольная камера (размер кадра не меняется) "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================
                ui->lineEdit_SNCam01->setText("--");
                ui->lineEdit_SNCam02->setText("--");
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));

        break;






    case 1:

        // ======================
        // (0) >>> Параметры камеры по умолчанию из Config.xml
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/png-transparent.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================





        fs.open(settings_ptr->Config_filename, cv::FileStorage::READ);
        if( fs.isOpened() )
        {
            // =============================
            //  Переменные управления размером изображения Basler камер
            // =============================
            // Параметры для обрезки изображений
            fs["cam_CX"] >> settings_ptr->cam_CX;  // Центрирование изображения по горизонтали
            fs["cam_CY"] >> settings_ptr->cam_CY;  // Центрирование изображения по вертикали
            fs["cam_dX"] >> settings_ptr->cam_dX;  // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
            fs["cam_dY"] >> settings_ptr->cam_dY;  // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
            fs["cam_X"] >> settings_ptr->cam_X;    // Ширина активной области (активной зоны) камеры в пикселях
            fs["cam_Y"] >> settings_ptr->cam_Y;    // Высота активной области (активной зоны) камеры в пикселях.
            // Параметры сжатия изображения
            fs["img_X"] >> settings_ptr->img_X;
            fs["img_Y"] >> settings_ptr->img_Y;
            fs["img_coeff"] >> settings_ptr->img_coeff;

            // =============================
            // Серийные номера устройств (камер) HikRobot или Basler  по 2-ому списку (для библиотек Hikrobot Machine Vision)
            // =============================
            fs["CamSN"] >> settings_ptr->CamSN;

            // =============================
            //  Серийные номера устройств (камер) Basler (для библиотек Pylon)
            // =============================
            fs["c_maxCamerasToUse"] >> settings_ptr->c_maxCamerasToUse; // Количесво задействованных камер
            fs["SerialNumber_StereoCam00left"] >> settings_ptr->SerialNumber_StereoCam00left; // Серийный номер левой камеры стереокамеры
            fs["SerialNumber_StereoCam01right"] >> settings_ptr->SerialNumber_StereoCam01right; // Серийный номер правой камеры стереокамеры
            fs["SerialNumber_HandCam02left"] >> settings_ptr->SerialNumber_HandCam02left; // Серийный номер  камеры на левой руке манипулятора
            fs["SerialNumber_HandCam03right"] >> settings_ptr->SerialNumber_HandCam03right; // Серийный номер  камеры на првавой руке манипулятора


            // =============================

        }

        fs.release();

        // ++++++++++++++++++++++++++++++++++++++
        // Вывод пути файла в строку на ui
        // ++++++++++++++++++++++++++++++++++++++
         // FirstCalib_filename - из config-файла файла настроек
        ui->lineEdit_OpenCalibr->setText( FirstCalib_filename );
        obj_ptr->setCalibration(FirstCalib_filename.toStdString().c_str() );

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "   (0) >>> Параметры камеры по умолчанию из Config.xml "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================
        ui->lineEdit_SNCam01->setText("--");
        ui->lineEdit_SNCam02->setText("--");
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));

        break;



    case 2:
        // ======================
         // (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 960x600
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/logoSTV01_100.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================




        settings_ptr->SerialNumber_StereoCam00left = 22960744;   // Серийный номер левой камеры стереокамеры
        settings_ptr->SerialNumber_StereoCam01right = 22960745; // Серийный номер правой камеры стереокамеры

        // HikVision Серийные номера устройств (камер) HikRobot по 2-ому списку
        settings_ptr->CamSN = {std::to_string(settings_ptr->SerialNumber_StereoCam00left),
                               std::to_string(settings_ptr->SerialNumber_StereoCam01right)};

        // Параметры для обрезки изображений
        settings_ptr->cam_CX = {1, 1};       // Центрирование изображения по горизонтали
        settings_ptr->cam_CY = {1, 1};       // Центрирование изображения по вертикали
        settings_ptr->cam_dX = {8, 8};       // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
        settings_ptr->cam_dY = {8, 8};       // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
        settings_ptr->cam_X = {1920, 1920};  // Ширина активной области (активной зоны) камеры в пикселях
        settings_ptr->cam_Y = {1200, 1200};  // Высота активной области (активной зоны) камеры в пикселях.

        // Параметры сжатия изображения

        settings_ptr->img_X = 960; // Ширина сжатого изображения
        settings_ptr->img_Y = 600; // Высота сжатого изображения

        settings_ptr->img_coeff = 1;

        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================
        ui->lineEdit_SNCam01->setText(QString::number(settings_ptr->SerialNumber_StereoCam00left));
        ui->lineEdit_SNCam02->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right));
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));

        // ============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::number( settings_ptr->SerialNumber_StereoCam00left ));
        ui->lineEdit_OpenFile2R->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");
        // ============================

        // Загрузка нужной калибровки
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(62a)_(960p)_StereoModule(F1.2_D2)_WATER-Err2.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(61a)_(960p)_StereoModule(F1.2_D2)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }




        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "   (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 960x600 "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        break;

    case 3:

        // ======================
        //   (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 1440x900
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/logoSTV01_100.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================




        settings_ptr->SerialNumber_StereoCam00left = 22960744;   // Серийный номер левой камеры стереокамеры
        settings_ptr->SerialNumber_StereoCam01right = 22960745; // Серийный номер правой камеры стереокамеры

        // HikVision Серийные номера устройств (камер) HikRobot по 2-ому списку
        settings_ptr->CamSN = {std::to_string(settings_ptr->SerialNumber_StereoCam00left),
                               std::to_string(settings_ptr->SerialNumber_StereoCam01right)};

        // Параметры для обрезки изображений
        settings_ptr->cam_CX = {1, 1};       // Центрирование изображения по горизонтали
        settings_ptr->cam_CY = {1, 1};       // Центрирование изображения по вертикали
        settings_ptr->cam_dX = {8, 8};       // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
        settings_ptr->cam_dY = {8, 8};       // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
        settings_ptr->cam_X = {1920, 1920};  // Ширина активной области (активной зоны) камеры в пикселях
        settings_ptr->cam_Y = {1200, 1200};  // Высота активной области (активной зоны) камеры в пикселях.

        // Параметры сжатия изображения

        settings_ptr->img_X = 1440; // Ширина сжатого изображения
        settings_ptr->img_Y = 900; // Высота сжатого изображения

        settings_ptr->img_coeff = 1;

        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================
        ui->lineEdit_SNCam01->setText(QString::number(settings_ptr->SerialNumber_StereoCam00left));
        ui->lineEdit_SNCam02->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right));
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));



        // ============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::number( settings_ptr->SerialNumber_StereoCam00left ));
        ui->lineEdit_OpenFile2R->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");
        // ============================



        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(62a)_(1440p)_StereoModule(F1.2_D2)_WATER-Err2.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(61a)_(1440p)_StereoModule(F1.2_D2)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "   (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 1440x900 "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        break;

    case 4:

        // ======================
        //    (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 1920x1200
        // ======================



        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/logoSTV01_100.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================


        settings_ptr->SerialNumber_StereoCam00left = 22960744;   // Серийный номер левой камеры стереокамеры
        settings_ptr->SerialNumber_StereoCam01right = 22960745; // Серийный номер правой камеры стереокамеры

        // HikVision Серийные номера устройств (камер) HikRobot по 2-ому списку
        settings_ptr->CamSN = {std::to_string(settings_ptr->SerialNumber_StereoCam00left),
                               std::to_string(settings_ptr->SerialNumber_StereoCam01right)};


        // Параметры для обрезки изображений
        settings_ptr->cam_CX = {1, 1};       // Центрирование изображения по горизонтали
        settings_ptr->cam_CY = {1, 1};       // Центрирование изображения по вертикали
        settings_ptr->cam_dX = {8, 8};       // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
        settings_ptr->cam_dY = {8, 8};       // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
        settings_ptr->cam_X = {1920, 1920};  // Ширина активной области (активной зоны) камеры в пикселях
        settings_ptr->cam_Y = {1200, 1200};  // Высота активной области (активной зоны) камеры в пикселях.

        // Параметры сжатия изображения

        settings_ptr->img_X = 1920; // Ширина сжатого изображения
        settings_ptr->img_Y = 1200; // Высота сжатого изображения

        settings_ptr->img_coeff = 1;

        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================
        ui->lineEdit_SNCam01->setText(QString::number(settings_ptr->SerialNumber_StereoCam00left));
        ui->lineEdit_SNCam02->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right));
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));



        // ============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::number( settings_ptr->SerialNumber_StereoCam00left ));
        ui->lineEdit_OpenFile2R->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");
        // ============================



        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(62a)_(1920p)_StereoModule(F1.2_D2)_WATER-Err2.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {
            settings_ptr->CurrentCalib_filename =  "./files/Calibration_Files/(61a)_(1920p)_StereoModule(F1.2_D2)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "   (1) >>> Basler :: acA1920-50gc :: 1920x1200 -> 1920x1200 "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;


    case 5:

        // ======================
       //  (2) >>> Basler :: acA1300-60gc :: 1280x1024  -> 960x600
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/logoSTV01_100.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================

        settings_ptr->SerialNumber_StereoCam00left = 21569689;  // Серийный номер левой камеры стереокамеры
        settings_ptr->SerialNumber_StereoCam01right = 21569694; // Серийный номер правой камеры стереокамеры

        // HikVision Серийные номера устройств (камер) HikRobot по 2-ому списку
        settings_ptr->CamSN = {std::to_string(settings_ptr->SerialNumber_StereoCam00left),
                               std::to_string(settings_ptr->SerialNumber_StereoCam01right)};

        // Параметры для обрезки изображений
        settings_ptr->cam_CX = {1, 1};       // Центрирование изображения по горизонтали
        settings_ptr->cam_CY = {1, 1};       // Центрирование изображения по вертикали
        settings_ptr->cam_dX = {0, 0};       // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
        settings_ptr->cam_dY = {0, 0};       // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
        settings_ptr->cam_X = {1280, 1280};  // Ширина активной области (активной зоны) камеры в пикселях
        settings_ptr->cam_Y = {1024, 1024};  // Высота активной области (активной зоны) камеры в пикселях.
        // Параметры сжатия изображения

        settings_ptr->img_X = 960; // Ширина сжатого изображения
        settings_ptr->img_Y = 600; // Высота сжатого изображения


        settings_ptr->img_coeff = 1;

        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================
        ui->lineEdit_SNCam01->setText(QString::number(settings_ptr->SerialNumber_StereoCam00left));
        ui->lineEdit_SNCam02->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right));
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));



        // ============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::number( settings_ptr->SerialNumber_StereoCam00left ));
        ui->lineEdit_OpenFile2R->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");
        // ============================



        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(81a)_(960x600p)_StereoModule(acA1300-60gc)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(81a)_(960x600p)_StereoModule(acA1300-60gc)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "   (2) >>> Basler :: acA1300-60gc :: 1280x1024  -> 960x600 "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;



    case 6:

        // ======================
       //  (2) >>> Basler :: acA1300-60gc :: 1280x1024  -> 1280x1024
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/logoSTV01_100.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================


        settings_ptr->SerialNumber_StereoCam00left = 21569689;  // Серийный номер левой камеры стереокамеры
        settings_ptr->SerialNumber_StereoCam01right = 21569694; // Серийный номер правой камеры стереокамеры


        // HikVision Серийные номера устройств (камер) HikRobot по 2-ому списку
        settings_ptr->CamSN = {std::to_string(settings_ptr->SerialNumber_StereoCam00left),
                               std::to_string(settings_ptr->SerialNumber_StereoCam01right)};



        // Параметры для обрезки изображений
        settings_ptr->cam_CX = {1, 1};       // Центрирование изображения по горизонтали
        settings_ptr->cam_CY = {1, 1};       // Центрирование изображения по вертикали
        settings_ptr->cam_dX = {0, 0};       // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
        settings_ptr->cam_dY = {0, 0};       // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
        settings_ptr->cam_X = {1280, 1280};  // Ширина активной области (активной зоны) камеры в пикселях
        settings_ptr->cam_Y = {1024, 1024};  // Высота активной области (активной зоны) камеры в пикселях.

        // Параметры сжатия изображения

        settings_ptr->img_X = 1280; // Ширина сжатого изображения
        settings_ptr->img_Y = 1024; // Высота сжатого изображения


        settings_ptr->img_coeff = 1;

        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================
        ui->lineEdit_SNCam01->setText(QString::number(settings_ptr->SerialNumber_StereoCam00left));
        ui->lineEdit_SNCam02->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right));
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));


        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->videoStreamAddressLeft.c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->videoStreamAddressRight.c_str() ));


         // ============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::number( settings_ptr->SerialNumber_StereoCam00left ));
        ui->lineEdit_OpenFile2R->setText(QString::number( settings_ptr->SerialNumber_StereoCam01right ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");
        // ============================




        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(82a)_(1280x1024p)_StereoModule(acA1300-60gc)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(83a)_(1280x1024p)_StereoModule(acA1300-60gc)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "   (2) >>> Basler :: acA1300-60gc :: 1280x1024  -> 1280x1024 "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;





    case 7:

        // ======================
        //  (3) >>> HikR :: MV-CA032-10GC :: 1280x1024  -> 1024x768
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/logoAurora01_100.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================



        settings_ptr->CamSN = {"02K37741088",
                               "02K37741018"}; // Серийные номера устройств (камер) HikRobot по 2-ому списку



        // Параметры для обрезки изображений
        settings_ptr->cam_CX = {1, 1};       // Центрирование изображения по горизонтали
        settings_ptr->cam_CY = {1, 1};       // Центрирование изображения по вертикали
        settings_ptr->cam_dX = {0, 0};       // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
        settings_ptr->cam_dY = {0, 0};       // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
        settings_ptr->cam_X = {960, 960};  // Ширина активной области (активной зоны) камеры в пикселях
        settings_ptr->cam_Y = {720, 720};  // Высота активной области (активной зоны) камеры в пикселях.

        // Параметры сжатия изображения

        settings_ptr->img_X = 960; // Ширина сжатого изображения
        settings_ptr->img_Y = 720; // Высота сжатого изображения

        settings_ptr->img_coeff = 1;


        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================

        ui->lineEdit_SNCam01->setText(QString::fromStdString(settings_ptr->CamSN.at(0)) );
        ui->lineEdit_SNCam02->setText(QString::fromStdString(settings_ptr->CamSN.at(1)));
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));


        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromStdString(settings_ptr->CamSN.at(0)) );
        ui->lineEdit_OpenFile2R->setText(QString::fromStdString(settings_ptr->CamSN.at(1)) );

        // =============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->CamSN[0].c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->CamSN[1].c_str() ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");
        // =============================


        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(98a)_960x720p_StereoModule_HikR_MV-CA032-10GC_WATER.xml";

            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {

            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(95d)_(960x720p)_StereoModule(HikR_MV-CA032-10GC)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "    (3) >>> HikR :: MV-CA032-10GC :: 1280x1024  -> 960x720 "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;

    case 8:

        // ======================
        // (4) >>> IP Cam :: StereoPi :: 640x480
        // ======================
        settings_ptr->img_X = 640; // Ширина сжатого изображения
        settings_ptr->img_Y = 480; // Высота сжатого изображения

        settings_ptr->img_coeff = 1;


        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(100b)_(640p)_StereoModule(BaracudaCam)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(100b)_(640p)_StereoModule(BaracudaCam)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        // =============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->videoStreamAddressLeft.c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->videoStreamAddressRight.c_str() ));


        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply IP Cam01");
        ui->pushButton_OpenFile2R->setText("apply IP Cam02");
        // =============================


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "   (4) >>> IP Cam :: StereoPi :: 640x480 "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;





    case 9:

        // ======================
        //  (5) ROV >>> HikR :: MV-CB016-10GC-C :: 1440x1080  -> 1440x1080
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/Logo_ROV.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================



        settings_ptr->CamSN = {"DA0007545",
                               "DA0007551"}; // Серийные номера устройств (камер) HikRobot по 2-ому списку



        // Параметры для обрезки изображений
        settings_ptr->cam_CX = {1, 1};       // Центрирование изображения по горизонтали
        settings_ptr->cam_CY = {1, 1};       // Центрирование изображения по вертикали
        settings_ptr->cam_dX = {0, 0};       // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
        settings_ptr->cam_dY = {0, 0};       // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
        settings_ptr->cam_X = {1440, 1440};  // Ширина активной области (активной зоны) камеры в пикселях
        settings_ptr->cam_Y = {1080, 1080};  // Высота активной области (активной зоны) камеры в пикселях.

        // Параметры сжатия изображения

        settings_ptr->img_X = 1440; // Ширина сжатого изображения
        settings_ptr->img_Y = 1080; // Высота сжатого изображения

        settings_ptr->img_coeff = 1;


        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================

        ui->lineEdit_SNCam01->setText(QString::fromStdString(settings_ptr->CamSN.at(0)) );
        ui->lineEdit_SNCam02->setText(QString::fromStdString(settings_ptr->CamSN.at(1)));
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));


        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromStdString(settings_ptr->CamSN.at(0)) );
        ui->lineEdit_OpenFile2R->setText(QString::fromStdString(settings_ptr->CamSN.at(1)) );

        // =============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->CamSN[0].c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->CamSN[1].c_str() ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");
        // =============================


        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(98a)_(1440x1080p)_ROV_StereoModule(HikR_MV-CB016-10GC-C)_AIR.xml.xml";

            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {

            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(98a)_(1440x1080p)_ROV_StereoModule(HikR_MV-CB016-10GC-C)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "    (5) ROV >>> HikR :: MV-CB016-10GC-C :: 1440x1080  -> 1440x1080  "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;


    case 10:

        // ======================
        //  (5) ROV >>> HikR :: MV-CB016-10GC-C :: 1440x1080  -> 900x675
        // ======================


        // =============================
        // Логотип на форму
        // =============================
        pix->load( ":/res_ImgLogo/files/Logo/Logo_ROV.png" );
        w = ui->label_logo_WorkMode->width();
        h = ui->label_logo_WorkMode->height();
        ui->label_logo_CamTypy_2->setPixmap( pix->scaled(w, h, Qt::KeepAspectRatio));
        // =============================



        settings_ptr->CamSN = {"DA0007545",
                               "DA0007551"}; // Серийные номера устройств (камер) HikRobot по 2-ому списку



        // Параметры для обрезки изображений
        settings_ptr->cam_CX = {1, 1};       // Центрирование изображения по горизонтали
        settings_ptr->cam_CY = {1, 1};       // Центрирование изображения по вертикали
        settings_ptr->cam_dX = {0, 0};       // Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
        settings_ptr->cam_dY = {0, 0};       // Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
        settings_ptr->cam_X = {1440, 1440};  // Ширина активной области (активной зоны) камеры в пикселях
        settings_ptr->cam_Y = {1080, 1080};  // Высота активной области (активной зоны) камеры в пикселях.

        // Параметры сжатия изображения

        settings_ptr->img_X = 900; // Ширина сжатого изображения
        settings_ptr->img_Y = 675; // Высота сжатого изображения

        settings_ptr->img_coeff = 1;


        // =====================================
        //  Вывод серийных номеров и размера кадра на форму
        // =====================================

        ui->lineEdit_SNCam01->setText(QString::fromStdString(settings_ptr->CamSN.at(0)) );
        ui->lineEdit_SNCam02->setText(QString::fromStdString(settings_ptr->CamSN.at(1)));
        ui->lineEdit_FrameSize->setText(QString::number(settings_ptr->img_X) + "x" + QString::number(settings_ptr->img_Y));


        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromStdString(settings_ptr->CamSN.at(0)) );
        ui->lineEdit_OpenFile2R->setText(QString::fromStdString(settings_ptr->CamSN.at(1)) );

        // =============================
        // Заносим адреса камер в сторку на форме
        ui->lineEdit_OpenFile1L->setText(QString::fromUtf8( settings_ptr->CamSN[0].c_str() ));
        ui->lineEdit_OpenFile2R->setText(QString::fromUtf8( settings_ptr->CamSN[1].c_str() ));

        // Меняем названии на кнопки в сотвествии её изменённым назначением
        ui->pushButton_OpenFile1L->setText("apply ID Cam01");
        ui->pushButton_OpenFile2R->setText("apply ID Cam02");
        // =============================


        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(99a)_(900x675p)_ROV_StereoModule(HikR_MV-CB016-10GC-C)_AIR.xml";

            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        else
        {

            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(99a)_(900x675p)_ROV_StereoModule(HikR_MV-CB016-10GC-C)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "    (5) ROV >>> HikR :: MV-CB016-10GC-C :: 1440x1080  -> 900x675  "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>
        break;



default:


        // ++++++++++++++++++++++++++++++++++++++
        // Вывод пути файла в строку на ui
        // ++++++++++++++++++++++++++++++++++++++
        // FirstCalib_filename - из config-файла файла настроек
        ui->lineEdit_OpenCalibr->setText( FirstCalib_filename );
        obj_ptr->setCalibration(FirstCalib_filename.toStdString().c_str() );
        break;



    } // switch end


    obj_ptr->set_BaslerСamsCrop(settings_ptr->cam_X,
                                settings_ptr->cam_Y,
                                settings_ptr->cam_CX,
                                settings_ptr->cam_CY,
                                settings_ptr->cam_dX,
                                settings_ptr->cam_dY);


    obj_ptr->set_ImgCompress(settings_ptr->img_X,
                             settings_ptr->img_Y,
                             settings_ptr->img_coeff);


    // установка серийного номера Basler камер
    string sn0 = QString::number(settings_ptr->SerialNumber_StereoCam00left).toStdString();  // Серийный номер левой камеры стереокамеры
    string sn1 = QString::number(settings_ptr->SerialNumber_StereoCam01right).toStdString(); // Серийный номер правой камеры стереокамеры
    string sn2 = QString::number(settings_ptr->SerialNumber_HandCam02left).toStdString();    // Серийный номер  камеры на левой руке манипулятора
    string sn3 = QString::number(settings_ptr->SerialNumber_HandCam03right).toStdString();   // Серийный номер  камеры на првавой руке манипулятора

    obj_ptr->set_SerialNumber_Cams({sn0.c_str(),
                                    sn1.c_str(),
                                    sn2.c_str(),
                                    sn3.c_str()});

}







void MainWindow::on_checkBox_WiriteLogAirCraft3DCoordinates_released()
{





    if (exitCode_global.at(0) && ui->checkBox_WiriteLogAirCraft3DCoordinates->isChecked())
    {


        timer_LOG_AirCraft->start();            // запуск таймера (время в милисикундах)
        ui->pushButton_WiriteLogAirCraft3DCoordinates->setText("Остановить запись координат Самолёта в LOG-файл ...");  // меняем название кнопки
        flag_WiriteLogAirCraft3DCoordinates = !flag_WiriteLogAirCraft3DCoordinates;


        // =================================


    }

    else if (!exitCode_global.at(0) )
    {
          ui->checkBox_WiriteLogAirCraft3DCoordinates->setChecked(false);
    }
    else
    {
        timer_LOG_AirCraft->stop();            // стоп таймера (время в милисикундах)
        ui->pushButton_WiriteLogAirCraft3DCoordinates->setText("Продолжить запись координат Самолёта в LOG-файл");
        flag_WiriteLogAirCraft3DCoordinates = !flag_WiriteLogAirCraft3DCoordinates;

    }




}


void MainWindow::on_pushButton_WiriteLogAirCraft3DCoordinates_pressed()
{



    // меняет флаг значения кнопки на противоположенный
    flag_WiriteLogAirCraft3DCoordinates = !flag_WiriteLogAirCraft3DCoordinates;


    if ( flag_WiriteLogAirCraft3DCoordinates)
    {


        timer_LOG_AirCraft->start();            // запуск таймера (время в милисикундах)
        ui->pushButton_WiriteLogAirCraft3DCoordinates->setText("Остановить запись координат ЛА в LOG-файл ...");  // меняем название кнопки
        ui->checkBox_WiriteLogAirCraft3DCoordinates->setChecked(true);
        // =================================


    }
    else
    {
        timer_LOG_AirCraft->stop();            // стоп таймера (время в милисикундах)
        ui->pushButton_WiriteLogAirCraft3DCoordinates->setText("Продолжить запись координат ЛА в LOG-файл");
        ui->checkBox_WiriteLogAirCraft3DCoordinates->setChecked(false);
    }

}


void MainWindow::on_spinBox_LogDataWriteSpeed_Aircraft_valueChanged(int arg1)
{

    timer_LOG_AirCraft->setInterval(arg1);            // запуск таймера (время в милисикундах)
}



void MainWindow::on_checkBox_WriteResultVideo_released()
{
    if ( ui->checkBox_WriteResultVideo->isChecked() )
    {

        // Сообщение о влючении записи

       if (!settings_ptr->MissActions.MissWarnings) QMessageBox::warning(this, "Write Result Video", "Включена Запись видео !!");


        // контроль потоков
        exitCode_global.at(17)= true;
        settings_ptr->exitCode = exitCode_global;
        WriteResultVideo_thread(obj_ptr, settings_ptr);

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " Start Write Result Vide  "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>


    }
    else
    {

        exitCode_global.at(17)= false;
        settings_ptr->exitCode = exitCode_global;

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( " Stop Write Result Vide  "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>


    }
}


// Для кнопки закрытия окна
void MainWindow::closeEvent (QCloseEvent *event)
{


    QString text = "Закрыть программу ??? ";

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Close Event", text,
                                                              QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No) return;





    if (flag_StartButton) on_pushButton_Start_released();
    event->accept();

    // остановка потоков
    fill(exitCode_global.begin(), exitCode_global.end(), false);


    settings_ptr->exitCode = exitCode_global;
    //   closeBaslerCam(obj_ptr);



    qDebug() << " exitCode_global.at(0):: " <<  settings_ptr->exitCode.at(0);
    qDebug() << " exitCode_global.at(1):: " <<  settings_ptr->exitCode .at(1);
    qDebug() << " exitCode_global.at(2):: " <<  settings_ptr->exitCode.at(2);
    qDebug() << " exitCode_global.at(3):: " <<  settings_ptr->exitCode.at(3);
    qDebug() << " exitCode_global.at(4):: " <<  settings_ptr->exitCode.at(4);
    qDebug() << " exitCode_global.at(5):: " <<  settings_ptr->exitCode.at(5);
    qDebug() << " exitCode_global.at(6):: " <<  settings_ptr->exitCode.at(6);
    qDebug() << " exitCode_global.at(7):: " <<  settings_ptr->exitCode.at(7);
    qDebug() << " exitCode_global.at(8):: " <<  settings_ptr->exitCode.at(8);
    qDebug() << " exitCode_global.at(9):: " <<  settings_ptr->exitCode.at(9);
    qDebug() << " exitCode_global.at(10):: " <<  settings_ptr->exitCode.at(10);
    qDebug() << " exitCode_global.at(11):: " <<  settings_ptr->exitCode.at(11);

    qDebug() << " exitCode_global.at(17):: " <<  settings_ptr->exitCode.at(17);
    qDebug() << " exitCode_global.at(18):: " <<  settings_ptr->exitCode.at(18);


#if defined(_WIN64)
    // ################################
    Sleep(1000);
    // ################################
#else
    // ################################
    usleep(1000*1000);
    // ################################
#endif


    MainWindow::~MainWindow();






}

void MainWindow::on_lineEdit_OpenFile1L_editingFinished()
{




    switch ( ui->comboBoxMain_SelectCam->currentIndex() )
    {

    case 1:

        // IP камеры
        // Заносим адреса камер в сторку на форме
        settings_ptr->videoStreamAddressLeft = ui->lineEdit_OpenFile1L->text().toStdString();


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( ui->lineEdit_OpenFile1L->text() ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

       break;

    case 2:

        // Видео файл
        // Заносим путь к видеофайлу  камер в сторку на форме
        settings_ptr->video1Path = ui->lineEdit_OpenFile1L->text().toStdString();

        // >>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( ui->lineEdit_OpenFile1L->text() ); // отправка текста в информационное окно
       // >>>>>>>>>>>>>>>
        break;


    case 3:

        // Изображение
        // Заносим путь к видеофайлу  камер в сторку на форме
        settings_ptr->img1Path = ui->lineEdit_OpenFile1L->text().toStdString();


        // >>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( ui->lineEdit_OpenFile1L->text() ); // отправка текста в информационное окно
       // >>>>>>>>>>>>>>>>

        break;


    case 6:

        // ======================
        // IP камеры - параллено левая и правая камера (новый)
        // ======================
        // Заносим адреса камер в сторку на форме
        settings_ptr->videoStreamAddressLeft = ui->lineEdit_OpenFile1L->text().toStdString();


        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( ui->lineEdit_OpenFile1L->text() ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>

        break;


    }



}


void MainWindow::on_lineEdit_OpenFile2R_editingFinished()
{



    switch (ui->comboBoxMain_SelectCam->currentIndex())
    {

    case 1:

        // IP камеры
        // Заносим адреса камер в сторку на форме
        settings_ptr->videoStreamAddressRight = ui->lineEdit_OpenFile2R->text().toStdString();


        // >>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( ui->lineEdit_OpenFile2R->text() ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>

        break;

    case 2:

        // Видео файл
        // Заносим путь к видеофайлу  камер в сторку на форме
        settings_ptr->video2Path = ui->lineEdit_OpenFile2R->text().toStdString();

        // >>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( ui->lineEdit_OpenFile2R->text() ); // отправка текста в информационное окно
       // >>>>>>>>>>>>>>>>>>
        break;


    case 3:

        // Изображение
        // Заносим путь к видеофайлу  камер в сторку на форме
        settings_ptr->img2Path = ui->lineEdit_OpenFile2R->text().toStdString();


        // >>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( ui->lineEdit_OpenFile2R->text() ); // отправка текста в информационное окно
       // >>>>>>>>>>>>>>>>>

        break;


    case 6:

        // ======================
        // IP камеры - параллено левая и правая камера (новый)
        // ======================
        // Заносим адреса камер в сторку на форме
        settings_ptr->videoStreamAddressRight = ui->lineEdit_OpenFile2R->text().toStdString();


        // >>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( ui->lineEdit_OpenFile2R->text() ); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>

        break;


    }



}




//// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//emit signal_sendText_plainTextEdit_StateInfo( " Basler Cam Loss Connection "); // отправка текста в информационное окно
//// >>>>>>>>>>>>>>>>>>>>


void MainWindow::on_comboBoxMain_ImgType_currentIndexChanged(int index)
{

    switch (ui->comboBoxMain_ImgType->currentIndex())
    {

    case 100:
    {

        break;
    }

    case 7:
    {


        settings_ptr->Rotation3Dpoints.angx = ui->horizontalSlider_Rx->value();
        settings_ptr->Rotation3Dpoints.angy = ui->horizontalSlider_Ry->value();
        settings_ptr->Rotation3Dpoints.angz = ui->horizontalSlider_Rz->value();
        settings_ptr->Rotation3Dpoints.tx = ui->horizontalSlider_Tx->value();
        settings_ptr->Rotation3Dpoints.ty = ui->horizontalSlider_Ty->value();
        settings_ptr->Rotation3Dpoints.tz = ui->horizontalSlider_Tz->value();
        settings_ptr->Rotation3Dpoints.Scale = ui->horizontalSlider_Scale->value();
        settings_ptr->Rotation3Dpoints.Dz = ui->horizontalSlider_Dz->value();



        // =====================================
        //  Предварительный ввод данных для вращения 3D точек
        // =====================================

        //  Номер алгоритма
        settings_ptr->Algorithm_inThread_seq.at(10) = {1000, 35};


        //  галочки в доп. потоков
        ui->checkBox_10->setChecked(true);
        if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= true;
        settings_ptr->exitCode = exitCode_global;

        // Подпись пояснений
        ui->label_NoteThread10->setText(QString::fromUtf8( "Вращение 3D точек" ));

        // Запись в lineEdit
        ui->lineEdit_Alg_inThr10->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(10))   ));



        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        emit signal_sendText_plainTextEdit_StateInfo( "    ImgType:: Вращение облака 3D точек "); // отправка текста в информационное окно
        // >>>>>>>>>>>>>>>>>>>>


        break;
    }

    default:

        {

            // =====================================
            //  Освобождение доп. потока
            // =====================================

            //  Номер алгоритма
            settings_ptr->Algorithm_inThread_seq.at(10) = {};


            //  галочки в доп. потоков
            ui->checkBox_10->setChecked(false);
            if ( ui->checkBox_10->isChecked() ) exitCode_global.at(10)= false;
            settings_ptr->exitCode = exitCode_global;


            // Подпись пояснений
            ui->label_NoteThread10->setText(QString::fromUtf8( "-" ));

            // Запись в lineEdit
            ui->lineEdit_Alg_inThr10->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(10))   ));




            // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            emit signal_sendText_plainTextEdit_StateInfo( "    ImgType:: default "); // отправка текста в информационное окно
            // >>>>>>>>>>>>>>>>>>>>


            break;

        }









    }   // switch








}


void MainWindow::on_checkBox_setWarterCalibr_clicked()
{


    switch (ui->comboBox_CamType->currentIndex())
    {

    case 2:

        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(62a)_(960p)_StereoModule(F1.2_D2)_WATER-Err2.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }
        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(61a)_(960p)_StereoModule(F1.2_D2)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        break;

    case 3:

        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(62a)_(1440p)_StereoModule(F1.2_D2)_WATER-Err2.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }
        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(61a)_(1440p)_StereoModule(F1.2_D2)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        break;


    case 4:

        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(62a)_(1920p)_StereoModule(F1.2_D2)_WATER-Err2.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }
        else
        {
            settings_ptr->CurrentCalib_filename =  "./files/Calibration_Files/(61a)_(1920p)_StereoModule(F1.2_D2)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        break;

    case 5:

        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(81a)_(960x600p)_StereoModule(acA1300-60gc)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }
        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(81a)_(960x600p)_StereoModule(acA1300-60gc)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        break;

    case 6:

        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(82a)_(1280x1024p)_StereoModule(acA1300-60gc)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }
        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(83a)_(1280x1024p)_StereoModule(acA1300-60gc)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        break;

    case 7:

        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(98a)_960x720p_StereoModule_HikR_MV-CA032-10GC_WATER.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }
        else
        {

            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(95d)_(960x720p)_StereoModule(HikR_MV-CA032-10GC)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        break;

    case 8:

        // ============================
        // Загрузка нужной калибровки
        // ============================
        if (ui->checkBox_setWarterCalibr->isChecked() )
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(100b)_(640p)_StereoModule(BaracudaCam)_AIR.xml" ;
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }
        else
        {
            settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(100b)_(640p)_StereoModule(BaracudaCam)_AIR.xml";
            obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename );
            ui->lineEdit_OpenCalibr->setText( settings_ptr->CurrentCalib_filename.c_str() );
        }

        break;

    default:


        // ++++++++++++++++++++++++++++++++++++++
        // Вывод пути файла в строку на ui
        // ++++++++++++++++++++++++++++++++++++++
        // FirstCalib_filename - из config-файла файла настроек
        ui->lineEdit_OpenCalibr->setText( FirstCalib_filename );
        obj_ptr->setCalibration(FirstCalib_filename.toStdString().c_str() );
    }


}


void MainWindow::on_pushButton_ResultVideoStream_clicked()
{

    // http://localhost:8080/pc         - экран оператора
    // http://localhost:8080/result     - изображение с метками
    // http://localhost:8080/disp       - карта диспаратности
    // http://localhost:8080/stereo     - стерео после ректификации
    // http://localhost:8080/3Dp        - 3D точки


    QString text = "\n localhost:8080/pc \t\t - экран оператора \n localhost:8080/result \t\t - изображение с метками \n localhost:8080/disp\t\t - карта диспаратности \n localhost:8080/stereo \t\t - стерео после ректификации \n localhost:8080/3Dp \t\t - 3D точки ";


    QMessageBox::StandardButton reply = QMessageBox::question(this, "Video Stream", text,
                          QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No) return;



    // меняет флаг значения кнопки на противоположенный
    G_flag_VideoStream = !G_flag_VideoStream;


    if (G_flag_VideoStream)
    {
        // ======================
        // Установка галочек доп. потоков
        // ======================
        settings_ptr->Algorithm_inThread_seq.at(11) = {178, 71, 971};


        ui->checkBox_11->setChecked(true);
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= true;
        settings_ptr->exitCode = exitCode_global;

        ui->label_NoteThread11->setText(QString::fromUtf8( "Видео Стрим" ));
        ui->lineEdit_Alg_inThr11->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(11))   ));

        Start_certainThread(obj_ptr, settings_ptr, 11);


        //        while(exitCode_global.at(7))
        //        {
        //            cv::Mat frame =  obj_ptr->getResultImage();
        //            settings_ptr->Objs_ptr.objp_VideoStreamer->set_imgStream(frame);
        //            settings_ptr->Objs_ptr.objp_VideoStreamer->Start_VideoStream();

        //        }


        ui->pushButton_ResultVideoStream->setText("Stop Stream"); // меняем название кнопки

    }
    else
    {


        // ======================
        // Установка галочек доп. потоков
        // ======================
        settings_ptr->Algorithm_inThread_seq.at(11) = {};


        ui->checkBox_11->setChecked(false);
        if ( ui->checkBox_11->isChecked() ) exitCode_global.at(11)= false;
        settings_ptr->exitCode = exitCode_global;

        ui->label_NoteThread11->setText(QString::fromUtf8( "---" ));

        ui->pushButton_ResultVideoStream->setText("Start Stream"); // меняем название кнопки
        ui->lineEdit_Alg_inThr11->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(11))   ));
    }






}


void MainWindow::on_lineEdit_SNCam01_editingFinished()
{
    // ======================
    // Изменение серийного номер камеры 01 вручную
    // ======================

    switch (ui->comboBox_CamType->currentIndex())
    {

    case 0:

        break;

    case 1:

        break;

    case 2:
        settings_ptr->SerialNumber_StereoCam00left = ui->lineEdit_SNCam01->text().toInt();
        settings_ptr->CamSN[0] = std::to_string(settings_ptr->SerialNumber_StereoCam00left);
        break;

    case 3:
         settings_ptr->SerialNumber_StereoCam00left = ui->lineEdit_SNCam01->text().toInt();
        settings_ptr->CamSN[0] = std::to_string(settings_ptr->SerialNumber_StereoCam00left);
        break;
    case 4:
         settings_ptr->SerialNumber_StereoCam00left = ui->lineEdit_SNCam01->text().toInt();
        settings_ptr->CamSN[0] = std::to_string(settings_ptr->SerialNumber_StereoCam00left);
        break;
    case 5:
         settings_ptr->SerialNumber_StereoCam00left = ui->lineEdit_SNCam01->text().toInt();
        settings_ptr->CamSN[0] = std::to_string(settings_ptr->SerialNumber_StereoCam00left);
        break;
    case 6:
         settings_ptr->SerialNumber_StereoCam00left = ui->lineEdit_SNCam01->text().toInt();
        settings_ptr->CamSN[0] = std::to_string(settings_ptr->SerialNumber_StereoCam00left);
        break;
    case 7:
         settings_ptr->CamSN[0] =  ui->lineEdit_SNCam01->text().toStdString(); // Серийные номера устройств (камер) HikRobot по 2-ому списку
        break;

    }

}


void MainWindow::on_lineEdit_SNCam02_editingFinished()
{
    // ======================
    // Изменение серийного номер камеры 02 вручную
    // ======================
    switch (ui->comboBox_CamType->currentIndex())
    {

    case 0:

        break;

    case 1:

        break;

    case 2:

        settings_ptr->SerialNumber_StereoCam01right = ui->lineEdit_SNCam02->text().toInt();
        settings_ptr->CamSN[1] = std::to_string(settings_ptr->SerialNumber_StereoCam01right);

        break;

    case 3:
        settings_ptr->SerialNumber_StereoCam01right = ui->lineEdit_SNCam02->text().toInt();
        settings_ptr->CamSN[1] = std::to_string(settings_ptr->SerialNumber_StereoCam01right);
        break;
    case 4:
        settings_ptr->SerialNumber_StereoCam01right = ui->lineEdit_SNCam02->text().toInt();
        settings_ptr->CamSN[1] = std::to_string(settings_ptr->SerialNumber_StereoCam01right);
        break;
    case 5:
        settings_ptr->SerialNumber_StereoCam01right = ui->lineEdit_SNCam02->text().toInt();
        settings_ptr->CamSN[1] = std::to_string(settings_ptr->SerialNumber_StereoCam01right);
        break;
    case 6:
        settings_ptr->SerialNumber_StereoCam01right = ui->lineEdit_SNCam02->text().toInt();
        settings_ptr->CamSN[1] = std::to_string(settings_ptr->SerialNumber_StereoCam01right);
        break;
    case 7:

        settings_ptr->CamSN[1] =  ui->lineEdit_SNCam02->text().toStdString(); // Серийные номера устройств (камер) HikRobot по 2-ому списку
        break;
    }



}




void MainWindow::on_spinBox_VideoStreamQualiy_valueChanged(int arg1)
{

    settings_ptr->VideoStreamer.StreamQualiy = arg1;

}


void MainWindow::on_pushButton_ResultVideoStream_2_clicked()
{

    if (G_flag_VideoStream)
    {

        MainWindow::on_pushButton_ResultVideoStream_clicked();
        ui->pushButton_ResultVideoStream_2->setText("Stop Stream"); // меняем название кнопки

    }
    else
    {

        MainWindow::on_pushButton_ResultVideoStream_clicked();
        ui->pushButton_ResultVideoStream_2->setText("Start Stream"); // меняем название кнопки

    }
}


void MainWindow::on_spinBox_timeMinThreadDela_00_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(0) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDelay_01_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(1) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDela_02_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(2) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDela_03_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(3) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDela_04_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(4) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDela_05_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(5) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDelay_06_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(6) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDelay_07_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(7) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDelay_08_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(8) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDelay_09_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(9) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDelay_10_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(10) = arg1;
}


void MainWindow::on_spinBox_timeMinThreadDelay_11_valueChanged(int arg1)
{
    settings_ptr->timeMsMinDelay.at(11) = arg1;
}


void MainWindow::on_pushButton_aboutQt_clicked()
{
    QMessageBox::aboutQt(this, "about Qt");
}


void MainWindow::on_pushButton_DarkNet_clicked()
{

    QString text;
    if (G_flag_DarkNet) text = "Остановить DarkNet ??? ";
    else text = "Запустить DarkNet ??? ";

    QMessageBox::StandardButton reply = QMessageBox::question(this, "DarkNet", text,
                                                              QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No) return;



    // меняет флаг значения кнопки на противоположенный
    G_flag_DarkNet = !G_flag_DarkNet;


    if (G_flag_DarkNet)
    {
        // ======================
        // Установка галочек доп. потоков
        // ======================
        settings_ptr->Algorithm_inThread_seq.at(7) = {175, 75, 975};


        ui->checkBox_07->setChecked(true);
        if ( ui->checkBox_07->isChecked() ) exitCode_global.at(7)= true;
        settings_ptr->exitCode = exitCode_global;

        ui->label_NoteThread07->setText(QString::fromUtf8( "DarkNet" ));
        ui->lineEdit_Alg_inThr07->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(7))   ));

        Start_certainThread(obj_ptr, settings_ptr, 7);


        //        while(exitCode_global.at(7))
        //        {
        //            cv::Mat frame =  obj_ptr->getResultImage();
        //            settings_ptr->Objs_ptr.objp_VideoStreamer->set_imgStream(frame);
        //            settings_ptr->Objs_ptr.objp_VideoStreamer->Start_VideoStream();

        //        }


        ui->pushButton_DarkNet->setText("Stop DarkNet"); // меняем название кнопки

    }
    else
    {


        // ======================
        // Установка галочек доп. потоков
        // ======================
        settings_ptr->Algorithm_inThread_seq.at(7) = {};


        ui->checkBox_07->setChecked(false);
        if ( ui->checkBox_07->isChecked() ) exitCode_global.at(7)= false;
        settings_ptr->exitCode = exitCode_global;

        ui->label_NoteThread07->setText(QString::fromUtf8( "---" ));

        ui->pushButton_DarkNet->setText("Start DarkNet"); // меняем название кнопки
        ui->lineEdit_Alg_inThr07->setText(QString::fromUtf8(  vecint_to_string( settings_ptr->Algorithm_inThread_seq.at(7))   ));
    }


}


void MainWindow::on_pushButton_Start3DRuler_clicked()
{
    ADS::cl_DenseStereo * obj_ptrf = obj_ptr;
   str_Settings *settings_ptrf = settings_ptr;  // указатель на структуру настроек

   ADS::cl_DenseStereo * obj_ptrf_temp = new  ADS::cl_DenseStereo; // копия на объекта класса


    obj_ptrf_temp->set_vuxyzrgb ( obj_ptrf->get_vuxyzrgb() );


    // !!!!!!!!! В cl_DenseStereo.h если clone() то тормоза, скорее всего из за double в  cv::Mat всесто uint8
    obj_ptrf_temp->vuxyzrgb.xyz_Disparity_AllPoints = obj_ptrf->vuxyzrgb.xyz_Disparity_AllPoints.clone();

    obj_ptrf_temp->setWorkingAria(settings_ptrf->limit_WorkingArea);

    vector<cv::Mat> imgs = obj_ptrf->getImgOpenCV_StereoPair();
    obj_ptrf_temp->setImgOpenCV_StereoPair( imgs[0], imgs[1] );

    obj_ptrf_temp->setImgOpenCV_1left(  obj_ptrf->getImgOpenCV_1left() );
    obj_ptrf_temp->setImgOpenCV_2right(  obj_ptrf->getImgOpenCV_2right() );
    obj_ptrf_temp->setImgColorPreProcess_StereoPair(  obj_ptrf->getImgColorPreProcess_StereoPair() );

    obj_ptrf_temp->setResultImage ( obj_ptrf->getResultImage()  );


    // Show tool window
    toolWindow->setData(obj_ptrf_temp->getImgColorPreProcess_StereoPair().at(0), obj_ptrf_temp->get_vuxyzrgb());

    // открываем окно
    toolWindow->show();
    toolWindow->activateWindow();
}

