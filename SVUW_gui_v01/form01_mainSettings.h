#ifndef FORM01_MAINSETTINGS_H
#define FORM01_MAINSETTINGS_H

#include <QWidget>
#include <QTime>
#include <QDateTime>
#include <vector>
#include <QTimer>
#include <QMainWindow>
// QSerialPort
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTest> // Qt Test - это платформа для модульного тестирования приложений и библиотек на основе Qt.

// function
// =======
// #include "func_main.h" // уже есть в cl_LOG.h
#include <cl_LOG.h> // Пренесён в form01_mainSettings.h   из-за error: LNK2005: already defined in cl_LOG.h
// ======


namespace Ui {
class form01_mainSettings;
}

class form01_mainSettings : public QWidget
{
    Q_OBJECT

        public:

                 explicit form01_mainSettings(QWidget *parent = nullptr);
    ~form01_mainSettings();

    void ChangeHSVFindColor();
    void SwitchAuroraLantern(int value, string COMport);

    int G_AuroraLantern_SerialPort_value; // Значение яркости фонаря подаваемое на SerialPort

signals:
    // сигналы на главную форму для отправки сообщениий по TCP
//    void signal_F01ToMF_Send_StateInformation_pressed();
//    void signal_F01ToMF_Send_MomentOjectDetection_pressed();

    // Сигнал отправки на TCP Server ссылок на объект класса и ссылки на стуктуру параметров настройки
   // void signal_Form01ToTcpServer(ADS::cl_DenseStereo*, str_Settings*);

    void signal_sendText_plainTextEdit_StateInfo(QString text); // сигнал для отправки текста на главную форму

    void signal_Send_ObjectDetectionCompressedImg(); // сигнал для запуска отправки изображения по сигналу, только в случаи обнаружения объекта



public slots:

    void slot_MainFormToForm01(ADS::cl_DenseStereo* obj_ptr, str_Settings* settings_ptr); // Слот для принятия данных с главной формы
    void slot_TcpServetToForm_Receive(); // Слот для обработки сигнала от TCP Server

    void TimerSlot_f01();             // Слот запускает таймера на форме
    void slot_Initiate_timerTCP();   // Слот запускает таймер для переодической отправки сообщений по TCP

    void slot_StopAllTCP();   // Слот остановки TCP для отключения всей программы
    void slot_StartAllTCP();   // Слот запуска TCP для отключения всей программы
    void slot_StartAuroraLantern();   // Включение фонаря (при нажании кнопки старт программы)
    void slot_StopAuroraLantern();

    void on_checkBox_Send_ObjectDetectionImgFile_clicked();
    void on_checkBox_Send_ObjectDetectionCompressedImg_clicked();


    void on_checkBox_StartTcpClient_clicked();                // старт TCP клиента


    void on_checkBox_Send_InitialStateInformation_clicked();  // отправка информация о начальном состоянии (только в начале работы)
    void on_checkBox_Send_StatusStateInformation_clicked();        // отправка информация о состоянии (в текущий момент времени)
    void on_checkBox_Send_MomentOjectDetection_clicked();     // отправка информация об обнаружении объкта (в момент обнаружения)

    void on_checkBox_StartTcpServer_clicked();     // старт TCP сервера
    void on_checkBox_Receive_OutsideInformationANPA_clicked();



private slots:
    void on_spinBox_LowHSV_h_valueChanged(int arg1);
    void on_spinBox_LowHSV_s_valueChanged(int arg1);
    void on_spinBox_LowHSV_v_valueChanged(int arg1);
    void on_spinBox_HieghHSV_h_valueChanged(int arg1);
    void on_spinBox_HieghHSV_s_valueChanged(int arg1);
    void on_spinBox_HieghHSV_v_valueChanged(int arg1);
    void on_spinBox_smbNumDisparities_valueChanged(int arg1);


    void on_spinBox_X_Low_valueChanged(int arg1);
    void on_spinBox_Y_Low_valueChanged(int arg1);
    void on_spinBox_Z_Low_valueChanged(int arg1);
    void on_spinBox_X_Up_valueChanged(int arg1);
    void on_spinBox_Y_Up_valueChanged(int arg1);
    void on_spinBox_Z_Up_valueChanged(int arg1);

    void on_spinBox_smbBlockSize_valueChanged(int arg1);
    void on_spinBox_smbPreFilterCap_valueChanged(int arg1);
    void on_spinBox_smbMinDisparity_valueChanged(int arg1);
    void on_spinBox_smbTextureThreshold_valueChanged(int arg1);
    void on_doubleSpinBox_coef_pow_dbs_valueChanged(double arg1);
    void on_doubleSpinBox_coef_base_dbs_valueChanged(double arg1);

    void on_spinBox_smbUniquenessRatio_valueChanged(int arg1);
    void on_spinBox_smbSpeckleWindowSize_valueChanged(int arg1);
    void on_spinBox_smbSpeckleRange_valueChanged(int arg1);
    void on_spinBox_smbDisp12MaxDiff_valueChanged(int arg1);
    void on_spinBox_filter_ClusterPoints_valueChanged(int arg1);
    void on_spinBox_dbsNeighbors_valueChanged(int arg1);
    void on_spinBox_dbsRad_valueChanged(int arg1);

    void on_comboBox_MetodDisparity_currentIndexChanged(int index);

    void on_checkBox_PointCorrectinForClastering_stateChanged(int arg1);
    void on_comboBox_ImagePreProcessing_currentIndexChanged(int index);
    void on_checkBox_PointCorrectinForClastering_pressed();

    void on_spinBox_limit_points_valueChanged(int arg1);



    void on_lineEdit_TCPhostIP_C_editingFinished();

    void on_lineEdit_TCPhostPort_C_editingFinished();

    void on_lineEdit_AuroraLogPath_editingFinished();

    void on_lineEdit_AuroraFilesPath_editingFinished();


    void on_checkBox_SwitchAuroraLantern_clicked(bool checked);

    void on_spinBox_SwitchAuroraLantern_editingFinished();

    void on_checkBox_ShowMarkersImgRTK_00_clicked();

    void on_checkBox_ShowMarkersImgRTK_01_clicked();

    void on_checkBox_ShowMarkersImgRTK_04_clicked();

    void on_checkBox_ShowMarkersImgRTK_05_clicked();

    void on_checkBox_ShowMarkersImgRTK_06_clicked();

    void on_checkBox_ShowMarkersImgRTK_07_clicked();

    void on_checkBox_ShowMarkersImgRTK_02_clicked();

    void on_radioButton_ShowMarkersImgRTK_02_1_clicked();

    void on_radioButton_ShowMarkersImgRTK_02_2_clicked();

    void on_checkBox_ShowMarkersImgRTK_03_clicked();

    void on_radioButton_ShowMarkersImgRTK_03_1_clicked();

    void on_radioButton_ShowMarkersImgRTK_03_2_clicked();

private:
    Ui::form01_mainSettings *ui;



    // =====================================
    // Таймеры
    // =====================================

    QTimer *timer; // Объект класса QTimer
    int timer_value;   // переменная для отчсчёта времени


    QTimer *timer_TCP;
    int timer_value_TCP;


    int countStrSubtitle = 1; // Номер записи в субтитрах для видео файла

//    ADS::cl_DenseStereo *obj_ptrf; // указатель оъект класса
//    str_Settings *settings_ptrf;  // указатель на структуру настроек


public:




};

#endif // FORM01_MAINSETTINGS_H
