#ifndef FUNC_MAIN_H
#define FUNC_MAIN_H

#include "cl_DenseStereo.h"




// Подключение классов (можно не указыать так как уже есть в <func_DenseStereo.h>
//#include <QTime>
//#include <QDateTime>
 #include <QTcpServer>



// Namespace for using pylon objects.
using namespace Pylon;
using namespace Basler_UniversalCameraParams;
// Namespace for using cout.
using namespace std;

// ********************************
// Если вы хотите объявить и реализовать подкласс QObject в файле cpp, вам необходимо вручную включить файл moc.
//#include "func_main.moc"
struct SubObject : QObject
{
    Q_OBJECT
};
// ********************************



// =============================================
// Вложенные структуры
// =============================================
struct Objs_ptr     // Для хранения ссылок на объекты класса
{
    ADS::cl_DenseStereo* obj_ptr;
    ADS::t_vuxyzrgb* vuxyzrgb_copy;
    TCPServer* objTcpServer_ptr;
    TCPClient* objTcpClient_ptr;
    cl_VideoStreamer* objp_VideoStreamer;  // ссылка на объект класса для видео стрима
    ADS::NeuralNetDetector * DarkNetDetector;   // ссылка на нейронной сети для распознания объектов на изображении


};

// =============================================
// Вложенные структуры Aurora
// =============================================

struct SerialPort     // SerialPort для передачи данных на Arduino
{
    int value = 20;             // передаваемое значение
    std::string COMport = "COM4";   // номер компорта

};
struct InitialStateInformation    // информация о начальном состоянии (только в начале работы)
{
    int SecondaryPowerSupply;               // Наличие вторичного электропитания отсека СТЗ
    int ConnectToCams;                      // Наличие соединения с камерами
    int ImageAquisition;                    // Получение изображения камерами
    int ReadyForOperationCompartmentLamp;   // Готовность к работе регулятора яркости светильника отсека
    int FuctSuccessfulOperation;            // факт успешной работы
};

struct StatusStateInformation    // информация о состоянии (в текущий момент времени)
{
    int StatusSuccessfulOperation;         // информация о состоянии
};


struct DetectObjSize    // Габариты объекта
{

    //=================================================
    // Габариты объекта
    //=================================================
    // Вывод габаритов параллелепипеда, где (L-длина W-ширина H-высота параллелепипид и габариты объекты: Length-высота, Width-ширина)
    double circumscribedLength = -1;
    double circumscribedWidth = -1;
    double circumscribedHeight = -1;
    double ObjLength = -1;
    double ObjWidth = -1;      //

};



struct MomentOjectDetection    // факт обнаружения объкта (в момент обнаружения)
{
    int FuctOjectDetection;                            // факт обнаружения объекта
    double Time;                                       // время обнаружения объекта с фиксацией на кадре
    vector<float> GeographicalCoordinates {0, 0};      // географические координаты объекта с фиксацией на кадре
    float ObjectDistance;                              // дальность до обнаруженного объекта
    vector<float> LinearCoordinates {0, 0, 0};         // линейные координаты обнаруженного объекта в системе координат СТЗ
    int ContouringCompletion;                          // отчёт о завершении оконтуривания и готовности передать контур объекта в БПУ АНПА
    Mat SendingImg;                                    // кадр в сжатом виде и контур обнаруженного объекта, дальность до него и линейные координаты (один объект – один кадр)

    struct DetectObjSize DetectObjSize;  // Габариты объекта
};

struct OutsideInformationANPA    // информация из вне (от Авроры)


{
    vector<double> GeographicCoordinates {0, 0}; // Текущие географические координаты АНПА (широта, долгота)
    double HardwareTime;                         // Аппаратное время*
    float FloorDistance;                         // Отстояние от дна
    float RollANPA;                              // Крен АНПА
    float TrimANPA;                              // Дифферент
    vector<float> AngularVelocity {0, 0, 0};         // Угловая скорость АНПА
    vector<float> ApparentAcceleration {0, 0, 0};    // Кажущееся ускорение АНПА
    float AbsoluteVelocity;                          //Абсолютная скорость

};



struct Aurora     // Для Авроры флаги (коды) работы
{

    struct InitialStateInformation InitialStateInformation;  // информация о начальном состоянии (только в начале работы)
    struct StatusStateInformation StatusStateInformation;    // информация о состоянии (в текущий момент времени)
    struct MomentOjectDetection MomentOjectDetection;        // факт обнаружения объкта (в момент обнаружения)
    struct OutsideInformationANPA OutsideInformationANPA;    // информация из вне (от Авроры)

    // =============================================
    // Путь к файлам Аврора
    // =============================================
    string Aurora_AuroraLogPath;        // путь к log-файлу Аврора
    string Aurora_AuroraFilesPath;   // путь сохранению файлов Аврора





};


struct TCP     //
{
    string TCPhostIP_C;
    int TCPhostPort_C;
    int TCPhostPort_S;

    int TCPsetTimerInterval = 1000;  // Задаем интервал таймера (мс)

    bool flag_StartProgram = false; // флаг указывает работает ли программа (или не работает)
};



struct HikVision    // HikRobot (HikVision)
{

    vector <unsigned int> HR_CamIndex;     // номер камеры по 1-му списку; // Серийные номера устройств (камер) по 2-ому списку
   // std::vector<cv::Mat> HR_Imgs;


};

struct Rotation3Dpoints    // Rotation3Dpoints
{

    double angx = 0; // угол поворота вокруг оси x центра вращения
    double angy = 0; // угол поворота вокруг оси y центра вращения
    double angz = 0; // угол поворота вокруг оси z центра вращения
    double tx = 0;   // смещение вдоль оси x центра вращения
    double ty = 0;   // смещение вдоль оси y центра вращения
    double tz = 0;   // смещение вдоль оси z центра вращения
    double Scale = 100; // Масштаб
    double Dz = -3000;    // смещение центра вращения от камер вдоль оси z


};

struct VideoStreamer    // Rotation3Dpoints
{

    int StreamQualiy = 50;
    int SreamPort = 8080;
    std::string StreamPublishPath = "/result";

};

struct DarkNet    // DarkNet
{

    bool reply = false;
    std::string replyAll = "-";
    std::string model_path = "./files/nn/yolov5.onnx";
    std::string classes_path = "./files/nn/yolov5.names";

};


struct MissActions    //
{
    bool MissAutorization = false;
    bool MissQuetions = false;
    bool MissWarnings = false;
    bool MissCriticals = false;
};



// =============================================
// Основная структура
// =============================================
struct str_Settings     // Структура в формате std:vector
{
    struct Objs_ptr Objs_ptr;     // Для хранения ссылок на объекты класса

    struct SerialPort SerialPort;  // для передачи данных на Arduino

    struct Aurora Aurora;    // Для Авроры флаги (коды) работы

    struct TCP TCP;  //

    struct HikVision HikVision;  // HikRobot (HikVision)

    struct Rotation3Dpoints Rotation3Dpoints;  // HikRobot (HikVision)

    struct VideoStreamer VideoStreamer;  // HikRobot (HikVision)

    struct DarkNet DarkNet;  // Кирилл - нейронная сеть

    struct  MissActions MissActions;  // всплывающие окна

    // =============================================
    // Путь к файлу конфигурации
    // =============================================
    string Config_filename;
    string CurrentCalib_filename;

    // =============================================
    // Конфигурируемые параметры

    // =============================================
    int amountcaptureFrames;          // количесво кадров получаемых с Basler камеры за один раз
    int amountFrames;                // количество циклов ~ 120 циклов = 1 минуте.
    int limit_points;               // уменьшение количесва найденых точек для кластеризации
    int filter_ClusterPoints;       // фильтр отсеивает прорисовку кластеров с количесвом точек менее чем ...
    vector<double> limit_WorkingArea; // Границы рабоче зоны для стереокамры СТЗ, за её пределами ничего не видит


    // Параметры DBSCAN algorithm.
    double rad; // Connectivity radius between points, points may be connected if distance between them less then the radius.
    size_t neighbors; // minimum number of shared neighbors that is required for establish links between points.

    // Параметры формулы преобразования равноменого сгущения для кластеризации
    double coef_base_dbs;
    double coef_pow_dbs;


    // =============================================
    // Имена файлов
    // =============================================
    // Для видео
    string video1Path;
    string video2Path;

    // Для одиночного изображения

    string img1Path;
    string img2Path;

    // =============================================
    // Параметры записи видео файлов
    // =============================================
    string path_WriteResultVideo;      // путь для записи видео результатов
    string path_WriteSourceVideo;      // путь для записи видео исходного
    double timeLength_VideoFile;      // максимальная длина видео файла (сек)


    string fileName_WriteResultVideo = "-";      // имя файла записи видео результатов
    vector<string> fileName_WriteSourceVideo = {"-", "-"};      // имя файла записи видео исходного

    double CurrentTime_WriteResultVideo = 0;      // текущие время записи видео результатов (сек)
    double CurrentTime_WriteSourceVideo = 0;      // текущие время записи видео исходного (сек)

    // =============================
    // переменная для потока 06 (запись данных)
    // =============================
    int countSourceVideo = 1000;
    int countResultVideo = 1000;
    int countSourceImg = 1000;
    // regimeWriting
    // 0 - запись всех
    // 1 - запись только исодной стереопары
    // 2 - запись только результата обработки
    int regimeWriting;

    int currentIndex_selectorImage; // текущий индеск вывода результатов (вывод на экран или в файл)




    //======================================
    // Параметры для проведения калибровки
    //=====================================
    string setNewCalibImg_folderName;
    string setNewCalib_fileNamePath;
    string setNewCalib_fileList;
    string usedNewCalib_fileList;

    string WriteSourseStereoVideoFilesName;


    int board_w, board_h;
    double  squareSize;   // square size (mm)


    //==============================
    // Параметры для включения фильтров предобработки
    //==============================
    //
    //    0 - без обработки;
    //    1 - equalizeHist (Color RGB);
    //    2 - CLAHE (Color RGB);
    //    3 - CLAHE;
    //    4 - equalizeHist;


    vector<int> metod_ImagePerProcessing = {0, 0, 0, 0, 0, 0};

    //==============================
    // Параметры для включения фильтров предобработки
    //==============================
    int metod_PointCorrectinForClastering = 0;

    //==============================
    // Параметры для StereoBM
    //==============================
    int metod_Disparity = 0;   // Метод 0 - BM; 1 - MODE_SGBM; 2 - MODE_HH; 3 - MODE_SGBM_3WAY; 4 - MODE_HH4;
    int smbNumDisparities;   /**< Range of disparity */ // ndisparities = 16*43
    int smbBlockSize; /**< Size of the block window. Must be odd */ //SADWindowSize = 21

    int smbPreFilterCap;        // 31 -> 15 +++

    int smbMinDisparity;

    int smbTextureThreshold;    // 10   +++
    int smbUniquenessRatio;     // 15   +++
    int smbSpeckleWindowSize;  // 100 +++
    int smbSpeckleRange;        // 32
    int smbDisp12MaxDiff;


    // Коэффициент изменения размера выводимого изображения
    double CoefShowWindow;



    // Переменная для настроек прорисовки на изображении
    vector<int> mode_AddResults;


    //==============================
    // Параметры выбора Basler камер
    //==============================

    int c_maxCamerasToUse;

    int SerialNumber_StereoCam00left;
    int SerialNumber_StereoCam01right;
    int SerialNumber_HandCam02left;
    int SerialNumber_HandCam03right;

    //==============================
    // Сирийные номера для выбора камер HikRobot
    //==============================
    vector <string> CamSN;  // Серийные номера устройств (камер) по 2-ому списку


    // =============================
    // Переременные для работы IP с камерами симулятора CaptureFromIP()
    // ============================
    // адреса камер симулятора
    string videoStreamAddressLeft;
    string videoStreamAddressRight;
    string videoStreamAddressLeftHand;
    string videoStreamAddressRightHand;

    // =============================
    // Матрица преобразования (в неодородной)
    // ============================

    cv::Mat TransformMatrix = cv::Mat(4, 4, CV_64F);
    //cv::Mat TransformMatrix;







    // =============================
    //  Переменные управления размером изображения Basler камер
    // =============================
    // Параметры для обрезки изображений
    vector<int> cam_CX;// Центрирование изображения по горизонтали
    vector<int> cam_CY;// Центрирование изображения по вертикали
    vector<int>  cam_dX;// Горизонтальный сдвиг активной области относительно левой стороны сенсора (в пикселях)
    vector<int>  cam_dY;// Вертикальный сдвиг активной области относительно верхней части сенсора (в пикселях)
    vector<int>  cam_X;// Ширина активной области (активной зоны) камеры в пикселях
    vector<int>  cam_Y;// Высота активной области (активной зоны) камеры в пикселях.
    // Параметры сжатия изображения
    int  img_X;// Ширина сжатого изображения
    int  img_Y;// Высота сжатого изображения
    double  img_coeff;// Коэфициент восколько раз уменьшено изображение (не связан с размерами img_X img_Y)


    // =============================
    //  Переменные автозапуска GUI
    // =============================
    int GuiWindowHide;         // Скрытия окна интерфейса
    int GuiAutoStart;          // Авто запустк программы без кнопки старт
    int GuiSelectSorceImg;     // Выбор источника изображений: камера, видео итд
    int GuiSelectWorkMode;       // Выбор режима работы: Обычный, Калибровка, "Аврора"
    int GuiSelecttTipeShowWindow; // Выбор картинки, кот. выводится
    int GuiSelectCamType;           // Выбор типа камеры
    // =============================
    //  Переменные параметров цвета для поиска по объектов (Олег К.)
    // =============================
    Scalar LowHSV;       // Нижний граница порога HSV
    Scalar HieghHSV;     // Верхняя граница порога HSV



    // Парамет выбора способа ввода кординат клика мыши
    // 0 - ввод через окно OpenCV
    // 1 - ввод из стороннего источника

    int MouseClickSelector = 1;

    // =============================
    //  Настройки потоков
    // =============================

    vector<bool> exitCode; // переменная для выхода из цикла в потоках
    int timeDelay_startThread; // Задержка запуска потоков в порядке очередности
    int timeStep_Thread; // Время шага одной операции в потоке
    int maxNumThread; // максимальное количество потоков
    vector<bool> idxThreadAutoRestart; // переменная для перезапуска потока в случаи отключения


    vector<int>timeMsThreadRuntime = {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0};
    vector<int> timeMsMinDelay = {20,20,20,20,20, 20,20,20,20,20, 20,20,20,20,20, 20,20,20,20,20, 20,20,20,20,20};

    vector<vector<int>> Algorithm_inThread_seq; // последовательность номеров алгормов для запуска в потоках
    vector <std::thread> threads; // вектор с информацией о потоках


    // Наборы распределённызх по потокам алгоримов для Algorithm_inThread_seq
    vector<vector<int>> set01_RTK_Algorithm_inThread_seq;
    vector<vector<int>> set02_Calibration_Algorithm_inThread_seq;
    vector<vector<int>> set03_Aurora_Algorithm_inThread_seq;



    // =============================
    //  Для Автозапуска
    // =============================

    bool LossCamconnection = false; // флаг потери соединения с камерами


};

//#if defined(_WIN64)
//// ================
//// Only Windows
//// ================



//extern "C"
//{
//// =====================================
////  Initialize
//// ====================================
//__declspec(dllexport) ADS::cl_DenseStereo* __stdcall CreateObj();
//__declspec(dllexport) str_Settings* __stdcall Create_str_Settings();
//__declspec(dllexport) void __stdcall InitializeSettings(str_Settings*);
//__declspec(dllexport) void __stdcall InitializeObj(ADS::cl_DenseStereo*, str_Settings*, int);
//__declspec(dllexport) void __stdcall InitializeDrawWindows(ADS::cl_DenseStereo*);

//// =====================================
////  Grabing Images
//// ====================================
////  Basler
//__declspec(dllexport) void __stdcall InitializeBaslerCam(ADS::cl_DenseStereo* );
//__declspec(dllexport) void __stdcall openBaslerCam(ADS::cl_DenseStereo* );
//__declspec(dllexport) void __stdcall closeBaslerCam(ADS::cl_DenseStereo* );
//__declspec(dllexport) void __stdcall stopCaptureBaslerCam(ADS::cl_DenseStereo*);

//__declspec(dllexport) void __stdcall CaptureBaslerCamImg(ADS::cl_DenseStereo*, str_Settings*);
//__declspec(dllexport) void __stdcall CaptureBaslerCamImg_thread(ADS::cl_DenseStereo*, str_Settings*);

//// Simulator
////IP-cam
//__declspec(dllexport) void __stdcall OpenIPcam(ADS::cl_DenseStereo*, str_Settings*);
//__declspec(dllexport) void __stdcall CaptureFromIP(ADS::cl_DenseStereo*, str_Settings*);
//__declspec(dllexport) void __stdcall CaptureFromIP_thread(ADS::cl_DenseStereo*, str_Settings*);

////
//__declspec(dllexport) ADS::cl_DenseStereo* __stdcall СaptureFromImgFile(ADS::cl_DenseStereo*, str_Settings*);
//    __declspec(dllexport) ADS::cl_DenseStereo* __stdcall CaptureFromVideo(ADS::cl_DenseStereo*, str_Settings* );
//__declspec(dllexport) ADS::cl_DenseStereo* __stdcall CaptureFromVideo_thread(ADS::cl_DenseStereo*, str_Settings* );



//__declspec(dllexport) vector<cv::Mat> __stdcall getSorceStereoImg(ADS::cl_DenseStereo* );



//// =====================================
//// Получение 3D точек
//// =====================================


//__declspec(dllexport) void __stdcall Detect3Dpoints_ADS(ADS::cl_DenseStereo*, str_Settings*);
//__declspec(dllexport) void __stdcall Detect3Dpoints_ADS_thread(ADS::cl_DenseStereo*, str_Settings*);
//__declspec(dllexport) void __stdcall Detect3Dpoints_ASS_thread(ADS::cl_DenseStereo*, str_Settings*);


//// =====================================
//// Clasering
//// =====================================
//__declspec(dllexport) void __stdcall ClusteringDBScan(ADS::cl_DenseStereo*, str_Settings*, int);
//__declspec(dllexport) void __stdcall ClusteringDBScan_thread(ADS::cl_DenseStereo*, str_Settings*, int);


//// =====================================
//// AddResults
//// =====================================


//__declspec(dllexport) void __stdcall  AddResults(ADS::cl_DenseStereo*, str_Settings*);
//__declspec(dllexport) void __stdcall  AddResults_thread(ADS::cl_DenseStereo*, str_Settings*);

//__declspec(dllexport) void __stdcall  getResultsIm(ADS::cl_DenseStereo*, uchar**,  int*, int*,int* );



//// =====================================
//// Primitive
//// =====================================


//__declspec(dllexport) void __stdcall  DetectAddPrimitive(ADS::cl_DenseStereo*, str_Settings*);
//__declspec(dllexport) void __stdcall  DetectAddPrimitive_thread(ADS::cl_DenseStereo*, str_Settings*);



//// =====================================
//// Write Source Video
//// =====================================
//__declspec(dllexport) void __stdcall  WriteSourceVideo_thread(ADS::cl_DenseStereo*, str_Settings*);


//// =====================================
//// set MouseClick Coordinates
//// =====================================

//__declspec(dllexport) void __stdcall setMouseClickCoordinates(ADS::cl_DenseStereo* ,int, int);



//// =====================================
//// ShowResults
//// =====================================

//__declspec(dllexport) void __stdcall ShowResults(ADS::cl_DenseStereo*,  str_Settings *, int, int);


//__declspec(dllexport) void __stdcall ShowResults_cycle(ADS::cl_DenseStereo*,  str_Settings *);



//// =====================================
//// Aurora
//// =====================================
//__declspec(dllexport) void __stdcall  Aurora_FindColorObj(ADS::cl_DenseStereo*, str_Settings *);



//// =====================================
//// Calibrate StereoCam
//// =====================================
//__declspec(dllexport) void __stdcall CalibrateStereoCam(ADS::cl_DenseStereo*, str_Settings*);


//// =====================================
//// Resieve Results
//// =====================================

//__declspec(dllexport) ADS::cl_DenseStereo*  __stdcall copy_obj(ADS::cl_DenseStereo* obj_ptr);



//__declspec(dllexport) ADS::t_vuxyzrgb* __stdcall Create_copy_vuxyzrgb();
//__declspec(dllexport) void __stdcall copy_vuxyzrgb(ADS::cl_DenseStereo*, ADS::t_vuxyzrgb* vuxyzrgb_copy );




//__declspec(dllexport) ADS::type_arma_vuxyzrgb* __stdcall copy_vuxyzrgb_arma(ADS::cl_DenseStereo* );


//__declspec(dllexport) int __stdcall get_num_cl(ADS::t_vuxyzrgb*);
//__declspec(dllexport) int __stdcall get_num_prim(ADS::t_vuxyzrgb*);
//__declspec(dllexport) int __stdcall get_num_p(ADS::t_vuxyzrgb*);
//__declspec(dllexport) int __stdcall get_SelectedClaster(ADS::t_vuxyzrgb *);


//__declspec(dllexport) int __stdcall get_objRect2D(ADS::t_vuxyzrgb* , int , int );
//__declspec(dllexport) double __stdcall get_objRect3D(ADS::t_vuxyzrgb *, int ,int);
//__declspec(dllexport) double __stdcall get_objRect3D_robot(ADS::t_vuxyzrgb *, int ,int);
//__declspec(dllexport) double __stdcall get_distance2Point3D(ADS::t_vuxyzrgb *, int );


//__declspec(dllexport) int __stdcall get_typeDetectPrimitive(ADS::t_vuxyzrgb*, int );
//__declspec(dllexport) double __stdcall get_PrimitiveParameter(ADS::t_vuxyzrgb *, int ,int);


//__declspec(dllexport)   double __stdcall get_x_center(ADS::t_vuxyzrgb *, int);
//__declspec(dllexport)   double __stdcall get_y_center(ADS::t_vuxyzrgb *, int );
//__declspec(dllexport)   double __stdcall get_z_center(ADS::t_vuxyzrgb *, int );
//__declspec(dllexport)   double __stdcall get_radius(ADS::t_vuxyzrgb *, int );


//__declspec(dllexport) void __stdcall saveFileResult(ADS::cl_DenseStereo* );

//__declspec(dllexport) void __stdcall write2File_vuxyzrgb(ADS::cl_DenseStereo*);
//__declspec(dllexport) void __stdcall write2File_vuxyzrgb_arma(ADS::cl_DenseStereo*);


//__declspec(dllexport) void __stdcall getImg(ADS::cl_DenseStereo*, int , uchar** , int* , int* , int* );
//__declspec(dllexport) cv::Mat __stdcall getImg02(ADS::cl_DenseStereo*,  str_Settings *, int, int);

//// =====================================
//// Test
//// =====================================

//__declspec(dllexport) cv::Mat returnImg();


//__declspec(dllexport) void WriteOneStereoPair(ADS::cl_DenseStereo*, str_Settings*);


//}



//// ================
//// Only Windows END
//// ================

//#else

//// ================
//// Only Linux
//// ================



//extern "C"
//{
// =====================================
//  Initialize
// ====================================
// TCP
    TCPServer*  CreateTcpServerObj();
    TCPClient*  CreateTcpClientObj();

 ADS::cl_DenseStereo*  CreateObj();
 str_Settings*  Create_str_Settings();
 void  InitializeSettings(str_Settings*);
 void  InitializeObj(ADS::cl_DenseStereo*, str_Settings*, int);
 void  InitializeDrawWindows(ADS::cl_DenseStereo*);

// =====================================
//  Grabing Images
// ====================================
//  Basler
 void  InitializeBaslerCam(ADS::cl_DenseStereo* );
 void  openBaslerCam(ADS::cl_DenseStereo* );
 void  closeBaslerCam(ADS::cl_DenseStereo* );
 void  stopCaptureBaslerCam(ADS::cl_DenseStereo*);

 void  CaptureBaslerCamImg(ADS::cl_DenseStereo*, str_Settings*);
 void  CaptureBaslerCamImg_thread(ADS::cl_DenseStereo*, str_Settings*);

// Simulator
//IP-cam
 void  OpenIPcam(ADS::cl_DenseStereo*, str_Settings*);
 void  CaptureFromIP(ADS::cl_DenseStereo*, str_Settings*);
 void  CaptureFromIP_thread(ADS::cl_DenseStereo*, str_Settings*);

//
 ADS::cl_DenseStereo*  СaptureFromImgFile(ADS::cl_DenseStereo*, str_Settings*);
     ADS::cl_DenseStereo*  CaptureFromVideo(ADS::cl_DenseStereo*, str_Settings* );
 ADS::cl_DenseStereo*  CaptureFromVideo_thread(ADS::cl_DenseStereo*, str_Settings* );



 vector<cv::Mat>  getSorceStereoImg(ADS::cl_DenseStereo* );





 // =====================================
 // обобщение алгоритмов и обобщение потоков
 // =====================================
 void Start_certainThread(ADS::cl_DenseStereo*, str_Settings*, int);



// =====================================
// Получение 3D точек
// =====================================


 void  Detect3Dpoints_ADS(ADS::cl_DenseStereo*, str_Settings*);
 void  Detect3Dpoints_ADS_thread(ADS::cl_DenseStereo*, str_Settings*);
 void  Detect3Dpoints_ASS_thread(ADS::cl_DenseStereo*, str_Settings*);


// =====================================
// Clasering
// =====================================
 void  ClusteringDBScan(ADS::cl_DenseStereo*, str_Settings*, int);
 void  ClusteringDBScan_thread(ADS::cl_DenseStereo*, str_Settings*, int);


// =====================================
// AddResults
// =====================================


 void   AddResults(ADS::cl_DenseStereo*, str_Settings*);
 void   AddResults_thread(ADS::cl_DenseStereo*, str_Settings*);

 void   getResultsIm(ADS::cl_DenseStereo*, uchar**,  int*, int*,int* );



// =====================================
// Primitive
// =====================================


 void   DetectAddPrimitive(ADS::cl_DenseStereo*, str_Settings*);
 void   DetectAddPrimitive_thread(ADS::cl_DenseStereo*, str_Settings*);



// =====================================
// Write Source & Result Video
// =====================================
 void   WriteResultVideo_thread(ADS::cl_DenseStereo*, str_Settings*);
 void   WriteSourceVideo_thread(ADS::cl_DenseStereo*, str_Settings*);

// =====================================
// set MouseClick Coordinates
// =====================================

 void  setMouseClickCoordinates(ADS::cl_DenseStereo* ,int, int);



// =====================================
// ShowResults
// =====================================

 void  ShowResults(ADS::cl_DenseStereo*,  str_Settings *, int, int);


 void  ShowResults_cycle(ADS::cl_DenseStereo*,  str_Settings *);



// =====================================
// Aurora
// =====================================
 void   Aurora_FindColorObj(ADS::cl_DenseStereo*, str_Settings *);



// =====================================
// Calibrate StereoCam
// =====================================
 void  CalibrateStereoCam(ADS::cl_DenseStereo*, str_Settings*);


// =====================================
// Resieve Results
// =====================================

 ADS::cl_DenseStereo*   copy_obj(ADS::cl_DenseStereo* obj_ptr);



 ADS::t_vuxyzrgb*  Create_copy_vuxyzrgb();
 void  copy_vuxyzrgb(ADS::cl_DenseStereo*, ADS::t_vuxyzrgb* vuxyzrgb_copy );




 ADS::type_arma_vuxyzrgb*  copy_vuxyzrgb_arma(ADS::cl_DenseStereo* );


 int  get_num_cl(ADS::t_vuxyzrgb*);
 int  get_num_prim(ADS::t_vuxyzrgb*);
 int  get_num_p(ADS::t_vuxyzrgb*);
 int  get_SelectedClaster(ADS::t_vuxyzrgb *);


 int  get_objRect2D(ADS::t_vuxyzrgb* , int , int );
 double  get_objRect3D(ADS::t_vuxyzrgb *, int ,int);
 double  get_objRect3D_robot(ADS::t_vuxyzrgb *, int ,int);
 double  get_distance2Point3D(ADS::t_vuxyzrgb *, int );


 int  get_typeDetectPrimitive(ADS::t_vuxyzrgb*, int );
 double  get_PrimitiveParameter(ADS::t_vuxyzrgb *, int ,int);


   double  get_x_center(ADS::t_vuxyzrgb *, int);
   double  get_y_center(ADS::t_vuxyzrgb *, int );
   double  get_z_center(ADS::t_vuxyzrgb *, int );
   double  get_radius(ADS::t_vuxyzrgb *, int );


 void  saveFileResult(ADS::cl_DenseStereo* );

 void  write2File_vuxyzrgb(ADS::cl_DenseStereo*);
 void  write2File_vuxyzrgb_arma(ADS::cl_DenseStereo*);


 void  getImg(ADS::cl_DenseStereo*, int , uchar** , int* , int* , int* );
 cv::Mat  getImg02(ADS::cl_DenseStereo*,  str_Settings *, int, int);

// =====================================
// Test
// =====================================

 cv::Mat returnImg();
 void WriteOneStereoPair(ADS::cl_DenseStereo*, str_Settings*);




 // ********************************
 // Если вы хотите объявить и реализовать подкласс QObject в файле cpp, вам необходимо вручную включить файл moc.
 //#include "func_main.moc"

//}


//// ================
//// Only Linux END
//// ================

//#endif


#endif // FUNC_MAIN_H
