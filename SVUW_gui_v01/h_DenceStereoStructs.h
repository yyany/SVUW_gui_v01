#ifndef H_DENCESTEREOSTRUCTS_H
#define H_DENCESTEREOSTRUCTS_H


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




using namespace std;
using namespace cv;
//using namespace Concurrency;


namespace ADS {




// =============================================
// Вложенные структуры статусов оборудования
// =============================================



struct StateInformation    // информация о состоянии
{
    int SecondaryPowerSupply;   // вторичное питание
    int ConnectToCams;          // соединение с камерами
    int ImageAquisition;        // получение изображения
    int SuccessfulWork;         // статус успешной работы
    int FuctSuccessfulWork;    // факт успешной работы
};



struct t_vuxyzrgb     // Структура в формате std:vector
{
    int num_p;                         // Количество точек
    int num_cl;                        // Количество кластеров
    vector<int> num_p_cl;              // Количесво точек в кластере

    int SelectedClaster;               // Номер кластера, выбранного оператором

    // до кластеризаци сразу полсе обнаружения 3D точек
    int num_p0;                         // Количество точек
    vector<vector<int>> vu0;            // 2D координаты точки на изображении
    vector<vector<double>> xyz0;        // 3D координаты точки на пространсве
    vector<vector<int>> rgb0;           // цвет 3D точки


    // после кластеризации
    vector<vector<int>> vu;            // 2D координаты точки на изображении
    vector<vector<double>> xyz;        // 3D координаты точки на пространсве
    vector<vector<int>> rgb;           // цвет точки
    vector<int> cluster;               // Для каждой точки номер кастера

    vector<vector<size_t>> clusters;   // Для каждого кластера номер точки
    vector<vector<int>> objRect2D;       // Координаты рамки выделяющей объект на изображении

    vector<vector<double>> objRect3D;       // Координаты рамки выделяющей объект в 3D пространсве

    vector<double> distance2Point3D;      // координат 2-х выбранных точек и рассояние между ними



    // distance2Point3D_temp = {xyz01[0], xyz01[1], xyz01[2], xyz02[0], xyz02[1], xyz02[2], Distance};
    // {vu_min.x, vu_min.y, vu_max.x, vu_max.y, Center2D.x, Center2D.y, radius2D}
    // {xyz_min.x, xyz_min.y, xyz_min.z, xyz_max.x, xyz_max.y, xyz_max.z, Center3D.x, Center3D.y,Center3D.z, radius3D }

    //=================================================
    // Координаты в системе координат робота
    //=================================================

    // Матрица трансформации для перевода из координат камеры в кординаты робота
    cv::Mat RTmatrix_cam2robot_cv = cv::Mat(4, 4, CV_64F);

    // Матрица вращения для пересчета координат при поворете камеры поворотным устройсвтвом
    cv::Mat RTmatrix_camRotation_cv = cv::Mat(4, 4, CV_64F);



    vector<vector<double>> objRect3D_robot_vec;       // Координаты рамки выделяющей объект в 3D пространсве

    //=================================================

    // для храниения, какого вида примитив у объекта
    // 0 - не определено
    // 1 - плоскость
    // 2 - сфера
    // 3 - цилиндр

    int num_prim;                 // количесво примитивов, должно совпадать с num_cl, введено так как иногда не совпадаес из-за рассинхронизации
    vector<int> PrimitiveDetect; //  виды примитивов объектов (всех кластеров)
    vector<vector<double>> PrimitiveParameter; //  параметры модели привитивов (всех кластеров)
    vector<vector<Point3d>>PrimitivePoints; // векотор точек плоскоси (4 шт) и цилиндра (30 шт) для прорисовки их на изображении


    cv::Mat xyz_Disparity_AllPoints; // Координаты всех найденных точек в виде изображения uv по слоям xyz

    cv::Size imgSize; // Размер изображения

    vector<double> limit_WorkingArea;   // Границы рабоче зоны для стереокамры СТЗ, за её пределами ничего не видит



    //=================================================
    // Габариты объекта
    //=================================================
    double ObjSize_circumscribedLength = -1;
    double ObjSize_circumscribedWidth = -1;
    double ObjSize_circumscribedHeight = -1;
    double ObjSize_ObjLength = -1;
    double ObjSize_ObjWidth = -1;      //


};

struct type_arma_vuxyzrgb     // Структура в Armadillo формате
{
    int num_p;                                  // Количество точек
    int num_cl;                                 // Количество кластеров
    int SelectedClaster;                        // Номер кластера, выбранного оператором

    arma::Col<int> num_p_cl;                    // Количесво точек в кластере

    arma::Mat<int> vu;                          // 2D координаты точки на изображении
    arma::Mat<double> xyz;                      // 3D координаты точки на пространсве  // dmat = Mat<double>
    arma::Mat<int> rgb;                         // цвет точки
    arma::Col<int> cluster;                     // Для каждой точки номер кастера

    // Для каждого кластера соответсвующая матрица с: ..
    arma::field<arma::Mat<int>> clusters_indx;  // Для каждого кластера номера точек
    arma::field<arma::Mat<int>> clusters_vu;    // Для каждого кластера 2D координаты на изображении
    arma::field<arma::Mat<double>> clusters_xyz; // Для каждого кластера 3D координаты точки в пространсве
    arma::field<arma::Mat<int>> clusters_rgb;    // Для каждого кластера цвет точки


    arma::Mat<int> objRect2D;       // Координаты рамки выделяющей объект на изображении
    arma::Mat<double> objRect3D;       // Координаты рамки выделяющей объект в 3D пространсве


    //=================================================
    // Координаты в системе координат робота
    //=================================================

    // Матрица трансформации для перевода из координат камеры в кординаты робота

    arma::Mat<double> RTmatrix_cam2robot_arma;
    // arma::mat RTmatrix_cam2robot_arma;
    // Матрица вращения для пересчета координат при поворете камеры поворотным устройсвтвом
    arma::Mat<double> RTmatrix_camRotation_arma;
    //arma::mat RTmatrix_camRotation_arma;

    arma::Mat<double> objRect3D_robot_arma;       // Координаты рамки выделяющей объект в 3D пространсве





};



}



#endif // H_DENCESTEREOSTRUCTS_H
