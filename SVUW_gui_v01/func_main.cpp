
#include "func_main.h"





//

uchar* imageDataCopy = new uchar[1920*1200*3];


// Инициализация  переменной, по которой изменяется тип выводимого изображеня  активируемый через нажатие клавиши
int DrawSelector = 0;




// =====================================
// Потоки
// =====================================





// =====================================
// ОБЩАЯ Функция для всех потоков
// =====================================


// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//  (I) Алгоритмы перед циклом потоков
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


// Номера алгоритмов в потоках
// 0 - пустой

// =====================================
// (01) Алгоритмы захвата кадров (группа 01)
// =====================================
// 111 - инициализация камер Basler
// 112 - открытие (подключение к) камер Basler
// 113 - открытие (подключение к) IP самер

// 115 - инициализация камер HikRobot
//•	Получить список подключённых устройств (камер)
//•	Создать описание всех устройств (камер)

// 116 - Open камер HikRobot
//•	Найти устройство (камеру) по серийному номеру. Возвращает номер камеры в списке камер
//•	Подключение к устройству (камере)
//•	Настройка устройства (камеры)

// 117 - Отправка команды на включение захвата кадров камер HikRobot


// 121 - Алгоритм получения изображений для калибровки getCalibrateImages
// 122- Калибровка стереокамеры CalibrateStereoCam


// =====================================
// (06) Одометрия (Леша) (группа 06)
// =====================================
// 160 - Одометрия ADS::cl_DenseStereo::Odometry(bool exitCode);


// =====================================
// (07) Одометрия (Леша) (группа 06)
// =====================================
// 178 - Старт стрима




// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// (II) Цикл потока. Алгоритмы внутри цикла
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


// =====================================
// (01) Алгоритмы захвата кадров (группа 01)
// =====================================
// 11 - захват кадра с Basler Cam
// 12 - захват кадра с IP камер
// 13 - захват кадра из видео
// 14 - захват кадра из фала изображения

// 17 - Захват кадров GetImage камер HikRobot


// =====================================
// (02) Алгоритмы предобработки изображений (группа 02)
// =====================================
// 21 -
// 22 -
// 23 -  Ректификация изображений
// 24 -  Пред обработка изображений




// =====================================
// (03) Алгоритмы поиска 3D точек (группа 03)
// =====================================
// 31 - (Dence Stereo) алгоритмы на основе построения карты диспаратности
// установка  рабочей области setWorkingAria
// установка параметров алгоритма setParametersSBM
// алгоритм 3D  поиска точек


// 32 - (Sparce Stereo) алгоритмы на основе поиска ключевых точек


// 35 -  (Rotation3Dpoints) Вращение 3D точек


// =====================================
// (04) Алгоритмы анализа 3D точек (группа 04)
// =====================================
// 41 - Кластеризация DBScan
// 42 - Собственный алгоритм кластеризации
// 43 - Обработчик событий от мыши
// 44 - Пересчет координат камеры в координаты робота
// 45 - Объеденненая определение и прорисовка примитивов
// 46 - примитив для контура (для режима Аврора)


// =====================================
// (05) Алгоритмы нанесения меток (группа 05)
// =====================================
// 51 - нанесение результатов на изображение
// 52 - нанесение результатов на Карту диспаратности
// 53 - нанесение доп. информации на изображение с результатами
// 54 -
// 55 -



// =====================================
// (06) Сохранение результатов (группа 06)
// =====================================
// 61 -
// 62 -
// 63 -
// 64 -
//


// =====================================
// (07)  (группа 07) Видео Стирм
// =====================================
// 71 - Запуск видео стрима
// 72 -
// 73 -
// 74 -
//

// =====================================
// (08) Режим Аврора, для контуров (группа 08)
// =====================================
// 81 -
// установка параметров цвета в HSV для поиска
// поиск контуров
// определение маски объекта определенного цвета
// поиск самого большого контура


// 82 - добавляет контура на изображение
// 83 - сопоставление 3д точек с контуром
// 84 - сохранение результатов в файл
// 85 -


// =====================================
// (09)  дополнительно (группа 09)
// =====================================
// 91 - Алгоритм формирования стереопары из одельных снимков








// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//  (III) Алгоритмы после цикла потоков
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// =====================================
// (01) Алгоритмы захвата кадров (группа 01)
// =====================================

// 911 - Остановка захвата кадров с камер Basler obj.stopCapture();
// 912 - Отключение (закрытие) камер Basler obj.closeBaslerCam();

// 915 - Остановка работы устройства (камеры) HikRobot
// 916 - Отключение и Очистка HikRobot

// 971 - Стоп стрима


// 999 - Авто рестарт потока settings.idxThreadAutoRestart.at(q_thread) = true;






// =====================================
// ОБЩАЯ Функция для всех потоков
// =====================================

void threadFuncCommon(ADS::cl_DenseStereo& obj,
                      str_Settings& settings,
                      TCPServer& objTcpServer,
                      TCPClient& objTcpClient,
                      int q_thread)


{

    //==============================
    // HikRobot
    //==============================
    int HR_nRet;                       // статус выполнения
    std::vector<cv::Mat> HR_Imgs;  // Изображения с камер
    HR_Imgs.resize(settings.CamSN.size());

    bool flag_ConnectToCam;    // флаг есть ли подключение к камерам для их автозапуска


     cl_VideoStreamer* objp_VideoStreamer = settings.Objs_ptr.objp_VideoStreamer; // Ссылка на объект класса для Стрима видео


     //===================================
     //  DarkNet-test-simple
     //===================================
     ADS::DarkNetDetector * DND = new ADS::DarkNetDetector;



    cout << "Thread0"<< q_thread << ": Begin  \n";



    // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //  (I) Алгоритмы перед циклом потоков
    // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

    for (int q = 0; q < int(settings.Algorithm_inThread_seq.at(q_thread).size()); q++ )
    {


//        if ( settings.Algorithm_inThread_seq[q_thread][q] < 100 &&
//            settings.Algorithm_inThread_seq[q_thread][q] > 300 ) continue;

        switch (settings.Algorithm_inThread_seq[q_thread][q])
        {
        case 0:

            // пустой
            break;

        case 1000:

#if defined(_WIN64)
            // ################################
            Sleep(1000);
            // ################################
#else \
    // ################################
            usleep(1000*1000);
            // ################################
#endif


            break;


            // =====================================
            // (01) Алгоритмы захвата кадров (группа 01)
            // =====================================

        case 111:
            // 111 - инициализация камер Basler
            obj.InitializeBaslerCam();
            break;
        case 112:
            // 112 -  подключение к камерам Basler

//            obj.set_SerialNumber_Cams(
//                {
//                 QString::number( settings.SerialNumber_StereoCam00left ).toStdString().c_str(),
//                 QString::number( settings.SerialNumber_StereoCam01right ).toStdString().c_str(),
//                 QString::number( settings.SerialNumber_HandCam02left ).toStdString().c_str(),
//                 QString::number( settings.SerialNumber_HandCam03right ).toStdString().c_str()
//                }
//                );
            obj.openBaslerCam();

#if defined(_WIN64)

            // ################################
            Sleep(settings.timeDelay_startThread);
            // ################################

#else

            // ################################
            usleep(settings.timeDelay_startThread*1000);
            // ################################

#endif

            break;

        case 113:
            // Загрузка из камеры
            obj.set_StreamAddress_IPcams(
                                         {settings.videoStreamAddressLeft,
                                          settings.videoStreamAddressRight,
                                          settings.videoStreamAddressLeftHand,
                                          settings.videoStreamAddressRightHand});
            obj.openIPcam();
            break;


        case 115:
            // 115 - инициализация камер HikRobot
            HR_nRet = obj.HRCam_GetCamList();              // Получить список подключённых устройств (камер)
            HR_nRet = obj.HRCam_CreateHandleAllCam();     // Создать описание всех устройств (камер)


            break;

        case 116:
        {
            // 116 - Open камер HikRobot

            for(size_t q = 0; q < settings.CamSN.size(); ++q)
            {

                settings.HikVision.HR_CamIndex[q] = obj.HRCam_FindCamBySN(settings.CamSN[q]); // Найти устройство (камеру) по серийному номеру. Возвращает номер  камеры в списке камер

            }


            for(size_t q = 0; q < settings.CamSN.size(); ++q)
            {

                HR_nRet = obj.HRCam_ConnectToCam(settings.HikVision.HR_CamIndex[q]);            // Подключение к устройству (камере)

                HR_nRet = obj.HRCam_SetCamSettings(settings.HikVision.HR_CamIndex[q]);   // Настройка устройства (камеры)

            }


            break;

        }
        case 117:

            // 117 - StartGrabbing камер HikRobot


            for(size_t q = 0; q < settings.CamSN.size(); ++q)
            {

                HR_nRet = obj.HRCam_StartGrabbing(settings.HikVision.HR_CamIndex[q]);   // Получить изображение c устройсва (камеры), полученных во время захвата

            }


            break;






        case 121:

            obj.getCalibrateImages( settings.setNewCalib_fileList.c_str(),
                                    settings.setNewCalibImg_folderName.c_str(),
                                   &(settings.exitCode) );

            if (settings.exitCode.at(q_thread))
            {
                obj.CalibrateStereoCam(settings.setNewCalib_fileNamePath.c_str(),
                                       settings.usedNewCalib_fileList.c_str(),
                                       settings.board_w,
                                       settings.board_h,
                                       settings.squareSize);
            }

            break;


        case 160:
            // Одометрия
           // obj.Odometry(settings.exitCode.at(q_thread));
            break;



        case 170:
            // Запуск TCP сервера
            objTcpServer.StartTcpServer();
            break;


        case 171:
            //
            objTcpClient.StartTcpClient();

            break;

        case 172:
            //
            objTcpClient.ConnectToServer("192.168.56.1", 10099); // 10.32.31.22    192.168.56.1
            break;


        case 173:
            //
            objTcpClient.SendToServer("000000000000");
            break;


        case 175:
            //




            break;



        case 178:
            //


            // objp_VideoStreamer->StartStreamer();

            settings.Objs_ptr.objp_VideoStreamer->StartStreamer();


            break;


        case 179:
            //
            cv::Mat frame =  obj.getResultImage();

            //                objp_VideoStreamer->set_imgStream(frame);
            //                objp_VideoStreamer->Start_VideoStream();

            settings.Objs_ptr.objp_VideoStreamer->set_imgStream(frame);
            settings.Objs_ptr.objp_VideoStreamer->Start_VideoStream_CycleIn();

            break;




        } // switch

    } // for




    // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    // (II) Цикл потока. Алгоритмы внутри цикла
    // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\




    do
    {



        // Алгоритм выхода из цикла если "-1"
        int dexit = 0;
        for (int q = 0; q < int(settings.Algorithm_inThread_seq.at(q_thread).size()); q++ )
        {
            // выход из цикла потоков, цикл не запустится если будет стоять -1
            if (settings.Algorithm_inThread_seq[q_thread][q] == -1)
            {
                dexit = 1;
                break;
            }
        }
        if (dexit == 1) break;


        // +++++++++++++++++++++++++++++++++++++
        // Время (начало)
        // +++++++++++++++++++++++++++++++++++++
        int64 time = cv::getTickCount();
        // +++++++++++++++++++++++++++++++++++++




        for (int q = 0; q < int(settings.Algorithm_inThread_seq.at(q_thread).size()); q++ )
        {

            switch (settings.Algorithm_inThread_seq[q_thread][q])
            {


            case 0:
                // пустой
                break;

                // =====================================
                // (01) Алгоритмы захвата кадров (группа 01)
                // =====================================

            case 11: // !! связано ниже после цикла do {}
                // 1 - захват кадра с Basler Cam
                obj.captureContinuous(settings.exitCode[q]);
                break;
            case 12:
                // 2 - захват кадра с IP камер
                obj.CaptureFromIP();
                break;
            case 13:
                // 3 - захват кадра из видео
                obj.CaptureFromVideo(settings.video1Path.c_str(),
                                     settings.video2Path.c_str(),
                                     &(settings.exitCode)  );
                break;
            case 14:
                // 4 - захват кадра из фала изображения
                obj.CaptureFromFile(settings.img1Path.c_str(), settings.img2Path.c_str());

                break;

            case 15:
                // 2 - захват кадра с IP камер
                obj.CaptureFromIP_SingleCam(0, true, false);
                break;
            case 16:
                // 2 - захват кадра с IP камер
                obj.CaptureFromIP_SingleCam(1, true, false);
                break;

            case 17:
            {
                // 17 - GetImage камер HikRobot

                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                // Занесение информации о состоянии для передачи по TCP
                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

                std::vector<bool> HR_nRet_vec(2,false);               // статус выполнения

                // ------------------------------------------
                // ------------------------------------------
                // проверка на подключение устройств (камер)
                for(size_t q = 0; q < settings.CamSN.size(); ++q)
                {
                    HR_nRet_vec[q] = obj.HRCam_IsDeviceConnected(settings.HikVision.HR_CamIndex[q]);
                }
                // ------------------------------------------
                // ------------------------------------------

                // Наличие соединения с камерами
                if (HR_nRet_vec[0] && HR_nRet_vec[1])
                {
                    obj.ConnectToCams_Cam = 1;
                    settings.Aurora.InitialStateInformation.ConnectToCams = 1;
                }
                else
                {
                    obj.ConnectToCams_Cam = 0;
                    settings.Aurora.InitialStateInformation.ConnectToCams = 0;
                }
                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

                // ------------------------------------------
                // ------------------------------------------
                for (size_t q = 0; q < settings.CamSN.size(); ++q)
                {
                    HR_Imgs[q] = obj.HRCam_GetImage(settings.HikVision.HR_CamIndex[q]);    // Включение работы устройсва (камеры) - запуск захват кадров
                }
                // ------------------------------------------
                // ------------------------------------------


                // Получение изображения камерами

                if (!HR_Imgs[0].empty() && !HR_Imgs[1].empty())
                {
                    obj.ImageAquisition_Cam = 1;
                    settings.Aurora.InitialStateInformation.ImageAquisition = 1;
                }
                else
                {
                   obj.ImageAquisition_Cam = 0;
                   settings.Aurora.InitialStateInformation.ImageAquisition = 0;
                }
                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                // Факт успешной работы  (только в начале работы)
                if ( obj.SecondaryPowerSupply_Cam == 1 &&
                    obj.ConnectToCams_Cam == 1 &&
                    obj.ImageAquisition_Cam == 1 &&
                    obj.ReadyForOperationCompartmentLamp_Cam== 1)
                {
                    obj.FuctSuccessfulOperation_Cam = 1;
                    settings.Aurora.InitialStateInformation.FuctSuccessfulOperation = 1;
                }
                else
                {
                    obj.FuctSuccessfulOperation_Cam = 0;
                    settings.Aurora.InitialStateInformation.FuctSuccessfulOperation = 0;
                }

                // информация о состоянии StatusStateInformation (отсылается постоянно)
                obj.StatusSuccessfulOperation_Cam = obj.FuctSuccessfulOperation_Cam;
                settings.Aurora.StatusStateInformation.StatusSuccessfulOperation =
                    settings.Aurora.InitialStateInformation.FuctSuccessfulOperation;
                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

                // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>




//                if ( !obj.HRCam_IsDeviceConnected(settings.HikVision.HR_CamIndex[q]) ) {break; settings.exitCode.at(q_thread) = false;}
//                if ( !obj.HRCam_IsDeviceConnected(settings.HikVision.HR_CamIndex[q]) ) {break; settings.exitCode.at(q_thread) = false;}

//                if (HR_Imgs[0].empty() ) {break; settings.exitCode.at(q_thread) = false;}
//                if (HR_Imgs[1].empty() ) {break; settings.exitCode.at(q_thread) = false;}


                if ( !HR_Imgs[0].empty() ) obj.setImgOpenCV_1left(HR_Imgs[0]);
                if ( !HR_Imgs[1].empty() ) obj.setImgOpenCV_2right(HR_Imgs[1]);
                if ( !HR_Imgs[0].empty() && !HR_Imgs[1].empty() ) obj.setImgOpenCV_StereoPair(HR_Imgs[0], HR_Imgs[1]);

                break;
            }
            case 18:

                // 18 - GetImage камер HikRobot для устройства (камеры) 0

                    HR_Imgs[0] = obj.HRCam_GetImage(settings.HikVision.HR_CamIndex[0]);    // Включение работы устройсва (камеры) - запуск захват кадров
                    if ( !HR_Imgs[0].empty() ) obj.setImgOpenCV_1left(HR_Imgs[0]);

                   // HR_Imgs[1] = obj.getImgOpenCV_2right();
                    //if ( !HR_Imgs[0].empty() && !HR_Imgs[1].empty() ) obj.setImgOpenCV_StereoPair(HR_Imgs[0], HR_Imgs[1]);

                   if ( !HR_Imgs[0].empty() ) obj.setImgOpenCV_StereoPair_SingleImg(0,HR_Imgs[0]);

                break;

            case 19:

                // 19 - GetImage камер HikRobot для устройства (камеры) 1

                HR_Imgs[1] = obj.HRCam_GetImage(settings.HikVision.HR_CamIndex[1]);    // Включение работы устройсва (камеры) - запуск захват кадров
                if ( !HR_Imgs[1].empty() ) obj.setImgOpenCV_2right(HR_Imgs[1]);

                //HR_Imgs[0] = obj.getImgOpenCV_1left();
                // if ( !HR_Imgs[0].empty() && !HR_Imgs[1].empty() ) obj.setImgOpenCV_StereoPair(HR_Imgs[0], HR_Imgs[1]);


                if ( !HR_Imgs[1].empty() ) obj.setImgOpenCV_StereoPair_SingleImg(1,HR_Imgs[1]);

                break;



                // =====================================
                // (02) Алгоритмы предобработки изображений (группа 02)
                // =====================================
            case 21:
//                // 21 - Алгоритм получения изображений для калибровки getCalibrateImages
//                obj.getCalibrateImages( settings.setPath_fileList.c_str(),
//                                       settings.setCalibrationImg_folderName.c_str(),
//                                       &(settings.exitCode) );
//                // 22 - Калибровка стереокамеры CalibrateStereoCam
////                if (settings.exitCode.at(q_thread))
////                {
//                    obj.CalibrateStereoCam(settings.setCalibration_fileNamePath.c_str(),
//                                           settings.usedPath_fileList.c_str(),
//                                           settings.board_w,
//                                           settings.board_h,
//                                           settings.squareSize);
////                }

                break;


            case 22:
            {



                break;
            }
            case 23:
            {

                obj.ImgRemaping();

                break;
            }
            case 24:
            {

                obj.ImgPerProcessing(settings.metod_ImagePerProcessing);


                break;
            }

            case 25:
            {
                vector<cv::Mat>  StereoPair_temp = obj.getImgRemap_StereoPair();
                vector<cv::Mat>  cam01_temp, cam02_temp;


                cam01_temp = obj.ImgPerProcessing_OneImg(StereoPair_temp[0].clone(),settings.metod_ImagePerProcessing);
                cam02_temp = obj.ImgPerProcessing_OneImg(StereoPair_temp[1].clone(),settings.metod_ImagePerProcessing);

                obj.setImgColorPreProcess_StereoPair( {cam01_temp.at(0), cam02_temp.at(0)} );
                obj.setImgGrayPreProc_StereoPair( {cam01_temp.at(1), cam02_temp.at(1)} );



                break;
            }


                // =====================================
                // (03) Алгоритмы поиска 3D точек (группа 03)
                // =====================================

            case 31:

                // 31 - Dence Stereo алгоритмы на основе построения карты диспаратности
                obj.setWorkingAria(settings.limit_WorkingArea);

                obj.setParametersSBM(
                                     settings.smbNumDisparities,
                                     settings.smbBlockSize,
                                     settings.smbPreFilterCap,
                                     settings.smbMinDisparity,
                                     settings.smbTextureThreshold,
                                     settings.smbUniquenessRatio,
                                     settings.smbSpeckleWindowSize,
                                     settings.smbSpeckleRange,
                                     settings.smbDisp12MaxDiff);

                obj.FindPointsADS(settings.limit_points, settings.metod_Disparity);

                break;

            case 32:
                 // 32 - Sparce Stereo алгоритмы на основе поиска ключевых точек
                obj.FindPointsASS();
                break;

            case 33:

                break;


            case 35:

                //    double angx; // угол поворота вокруг оси x центра вращения
                //    double angy; // угол поворота вокруг оси y центра вращения
                //    double angz; // угол поворота вокруг оси z центра вращения
                //    double tx;   // смещение вдоль оси x центра вращения
                //    double ty;   // смещение вдоль оси y центра вращения
                //    double tz;   // смещение вдоль оси z центра вращения
                //    double Scale; // Масштаб
                //    double Dz;    // смещение центра вращения от камер вдоль оси z




                obj.Rotation3Dpoints( settings.Rotation3Dpoints.angx,
                                          settings.Rotation3Dpoints.angy,
                                          settings.Rotation3Dpoints.angz,
                                          settings.Rotation3Dpoints.tx,
                                          settings.Rotation3Dpoints.ty,
                                          settings.Rotation3Dpoints.tz,
                                          (0.01 * settings.Rotation3Dpoints.Scale),
                                          settings.Rotation3Dpoints.Dz);


                break;


                // =====================================
                // (04) Алгоритмы анализа 3D точек (группа 04)
                // =====================================

            case 41:
                // Кластеризация DBScan
                obj.clusteringDBScan(settings.rad,
                                     settings.neighbors,
                                     settings.filter_ClusterPoints,
                                     settings.coef_base_dbs,
                                     settings.coef_pow_dbs,
                                     settings.metod_PointCorrectinForClastering);

                break;


            case 42:
                // 42 - Собсвенный алгоритм кластеризации
                obj.Clustering();
                break;



            case 43:
                // Обработчик событий от мыши
                // MouseClickSelector - Парамет выбора способа ввода кординат клика мыши
                // 0 - ввод через окно OpenCV
                // 1 - ввод из стороннего источника
                obj.eventMouse_SelectObject(settings.MouseClickSelector);
                break;

            case 44:
                // Пересчет координат камеры в координаты робота
                // obj.Coordinate_cam2robot();
                //  Вывод 3D точек
                //obj.Rotation3Dpoints(0,0,0,0,0,0);
                break;

            case 45:
                // Вычисление примитива
                //obj.DetectPrimitive_all();

                // Создание изображения с примитивами
                //obj.AddPrimitiveToIm();

                // объеденненая определение и прорисовка примитивов
                obj.DetectAddPrimitive();
                break;

            case 46:
                // примитив для контура
                obj.DetectAddPrimitiveForContour();
                break;

            case 47:
                // примитив для контура
                obj.DetectObjSize();
                break;







                // =====================================
                // (05) Алгоритмы нанесения меток (группа 05)
                // =====================================
            case 51:
                // нанесение результатов на изображение
                obj.AddResultsToIm(settings.mode_AddResults);
                break;
            case 52:
                // нанесение результатов на Карту диспаратности
                obj.AddResultsToDisparityMap02();
                break;

            case 53:
                // нанесение доп. информации на изображение с результатами
                obj.AddResultsToDisparityMap03();
                break;



                // =====================================
                // (06) Алгоритмы сохрания результатов (группа 06)
                // =====================================
            case 61:


                break;




                // =====================================
                // (07) TCP (группа 07) Видео Стрим
                // =====================================
            case 71:
            {

                // cl_VideoStreamer* objp_VideoStreamer = settings.Objs_ptr.objp_VideoStreamer;

                //                objp_VideoStreamer->set_imgStream(frame);
                //                objp_VideoStreamer->Start_VideoStream();


                //  cv::Mat frame =  obj.getResultImage();

                // 01
                cv::Mat frame1 =  obj.getShowImg();

                settings.Objs_ptr.objp_VideoStreamer->setStreamQualiy(settings.VideoStreamer.StreamQualiy);
                settings.Objs_ptr.objp_VideoStreamer->setStreamPublishPath("/pc");
                settings.Objs_ptr.objp_VideoStreamer->set_imgStream(frame1);
                settings.Objs_ptr.objp_VideoStreamer->VideoStream();

                // 02
                cv::Mat frame2 =  obj.getResultImage();

                settings.Objs_ptr.objp_VideoStreamer->setStreamQualiy(settings.VideoStreamer.StreamQualiy);
                settings.Objs_ptr.objp_VideoStreamer->setStreamPublishPath("/result");
                settings.Objs_ptr.objp_VideoStreamer->set_imgStream(frame2);
                settings.Objs_ptr.objp_VideoStreamer->VideoStream();


                // 03

                cv::Mat frame3 =  obj.getDisparityMapImage02();

                settings.Objs_ptr.objp_VideoStreamer->setStreamQualiy(settings.VideoStreamer.StreamQualiy);
                settings.Objs_ptr.objp_VideoStreamer->setStreamPublishPath("/disp");
                settings.Objs_ptr.objp_VideoStreamer->set_imgStream(frame3);
                settings.Objs_ptr.objp_VideoStreamer->VideoStream();


                // 04
                cv::Mat frame4 =  obj.getImgColorPreProcess_DoubleStereoImg();

                settings.Objs_ptr.objp_VideoStreamer->setStreamQualiy(settings.VideoStreamer.StreamQualiy);
                settings.Objs_ptr.objp_VideoStreamer->setStreamPublishPath("/stereo");
                settings.Objs_ptr.objp_VideoStreamer->set_imgStream(frame4);
                settings.Objs_ptr.objp_VideoStreamer->VideoStream();



                // 05

                cv::Mat frame5 =  obj.getImg3Dpoins();

                settings.Objs_ptr.objp_VideoStreamer->setStreamQualiy(settings.VideoStreamer.StreamQualiy);
                settings.Objs_ptr.objp_VideoStreamer->setStreamPublishPath("/3Dp");
                settings.Objs_ptr.objp_VideoStreamer->set_imgStream(frame5);
                settings.Objs_ptr.objp_VideoStreamer->VideoStream();



            }
                break;


            case 75:
            {
                //===================================
                //  DarkNet-test-simple
                //===================================


                // vector<cv::Mat>  StereoPair_temp = obj.getImgRemap_StereoPair();
               // bool reply2 =  ADS::dnn_check(network, StereoPair_temp.at(0), true);


                cv::Mat  Img_temp = obj.getImgOpenCV_1left();
               // cv::Mat  Img_temp = cv::imread("./files/nn/imgLeft_1001.bmp", cv::IMREAD_UNCHANGED);

                std::cout << "===>>> imread = " << Img_temp.empty() << std::endl;

                bool reply2 =  DND->check( Img_temp, true);

                std::cout << "===>>> reply2 = " << reply2 << std::endl;

                settings.DarkNet.reply = reply2;

                if (reply2)
                std::cout << "===>>> Neural network been reply !!!!!!!!!!!!!!!!!!!" << std::endl;


                std::vector<std::string> info = DND->info();


                settings.DarkNet.replyAll = info;

                for (size_t i = 0; i < info.size(); i++) std::cout << info[i];



            }
                break;

                // =====================================
                // (08) Режим Аврора, для контуров (группа 08)
                // =====================================

            case 81:
                obj.setFindColor(settings.LowHSV, settings.HieghHSV); // установка параметров цвета в HSV для поиска
                obj.SearchContours();             // поиск контуров
                obj.FindColorObjMask();           // определение маски объекта определенного цвета
                obj.SearchBigObjContours();       // поиск самого большого контура
                break;

            case 82:
                obj.AddContoursToIm();          // добавляет контура на изображение
                break;

            case 83:
                obj.matchContoursTo3Dpoints();    // сопоставление 3д точек с контуром
                break;
            case 84:
                 obj.SaveInfColorObj();            // сохранение результатов в файл
                break;
            case 85:

                break;

            case 86:



                break;



                // Авто запуск устройств (камер)
            case 90:



#if defined(_WIN64)

                // ################################
                Sleep(500);
                // ################################

#else

                // ################################
                usleep(500*1000);
                // ################################

#endif





                // Если разрыва не было то выход
                if (!settings.LossCamconnection) break;


                HR_nRet = obj.HRCam_GetCamList();   // Получить список подключённых устройств (камер)


                // флаг есть ли подключение к камерам для их автозапуска
                flag_ConnectToCam = true;

                for(size_t q = 0; q < settings.CamSN.size(); ++q)
                {



                    // obj.HRCam_FindCamBySN(settings.CamSN[q]); // Найти устройство (камеру) по серийному номеру. Возращает номер  камеры в списке камер
                    // Если какая либо из камер не подлкючена те  false
                    flag_ConnectToCam &= ( obj.HRCam_FindCamBySN(settings.CamSN[q]) != (unsigned int) 9999 );

                }





                cout << "!settings.exitCode[1] && flag_ConnectToCam && settings.LossCamconnection = " << (!settings.exitCode[1] && flag_ConnectToCam && settings.LossCamconnection) << endl;

                cout << "!settings.exitCode[1]  = " <<  (!settings.LossCamconnection) << endl;
                cout << "flag_ConnectToCam  = " <<   flag_ConnectToCam  << endl;
                cout << "settings.LossCamconnection = " <<  settings.LossCamconnection << endl;




                // Если поток отключен, камеры найдены, флаг потери соединения с камерами активен то запустить поток
//                if (!settings.exitCode[1] && flag_ConnectToCam && settings.LossCamconnection )
                  if (flag_ConnectToCam && settings.LossCamconnection )
                {

                    settings.exitCode[1] = true ;
                    settings.LossCamconnection = false;
                    flag_ConnectToCam = true;

                }



                break;



            case 99:

                cout << "settings.exitCode[0] = " << settings.exitCode[0] << endl ;
                cout << "settings.exitCode[1] = " << settings.exitCode[1] << endl ;
                cout << "settings.exitCode[2] = " << settings.exitCode[2] << endl ;
                cout << "settings.exitCode[3] = " << settings.exitCode[3] << endl ;
                cout << "settings.exitCode[4] = " << settings.exitCode[4] << endl ;
                cout << "settings.exitCode[5] = " << settings.exitCode[5] << endl ;
                cout << "settings.exitCode[6] = " << settings.exitCode[6] << endl ;
                cout << "settings.exitCode[7] = " << settings.exitCode[7] << endl ;


#if defined(_WIN64)

                // ################################
                Sleep(3000);
                // ################################

#else

                // ################################
                usleep(3000*1000);
                // ################################

#endif


                break;


            } // switch

        } // for



        if (settings.Algorithm_inThread_seq[q_thread][0] != -1)
        {
            // +++++++++++++++++++++++++++++++++++++
            // Время (Конец)
            // +++++++++++++++++++++++++++++++++++++
            time = cv::getTickCount() - time;

            cout << "\n  >>    time Thread0" << q_thread << ": " << time*1/cv::getTickFrequency() << endl;

            int delay = settings.timeMsMinDelay.at(q_thread) - int(time * 1000/cv::getTickFrequency() );

            // Занесение времени работы потока в общие данные
            settings.timeMsThreadRuntime.at(q_thread) = int( time*1000/cv::getTickFrequency() );


            if ( delay > 1 )
            {

                cout << " >>    delay Thread0" << q_thread << ":: " << delay  << endl;
                // +++++++++++++++++++++++++++++++++++++

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



    }while (settings.exitCode.at(q_thread));



    // сигнализирует, что произведен выход из цикла потока
    settings.exitCode.at(q_thread) = false;


    cout << "============== EXIT FROM Thead #" << q_thread << " ================" << endl;




#if defined(_WIN64)

    // ################################
    Sleep(2000);
    // ################################

#else

    // ################################
    usleep(2000*1000);
    // ################################

#endif




    // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //  (III) Алгоритмы после цикла потоков
    // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

    for (int q = 0; q < int(settings.Algorithm_inThread_seq.at(q_thread).size()); q++ )
    {

        switch (settings.Algorithm_inThread_seq[q_thread][q])
        {
        case 0:
            // пустой
            break;

            // =====================================
            // (01) Алгоритмы захвата кадров (группа 01)
            // =====================================

        case 911:
            //
            obj.stopCapture();

            break;
        case 912:
            //
            obj.closeBaslerCam();
            break;



        case 915:

            for(size_t q = 0; q < settings.CamSN.size(); ++q)
            {
                // 915 - Остановка работы устройства (камеры) HikRobot
                HR_nRet = obj.HRCam_StopGrabbing(settings.HikVision.HR_CamIndex[q]); // Остановка работы устройства (камеры)
            }

            break;

        case 916:

            for(size_t q = 0; q < settings.CamSN.size(); ++q)
            {
                // 916 - Отключение и Очистка HikRobot
                HR_nRet = obj.HRCam_DisconnecCam(settings.HikVision.HR_CamIndex[q]); // Отключение от усройства
                obj.HRCam_DestroyHandle(); // Очистка
                break;
            }


        case 971:
        {

            // cl_VideoStreamer* objp_VideoStreamer = settings.Objs_ptr.objp_VideoStreamer;
            objp_VideoStreamer->Stop_VideoStream();


        }






        case 999:


#if defined(_WIN64)

            // ################################
            Sleep(5000);
            // ################################

#else

            // ################################
            usleep(5000*1000);
            // ################################

#endif


            // Авто рестарт потока
            settings.idxThreadAutoRestart.at(q_thread) = true;






            break;





        } // switch

    } // for






}






// =====================================
// Функция запустка всех выбранных потоков
// =====================================
void  Start_certainThread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr, int idxThread)
{


        // если не стоят номера алгоритмов то поток не запускается


        if (!settings_ptr->exitCode.at(idxThread)) return; // добавил новая, раньше не было

        if (settings_ptr->Algorithm_inThread_seq.at(idxThread).empty())
        {
            settings_ptr->exitCode.at(idxThread) = false;
            return;
        }


        TCPServer* objTcpServer_ptr = settings_ptr->Objs_ptr.objTcpServer_ptr;
        TCPClient* objTcpClient_ptr = settings_ptr->Objs_ptr.objTcpClient_ptr;

        settings_ptr->threads.at(idxThread) = std::thread(threadFuncCommon,
                      ref(*obj_ptr),
                      ref(*settings_ptr),
                      ref(*objTcpServer_ptr),
                      ref(*objTcpClient_ptr),
                      idxThread );


#if defined(_WIN64)


        // ################################
        Sleep(settings_ptr->timeDelay_startThread);
        // ################################
#else

        // ################################
        usleep(settings_ptr->timeDelay_startThread*1000);
        // ################################

#endif

        settings_ptr->exitCode.at(idxThread) = true;

        settings_ptr->threads.at(idxThread).detach();

}




cv::Mat  getImg02(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr, int idx, int selectorImage)
//void  ShowResults()
{

    //  Вывод изображений на экран
    // idx - отображение окна:
    // 0 - откл.
    // 1 - вкл.
    // Альтернатива просто закоментировать

    // Получение изображения
    // первый входной аргумент фунции - это номер камеры:

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




    // =====================================
    // Время (начало)
    // =====================================
    int64 time = cv::getTickCount();
    // =====================================
    cv::Mat m;
    vector<cv::Mat> Stereo_m;

    if (idx == 1)
    {



        switch ( selectorImage )
        {
        case 0:
            m = obj_ptr->getImgOpenCV_1left();
            break;
        case 1:
            m = obj_ptr->getImgOpenCV_2right();
            break;
        case 2:
            m = obj_ptr->getImgOpenCV_3leftHand();
            break;
        case 3:
            m = obj_ptr->getImgOpenCV_4rightHand();
            break;

        case 9:
            m = obj_ptr->getImgOpenCV_DoubleStereoImg();
            break;

        case 10:
        {
            m = obj_ptr->getResultImage();
            break;
        }

        case 11:
            m = obj_ptr->getDisparityMapImage();
            break;
        case 12:
            m = obj_ptr->getDisparityMapImage02();
            break;
        case 13:
            m = obj_ptr->getDisparityMapImage03();
            break;

        case 14:
            Stereo_m = obj_ptr->getImgGrayPreProc_StereoPair();
            m = Stereo_m.at(0);


            break;

        case 15:
            Stereo_m = obj_ptr->getImgRemap_StereoPair();
            m = Stereo_m.at(0);

            break;

        case 16:
            Stereo_m = obj_ptr->getImgRemap_StereoPair();
            m = Stereo_m.at(1);

            break;
        case 17:
            m = obj_ptr->getImgRemap_DoubleStereoImg();

            break;

        case 20:
            Stereo_m = obj_ptr->getImgColorPreProcess_StereoPair();
            m = Stereo_m.at(0);

            break;

        case 21:
            Stereo_m = obj_ptr->getImgColorPreProcess_StereoPair();
            m = Stereo_m.at(1);

            break;
        case 22:
            m = obj_ptr->getImgColorPreProcess_DoubleStereoImg();

            break;

        case 25:
            m = obj_ptr->getResultPrimitiveGeneralImage();
            break;
        case 26:
            m = obj_ptr->getResultPrimitivesXYImage();
            break;
        case 27:
            m = obj_ptr->getResultPrimitivesXZImage();
            break;
        case 28:
            m = obj_ptr->getResultPrimitivesYZImage();
            break;

        case 30:
            m = obj_ptr->getResultColorSearchImage();
            break;

        case 40:
            m = obj_ptr->getImgFindColorObj();
            break;

        case 50:
            m = obj_ptr->getImg3Dpoins();
            break;


        default:
            cout<<"Error, bad input, quitting\n";
            break;

        }



    }



    // =====================================
    // Время (Конец)
    // =====================================
    time = cv::getTickCount() - time;
    printf("\n <<< >>> Main000: Time ShowResults:      %fsec\n", time*1/cv::getTickFrequency());
    // =====================================
    // Занесение времени работы потока в общие данные
    settings_ptr->timeMsThreadRuntime.at(0) = int( time*1000/cv::getTickFrequency() );

    int delay = settings_ptr->timeMsThreadRuntime.at(0) - (time * 1000/cv::getTickFrequency() );
    if ( delay < 1 ) delay = 1;



    cout << " <<< >>> Main000:ShowResults: delay: "<< delay  << endl;


    #if defined(_WIN64)

        // ################################
        Sleep(delay);
        // ################################

    #else

        // ################################
        usleep(delay*1000);
        // ################################

    #endif

    return m;

}













// =====================================
// Функция для потока 01 (Grabing Images)
// =====================================
void threadFunc001(ADS::cl_DenseStereo& obj,
                   vector<int> q_vec,
                   int timeStep_Thread,
                   string img1Path,
                   string img2Path,
                   vector<bool>& exitCode)

{
//    cout << "Func001: Begin  \n" ;

//    do {


//        // =====================================
//        // Время (начало)
//        // =====================================
//        int64 time = cv::getTickCount();
//        // =====================================



//        // Переменнная для передачи ссылок, так как элемент вектора не предавался  >>> settings.exitCode.at(q_thread)
//          bool exitCode = true;


//        if (q_vec[0] == 1 ) obj.captureContinuous(true); // obj.capture();
//        else if (q_vec[0] == 2 ) obj.CaptureFromIP();

//        else if (q_vec[0] == 3 ) obj.CaptureFromVideo(img1Path.c_str(), img2Path.c_str(), &exitCode);
//        else if (q_vec[0] == 4 ) obj.CaptureFromFile(img1Path.c_str(), img2Path.c_str());




//        // =====================================
//        // Время (Конец)
//        // =====================================
//        time = cv::getTickCount() - time;

//        printf("\n  >     Func001: Time Capture:      %fsec\n", time*1/cv::getTickFrequency());
//        // =====================================


//        int delay = timeStep_Thread - int(time * 1000/cv::getTickFrequency() );

//        if ( delay < 1 ) delay = 1;



//        cout << " >     Func001: CaptureFromCam: delay: "<< delay  << endl;

//#if defined(_WIN64)

//        // ################################
//        Sleep(delay);
//        // ################################

//#else

//        // ################################
//        usleep(delay*1000);
//        // ################################

//#endif






//    } while (exitCode[1]);



//    if (q_vec[0] == 1 )
//    {
//        obj.stopCapture();
//#if defined(_WIN64)

//        // ################################
//        Sleep(500);
//        // ################################

//#else

//        // ################################
//        usleep(500*1000);
//        // ################################

//#endif
//        obj.closeBaslerCam();
//    }



//    cout << "-->> exitCode[1]: " << exitCode[1] << endl;
//#if defined(_WIN64)

//    // ################################
//    Sleep(2000);
//    // ################################

//#else

//    // ################################
//    usleep(2000*1000);
//    // ################################

//#endif



}


// =====================================
// Функция для потока 02 (ADS)
// =====================================

void threadFunc002(ADS::cl_DenseStereo& obj,
                   vector<int> q_vec,
                   int& timeStep_Thread,
                   int& limit_points,                    // уменьшение количесва найденых точек для кластеризации
                   int& smbNumDisparities,
                   int& smbBlockSize,
                   int& smbPreFilterCap,
                   int& smbMinDisparity,
                   int& smbTextureThreshold,
                   int& smbUniquenessRatio,
                   int& smbSpeckleWindowSize,
                   int& smbSpeckleRange,
                   int& smbDisp12MaxDiff,
                   vector<bool>& exitCode )
{



    cout << "Func002: Begin  \n";

    // ==============
    //  3D Points
    // =============

    do {

        // =====================================
        // Время (начало)
        // =====================================
        int64 time = cv::getTickCount();
        // =====================================

        if (q_vec[1] == 1 )
        {

            obj.setParametersSBM( smbNumDisparities,
                                 smbBlockSize,
                                 smbPreFilterCap,
                                 smbMinDisparity,
                                 smbTextureThreshold,
                                 smbUniquenessRatio,
                                 smbSpeckleWindowSize,
                                 smbSpeckleRange,
                                 smbDisp12MaxDiff);

            obj.FindPointsADS(limit_points,0);

        }
        if (q_vec[1] == 2 ) obj.FindPointsASS();


        if (q_vec[1] == 3 )
        {


            // =====================================
            // для контуров
            // =====================================


            obj.setParametersSBM( smbNumDisparities,
                                 smbBlockSize,
                                 smbPreFilterCap,
                                 smbMinDisparity,
                                 smbTextureThreshold,
                                 smbUniquenessRatio,
                                 smbSpeckleWindowSize,
                                 smbSpeckleRange,
                                 smbDisp12MaxDiff);
             obj.FindPointsADS(limit_points,0);


            obj.FindColorObjMask();           // определение маски объекта определенного цвета
            obj.SearchContours();             // поиск контуров
            obj.SearchBigObjContours();       // поиск самого большого контура

            obj.AddContoursToIm();          // добавляет контура на изображение

            obj.matchContoursTo3Dpoints();    // сопоставление 3д точек с контуром
            obj.SaveInfColorObj();            // сохранение результатов в файл

            // ################################
            //Sleep(1000);
            // ################################


        }



        // =====================================
        // Время (Конец)
        // =====================================
        time = cv::getTickCount() - time;

        printf("\n >>    Func002: Time 3D Points:      %fsec\n", time*1/cv::getTickFrequency());
        // =====================================


        int delay = timeStep_Thread - int(time * 1000/cv::getTickFrequency() );
        if ( delay < 1 ) delay = 1;


        cout << " >>    Func002:  Detect3Dpoints: delay: "<< delay  << endl;

#if defined(_WIN64)

        // ################################
        Sleep(delay);
        // ################################

#else

        // ################################
        usleep(delay*1000);
        // ################################

#endif



    } while (exitCode[2]);

}


// =====================================
// Функция для потока 03 (Clustering)
// =====================================
void threadFunc003(ADS::cl_DenseStereo& obj,
                   vector<int> q_vec,
                   int& timeStep_Thread,
                   double& rad,                         // Connectivity radius between points, points may be connected if distance between them less then the radius.
                   size_t& neighbors,
                   int& filter_ClusterPoints,
                   double& coef_base_dbs,
                   double& coef_pow_db,
                   int MouseClickSelector,
                   vector<bool>& exitCode)
{



    cout << "Func003: Begin  \n";
    // ============
    // Clustering
    // =============

    do {

        // =====================================
        // Время (начало)
        // =====================================
        int64 time = cv::getTickCount();
        // =====================================


        if (q_vec[2] == 1 )
        {

            // Кластеризация DBScan
            obj.clusteringDBScan(rad,neighbors, filter_ClusterPoints, coef_base_dbs, coef_pow_db, false);

            // Пересчет координат камеры в координаты робота
           // obj.Coordinate_cam2robot();

            // Обработчик событий от мыши
            obj.eventMouse_SelectObject(MouseClickSelector);


            //  Вывод 3D точек
            //obj.Rotation3Dpoints(0,0,0,0,0,0);


//            // =====================================
//            // для контуров
//            // =====================================
//            obj.FindColorObjMask();           // определение маски объекта определенного цвета
//            obj.SearchContours();             // поиск контуров
//            obj.SearchBigObjContours();       // поиск самого большого контура
//            obj.matchContoursTo3Dpoints();    // сопоставление 3д точек с контуром
//            obj.SaveInfColorObj();            // сохранение результатов в файл

//            // ################################
//            Sleep(2000);
//            // ################################


        }

        if (q_vec[2] == 2 ) obj.Clustering();

        if (q_vec[2] == 3 )
        {



        }





        // =====================================
        // Время (Конец)
        // =====================================
        time = cv::getTickCount() - time;

        printf("\n >>>   Func003: Time Clustering:      %fsec\n", time*1/cv::getTickFrequency());
        // =====================================


        int delay = timeStep_Thread - int(time * 1000/cv::getTickFrequency() );
        if ( delay < 1 ) delay = 1;



        cout << " >>>   Func003: Clustering: delay: "<< delay  << endl;


#if defined(_WIN64)

        // ################################
        Sleep(delay);
        // ################################

#else

        // ################################
        usleep(delay*1000);
        // ################################

#endif


    } while (exitCode[3]);


}



// =====================================
// Функция для потока 04 (Add Results)
// =====================================
void threadFunc004(ADS::cl_DenseStereo& obj,
                   vector<int> q_vec,
                   int timeStep_Thread,
                   vector<int> mode_AddResults,
                   vector<bool>& exitCode)            // фильтр отсеивает прорисовку кластеров с количесвом точек менее чем ...

{



    cout << "Func004: Begin  \n" ;


    do {


        // =====================================
        // Время (начало)
        // =====================================
        int64 time = cv::getTickCount();
        // =====================================


        if (q_vec[3] == 0 )
        {

            // Обработчик событий от мыши
            // obj.eventMouse_SelectObject(MouseClickSelector);

            // нанесение результатов на изображение
            obj.AddResultsToIm(mode_AddResults);

            // нанесение результатов на изображение 02 и 03
            obj.AddResultsToDisparityMap02();
            obj.AddResultsToDisparityMap03();

//            // =====================================
//            // для контуров
//            // =====================================
//            obj.AddContoursToIm();          // добавляет контура на изображение


        }

        else if (q_vec[3] == 1 ) obj.searchColor();
        else if (q_vec[3] == 2 ) {}
        else if (q_vec[3] == 3 )
        {
            //  obj.getCalibrateImages( setPath_fileList,  setCalibrationImg_folderName);
            // obj.CalibrateStereoCam(setCalibration_fileNamePath, usedPath_fileList, board_w, board_h, squareSize);
        }
        else if (q_vec[3] == 4 )
        {

            // =====================================
            // для контуров
            // =====================================
          //  obj.AddContoursToIm();          // добавляет контура на изображение

        };


        // =====================================
        // Время (Конец)
        // =====================================
        time = cv::getTickCount() - time;

        printf("\n >>>>  Func004: Time Add Results:      %fsec\n", time*1/cv::getTickFrequency());
        // =====================================


        int delay = timeStep_Thread - int(time * 1000/cv::getTickFrequency() );
        if ( delay < 1 ) delay = 1;


        cout << " >>>>  Func004: Add Results: delay: "<< delay  << endl;



#if defined(_WIN64)

        // ################################
        Sleep(delay);
        // ################################

#else

        // ################################
        usleep(delay*1000);
        // ################################

#endif


    } while (exitCode[4]);


}




// =====================================
// Функция для потока 05 (Primitives)
// =====================================
void threadFunc005(ADS::cl_DenseStereo& obj,
                   vector<int> q_vec,
                   int timeStep_Thread,
                   vector<bool>& exitCode)

{



    cout << "Func005: Begin  \n" ;


    do {


        // =====================================
        // Время (начало)
        // =====================================
        int64 time = cv::getTickCount();
        // =====================================




        // Вычисление примитива
        //obj.DetectPrimitive_all();

        // Создание изображения с примитивами
        //obj.AddPrimitiveToIm();

        // объеденненая определение и прорисовка примитивов
        obj.DetectAddPrimitive();



        // =====================================
        // Время (Конец)
        // =====================================
        time = cv::getTickCount() - time;

        printf("\n >>>>>  Func005: Time Primitive:      %fsec\n", time*1/cv::getTickFrequency());
        // =====================================


        int delay = timeStep_Thread - int(time * 1000/cv::getTickFrequency() );
        if ( delay < 1 ) delay = 1;


        cout << " >>>>>  Func005: Primitive: delay: "<< delay  << endl;



#if defined(_WIN64)

        // ################################
        Sleep(delay);
        // ################################

#else

        // ################################
        usleep(delay*1000);
        // ################################

#endif


    } while (exitCode[5]);


}








// =====================================
// Функция для потока 017 (WriteVideo)
// =====================================
void threadFunc017(ADS::cl_DenseStereo& obj,
                   str_Settings& settings,
                   vector<int> q_vec,
                   int timeStep_Thread,
                   int countResultVideo,
                   vector<bool>& exitCode)

{

    // switch нужно потом добавить возможность записи серии снимков
    int ModeSelector =0;
    switch (ModeSelector)
    {
    case 0:
    {

        cout << "Func017: Begin  \n" ;

        // =====================================
        // Инициализация видео записи
        // =====================================

        cv::Mat Img_Result;


        Img_Result =  obj.getImgOpenCV_1left();
        cv::Size frameSize = Img_Result.size();

        //        cv::Size frameSize;
        //        frameSize.width = obj.img_X;
        //        frameSize.height = obj.img_Y;


        // The constructors/functions initialize video writers
        cv::VideoWriter video_Result;

        // Создание папки для калибровочных изображений
        // string path_Video =   "./files/Video/";
        string path_Video = settings.path_WriteResultVideo;



#if defined(_WIN64)

        // ================
        // Only Windows
        // ================

        int result01 = _mkdir(path_Video.c_str());
        if (result01==0 )
            cout << "threadFunc017: Folder WriteOneStereoPair is made \n";
        else
            cout << "threadFunc017: Don't made Folder WriteOneStereoPair \n";

// ================
// Only Windows END
// ================

#else

        // ================
        // Only Linux
        // ================

        if (mkdir(path_Video.c_str(), 0777) == -1  )
            cout << "threadFunc018: Folder WriteOneStereoPair is made \n \n";
        else
            cout << "threadFunc018: Don't made Folder WriteOneStereoPair \n";

// ================
// Only Linux END
// ================

#endif




        double timeLength_VideoFile =  settings.timeLength_VideoFile;                 // максимальная длина видео файла (сек)

        do {


            // =====================================
            // Время (начало)  для ограничения длины видео
            // =====================================
            int64 StartTick_VideoFile = cv::getTickCount();  // старт записи в файла (такты или тики)
            double CurrentTime_VideoFile;                       // текущая длина записуемого файла (сек)
            // =====================================



            // Получение текущего времени для занисения в название файла
            // QDateTime DateTime = QDateTime::currentDateTime();
            // qint64 t_ms = DateTime.toMSecsSinceEpoch();
            string FileDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss").toStdString();

            // Формирование путей для видео файлов
            string videoFilesName = "SV";
            stringstream videoFileName_Result;
            videoFileName_Result << path_Video << videoFilesName << "_Result_" << FileDateTime  << "_"  << countResultVideo << ".mp4";


            // =====================================
            // Занесение в общие данные имени и пути к файлу
            // =====================================
            settings.fileName_WriteResultVideo = videoFileName_Result.str();



            // MJ2C: Motion JPEG 2000
            //int codec = cv::VideoWriter::fourcc('M','J','2','C');
            // HFYU: Huffman Lossless Codec
            //int codec = cv::VideoWriter::fourcc('H', 'F', 'Y', 'U');

//int codec = cv::VideoWriter::fourcc('M', 'P', '4', '2');

//int codec = cv::VideoWriter::fourcc('M','J','P','G');
 //int codec = 0;

#if defined(_WIN64)
            // ================
            // Only Windows
            // ================

            int codec = cv::VideoWriter::fourcc('H', 'F', 'Y', 'U');

// ================
// Only Windows END
// ================

#else

            // ================
            // Only Linux
            // ================

            int codec = cv::VideoWriter::fourcc('M','J','P','G');


// ================
// Only Linux END
// ================

#endif



            double fps = 25;


            video_Result.open(videoFileName_Result.str(), codec, fps, frameSize,1);

            if ( !video_Result.isOpened()  )
            {
                cerr << "threadFunc017: Could not open the output video file for write\n";
            }




            do {


                // =====================================
                // Время (начало)
                // =====================================
                int64 time = cv::getTickCount();
                // =====================================



                // =====================================
                // Получение текущий  результатов
                // =====================================




               // currentIndex_ShowResultImgType





                // (Старое) Загрузка результирующего изображения
//                Img_Result =  obj.getResultImage();


                // (Новое) Загрузка результирующего изображения


                cv::Mat m;
                vector<cv::Mat> Stereo_m;


                int selectorImage= settings.currentIndex_selectorImage;

                switch ( selectorImage )
                {
                case 0:
                    m = obj.getImgOpenCV_1left();
                    break;
                case 1:
                    m = obj.getImgOpenCV_2right();
                    break;
                case 2:
                    m = obj.getImgOpenCV_3leftHand();
                    break;
                case 3:
                    m = obj.getImgOpenCV_4rightHand();
                    break;

                case 9:
                    m = obj.getImgOpenCV_DoubleStereoImg();
                    break;

                case 10:
                {
                    m = obj.getResultImage();
                    break;
                }

                case 11:
                    m = obj.getDisparityMapImage();
                    break;
                case 12:
                    m = obj.getDisparityMapImage02();
                    break;
                case 13:
                    m = obj.getDisparityMapImage03();
                    break;

                case 14:
                    Stereo_m = obj.getImgGrayPreProc_StereoPair();
                    m = Stereo_m.at(0);


                    break;

                case 15:
                    Stereo_m = obj.getImgRemap_StereoPair();
                    m = Stereo_m.at(0);

                    break;

                case 16:
                    Stereo_m = obj.getImgRemap_StereoPair();
                    m = Stereo_m.at(1);

                    break;
                case 17:
                    m = obj.getImgRemap_DoubleStereoImg();

                    break;



                case 20:
                    m = obj.getResultPrimitiveGeneralImage();
                    break;
                case 21:
                    m = obj.getResultPrimitivesXYImage();
                    break;
                case 22:
                    m = obj.getResultPrimitivesXZImage();
                    break;
                case 23:
                    m = obj.getResultPrimitivesYZImage();
                    break;

                case 30:
                    m = obj.getResultColorSearchImage();
                    break;

                case 40:
                    m = obj.getImgFindColorObj();
                    break;

                case 50:
                    m = obj.getImg3Dpoins();
                    break;


                default:
                    cout<<"Error, bad input, quitting\n";
                    break;

                }



                Img_Result = m.clone();


                // =====================================
                // Запись
                // =====================================

                // Если кадр пустой то пропуск записи
                if ( Img_Result.empty() )
                {
                    cerr << "threadFunc017: Img Result Empty \n";
                }
                else
                {

                    // encode the frame into the videofile stream
                    video_Result.write(Img_Result);

                }



                // =====================================
                // Время (Конец)
                // =====================================
                time = cv::getTickCount() - time;

                printf("\n >>>>>  Func017: Time WriteSourceVideo:      %fsec\n", time*1/cv::getTickFrequency());
                // =====================================

                //int delay = timeStep_Thread - int(time * 1000/cv::getTickFrequency() );
                int delay = 40 - int(time * 1000/cv::getTickFrequency() );
                if ( delay < 1 ) delay = 1;


                cout << " >>>>>  Func017: WriteSourceVideo: delay: "<< delay  << endl;


                // =====================================
                // Время (Конец) для ограничения длины видео
                // =====================================
                // cv::getTickFrequency()  - количество тактов в секунду
                CurrentTime_VideoFile = ( (double) (cv::getTickCount() - StartTick_VideoFile) ) / cv::getTickFrequency();

                // =====================================
                // Занесение в общие данные текущие время записи видео исходного (сек)
                // =====================================
                settings.CurrentTime_WriteResultVideo = CurrentTime_VideoFile;


#if defined(_WIN64)

                // ################################
                Sleep(delay);
                // ################################

#else

                // ################################
                usleep(delay*1000);
                // ################################

#endif




            } while (CurrentTime_VideoFile < timeLength_VideoFile && exitCode[17]); // Важно если изменять код для выхода из цикла и закрытия потока




            video_Result.release();
            countResultVideo ++;

        }  while (exitCode[17]); // от while (exitCode[18])

    }  // от  case 0:

    break;

    case 1:




        break;

    default:
        cout<<"Error, Write Selector \n";
        break;
    }



}








// =====================================
// Функция для потока 018 (WriteVideo)
// =====================================
void threadFunc018(ADS::cl_DenseStereo& obj,
                   str_Settings& settings,
                   vector<int> q_vec,
                   int timeStep_Thread,
                   int countSourceVideo,
                   vector<bool>& exitCode)

{

    // switch нужно потом добавить возможность записи серии снимков
    int ModeSelector =0;
    switch (ModeSelector)
    {
    case 0:
    {

        cout << "Func018: Begin  \n" ;

        // =====================================
        // Инициализация видео записи
        // =====================================

        cv::Mat Img_1left, Img_2right;

        Img_1left =  obj.getImgOpenCV_1left();
        cv::Size frameSize = Img_1left.size();

        //        cv::Size frameSize;
        //        frameSize.width = obj.img_X;
        //        frameSize.height = obj.img_Y;


        // The constructors/functions initialize video writers
        cv::VideoWriter video_01, video_02;

        // Создание папки для видео
      // string path_Video =   "./files/Video/";
        string path_Video = settings.path_WriteSourceVideo;


#if defined(_WIN64)

        // ================
        // Only Windows
        // ================

        int result01 = _mkdir(path_Video.c_str());
        if (result01==0 )
            cout << "threadFunc018: Folder WriteOneStereoPair is made \n";
        else
            cout << "threadFunc018: Don't made Folder WriteOneStereoPair \n";

// ================
// Only Windows END
// ================

#else

        // ================
        // Only Linux
        // ================

        if (mkdir(path_Video.c_str(), 0777) == -1  )
            cout << "threadFunc018: Folder WriteOneStereoPair is made \n \n";
        else
            cout << "threadFunc018: Don't made Folder WriteOneStereoPair \n";

// ================
// Only Linux END
// ================

#endif



        //        QTimer *timer; // Объект класса QTimer
        //        int timer_value;   // переменная для отчсчёта времени

        //        // =================================
        //        // Таймер для деления записанных видео файлов на части по N минут
        //        // =================================


        //        timer_value = 0;            // обнуление счётчика тайм
        //        timer = new QTimer(this);   // инициализация
        //        connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot_f01())); // соединение сигнала со слотом для таймера
        //        timer->setInterval(1000);  // Задаем интервал таймера

        //        timer->start();            // запуск таймера (время в милисикундах)



         double timeLength_VideoFile =  settings.timeLength_VideoFile;                 // максимальная длина видео файла (сек)

        do {


            // =====================================
            // Время (начало)  для ограничения длины видео
            // =====================================
            int64 StartTick_VideoFile = cv::getTickCount();  // старт записи в файла (такты или тики)
            double CurrentTime_VideoFile;                    // текущая длина записуемого файла (сек)
            // =====================================




            // Получение текущего времени для занисения в название файла
            //        QDateTime DateTime = QDateTime::currentDateTime();
            //        qint64 t_ms = DateTime.toMSecsSinceEpoch();
            string FileDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss").toStdString();


            // Формирование путей для видео файлов
            string videoFilesName = "SV";
            stringstream videoFileName_01, videoFileName_02;
            videoFileName_01 << path_Video << videoFilesName << "_01_left_" << FileDateTime  << "_"  << countSourceVideo << ".mp4";
            videoFileName_02 << path_Video << videoFilesName << "_02_right_" << FileDateTime  << "_"  << countSourceVideo << ".mp4";

            // =====================================
            // Занесение в общие данные имени и пути к файлу
            // =====================================
            settings.fileName_WriteSourceVideo = {videoFileName_01.str(), videoFileName_02.str()};


            // MJ2C: Motion JPEG 2000
            //int codec = cv::VideoWriter::fourcc('M','J','2','C');
            // HFYU: Huffman Lossless Codec
            //int codec = cv::VideoWriter::fourcc('H', 'F', 'Y', 'U');

            //int codec = cv::VideoWriter::fourcc('M', 'P', '4', '2');

            //int codec = cv::VideoWriter::fourcc('M','J','P','G');

            //int codec = 0;

#if defined(_WIN64)
            // ================
            // Only Windows
            // ================

        int codec = cv::VideoWriter::fourcc('H', 'F', 'Y', 'U');

// ================
// Only Windows END
// ================

#else

            // ================
            // Only Linux
            // ================

            int codec = cv::VideoWriter::fourcc('M','J','P','G');


// ================
// Only Linux END
// ================

#endif



            double fps = 25;

            //        video_01.open(videoFileName_01.str(), codec, fps, frameSize,1);
            //        video_02.open(videoFileName_02.str(), codec, fps, frameSize,1);
            video_01.open(videoFileName_01.str(), codec, fps, frameSize,1);
            video_02.open(videoFileName_02.str(), codec, fps, frameSize,1);
            if ( !video_01.isOpened() || !video_02.isOpened() )
            {
                cerr << "threadFunc018: Could not open the output video file for write\n";
            }




            do {


                // =====================================
                // Время (начало)
                // =====================================
                int64 time = cv::getTickCount();
                // =====================================


                // =====================================
                // Запись
                // =====================================


                // Загрузка исходного изображения
                vector<cv::Mat> StereoPair = obj.getImgOpenCV_StereoPair();
                Img_1left = StereoPair.at(0);
                Img_2right = StereoPair.at(1);


                // Если кадр пустой то пропуск записи
                if ( Img_1left.empty() || Img_2right.empty() )
                {
                    cerr << "threadFunc018: Video Empty \n";

                }
                else
                {

                    //            // с левой камеры стереокамеры
                    //            Img_1left =  obj.getImgOpenCV_1left();
                    //            // с правой камеры стереокамеры
                    //            Img_2right =  obj.getImgOpenCV_2right();


                    // encode the frame into the videofile stream
                    video_01.write(Img_1left);
                    video_02.write(Img_2right);

                }

                // =====================================
                // Время (Конец)
                // =====================================
                time = cv::getTickCount() - time;

                printf("\n >>>>>  Func018: Time WriteSourceVideo:      %fsec\n", time*1/cv::getTickFrequency());
                // =====================================

                //int delay = timeStep_Thread - int(time * 1000/cv::getTickFrequency() );
                int delay = 40 - int(time * 1000/cv::getTickFrequency() );
                if ( delay < 1 ) delay = 1;


                cout << " >>>>>  Func018: WriteSourceVideo: delay: "<< delay  << endl;

                // =====================================
                // Время (Конец) для ограничения длины видео
                // =====================================
                // cv::getTickFrequency()  - количество тактов в секунду
                CurrentTime_VideoFile =  (cv::getTickCount() - StartTick_VideoFile) / cv::getTickFrequency();


                // =====================================
                // Занесение в общие данные текущие время записи видео исходного (сек)
                // =====================================
                settings.CurrentTime_WriteSourceVideo = CurrentTime_VideoFile;


#if defined(_WIN64)

                // ################################
                Sleep(delay);
                // ################################

#else

                // ################################
                usleep(delay*1000);
                // ################################

#endif




            } while (CurrentTime_VideoFile < timeLength_VideoFile && exitCode[18]); // Важно если изменять код для выхода из цикла и закрытия потока



            video_01.release();
            video_02.release();
            countSourceVideo ++;


        }  while (exitCode[18]); // от while (exitCode[18])

    }  // от  case 0:

    break;

    case 1:




        break;

    default:
        cout<<"Error, Write Selector \n";
        break;
    }



}






TCPServer*  CreateTcpServerObj()
{

    TCPServer *objTcpServer_ptr = new TCPServer; // указатель оъект класса

    return objTcpServer_ptr;

}



TCPClient*  CreateTcpClientObj()
{

    TCPClient *objTcpClient_ptr = new TCPClient; // указатель оъект класса

    return objTcpClient_ptr;

}





ADS::cl_DenseStereo*  CreateObj()
{

  ADS::cl_DenseStereo *obj_ptr = new ADS::cl_DenseStereo; // указатель оъект класса

  return obj_ptr;

}

str_Settings*  Create_str_Settings()
{

    str_Settings *settings_ptr  = new str_Settings;  // указатель на структуру настроек

    return settings_ptr;

}






void  InitializeSettings(str_Settings* settings_ptr)
//void  InitializeSettings()
{




     //==============================================
    // Создание папки для конфиг файлов
    //==============================================
    string path_Config =   "./files/Config_Files/";

#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    if (_mkdir(path_Config.c_str()) == 0 )
        cout << "InitializeSettings: Folder Config_Files is made \n";
    else
        cout << "InitializeSettings: Don't made Folder Config_Files \n";

// ================
// Only Windows END
// ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(path_Config.c_str(), 0777) == -1  )
        cout << "InitializeSettings: Folder Config_Files is made \n \n";
    else
        cout << "InitializeSettings: Don't made Folder Config_Files \n";

// ================
// Only Linux END
// ================

#endif

    // Создание папки для файлов калибровки
    string path_Calibration =   "./files/Calibration_Files/";

#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    if (_mkdir(path_Calibration.c_str()) == 0 )
        cout << "InitializeSettings: Folder Calibration_Files is made \n";
    else
        cout << "InitializeSettings: Don't made Folder Calibration_Files \n";

// ================
// Only Windows END
// ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(path_Calibration.c_str(), 0777) == -1  )
        cout << "InitializeSettings: Folder Calibration_Files is made \n \n";
    else
        cout << "InitializeSettings: Don't made Folder Calibration_Files \n";

// ================
// Only Linux END
// ================

#endif




    //==============================================
    // <<<<<<<<<  ИНИЦИАЛИЗАЦИЯ НАСТРОЕК >>>>>>>> // ЕСЛИ ДО ФАЙЛА НАСТРОЕК, если он не загрузился
    //==============================================


    // =============================================
    // Путь к файлу конфигурации
    // =============================================
    settings_ptr->Config_filename = "./files/Config_Files/Config_001.xml";

    // =============================================
    // Путь к файлу конфигурации
    // =============================================
    //settings_ptr->Calibration_filename = "./files/StereoCamParams_ModuleFarVision(F0.2_D4)_Air.xml";
    settings_ptr->CurrentCalib_filename = "./files/Calibration_Files/(61a)_(1920p)_StereoModule(F1.2_D2)_AIR.xml";

    // =============================================
    // Конфигурируемые параметры
    // =============================================
    settings_ptr->amountcaptureFrames =1;          // количесво кадров получаемых с Basler камеры за один раз
    settings_ptr->amountFrames = 1;                // количество циклов ~ 120 циклов = 1 минуте.
    settings_ptr->limit_points = 1000;          // уменьшение количесва найденых точек для кластеризации
    settings_ptr->filter_ClusterPoints = 30;    // фильтр отсеивает прорисовку кластеров с количесвом точек менее чем ...
    settings_ptr->limit_WorkingArea = {-1.0e3, -1.0e3, 250, 1.0e3, 1.0e3, 1.20e3}; // Границы рабоче зоны для стереокамры СТЗ, за её пределами ничего не видит


    // Параметры DBSCAN algorithm.
    settings_ptr->rad = 30; // Connectivity radius between points, points may be connected if distance between them less then the radius.
    settings_ptr->neighbors = 0; // minimum number of shared neighbors that is required for establish links between points.

    settings_ptr->coef_base_dbs = 0.000735;
    settings_ptr->coef_pow_dbs  =   0.80;




    // =============================================
    // Имена файлов
    // =============================================
    // Для видео
    settings_ptr->video1Path = "./files/Raw01a_fps10_CamLeft.mp4";
    settings_ptr->video2Path = "./files/Raw01a_fps10_CamRight.mp4";

    // Для одиночного изображения

    settings_ptr->img1Path = "./files/L1050.bmp";
    settings_ptr->img2Path = "./files/R1050.bmp";

    // =============================================
    // Параметры записи видео файлов
    // =============================================
    settings_ptr->path_WriteResultVideo = "./files/Video/";      //  путь для записи видео результотов
    settings_ptr->path_WriteSourceVideo = "./files/Video/";      //  путь для записи видео исходного
    settings_ptr->timeLength_VideoFile = 300.0; // максимальная длина видео файла (сек)



    //======================================
    // Параметры для проведения Новой калибровки
    //=====================================
    settings_ptr->setNewCalibImg_folderName =  "(41a)_MODUL(F0.2_D4)_AIR";
    settings_ptr->setNewCalib_fileNamePath = "./files/StereoCamParams_New.xml";
    settings_ptr->setNewCalib_fileList = "./files/Im_list_all_New.txt";
    settings_ptr->usedNewCalib_fileList = "./files/Im_list_all_New.txt";

    settings_ptr->WriteSourseStereoVideoFilesName = "SourseVideo";


    settings_ptr->board_w = 9,
    settings_ptr->board_h = 6;
    settings_ptr->squareSize = 20.0;   // square size (mm)

    //==============================
    // Параметры для StereoBM
    //==============================

    settings_ptr->smbNumDisparities = 16*43;   /**< Range of disparity */ // ndisparities = 16*43
    settings_ptr->smbBlockSize = 17; /**< Size of the block window. Must be odd */ //SADWindowSize = 21


    settings_ptr->smbPreFilterCap = 15;        // 31 -> 15 +++

    settings_ptr->smbMinDisparity = 6;

    settings_ptr->smbTextureThreshold = 10;    // 10   +++
    settings_ptr->smbUniquenessRatio = 15;     // 15   +++
    settings_ptr->smbSpeckleWindowSize = 100;  // 100 +++
    settings_ptr->smbSpeckleRange = 32;        // 32
    settings_ptr->smbDisp12MaxDiff= 1;


    // Коэффициент изменения размера выводимого изображения
    settings_ptr->CoefShowWindow = 0.8;



    // Переменная для настроек прорисовки на изображении
    // 0 - резерв
    // 1 - точки:                   	0 - без, 1 - с точками
    // 2 - центр кластера:              0 - без, 1 -  с геометрическим центром, 2 -  с 2D центром
    // 3 - текс возле рамки объекта: 	0 - без, 1 - только "z", 2 - "z" и номер кластера
    // 4 - отображение кнопки:          0 - выкл., 1 - вкл. (отображается)
    // 5 - текст возле кнопки:          0 - выкл., 1 - вкл. (отображается)
    // 6 - отображение клика мыши:      0 - выкл., 1 - вкл. (отображается)
    // 7 - номера выделенного кластера возле кнопки:  0 - выкл., 1 - вкл. (отображается)

    settings_ptr->mode_AddResults = {0, 1, 1, 2, 1, 1, 1, 1, 0, 0, 0, 0};


    settings_ptr->c_maxCamerasToUse = 2;

    settings_ptr->SerialNumber_StereoCam00left = 22960744;
    settings_ptr->SerialNumber_StereoCam01right = 22960745;

    settings_ptr->SerialNumber_HandCam02left = 22960746;
    settings_ptr->SerialNumber_HandCam03right = 22984583;





    // Создание вектора для выбора камер по серийным номерам
    vector<int> SerialNumber_Cams = {settings_ptr->SerialNumber_StereoCam00left,
                                     settings_ptr->SerialNumber_StereoCam01right,
                                     settings_ptr->SerialNumber_HandCam02left,
                                     settings_ptr->SerialNumber_HandCam03right};






    // =============================
    // Переременные для работы IP с камерами симулятора CaptureFromIP()
    // ============================
    // адреса камер симулятора
    settings_ptr->videoStreamAddressLeft =      "http://127.0.0.1:800";
    settings_ptr->videoStreamAddressRight =     "http://127.0.0.1:801";
    settings_ptr->videoStreamAddressLeftHand =  "http://127.0.0.1:802";
    settings_ptr->videoStreamAddressRightHand = "http://127.0.0.1:803";

    // Создание вектора для выбора камер по серийным номерам
    vector<string> StreamAddress_IPcams = {settings_ptr->videoStreamAddressLeft,
                                           settings_ptr->videoStreamAddressRight,
                                           settings_ptr->videoStreamAddressLeftHand,
                                           settings_ptr->videoStreamAddressRightHand};






    //     =============================
    //     Матрица преобразования (в неодородной)
    //     ============================

    cv::Mat RT = cv::Mat(4, 4, CV_64F);
    vector <vector<int>> Rvec;
    Rvec.resize(4);
    Rvec[0] = {0,  0,  1, 0};
    Rvec[1] = {1,  0,  0, 0};
    Rvec[2] = {0, -1,  0, 0};
    Rvec[3] = { 0, 0,  0, 1};


    for (int i =0; i<4 ; i++)
    {
        for (int j =0; j<4 ; j++)
        {
            RT.at<double>(i, j) = Rvec[i][j];
        }
    }

    RT = settings_ptr->TransformMatrix;



    // =============================
    //  Настройки потоков
    // =============================

    //  переменная для выхода из цикла в потоках
    fill(settings_ptr->exitCode.begin(), settings_ptr->exitCode.end(), true);
    //vector<bool> exitCode (7, true);

    // Задержка запуска потоков в порядке очередности
    settings_ptr->timeDelay_startThread = 1000;
    // Время шага одной операции в потоке
    settings_ptr->timeStep_Thread = 1000/25;
    settings_ptr->maxNumThread = 20;

    fill(settings_ptr->timeMsMinDelay.begin(), settings_ptr->timeMsMinDelay.end(), settings_ptr->timeStep_Thread);




    // Инициализация размера векторов в настройках
    settings_ptr->Algorithm_inThread_seq.resize(settings_ptr->maxNumThread);
    settings_ptr->threads.resize(settings_ptr->maxNumThread);
    // Инициализация переменная для перезапуска потока в случаи отключения
    settings_ptr->idxThreadAutoRestart.resize(settings_ptr->maxNumThread);
    fill(settings_ptr->idxThreadAutoRestart.begin(), settings_ptr->idxThreadAutoRestart.end(), false);


    // =============================
    //  Переменные управления размером изображения Basler камер
    // =============================
    // Параметры для обрезки изображений
    settings_ptr->cam_CX = {1, 1};
    settings_ptr->cam_CY = {1, 1};
    settings_ptr->cam_dX = {8, 8};;
    settings_ptr->cam_dY = {8, 8};;
    settings_ptr->cam_X = {1920, 1920};
    settings_ptr->cam_Y = {1200, 1200};
    //acA1300-60gc
    //     settings_ptr->cam_X = {1280, 1280};
    //     settings_ptr->cam_Y = {1024, 1024};

    // Параметры сжатия изображения

    settings_ptr->img_X = 1920;
    settings_ptr->img_Y = 1200;

    //     //acA1300-60gc
    //     settings_ptr->img_X = 1280;
    //     settings_ptr->img_Y = 1024;

    settings_ptr->img_coeff = 1.0;

    // =============================
    //  Переменные автозапуска GUI
    // =============================
    settings_ptr->GuiWindowHide = 1;
    settings_ptr->GuiAutoStart = 1;

    settings_ptr->GuiSelectSorceImg = 0;
    settings_ptr->GuiSelectWorkMode = 0;
    settings_ptr->GuiSelecttTipeShowWindow = 0;
    settings_ptr->GuiSelectCamType = 0;


    // =============================
    //  Переменные параметров цвета для поиска по объектов (Олег К.)
    // =============================
    settings_ptr->LowHSV = {0, 130, 40};
    settings_ptr->HieghHSV = {15, 220, 220};


    // =============================================
    // Путь к файлам Аврора
    // =============================================
    settings_ptr->Aurora.Aurora_AuroraLogPath = "./files/Aurora/Log/";    // путь к log-файлу Аврора
    settings_ptr->Aurora.Aurora_AuroraFilesPath = "./files/Aurora/";       // путь сохранению файлов Аврора



    // =============================
    //  Переменные параметров цвета для поиска по объектов (Олег К.)
    // =============================




    // =============================
    //  Настройки потоков
    // =============================
    // Для РТК

    settings_ptr->set01_RTK_Algorithm_inThread_seq.resize(20);

    settings_ptr->set01_RTK_Algorithm_inThread_seq.at(0) = {-1};
    settings_ptr->set01_RTK_Algorithm_inThread_seq.at(1) = {111, 112, 11, 911, 912};
    settings_ptr->set01_RTK_Algorithm_inThread_seq.at(2) = {23, 24};
    settings_ptr->set01_RTK_Algorithm_inThread_seq.at(3) = {31};
    settings_ptr->set01_RTK_Algorithm_inThread_seq.at(4) = {41, 43};
    settings_ptr->set01_RTK_Algorithm_inThread_seq.at(5) = {51, 52, 53};
    settings_ptr->set01_RTK_Algorithm_inThread_seq.at(6) = {45};
    settings_ptr->set01_RTK_Algorithm_inThread_seq.at(7) = {-1};

    // Для Калибровки

    settings_ptr->set02_Calibration_Algorithm_inThread_seq.resize(20);

    settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(0) = {-1};
    settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(1) = {111, 112, 11, 911, 912};
    settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(2) = {121};
    settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(3) = {-1};
    settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(4) = {-1};
    settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(5) = {-1};
    settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(6) = {-1};
    settings_ptr->set02_Calibration_Algorithm_inThread_seq.at(7) = {-1};


    // Для Аврора
    settings_ptr->set03_Aurora_Algorithm_inThread_seq.resize(20);

    settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(0) = {-1};
    settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(1) = {111, 112, 11, 911, 912};
    settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(2) = {23, 24};
    settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(3) = {31};
    settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(4) = {81, 82, 83, 84};
    settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(5) = {47};
    settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(6) = {46};
    settings_ptr->set03_Aurora_Algorithm_inThread_seq.at(7) = {-1};


    // ======================
    // Задание размерности некоторых параметров типа vector<T>
    // ======================
    settings_ptr->metod_ImagePerProcessing.resize(12);



    //==============================
    // камеры HikRobot
    //==============================
    settings_ptr->CamSN = {"02K37741088",
                           "02K37741018"};   // Серийные номера устройств (камер) HikRobot по 2-ому списку
    settings_ptr->HikVision.HR_CamIndex.resize(settings_ptr->CamSN.size());     // номер камеры по 1-му списку; // Серийные номера устройств (камер) по 2-ому списку
    fill(settings_ptr->HikVision.HR_CamIndex.begin(), settings_ptr->HikVision.HR_CamIndex.end(), -1);




    //==============================
    // TCP
    //==============================
    // Адрес и порт для TCP

    settings_ptr->TCP.TCPhostIP_C = "10.32.31.36";
    //  settings_ptr->TCP.TCPhostIP_C = "192.168.56.1";
    settings_ptr->TCP.TCPhostPort_C = 10099;
    settings_ptr->TCP.TCPhostPort_S = 10098;

    settings_ptr->TCP.TCPsetTimerInterval = 1000;  // Задаем интервал таймера (мс)

    settings_ptr->SerialPort.value = 30;      // передаваемое значение
    settings_ptr->SerialPort.COMport = "COM3";   // номер компорта

// =============================================
    // =============================================
    // Чтение некоторых параметров из файла Config
    // ============================================
// =============================================
    cout<<" func_main.cpp:InitializeSettings: READ FILES ..." <<endl;


    cv::FileStorage fs(settings_ptr->Config_filename, cv::FileStorage::READ);
    if( fs.isOpened() )
    {


        double neighbors_temp;

        fs["CoefShowWindow"] >> settings_ptr->CoefShowWindow;                 // Коэффициент изменения размера выводимого изображения
        fs["amountcaptureFrames"] >> settings_ptr->amountcaptureFrames;       // количесво кадров получаемых с Basler камеры за один раз
        fs["amountFrames"] >> settings_ptr->amountFrames;                      // количество циклов ~ 120 циклов = 1 минуте.
        fs["limit_points"] >> settings_ptr->limit_points;                     // уменьшение количесва найденых точек для кластеризации
        fs["filter_ClusterPoints"] >> settings_ptr->filter_ClusterPoints;     // фильтр отсеивает прорисовку кластеров с количесвом точек менее чем ...
        fs["limit_WorkingArea"] >> settings_ptr->limit_WorkingArea;           // Границы рабоче зоны для стереокамры СТЗ, за её пределами ничего не видит
        fs["rad"] >> settings_ptr->rad;                                   // Connectivity radius between points, points may be connected if distance between them less then the radius.
        fs["neighbors"] >> neighbors_temp;                        // minimum number of shared neighbors that is required for establish links between points.

        fs["coef_base_dbs"] >> settings_ptr->coef_base_dbs; // Параметры формулы преобразования равноменого сгущения для кластеризации
        fs["coef_pow_dbs"] >> settings_ptr->coef_pow_dbs;

        settings_ptr->neighbors = neighbors_temp;

        fs["path_WriteResultVideo"] >> settings_ptr->path_WriteResultVideo;    //  путь для записи видео Резултатов
        fs["path_WriteSourceVideo"] >> settings_ptr->path_WriteSourceVideo;    //  путь для записи видео Исходного
        fs["timeLength_VideoFile"] >> settings_ptr->timeLength_VideoFile;   // максимальная длина видео файла (сек)

        fs["CurrentCalib_filename"] >> settings_ptr->CurrentCalib_filename;

        fs["setNewCalibImg_folderName"] >> settings_ptr->setNewCalibImg_folderName; // папка для сохранения калибровочных изображений
        fs["setNewCalib_fileName"] >> settings_ptr->setNewCalib_fileNamePath;  // имя файла калибровки
        fs["setNewCalib_fileList"] >> settings_ptr->setNewCalib_fileList;
        fs["usedNewCalib_fileList"] >> settings_ptr->usedNewCalib_fileList;

        fs["board_w"] >> settings_ptr->board_w;                   // ширена калибровочной доски
        fs["board_h"] >> settings_ptr->board_h;                   // высота калибровочной доски
        fs["squareSize"] >> settings_ptr->squareSize;            // размер квадрата калибровочной доски
        fs["smbNumDisparities"] >> settings_ptr->smbNumDisparities;  // Maximum disparity minus minimum disparity. This parameter must be divisible by 16.
        fs["smbBlockSize"] >> settings_ptr->smbBlockSize;             // Matched block size. It must be an odd number >=1
        fs["smbPreFilterCap"] >> settings_ptr->smbPreFilterCap;       // Truncation value for the prefiltered image pixels
        fs["smbMinDisparity"] >> settings_ptr->smbMinDisparity;       // Minimum possible disparity value
        fs["smbTextureThreshold"] >> settings_ptr->smbTextureThreshold;     //
        fs["smbUniquenessRatio"] >> settings_ptr->smbUniquenessRatio;      // Margin in percentage by which the best (minimum) computed cost function value should “win” the second best value to consider the found match correct. Normally, a value within the 5-15 range is good enough
        fs["smbSpeckleWindowSize"] >> settings_ptr->smbSpeckleWindowSize;  // Maximum size of smooth disparity regions to consider their noise speckles and invalidate
        fs["smbSpeckleRange"] >> settings_ptr->smbSpeckleRange;           // Maximum disparity variation within each connected component
        fs["smbDisp12MaxDiff"] >> settings_ptr->smbDisp12MaxDiff;         // Maximum allowed difference (in integer pixel units) in the left-right disparity check

        fs["startThread"] >> settings_ptr->timeDelay_startThread; // Задержка запуска потоков в порядке очередности
        fs["timeStep_Thread"] >> settings_ptr->timeStep_Thread; // Время шага одной операции в потоке
        fill(settings_ptr->timeMsMinDelay.begin(), settings_ptr->timeMsMinDelay.end(), settings_ptr->timeStep_Thread);

        fs["mode_AddResults"] >> settings_ptr->mode_AddResults; // Переменная для настроек прорисовки на изображении



        fs["c_maxCamerasToUse"] >> settings_ptr->c_maxCamerasToUse; // Количесво задействованных камер
        fs["SerialNumber_StereoCam00left"] >> settings_ptr->SerialNumber_StereoCam00left; // Серийный номер левой камеры стереокамеры
        fs["SerialNumber_StereoCam01right"] >> settings_ptr->SerialNumber_StereoCam01right; // Серийный номер правой камеры стереокамеры
        fs["SerialNumber_HandCam02left"] >> settings_ptr->SerialNumber_HandCam02left; // Серийный номер  камеры на левой руке манипулятора
        fs["SerialNumber_HandCam03right"] >> settings_ptr->SerialNumber_HandCam03right; // Серийный номер  камеры на првавой руке манипулятора

        fs["videoStreamAddressLeft"] >> settings_ptr->videoStreamAddressLeft;  // IP адрес IP камеры:  левой камеры стереокамеры

        string a;
        fs["videoStreamAddressLeft"] >> a;
        cout << "a" << a << endl;
        cout << "settings_ptr->videoStreamAddressLeft" << settings_ptr->videoStreamAddressLeft << endl;

        fs["videoStreamAddressRight"] >> settings_ptr->videoStreamAddressRight; // IP адрес IP камеры: правой камеры стереокамеры
        fs["videoStreamAddressLeftHand"] >> settings_ptr->videoStreamAddressLeftHand;    // IP адрес IP камеры:  камеры на левой руке манипулятора
        fs["videoStreamAddressRightHand"] >> settings_ptr->videoStreamAddressRightHand;   // IP адрес IP камеры:  камеры на првавой руке манипулятора

        fs["TransformationMatrix"] >> settings_ptr->TransformMatrix; // Матрица преобразований координат



        // =============================
        //  Переменные управления размером изображения Basler камер
        // =============================
        // Параметры для обрезки изображений
        int cam_CX_temp, cam_CY_temp;

        fs["cam_CX"] >> settings_ptr->cam_CX;
        fs["cam_CY"] >> settings_ptr->cam_CY;
        fs["cam_dX"] >> settings_ptr->cam_dX;
        fs["cam_dY"] >> settings_ptr->cam_dY;
        fs["cam_X"] >> settings_ptr->cam_X;
        fs["cam_Y"] >> settings_ptr->cam_Y;
        // Параметры сжатия изображения
        fs["img_X"] >> settings_ptr->img_X;
        fs["img_Y"] >> settings_ptr->img_Y;
        fs["img_coeff"] >> settings_ptr->img_coeff;
        // =============================
        //  Переменные автозапуска GUI
        // =============================
        fs["GuiWindowHide"] >> settings_ptr->GuiWindowHide;
        fs["GuiAutoStart"] >> settings_ptr->GuiAutoStart;

        fs["GuiSelectSorceImg"] >> settings_ptr->GuiSelectSorceImg;
        fs["GuiSelectWorkMode"] >> settings_ptr->GuiSelectWorkMode;
        fs["GuiSelecttTipeShowWindow"] >> settings_ptr->GuiSelecttTipeShowWindow;

        fs["GuiSelectCamType"] >> settings_ptr->GuiSelectCamType;

        // =============================
        //  "Аврора" Переменные параметров цвета для поиска по объектов  (Олег К.)
        // =============================
        fs["LowHSV"] >> settings_ptr->LowHSV;
        fs["HieghHSV"] >> settings_ptr->HieghHSV;

        // =============================
        // "Аврора" Переменные для TCP клиента и сервера
        // =============================

        // Адрес и порт для TCP клиента
        fs["TCPhostIP_C"] >> settings_ptr->TCP.TCPhostIP_C;     // адрес  TCP клиента
        fs["TCPhostPort_C"] >> settings_ptr->TCP.TCPhostPort_C; // порт для TCP клиента
        fs["TCPhostPort_S"] >> settings_ptr->TCP.TCPhostPort_S; // порт для TCP сервера

        fs["TCPsetTimerInterval"] >> settings_ptr->TCP.TCPsetTimerInterval; // Задаем интервал таймера (мс)



        // =============================================
        // Путь к файлам Аврора
        // =============================================
        fs["Aurora_AuroraLogPath"] >> settings_ptr->Aurora.Aurora_AuroraLogPath;        // путь к log-файлу Аврора
        fs["Aurora_AuroraFilesPath"] >> settings_ptr->Aurora.Aurora_AuroraFilesPath;    // путь сохранению файлов Аврора


        // =============================================
        // SerialPort для передачи данных на Arduino
        // =============================================

        fs["AuroraLantern_SerialPort_value"] >> settings_ptr->SerialPort.value;      // передаваемое значение
        fs["AuroraLantern_SerialPort_COMport"] >> settings_ptr->SerialPort.COMport;   // номер компорта

        // =============================
        // Серийные номера устройств (камер) HikRobot по 2-ому списку
        // =============================
        fs["CamSN"] >> settings_ptr->CamSN;


        //         minimum number of shared neighbors that is required for establish links between points.



        fs.release();

    }
    else {
        cout << "Error: func_main.cpp:InitializeSettings(str_Settings* settings_ptr):  Can not READ the extrinsics parameters\n";
        fs.releaseAndGetString();
    }




}

void  InitializeObj(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr, int num_Mode)
{

    //==============================
    // Инициалазация свойсв классов нижнего уровня
    //==============================

    // Инициализация размеров работчей зоны
    obj_ptr->setWorkingAria(settings_ptr->limit_WorkingArea);

    // Инициализация размеров окна вывода результатов
    obj_ptr->setCoefShowWindow( settings_ptr->CoefShowWindow);

    obj_ptr->setRTmatrix_cam2robot_cv(settings_ptr->TransformMatrix);



    // Инициализация  размера изображения в Basler камер
    obj_ptr->set_BaslerСamsCrop(
                                settings_ptr->cam_X,
                                settings_ptr->cam_Y,
                                settings_ptr->cam_CX,
                                settings_ptr->cam_CY,
                                settings_ptr->cam_dX,
                                settings_ptr->cam_dY);
//    // Инициализация  параметров сжатия изображения
    obj_ptr->set_ImgCompress(settings_ptr->img_X,
                             settings_ptr->img_Y,
                             settings_ptr->img_coeff);

//    // Инициализация HSV настройки поиска по цвету
    obj_ptr->setFindColor(settings_ptr->LowHSV,settings_ptr->HieghHSV);





    //==============================
    // Calibration
    //==============================
    obj_ptr->setCalibration(settings_ptr->CurrentCalib_filename);


    if ( num_Mode == 1 )
    {
        cout << "InitializeObj: Cameras Basler" << endl;

        //obj_ptr->InitializeBaslerCam();
        // obj_ptr->openBaslerCam();


        //==============================
        // Параметры выбора Basler камер
        //==============================
        // char buffer0[20], buffer1[20], buffer2[20], buffer3[20]; //результат
        // char *sn0, *sn1, *sn2, *sn3;  //указатель на результат

        QString qtsn0 = QString::number(settings_ptr->SerialNumber_StereoCam00left);
        QString qtsn1 = QString::number(settings_ptr->SerialNumber_StereoCam01right);
        QString qtsn2 = QString::number(settings_ptr->SerialNumber_HandCam02left);
        QString qtsn3 = QString::number(settings_ptr->SerialNumber_HandCam03right);



        //        sn0 = ADS::itoa2(settings_ptr->SerialNumber_StereoCam00left, buffer0, 10);
        //        sn1 = ADS::itoa2(settings_ptr->SerialNumber_StereoCam01right, buffer1, 10);
        //        sn2 = ADS::itoa2(settings_ptr->SerialNumber_HandCam02left, buffer2, 10);
        //        sn3 = ADS::itoa2(settings_ptr->SerialNumber_HandCam03right, buffer3, 10);
        //obj_ptr->set_SerialNumber_Cams({sn0, sn1, sn2, sn3});


        string sn0st = qtsn0.toStdString();
        string sn1st = qtsn1.toStdString();
        string sn2st = qtsn2.toStdString();
        string sn3st = qtsn3.toStdString();
        obj_ptr->set_SerialNumber_Cams({sn0st.c_str(),
                                        sn1st.c_str(),
                                        sn2st.c_str(),
                                        sn3st.c_str()});

        obj_ptr->set_c_maxCamerasToUse(settings_ptr->c_maxCamerasToUse);



    }
    else if ( num_Mode == 2 )
    {
        cout << "InitializeObj: IP-cams (simulator)" << endl;
        //obj.setCalibration("./files/StereoCamParams_Simulator.xml");


        obj_ptr->set_StreamAddress_IPcams( {settings_ptr->videoStreamAddressLeft,
                                           settings_ptr->videoStreamAddressRight,
                                           settings_ptr->videoStreamAddressLeftHand,
                                           settings_ptr->videoStreamAddressRightHand} );
        obj_ptr->set_c_maxCamerasToUse(settings_ptr->c_maxCamerasToUse);


    }
    else if ( num_Mode == 3 )
    {
        cout << "InitializeObj: Load from File" << endl;

    }
    else if ( num_Mode == 4 )
    {
        cout << "InitializeObj: Load from Video" << endl;


    }

    else if ( num_Mode == 5 )
    {
        cout << "InitializeObj: old Module" << endl;

        obj_ptr->setCalibration("./files/StereoCamParams_Basler_oldModule_water.xml");
    }





#if defined(_WIN64)

    // ################################
    Sleep(100);
    // ################################

#else

    // ################################
    usleep(100*1000);
    // ################################

#endif


    //  Вывод результата на выход



}





void  InitializeDrawWindows(ADS::cl_DenseStereo* obj_ptr)
{
    obj_ptr->InitializeDrawWindows();
}




// =====================================
//  Grabing Images
// ====================================

void  InitializeBaslerCam(ADS::cl_DenseStereo* obj_ptr)
{

    obj_ptr->InitializeBaslerCam();

}


void  openBaslerCam(ADS::cl_DenseStereo* obj_ptr)
{

    obj_ptr->openBaslerCam();

}



void  closeBaslerCam(ADS::cl_DenseStereo* obj_ptr)
{
    obj_ptr->closeBaslerCam();

}


void   CaptureBaslerCamImg(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{
    // Загрузка из камеры
    obj_ptr->captureContinuous(settings_ptr->amountcaptureFrames);


}

// thread 01
void  CaptureBaslerCamImg_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{
    // Поток 01 (Grabing Images)
    vector<int> q_vec = {1,0,0,  0,0,0};

    std::thread thr01(threadFunc001,
                      ref(*obj_ptr),
                      q_vec,
                      settings_ptr->timeStep_Thread,
                      settings_ptr->img1Path,
                      settings_ptr->img2Path,
                      ref(settings_ptr->exitCode));


    thr01.detach();


}

void  stopCaptureBaslerCam(ADS::cl_DenseStereo* obj_ptr)
{

    obj_ptr->stopCapture();

}

void  OpenIPcam(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{

    // Загрузка из камеры
    obj_ptr->openIPcam();

}



void  CaptureFromIP(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{

    // Загрузка из камеры
    obj_ptr->CaptureFromIP();

}





// thread 01
void  CaptureFromIP_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{
    // Поток 01 (Grabing Images)
    vector<int> q_vec = {2,0,0,  0,0,0};
    std::thread thr01(threadFunc001,
                      ref(*obj_ptr),
                      q_vec,
                      settings_ptr->timeStep_Thread,
                      settings_ptr->img1Path,
                      settings_ptr->img2Path,
                      ref(settings_ptr->exitCode));
    thr01.detach();

}





ADS::cl_DenseStereo*  СaptureFromImgFile(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{
    // Загрузка из файла
    obj_ptr->CaptureFromFile(settings_ptr->img1Path.c_str(), settings_ptr->img2Path.c_str());

    return obj_ptr;
}





ADS::cl_DenseStereo*  CaptureFromVideo(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{

//    // Переменнная для передачи ссылок, так как элемент вектора не предавался  >>> settings.exitCode.at(q_thread)
//    bool exitCode = settings_ptr->exitCode.at(1);

//    obj_ptr->CaptureFromVideo(settings_ptr->video1Path.c_str(), settings_ptr->video2Path.c_str(), &exitCode);


    return obj_ptr;
}



ADS::cl_DenseStereo*  CaptureFromVideo_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{

    // Поток 01 (Grabing Images)
    vector<int> q_vec = {3,0,0,  0,0,0};
    std::thread thr01(threadFunc001,
                      ref(*obj_ptr),
                      q_vec,
                      settings_ptr->timeStep_Thread,
                      settings_ptr->video1Path.c_str(),
                      settings_ptr->video2Path.c_str(),
                      ref(settings_ptr->exitCode));
    thr01.detach();


    return obj_ptr;
}






vector<cv::Mat>  getSorceStereoImg(ADS::cl_DenseStereo* obj_ptr )
{

    vector<cv::Mat> SourceImgOpenCV;
    SourceImgOpenCV.push_back (obj_ptr->getImgOpenCV_1left() );
    SourceImgOpenCV.push_back( obj_ptr->getImgOpenCV_2right() );

    return SourceImgOpenCV;

}


ADS::t_vuxyzrgb  getResultProcessingStereoImg(ADS::cl_DenseStereo* obj_ptr )
{
    return obj_ptr->get_vuxyzrgb();
}



// =====================================
// Получение 3D точек
// =====================================


void  Detect3Dpoints_ADS(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr )
{
    //  3D Points
    obj_ptr->setParametersSBM( settings_ptr->smbNumDisparities,
                              settings_ptr->smbBlockSize,
                              settings_ptr->smbPreFilterCap,
                              settings_ptr->smbMinDisparity,
                              settings_ptr->smbTextureThreshold,
                              settings_ptr->smbUniquenessRatio,
                              settings_ptr->smbSpeckleWindowSize,
                              settings_ptr->smbSpeckleRange,
                              settings_ptr->smbDisp12MaxDiff);

    obj_ptr->FindPointsADS(settings_ptr->limit_points, 0);

}




void  Detect3Dpoints_ADS_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{


    std::ofstream out4;          // поток для записи
    out4.open("./files/log_Detect3Dpoints_ADS_thread.txt", ios_base::out); // окрываем файл для записи
    if (out4.is_open()) out4 << "Start loging ..." << std::endl;

    try
    {

        //  3D Points

        vector<int> q_vec = {0,1,0,  0,0,0};
        std::thread thr02(threadFunc002,
                          ref(*obj_ptr),
                          q_vec,
                          ref(settings_ptr->timeStep_Thread),
                          ref(settings_ptr->limit_points),                    // уменьшение количесва найденых точек для кластеризации
                          ref(settings_ptr->smbNumDisparities),
                          ref(settings_ptr->smbBlockSize),
                          ref(settings_ptr->smbPreFilterCap),
                          ref(settings_ptr->smbMinDisparity),
                          ref(settings_ptr->smbTextureThreshold),
                          ref(settings_ptr->smbUniquenessRatio),
                          ref(settings_ptr->smbSpeckleWindowSize),
                          ref(settings_ptr->smbSpeckleRange),
                          ref(settings_ptr->smbDisp12MaxDiff),
                          ref(settings_ptr->exitCode));



        thr02.detach();


    }
    catch(const std::runtime_error& re)
    {
        // speciffic handling for runtime_error
        std::cerr << "Runtime error: " << re.what() << std::endl;

        if (out4.is_open()) out4 << "Runtime error: " << re.what() << std::endl;




    }
    catch(const std::exception& ex)
    {
        // speciffic handling for all exceptions extending std::exception, except
        // std::runtime_error which is handled explicitly
        std::cerr << "Error occurred: " << ex.what() << std::endl;


        if (out4.is_open()) out4 << "Error occurred: " << ex.what() << std::endl;


    }
    catch(...)
    {
        // catch any other errors (that we have no information about)
        std::cerr << "Unknown failure occurred. Possible memory corruption" << std::endl;

        if (out4.is_open()) out4 << "Unknown failure occurred. Possible memory corruption" << std::endl;


    }

    out4.close();

}



void  Detect3Dpoints_ASS_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{


    std::ofstream out4;          // поток для записи
    out4.open("./files/log_Detect3Dpoints_ASS_thread.txt", ios_base::out); // окрываем файл для записи
    if (out4.is_open()) out4 << "Start loging ..." << std::endl;

    try
    {

        //  3D Points

        vector<int> q_vec = {0,2,0,  0,0,0};
        std::thread thr02(threadFunc002,
                          ref(*obj_ptr),
                          q_vec,
                          ref(settings_ptr->timeStep_Thread),
                          ref(settings_ptr->limit_points),                    // уменьшение количесва найденых точек для кластеризации
                          ref(settings_ptr->smbNumDisparities),
                          ref(settings_ptr->smbBlockSize),
                          ref(settings_ptr->smbPreFilterCap),
                          ref(settings_ptr->smbMinDisparity),
                          ref(settings_ptr->smbTextureThreshold),
                          ref(settings_ptr->smbUniquenessRatio),
                          ref(settings_ptr->smbSpeckleWindowSize),
                          ref(settings_ptr->smbSpeckleRange),
                          ref(settings_ptr->smbDisp12MaxDiff),
                          ref(settings_ptr->exitCode));


        thr02.detach();


    }
    catch(const std::runtime_error& re)
    {
        // speciffic handling for runtime_error
        std::cerr << "Runtime error: " << re.what() << std::endl;

        if (out4.is_open()) out4 << "Runtime error: " << re.what() << std::endl;




    }
    catch(const std::exception& ex)
    {
        // speciffic handling for all exceptions extending std::exception, except
        // std::runtime_error which is handled explicitly
        std::cerr << "Error occurred: " << ex.what() << std::endl;


        if (out4.is_open()) out4 << "Error occurred: " << ex.what() << std::endl;


    }
    catch(...)
    {
        // catch any other errors (that we have no information about)
        std::cerr << "Unknown failure occurred. Possible memory corruption" << std::endl;

        if (out4.is_open()) out4 << "Unknown failure occurred. Possible memory corruption" << std::endl;


    }

    out4.close();

}


// =====================================
// Clasering
// =====================================

void  ClusteringDBScan(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr, int MouseClickSelector )
{

    // Clustering
    obj_ptr->clusteringDBScan(settings_ptr->rad,
                              settings_ptr->neighbors,
                              settings_ptr->filter_ClusterPoints,
                              settings_ptr->coef_base_dbs,
                              settings_ptr->coef_pow_dbs, false);

    // Обработчик событий от мыши
    obj_ptr->eventMouse_SelectObject(MouseClickSelector);

}

// thread 03
void  ClusteringDBScan_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr,
                                       int MouseClickSelector)
{
    // Clustering
    vector<int> q_vec = {0,0,1,  0,0,0};
    std::thread thr03(threadFunc003,
                      ref(*obj_ptr),
                      q_vec,
                      ref(settings_ptr->timeStep_Thread),
                      ref(settings_ptr->rad),                         // Connectivity radius between points, points may be connected if distance between them less then the radius.
                      ref(settings_ptr->neighbors),
                      ref(settings_ptr->filter_ClusterPoints),
                      ref(settings_ptr->coef_base_dbs),
                      ref(settings_ptr->coef_pow_dbs),
                      MouseClickSelector,
                      ref(settings_ptr->exitCode));
    thr03.detach();
}


// =====================================
// AddResults
// =====================================



void  AddResults(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{


    // Нанесение результатов на изображение
    obj_ptr->AddResultsToIm(settings_ptr->mode_AddResults);



}

// thread 04
void  AddResults_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{
    vector<int> q_vec = {0,0,0,  0,0,0};
    std::thread thr04(threadFunc004,
                      ref(*obj_ptr),
                      q_vec,
                      settings_ptr->timeStep_Thread,
                      settings_ptr->mode_AddResults,
                      ref(settings_ptr->exitCode));
    thr04.detach();
}




// =====================================
// Primitive
// =====================================

void  DetectAddPrimitive(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{


    // Вычисление примитива
    //obj_ptr->DetectPrimitive_all();

    // Создание изображения с примитивами
    //obj_ptr->AddPrimitiveToIm();

    // объеденненая определение и прорисовка примитивов
    obj_ptr->DetectAddPrimitive();


}

// thread 05
void  DetectAddPrimitive_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{
    vector<int> q_vec = {0,0,0,  0,0,0};
    std::thread thr05(threadFunc005,
                      ref(*obj_ptr),
                      q_vec,
                      settings_ptr->timeStep_Thread,
                      ref(settings_ptr->exitCode));

    thr05.detach();
}



// thread 017
void  WriteResultVideo_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{
    vector<int> q_vec = {0,0,0,  0,0,0};
    std::thread thr17(threadFunc017,
                      ref(*obj_ptr),
                      ref(*settings_ptr),
                      q_vec,
                      settings_ptr->timeStep_Thread,
                      ref(settings_ptr->countResultVideo),
                      ref(settings_ptr->exitCode));

    settings_ptr->countResultVideo ++;

    thr17.detach();
}

// thread 018
void  WriteSourceVideo_thread(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{
    vector<int> q_vec = {0,0,0,  0,0,0};
    std::thread thr18(threadFunc018,
                      ref(*obj_ptr),
                      ref(*settings_ptr),
                      q_vec,
                      settings_ptr->timeStep_Thread,
                      ref(settings_ptr->countSourceVideo),
                      ref(settings_ptr->exitCode));

    settings_ptr->countSourceVideo ++;

    thr18.detach();
}


// =====================================
// ShowResults
// =====================================

void  getResultsIm(ADS::cl_DenseStereo* obj_ptr, uchar** data, int* cols, int* rows, int* channels)
{


    cv::Mat m = obj_ptr->getResultImage();
    // uint size = m.total() * m.elemSize();
    // uchar* imageDataCopy = new uchar[size];
    uint size = 1920*1200*3;
    memcpy(imageDataCopy, m.data, size);
    memcpy(imageDataCopy, m.data, size);

    *cols = m.cols;
    *rows = m.rows;
    *channels = m.channels();
    *data = imageDataCopy;




}



void  setMouseClickCoordinates(ADS::cl_DenseStereo* obj_ptr, int u, int v)
{

    obj_ptr->setMouseClickCoordinates02(u,v);

}


void  ShowResults(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr, int idx, int selectorImage)
//void  ShowResults()
{

    //  Вывод изображений на экран
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

    // 10 - Изображение с результатами - левая камера стереокамеры: imgResult01r;
    // 11 - Изображение карты глубин в цвете: imgResultColorSearch

    // 20 - ОБЩЕЕ изображение с результатами построения примитивов объектов: imgPrimitive01r_general;
    // 21 - изображение отдельной проекции примитивов: imgPrimitivesXY;
    // 22 - изображение отдельной проекции примитивов: imgPrimitivesYZ;
    // 23 - изображение отдельной проекции примитивов: imgPrimitivesXZ;

    // 30 - Изображение результатов поиска по цвету





    // =====================================
    // Время (начало)
    // =====================================
    int64 time = cv::getTickCount();
    // =====================================


    if (idx == 1)
    {



        // ################################
        int key = cv::waitKeyEx(5);
        // ################################

        if (key == 7405568) //  - это номер клавиши F2
        {
            if (DrawSelector == 0)
            {
                DrawSelector = 1;

            }
            else if (DrawSelector == 1)
            {

                DrawSelector = 0;

            }
        }
        else if (key == 7471104) //  - это номер клавиши F3
        {
            if (DrawSelector == 0)
            {
                DrawSelector = 2;
            }
            else if (DrawSelector == 2)
            {
                DrawSelector = 0;
            }
        }
        else if (key == 7536640) //  - это номер клавиши F4
        {
            if (DrawSelector == 0)
            {
                DrawSelector = 3;
            }
            else if (DrawSelector == 3)
            {
                DrawSelector = 0;
            }
        }
        else if (key == 7602176) //  - это номер клавиши F5
        {
            if (DrawSelector == 0)
            {
                DrawSelector = 4;
            }
            else if (DrawSelector == 4)
            {
                DrawSelector = 0;
            }
        }


        //  F2 - 7405568
        //  F3 - 7471104
        //  F4 - 7536640

        //  F5 - 7602176

        //  F6 - 7667712
        //  F7 - 7733248
        //  F8 - 7798784





        //        cout << "|||||||||||||||||||||||| waitKey: key = "<< key <<" |||||||||||||||||||" << endl;
        //        cout << "|||||||||||||||||||||||| waitKey: DrawSelector = "<< DrawSelector <<" |||||||||||||||||||" << endl;


        switch (DrawSelector)
        {
        case 0:
            // Вывод результата в окно OpenCV
            obj_ptr->DrawResultsInImage_AlLIm(selectorImage);
            break;

        case 1:
            // Вывод результата с примитивами
            obj_ptr->DrawPrimitive();
            break;

        case 2:
            // Вывод карты диспаратности
           // obj_ptr->AddResultsToDisparityMap02();
            obj_ptr->DrawDisparityMap02();
            break;
        case 3:
            // Вывод результата
           // obj_ptr->AddResultsToDisparityMap03();
            obj_ptr->DrawDisparityMap03();

            break;
        case 4:

            // Вывод результата поиск по цвету
            obj_ptr->DrawSearchColor();


            break;


        }


    }



    // =====================================
    // Время (Конец)
    // =====================================
    time = cv::getTickCount() - time;
    printf("\n <<< >>> Main000: Time ShowResults:      %fsec\n", time*1/cv::getTickFrequency());
    // =====================================


    int delay = settings_ptr->timeStep_Thread - (time * 1000/cv::getTickFrequency() );
    if ( delay < 1 ) delay = 1;



    cout << " <<< >>> Main000:ShowResults: delay: "<< delay  << endl;


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









void  ShowResults_cycle(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr, vector<bool> exitCode)

{



    cout << " >>>> ShowResults_cycle: Begin  \n";

    do {
        // =====================================
        // Время (начало)
        // =====================================
        int64 time = cv::getTickCount();
        // =====================================


        // Вывод результата в окно OpenCV
        obj_ptr->DrawResultsInImage();


        // =====================================
        // Время (Конец)
        // =====================================
        time = cv::getTickCount() - time;
        printf("\n <<< >>> Main000b: Time ShowResults_cycle:      %fsec\n", time*1/cv::getTickFrequency());
        // =====================================


        int delay = settings_ptr->timeStep_Thread - (time * 1000/cv::getTickFrequency() );
        if ( delay < 1 ) delay = 1;


#if defined(_WIN64)

        // ################################
        Sleep(delay);
        // ################################

#else

        // ################################
        usleep(delay*1000);
        // ################################

#endif



        cout << " ==>> (0b) ShowResults_cycle: delay: "<< delay  << endl;


    }while(exitCode[0]);


}




void  saveFileResult(ADS::cl_DenseStereo* obj_ptr)
{
    // Сохраниение Результата (в файл)
    obj_ptr->saveFileResult();
}
//=================================================
//=================================================


ADS::cl_DenseStereo*  copy_obj(ADS::cl_DenseStereo* obj_ptr)
{

    return obj_ptr;
}




ADS::t_vuxyzrgb*  Create_copy_vuxyzrgb()
{

    // Копии
    // указатель на переменную числовых результатов
    ADS::t_vuxyzrgb *vuxyzrgb_copy = new ADS::t_vuxyzrgb;


    return vuxyzrgb_copy;
}






void  copy_vuxyzrgb(ADS::cl_DenseStereo* obj_ptr, ADS::t_vuxyzrgb* vuxyzrgb_copy)
{


    *vuxyzrgb_copy = obj_ptr->get_vuxyzrgb();


    // cout << "\t\t --->>> (1) copy_vuxyzrgb: obj_ptr->getResultVector().SelectedClaster: "<< obj_ptr->getResultVector().SelectedClaster  << endl;
    // cout << "\t\t --->>> (2) copy_vuxyzrgb: vuxyzrgb_copy->SelectedClaster: "<< vuxyzrgb_copy->SelectedClaster  << endl;

}







ADS::type_arma_vuxyzrgb*  copy_vuxyzrgb_arma(ADS::cl_DenseStereo* obj_ptr)
{

    // Копии
    // указатель на переменную числовых результатов
    ADS::type_arma_vuxyzrgb *vuxyzrgb_arma_copy = new ADS::type_arma_vuxyzrgb;

    *vuxyzrgb_arma_copy = obj_ptr->get_vuxyzrgb_arma();
    return vuxyzrgb_arma_copy;
}



int  get_num_cl(ADS::t_vuxyzrgb *vuxyzrgb_copy)
{

    int num_cl = vuxyzrgb_copy->num_cl;

    return num_cl;
}



int  get_num_prim(ADS::t_vuxyzrgb *vuxyzrgb_copy)
{

    int num_cl = vuxyzrgb_copy->num_prim;

    return num_cl;
}





int  get_SelectedClaster(ADS::t_vuxyzrgb *vuxyzrgb_copy)
{
    int idxSelectedClaster = vuxyzrgb_copy->SelectedClaster;


    // Костыль. Для того чтобы индекс выделенного гластера не был больше количесва кластеров
    int num_cl = vuxyzrgb_copy->num_cl;

    if (idxSelectedClaster > num_cl) idxSelectedClaster = num_cl;


//    cout << "\t\t --->>> (3) vuxyzrgb_copy->SelectedClaster: "<< vuxyzrgb_copy->SelectedClaster  << endl;

    return idxSelectedClaster;

}

int  get_num_p(ADS::t_vuxyzrgb *vuxyzrgb_copy)
{
    int idxSelectedClaster = vuxyzrgb_copy->num_p;

//    cout << "\t\t --->>> (3) vuxyzrgb_copy->SelectedClaster: "<< vuxyzrgb_copy->SelectedClaster  << endl;

    return idxSelectedClaster;

}







int  get_objRect2D(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idxTypeData, int numClaster)
{

    // Фунция для передачи данных о 2D координатах найденых кластеров
    // numClaster - номер кластера
    // idxTypeData - тип данных :
    // 0 - u_min
    // 1 - v_min

    // 2 - u_max
    // 3 - v_max

    // 4 - u_center
    // 5 - v_center

    // 6 - radius2D


    int result = 0;

    switch (idxTypeData)
    {

    case 0:
        // u_min;
        result = vuxyzrgb_copy->objRect2D[numClaster][0];
        break;
    case 1:
        // v_min
        result = vuxyzrgb_copy->objRect2D[numClaster][1];
        break;
    case 2:
        // u_max;
        result = vuxyzrgb_copy->objRect2D[numClaster][2];
        break;
    case 3:
        //v_max;
        result = vuxyzrgb_copy->objRect2D[numClaster][3];
        break;
    case 4:
        //u_center;
        result = vuxyzrgb_copy->objRect2D[numClaster][4];
        break;
    case 5:
        //v_center;
        result = vuxyzrgb_copy->objRect2D[numClaster][5];
        break;
    case 6:
        //radius2D;
        result = vuxyzrgb_copy->objRect2D[numClaster][6];
        break;

    }

    return result;

}

double  get_objRect3D(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idxTypeData, int numClaster)
{

    // Фунция для передачи данных о 3D координатах найденых кластеров
    // numClaster - номер кластера
    // idxTypeData - тип данных :
    // 0 - x_min
    // 1 - y_min
    // 2 - z_min

    // 3 - x_max
    // 4 - y_max
    // 5 - z_max

    // 6 - x_center
    // 7 - y_center
    // 8 - z_center

    // 9 - radius3D

    double result = 0;
    switch (idxTypeData)
    {

    case 0:
        // x_min;
        result = vuxyzrgb_copy->objRect3D[numClaster][0];
        break;
    case 1:
        // y_min
        result = vuxyzrgb_copy->objRect3D[numClaster][1];
        break;
    case 2:
        // z_min;
        result = vuxyzrgb_copy->objRect3D[numClaster][2];
        break;

    case 3:
        // x_max;
        result = vuxyzrgb_copy->objRect3D[numClaster][3];
        break;
    case 4:
        // y_max;
        result = vuxyzrgb_copy->objRect3D[numClaster][4];
        break;
    case 5:
        // z_max;
        result = vuxyzrgb_copy->objRect3D[numClaster][5];
        break;

    case 6:
        // x_center;
        result = vuxyzrgb_copy->objRect3D[numClaster][6];
        break;
    case 7:
        // y_center;
        result = vuxyzrgb_copy->objRect3D[numClaster][7];
        break;
    case 8:
        // z_center;
        result = vuxyzrgb_copy->objRect3D[numClaster][8];
        break;

    case 9:
        // radius3D;
        result = vuxyzrgb_copy->objRect3D[numClaster][9];
        break;

    }

    return result;

}




double  get_distance2Point3D(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idxTypeData)
{

    // Фунция для передачи данных о 3D координатах найденых кластеров
    // numClaster - номер кластера
    // idxTypeData - тип данных :
    // 0 - x_1
    // 1 - y_1
    // 2 - z_1

    // 3 - x_2
    // 4 - y_2
    // 5 - z_2

    // 6 - distance

    double result = vuxyzrgb_copy->distance2Point3D.at(idxTypeData);

    return result;

}




double  get_objRect3D_robot(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idxTypeData, int numClaster)
{

    // Фунция для передачи данных о 3D координатах найденых кластеров
    // numClaster - номер кластера
    // idxTypeData - тип данных :
    // 0 - x_min
    // 1 - y_min
    // 2 - z_min

    // 3 - x_max
    // 4 - y_max
    // 5 - z_max

    // 6 - x_center
    // 7 - y_center
    // 8 - z_center

    // 9 - radius3D

    double result = 0;
    switch (idxTypeData)
    {

    case 0:
        // x_min;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][0];
        break;
    case 1:
        // y_min
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][1];
        break;
    case 2:
        // z_min;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][2];
        break;

    case 3:
        // x_max;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][3];
        break;
    case 4:
        // y_max;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][4];
        break;
    case 5:
        // z_max;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][5];
        break;

    case 6:
        // x_center;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][6];
        break;
    case 7:
        // y_center;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][7];
        break;
    case 8:
        // z_center;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][8];
        break;

    case 9:
        // radius3D;
        result = vuxyzrgb_copy->objRect3D_robot_vec[numClaster][9];
        break;

    }

    return result;

}

double  get_x_center(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idx)
{
    double x_center;

    //    cout << "\t\t >>-------->>>> (3) get_x_center: x_center: "<< x_center  << endl;
//    cout << "\t\t >>-------->>>> (3) get_x_center: vuxyzrgb_copy->objRect3D.size(): "<< vuxyzrgb_copy->objRect3D.size()  << endl;
//    cout << "\t\t >>-------->>>> (3) get_x_center: idx: "<< idx  << endl;



//    if (idx > vuxyzrgb_copy->num_cl) idx = vuxyzrgb_copy->num_cl;
//    if (idx > int(vuxyzrgb_copy->objRect3D.size()-1 ))  idx = int (vuxyzrgb_copy->objRect3D.size()-1); // исправило вылеты, которые долго искали

    if (idx >= 0)
    {
//        x_center = vuxyzrgb_copy->objRect3D[idx][6];
        x_center = vuxyzrgb_copy->objRect3D.at(idx).at(6);
         // x_center = 0;

    }

    else  x_center = 0;



    return x_center;
}


double  get_y_center(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idx)
{

    double y_center;

    if (idx >= 0)
    {
        y_center = vuxyzrgb_copy->objRect3D[idx][7];
//        y_center = 0;
    }
    else  y_center = 0;

    return y_center;
}

double  get_z_center(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idx)
{

    double z_center;

    if (idx >= 0)
    {
        z_center = vuxyzrgb_copy->objRect3D[idx][8];
//        z_center = 0;
    }
    else  z_center = 0;

    return z_center;
}

double  get_radius(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idx)
{



    double radius3D;

    if (idx >= 0)
    {
        radius3D = vuxyzrgb_copy->objRect3D[idx][9];
//        radius3D = 0;
    }
    else  radius3D = 0;


    return radius3D;
}





int  get_typeDetectPrimitive(ADS::t_vuxyzrgb *vuxyzrgb_copy, int numClaster )
{
    int PrimitiveDetect = vuxyzrgb_copy->PrimitiveDetect[numClaster];

    return PrimitiveDetect;

}





double  get_PrimitiveParameter(ADS::t_vuxyzrgb *vuxyzrgb_copy, int idxTypeData, int numClaster)
{

    // Фунция для передачи данных о приметиве кластер
    // numClaster - номер кластера
    // idxTypeData - тип данных :
    // Plane:
    // z = ax + by + c: [a, b, c, err]
    // 0 - a
    // 1 - b
    // 2 - c
    // 3 - err


    //Sphere:
    //[xc, yc, zc, radius, err]
    // 0 - xc
    // 1 - yc
    // 2 - zc
    // 3 - radius
    // 4 - err


    // Cylinder:
    // 0 - xc1
    // 1 - yc1
    // 2 - zc1

    // 3 - xc2
    // 4 - yc2
    // 5 - zc2

    // 6 - radius
    // 7 - err

    int result = 0;
    switch (idxTypeData)
    {

    case 0:
        // ;
        result = vuxyzrgb_copy->PrimitiveParameter[numClaster][0];
        break;
    case 1:
        //
        result = vuxyzrgb_copy->PrimitiveParameter[numClaster][1];
        break;
    case 2:
        // ;
        result = vuxyzrgb_copy->PrimitiveParameter[numClaster][2];
        break;

    case 3:
        // ;
        result = vuxyzrgb_copy->PrimitiveParameter[numClaster][3];
        break;
    case 4:
        // ;
        result = vuxyzrgb_copy->PrimitiveParameter[numClaster][4];
        break;
    case 5:
        // ;
        result = vuxyzrgb_copy->PrimitiveParameter[numClaster][5];
        break;

    case 6:
        // ;
        result = vuxyzrgb_copy->PrimitiveParameter[numClaster][6];
        break;
    case 7:
        // ;
        result = vuxyzrgb_copy->PrimitiveParameter[numClaster][7];
        break;


    }

    return result;

}












void  get_vuxyzrgb_arma(ADS::type_arma_vuxyzrgb *vuxyzrgb_arma_copy)
{
}



void   write2File_vuxyzrgb(ADS::cl_DenseStereo* obj_ptr)
{


    ADS::t_vuxyzrgb vuxyzrgb_copy2 = obj_ptr->get_vuxyzrgb();

    // =============================
    // Save result vuxyzrgb
    //==============================

    std::ofstream out3;          // поток для записи
    out3.open("./files/finish_resultsS22222.txt", ios_base::out); // окрываем файл для записи

    if (out3.is_open())

        for (int q = 0; q <  vuxyzrgb_copy2.num_p; q++)
        {

            out3 << vuxyzrgb_copy2.vu.at(q)[0] << " \t " << vuxyzrgb_copy2.vu.at(q)[1] << " \t "
                 << vuxyzrgb_copy2.xyz.at(q)[0] << " \t " << vuxyzrgb_copy2.xyz.at(q)[1] << " \t " << vuxyzrgb_copy2.xyz.at(q)[2] << " \t "
                 << vuxyzrgb_copy2.rgb.at(q)[0] << " \t " << vuxyzrgb_copy2.rgb.at(q)[1] << " \t " << vuxyzrgb_copy2.rgb.at(q)[2] << " \t "
                 << vuxyzrgb_copy2.cluster.at(q)<< endl;

        }


    out3.close();


}




void   write2File_vuxyzrgb_arma(ADS::cl_DenseStereo* obj_ptr)
{
    ADS::type_arma_vuxyzrgb vuxyzrgb_arma_copy2 = obj_ptr->get_vuxyzrgb_arma();

    vuxyzrgb_arma_copy2.clusters_indx.save("./files/clusters_ind_arma22222.txt", arma::csv_ascii);

}








void  getImg(ADS::cl_DenseStereo* obj_ptr, int selectorImage, uchar** data, int* cols, int* rows, int* channels)
{
    // Получение изображения
    // первый входной аргумент фунции - это номер камеры:

    // 0 - StereoCam00left	 - левая камеры стереокамеры
    // 1 - StereoCam01right	 - правая камеры стереокамеры
    // 2 - HandCam02left 	 - камерана левой руке манипулятора
    // 3 - HandCam03right	 - камера на првавой руке манипулятора

    // 10 - Изображение с результатами - левая камера стереокамеры: imgResult01r;
    // 11 - Изображение катры глубин в цвете: imgResultColorSearch

    // 20 - ОБЩЕЕ изображение с результатами построения примитивов объетков: imgPrimitive01r_general;
    // 21 - изображение отдельной проекции примитивов: imgPrimitivesXY;
    // 22 - изображение отдельной проекции примитивов: imgPrimitivesYZ;
    // 23 - изображение отдельной проекции примитивов: imgPrimitivesXZ;

    // 30 - Изображение результатов поиска по цвету




    cv::Mat m;
    switch ( selectorImage )
    {
    case 0:
        m = obj_ptr->getImgOpenCV_1left();
        break;
    case 1:
        m = obj_ptr->getImgOpenCV_2right();
        break;
    case 2:
        m = obj_ptr->getImgOpenCV_3leftHand();
        break;
    case 3:
        m = obj_ptr->getImgOpenCV_4rightHand();
        break;


    case 10:
        m = obj_ptr->getResultImage();
        break;
    case 11:
        m = obj_ptr->getDisparityMapImage();
        break;

    case 20:
        m = obj_ptr->getResultPrimitiveGeneralImage();
        break;
    case 21:
        m = obj_ptr->getResultPrimitivesXYImage();
        break;
    case 22:
        m = obj_ptr->getResultPrimitivesXZImage();
        break;
    case 23:
        m = obj_ptr->getResultPrimitivesYZImage();
        break;

    case 30:
        m = obj_ptr->getResultColorSearchImage();
        break;


    default:
        cout<<"Error, bad input, quitting\n";
        break;

    }

    // uint size = m.total() * m.elemSize();
    // uchar* imageDataCopy = new uchar[size];
    uint size = 1920*1200*3;
    memcpy(imageDataCopy, m.data, size);

    *cols = m.cols;
    *rows = m.rows;
    *channels = m.channels();
    *data = imageDataCopy;




}









void   Aurora_FindColorObj(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{




    vector<int> q_vec = {1,3,1,  0,0,0};
    std::thread thr02(threadFunc002,
                      ref(*obj_ptr),
                      q_vec,
                      ref(settings_ptr->timeStep_Thread),
                      ref(settings_ptr->limit_points),                    // уменьшение количесва найденых точек для кластеризации
                      ref(settings_ptr->smbNumDisparities),
                      ref(settings_ptr->smbBlockSize),
                      ref(settings_ptr->smbPreFilterCap),
                      ref(settings_ptr->smbMinDisparity),
                      ref(settings_ptr->smbTextureThreshold),
                      ref(settings_ptr->smbUniquenessRatio),
                      ref(settings_ptr->smbSpeckleWindowSize),
                      ref(settings_ptr->smbSpeckleRange),
                      ref(settings_ptr->smbDisp12MaxDiff),
                      ref(settings_ptr->exitCode));

    thr02.detach();




}





void   CalibrateStereoCam(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{

    obj_ptr->getCalibrateImages( settings_ptr->setNewCalib_fileList.c_str(),
                                settings_ptr->setNewCalibImg_folderName.c_str(),
                                &(settings_ptr->exitCode) );

    if (settings_ptr->exitCode.at(0))
    {
        obj_ptr->CalibrateStereoCam(settings_ptr->setNewCalib_fileNamePath.c_str(),
                                    settings_ptr->usedNewCalib_fileList.c_str(),
                                    settings_ptr->board_w,
                                    settings_ptr->board_h,
                                    settings_ptr->squareSize);
    }
}





void WriteOneStereoPair(ADS::cl_DenseStereo* obj_ptr, str_Settings *settings_ptr)
{




    // Создание папки для калибровочных изображений
    string path_ScreenShots =   "./files/ScreenShots";

#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    int result01 = _mkdir(path_ScreenShots.c_str());
    if (result01==0 )
        cout << "WriteOneStereoPair: Folder WriteOneStereoPair is made \n";
    else
        cout << "WriteOneStereoPair: Don't made Folder WriteOneStereoPair \n";

        // ================
        // Only Windows END
        // ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(path_ScreenShots.c_str(), 0777) == -1  )
        cout << "WriteOneStereoPair: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "WriteOneStereoPair: Don't made Folder WriteOneStereoPair \n";

        // ================
        // Only Linux END
        // ================

#endif





    int countImg = settings_ptr->countSourceImg;

    stringstream ss01, ss02;
    ss01 << path_ScreenShots << "/L" << countImg << ".bmp";
    ss02 << path_ScreenShots << "/R" << countImg << ".bmp";



    // Загрузка исходного изображения
    vector<cv::Mat> StereoPair = obj_ptr->getImgOpenCV_StereoPair();
    cv::Mat image01 = StereoPair.at(0);
    cv::Mat image02 = StereoPair.at(1);



    cv::imwrite(ss01.str(), image01);
    cv::imwrite(ss02.str(), image02);

    settings_ptr->countSourceImg ++;


}













