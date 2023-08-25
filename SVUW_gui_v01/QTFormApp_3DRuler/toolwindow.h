#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include <QDialog>
#include <Q3DScatter>

//#include <opencv2/opencv.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

#include "datastructure.h"

#include <algorithm>
#include <iterator>

#include "QTFormApp_3DRuler/camerascene.h"




namespace Ui {
class ToolWindow;
}

class ToolWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ToolWindow(cv::Mat image, ADS::t_vuxyzrgb data, QWidget *parent = nullptr);
    ~ToolWindow();

    enum ToolMode {Mode2D, Mode3D};
    void setMode(ToolMode mode);
    ToolMode getMode();

public slots:
    void updateInfoA(double X, double Y, double Z, double D);

private slots:
    void on_lswClusters_itemSelectionChanged();

    void on_btn2D_clicked();

    void on_btn3D_clicked();

    void on_btnSave_clicked();

    void on_btnDelete_clicked();

private:
    Ui::ToolWindow *ui;    
    cv::Mat source;
    cv::Mat destination;
    QImage imgcam;    
    ADS::t_vuxyzrgb allPoints;
    ADS::t_vuxyzrgb clusterPoints;

    ToolMode toolMode;

    Q3DScatter *graph3D;
    QScatter3DSeries *series3D;
    QWidget *container3D;

    std::vector<int> getClusterIDs(ADS::t_vuxyzrgb points);

    CameraScene *cameraScene;
    QRect originalSize;

    double geometryL;
    double geometryW;
    double geometryH;
    double geometryLength;
    double geometryWidth;
    double geometryDistance;

    double get_distance(Point3D p1, Point3D p2);
    size_t get_sum_count(std::vector<double> X,
                         std::vector<double> Y,
                         std::vector<double> Z,
                         Point3D MN, Point3D M0);

    double np_linalg_norm(std::vector<double> a);

    std::vector<double> cross(std::vector<double> a,
                              std::vector<double> b);

    std::vector<double> substr(std::vector<double> a,
                               std::vector<double> b);

    double lineseg_dist(std::vector<double> p,
                        std::vector<double> a,
                        std::vector<double> b);


    void get_sizes(ADS::t_vuxyzrgb data,     // Входной массив точек кластера
                   double* L,           // Выход - Длина
                   double* W,           // Выход - Ширина
                   double* H,           // Выход - Высота
                   double* Length,      // Выход - Длина осевой линии
                   double* Width,       // Выход - Ширина осевой линии
                   double* Distance);   // Выход - Расстояние до центра масс

};

#endif // TOOLWINDOW_H
