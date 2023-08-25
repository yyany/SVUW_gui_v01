#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// =======

//#include "cl_VideoStreamer.h" // ошибка  error: C2011: 'sockaddr' подключается в func_DenseStereo.h


// ======

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QDate>
#include <QMessageBox>

#include <QCamera>




// Заголовочные файлы других окон
#include <form01_mainSettings.h>
#include <f02_ruler.h>
// #include <cl_LOG.h>  // Пренесён в form01_mainSettings.h   из-за error: LNK2005: already defined in cl_LOG.h
//#include "func_main.h" // закоментирована так как уже есть в cl_LOG.h    из-за error: LNK2005: already defined in cl_LOG.h
// ======


//// Заголовочные файлы других окон QTFormApp_3DRuler (В.В.Альчаков)
//#include "QTFormApp_3DRuler/camerascene.h" // ошибка  error: C2011: 'sockaddr' подключается в func_DenseStereo.h
//#include "QTFormApp_3DRuler/toolwindow.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:

    void hideView();
    void showView();


    // Не слоты (потом нужно перенести) !!!
    void showImg_Cycle();
    void treadFunc_Stating();
    void FuncCam_Stat();
    void FuncCam_Stop();
    vector<int> string_to_vecint(string);         //  Функция преобразования строки в вектор (сделано для ввода алгоритмов в потоки)
    std::string vecint_to_string(vector<int> v);  // Функция перевода вектора int в стороку
    // ============


     void slot_writeText_plainTextEdit_StateInfo(QString text); // слот для прорисовки текста на форму

     void slot_ApplyStartCommand(); // слот принятия сигнала от TCP сервера к главной форме для старта программы по команде от стороннего TCP клиента



private slots:


    void mousePressEvent(QMouseEvent *event);

    void on_checkBox_05_released();
    void on_checkBox_04_released();
    void on_checkBox_03_released();
    void on_checkBox_02_released();
    void on_checkBox_01_released();
    void on_checkBox_00_released();


    void on_pushButton_OpenFile1L_released();
    void on_pushButton_OpenFile2R_released();
    void on_pushButton_OpenCalibr_released();

    void on_pushButton_WriteExp_released();
    void on_checkBox_WriteSourceVideo_released();
    void on_pushButton_WriteOneStereoPair_released();

    void on_pushButton_Start_released();

    void on_pushButton_Stop_released();

    void on_comboBoxMain_ModeWork_currentIndexChanged(int index);

    void on_checkBox_06_released();
    void on_checkBox_07_released();
    void on_checkBox_08_released();
    void on_checkBox_09_released();
    void on_checkBox_10_released();
    void on_checkBox_11_released();

    void on_comboBoxMain_SelectCam_currentIndexChanged(int index);

    void on_pushButton_Settings_released();


    void TimerSlot_fMain(); // слот таймера для строки статуса



    // сигналы на главную форму для отправки сообщениий по TCP
//    void slot_F01ToMF_Send_StateInformation_pressed();
//    void slot_F01ToMF_Send_MomentOjectDetection_pressed();



    void on_comboBox_CamType_currentIndexChanged(int index);
    void on_checkBox_WiriteLogAirCraft3DCoordinates_released();
    void on_pushButton_WiriteLogAirCraft3DCoordinates_pressed();
    void on_spinBox_LogDataWriteSpeed_Aircraft_valueChanged(int arg1);
    void on_checkBox_WriteResultVideo_released();


    // Для кнопки закрытия окна
    void closeEvent (QCloseEvent *event);


    void on_lineEdit_OpenFile1L_editingFinished();

    void on_lineEdit_OpenFile2R_editingFinished();

    void on_comboBoxMain_ImgType_currentIndexChanged(int index);

    void on_checkBox_setWarterCalibr_clicked();

    void on_pushButton_ResultVideoStream_clicked();

    void on_lineEdit_SNCam01_editingFinished();

    void on_lineEdit_SNCam02_editingFinished();

    void on_pushButton_RulerStart_clicked();

    void on_spinBox_VideoStreamQualiy_valueChanged(int arg1);

    void on_pushButton_ResultVideoStream_2_clicked();

    void on_spinBox_timeMinThreadDela_00_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDelay_01_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDela_02_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDela_03_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDela_04_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDela_05_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDelay_06_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDelay_07_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDelay_08_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDelay_09_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDelay_10_valueChanged(int arg1);

    void on_spinBox_timeMinThreadDelay_11_valueChanged(int arg1);

    void on_pushButton_aboutQt_clicked();

    void on_pushButton_DarkNet_clicked();

    void on_pushButton_Start3DRuler_clicked();

signals:

    void signal_MainFormToForm01(ADS::cl_DenseStereo* obj_ptr, str_Settings* settings_ptr); // отправка главных объектов в на форму настроек
    void signal_MainFormToLOG(ADS::cl_DenseStereo* obj_ptr, str_Settings* settings_ptr); // отправка главных объектов в LOG
    void MainFormToF02_Ruler(ADS::cl_DenseStereo* obj_ptr, str_Settings* settings_ptr); // отправка главных объектов в на форму измерение линейкой



    void WiriteLogAirCraft3DCoordinates(); // создание и запись лог-файла

    void signal_StopAllTCP();    // Сигнал остановки TCP для отключения всей программы
    void signal_StartAllTCP();    // Сигнал остановки TCP для отключения всей программы
    void signal_StartAuroraLantern();    // Включение фонаря (при нажании кнопки старт программы)
    void signal_StopAuroraLantern();


    void signal_SendStartRuler(); // Сигнал для старта измерения линейкой

    void signal_MainFormToLOG_AirCraftData(            // сиглнал для принятия данных с AirCraft
        int counter_AirCraft,
        double x_AirCraft,
        double y_AirCraft,
        double z_AirCraft,
        double x_GlidePath,
        double y_GlidePath,
        double z_GlidePath,
        double dx_AirCraft,
        double dy_AirCraft,
        double dz_AirCraft,
        double ro_AirCraft,
        double theta_AirCraft,
        double phi_AirCraft,
        double ro_GlidePath,
        double theta_GlidePath,
        double phi_GlidePath,
        double dro_AirCraft,
        double dtheta_AirCraft,
        double dphi_AirCraft);

    void signal_sendText_plainTextEdit_StateInfo(QString text); // сигнал для отправки текста


private:
    Ui::MainWindow *ui;

    // ToolWindow *toolWindow;

    form01_mainSettings *form01_mainSettings01; // объект класса form01_mainSettings, обязательно динамический
    cl_LOG *obj_LOG_01;                        // объект класса LOG, обязательно динамический
    f02_Ruler *objp_f02_Ruler;                   // объект класса f02_Ruler, обязательно динамический


    bool G_flag_DarkNet = false;                         // флаг для переключения значения кнопки: true - запущено false - остановлено
    bool G_flag_VideoStream = false;                         // флаг для переключения значения кнопки: true - запущено false - остановлено
    bool flag_StartButton = false;                           // флаг для переключения значения кнопки: true - запущено false - остановлено
    bool flag_WiriteLogAirCraft3DCoordinates = false;        // флаг для переключения значения кнопки: true - запущено false - остановлено
    bool autoStart_WiriteLogAirCraft3DCoordinates  = false;   // Авто запуск записи лог файла Airfield



    // =====================================
    // Для определения слудующего самолёта
    // =====================================
    int counter_AirCraft = 0;      // переменная для номера самёта (если по z произошел большой разрыв)
    double z_AirCraft_prev = -77888; // предудущая координата самолёта по z
    double y_AirCraft_prev = -77888; // предудущая координата самолёта по y
    double x_AirCraft_prev = -77888; // предудущая координата самолёта по x

    // коэффициенты параметрического уравнения кривой для расчёта глиссады
    double ax = - 0.065599437857381;
    double ay = - 0.084760701959871;
    double az = 1;
    double bx = - 0.065599437857381;
    double by = 170.9628759316813;
    double bz = 0;



    // Таймер для главного окна
    QTimer *timer_Main; // Объект класса QTimer
    int timer_Main_dsec, timer_Main_sec, timer_Main_min, timer_Main_hour;   // переменная для отчсчёта времени

    // Таймер для записи в лог файл по времени
    QTimer *timer_LOG_AirCraft;     // Объект класса QTimer
    int timer_value_LOG_AirCraft;   // переменная для отчсчёта времени


};
#endif // MAINWINDOW_H
