#ifndef F02_RULER_H
#define F02_RULER_H

// #include <QDockWidget>
#include <QWidget>

#include <QTime>
#include <QDateTime>
#include <vector>
#include <QTimer>
#include <QMessageBox>

#include <cl_LOG.h> // Пренесён в form01_mainSettings.h   из-за error: LNK2005: already defined in cl_LOG.h


namespace Ui {
class f02_Ruler;
}

//class f02_Ruler : public QDockWidget
class f02_Ruler : public QWidget
{
    Q_OBJECT

        public:
                 explicit f02_Ruler(QWidget *parent = nullptr);
    ~f02_Ruler();

private:
    Ui::f02_Ruler *ui;


    QGraphicsPixmapItem G_pixmap;  // для прорисовки
    cv::Mat G_frame;               // для прорисовки



signals:


    void signal_sendText_plainTextEdit_StateInfo(QString text); // сигнал для отправки текста на главную форму

    void signal_RulerChangingDrawing();                         // сигнал смены прорисовки


public slots:

    void slot_MainFormToF02_Ruler(ADS::cl_DenseStereo* obj_ptr, str_Settings* settings_ptr); // Слот для принятия данных с главной формы

    void slot_StartRuler(); // Слот для сигнала старта измерений  на главной форме

    void slot_RulerChangingDrawing(); // Слот для смены прорисовки


    // Для кнопки закрытия окна
    void closeEvent (QCloseEvent *event);


public:

    ADS::cl_DenseStereo * G_obj_ptrf;    // указатель оъект класса
    ADS::cl_DenseStereo * G_obj_ptrf_temp = new  ADS::cl_DenseStereo; // копия на объекта класса

    // ADS::t_vuxyzrgb * vuxyzrgb_copy = new ADS::t_vuxyzrgb; // указатель на переменную числовых результатов

    str_Settings *G_settings_ptrf;  // указатель на структуру настроек
    bool G_exitCode;

private slots:
    void on_pushButton_Stop_clicked();


    void mousePressEvent(QMouseEvent *event);



    void on_pushButton_Reload_clicked();
};

#endif // F02_RULER_H
