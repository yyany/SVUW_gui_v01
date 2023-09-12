#ifndef FUNC_DENSESTEREO_H
#define FUNC_DENSESTEREO_H





// ====================================================
#include "cl_VideoStreamer.h" // ошибка  error: C2011: 'sockaddr' подключается в func_DenseStereo.h
// ====================================================

// Заголовочные файлы других окон QTFormApp_3DRuler (В.В.Альчаков)
#include "QTFormApp_3DRuler/camerascene.h" // ошибка  error: C2011: 'sockaddr' подключается в func_DenseStereo.h
#include "QTFormApp_3DRuler/toolwindow.h"




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



// Новые библиотеки для определения габаритов объектов
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <list>


// OpenCV
#include <opencv2/opencv.hpp>
//#include <opencv2/ximgproc/disparity_filter.hpp>
// #include <opencv2/cudastereo.hpp>

//



#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>

#include <math.h>
#include <ctime>

#include <chrono>
#include <fstream>

#include <cerrno> // Для кода ошибок в DarkNet-test-simple

// Для Одометрии (Леша)
#include <iomanip>
//#include "Odometry_Include/StereoParams.h"
//#include "Odometry_Include/StereoImage.h"
//#include "Odometry_Include/PointCloud.h"
//#include "Odometry_Include/Trajectory.h"






// thread
#include <thread>
#include <mutex>

// Armadilo
#include <armadillo>


// Pyclustering
#include <pyclustering/definitions.hpp>
#include <pyclustering/cluster/dbscan.hpp>
#include <pyclustering/cluster/dbscan_data.hpp>
#include <pyclustering/cluster/cluster_data.hpp>




// Qt widgets
#include <QApplication>


#include <QtGui>    // **** возможно лишнее ****

#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QtGlobal>



#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPoint>

#include <QFile>


using namespace std;
using namespace cv;
//using namespace Concurrency;


namespace ADS {




//// =============================================
//// Вложенные структуры статусов оборудования
//// =============================================



//struct StateInformation    // информация о состоянии
//{
//    int SecondaryPowerSupply;   // вторичное питание
//    int ConnectToCams;          // соединение с камерами
//    int ImageAquisition;        // получение изображения
//    int SuccessfulWork;         // статус успешной работы
//    int FuctSuccessfulWork;    // факт успешной работы
//};



//struct t_vuxyzrgb     // Структура в формате std:vector
//{
//    int num_p;                         // Количество точек
//    int num_cl;                        // Количество кластеров
//    vector<int> num_p_cl;              // Количесво точек в кластере

//    int SelectedClaster;               // Номер кластера, выбранного оператором

//    // до кластеризаци сразу полсе обнаружения 3D точек
//    int num_p0;                         // Количество точек
//    vector<vector<int>> vu0;            // 2D координаты точки на изображении
//    vector<vector<double>> xyz0;        // 3D координаты точки на пространсве
//    vector<vector<int>> rgb0;           // цвет 3D точки


//   // после кластеризации
//    vector<vector<int>> vu;            // 2D координаты точки на изображении
//    vector<vector<double>> xyz;        // 3D координаты точки на пространсве
//    vector<vector<int>> rgb;           // цвет точки
//    vector<int> cluster;               // Для каждой точки номер кастера

//    vector<vector<size_t>> clusters;   // Для каждого кластера номер точки
//    vector<vector<int>> objRect2D;       // Координаты рамки выделяющей объект на изображении

//    vector<vector<double>> objRect3D;       // Координаты рамки выделяющей объект в 3D пространсве

//    vector<double> distance2Point3D;      // координат 2-х выбранных точек и рассояние между ними



//    // distance2Point3D_temp = {xyz01[0], xyz01[1], xyz01[2], xyz02[0], xyz02[1], xyz02[2], Distance};
//    // {vu_min.x, vu_min.y, vu_max.x, vu_max.y, Center2D.x, Center2D.y, radius2D}
//    // {xyz_min.x, xyz_min.y, xyz_min.z, xyz_max.x, xyz_max.y, xyz_max.z, Center3D.x, Center3D.y,Center3D.z, radius3D }

//    //=================================================
//    // Координаты в системе координат робота
//    //=================================================

//    // Матрица трансформации для перевода из координат камеры в кординаты робота
//    cv::Mat RTmatrix_cam2robot_cv = cv::Mat(4, 4, CV_64F);

//    // Матрица вращения для пересчета координат при поворете камеры поворотным устройсвтвом
//    cv::Mat RTmatrix_camRotation_cv = cv::Mat(4, 4, CV_64F);



//    vector<vector<double>> objRect3D_robot_vec;       // Координаты рамки выделяющей объект в 3D пространсве

//    //=================================================

//    // для храниения, какого вида примитив у объекта
//    // 0 - не определено
//    // 1 - плоскость
//    // 2 - сфера
//    // 3 - цилиндр

//    int num_prim;                 // количесво примитивов, должно совпадать с num_cl, введено так как иногда не совпадаес из-за рассинхронизации
//    vector<int> PrimitiveDetect; //  виды примитивов объектов (всех кластеров)
//    vector<vector<double>> PrimitiveParameter; //  параметры модели привитивов (всех кластеров)
//    vector<vector<Point3d>>PrimitivePoints; // векотор точек плоскоси (4 шт) и цилиндра (30 шт) для прорисовки их на изображении


//    cv::Mat xyz_Disparity_AllPoints; // Координаты всех найденных точек в виде изображения uv по слоям xyz

//    cv::Size imgSize; // Размер изображения

//    vector<double> limit_WorkingArea;   // Границы рабоче зоны для стереокамры СТЗ, за её пределами ничего не видит



//    //=================================================
//    // Габариты объекта
//    //=================================================
//    double ObjSize_circumscribedLength = -1;
//    double ObjSize_circumscribedWidth = -1;
//    double ObjSize_circumscribedHeight = -1;
//    double ObjSize_ObjLength = -1;
//    double ObjSize_ObjWidth = -1;      //


//};

//struct type_arma_vuxyzrgb     // Структура в Armadillo формате
//{
//    int num_p;                                  // Количество точек
//    int num_cl;                                 // Количество кластеров
//    int SelectedClaster;                        // Номер кластера, выбранного оператором

//    arma::Col<int> num_p_cl;                    // Количесво точек в кластере

//    arma::Mat<int> vu;                          // 2D координаты точки на изображении
//    arma::Mat<double> xyz;                      // 3D координаты точки на пространсве  // dmat = Mat<double>
//    arma::Mat<int> rgb;                         // цвет точки
//    arma::Col<int> cluster;                     // Для каждой точки номер кастера

//    // Для каждого кластера соответсвующая матрица с: ..
//    arma::field<arma::Mat<int>> clusters_indx;  // Для каждого кластера номера точек
//    arma::field<arma::Mat<int>> clusters_vu;    // Для каждого кластера 2D координаты на изображении
//    arma::field<arma::Mat<double>> clusters_xyz; // Для каждого кластера 3D координаты точки в пространсве
//    arma::field<arma::Mat<int>> clusters_rgb;    // Для каждого кластера цвет точки


//    arma::Mat<int> objRect2D;       // Координаты рамки выделяющей объект на изображении
//    arma::Mat<double> objRect3D;       // Координаты рамки выделяющей объект в 3D пространсве


//    //=================================================
//    // Координаты в системе координат робота
//    //=================================================

//    // Матрица трансформации для перевода из координат камеры в кординаты робота

//    arma::Mat<double> RTmatrix_cam2robot_arma;
//   // arma::mat RTmatrix_cam2robot_arma;
//    // Матрица вращения для пересчета координат при поворете камеры поворотным устройсвтвом
//    arma::Mat<double> RTmatrix_camRotation_arma;
//    //arma::mat RTmatrix_camRotation_arma;

//    arma::Mat<double> objRect3D_robot_arma;       // Координаты рамки выделяющей объект в 3D пространсве





//};




type_arma_vuxyzrgb conv_std2arma (const t_vuxyzrgb& );
t_vuxyzrgb get_uvxyz_02 (const cv::Mat&, const cv::Mat& , int , vector<double> );


vector<double> FitPlane(vector<Point3d>, vector<Point3d>&);
vector<double> FitSphere(vector<Point3d>);
vector<double> FitCylinder(vector<Point3d>, vector<double>, vector<Point3d>&);

Mat MNK(Mat, Mat);
double minMatElement(Mat);
double maxMatElement(Mat);


QImage Mat2QImage(cv::Mat const& src);
cv::Mat QImage2Mat(QImage const& src);
QPixmap QImage2QPixmap( QImage const& src );

// Для объединения облака 3D точек с контурами
bool FindPoint(double PoitsF[4][4], double x,double y);




//===================================
// Функции и класс для определения габоритов объектов (начало)
//===================================

class Point3D {
public:
    // Координаты вершины
    double X;
    double Y;
    double Z;
    // Кол-во точек возле вершины
    int NumberOfPoint;
    // Имя точки
    std::string Name;
    Point3D(double x, double y, double z, std::string name);
    // Метод для установки кол-ва точек
    void setNumberOfPoint(int numberofpoints);

    // Переопределяем оператор для возможности сортировки
    bool operator< (const Point3D& p);

};


// Формула Расстояния между двумя точками 3D
double get_distance(ADS::Point3D p1, ADS::Point3D p2);
double get_sum_distance(std::vector<double> X,
                        std::vector<double> Y,
                        std::vector<double> Z,
                        ADS::Point3D MN, ADS::Point3D M0);

size_t get_sum_count(std::vector<double> X,
                     std::vector<double> Y,
                     std::vector<double> Z,
                     ADS::Point3D MN, ADS::Point3D M0) ;

std::vector<double> sum(std::vector<double> a, std::vector<double> b) ;
std::vector<double> substr(std::vector<double> a, std::vector<double> b) ;
std::vector<double> cross(std::vector<double> a, std::vector<double> b);
std::vector<double> np_divide(std::vector<double> a, std::vector<double> b) ;
double np_linalg_norm(std::vector<double> a) ;
int np_dot(std::vector<double> v1, std::vector<double> v2) ;
// нахождение самого длинного перпендикуляра к прямой
double lineseg_dist(std::vector<double> p, std::vector<double> a, std::vector<double> b) ;
// Нахождение минимальных и максимальных элементов в массивах
void get_sizes(std::vector<double> X, std::vector<double> Y, std::vector<double> Z, double* L, double* W, double* H, double* Length, double* Width);


//===================================
// Функции и класс для определения габоритов объектов (конец)
//===================================


float expRunningAverage(float newVal, int filVal);


//===================================
//  DarkNet-test-simple (начало)
//===================================




/** Параметры обработки */
static const float SCORE_THRESHOLD      = 0.50;
static const float NMS_THRESHOLD        = 0.45;
static const float CONFIDENCE_THRESHOLD = 0.45;

/** Параметры шрифтов */
static const float FONT_SCALE = 0.7;
static const int   THICKNESS  = 1;

/** Цветовые константы */
static cv::Scalar BLACK  = cv::Scalar(0,   0,   0);
static cv::Scalar YELLOW = cv::Scalar(0, 255, 255);
static cv::Scalar RED    = cv::Scalar(0,   0, 255);
static cv::Scalar GREEN  = cv::Scalar(0, 255,   0);

/** Класс детектора */
class NeuralNetDetector {
private:
    /** Структура нейросети */
    cv::dnn::Net network;
    /** Ширина и высота входного изображения */
    int input_width = 640;
    int input_height = 640;
    /** Вектор распознаваемых классов */
    std::vector<std::string> classes;
    /** Структуры для хранения результатов обработки */
    std::vector<int> classes_id_set;
    std::vector<cv::Rect> boxes_set;
    std::vector<float> confidences_set;
    std::vector<std::string> classes_set;
    cv::Mat processed_image;
    /** Время обработки */
    float inference_time;
    /** Получить строковые значения классов */
    errno_t read_classes(const std::string file_path);
    /** Инициализация нейросети */
    errno_t init_network(const std::string model_path,
                         const std::string classes_path);
    /** Отрисовка метки */
    void draw_label(cv::Mat& img, std::string label, int left, int top);
    /** Предобработка результатов */
    std::vector<cv::Mat> pre_process(cv::Mat &img, cv::dnn::Net &net);
    /** Постобработка результатов */
    cv::Mat post_process(cv::Mat &img, std::vector<cv::Mat> &outputs,
                         const std::vector<std::string> &class_name);
public:
    NeuralNetDetector(const std::string model, const std::string classes);
    NeuralNetDetector(const std::string model, const std::string classes,
                      int width, int height);
    std::vector<float> get_confidences(void) { return confidences_set; }
    std::vector<cv::Rect> get_boxes(void) { return boxes_set; }
    std::vector<int> get_class_ids(void) { return classes_id_set; }
    std::vector<std::string> get_classes(void) { return classes_set; }
    float get_inference(void) { return inference_time; }
    std::string get_info(void);
    cv::Mat process(cv::Mat &img);
    cv::Mat get_image(void);
};



//===================================
//  DarkNet-test-simple (конец)
//===================================
//===================================





}

#endif // FUNC_FIT_FIGURE_H
