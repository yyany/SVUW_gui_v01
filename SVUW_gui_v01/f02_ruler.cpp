#include "f02_ruler.h"
#include "ui_f02_ruler.h"

f02_Ruler::f02_Ruler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::f02_Ruler)
{
    ui->setupUi(this);
    ui->graphicsView_RulerFame->setScene(new QGraphicsScene(this)); // Необходимо для графического окна

    // =================================
    // Соеденение сигнала и слота для смены прорисовки после клика мыши
    // =================================
    connect(this, &f02_Ruler::signal_RulerChangingDrawing,
            this, &f02_Ruler::slot_RulerChangingDrawing);
    // =================================


}

f02_Ruler::~f02_Ruler()
{
    delete ui;
}

void f02_Ruler::slot_MainFormToF02_Ruler(ADS::cl_DenseStereo *obj_ptr_slot, str_Settings *settings_ptr_slot)
{


    G_obj_ptrf = obj_ptr_slot;
    G_settings_ptrf = settings_ptr_slot;




}

void f02_Ruler::slot_StartRuler()
{

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    emit signal_sendText_plainTextEdit_StateInfo( " Start Ruler"); // отправка текста в информационное окно
    // >>>>>>>>>>>>>>>>>>>>>>

    //    ADS::cl_DenseStereo* obj_ptrf_temp = new ADS::cl_DenseStereo;
    //    obj_ptrf_temp = &(*G_obj_ptrf);


//    ADS::t_vuxyzrgb vuxyzrgb_temp = G_obj_ptrf->get_vuxyzrgb();
//    G_obj_ptrf_temp->set_vuxyzrgb ( vuxyzrgb_temp );


   // copy_vuxyzrgb(G_obj_ptrf, vuxyzrgb_copy);


    G_obj_ptrf_temp->set_vuxyzrgb ( G_obj_ptrf->get_vuxyzrgb() );


    // !!!!!!!!! В cl_DenseStereo.h если clone() то тормоза, скорее всего из за double в  cv::Mat всесто uint8
    G_obj_ptrf_temp->vuxyzrgb.xyz_Disparity_AllPoints = G_obj_ptrf->vuxyzrgb.xyz_Disparity_AllPoints.clone();

    G_obj_ptrf_temp->setWorkingAria(G_settings_ptrf->limit_WorkingArea);

    vector<cv::Mat> imgs = G_obj_ptrf->getImgOpenCV_StereoPair();
    G_obj_ptrf_temp->setImgOpenCV_StereoPair( imgs[0], imgs[1] );

    G_obj_ptrf_temp->setImgOpenCV_1left(  G_obj_ptrf->getImgOpenCV_1left() );
    G_obj_ptrf_temp->setImgOpenCV_2right(  G_obj_ptrf->getImgOpenCV_2right() );
    G_obj_ptrf_temp->setImgColorPreProcess_StereoPair(  G_obj_ptrf->getImgColorPreProcess_StereoPair() );

    G_obj_ptrf_temp->setResultImage ( G_obj_ptrf->getResultImage()  );

    G_exitCode = true;


    f02_Ruler::slot_RulerChangingDrawing();



    // ========================
    //
    // ========================
    // =====================================
    //  Вывод изображений на экран
    // =====================================
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



//    QGraphicsPixmapItem pixmap;
//    cv::Mat frame;

////    int selectorImage = 13;
////    int idx = 1; // пояснение выше


//    do {

//        // frame =  getImg02(obj_ptrf_temp, G_settings_ptrf, idx, selectorImage);

//        // Обработчик событий от мыши
//        // MouseClickSelector - Парамет выбора способа ввода кординат клика мыши
//        // 0 - ввод через окно OpenCV
//        // 1 - ввод из стороннего источника



//       ADS::t_vuxyzrgb vuxyzrgb_temp22 = G_obj_ptrf_temp->get_vuxyzrgb();

//        G_obj_ptrf_temp->eventMouse_SelectObject(G_settings_ptrf->MouseClickSelector);
//        G_obj_ptrf_temp->AddResultsToDisparityMap03();
//        frame = G_obj_ptrf_temp->getDisparityMapImage03();

//        // добавлено как альтернатива  qApp->processEvents(); можно убрать синнонимы
//        QApplication::processEvents();


//        QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
//        pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
//        // pixmap.setPos(-9, -37);


//        ui->graphicsView_RulerFame->fitInView(&pixmap, Qt::KeepAspectRatio);
//        ui->graphicsView_RulerFame->scene()->addItem(&pixmap);


//        // qApp->processEvents();



//        // =====================================
//        // Вывод в таблицу координат 2-х выбранных точек и рассояние между ними
//        // =====================================
//        if (!vuxyzrgb_temp22.distance2Point3D.empty())
//        {
//            ui->tableWidget_RulerPoints->setItem(0,0, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(0))));
//            ui->tableWidget_RulerPoints->setItem(0,1, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(1))));
//            ui->tableWidget_RulerPoints->setItem(0,2, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(2))));
//            ui->tableWidget_RulerPoints->setItem(1,1, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(3))));
//            ui->tableWidget_RulerPoints->setItem(1,1, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(4))));
//            ui->tableWidget_RulerPoints->setItem(1,2, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(5))));
//            ui->tableWidget_RulerPoints->setItem(1,3, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(6))));
//        }




//    } while (G_exitCode);


}

void f02_Ruler::slot_RulerChangingDrawing()
{





    // ========================
    //
    // ========================
    // =====================================
    //  Вывод изображений на экран
    // =====================================
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



//    QGraphicsPixmapItem pixmap;
//    cv::Mat frame;

//    int selectorImage = 13;
//    int idx = 1; // пояснение выше




        // frame =  getImg02(obj_ptrf_temp, G_settings_ptrf, idx, selectorImage);

        // Обработчик событий от мыши
        // MouseClickSelector - Парамет выбора способа ввода кординат клика мыши
        // 0 - ввод через окно OpenCV
        // 1 - ввод из стороннего источника



       ADS::t_vuxyzrgb vuxyzrgb_temp22 = G_obj_ptrf_temp->get_vuxyzrgb();

        G_obj_ptrf_temp->eventMouse_SelectObject(G_settings_ptrf->MouseClickSelector);
        G_obj_ptrf_temp->AddResultsToDisparityMap03();
        G_frame = G_obj_ptrf_temp->getDisparityMapImage03();

        // добавлено как альтернатива  qApp->processEvents(); можно убрать синнонимы
        QApplication::processEvents();


        QImage qimg(G_frame.data, G_frame.cols, G_frame.rows, G_frame.step, QImage::Format_RGB888);
        G_pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
        // pixmap.setPos(-9, -37);


        ui->graphicsView_RulerFame->fitInView(&G_pixmap, Qt::KeepAspectRatio);
        ui->graphicsView_RulerFame->scene()->addItem(&G_pixmap);


        // qApp->processEvents();



        // =====================================
        // Вывод в таблицу координат 2-х выбранных точек и рассояние между ними
        // =====================================
        if (!vuxyzrgb_temp22.distance2Point3D.empty())
        {
            ui->tableWidget_RulerPoints->setItem(0,0, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(0))));
            ui->tableWidget_RulerPoints->setItem(0,1, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(1))));
            ui->tableWidget_RulerPoints->setItem(0,2, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(2))));
            ui->tableWidget_RulerPoints->setItem(1,1, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(3))));
            ui->tableWidget_RulerPoints->setItem(1,1, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(4))));
            ui->tableWidget_RulerPoints->setItem(1,2, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(5))));
            ui->tableWidget_RulerPoints->setItem(1,3, new QTableWidgetItem(QString::number(vuxyzrgb_temp22.distance2Point3D.at(6))));
        }





}

void f02_Ruler::closeEvent(QCloseEvent *event)
{

    G_exitCode = false;


    QString text = "Закрытm окно ??? ";

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Close Event", text,
                                                              QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No) return;




}

void f02_Ruler::on_pushButton_Stop_clicked()
{

      G_exitCode = false;

    emit QCloseEvent();

}







void f02_Ruler::mousePressEvent(QMouseEvent *event)
{

    // Detect if the click is in the view.
    // обработка клика мыши


    QPoint remapped = ui->graphicsView_RulerFame->mapFromParent( event->pos() );
    QPointF mousePoint;
    if ( ui->graphicsView_RulerFame->rect().contains( remapped ) )
    {
        // mousePoint = ui->graphicsView_RulerFame->mapToScene( remapped.x()-9, remapped.y()-37 );

        mousePoint = ui->graphicsView_RulerFame->mapToScene( remapped.x(), remapped.y() );

        //        QTableWidgetItem *twi = new QTableWidgetItem;
        //        twi->setText(QString::number(mousePoint.x()));
        //        ui->tableWidget_TwoPoint->setItem(0,1, twi);



        G_obj_ptrf_temp->setMouseClickCoordinates02( mousePoint.x(), mousePoint.y() );


#if defined(_WIN64)
        // ################################
        Sleep(40);
        // ################################
#else
        // ################################
        usleep(40*1000);
        // ################################
#endif



    ui->tableWidget_RulerPoints->setItem(0,4, new QTableWidgetItem(QString::number(  mousePoint.x() )));
    ui->tableWidget_RulerPoints->setItem(1,4, new QTableWidgetItem(QString::number(  mousePoint.y() )));



    // =================================
    // Отсылаем  сигнала для смены прорисовки после клика мыши
    emit signal_RulerChangingDrawing();
    // =================================


    }



    // Код для расчета размера выводимого на экран изображения
    //    int v = ui->graphicsView01->height();
    //    int u = ui->graphicsView01->width();

    //    if(v*1.6 < u)
    //    {
    //        u = v*1.6 - 8;
    //    }
    //    else
    //    {
    //        v = u/1.6 - 8;
    //    }







}


void f02_Ruler::wheelEvent(QWheelEvent *event)
{
//    if (event->modifiers() & Qt::ControlModifier)
//    {
//        // zoom
//        const QGraphicsView::ViewportAnchor anchor = ui->graphicsView_RulerFame->transformationAnchor();
//        ui->graphicsView_RulerFame->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//        int angle = event->angleDelta().y();
//        qreal factor;
//        if (angle > 0)
//        {
//            factor = 1.1;
//        }
//        else
//        {
//            factor = 0.9;
//        }
//        ui->graphicsView_RulerFame->scale(factor, factor);
//        ui->graphicsView_RulerFame->setTransformationAnchor(anchor);


//    }
//    else
//    {
//        wheelEvent(event);
//    }

// =====================
// Без if
// =====================

    // zoom
    const QGraphicsView::ViewportAnchor anchor = ui->graphicsView_RulerFame->transformationAnchor();
    ui->graphicsView_RulerFame->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    int angle = event->angleDelta().y();
    qreal factor;
    if (angle > 0)
    {
        factor = 1.1;
    }
    else
    {
        factor = 0.9;
    }
    ui->graphicsView_RulerFame->scale(factor, factor);
    ui->graphicsView_RulerFame->setTransformationAnchor(anchor);

}


void f02_Ruler::on_pushButton_Reload_clicked()
{
    f02_Ruler::slot_StartRuler();
}

