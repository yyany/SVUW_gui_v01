#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <vector>

#include <opencv2/opencv.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>


// TODO: Вынести в отдельный модуль
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
    Point3D(double x, double y, double z, std::string name) {
        X = x;
        Y = y;
        Z = z;
        Name = name;
    }
    // Метод для установки кол-ва точек
    void setNumberOfPoint(int numberofpoints)
    {
        NumberOfPoint = numberofpoints;
    }
    // Переопределяем оператор для возможности сортировки
    bool operator< (const Point3D& p) {
        return NumberOfPoint < p.NumberOfPoint;
    }
};


#endif // DATASTRUCTURE_H
