#ifndef CL_DENSESTEREO_H
#define CL_DENSESTEREO_H




// Classes
#include "cl_ImGrabBaslerCam.h"
// function


#include <func_DenseStereo.h>
#include <cl_TCP_server.h>
#include <cl_TCP_client.h>


//#include <QTcpSocket>
//#include <QObject>

using namespace std;
using namespace cv;

#if defined(_WIN64)

// ================
// Only Windows
// ================
using namespace Concurrency;
// ================
// Only Windows END
// ================

#else

// ================
// Only Linux
// ================

// ================
// Only Linux END
// ================

#endif


namespace ADS {


class cl_DenseStereo:  public Basler::cl_ImGrabBaslerCam,
                       public TCPClient
//              public TCPServer, public TCPClient
//    , public pyclustering::clst::dbscan, public pyclustering::clst::cluster_data
{

   // Q_OBJECT // используем сигнально-слотовые связи поэтому прописываем Q_OBJECT


public:

    cl_DenseStereo();
    // virtual  ~cl_DenseStereo();


    void InitializeDrawWindows();

    void setCalibration(string calibration_filename);

    void FindPointsCheckerBoard();

    void FindPointsADS(int limit_OutPoints,
                       int metod_Disparity); // <<<<<<<<<<<<<<
    void FindPointsASS();

    void DrawDisparityMap();
    void DrawDisparityMap02();
    void DrawDisparityMap03();





    // =============================
    // Одометрия Леша
    // =============================
    int Odometry(bool);


    // =============================
    // Работа с контурами (Олег К.)
    // =============================

    t_vuxyzrgb get_vuxyzrgbColorObj();
    void set_vuxyzrgbColorObj(t_vuxyzrgb);
    cv::Mat getImgFindColorObj();
    void setImgFindColorObj(cv::Mat);
    void setFindColor(cv::Scalar LowScalar, cv::Scalar HScalar);



    void FindColorObjMask();        // определение маски объекта определенного цвета
    void SearchContours();          // поиск контуров
    void SearchBigObjContours();    // поиск самого большого контура
    void AddContoursToIm();         // добавляет контура на изображение
    void matchContoursTo3Dpoints(); // сопоставление 3д точек с контуром
    void SaveInfColorObj();         // сохранение результатов в файл

    void Draw3dPoint();




   // ==================
   // Кластеризация
   // ==================
    void clusteringDBScan(const double rad,
                          const size_t neighbors,
                          const int filter_ClusterPoints,
                          const double coef_base_dbs,
                          const double coef_pow_dbs,
                          bool metod_PointCorrectinForClastering);     // Кластеризация DBScan

    vector<double>  getClasterCoordinate3D(int clusterID);


    void Coordinate_cam2robot();

    // ==================
    // Добавление результатов на изображение
    // ==================
    void AddResultsToIm( vector<int> mode);    // функция нанесения результатов на изображение
    void AddResultsToDisparityMap02();         // функция нанесение результатов на Карту диспаратности
    void AddResultsToDisparityMap03();         // функция  нанесение доп. информации на изображение с результатами

    void saveFileResult();
    void DrawResultsInImage();
    void DrawResultsInImage_AlLIm(int selectorImage);

    // ==================
    cv::Mat  getResultImage();
    void  setResultImage(cv::Mat);
    // ==================

    // ==================
    cv::Mat  getShowImg();
    void  setShowImg(cv::Mat);
    // ==================

    // ==================
    // Предобрабока
    // ==================

    void ImgRemaping();
    void ImgPerProcessing(vector<int> metod_ImagePerProcessing);
    vector<cv::Mat> ImgPerProcessing_OneImg(cv::Mat img, vector<int> metod_ImagePerProcessing); // <<<<<<<<<<<<<<




    // ==================
    vector<cv::Mat> getImgRemap_StereoPair();
    void setImgRemap_StereoPair(cv::Mat, cv::Mat);
    cv::Mat getImgRemap_DoubleStereoImg();


    vector<cv::Mat> getImgColorPreProcess_StereoPair();
    void setImgColorPreProcess_StereoPair(vector<cv::Mat>);
    cv::Mat getImgColorPreProcess_DoubleStereoImg();

    vector<cv::Mat> getImgGrayPreProc_StereoPair();
    void setImgGrayPreProc_StereoPair(vector<cv::Mat>);


   // ==================





    cv::Mat  getDisparityMapImage();
    cv::Mat  getDisparityMapImage02();
    cv::Mat  getDisparityMapImage03();

    cv::Mat  getResultPrimitiveGeneralImage();
    cv::Mat  getResultPrimitivesXYImage();
    cv::Mat  getResultPrimitivesYZImage();
    cv::Mat  getResultPrimitivesXZImage();

    cv::Mat  getResultColorSearchImage();


    t_vuxyzrgb  get_vuxyzrgb();
    type_arma_vuxyzrgb  get_vuxyzrgb_arma();

     void set_vuxyzrgb(t_vuxyzrgb );
     void set_vuxyzrgb_arma(type_arma_vuxyzrgb);

    void setRTmatrix_cam2robot_cv(cv::Mat);
    void setRTmatrix_camRotation_angle(vector<double>);



    void searchColor();
    void DrawSearchColor();


    // ==================
    // Detect Primitive
    // ==================
    void DetectPrimitive_old();
    void DetectPrimitive_single();
    void DetectPrimitive_all();
    void AddPrimitiveToIm();
    void DetectAddPrimitive();            // объеденненая определение и прорисовка примитивов  // рабочая
    void DrawPrimitive();
    void DetectAddPrimitiveForContour(); // примитив для контура Олег К.

    // ==================
    void Clustering();

    vector<int> getMouseClickCoordinates02();
    void setMouseClickCoordinates02( int, int);
    void eventMouse_SelectObject(int);                   // Обработчик событий от мыши
    void setWorkingAria(vector<double>);
    void setParametersSBM( int, int, int, int, int, int, int, int, int);   // ввод параметров для  FindPointsADS()
    void setCoefShowWindow(double);

    void WriteSourseStereoImg_as_FilesImgSequence();
    void SaveImgSequence();

    void getCalibrateImages(const char*, const char* , vector<bool>* exitCode);
    void CalibrateStereoCam(const char*, const char*, int, int, float );





    void Rotation3Dpoints(double, double, double, double, double, double, double, double);
    cv::Mat  getImg3Dpoins();


    //===================================
    // Функции для определения габоритов объектов
    //===================================
    void DetectObjSize();




private:
    string calibration_filename = "./files/StereoCamParams.xml";
    const string disparity_filename = "./files/disp.txt";
    const string point_cloud_filename = "./files/point_cloud.txt";
     int limit_OutPoints = 3000;          // уменьшение количества найденных  точек для кластеризации
     int filter_ClusterPoints = 50;    // фильтр отсеивает прорисовку кластеров с количеством точек менее чем ...


    // Границы рабоче зоны для стереокамры СТЗ, за её пределами ничего не видит
    //    const double max_z = 5.0e3;
    //    const double max_x = 1.0e3;
    //    const double max_y = 1.0e3;
    vector<double> limit_WorkingArea {-3.0e3, -3.0e3, 250, 3.0e3, 3.0e3, 3.20e3};

    // Параметры цвета по которым происходит поиск объектов по цвету
    //vector<int> limit_searchColor {0, 110, 130, 25, 150, 170};

    // Параметры DBSCAN algorithm.
    const double rad = 60; // Connectivity radius between points, points may be connected if distance between them less then the radius.
    const size_t neighbors = 0; // minimum number of shared neighbors that is required for establish links between points.
    // Параметры формулы преобразования равноменого сгущения для кластеризации
    const double coef_base_dbs = 0.000735;
    const double coef_pow_dbs  =   0.80;


    // Координаты точки клика мыши
    vector<int> MouseClickCoordinates {0, 0};   // для ввода по клику мыши
    vector<int> MouseClickCoordinates02 {0, 0}; // для стороннего ввода координат
    // Переменная для проверки на изменение кликак мышы - сохряняет предидущий клик для
    vector<int> MouseCheck {1, 1};
    // Переменная для проверки на изменение кликак мышы  для searchColor();
    vector<int> MouseCheck_searchColor {0, 0};

    // Переменная для хранения цвета для поиска по цвету
     cv::Vec3b SelectColor {15, 130, 150};


    //int CheckZ = 1;
      //vector<int>* ptr_MouseClickCoordinates = &MouseClickCoordinates;
      vector<int>* ptr_MouseClickCoordinates = new vector<int>(2);

      // Координаты мыши привязанные к центру кластера
      int mouse_u = 0;
      int mouse_v = 0;
      //int mouse_z = 0;

      // Запоминание предыдущего центра кластера
      cv::Point previousCenter2D = {0,0};
      cv::Point3d previousCenter3D = {0,0,0};



     // Коэффициент изменения размера выводимого изображения
    double CoefShowWindow = 0.8;






    // Calibration parameters - Параметры калибровки
    cv::Mat M1, D1, M2, D2, R, T, F, E, R1, R2, P1, P2, Q;
    // Доп. к параметрам калибровки
    int nframes,num_Im;
    double rms,avgErr;
    cv::Size  imageSize;



    // Свойсво для настроек прорисовки на изображении
    vector<int> mode_AddResults;


//    std::vector< cv::Point3f >          boardModel;
//    std::vector< std::vector<cv::Point3f> >  objectPoints;
//    std::vector< std::vector<cv::Point2f> >  points[2];
//    std::vector< cv::Point2f >          corners[2];


    // Параметры алгоритма

    int Counter_AddResultsToIm = 0; // счётчик для вывода на изображение номера прорисовки
//    cv::Mat gray01r, gray02r;
    cv::Mat map11, map12, map21, map22;
    cv::Mat disparity, disparity_01, disparity_02;
    cv::Mat depth;


    cv::Mat xyz_AllPoints, vdisp, outImg01r, outImg02r, outGray01r;
    cv::Mat  disp_norm; // нормированная для наглядности, карта глубин изображения


    //ASS
    cv::Rect roi1, roi2;
    // ASS




    Mat image01_Undistored, image02_Undistored;
    //Mat image01 = Mat::zeros(1920, 1200, CV_8UC3);
    //Mat image02 = Mat::zeros(1920, 1200, CV_8UC3);

    //==============================
    // Параметры для StereoBM
    //==============================


    int smbNumDisparities = 16*43;   /**< Range of disparity */ // ndisparities = 16*43
    int smbBlockSize = 17; /**< Size of the block window. Must be odd */ //SADWindowSize = 21

    int smbPreFilterCap = 15;        // 31 -> 15 +++

    int smbMinDisparity = 6;

    int smbTextureThreshold = 10;    // 10   +++
    int smbUniquenessRatio = 15;     // 15   +++
    int smbSpeckleWindowSize = 100;  // 100 +++
    int smbSpeckleRange = 32;        // 32
    int smbDisp12MaxDiff= 1;        // 1


    // Параметр определяющий размеры кнопки при прорисовки экрана
    Point button01Size = {200,60};


    // Исходные изображения
    // cv::Mat imgLeft, imgRight;



    cv::Mat imgShow;

    cv::Mat img01remap, img02remap;                 // изображения после remap() -  ректификации (выравнивания)
    cv::Mat img01PreProc_color, img02PreProc_color;             // цветное изображение после предобработки и ректификации
    cv::Mat img01PreProc_gray, img02PreProc_gray;   // серое изображение после предобработки и ректификации


    // Параметры для добавления результата AddResultsToIm()  (по определеню объектов и их координат)
    cv::Mat imgResult01r;

    // Параметры для добавления результата AddPrimitiveToIm() (по определению примитивов)
    cv::Mat imgPrimitive01r_general;
    cv::Mat imgPrimitivesXY;
    cv::Mat imgPrimitivesYZ;
    cv::Mat imgPrimitivesXZ;

   // Параметры для добавления результата searchColor() (по определению примитивов)
    cv::Mat imgResultColorSearch;

    // Параметры для добавления результата по карте диспаратности (для линейки)
    cv::Mat imgResult_disp02;
    cv::Mat imgResult_disp03;



    //==============================
    // Параметры предобработки изображений (в плане на будущие)
    //==============================
    cv::Mat imgPreProc; // Изображение цветнного объекта




    //==============================
    // Параметры поиска объектов по цвету и контурам (Олег К.)
    //==============================
    t_vuxyzrgb vuxyzrgb_ColorObj;       // 3D точки, которые сопоставлены с контуром

    cv::Mat imgFindColorObjBoolMask;    // Изображение бинарное (фон черный объект белый)
    cv::Mat imgFindColorObjColorMask;   // Изображение цветнного (фон черный объект цветной)
    int iNum= -1;                       // Номер самого большого цветного объкта
    Mat canny_output;                   // Изображение цветнного
    vector<vector<Point>> contours;     // вектор для хранения всех найденных контуров (не маска)

    //(g1, b1, r1)) // зеленая палка
    //Scalar LowScalar2(30, 30, 0);
    //Scalar HScalar2(94, 255, 255);

    //(g1, b1, r1)) // красный шарик
    Scalar LowScalar = {0, 130, 40};// Нижний граница порога HSV
    Scalar HScalar = {15, 220, 220};// Верхняя граница порога HSV

    int CountfilenameColorObj = 1001;



    // >>>>>>> mutex <<<<<<<
    cv::Mat imgFindColorObj; // Изображение с найденным цветным объектом
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    //==============================
    // Параметры для searchColor
    //==============================

    // Параметр вывода изображения на экран
    cv::Mat  outResult01r;



    // Переменные для записи последовательности изображений
    int count_SequenceImgGrabs =1001;


    //==============================
    // Параметры для DrawDisparityMap()
    //==============================

    // флаги для Mouse01_temp и Mouse02_temp // true: занесены false: стерты
    bool flag01 = false;
    bool flag02 = false;

    // координаты начального и конечного клика
    vector<int> Mouse01 {0, 0};
    vector<int> Mouse02 {0, 0};
    vector<double> xyz01 {0, 0, 0};
    vector<double> xyz02 {0, 0, 0};



    //==============================
    // Параметры для отображения и вращения 3D точек
    //==============================
    cv::Mat img3Dpoins; // изображение с проекцией 3D точек


public:


    // Основные переменные для хранения результатов в виде структуры
    t_vuxyzrgb vuxyzrgb_3Dpoints; // Структура в формате std:vector как буфер между 3D поиском точек и кластеризацией
    t_vuxyzrgb vuxyzrgb; // Структура в формате std:vector
    type_arma_vuxyzrgb vuxyzrgb_arma; // Структура в Armadillo формате



    // =====================================
    // Mutex для потоков
    // =====================================


    std::mutex  vuxyzrgb_3Dpoints_mutex;    // для 3D точек после их обнаружения
    std::mutex  vuxyzrgb_mutex;             // данные
    std::mutex vuxyzrgb_ColorObj_mutex;     // мютекс для данны о 3D точках для контура

    std::mutex  mouse_mutex;

    std::mutex  imgDisparity_mutex;
    std::mutex  imgResultDisparity_mutex;
    std::mutex  imgPrimitives_mutex;
    std::mutex  imgResultColorSearch_mutex;


    std::mutex  imgRemap_mutex;
    std::mutex  imgPerProcess_mutex; // изображение результат предобработки для контуров
    std::mutex  imgResult_mutex;            // для результата
    std::mutex  imgShow_mutex;            // для текущего изображения на экране


    std::mutex  imgFindColorObj_mutex; // изображение результат поиска цветного объекта

    std::mutex img3Dpoins_mutex; // для изображения с проекцией 3D точек

    std::mutex CalibParam_mutex; // для параметров калибровки




// !!!!!!!!!!!!!
    // =============================
    // Работа с контурами (Олег К.) переменные для передачи данных по TCP
    // =============================


    // данных о факте обнаружения объкта (в момент обнаружения)
    int FuctOjectDetection_DS = 0;     // факт обнаружения объекта

    double OjectDetection_x_DS = -1 ; // x: линейные координаты обнаруженного объекта в системе координат СТЗ
    double OjectDetection_y_DS = -1;  // y:
    double OjectDetection_z_DS = -1;  // z:
    double ObjectDistance_DS = -1;    // дальность до обнаруженного объекта

    int ContouringCompletion_DS = 0;  // отчёт о завершении оконтуривания и готовности передать контур объекта в БПУ АНПА

// !!!!!!!!!!!!!


};




}
#endif // CL_ADS_H
