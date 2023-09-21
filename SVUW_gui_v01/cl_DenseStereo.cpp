#include "cl_DenseStereo.h"


// Armadilo
//#include <armadillo>

using namespace std;

ADS::cl_DenseStereo::cl_DenseStereo(): Basler::cl_ImGrabBaslerCam::cl_ImGrabBaslerCam()


{

    // ============
    // Инициализация некоторых переменных
    // ============



    vuxyzrgb_arma.SelectedClaster = -1;


    // Инициализация всех переменных
    vuxyzrgb.num_p = -1;
    vuxyzrgb.num_cl = -1;
    vuxyzrgb.num_p_cl = {-1};
    vuxyzrgb.SelectedClaster = -1;

    vuxyzrgb.num_p0 = -1;
    vuxyzrgb.vu0 = { {-1} };
    vuxyzrgb.xyz0 = { {-1} };
    vuxyzrgb.rgb0 = { {-1} };


    // Потом добавить еще другиие


    vuxyzrgb.PrimitiveParameter.resize(8);
    vuxyzrgb.distance2Point3D = {0,0,0, 0,0,0, 0};
    vuxyzrgb.imgSize = {img_Y, img_X};





    // добавлено для устранения ошибок

    vuxyzrgb.xyz_Disparity_AllPoints = cv::Mat::zeros(img_Y, img_X, CV_32F);



//    // Инициализация индикатора о том, что приметивы не найдены
//    vuxyzrgb.PrimitiveDetect = {-1} ;




    //=================================================
    // Матрицы трансформации для определения координат в системе координат робота
    //=================================================

    // инициализация матриц в arma
    vuxyzrgb_arma.RTmatrix_cam2robot_arma.set_size(4,4);
    vuxyzrgb_arma.RTmatrix_camRotation_arma.set_size(4,4);

    // Матрица трансформации для перевода из координат камеры в кординаты робота
    vector <vector<double>> RT_cam2robot;
    RT_cam2robot.resize(4);
    RT_cam2robot[0] = {1,  0,  0, 0};
    RT_cam2robot[1] = {0,  0, -1, 0};
    RT_cam2robot[2] = {0,  1,  0, 0};
    RT_cam2robot[3] = { 0, 0,  0, 1};


    // Матрица вращения для пересчета координат при поворете камеры поворотным устройсвтвом
    // будет менятся в зависимости от углов поворота
    vector <vector<double>> RT_camRotation;
    RT_camRotation.resize(4);
    RT_camRotation[0] = {1,  0,  0, 0};
    RT_camRotation[1] = {0,  1,  0, 0};
    RT_camRotation[2] = {0,  0,  1, 0};
    RT_camRotation[3] = {0,  0,  0, 1};



    for (int i =0; i<4 ; i++)
    {
        for (int j =0; j<4 ; j++)
        {
            vuxyzrgb.RTmatrix_cam2robot_cv.at<double>(i, j) = RT_cam2robot[i][j];
            vuxyzrgb.RTmatrix_camRotation_cv.at<double>(i, j) = RT_camRotation[i][j];

            //             vuxyzrgb_arma.RTmatrix_cam2robot_arma(i,j) = RT_cam2robot[i][j];
            //             vuxyzrgb_arma.RTmatrix_camRotation_arma(i,j) = RT_camRotation[i][j];

        }
    }



    // ДОБАВЛЯЕМ изменения во 2-ю структуру vuxyzrgb_3Dpoints
    vuxyzrgb_3Dpoints = vuxyzrgb;


}


void ADS::cl_DenseStereo::InitializeDrawWindows()
{
    // Запуст окна для вывода результатов
    cv::namedWindow("Window_imgResult01Left", cv::WINDOW_AUTOSIZE); // Создаем  окна
    //===============

    // Запуст окна для прорисовки приметивов
    // cv::namedWindow("Window_DrawPrimitive", cv::WINDOW_AUTOSIZE); // Создаем  окна
    //===============



#if defined(_WIN64)

    // ################################
    Sleep(50);
    // ################################

#else

    // ################################
    usleep(50*1000);
    // ################################

#endif


}


void ADS::cl_DenseStereo::setCalibration(string calibration_filename)
{
    cout<<"READ CALIBRATION FILES"<<endl;



    cv::FileStorage fs(calibration_filename, cv::FileStorage::READ);
    if( fs.isOpened() )
    {
        fs["M1"] >> M1;
        fs["D1"] >> D1;
        fs["M2"] >> M2;
        fs["D2"] >> D2;

        fs["imageSize"] >> imageSize;
        fs["rms"] >> rms;
        fs["avgErr"] >> avgErr;
        fs["nframes"] >> nframes;
        fs["num_Im"] >> num_Im;

        fs["E"] >> E;
        fs["F"] >> F;
        fs["R"] >> R;
        fs["T"] >> T;
        fs["R1"] >> R1;
        fs["R2"] >> R2;
        fs["P1"] >> P1;
        fs["P2"] >> P2;
        fs["Q"] >> Q;

        fs.release();

    }
    else {
        cout << "Error: ADS::cl_DenseStereo::setCalibration: Can not READ the extrinsics parameters\n";
        fs.releaseAndGetString();
    }




// Установка размера кадра из настроек по изменению размера
//    imageSize.height = img_Y;
//    imageSize.width = img_X;




    cv::stereoRectify(
        M1, D1, M2, D2, imageSize, R, T,
        R1, R2, P1, P2, Q,
        cv::CALIB_ZERO_DISPARITY,
        //0,
        -1,
        imageSize,
        &roi1, &roi2);

//    cv::stereoRectify(
//        M1, D1, M2, D2, imageSize, R, T,
//        R1, R2, P1, P2, Q,
//        0,
//        -1,
//        {0,0},
//        &roi1, &roi2);





    cv::Mat M1n, M2n;
// Ошибка закоментирована
//    M1n = M1.clone();
//    M2n = M2.clone();
      M1n = P1.clone();
      M2n = P2.clone();


    // Precompute map for cvRemap()
    cv::initUndistortRectifyMap( M1, D1, R1, M1n,
                                imageSize,
                                CV_16SC2 ,
                                map11, map12 );
    cv::initUndistortRectifyMap( M2, D2, R2, M2n,
                                imageSize,
                                CV_16SC2 ,
                                map21, map22 );

    // DBSCAN algorithm.
    //pyclustering::clst::dbscan dbs(rad, neighbors);


    // Инициализация координат мышы
    //    MouseClickCoordinates.resize(2);
    //    MouseClickCoordinates.at(0) = -1;
    //    MouseClickCoordinates.at(1) = -1;





#if defined(_WIN64)

    // ################################
    Sleep(500);
    // ################################

#else

    // ################################
    usleep(500*1000);
    // ################################

#endif



}


void ADS::cl_DenseStereo::FindPointsCheckerBoard()
{

    // Оригиналы изображений
    cv::Mat image01, image02;



    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ===========


    image01 = getImgOpenCV_1left();
    image02 = getImgOpenCV_2right();


    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ===========



    //    int nx = 9;
    //    int ny = 7;
    //    float squareSize =20.1;   // square size (mm))
    //{



    //    bool              useUncalibrated = false;
    //    bool              displayCorners   = true;
    //    bool              showUndistorted  = true;
    //    bool              isVerticalStereo = false; // horiz or vert cams



    //    int               i, j, lr, nframes, num_Im;
    //    int               N                = nx*ny;


    //    vector< cv::Point3f >          boardModel;
    //    vector< vector<cv::Point3f> >  objectPoints;
    //    vector< vector<cv::Point2f> >  points[2];
    //    vector< cv::Point2f >          corners[2];
    //    bool                           found[2]         = {false, false};
    //    cv::Size                       imageSize;







    //    for( i = 0; i < ny; i++ )
    //        for( j = 0; j < nx; j++ )
    //            boardModel.push_back(
    //                cv::Point3f((float)(i*squareSize), (float)(j*squareSize), 0.f));







    //        found[0] = cv::findChessboardCorners(
    //            image01,
    //            cv::Size(nx, ny),
    //            corners[lr],
    //            cv::CALIB_CB_ADAPTIVE_THRESH +
    //            cv::CALIB_CB_NORMALIZE_IMAGE +
    //            cv::CALIB_CB_FILTER_QUADS
    //            // cv::CALIB_CB_FAST_CHECK
    //            );

    //        if(found[0])
    //            cv::cornerSubPix(image01, corners[lr], cv::Size(11, 11), cv::Size(-1, -1),
    //                             cv::TermCriteria(cv::TermCriteria::EPS+cv::TermCriteria::MAX_ITER, 30, 0.1));






    //        if( displayCorners )
    //        {
    //            cout << buf << endl;
    //            cv::Mat cimg;
    //            cv::cvtColor( img, cimg, cv::COLOR_GRAY2BGR );
    //            // draw chessboard corners works for circle grids too

    //            cv::drawChessboardCorners(
    //                cimg, cv::Size(nx, ny), corners[lr], found[lr]
    //                );

    //            double Dx = 0.4;
    //            cv::resize(cimg, cimg, cv::Size(cimg.cols * Dx,cimg.rows * Dx), 0, 0, cv::INTER_LINEAR);

    //            cv::imshow( "Corners", cimg );
    //            cv::waitKey(100);

    //            //    if( (cv::waitKey(0)&255) == 27 ) // Allow ESC to quit
    //            //       exit(-1);
    //        }
    //        else

    //        if( lr == 1 && found[0] && found[1] )
    //        {
    //            objectPoints.push_back(boardModel);
    //            points[0].push_back(corners[0]);
    //            points[1].push_back(corners[1]);
    //        }





    //    }





}



void ADS::cl_DenseStereo::ImgRemaping()
{


    vector<cv::Mat> StereoPair = getImgOpenCV_StereoPair();

    cv::Mat imgLeft_temp, imgRight_temp;



    imgLeft_temp = StereoPair.at(0).clone();
    imgRight_temp = StereoPair.at(1).clone();






    if ( imgLeft_temp.empty() || imgRight_temp.empty() ) return;




// int qk = 0;
//    do
//    {
//        // Загрузка исходного изображения

//        imgLeft = StereoPair.at(0);
//        imgRight = StereoPair.at(1);



//        #if defined(_WIN64)
//                // ################################
//                Sleep(10);
//                // ################################
//        #else
//                // ################################
//                usleep(10*1000);
//                // ################################
//        #endif

//        qk++;


//    }while  ( (imgLeft.empty() || imgRight.empty() ) && qk < 1000 );




    imageSize = cv::Size(imgLeft_temp.cols,imgLeft_temp.rows);



    // Ректификация (выравнивание) изображения- Rectify
    cv::Mat img01remap_temp, img02remap_temp;
    cv::remap( imgLeft_temp, img01remap_temp, map11, map12,
              cv::INTER_LINEAR );
    cv::remap( imgRight_temp, img02remap_temp, map21, map22,
              cv::INTER_LINEAR );


    // NEWImageSize
   // imageSize = cv::Size(img01remap_temp.cols,img01remap_temp.rows);





    // ===========
    setImgRemap_StereoPair(img01remap_temp, img02remap_temp);
    // ===========




}



vector<cv::Mat> ADS::cl_DenseStereo::getImgRemap_StereoPair()
{

    vector<cv::Mat> StereoPair_temp;
    StereoPair_temp.resize(2);
    // ===========
    // Mutex (lock)
    imgRemap_mutex.lock();
    // ===========

    if (img01remap.empty() || img02remap.empty() )
    {


        img01remap =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(img01remap,
                    "getImgRemap:: Image is Empty",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);

        img02remap =  img01remap.clone();



        //                //=========================================
        //                // Вывод в окно OpenCV
        //                //=========================================
        //                double img_coeff = 0.45;
        //                cv::Mat image;


        //                cv::resize(HR_Img_, image,
        //                           cv::Size( double(HR_Img_.cols * img_coeff), double(HR_Img_.rows * img_coeff) ), 0, 0, cv::INTER_LINEAR);
        //                cv::imshow("01", image);
        //                cv::waitKey(10);


    }





    StereoPair_temp[0] = img01remap.clone();
    StereoPair_temp[1] = img02remap.clone();

    // ===========
    // Mutex (unlock)
    imgRemap_mutex.unlock();
    // ===========



    return StereoPair_temp;

}

void ADS::cl_DenseStereo::setImgRemap_StereoPair(cv::Mat img01remap_temp, cv::Mat img02remap_temp)
{
    // ===========
    // Mutex (lock)
    imgRemap_mutex.lock();
    // ===========

    img01remap = img01remap_temp.clone();
    img02remap = img02remap_temp.clone();

    // ===========
    // Mutex (unlock)
    imgRemap_mutex.unlock();
    // ===========

}






vector<cv::Mat> ADS::cl_DenseStereo::ImgPerProcessing_OneImg(cv::Mat img_temp, vector<int> metod_ImagePerProcessing)
{

    //==============================
    // Параметры для включения фильтров предобработки
    //==============================
    //
    //    0 - без обработки;
    //    1 - equalizeHist (Color RGB);
    //    2 - CLAHE (Color RGB);
    //    3 - CLAHE;
    //    4 - equalizeHist;




    if ( img_temp.empty() )
    {


        img_temp =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(img_temp,
                    "PerProcessing:: Image is Empty",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);





        //                //=========================================
        //                // Вывод в окно OpenCV
        //                //=========================================
        //                double img_coeff = 0.45;
        //                cv::Mat image;


        //                cv::resize(HR_Img_, image,
        //                           cv::Size( double(HR_Img_.cols * img_coeff), double(HR_Img_.rows * img_coeff) ), 0, 0, cv::INTER_LINEAR);
        //                cv::imshow("01", image);
        //                cv::waitKey(10);

        return  img_temp;
    }




    //    int qk = 0;
    //    do
    //    {


    //        #if defined(_WIN64)
    //                // ################################
    //                Sleep(10);
    //                // ################################
    //        #else
    //                // ################################
    //                usleep(10*1000);
    //                // ################################
    //        #endif

    //        qk++;
    //        //if(qk == 9) cout << "FindPointsADS: (qk == 9) " << endl;

    //    } while ( img_temp.empty() && qk < 1000 );


    cv::Mat gray_temp;

    bool flag_gray = false; // Флаг использования предобработки серего изображения
    // Если true то будет использовано серое изображение, а не перевод цветного в серое
    // Если false  то будет проведено перевод в серое cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);


    for (int q = 0 ; q < (int) metod_ImagePerProcessing.size(); q++)
    {


        switch ( metod_ImagePerProcessing.at(q) )
        {

        case 0:
        {



            break;
        }

        case 1:
        {

            // ===========
            // equalizeHist (Color RGB) эквализация цветного изображения
            // ============

            // Разделяем изображение на 3 канала (B, G и R)
            vector<Mat> bgr_planes;
            split( img_temp, bgr_planes );
            // Применяем выравнивание к гистограммам всех каналов
            equalizeHist(bgr_planes[0], bgr_planes[0]);
            equalizeHist(bgr_planes[1], bgr_planes[1]);
            equalizeHist(bgr_planes[2], bgr_planes[2]);
            // Объединяем выровненные каналы в выровненное цветное изображение
            merge(bgr_planes, img_temp );

            flag_gray = false;  // Если false  то будет проведено перевод в серое cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);
            break;
        }

        case 2:
        {

            // ===========
            // CLAHE (Color RGB) цветного изображения
            // ============

            cv::Ptr <cv::CLAHE> clahe = cv::createCLAHE(40,cv::Size(8, 8));

            //  Разделяем изображение на 3 канала (B, G и R)
            vector<Mat> bgr_planes;

            split( img_temp, bgr_planes );
            // Применяем выравнивание к гистограммам всех каналов
            clahe->apply(bgr_planes[0], bgr_planes[0]);
            clahe->apply(bgr_planes[1], bgr_planes[1]);
            clahe->apply(bgr_planes[2], bgr_planes[2]);
            // Объединяем выровненные каналы в выровненное цветное изображение
            merge(bgr_planes, img_temp );



            flag_gray = false;  // Если false  то будет проведено перевод в серое cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);
            break;
        }
        case 3:
        {

            if (!flag_gray)
            {
                // Convert the rectified images to grayscale images
                cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);
                flag_gray = true;
            }

            // ===========
            // Фильтр предобработки изображений: Contrast Limited Adaptive Histogram Equalization
            // ===========
            cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(40,cv::Size(8, 8));
            clahe->apply(gray_temp, gray_temp);

            break;
        }
        case 4:
        {
            if (!flag_gray)
            {
                // Convert the rectified images to grayscale images
                cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);
                flag_gray = true;
            }
            // ===========
            // Фильтр предобработки изображений: Contrast Limited Adaptive Histogram Equalization
            // ===========
            equalizeHist(gray_temp, gray_temp );

            break;
        }

        case 5:
        {
            if (!flag_gray)
            {
                // Convert the rectified images to grayscale images
                cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);
                flag_gray = true;
            }
            // ===========
            // Фильтр повышения резкости
            // ===========
            // sharpen image using "unsharp mask" algorithm

//            Mat blurred; double sigma = 1, threshold = 5, amount = 1;
//            GaussianBlur(my_img, blurred, Size(), sigma, sigma);
//            Mat lowContrastMask = abs(my_img - blurred) < threshold;
//            Mat sharpened = my_img*(1+amount) + blurred*(-amount);
//            my_img.copyTo(sharpened, lowContrastMask);

            break;
        }
        // default:




        } // switch



    } // for;



    if ( gray_temp.empty() )
    {

        cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);
        flag_gray = true;

    }




    // =====================================
    // Проверка
    // =====================================

    //    if (!gray01r.empty())
    //    {

    //        imshow("gray01r", gray01r);
    //        // ################################
    //        cv::waitKey(10);
    //        // ################################
    //    }



    return  {img_temp.clone(), gray_temp.clone()};



}





void ADS::cl_DenseStereo::ImgPerProcessing(vector<int> metod_ImagePerProcessing)
{



    //==============================
    // Параметры для включения фильтров предобработки
    //==============================
    //
    //    0 - без обработки;
    //    1 - equalizeHist (Color RGB);
    //    2 - CLAHE (Color RGB);
    //    3 - CLAHE;
    //    4 - equalizeHist;




    vector<cv::Mat> StereoPair = getImgRemap_StereoPair();

    cv::Mat img01Remap_temp, img02Remap_temp;


    img01Remap_temp = StereoPair.at(0).clone();
    img02Remap_temp = StereoPair.at(1).clone();



    if ( img01Remap_temp.empty() || img02Remap_temp.empty() ) return;





    cv::Mat gray01_temp, gray02_temp;

    bool flag_gray = false; // Флаг использования предобработки серего изображения
    // Если true то будет использовано серое изображение, а не перевод цветного в серое
    // Если false  то будет проведено перевод в серое cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);


    for (int q = 0 ; q < (int) metod_ImagePerProcessing.size(); q++)
    {


        switch ( metod_ImagePerProcessing.at(q) )
        {

        case 0:
        {



            break;
        }

        case 1:
        {

            // ===========
            // equalizeHist (Color RGB) эквализация цветного изображения
            // ============
            // 01
            // Разделяем изображение на 3 канала (B, G и R)
            vector<Mat> bgr_planes;
            split( img01Remap_temp, bgr_planes );
            // Применяем выравнивание к гистограммам всех каналов
            equalizeHist(bgr_planes[0], bgr_planes[0]);
            equalizeHist(bgr_planes[1], bgr_planes[1]);
            equalizeHist(bgr_planes[2], bgr_planes[2]);
            // Объединяем выровненные каналы в выровненное цветное изображение
            merge(bgr_planes, img01Remap_temp );

            // 02
            split( img02Remap_temp, bgr_planes );
            // Применяем выравнивание к гистограммам всех каналов
            equalizeHist(bgr_planes[0], bgr_planes[0]);
            equalizeHist(bgr_planes[1], bgr_planes[1]);
            equalizeHist(bgr_planes[2], bgr_planes[2]);
            // Объединяем выровненные каналы в выровненное цветное изображение
            merge(bgr_planes, img02Remap_temp );






            flag_gray = false;  // Если false  то будет проведено перевод в серое cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);
            break;
        }

        case 2:
        {

            // ===========
            // CLAHE (Color RGB) цветного изображения
            // ============

            cv::Ptr <cv::CLAHE> clahe = cv::createCLAHE(40,cv::Size(8, 8));

            //  Разделяем изображение на 3 канала (B, G и R)
            vector<Mat> bgr_planes;



            // 01
            split( img01Remap_temp, bgr_planes );
            // Применяем выравнивание к гистограммам всех каналов
            clahe->apply(bgr_planes[0], bgr_planes[0]);
            clahe->apply(bgr_planes[1], bgr_planes[1]);
            clahe->apply(bgr_planes[2], bgr_planes[2]);
            // Объединяем выровненные каналы в выровненное цветное изображение
            merge(bgr_planes, img01Remap_temp );



            // 02
            split( img02Remap_temp, bgr_planes );
            // Применяем выравнивание к гистограммам всех каналов
            clahe->apply(bgr_planes[0], bgr_planes[0]);
            clahe->apply(bgr_planes[1], bgr_planes[1]);
            clahe->apply(bgr_planes[2], bgr_planes[2]);
            // Объединяем выровненные каналы в выровненное цветное изображение
            merge(bgr_planes, img02Remap_temp );




            flag_gray = false;  // Если false  то будет проведено перевод в серое cv::cvtColor(img_temp, gray_temp,  cv::COLOR_BGR2GRAY);
            break;
        }
        case 3:
        {

            if (!flag_gray)
            {
                // Convert the rectified images to grayscale images
                cv::cvtColor(img01Remap_temp, gray01_temp,  cv::COLOR_BGR2GRAY);
                cv::cvtColor(img02Remap_temp, gray02_temp,  cv::COLOR_BGR2GRAY);

                flag_gray = true;
            }

            // ===========
            // Фильтр предобработки изображений: Contrast Limited Adaptive Histogram Equalization
            // ===========
            cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(40,cv::Size(8, 8));

            clahe->apply(gray01_temp, gray01_temp);
            clahe->apply(gray02_temp, gray02_temp);

            break;
        }
        case 4:
        {
            if (!flag_gray)
            {
                // Convert the rectified images to grayscale images
                cv::cvtColor(img01Remap_temp, gray01_temp,  cv::COLOR_BGR2GRAY);
                cv::cvtColor(img02Remap_temp, gray02_temp,  cv::COLOR_BGR2GRAY);
                flag_gray = true;
            }
            // ===========
            // Фильтр предобработки изображений: Contrast Limited Adaptive Histogram Equalization
            // ===========
            equalizeHist(gray01_temp, gray01_temp );
            equalizeHist(gray02_temp, gray02_temp );

            break;
        }


            // default:




        } // switch



    } // for;



    if (  gray01_temp.empty() || gray02_temp.empty() )
    {

        cv::cvtColor(img01Remap_temp, gray01_temp,  cv::COLOR_BGR2GRAY);
        cv::cvtColor(img02Remap_temp, gray02_temp,  cv::COLOR_BGR2GRAY);
        flag_gray = true;

    }







    // =====================================
    // Проверка
    // =====================================

    //    if (!gray01r.empty())
    //    {

    //        imshow("gray01r", gray01r);
    //        // ################################
    //        cv::waitKey(10);
    //        // ################################
    //    }



    ADS::cl_DenseStereo::setImgColorPreProcess_StereoPair( { img01Remap_temp, img02Remap_temp }  );

    ADS::cl_DenseStereo::setImgGrayPreProc_StereoPair(  { gray01_temp, gray02_temp } );



}




void ADS::cl_DenseStereo::setImgColorPreProcess_StereoPair(vector<cv::Mat> imgPreProc_temp)
{


    // ===========
    // Mutex (lock)
    imgRemap_mutex.lock();
    // ===========

    img01PreProc_color = imgPreProc_temp[0].clone();
    img02PreProc_color = imgPreProc_temp[1].clone();

    // ===========
    // Mutex (unlock)
    imgRemap_mutex.unlock();
    // ===========


}

vector<cv::Mat> ADS::cl_DenseStereo::getImgColorPreProcess_StereoPair()
{

    vector<cv::Mat> StereoPair_temp;
    StereoPair_temp.resize(2);
    // ===========
    // Mutex (lock)
    imgRemap_mutex.lock();
    // ===========


    if (img01PreProc_color.empty() || img02PreProc_color.empty() )
    {


        img01PreProc_color =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(img01PreProc_color,
                    "getImgPreProcess:: Image is Empty",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);


        cv::cvtColor(img01PreProc_color, img01PreProc_color,  cv::COLOR_BGR2GRAY);
        img02PreProc_color =  img01PreProc_color.clone();



        //                //=========================================
        //                // Вывод в окно OpenCV
        //                //=========================================
        //                double img_coeff = 0.45;
        //                cv::Mat image;


        //                cv::resize(HR_Img_, image,
        //                           cv::Size( double(HR_Img_.cols * img_coeff), double(HR_Img_.rows * img_coeff) ), 0, 0, cv::INTER_LINEAR);
        //                cv::imshow("01", image);
        //                cv::waitKey(10);


    }



    StereoPair_temp[0] = img01PreProc_color.clone();
    StereoPair_temp[1] = img02PreProc_color.clone();

    // ===========
    // Mutex (unlock)
    imgRemap_mutex.unlock();
    // ===========

    return StereoPair_temp;


}



vector<cv::Mat> ADS::cl_DenseStereo::getImgGrayPreProc_StereoPair()
{

    vector<cv::Mat> StereoPairGray_temp;
    StereoPairGray_temp.resize(2);


    // ===========
    // Mutex (lock)
    imgRemap_mutex.lock();
    // ===========





    if (img01PreProc_gray.empty() || img02PreProc_gray.empty() )
    {


        cv::Mat img_temp =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(img_temp,
                    "getImgGrayPreProc:: Image is Empty",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);


        // Convert the rectified images to grayscale images
        cv::cvtColor(img_temp, img01PreProc_gray,  cv::COLOR_BGR2GRAY);


        img02PreProc_gray =  img01PreProc_gray.clone();



        //                //=========================================
        //                // Вывод в окно OpenCV
        //                //=========================================
        //                double img_coeff = 0.45;
        //                cv::Mat image;


        //                cv::resize(HR_Img_, image,
        //                           cv::Size( double(HR_Img_.cols * img_coeff), double(HR_Img_.rows * img_coeff) ), 0, 0, cv::INTER_LINEAR);
        //                cv::imshow("01", image);
        //                cv::waitKey(10);


    }




    StereoPairGray_temp[0] = img01PreProc_gray.clone();
    StereoPairGray_temp[1] = img02PreProc_gray.clone();

    // ===========
    // Mutex (unlock)
    imgRemap_mutex.unlock();
    // ===========

//    StereoPair_temp[0].convertTo(StereoPair_temp[0], CV_32F);
//    StereoPair_temp[1].convertTo(StereoPair_temp[1], CV_32F );

    return StereoPairGray_temp;

}



    void ADS::cl_DenseStereo::setImgGrayPreProc_StereoPair(vector<cv::Mat> imgGrayPreProc_temp)
    {


        // ===========
        // Mutex (lock)
        imgRemap_mutex.lock();
        // ===========

        img01PreProc_gray = imgGrayPreProc_temp[0].clone();
        img02PreProc_gray = imgGrayPreProc_temp[1].clone();

        // ===========
        // Mutex (unlock)
        imgRemap_mutex.unlock();
        // ===========


    }





cv::Mat ADS::cl_DenseStereo::getImgColorPreProcess_DoubleStereoImg()
{


    vector<cv::Mat> StereoPair_temp = getImgColorPreProcess_StereoPair();
    cv::Mat image01 = StereoPair_temp[0];
    cv::Mat image02 = StereoPair_temp[1];


//    if (image01.empty()) image01 = sample.clone();
//    if (image02.empty()) image02 = sample.clone();

    //=========================================
    // Создание стереопары из 2-х изображений
    //=========================================

    // Get dimension of final image
    int maxV = max(image01.rows, image02.rows);
    int maxU = image01.cols + image02.cols;


    // Create a black image
    cv::Mat3b Img_StereoPair(maxV, maxU, cv::Vec3b(0,0,0));
    // Copy images in correct position
    image01.copyTo(Img_StereoPair(cv::Rect(0, 0, image01.cols, image01.rows)));
    image02.copyTo(Img_StereoPair(cv::Rect(image01.cols, 0, image02.cols, image02.rows)));



    return Img_StereoPair;

}

cv::Mat ADS::cl_DenseStereo::getImgRemap_DoubleStereoImg()
{

    vector<cv::Mat> StereoPair_temp = getImgRemap_StereoPair();

    //=========================================
    // Создание стереопары из 2-х изображений
    //=========================================

    // Get dimension of final image
    int maxV = max(StereoPair_temp[0].rows, StereoPair_temp[1].rows);
    int maxU = StereoPair_temp[0].cols + StereoPair_temp[1].cols;


    // Create a black image
    cv::Mat3b Img_StereoPair(maxV, maxU, cv::Vec3b(0,0,0));
    // Copy images in correct position
    StereoPair_temp[0].copyTo(Img_StereoPair(cv::Rect(0, 0, StereoPair_temp[0].cols, StereoPair_temp[0].rows)));
    StereoPair_temp[1].copyTo(Img_StereoPair(cv::Rect(StereoPair_temp[0].cols, 0, StereoPair_temp[1].cols, StereoPair_temp[1].rows)));


    return Img_StereoPair;

}






void ADS::cl_DenseStereo::FindPointsADS(int limit_points,  int metod_Disparity)
{

    cv::Mat img01RemapGray_temp, img02RemapGray_temp;

    vector<cv::Mat> StereoPairGray = ADS::cl_DenseStereo::getImgGrayPreProc_StereoPair();
    img01RemapGray_temp = StereoPairGray.at(0).clone();
    img02RemapGray_temp = StereoPairGray.at(1).clone();




    if ( (img01RemapGray_temp.empty() || img02RemapGray_temp.empty() ) ) return;

    cv::Mat img01Remap_temp, img02Remap_temp;

    vector<cv::Mat> StereoPair = ADS::cl_DenseStereo::getImgColorPreProcess_StereoPair();
    img01Remap_temp = StereoPair.at(0).clone();
    img02Remap_temp = StereoPair.at(1).clone();


    if ( (img01Remap_temp.empty() || img02Remap_temp.empty() ) ) return;





//    int qk = 0;
//    do
//    {

//        //        // Загрузка исходного изображения
//        //        imgLeft = getImgOpenCV_1left();
//        //        imgRight = getImgOpenCV_2right();


//        // Загрузка исходного изображения
//        vector<cv::Mat> StereoPair = getImgGrayPreProc_StereoPair();
//        img01remap_temp = StereoPair.at(0);
//        img02remap_temp = StereoPair.at(1);


//        #if defined(_WIN64)
//                // ################################
//                Sleep(10);
//                // ################################
//        #else
//            // ################################
//                usleep(10*1000);
//                // ################################
//        #endif

//        qk++;


//    } while ( (img01remap_temp.empty() || img02remap_temp.empty() ) && qk < 200 );








//    qk = 0;
//    do
//    {

//        // Загрузка исходного изображения
//        vector<cv::Mat> StereoPair = getImgOpenCV_StereoPair();
//        img01remap_temp = StereoPair.at(0);
//        img02remap_temp = StereoPair.at(1);

//        cv::remap( img01remap_temp, img01remap_temp, map11, map12,
//                  cv::INTER_LINEAR );
//        cv::remap( img01remap_temp, img01remap_temp, map21, map22,
//                  cv::INTER_LINEAR );

//        cv::cvtColor(img01remap_temp, img01remap_temp,  cv::COLOR_BGR2GRAY);
//        cv::cvtColor(img01remap_temp, img01remap_temp,  cv::COLOR_BGR2GRAY);



//#if defined(_WIN64)
//        // ################################
//        Sleep(10);
//        // ################################
//#else
//        // ################################
//        usleep(10*1000);
//        // ################################
//#endif

//        qk++;


//    } while ( (img01remap_temp.empty() || img02remap_temp.empty() ) && qk < 200 );



    // =====================================
    // Время (начало)
    // =====================================
    int64 time = cv::getTickCount();
    // =====================================

    // Create disparity map

    // double minVal, maxVal;
    //bool metod =  0;
    if (metod_Disparity == 0 ) // BM
    {



        cv::Ptr<cv::StereoBM> bm = cv::StereoBM::create(smbNumDisparities, smbBlockSize);


        bm->setROI1(roi1);
        bm->setROI2(roi2);



        //bm->setPreFilterType(cv::StereoBM::PREFILTER_NORMALIZED_RESPONSE);
        bm->setPreFilterType(cv::StereoBM::PREFILTER_XSOBEL);
        bm->setPreFilterSize(7);
        bm->setPreFilterCap(smbPreFilterCap);        // 31 -> 15 +++

        bm->setMinDisparity(smbMinDisparity);

        bm->setTextureThreshold(smbTextureThreshold);    // 10   +++
        bm->setUniquenessRatio(smbUniquenessRatio);     // 15   +++
        bm->setSpeckleWindowSize(smbSpeckleWindowSize);  // 100 +++
        bm->setSpeckleRange(smbSpeckleRange);        // 32
        bm->setDisp12MaxDiff(smbDisp12MaxDiff);        // 1



        bm->compute(img01RemapGray_temp, img02RemapGray_temp, disparity);



        //        // Added code
        //        cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter = cv::ximgproc::createDisparityWLSFilter(bm);
        //        cv::Ptr<cv::StereoMatcher> bm_02 = cv::ximgproc::createRightMatcher(bm);

        //        bm_02->compute(gray02r, gray01r, disparity_02);


        //        wls_filter->setLambda(100);
        //        wls_filter->setSigmaColor(1.5); // 0.8 - 2.0
        //        /// filtering_time = (double)getTickCount();
        //        wls_filter->filter(disparity_01,img01r,disparity,disparity_02);
        //        //filtering_time = ((double)getTickCount() - filtering_time)/getTickFrequency();




    }

    else if (metod_Disparity > 0 )

      // SGBM
    {

        int sgbmWinSize = smbBlockSize;
        cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(64,    //int minDisparity
                                                              smbNumDisparities,     //int numDisparities
                                                              sgbmWinSize,      //int BlockSize
                                                              0,    //int P1 = 0
                                                              0,   //int P2 = 0
                                                              0,     //int disp12MaxDiff = 0
                                                              0,     //int preFilterCap = 0
                                                              40,      //int uniquenessRatio = 0
                                                              200,    //int speckleWindowSize = 0
                                                              2,     //int speckleRange = 0
                                                              false);  //bool fullDP = false

        sgbm->setPreFilterCap(smbPreFilterCap);


        int cn = img01RemapGray_temp.channels();
        sgbm->setP1(8*cn*sgbmWinSize*sgbmWinSize);
        sgbm->setP2(32*cn*sgbmWinSize*sgbmWinSize);
        sgbm->setMinDisparity(smbMinDisparity);
        sgbm->setNumDisparities(smbNumDisparities);
        sgbm->setUniquenessRatio(smbUniquenessRatio);
        sgbm->setSpeckleWindowSize(smbSpeckleWindowSize);
        sgbm->setSpeckleRange(smbSpeckleRange);
        sgbm->setDisp12MaxDiff(smbDisp12MaxDiff);


       if (metod_Disparity == 1 ) sgbm->setMode(cv::StereoSGBM::MODE_SGBM );        // обычный режим
       if (metod_Disparity == 2 ) sgbm->setMode(cv::StereoSGBM::MODE_HH );          // выполнение полномасштабного двухпроходного алгоритма
       if (metod_Disparity == 3 ) sgbm->setMode(cv::StereoSGBM::MODE_SGBM_3WAY );  // алгоритм выполняется быстрее обычного режима
       if (metod_Disparity == 4 ) sgbm->setMode(cv::StereoSGBM::MODE_HH4 );



        // Для работы с цветным изображениями или серыми
        // sgbm->compute(img01r, img02r, disparity);
        sgbm->compute(img01RemapGray_temp, img02RemapGray_temp, disparity);


        //                  // Added code
        //                  cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter = cv::ximgproc::createDisparityWLSFilter(sgbm);
        //                  cv::Ptr<cv::StereoMatcher> bm_02 = cv::ximgproc::createRightMatcher(sgbm);

        //                  bm_02->compute(gray02r, gray01r, disparity_02);


        //                  wls_filter->setLambda(100);
        //                  wls_filter->setSigmaColor(1.5); // 0.8 - 2.0
        //                  /// filtering_time = (double)getTickCount();
        //                  wls_filter->filter(disparity_01,img01r,disparity,disparity_02);
        //                  //filtering_time = ((double)getTickCount() - filtering_time)/getTickFrequency();


    }



    // =====================================
    // Время (Конец)
    // =====================================
    time = cv::getTickCount() - time;

    printf("\n >>>>  SGBM: Time 3D Points (2), BM:      %fsec\n", time*1/cv::getTickFrequency());
    // =====================================


    // =====================================
    // Время (начало)
    // =====================================
    time = cv::getTickCount();
    // =====================================


     // 1/16 = 0.0625f
    disparity.convertTo(disparity, CV_32F, 0.0625f );


    // =====================================
    // Время (Конец)
    // =====================================
    time = cv::getTickCount() - time;

    printf("\n >>>>  SGBM: Time 3D Points (3), disparity.convertTo:      %fsec\n", time*1/cv::getTickFrequency());
    // =====================================


    //     disparity.convertTo(disparity, CV_8U, 255/(numDisparities*16.));

    //     disparity.convertTo(disparity, CV_8U);

    // =====================================
    // Время (начало)
    // =====================================
    time = cv::getTickCount();
    // =====================================

    // Create depth map
    cv::reprojectImageTo3D(disparity, xyz_AllPoints, Q, true, CV_32F);

    // =====================================
    // Время (Конец)
    // =====================================
    time = cv::getTickCount() - time;

    printf("\n >>>>  SGBM: Time 3D Points (4), reprojectImageTo3D:      %fsec\n", time*1/cv::getTickFrequency());
    // =====================================




    // =====================================
    // Время (начало)
    // =====================================
    time = cv::getTickCount();
    // =====================================

    // =====================================
    t_vuxyzrgb vuxyzrgb_3Dpoints_temp = get_uvxyz_02 (xyz_AllPoints, img01Remap_temp, limit_points, limit_WorkingArea );
    // =====================================



    // =====================================
    // Сохранение результатов до кластеризаци сразу полсе обнаружения 3D точек
    // =====================================
    // vuxyzrgb_3Dpoints_temp.xyz_Disparity_AllPoints = xyz_AllPoints.clone();
    vuxyzrgb_3Dpoints_temp.xyz_Disparity_AllPoints = xyz_AllPoints;


    //vector<double> limit_WorkingArea = vuxyzrgb_3Dpoints_temp.limit_WorkingArea;

    vector<double> limit_outlierArea {-8.0e3, -8.0e3, 250, 8.0e3, 8.0e3, 15.20e3};

    vector<vector<int>> vu;            // 2D координаты точки на изображении
    vector<vector<double>> xyz;        // 3D координаты точки на пространсве
    vector<vector<int>> rgb;        // цвет 3D точки

    for(int v = 0; v < xyz_AllPoints.rows; v++)
    {
        for(int u = 0; u < xyz_AllPoints.cols; u++)
        {

            cv::Vec3f xyz3D = xyz_AllPoints.at<cv::Vec3f>(v, u);


            if( xyz3D[0] < limit_outlierArea[0] ) continue;
            if( xyz3D[1] < limit_outlierArea[1] ) continue;
            if( xyz3D[2] < limit_outlierArea[2] ) continue;

            if( xyz3D[0] > limit_outlierArea[3] ) continue;
            if( xyz3D[1] > limit_outlierArea[4] ) continue;
            if( xyz3D[2] > limit_outlierArea[5] ) continue;


            vu.push_back({v, u});
            xyz.push_back(vector<double> ({xyz3D[0], xyz3D[1], xyz3D[2]}));

            cv::Vec3b rgb2 = img01Remap_temp.at<cv::Vec3b>(v,u);
            rgb.push_back(vector<int> ({rgb2[0], rgb2[1], rgb2[2]} ));

        }
    }


    vuxyzrgb_3Dpoints_temp.vu0 = vu;
    vuxyzrgb_3Dpoints_temp.xyz0 = xyz;
    vuxyzrgb_3Dpoints_temp.rgb0 = rgb;
    vuxyzrgb_3Dpoints_temp.num_p0 = vu.size();






    // =====================================
    // Время (Конец)
    // =====================================
    time = cv::getTickCount() - time;

    printf("\n >>>>  SGBM: Time 3D Points (5), get_uvxyz:      %fsec\n", time*1/cv::getTickFrequency());
    // =====================================

    // ===========
    // Mutex  (lock)
    vuxyzrgb_3Dpoints_mutex.lock();
    // ============

    vuxyzrgb_3Dpoints = vuxyzrgb_3Dpoints_temp;


    // ===========
    // Mutex  (unlock)
    vuxyzrgb_3Dpoints_mutex.unlock();
    // ============

    double minVal, maxVal;
    cv::Mat disp_norm_temp;
    minMaxLoc( disparity, &minVal, &maxVal );
    disparity.convertTo( disp_norm_temp, CV_8UC1, 255/(maxVal - minVal));



    // Apply the colormap:
    // applyColorMap(disp_norm_temp, disp_norm_temp, COLORMAP_JET);
    applyColorMap(disp_norm_temp, disp_norm_temp, COLORMAP_TURBO);


    // ===========
    // Mutex (lock)
    imgDisparity_mutex.lock();
    // ===========
    disp_norm = disp_norm_temp;

    // ===========
    // Mutex (unlock)
    imgDisparity_mutex.unlock();
    // ===========


//    // ===========
//    // Mutex (lock)
//    imgRemap_mutex.lock();
//    // ===========


//    img01remap_gray_preproc = gray01r;
//    img02remap_gray_preproc = gray02r;


//    img01remap = img01remap_temp;
//    img02remap = img02remap_temp;

//    // ===========
//    // Mutex (unlock)
//    imgRemap_mutex.unlock();
//    // ===========


    // cout << "\t\t $$$$$$ FindPointsADS: num_p: " << vuxyzrgb_3Dpoints_temp.num_p << endl;

}


void ADS::cl_DenseStereo::DrawDisparityMap()
{



    // ===========
    // Mutex (lock)
    imgDisparity_mutex.lock();
    // ===========


    cv::Mat disp_norm_temp = disp_norm.clone();

    // ===========
    // Mutex (unlock)
    imgDisparity_mutex.unlock();
    // ===========



    cv::Mat outDisparity_temp;

    if (!disp_norm_temp.empty())
    {


        cv::resize(disp_norm_temp, outDisparity_temp, cv::Size( int(disp_norm_temp.cols * CoefShowWindow),
                                                               int(disp_norm_temp.rows * CoefShowWindow)), 0, 0, cv::INTER_LINEAR);

        //cv::imshow( "Window_DrawPrimitive", outResult01r_temp );
        cv::imshow( "Window_imgResult01Left", outDisparity_temp );

    }

    // ################################
    cv::waitKey(10);
    // ################################


}


void ADS::cl_DenseStereo::AddResultsToDisparityMap02()
{


    double min_x = limit_WorkingArea[0];
    double min_y = limit_WorkingArea[1];
    double min_z = limit_WorkingArea[2];

    double max_x = limit_WorkingArea[3];
    double max_y = limit_WorkingArea[4];
    double max_z = limit_WorkingArea[5];

    // вектор координат 2-выбранных точек + расстояние между этими точками
    vector<double> distance2Point3D_temp(0,7);


    // ===========
    // Mutex (lock)
    imgDisparity_mutex.lock();
    // ===========

    cv::Mat disp_norm_temp = disp_norm.clone();

    // ===========
    // Mutex (unlock)
    imgDisparity_mutex.unlock();
    // ===========



    // ===========
    // Mutex (lock)
    std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============

    t_vuxyzrgb vuxyzrgb_temp = vuxyzrgb; // Временная Структура в формате std:vector

    vector<int> MouseClickCoordinates_temp = MouseClickCoordinates;


    // ===========
    // Mutex (unlock)
    mouse_mutex.unlock();
    vuxyzrgb_mutex.unlock();
    // ============


    Mat xyzD;
//        xyzD = vuxyzrgb_temp.xyz_Disparity_AllPoints.clone();
    xyzD = vuxyzrgb_temp.xyz_Disparity_AllPoints;

    cv::Vec3f xyz01_, xyz02_;




    if (
        (Mouse01.at(0) != MouseClickCoordinates_temp.at(0) && Mouse01.at(1) != MouseClickCoordinates_temp.at(1))
        && (Mouse02.at(0) != MouseClickCoordinates_temp.at(0) && Mouse02.at(1) != MouseClickCoordinates_temp.at(1))
        && !(flag01) && !(flag02)  )
    {


        // Запоминаем клик мыши
        Mouse01 = MouseClickCoordinates_temp;
        xyz01_ = xyzD.at<cv::Vec3f>(Mouse01.at(1), Mouse01.at(0));



        if( xyz01_[0] < min_x ) return;
        if( xyz01_[1] < min_y ) return;
        if( xyz01_[2] < min_z ) return;

        if( xyz01_[0] > max_x ) return;
        if( xyz01_[1] > max_y ) return;
        if( xyz01_[2] > max_z ) return;


        xyz01 = vector<double> ({xyz01_[0], xyz01_[1], xyz01_[2]});


        flag01 = true;

    }




    if (
        (Mouse01.at(0) != MouseClickCoordinates_temp.at(0) && Mouse01.at(1) != MouseClickCoordinates_temp.at(1))
        && (Mouse02.at(0) != MouseClickCoordinates_temp.at(0) && Mouse02.at(1) != MouseClickCoordinates_temp.at(1))
        && flag01 && !(flag02)  )
    {
        // Запоминаем клик мыши
        Mouse02 = MouseClickCoordinates_temp;

        xyz02_ = xyzD.at<cv::Vec3f>(Mouse02.at(1), Mouse02.at(0));

        if( xyz02_[0] < min_x ) return;
        if( xyz02_[1] < min_y ) return;
        if( xyz02_[2] < min_z ) return;

        if( xyz02_[0] > max_x ) return;
        if( xyz02_[1] > max_y ) return;
        if( xyz02_[2] > max_z ) return;



        xyz02 = vector<double> ({xyz02_[0], xyz02_[1], xyz02_[2]});

        flag02 = true;
    }

    cout << "\t \t \t \t \t >>> Mouse01_temp: " << Mouse01.at(0) << ", " << Mouse01.at(1) << endl;
    cout << "\t \t \t \t \t >>> Mouse02_temp: " << Mouse02.at(0) << ", " << Mouse02.at(1) << endl;
    cout << "\t \t \t \t \t >>> flag: " << flag01 << ", " <<flag02 << endl;

    cout << "\t \t \t \t \t >>> xyzD.size: " << xyzD.size()   << endl;


    // cout << "\t \t \t \t \t >>> xyz01:  x:" << xyz01[0] << ", y:" << xyz01[1] << ", z:" << xyz01[2] << endl;
    // cout << "\t \t \t \t \t >>> xyz02:  x:" << xyz02[0] << ", y:" << xyz02[1] << ", z:" << xyz02[2] << endl;








    // ======================================
    // Вывод информации от выбраном кластере возле кнопки сброса
    // ======================================
    if (  flag01 && flag02 )
    {





        // ======================================
        // Вывод координат выделенного объекта на экран
        // ======================================
        stringstream text01, text02, text03;

        text01 << "Point 01: "
               << "u: " << int(Mouse01[0]) << " pix;   "
               << "v: " << int(Mouse01[1]) << " pix;   "
               << "x: " << int(xyz01[0]) << " mm;   "
               << "y: " << int(xyz01[1]) << " mm;   "
               << "z: " << int(xyz01[2]) << " mm;      " ;

        text02 << "Point 02: "
               << "u: " << int(Mouse02[0]) << " pix;   "
               << "v: " << int(Mouse02[1]) << " pix;   "
               << "x: " << int(xyz02[0]) << " mm;   "
               << "y: " << int(xyz02[1]) << " mm;   "
               << "z: " << int(xyz02[2]) << " mm;      " ;


        double Distance = double(sqrt(pow(xyz02[0] - xyz01[0] , 2) +
                                      pow(xyz02[1] - xyz01[1], 2) +
                                      pow(xyz02[2] - xyz01[2], 2)  )   );

        text03 << "Distance between points : "
               <<(int) Distance << " mm;      " ;


        distance2Point3D_temp = {xyz01[0], xyz01[1], xyz01[2], xyz02[0], xyz02[1], xyz02[2], Distance};


        cv::putText	(disp_norm_temp,
                    text01.str(),
                    { 520, 30},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .8,              //double 	fontScale
                    {255,255,255},
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin

        cv::putText	(disp_norm_temp,
                    text02.str(),
                    { 520, 60},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .8,              //double 	fontScale
                    {200,200,255},
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin

        cv::putText	(disp_norm_temp,
                    text03.str(),
                    { 520, 90},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .8,              //double 	fontScale
                    {200,200,255},
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin


        cv::line(disp_norm_temp,
                 {Mouse01[0], Mouse01[1]},
                 {Mouse02[0], Mouse02[1]},
                 {200,200,255},
                 2,               // thickness,
                 cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                 false);       // bottomLeftOrigin



    }






    if (
        (Mouse01.at(0) != MouseClickCoordinates_temp.at(0) && Mouse01.at(1) != MouseClickCoordinates_temp.at(1))
        && (Mouse02.at(0) != MouseClickCoordinates_temp.at(0) && Mouse02.at(1) != MouseClickCoordinates_temp.at(1))
        && flag01 && flag02 )
    {
        flag01 = false;
        flag02 = false;
    }






    // ===========
    // Mutex (lock)
    std::lock(imgResultDisparity_mutex, vuxyzrgb_mutex);
    //std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============


    imgResult_disp02 = disp_norm_temp.clone();
    vuxyzrgb.distance2Point3D =  distance2Point3D_temp;

    // ===========
    // Mutex (unlock)
    imgResultDisparity_mutex.unlock();
    vuxyzrgb_mutex.unlock();
    // ===========



}


void ADS::cl_DenseStereo::DrawDisparityMap02()
{

    // ===========
    // Mutex (lock)
    imgResultDisparity_mutex.lock();
    // ===========

    cv::Mat disp_norm_temp = imgResult_disp02.clone();

    // ===========
    // Mutex (unlock)
    imgResultDisparity_mutex.unlock();
    // ===========





    cv::Mat outDisparity_temp;

    if (!disp_norm_temp.empty())
    {


        cv::resize(disp_norm_temp, outDisparity_temp, cv::Size( int(disp_norm_temp.cols * CoefShowWindow),
                                                               int(disp_norm_temp.rows * CoefShowWindow)), 0, 0, cv::INTER_LINEAR);

        //cv::imshow( "Window_DrawPrimitive", outResult01r_temp );
        cv::imshow( "Window_imgResult01Left", outDisparity_temp );

    }

    // ################################
    cv::waitKey(10);
    // ################################


}


void ADS::cl_DenseStereo::AddResultsToDisparityMap03()
{


    double min_x = limit_WorkingArea[0];
    double min_y = limit_WorkingArea[1];
    double min_z = limit_WorkingArea[2];

    double max_x = limit_WorkingArea[3];
    double max_y = limit_WorkingArea[4];
    double max_z = limit_WorkingArea[5];


    // вектор координат 2-выбранных точек + расстояние между этими точками
    vector<double> distance2Point3D_temp(0,7);

//    // ===========
//    // Mutex (lock)
//    imgResult_mutex.lock();
//    // ===========

//    cv::Mat imgLeft_temp = imgResult01r.clone();

//    // ===========
//    // Mutex (unlock)
//    imgResult_mutex.unlock();
//    // ===========



    cv::Mat imgLeft_temp = getImgColorPreProcess_StereoPair().at(0);
    if (imgLeft_temp.empty()) return;


    // ===========
    // Mutex (lock)
    std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============

    t_vuxyzrgb vuxyzrgb_temp = vuxyzrgb; // Временная Структура в формате std:vector

    vector<int> MouseClickCoordinates_temp = MouseClickCoordinates;


    // ===========
    // Mutex (unlock)
    mouse_mutex.unlock();
    vuxyzrgb_mutex.unlock();
    // ============

    // t_vuxyzrgb vuxyzrgb_temp = get_vuxyzrgb();

    // Mat xyzD = vuxyzrgb_temp.xyz_Disparity_AllPoints.clone();
    Mat xyzD = vuxyzrgb_temp.xyz_Disparity_AllPoints;

    cv::Vec3f xyz01_, xyz02_;

    cv::Vec3b pix_color;

    // ======================================
    // Прорисовка всех точек
    // ======================================
// Если орграинчить сильно зону то ошибка
    for(int v = 0; v < xyzD.rows; v++)
    {
        for(int u = 0; u < xyzD.cols; u++)
        {

            cv::Vec3f xyz = xyzD.at<cv::Vec3f>(v, u); // <<<<<<<<<<<<<<<<<<<<<<<<<<

            if( xyz[0] < min_x ) continue;
            if( xyz[1] < min_y ) continue;
            if( xyz[2] < min_z ) continue;

            if( xyz[0] > max_x ) continue;
            if( xyz[1] > max_y ) continue;
            if( xyz[2] > max_z ) continue;


            cv::Point center;
            center.x = u;
            center.y = v;

            pix_color = imgLeft_temp.at<cv::Vec3b>(v,u);

           int mean = (pix_color[0] + pix_color[1] + pix_color[2])/3;




           if (mean > 50) pix_color *= 0.85;
           else  pix_color *= 1.15;




            imgLeft_temp.at<Vec3b>(v,u) = pix_color;


//            cv::circle	(imgLeft_temp,
//                       center,  // coordinate
//                       1,  //radius
//                       cv::Scalar (pix_color[0] - 50, pix_color[1] + 50, pix_color[2] - 50),     // color   cv::Scalar ( 25, 83, 216)
//                       1,       // thickness,
//                       cv::LINE_4);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA


        }
    }


    if (
        (Mouse01.at(0) != MouseClickCoordinates_temp.at(0) && Mouse01.at(1) != MouseClickCoordinates_temp.at(1))
        && (Mouse02.at(0) != MouseClickCoordinates_temp.at(0) && Mouse02.at(1) != MouseClickCoordinates_temp.at(1))
        && !(flag01) && !(flag02)  )
    {


        // Запоминаем клик мыши
        Mouse01 = MouseClickCoordinates_temp;
        xyz01_ = xyzD.at<cv::Vec3f>(Mouse01.at(1), Mouse01.at(0));



        if( xyz01_[0] < min_x ) return;
        if( xyz01_[1] < min_y ) return;
        if( xyz01_[2] < min_z ) return;

        if( xyz01_[0] > max_x ) return;
        if( xyz01_[1] > max_y ) return;
        if( xyz01_[2] > max_z ) return;


        xyz01 = vector<double> ({xyz01_[0], xyz01_[1], xyz01_[2]});

        flag01 = true;



    }




    if (
        (Mouse01.at(0) != MouseClickCoordinates_temp.at(0) && Mouse01.at(1) != MouseClickCoordinates_temp.at(1))
        && (Mouse02.at(0) != MouseClickCoordinates_temp.at(0) && Mouse02.at(1) != MouseClickCoordinates_temp.at(1))
        && flag01 && !(flag02)  )
    {



        // Запоминаем клик мыши
        Mouse02 = MouseClickCoordinates_temp;

        xyz02_ = xyzD.at<cv::Vec3f>(Mouse02.at(1), Mouse02.at(0));

        if( xyz02_[0] < min_x ) return;
        if( xyz02_[1] < min_y ) return;
        if( xyz02_[2] < min_z ) return;

        if( xyz02_[0] > max_x ) return;
        if( xyz02_[1] > max_y ) return;
        if( xyz02_[2] > max_z ) return;



        xyz02 = vector<double> ({xyz02_[0], xyz02_[1], xyz02_[2]});

        flag02 = true;
    }

    cout << "\t \t \t \t \t >>> Mouse01_temp: " << Mouse01.at(0) << ", " << Mouse01.at(1) << endl;
    cout << "\t \t \t \t \t >>> Mouse02_temp: " << Mouse02.at(0) << ", " << Mouse02.at(1) << endl;
    cout << "\t \t \t \t \t >>> flag: " << flag01 << ", " <<flag02 << endl;

    cout << "\t \t \t \t \t >>> xyzD.size: " << xyzD.size()   << endl;


    // cout << "\t \t \t \t \t >>> xyz01:  x:" << xyz01[0] << ", y:" << xyz01[1] << ", z:" << xyz01[2] << endl;
    // cout << "\t \t \t \t \t >>> xyz02:  x:" << xyz02[0] << ", y:" << xyz02[1] << ", z:" << xyz02[2] << endl;



    // ======================================
    // прорисовка 1-го клика мыши
    // ======================================

    if (  flag01 && !(flag02) )
    {

        // прорисовка 1-го клика мыши
        cv::circle	(imgLeft_temp,
                   {Mouse01[0],Mouse01[1]},  // coordinate
                   5,  //radius
                   {100,0,0},     // color
                   3,       // thickness,
                   cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA


    }



    // ======================================
    // Вывод информации от выбраном кластере возле кнопки сброса
    // ======================================
    if (  flag01 && flag02 )
    {


        // ======================================
        // Вывод координат выделенного объекта на экран
        // ======================================
        stringstream text01, text02, text03;

        text01 << "Point 01: "
               << "u: " << int(Mouse01[0]) << " pix;   "
               << "v: " << int(Mouse01[1]) << " pix;   "
               << "x: " << int(xyz01[0]) << " mm;   "
               << "y: " << int(xyz01[1]) << " mm;   "
               << "z: " << int(xyz01[2]) << " mm;      " ;

        text02 << "Point 02: "
               << "u: " << int(Mouse02[0]) << " pix;   "
               << "v: " << int(Mouse02[1]) << " pix;   "
               << "x: " << int(xyz02[0]) << " mm;   "
               << "y: " << int(xyz02[1]) << " mm;   "
               << "z: " << int(xyz02[2]) << " mm;      " ;


        double Distance = double(sqrt(pow(xyz02[0] - xyz01[0] , 2) +
                                      pow(xyz02[1] - xyz01[1], 2) +
                                      pow(xyz02[2] - xyz01[2], 2)  )   );

        text03 << "Distance between points : "
               << (int) Distance  << " mm;      " ;

        distance2Point3D_temp = {xyz01[0], xyz01[1], xyz01[2], xyz02[0], xyz02[1], xyz02[2], Distance};



        cv::putText	(imgLeft_temp,
                    text01.str(),
                    { 120, imgLeft_temp.rows - 90},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .65,              //double 	fontScale
                    {180,180,180},
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin

        cv::putText	(imgLeft_temp,
                    text02.str(),
                    { 120, imgLeft_temp.rows - 60},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .65,              //double 	fontScale
                    {140,140,180},
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin

        cv::putText	(imgLeft_temp,
                    text03.str(),
                    { 120, imgLeft_temp.rows - 30},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .65,              //double 	fontScale
                    {140,140,180},
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin


        cv::line(imgLeft_temp,
                 {Mouse01[0], Mouse01[1]},
                 {Mouse02[0], Mouse02[1]},
                 {0,0,0},
                 3,               // thickness,
                 cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                 false);       // bottomLeftOrigin



    }


    // ======================================
    // Сброс флага после клика мышкой
    // ======================================

    if (
        (Mouse01.at(0) != MouseClickCoordinates_temp.at(0) && Mouse01.at(1) != MouseClickCoordinates_temp.at(1))
        && (Mouse02.at(0) != MouseClickCoordinates_temp.at(0) && Mouse02.at(1) != MouseClickCoordinates_temp.at(1))
        && flag01 && flag02 )
    {
        flag01 = false;
        flag02 = false;
    }



    // ===========
    // Mutex (lock)
    std::lock(imgResultDisparity_mutex, vuxyzrgb_mutex);
    //std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============


    imgResult_disp03 = imgLeft_temp.clone();
    vuxyzrgb.distance2Point3D =  distance2Point3D_temp;

    // ===========
    // Mutex (unlock)
    imgResultDisparity_mutex.unlock();
    vuxyzrgb_mutex.unlock();
    // ===========


}


void ADS::cl_DenseStereo::DrawDisparityMap03()
{


    // ===========
    // Mutex (lock)
    imgResultDisparity_mutex.lock();
    // ===========

    cv::Mat imgLeft_temp = imgResult_disp03.clone();

    // ===========
    // Mutex (unlock)
    imgResultDisparity_mutex.unlock();
    // ===========




    // ======================================
    // Вывод изображения на экран
    // ======================================


    cv::Mat outDisparity_temp;

    if (!imgLeft_temp.empty())
    {


        cv::resize(imgLeft_temp, outDisparity_temp, cv::Size( int(imgLeft_temp.cols * CoefShowWindow),
                                                             int(imgLeft_temp.rows * CoefShowWindow)), 0, 0, cv::INTER_LINEAR);

        //cv::imshow( "Window_DrawPrimitive", outResult01r_temp );
        cv::imshow( "Window_imgResult01Left", outDisparity_temp );

    }

    // ################################
    cv::waitKey(10);
    // ################################

}


void ADS::cl_DenseStereo::FindColorObjMask()
{



    //    // Не преобразованное исходное изображение
    //    // ======================================
    //    cv::Mat imgLeft_temp = getImgOpenCV_1left();
    //    // ======================================

    // Ректифицированное изображение
    // ======================================
    vector<cv::Mat> StereoPair_temp = ADS::cl_DenseStereo::getImgRemap_StereoPair();
    cv::Mat imgLeft_temp = StereoPair_temp.at(0);
    // ======================================



    // если пустое изображение
    // if (imgLeft_temp.empty()) return;


    // ======================================
    // Предобработка
    // ======================================

    Mat frameBlur,frame_threshold,frame2, frame_HSV;



    // int r1=69; int g1=50; int  b1=134; int r2=154; int g2=118; int b2=156;// стаканчик первое фото
    //  int r1=69; int g1=0; int  b1=0; int r2=255; int g2=255; int b2=49; // красный кубик
    // ======================================

    medianBlur(imgLeft_temp, frameBlur, 7);  //фильтр сглаживания



    cvtColor(frameBlur, frame_HSV, COLOR_BGR2HSV);


    inRange(frame_HSV, LowScalar, HScalar,frame_threshold);


    // морфологическое открытие и закрытие
    Mat se2 = getStructuringElement(MORPH_RECT, Size(20, 20));
    Mat se1 = getStructuringElement(MORPH_RECT, Size(30, 30));
    morphologyEx(frame_threshold, frame_threshold, MORPH_OPEN, se2);
    morphologyEx(frame_threshold, frame_threshold, MORPH_CLOSE, se1);

    // Наращиваем контур для включения точек по краям для приметивов
//    Mat se3 = getStructuringElement(MORPH_RECT, Size(15, 15));
//    morphologyEx(frame_threshold, frame_threshold, MORPH_DILATE, se3);

    frameBlur.copyTo(frame2, frame_threshold);


    imgFindColorObjBoolMask = frame_threshold;
    imgFindColorObjColorMask = frame2;

    //    Проверка




 // =======================
 //  не работает в  linux
// =======================

//    if (!frame2.empty())
//    {
//        double CoefShowWindow = 0.3;

//        cv::resize(frame2, frame2, cv::Size( double(frame2.cols * CoefShowWindow),
//                                            double(frame2.rows) * CoefShowWindow), 0, 0, cv::INTER_LINEAR);

//        imshow("frame2",frame2);
//        // ################################
//        cv::waitKey(50);
//        // ################################

//    }
// =======================






    //     //==============================
    //     // Параметры поиска объектов по цвету и контурам (Олег К.)
    //     //==============================
    //     t_vuxyzrgb vuxyzrgb_ColorObj;        // 3D точки, которые сопоставлены с контуром

    //     cv::Mat imgFindColorObjBoolMask;    // Изображение бинарное (фон черный объект белый)
    //     cv::Mat imgFindColorObjColorMask;   // Изображение цветнного (фон черный объект цветной)
    //     int iNum= -1;                       // Номер самого большого цветного объкта
    //     Mat canny_output;                   // Изображение цветнного
    //     vector<vector<Point>> contours;     // вектор для хранения всех найденных контуров (не маска)
    ////
    //     //(g1, b1, r1)) // красный шарик
    //     Scalar LowScalar = {0, 130, 40};// Нижний граница порога HSV
    //     Scalar HScalar = {15, 220, 220};// Верхняя граница порога HSV

    //     int CountfilenameColorObj = 1001;



    //     // >>>>>>> mutex <<<<<<<
    //     cv::Mat imgFindColorObj; // Изображение с найденным цветным объектом
    //     //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<





}


void ADS::cl_DenseStereo::setFindColor(cv::Scalar LowScalar_in, cv::Scalar HScalar_in)
{
    LowScalar = LowScalar_in;
    HScalar = HScalar_in;
}





void ADS::cl_DenseStereo::set_vuxyzrgbColorObj(ADS::t_vuxyzrgb vuxyzrgb_ColorObj_temp)
{


    // ===========
    // Mutex (lock)
    vuxyzrgb_ColorObj_mutex.lock();
    // ============
    // ======================================
    vuxyzrgb_ColorObj = vuxyzrgb_ColorObj_temp;
    // ======================================

    // ===========
    // Mutex (unlock)
    vuxyzrgb_ColorObj_mutex.unlock();
    // ============


}

ADS::t_vuxyzrgb ADS::cl_DenseStereo::get_vuxyzrgbColorObj()
{

    // ===========
    // Mutex (lock)
    vuxyzrgb_ColorObj_mutex.lock();
    // ============
    // ======================================
   ADS::t_vuxyzrgb vuxyzrgb_ColorObj_temp = vuxyzrgb_ColorObj;
    // ======================================

    // ===========
    // Mutex (unlock)
    vuxyzrgb_ColorObj_mutex.unlock();
    // ============

    return vuxyzrgb_ColorObj_temp;

}






void ADS::cl_DenseStereo::setImgFindColorObj(cv::Mat imgFindColorObj_temp)
{

    // ===========
    // Mutex (lock)
    imgFindColorObj_mutex.lock();
    // ===========

    imgFindColorObj = imgFindColorObj_temp;

    // ===========
    // Mutex (unlock)
    imgFindColorObj_mutex.unlock();
    // ===========
}

cv::Mat ADS::cl_DenseStereo::getImgFindColorObj()
{

    // ===========
    // Mutex (lock)
    imgFindColorObj_mutex.lock();
    // ===========
    cv::Mat imgFindColorObj_temp = imgFindColorObj.clone();

    // ===========
    // Mutex (unlock)
    imgFindColorObj_mutex.unlock();
    // ===========

    return imgFindColorObj_temp;

}



void ADS::cl_DenseStereo::SearchContours()
{

    cv::Mat frame2 = imgFindColorObjColorMask;






//    cv::Mat frame_temp = Mat::zeros(img_Y, img_X, CV_8UC3);

//    if (!frame2.empty())
//    {
//        double CoefShowWindow = 0.4;

//        cv::resize(frame2, frame_temp, cv::Size( double(frame2.cols * CoefShowWindow),
//                                                 double(frame2.rows) * CoefShowWindow), 0, 0, cv::INTER_LINEAR);
//        namedWindow( "frame_th2" , cv::WINDOW_AUTOSIZE);
//        imshow("frame_th2",frame_temp);
//        // ################################
//        cv::waitKey(50);
//        // ################################
//    }

    // ======================================
    // Поиск контуров
    // ======================================


    int thresh = 180;
    Mat canny_output;
    Canny( frame2, canny_output, thresh, thresh*2, 3 );
     findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
   // findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_NONE);


    // return:: (на выходе меняет) поле класса contours - вектор для хранения всех найденных контуров

//     //==============================
//     // Параметры поиска объектов по цвету и контурам (Олег К.)
//     //==============================
//     t_vuxyzrgb vuxyzrgb_ColorObj;

//     cv::Mat imgFindColorObjBoolMask;    // Изображение бинарное (фон черный объект белый)
//     cv::Mat imgFindColorObjColorMask;   // Изображение цветнного (фон черный объект цветной)
//     int iNum= -1;                       // Номер самого большого цветного объкта
//     Mat canny_output;                   // Изображение цветнного
//     vector<vector<Point>> contours;     // вектор для хранения всех найденных контуров (не маска)
////
//     //(g1, b1, r1)) // красный шарик
//     Scalar LowScalar = {0, 130, 40};// Нижний граница порога HSV
//     Scalar HScalar = {15, 220, 220};// Верхняя граница порога HSV

//     int CountfilenameColorObj = 1001;



//     // >>>>>>> mutex <<<<<<<
//     cv::Mat imgFindColorObj; // Изображение с найденным цветным объектом
//     //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<





}



void ADS::cl_DenseStereo::SearchBigObjContours()
{

    iNum= -1;  // Номер самого большого цветного объкта

    Point2f rect_points;
    vector<RotatedRect> minRect( contours.size() );
    double area0 = 0;    // самая большая площадь у обнаруженного объекта
    double areaTemp= 0 ; // для сравнения

    // =============================
    if (contours.empty()) return;      // вектор для хранения всех найденных контуров (не маска)
    // =============================


    // ищем самый большой предмет
    for( size_t i = 0; i< contours.size(); i++ )
    {

        //        minRect[i] = minAreaRect( contours[i] );
        //        rect_points= minRect[i].center;


        //        int coef = 100;
        //        if (rect_points.x < coef) continue;
        //        if (rect_points.x > (cam_X[0] - coef) ) continue;
        //        if (rect_points.y < coef ) continue;
        //        if (rect_points.y > (cam_Y[0] - coef) ) continue;




        // =============================
        // Поиск наибольшего по площади объекта
        // =============================

        areaTemp = contourArea(contours[i]); //площадь   // вектор для хранения всех найденных контуров (не маска)
        if (areaTemp > area0)
        {
            area0 = areaTemp;

            iNum=i;

        }

    }


    // =============================
    // Отсев объектов с малой площадью
    // =============================
        if (area0 < 500)
        {
            iNum = -1;
        }


    // =============================
    // Занесение информации о состоянии для передачи по TCP
    // =============================

    // отчёт о завершении оконтуривания и готовности передать контур объекта в БПУ АНПА
//    if (iNum > -1)  ContouringCompletion_DS = 1;
    if (iNum > -1)  ContouringCompletion_DS = contourArea(contours[iNum]); //площадь;
    else ContouringCompletion_DS = -1;
    // =============================



    // return:: (на выходе меняет) поле класса iNum - номер наибольшеко контура




    //     //==============================
    //     // Параметры поиска объектов по цвету и контурам (Олег К.)
    //     //==============================
    //     t_vuxyzrgb vuxyzrgb_ColorObj;

    //     cv::Mat imgFindColorObjBoolMask;    // Изображение бинарное (фон черный объект белый)
    //     cv::Mat imgFindColorObjColorMask;   // Изображение цветнного (фон черный объект цветной)
    //     int iNum= -1;                       // Номер самого большого цветного объекта
    //     Mat canny_output;                   // Изображение цветнного
    //     vector<vector<Point>> contours;     // вектор для хранения всех найденных контуров (не маска)
    ////
    //     //(g1, b1, r1)) // красный шарик
    //     Scalar LowScalar = {0, 130, 40};// Нижний граница порога HSV
    //     Scalar HScalar = {15, 220, 220};// Верхняя граница порога HSV

    //     int CountfilenameColorObj = 1001;



    //     // >>>>>>> mutex <<<<<<<
    //     cv::Mat imgFindColorObj; // Изображение с найденным цветным объектом
    //     //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<





}






void ADS::cl_DenseStereo::AddContoursToIm()
{

//    // Не преобразованное исходное изображение
//    // ======================================
//    cv::Mat drawing = getImgOpenCV_1left();
//    // ======================================

    // Ректифицированное изображение
    // ======================================
    vector<cv::Mat> StereoPair_temp = ADS::cl_DenseStereo::getImgRemap_StereoPair();
    cv::Mat drawing = StereoPair_temp.at(0);
    // ======================================




    Point2f rect_points[4];
    Point2f rect_points1,rect_points2,rect_points3,rect_points4;


    RNG rng(12345);
    Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
    Scalar color2=Scalar( 0, 0, 255 );


    // поиск прямоугольника
    //                    vector<Rect> boundRect( contours.size() );
    //                    vector<vector<Point> > contours_poly( contours.size() );
    //                    approxPolyDP( contours[iNum], contours_poly[iNum], 3, true );
    //                    boundRect[iNum] = boundingRect( contours_poly[iNum] );
    // rectangle( drawing, boundRect[iNum].tl(), boundRect[iNum].br(), color2, 2 ); // обведение в квадрат


    // Проверка на отсутствие
    if (iNum != -1)
    {

        // поиск вершин обнаруженного предмета
        vector<RotatedRect> minRect( contours.size() );
        minRect[iNum] = minAreaRect( contours[iNum] );

        minRect[iNum].points( rect_points );
        //    cout <<"size rectangle " << minRect[iNum].size<< "\n";
        //    cout <<"angle rectangle " << minRect[iNum].angle<< " grad \n";
        //    cout << "Coordinate  rectangle "<< rect_points[0] << " , "<< rect_points[1]<< " , "<< rect_points[2]<< " , "<< rect_points[3]<< endl;





        for ( int j = 0; j < 4; j++ )
        {
            line( drawing, rect_points[j], rect_points[(j+1)%4], color,4 );
        }
        // ////////////////////////////

        if ( iNum > -1 && iNum < int(contours.size()) )
            drawContours( drawing, contours, (int)iNum, color2, 5 );

        for ( size_t iii=0; iii<4; iii++ )
            circle( drawing, rect_points[iii], 4, 0x000000 , 6 );

    }
    // ======================================
    setImgFindColorObj(drawing);
    // ======================================


    // return:: (на выходе меняет) создаётся цветное исходное изображение с контуром на нём






    //     //==============================
    //     // Параметры поиска объектов по цвету и контурам (Олег К.)
    //     //==============================
    //     t_vuxyzrgb vuxyzrgb_ColorObj;

    //     cv::Mat imgFindColorObjBoolMask;    // Изображение бинарное (фон черный объект белый)
    //     cv::Mat imgFindColorObjColorMask;   // Изображение цветнного (фон черный объект цветной)
    //     int iNum= -1;                       // Номер самого большого цветного объкта
    //     Mat canny_output;                   // Изображение цветнного
    //     vector<vector<Point>> contours;     // вектор для хранения всех найденных контуров (не маска)
    ////
    //     //(g1, b1, r1)) // красный шарик
    //     Scalar LowScalar = {0, 130, 40};// Нижний граница порога HSV
    //     Scalar HScalar = {15, 220, 220};// Верхняя граница порога HSV

    //     int CountfilenameColorObj = 1001;



    //     // >>>>>>> mutex <<<<<<<
    //     cv::Mat imgFindColorObj; // Изображение с найденным цветным объектом
    //     //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




}


void ADS::cl_DenseStereo::matchContoursTo3Dpoints()
{

    // if (iNum < 0) return; // Выход если контур не найден

    cv::Mat frame = Mat::zeros(img_Y, img_X, CV_8UC3);
    cv::Mat frame_th;
    cv::Mat frame_mask = imgFindColorObjBoolMask; // Изображение бинарное (фон черный объект белый)
    t_vuxyzrgb vuxyzrgb_ColorObj_temp;            // Структура с данными для цветного объекта





    // ===========
    // Mutex (lock)
    vuxyzrgb_3Dpoints_mutex.lock();
    // ============
    t_vuxyzrgb vuxyzrgb_3Dpoints_temp = vuxyzrgb_3Dpoints; // временная Структура в формате t_vuxyzrgb, буфер между 3D поиском точек и кластеризацией
    // ===========
    // Mutex (unlock)
    vuxyzrgb_3Dpoints_mutex.unlock();
    // ============


    // Инициализация
    //    vuxyzrgb_ColorObj_temp.vu.resize(vuxyzrgb_beforeFilter_temp.num_cl);
    //    vuxyzrgb_ColorObj_temp.xyz.resize(vuxyzrgb_beforeFilter_temp.num_cl);

//    vuxyzrgb_ColorObj_temp.vu.clear();
//    vuxyzrgb_ColorObj_temp.xyz.clear();


//    cout << "\t\t >>> 000"<< endl;
//    cout << "\t\t >>> vuxyzrgb_beforeFilter_temp.xyz.size() = " << vuxyzrgb_3Dpoints_temp.xyz.size() << endl;
//    cout << "\t\t >>> vuxyzrgb_beforeFilter_temp.vu.size() = " << vuxyzrgb_3Dpoints_temp.vu.size() << endl;
//    cout << "\t\t >>> vuxyzrgb_beforeFilter_temp.num_p = " << vuxyzrgb_3Dpoints_temp.num_p << endl;



    // ==============================
    //  v.01 Заполнение контура для превращеиния его в маску
    // ==============================

     cout << "\t\t >>> matchContoursTo3Dpoints::000"<< endl;


//     if ( iNum > -1 && iNum < int(contours.size()) )
//     {
//         contours[iNum].push_back(contours[iNum][0]);
//         contours[iNum].push_back(contours[iNum][1]);
//         contours[iNum].push_back(contours[iNum][3]);
//         contours[iNum].push_back(contours[iNum][4]);
//     }

//     if ( iNum > -1 && iNum < int(contours.size()) )
//     {
//         for( int q = 0; q < int(contours[iNum].size()); ++q)
//         {
//             contours[iNum][q].y = contours[iNum][q].y + 100;

//         }

//     }


    Scalar color = Scalar(245, 245, 245);
   //vector<Vec4i> hierarchy;

    if ( iNum > -1 && iNum < int(contours.size()) )
    {
        drawContours(frame, contours, (int)iNum, color, 15, LINE_AA);
        drawContours(frame, contours, (int)iNum, color, FILLED, LINE_AA);
        drawContours(frame, contours, (int)iNum, color, FILLED);

    }



        cout << "\t\t >>> matchContoursTo3Dpoints::111"<< endl;

        cout << "\t\t >>>matchContoursTo3Dpoints::(int)iNum = " << (int)iNum << endl;

        inRange(frame, Scalar(1, 1, 1), Scalar(256, 256, 256), frame_th);

        Mat se1 = getStructuringElement(MORPH_RECT, Size(10, 10));
        morphologyEx(frame_th, frame_th, MORPH_CLOSE, se1);

        cout << "\t\t >>> matchContoursTo3Dpoints::222"<< endl;






    for( int q = 0; q < (int) vuxyzrgb_3Dpoints_temp.xyz.size(); ++q)
    {

         //cout << "\t\t >>> q = " << q << endl;

        int v = vuxyzrgb_3Dpoints_temp.vu.at(q).at(0);
        int u = vuxyzrgb_3Dpoints_temp.vu.at(q).at(1);



    // cout << "frame_th.at<int>(v,u)[" << q << "] x = " << frame_th.at<uchar>(v,u) << endl;

//        int v = vuxyzrgb_beforeFilter_temp.vu[q][0];
//        int u = vuxyzrgb_beforeFilter_temp.vu[q][1];

        if ( frame_th.at<uchar>(v,u) !=  0 )
        {
//            cout << "\t\t >>> imgFindColorObjBoolMask.at<int>(u,v) != 0  = " << (imgFindColorObjBoolMask.at<int>(v,u) != 0)  << endl;
          //  cout << "\t\t >>> q+++ = " << q << endl;

            vuxyzrgb_ColorObj_temp.rgb.push_back( vuxyzrgb_3Dpoints_temp.rgb.at(q) );
            vuxyzrgb_ColorObj_temp.xyz.push_back( vuxyzrgb_3Dpoints_temp.xyz.at(q) );
            vuxyzrgb_ColorObj_temp.vu.push_back( vuxyzrgb_3Dpoints_temp.vu.at(q) );




        }



    }

    //    cout << "\t\t >>> 222"<< endl;
    vuxyzrgb_ColorObj_temp.num_p = vuxyzrgb_ColorObj_temp.xyz.size();





    //    Проверка



    //        if ( iNum > -1 && iNum < int(contours.size()) )
    //        {
    //            for( int q = 0; q < (int)contours.at(iNum).size(); ++q)
    //            {

    //                cout << "iNum[" << q << "] x = " << contours[iNum][q].x << "; y = " << contours.at(iNum)[q].y << endl;

    //            }

    //        }


    //        ===========================
    //         Наносим точки
    //        ===========================


// =======================
//  не работает в  linux
// =======================
//    cv::Mat frame_th2 = Mat::zeros(img_Y, img_X, CV_8UC3);

//    if (!frame_th.empty())
//    {
//        double CoefShowWindow = 0.4;

//        cv::resize(frame_th, frame_th2, cv::Size( double(frame_th.cols * CoefShowWindow),
//                                                 double(frame_th.rows) * CoefShowWindow), 0, 0, cv::INTER_LINEAR);

//        imshow("frame_th2",frame_th2);
//        // ################################
//        cv::waitKey(50);
//        // ################################
//    }

// =======================




    cv::Mat frame3 = Mat::zeros(img_Y, img_X, CV_8UC3);

    for (int q = 0; q < vuxyzrgb_ColorObj_temp.num_p  ; q++)
    {

        int v = vuxyzrgb_ColorObj_temp.vu.at(q).at(0);
        int u = vuxyzrgb_ColorObj_temp.vu.at(q).at(1);

        cv::Point centerPoint = {u, v};
        int 	radius = 1;

        cv::circle (frame3,
                   centerPoint,  // coordinate
                   radius,  //radius
                   Scalar(255, 255, 255),     // color
                   1,       // thickness,
                   cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA

    }

// =======================
//  не работает в  linux
// =======================
//    if (!frame3.empty())
//    {
//        double CoefShowWindow = 0.7;

//        cv::resize(frame3, frame3, cv::Size( double(frame3.cols * CoefShowWindow),
//                                                 double(frame3.rows) * CoefShowWindow), 0, 0, cv::INTER_LINEAR);

//        imshow("frame3",frame3);
//        // ################################
//        cv::waitKey(50);
//        // ################################
//    }

// =======================


    cv::Mat frame4 = Mat::zeros(img_Y, img_X, CV_8UC3);;

    if ( iNum > -1 && iNum < int(contours.size()) )
    {
        for( int q = 0; q < (int)contours.at(iNum).size(); ++q)
        {


            int v = contours[iNum][q].y;
            int u = contours[iNum][q].x;


            cv::Point centerPoint = {u, v};
            int radius = 1;

            cv::circle (frame4,
                       centerPoint,  // coordinate
                       radius,  //radius
                       Scalar(0, 0, 255),     // color
                       1,       // thickness,
                       cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA




            //cout << "iNum[" << q << "] x = " << contours[iNum][q].x << "; y = " << contours.at(iNum)[q].y << endl;

        }




    stringstream text;
    text  << "size: " << int(contours.at(iNum).size()) << " ";


    cv::putText	(frame4,
                text.str(),
                {100, 100},
                cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                0.6,              //double 	fontScale
                Scalar(255, 255, 255),
                2,               // thickness,
                cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                false);       // bottomLeftOrigin
// =======================
//  не работает в  linux
// =======================
//    if (!frame4.empty())
//    {
//        double CoefShowWindow = 0.7;

//        cv::resize(frame4, frame4, cv::Size( double(frame4.cols * CoefShowWindow),
//                                            double(frame4.rows) * CoefShowWindow), 0, 0, cv::INTER_LINEAR);

//        imshow("frame4",frame4);
//        // ################################
//        cv::waitKey(50);
//        // ################################
//    }
// =======================



    }



//   cout << "\t\t >>> vuxyzrgb_ColorObj_temp.num_p:  "<< vuxyzrgb_ColorObj_temp.num_p  << endl;


//    // ==============================
//    //  v.02 Заполнение контура для превращеиния его в маску
//    // ==============================

////        if ( iNum > -1 && iNum < int(contours.size()) )
////            vector<Point> Contour = contours[iNum];


//    if ( iNum > -1 && iNum < int(contours.size()) )
//    {
//        for( int q = 0; q < (int) vuxyzrgb_3Dpoints_temp.xyz.size(); ++q)
//        {

//            int v = vuxyzrgb_3Dpoints_temp.vu.at(q).at(0);
//            int u = vuxyzrgb_3Dpoints_temp.vu.at(q).at(1);


//            for( int q = 0; q < (int)contours.at(iNum).size(); ++q)
//            {

//                if ( contours[iNum][q].x   )
//                {



//                }

//            }

//        }












    // =====================================
    // Нахождение центра через Armodilo
    // =====================================

//    type_arma_vuxyzrgb vuxyzrgb_arma_ColorObj_temp;
//    vuxyzrgb_arma_ColorObj_temp.xyz.set_size(vuxyzrgb_ColorObj_temp.num_p,3);

//   cout << "\t\t >>> 222"<< endl;

//    for (int q = 0; q <  vuxyzrgb_ColorObj_temp.num_cl; q++)
//    {
//        vuxyzrgb_arma_ColorObj_temp.xyz.row(q) = arma::conv_to<arma::drowvec> ::from( vuxyzrgb_ColorObj_temp.xyz [q] );
//    }

//    cout << "\t\t >>> 333"<< endl;

//    arma::Col<double> x = vuxyzrgb_arma_ColorObj_temp.xyz.col(0);
//    arma::Col<double> y = vuxyzrgb_arma_ColorObj_temp.xyz.col(1);
//    arma::Col<double> z = vuxyzrgb_arma_ColorObj_temp.xyz.col(2);


//    cout << "\t\t >>> 333aaa"<< endl;

//    cv::Point3d xyz_min, xyz_max, Center3D;
//    double radius3D;
//    // Для оперделения краев кластера и отсеевания выбросов используется с.к.о. (std) с коэффициентом = coef_STD
//    double coef_STD = 2.0;

//    cout << "\t\t >>> 444"<< endl;


//    xyz_min.x = mean(x) - coef_STD * stddev(x);
//    xyz_min.y = mean(y) - coef_STD * stddev(y);
//    xyz_min.z = mean(z) - coef_STD * stddev(z);

//    xyz_max.x = mean(x) + coef_STD * stddev(x);
//    xyz_max.y = mean(y) + coef_STD * stddev(y);
//    xyz_max.z = mean(z) + coef_STD * stddev(z);

//    Center3D.x = (xyz_max.x + xyz_min.x)/2;
//    Center3D.y = (xyz_max.y + xyz_min.y)/2;
//    Center3D.z = (xyz_max.z + xyz_min.z)/2;

//    radius3D =  0.5 * sqrt(pow(xyz_max.x - xyz_min.x, 2) +
//                          pow(xyz_max.y - xyz_min.y, 2) +
//                          pow(xyz_max.z - xyz_min.z, 2)  );

//     cout << "\t\t >>> 555"<< endl;

//    vuxyzrgb_ColorObj_temp.objRect3D.resize(1);
//    vuxyzrgb_ColorObj_temp.objRect3D[0] = {xyz_min.x, xyz_min.y, xyz_min.z, xyz_max.x, xyz_max.y, xyz_max.z, Center3D.x, Center3D.y,Center3D.z, radius3D };

//     cout << "\t\t >>> 666"<< endl;

   // =====================================
   // Нахождение центра (другое)
   // =====================================


//   cout << "\t\t >>> 333"<< endl;

   double mean_x = 0;
   double mean_y = 0;
   double mean_z = 0;

//    cout << "\t\t >>> 333"<< endl;
//    cout << "\t\t >>> vuxyzrgb_beforeFilter_temp.num_p = " << vuxyzrgb_3Dpoints_temp.num_p << endl;
//    cout << "\t\t >>> vuxyzrgb_ColorObj_temp.num_p = " << vuxyzrgb_ColorObj_temp.num_p << endl;

   if (vuxyzrgb_ColorObj_temp.num_p < 1)
   {



       //cout << "\t\t >>> 444"<< endl;

       mean_x = 0;
       mean_y = 0;
       mean_z = 0;

       // cout << "\t\t >>> 555"<< endl;
       vuxyzrgb_ColorObj_temp.objRect3D.resize(1);
       vuxyzrgb_ColorObj_temp.objRect3D[0] = {0, 1, 2, 3, 4, 5, mean_x, mean_y, mean_z, -1};

       // cout << "\t\t >>> 666"<< endl;

       // =====================================
       // Для того чтоб определялись примитивы дописываем информацию по кластеру
       // =====================================
       // int num_cl Количество кластеров
       vuxyzrgb_ColorObj_temp.num_cl = 0;

       // vector<int> num_p_cl  Количесво точек в кластере
       vuxyzrgb_ColorObj_temp.num_p_cl.resize(1, vuxyzrgb_ColorObj_temp.num_p);

       // vector<int> cluster;    Для каждой точки номер кастера
       vuxyzrgb_ColorObj_temp.cluster.resize(vuxyzrgb_ColorObj_temp.num_p, 0);

       // vector<vector<size_t>> clusters;  Для каждого кластера номер точки
       vuxyzrgb_ColorObj_temp.clusters.resize(vuxyzrgb_ColorObj_temp.num_cl);
       for (int q = 0; q<vuxyzrgb_ColorObj_temp.num_p;  q++)
       {
           vuxyzrgb_ColorObj_temp.clusters[0].push_back(q);
       }




   }


   else
   {
       for( int q = 0; q < (int) vuxyzrgb_ColorObj_temp.num_p; ++q)
       {

           // cout << "\t\t >>> q--- = " << q << endl;
           double x = vuxyzrgb_ColorObj_temp.xyz[q][0];
           double y = vuxyzrgb_ColorObj_temp.xyz[q][1];
           double z = vuxyzrgb_ColorObj_temp.xyz[q][2];

           mean_x = mean_x + x;
           mean_y = mean_y + y;
           mean_z = mean_z + z;

       }

       //cout << "\t\t >>> 444"<< endl;

       mean_x = mean_x / vuxyzrgb_ColorObj_temp.num_p;
       mean_y = mean_y / vuxyzrgb_ColorObj_temp.num_p;
       mean_z = mean_z / vuxyzrgb_ColorObj_temp.num_p;

       // cout << "\t\t >>> 555"<< endl;
       vuxyzrgb_ColorObj_temp.objRect3D.resize(1);
       vuxyzrgb_ColorObj_temp.objRect3D[0] = {0, 1, 2, 3, 4, 5, mean_x, mean_y, mean_z, -1};

       // cout << "\t\t >>> 666"<< endl;

       // =====================================
       // Для того чтоб определялись примитивы дописываем информацию по кластеру
       // =====================================
       // int num_cl Количество кластеров
       vuxyzrgb_ColorObj_temp.num_cl = 1;

       // vector<int> num_p_cl  Количесво точек в кластере
       vuxyzrgb_ColorObj_temp.num_p_cl.resize(1, vuxyzrgb_ColorObj_temp.num_p);

       // vector<int> cluster;    Для каждой точки номер кастера
       vuxyzrgb_ColorObj_temp.cluster.resize(vuxyzrgb_ColorObj_temp.num_p, 0);

       // vector<vector<size_t>> clusters;  Для каждого кластера номер точки
       vuxyzrgb_ColorObj_temp.clusters.resize(vuxyzrgb_ColorObj_temp.num_cl);
       for (int q = 0; q<vuxyzrgb_ColorObj_temp.num_p;  q++)
       {
           vuxyzrgb_ColorObj_temp.clusters[0].push_back(q);
       }

   }

    // ======================================
    set_vuxyzrgbColorObj(vuxyzrgb_ColorObj_temp);
    // ======================================




    // Попытка сохранять точки для Авроры (частично успешно - запись есть, но программа вылетает)

    //    // ======================================
    //    set_vuxyzrgb(vuxyzrgb_ColorObj_temp);
    //    // ======================================


//    //    // ===========
//    //    // Mutex (lock)
//    vuxyzrgb_mutex.lock();
//    //    // ============
//    vuxyzrgb.num_p = vuxyzrgb_ColorObj_temp.num_p;

//    vuxyzrgb.vu = vuxyzrgb_ColorObj_temp.vu;
//    vuxyzrgb.xyz = vuxyzrgb_ColorObj_temp.xyz;
//    vuxyzrgb.rgb = vuxyzrgb_ColorObj_temp.rgb;

//    vuxyzrgb.objRect3D = vuxyzrgb_ColorObj_temp.objRect3D;

//    vuxyzrgb.num_cl = vuxyzrgb_ColorObj_temp.num_cl;
//    vuxyzrgb.num_p_cl = vuxyzrgb_ColorObj_temp.num_p_cl;
//    vuxyzrgb.cluster = vuxyzrgb_ColorObj_temp.cluster;
//    vuxyzrgb.clusters = vuxyzrgb_ColorObj_temp.clusters;


//    //    // ===========
//    //    // Mutex (unlock)
//    vuxyzrgb_mutex.unlock();
//    //    // ============





    // =============================
    // Занесение информации о состоянии для передачи по TCP
    // =============================

    // факт обнаружения объекта
    if (vuxyzrgb_ColorObj_temp.num_p > 0)  FuctOjectDetection_DS = 1;

    // =============================




    //     //==============================
    //     // Параметры поиска объектов по цвету и контурам (Олег К.)
    //     //==============================
    //     t_vuxyzrgb vuxyzrgb_ColorObj;

    //     cv::Mat imgFindColorObjBoolMask;    // Изображение бинарное (фон черный объект белый)
    //     cv::Mat imgFindColorObjColorMask;   // Изображение цветнного (фон черный объект цветной)
    //     int iNum= -1;                       // Номер самого большого цветного объкта
    //     Mat canny_output;                   // Изображение цветнного
    //     vector<vector<Point>> contours;     // вектор для хранения всех найденных контуров (не маска)
    ////
    //     //(g1, b1, r1)) // красный шарик
    //     Scalar LowScalar = {0, 130, 40};// Нижний граница порога HSV
    //     Scalar HScalar = {15, 220, 220};// Верхняя граница порога HSV

    //     int CountfilenameColorObj = 1001;



    //     // >>>>>>> mutex <<<<<<<
    //     cv::Mat imgFindColorObj; // Изображение с найденным цветным объектом
    //     //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




}


void ADS::cl_DenseStereo::SaveInfColorObj()
{


    if ( !(iNum > -1 && iNum < int(contours.size())) ) return; // добавли return, раньше небыло

//    // ################################
//     Sleep(50);
//    // ################################


     // =====================================
     // Время (начало)
     // =====================================
     int64 time = cv::getTickCount();
     // =====================================



    cout << "\t\t >>> 000 Save"<< endl;

    if  (CountfilenameColorObj > 1025 ) CountfilenameColorObj = 1001;


    t_vuxyzrgb vuxyzrgb_ColorObj_temp = get_vuxyzrgbColorObj();   // Структура с данными для цветного объекта



    // =====================================
    // Создание папки
    // =====================================

    string path_Img =   "./files/Aurora/Aurora_ObjImgs/";


    bool flag_send = CountfilenameColorObj == 1012;

    if  ( flag_send ) path_Img =   "./files/Aurora/Aurora_Send/";





#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    int result01 = _mkdir(path_Img.c_str());
    if (result01==0 )
        cout << "Aurora: Folder WriteOneStereoPair is made \n";
    else
        cout << "Aurora: Don't made Folder WriteOneStereoPair \n";


        // ================
        // Only Windows END
        // ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(path_Img.c_str(), 0777) == -1  )
        cout << "Aurora: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "Aurora: Don't made Folder WriteOneStereoPair \n";

        // ================
        // Only Linux END
        // ================

#endif




     cout << "\t\t >>> 000a Save"<< endl;


    // =====================================
    // Пути к файлам
    // =====================================
    stringstream filename_imgFindColorObj,
        filename_LocationColorObj;

    filename_imgFindColorObj << path_Img << "imgFindColorObj_" << CountfilenameColorObj << ".jpg";
    filename_LocationColorObj << path_Img  << "imgFindColorObj_" << CountfilenameColorObj << ".txt";

    //    // =====================================
    //    // Сохранение изображения с результатом
    //    // =====================================
    cout << "\t\t >>> 111 Save"<< endl;
    cv::Mat frame_write =  getImgFindColorObj();

    cout << "\t\t >>> filename_imgFindColorObj = " << filename_imgFindColorObj.str() << endl;


    vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(30);

    if (!frame_write.empty()) cv::imwrite(filename_imgFindColorObj.str(), frame_write, compression_params);

    //cout << "\t\t >>> 222"<< endl;




    // =====================================
    //  Пропуск если точек не найдено
    // =====================================
    if (flag_send  && vuxyzrgb_ColorObj_temp.xyz.empty() )
    {

    }
      else
    {

        // =====================================
        //  Сохранение данных о 3D точках после кластеризации в текстовый файл
        // =====================================

        std::ofstream out3;          // поток для записи
        out3.open(filename_LocationColorObj.str(), ios_base::out); // окрываем файл для записи


        if (out3.is_open() )
        {

            cout << "\t\t >>> 333a Save"<< endl;

            int q = 0;

            out3
                << vuxyzrgb_ColorObj_temp.objRect3D[0][6] << " \t "
                << vuxyzrgb_ColorObj_temp.objRect3D[0][7] << " \t "
                << vuxyzrgb_ColorObj_temp.objRect3D[0][8] << " \t "
                << vuxyzrgb_ColorObj_temp.objRect3D[0][9] << " \t "
                << endl;



            // !!!!!!!!!!!!!
            // факт обнаружения объекта
            // FuctOjectDetection_DS = 1;

            FuctOjectDetection_DS = vuxyzrgb_ColorObj_temp.num_p;

            OjectDetection_x_DS = vuxyzrgb_ColorObj_temp.objRect3D[0][6] ;
            OjectDetection_y_DS = vuxyzrgb_ColorObj_temp.objRect3D[0][7] ;
            OjectDetection_z_DS = vuxyzrgb_ColorObj_temp.objRect3D[0][8];

            ObjectDistance_DS = sqrt(pow(OjectDetection_x_DS, 2) +
                                  pow(OjectDetection_y_DS, 2) +
                                  pow(OjectDetection_z_DS, 2)  );


            // уже определено выше  SearchBigObjContours()
            //ContouringCompletion_DS = 1;
            //ContouringCompletion_DS = contourArea(contours[iNum]); //площадь
            // !!!!!!!!!!!!!

        }

        out3.close();

    }

    CountfilenameColorObj++;




    // =====================================
    // Время (Конец)
    // =====================================
    time = cv::getTickCount() - time;

    //
    int delay = 40 - int(time * 1000/cv::getTickFrequency() );
    if ( delay < 1 ) delay = 1;

    // =====================================



#if defined(_WIN64)

    // ################################
    Sleep(delay);
    // ################################

#else

    // ################################
    usleep(delay*1000);
    // ################################

#endif



}




void ADS::cl_DenseStereo::Draw3dPoint()
{


    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    //std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============

   //  cv::Mat xyz_Disparity_AllPoints_temp = vuxyzrgb.xyz_Disparity_AllPoints.clone();
    cv::Mat xyz_Disparity_AllPoints_temp = vuxyzrgb.xyz_Disparity_AllPoints;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============


    cv::Mat im3dPoint_temp;


}



void ADS::cl_DenseStereo::FindPointsASS()
{

    cv::Mat imgLeft_temp, imgRight_temp;

    vector<cv::Mat> StereoPair = getImgGrayPreProc_StereoPair();
    imgLeft_temp = StereoPair.at(0);
    imgRight_temp = StereoPair.at(1);


   if ( (imgLeft_temp.empty() || imgRight_temp.empty() )  ) return;


    Ptr<CLAHE> clahe = createCLAHE();
    Ptr<AKAZE> kaze = AKAZE::create();


    // =============================================
    // Инициализация локальных переменных
    // =============================================
    Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = cv::Mat::eye(3, 3, CV_64F);
    cameraMatrix[1] = cv::Mat::eye(3, 3, CV_64F);




    // ===========
    // Mutex  (lock)
    CalibParam_mutex.lock();
    // ============

    cameraMatrix[0] = M1;
    cameraMatrix[1] = M2;

    distCoeffs[0] = D1;
    distCoeffs[1] = D2;

    // ===========
    // Mutex  (unlock)
    CalibParam_mutex.unlock();
    // ============


    // =============================================
    // Загружаем исходные изображения
    // =============================================





    // ===========
    // Mutex (lock)
    //img_mutex.lock();
    // ============


//    int qk = 0;
//    do
//    {

//        // Загрузка исходного изображения
//        imgLeft_temp = getImgOpenCV_1left();
//        imgRight_temp = getImgOpenCV_2right();

//        //        imgRight = Basler::cl_ImGrabBaslerCam::getImgOpenCV_1left();
//        //        imgLeft = Basler::cl_ImGrabBaslerCam::getImgOpenCV_2right();


//        //        cout << "\t FindPointsADS: imgLeft.empty()  " << imgLeft.empty() << endl;
//        //        cout << "\t FindPointsADS: imgRight.empty()  " << imgRight.empty() << endl;



//        qk++;
//        //if(qk == 9) cout << "FindPointsADS: (qk == 9) " << endl;

//    }while ( (imgLeft_temp.empty() || imgRight_temp.empty() ) && qk < 100 );




    // ===========
    // Mutex (unlock)
    // img_mutex.unlock();
    // ============







    // imgLeft = Basler::cl_ImGrabBaslerCam::getImgOpenCV_1left();
    //imgRight  = Basler::cl_ImGrabBaslerCam::getImgOpenCV_2right();


    //    imgRight = Basler::cl_ImGrabBaslerCam::getImgOpenCV_1left();
    //    imgLeft = Basler::cl_ImGrabBaslerCam::getImgOpenCV_2right();


    Mat image1gray,image2gray, image1clahe, image2clahe;


    image1gray = Mat::zeros(1920, 1200, CV_8UC1);
    image2gray = Mat::zeros(1920, 1200, CV_8UC1);
    image1clahe = Mat::zeros(1920, 1200, CV_8UC1);
    image2clahe = Mat::zeros(1920, 1200, CV_8UC1);

    cvtColor(imgLeft_temp, image1gray, COLOR_BGR2GRAY);
    cvtColor(imgRight_temp, image2gray, COLOR_BGR2GRAY);

    clahe->apply(image1gray, image1clahe);
    clahe->apply(image2gray, image2clahe);


    vector<KeyPoint> kpts1, kpts2;

    Mat desc1, desc2;

    kaze->detectAndCompute(image1clahe, noArray(), kpts1, desc1);
    kaze->detectAndCompute(image2clahe, noArray(), kpts2, desc2);

    BFMatcher matcher(NORM_HAMMING, true);
    vector< vector<DMatch> > nn_matches;

    matcher.knnMatch(desc1, desc2, nn_matches, 1);

    double nn_match_ratio = 70;

    vector<KeyPoint> matched1, matched2;
    for (size_t i = 0; i < nn_matches.size(); i++) {
        if (nn_matches[i].size()) {
            if (nn_matches[i][0].distance < nn_match_ratio) {
                matched1.push_back(kpts1[nn_matches[i][0].queryIdx]);
                matched2.push_back(kpts2[nn_matches[i][0].trainIdx]);
            }
        }
    }


    vector<Point2d> kp1, kp2, new_matched1, new_matched2;

    for (size_t i = 0; i < matched1.size(); i++)
    {
        kp1.push_back(matched1[i].pt);
        kp2.push_back(matched2[i].pt);
    }

    vector<DMatch> good_matches;
    vector<KeyPoint> inliers1, inliers2;
    for (size_t i = 0; i < matched1.size(); i++)
    {
        int new_i = static_cast<int>(inliers1.size());
        inliers1.push_back(matched1[i]);
        inliers2.push_back(matched2[i]);
        good_matches.push_back(DMatch(new_i, new_i, 0));
    }


    vector<Point2d> UndistCoords1, UndistCoords2;

    undistortPoints(kp1, UndistCoords1, cameraMatrix[0], distCoeffs[0], noArray(), cameraMatrix[0]);
    undistortPoints(kp2, UndistCoords2, cameraMatrix[1], distCoeffs[1], noArray(), cameraMatrix[1]);

    Mat triangCoords4D;



    // =============================================
    // Формирование отличных о ADS матриц P1 и P2
    // =============================================

    Mat P1concat, P2concat;
    hconcat(Mat::eye(3, 3, CV_64F), Mat::zeros(3, 1, CV_64F), P1concat);
    hconcat(R, T, P2concat);
    Mat P1_temp = cameraMatrix[0] * P1concat;
    Mat P2_temp = cameraMatrix[1] * P2concat;

    // =============================================



    triangulatePoints(P1_temp, P2_temp, UndistCoords1, UndistCoords2, triangCoords4D);

    Vec3d Coords3D;


    //vector <Point3d> xyz2;
    //xyz = Mat::zeros(imageSize, CV_8UC3);
    // Mat xyz;
    t_vuxyzrgb vuxyzPoints;
    vuxyzPoints.num_p = int(triangCoords4D.size().width );

    for ( int q = 0; q < int(triangCoords4D.size().width ); q++)
    {
        Vec4d triangCoords1 = triangCoords4D.col(q);

        for ( int i = 0; i < 3; i++)
            Coords3D[i] = triangCoords1[i] / triangCoords1[3];

        int v = UndistCoords1[q].y;
        int u = UndistCoords1[q].x;

        cv::Vec3b rgb = imgLeft_temp.at<cv::Vec3b>(kp1[q].y, kp1[q].x);
        // xyz.at<cv::Vec3d>(v, u) = Coords3D;
        // xyz2.push_back(Coords3D);






        vuxyzPoints.vu.push_back(vector<int> {v,u} );
        vuxyzPoints.xyz.push_back(vector<double> ({Coords3D[0], Coords3D[1], Coords3D[2]} ));

        vuxyzPoints.rgb.push_back({rgb[0], rgb[1], rgb[2]});
        // изменено (удалить в будущем)
        //vuxyzPoints.cluster.push_back(-1);


    }

    // Инициализация (перезапись)


    // ===========
    // Mutex  (lock)
    vuxyzrgb_3Dpoints_mutex.lock();
    // ============

    vuxyzrgb_3Dpoints = vuxyzPoints;


    // ===========
    // Mutex  (unlock)
    vuxyzrgb_3Dpoints_mutex.unlock();
    // ============




    // Инициализация изображения для вывода результатов (перенесено в Add Result)
    // imgResult01r = imgLeft;


    //    // =============================
    //    // Save result
    //    //==============================
    //    std::ofstream out2;          // поток для записи
    //    out2.open("./files/xyz_ASS.txt", ios_base::out); // окрываем файл для записи

    //    if (out2.is_open())

    //    for (int q = 0; q <  int( UndistCoords1.size() ); q++)
    //        {

    //                int v = UndistCoords1[q].y;
    //                int u = UndistCoords1[q].x;


    //                out2 <<  vuxyzrgb.xyz[q][2] << " ";


    //            out2  << endl << endl;
    //        }
    //    out2.close();




}


void ADS::cl_DenseStereo::clusteringDBScan(const double rad, const size_t neighbors,
                                           const int filter_ClusterPoints,
                                           const double coef_base_dbs,
                                           const double coef_pow_dbs,
                                           bool metod_PointCorrectinForClastering)
{

    // Start processing by DBSCAN.


    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    t_vuxyzrgb vuxyzrgb_beforeFilter_temp; // временная Структура в формате std:vector как буфер между 3D поиском точек и кластеризацией
    type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура


    //    // ===========
    //    // Mutex (lock)
    //    unique_lock <mutex> vuxyzrgb_ul(vuxyzrgb_mutex);
    //    vuxyzrgb_ul.lock();
    //    // ============


//    // ===========
//    // Mutex (lock)
//    vuxyzrgb_mutex.lock();
//    // ============

//    vuxyzrgb_temp = vuxyzrgb;
//    vuxyzrgb_arma_temp = vuxyzrgb_arma;

//    // ===========
//    // Mutex (unlock)
//    vuxyzrgb_mutex.unlock();
//    // ============



    // ===========
    // Mutex (lock)
    vuxyzrgb_3Dpoints_mutex.lock();
    // ============

    vuxyzrgb_beforeFilter_temp = vuxyzrgb_3Dpoints;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_3Dpoints_mutex.unlock();
    // ============
    //    // ===========
    //    // Mutex (unlock)
    //    vuxyzrgb_ul.unlock();
    //    // ============
    
    //=============================
    // проверка на пустоту и выход если пусто
    //=============================
    if ( vuxyzrgb_beforeFilter_temp.xyz.empty() )
    {

        // ===========
        // Mutex (lock)
        vuxyzrgb_mutex.lock();
        // ============



        //  to vector type
        // переделано из-за сбросов и обнулений
        // Никогда не использовать присвоение всей структуре
        // vuxyzrgb = vuxyzrgb_temp;

        vuxyzrgb.num_p = 0;
        vuxyzrgb.num_cl = 0;
        vuxyzrgb.num_p_cl.resize(0);

        vuxyzrgb.vu.resize(0);
        vuxyzrgb.rgb.resize(0);
        vuxyzrgb.xyz.resize(0);

        vuxyzrgb.num_p0 = 0;

        vuxyzrgb.vu0.resize(0);
        vuxyzrgb.rgb0.resize(0);
        vuxyzrgb.xyz0.resize(0);

        vuxyzrgb.cluster.resize(0);
        vuxyzrgb.clusters.resize(0);

        vuxyzrgb.objRect2D.resize(0);
        vuxyzrgb.objRect3D.resize(0);

        vuxyzrgb.xyz_Disparity_AllPoints.resize(0);


        vuxyzrgb.SelectedClaster = -1;

        //// ======================================






        //  to armadillo type
        // переделано из-за сбросов и обнулений
        // vuxyzrgb_arma = vuxyzrgb_arma_temp;

        vuxyzrgb_arma.num_p = 0;
        vuxyzrgb_arma.num_cl = 0;
//        vuxyzrgb_arma.num_p_cl = vuxyzrgb_arma_temp.num_p_cl;

//        vuxyzrgb_arma.vu = vuxyzrgb_arma_temp.vu;
//        vuxyzrgb_arma.rgb = vuxyzrgb_arma_temp.rgb;
//        vuxyzrgb_arma.xyz = vuxyzrgb_arma_temp.xyz;
//        vuxyzrgb_arma.cluster = vuxyzrgb_arma_temp.cluster;

//        vuxyzrgb_arma.clusters_indx = vuxyzrgb_arma_temp.clusters_indx;
//        vuxyzrgb_arma.clusters_vu = vuxyzrgb_arma_temp.clusters_vu;
//        vuxyzrgb_arma.clusters_rgb = vuxyzrgb_arma_temp.clusters_rgb;
//        vuxyzrgb_arma.clusters_xyz = vuxyzrgb_arma_temp.clusters_xyz;

//        vuxyzrgb_arma.objRect2D = vuxyzrgb_arma_temp.objRect2D;
//        vuxyzrgb_arma.objRect3D = vuxyzrgb_arma_temp.objRect3D;

//        //vuxyzrgb_arma.SelectedClaster = vuxyzrgb_arma_temp.SelectedClaster;





        // ===========
        // Mutex (unlock)
        vuxyzrgb_mutex.unlock();
        // ============

        return;
    }

    // vuxyzrgb_temp.SelectedClaster = -2;



    // DBSCAN algorithm.
    // class
    //pyclustering::clst::cluster_data result; // old
    pyclustering::clst::dbscan_data result; // new

    // 3-ий параметр для  dbs.process(sample, result);
    // pyclustering::clst::dbscan_data_t p_type = pyclustering::clst::dbscan_data_t::POINTS; // 2


    pyclustering::clst::dbscan dbs (rad, neighbors); // class

    vector<vector<double>> sample_db = vuxyzrgb_beforeFilter_temp.xyz;


    if (metod_PointCorrectinForClastering)
    {
        //    //=============================
        //    // преобразование xyz координаты для кластеризации
        //    //=============================

        for (int q = 0; q <  int(sample_db.size()); q++)
        {

            double coeff = 1.0 / ( coef_base_dbs * pow(sample_db.at(q).at(2), coef_pow_dbs) );


//            if (q < 5)
//            {
//                cout << "||||| \t \t \t  |||||||| >>> coef_base_dbs: " <<  coef_base_dbs << endl;
//                cout << "||||| \t \t \t  |||||||| >>> coef_pow_dbs: " <<  coef_pow_dbs << endl;
//                cout << "||||| \t \t \t  |||||||| >>> z: " <<  sample_db.at(q).at(2) << endl;
//                cout << "||||| \t \t \t  |||||||| >>> pow: " <<  pow(sample_db.at(q).at(2), coef_pow_dbs) << endl;
//                cout << "||||| \t \t \t  |||||||| >>> coeff: " <<  coeff << endl;
//            }

            sample_db.at(q).at(0) = coeff * sample_db.at(q).at(0);
            sample_db.at(q).at(1) = coeff * sample_db.at(q).at(1);

        }


    }
    else
    {
        //=============================
        // без преобразование
        //=============================
        sample_db = vuxyzrgb_beforeFilter_temp.xyz;
    }








    const pyclustering::dataset sample = sample_db; // новый

    //=============================
    // DBScan
    //=============================
    dbs.process(sample, result);


    // Add DBSCAN result
    // pyclustering::clst::cluster_sequence clusters = result.clusters();
    //vuxyzrgb.clusters = clusters;


    vuxyzrgb_beforeFilter_temp.num_cl = int(result.size());
    vuxyzrgb_beforeFilter_temp.clusters = result.clusters();




    // =============================
    // Доделать отсеивание ненужных точек и кластеров
    // =============================


    // Отсеивание кластеров с малым количесвом точек

    int n =0 ; // количесво точек после отсеивания
    int k =0 ; // количесво кластеров после отсеивания
    vector<int> idx; // индексы кластеров прошедших фильтрацию



    for (int qc = 0; qc <  vuxyzrgb_beforeFilter_temp.num_cl; qc++)
    {
        if (int( vuxyzrgb_beforeFilter_temp.clusters.at(qc).size() ) < filter_ClusterPoints) continue;

        // количесво оставшихся точек после фильтрации
        n = n + int( vuxyzrgb_beforeFilter_temp.clusters.at(qc).size() );
        k++;  // количесво оставшихся кластеров после фильтрации
        idx.push_back(qc); // индексы кластеров прошедших фильтрацию

    }

    cout << "\t \t >>> num_p: " << vuxyzrgb_beforeFilter_temp.num_p << endl;
    cout << "\t \t >>> num_cl: " << result.size() << endl;

    cout << "\t \t  >>> filt num_p: " <<  n << endl;
    cout << "\t \t >>> filt num_cl: " <<  k << endl;


    // Инициализация
    vuxyzrgb_temp.num_p = n;
    vuxyzrgb_temp.num_cl = k;
    vuxyzrgb_temp.num_p_cl.resize(vuxyzrgb_temp.num_cl,-1);


    //    vuxyzrgb_temp.vu.resize(vuxyzrgb_temp.num_p);
    //    vuxyzrgb_temp.xyz.resize(vuxyzrgb_temp.num_p);
    //    vuxyzrgb_temp.rgb.resize(vuxyzrgb_temp.num_p);
    //    vuxyzrgb_temp.cluster.resize(vuxyzrgb_temp.num_p);

    vuxyzrgb_temp.clusters.resize(vuxyzrgb_temp.num_cl);

    vuxyzrgb_temp.objRect3D.resize(vuxyzrgb_temp.num_cl);
    vuxyzrgb_temp.objRect2D.resize(vuxyzrgb_temp.num_cl);



    for (int qc = 0; qc <  vuxyzrgb_temp.num_cl; qc++)
    {

        vuxyzrgb_temp.num_p_cl[qc] = vuxyzrgb_beforeFilter_temp.clusters[ idx[qc] ].size();

        for (int qi = 0; qi <  int( vuxyzrgb_beforeFilter_temp.clusters[ idx[qc] ].size() ); qi++)
        {

            vuxyzrgb_temp.vu.push_back( vuxyzrgb_beforeFilter_temp.vu[   vuxyzrgb_beforeFilter_temp.clusters[ idx[qc] ][qi] ] );
            vuxyzrgb_temp.xyz.push_back( vuxyzrgb_beforeFilter_temp.xyz[ vuxyzrgb_beforeFilter_temp.clusters[ idx[qc] ][qi] ] );
            vuxyzrgb_temp.rgb.push_back( vuxyzrgb_beforeFilter_temp.rgb[ vuxyzrgb_beforeFilter_temp.clusters[ idx[qc] ][qi] ] );
            vuxyzrgb_temp.cluster.push_back(qc);

            vuxyzrgb_temp.clusters[qc].push_back(vuxyzrgb_temp.vu.size() - 1);

        }
    }

//    vuxyzrgb_temp.xyz_Disparity_AllPoints = vuxyzrgb_beforeFilter_temp.xyz_Disparity_AllPoints.clone();
    vuxyzrgb_temp.xyz_Disparity_AllPoints = vuxyzrgb_beforeFilter_temp.xyz_Disparity_AllPoints;

    // =============================
    // Convertion to armadillo type
    // =============================
    vuxyzrgb_arma_temp = conv_std2arma (vuxyzrgb_temp);


    for (int qc = 0; qc <  vuxyzrgb_temp.num_cl; qc++)
    {

        // Для оперделения краев кластера и отсеевания выбросов используется с.к.о. (std) с коэффициентом = coef_STD
        double coef_STD = 2.0;

        arma::Col<int> u = vuxyzrgb_arma_temp.clusters_vu(qc).col(1);
        arma::Col<int> v = vuxyzrgb_arma_temp.clusters_vu(qc).col(0);

        // Крайние точки ( min. и max)
        cv::Point vu_min,vu_max, Center2D;
        int radius2D;

        // Старое работает не на всех объектах
        //        vu_min.x = mean(u) - coef_STD * stddev(u);
        //        vu_min.y = mean(v) - coef_STD * stddev(v);

        //        vu_max.x = mean(u) + coef_STD * stddev(u);
        //        vu_max.y = mean(v) + coef_STD * stddev(v);

        //        int sredina = (max(u) + min(u)) / 2;
        //        vu_min.x =  sredina - coef_STD * stddev(u);
        //        vu_min.y = sredina - coef_STD * stddev(v);

        //        vu_max.x = sredina + coef_STD * stddev(u);
        //        vu_max.y = sredina + coef_STD * stddev(v);


        // vec Q = quantile(V, P);


        vu_min.x = min(u);
        vu_min.y = min(v);

        vu_max.x = max(u);
        vu_max.y = max(v);







        Center2D.x = (vu_max.x + vu_min.x)/2;
        Center2D.y = (vu_max.y + vu_min.y)/2;

        radius2D =  int(0.5 * sqrt( pow(vu_max.x - vu_min.x, 2) +
                                  pow(vu_max.y - vu_min.y, 2) ) );



        cv::Point3d xyz_min, xyz_max, Center3D;
        double radius3D;

        arma::Col<double> x = vuxyzrgb_arma_temp.clusters_xyz(qc).col(0);
        arma::Col<double> y = vuxyzrgb_arma_temp.clusters_xyz(qc).col(1);
        arma::Col<double> z = vuxyzrgb_arma_temp.clusters_xyz(qc).col(2);

        xyz_min.x = mean(x) - coef_STD * stddev(x);
        xyz_min.y = mean(y) - coef_STD * stddev(y);
        xyz_min.z = mean(z) - coef_STD * stddev(z);

        xyz_max.x = mean(x) + coef_STD * stddev(x);
        xyz_max.y = mean(y) + coef_STD * stddev(y);
        xyz_max.z = mean(z) + coef_STD * stddev(z);

        Center3D.x = (xyz_max.x + xyz_min.x)/2;
        Center3D.y = (xyz_max.y + xyz_min.y)/2;
        Center3D.z = (xyz_max.z + xyz_min.z)/2;

        radius3D =  0.5 * sqrt(pow(xyz_max.x - xyz_min.x, 2) +
                              pow(xyz_max.y - xyz_min.y, 2) +
                              pow(xyz_max.z - xyz_min.z, 2)  );



        // Add objRect
        vuxyzrgb_temp.objRect2D[qc] = {vu_min.x, vu_min.y, vu_max.x, vu_max.y, Center2D.x, Center2D.y, radius2D};
        // vuxyzrgb_temp.objRect2D[qc] = { vu_min.x, vu_min.y, vu_max.x - vu_min.x, vu_max.y - vu_min.y };
        vuxyzrgb_temp.objRect3D[qc] = {xyz_min.x, xyz_min.y, xyz_min.z, xyz_max.x, xyz_max.y, xyz_max.z, Center3D.x, Center3D.y,Center3D.z, radius3D };


        // чтоб не гонять функцию conv_std2arma (Convertion to armadillo type)
        vuxyzrgb_arma_temp.objRect2D.row(qc) = arma::conv_to<arma::Row<int>>::from( vuxyzrgb_temp.objRect2D [qc] );
        vuxyzrgb_arma_temp.objRect3D.row(qc) = arma::conv_to<arma::drowvec> ::from( vuxyzrgb_temp.objRect3D [qc] );


    }



    //    исправление на последний
    //    if (vuxyzrgb_temp.SelectedClaster > vuxyzrgb_temp.num_cl) vuxyzrgb_temp.SelectedClaster = vuxyzrgb_temp.num_cl;
    //    if (vuxyzrgb_temp.SelectedClaster > int(vuxyzrgb_temp.objRect3D.size()-1 ))  vuxyzrgb_temp.SelectedClaster = int (vuxyzrgb_temp.objRect3D.size()-1);


    //    исправление на -1 то есть на отмену выбора
    if (vuxyzrgb_temp.num_cl < vuxyzrgb_temp.SelectedClaster) vuxyzrgb_temp.SelectedClaster = vuxyzrgb_temp.num_cl;
    if (vuxyzrgb_temp.SelectedClaster > int(vuxyzrgb_temp.objRect3D.size()-1 ))  vuxyzrgb_temp.SelectedClaster = -1; // исправило вылеты, которые долго искали





    // ======================================
    //  Исправление ошибки выхода номера выбранного кластера из множества номеров кластеров
    // ======================================

    // Условаие для проверки выхода номера выбранного кластера из множества номеров кластеров полученых после фильтрации
    // Если выходит, чтоб избижать ошибки меняем его в где может возникнуть ошибка:
    // AddResultsToIm();
    // eventMouse_SelectObject();

    if (vuxyzrgb_temp.SelectedClaster > vuxyzrgb_temp.num_cl - 1 ||  vuxyzrgb_temp.num_cl == 0)
    {
        vuxyzrgb_temp.SelectedClaster = -2;
    }




    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    // ============



    //  to vector type
    // переделано из-за сбросов и обнулений
    // Никогда не использовать присвоение всей структуре
    // vuxyzrgb = vuxyzrgb_temp;

    vuxyzrgb.num_p = vuxyzrgb_temp.num_p;
    vuxyzrgb.num_cl = vuxyzrgb_temp.num_cl;
    vuxyzrgb.num_p_cl = vuxyzrgb_temp.num_p_cl;

    vuxyzrgb.vu = vuxyzrgb_temp.vu;
    vuxyzrgb.rgb = vuxyzrgb_temp.rgb;
    vuxyzrgb.xyz = vuxyzrgb_temp.xyz;

    vuxyzrgb.num_p0 = vuxyzrgb_temp.num_p0;
    vuxyzrgb.vu0 = vuxyzrgb_temp.vu0;
    vuxyzrgb.rgb0 = vuxyzrgb_temp.rgb0;
    vuxyzrgb.xyz0 = vuxyzrgb_temp.xyz0;

    vuxyzrgb.cluster = vuxyzrgb_temp.cluster;
    vuxyzrgb.clusters = vuxyzrgb_temp.clusters;

    vuxyzrgb.objRect2D = vuxyzrgb_temp.objRect2D;
    vuxyzrgb.objRect3D = vuxyzrgb_temp.objRect3D;

//    vuxyzrgb.xyz_Disparity_AllPoints = vuxyzrgb_temp.xyz_Disparity_AllPoints.clone();
    vuxyzrgb.xyz_Disparity_AllPoints = vuxyzrgb_temp.xyz_Disparity_AllPoints;

    vuxyzrgb.SelectedClaster = vuxyzrgb_temp.SelectedClaster;

    //// ======================================






    //  to armadillo type
    // переделано из-за сбросов и обнулений
    // vuxyzrgb_arma = vuxyzrgb_arma_temp;

    vuxyzrgb_arma.num_p = vuxyzrgb_arma_temp.num_p;
    vuxyzrgb_arma.num_cl = vuxyzrgb_arma_temp.num_p;
    vuxyzrgb_arma.num_p_cl = vuxyzrgb_arma_temp.num_p_cl;

    vuxyzrgb_arma.vu = vuxyzrgb_arma_temp.vu;
    vuxyzrgb_arma.rgb = vuxyzrgb_arma_temp.rgb;
    vuxyzrgb_arma.xyz = vuxyzrgb_arma_temp.xyz;
    vuxyzrgb_arma.cluster = vuxyzrgb_arma_temp.cluster;

    vuxyzrgb_arma.clusters_indx = vuxyzrgb_arma_temp.clusters_indx;
    vuxyzrgb_arma.clusters_vu = vuxyzrgb_arma_temp.clusters_vu;
    vuxyzrgb_arma.clusters_rgb = vuxyzrgb_arma_temp.clusters_rgb;
    vuxyzrgb_arma.clusters_xyz = vuxyzrgb_arma_temp.clusters_xyz;

    vuxyzrgb_arma.objRect2D = vuxyzrgb_arma_temp.objRect2D;
    vuxyzrgb_arma.objRect3D = vuxyzrgb_arma_temp.objRect3D;

    //vuxyzrgb_arma.SelectedClaster = vuxyzrgb_arma_temp.SelectedClaster;





    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============




    cout << "\t\t $$$$$$ clusteringDBScan: num_p: " << vuxyzrgb_temp.num_cl << endl;





    //     // =============================
    //     // Save result
    //     //==============================
    //      vuxyzrgb_arma.cluster.save("./files/cluster_arma.txt", arma::csv_ascii);

    //     // =============================
    //     // Save result
    //     //==============================
    //     std::ofstream out2;          // поток для записи
    //     out2.open("./files/tempResult222222.txt", ios_base::out); // окрываем файл для записи

    //     if (out2.is_open())

    //         for (int q = 0; q <  int( vuxyzrgb.clusters.size() ); q++)
    //         {
    //             out2 <<  q << ": ";
    //             for (int qi = 0; qi <  int( vuxyzrgb.clusters[q].size() ); qi++)
    //             {

    //                 out2 <<  vuxyzrgb.clusters[q][qi]<< " ";

    //             }
    //             out2  << endl << endl;
    //         }
    //     out2.close();


    //     //====================================================
    //    //   uvxyz.save("./files/point_cloud_arma.txt", arma::csv_ascii);

    //     vuxyzrgb_arma.clusters_indx(4).save("./files/clusters_ind_arma.txt", arma::csv_ascii);
    //   //====================================================

    ////   dbs.~dbscan();
    ////   result.~cluster_data();



}

vector<double> ADS::cl_DenseStereo::getClasterCoordinate3D(int clusterID)
{


    t_vuxyzrgb vuxyzrgb_temp;

    // ===========
    // Mutex (lock)
     vuxyzrgb_mutex.lock();
    // ============

    vuxyzrgb_temp = vuxyzrgb;
   // vuxyzrgb_arma_temp = vuxyzrgb_arma;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============



    vector<double> Coordinate3D;

    if (vuxyzrgb_temp.num_cl > 0 )
    {

        Coordinate3D = vuxyzrgb_temp.objRect3D.at(clusterID);
        // Coordinate3D = {0, 1, 2, 3, 4, 5, -1, -1, -1, -5};

    }

    else  Coordinate3D = {0, 0, 0, 0, 0, 0, -99777, -99777, -99777, -1};

    return Coordinate3D;

}


void ADS::cl_DenseStereo::Coordinate_cam2robot()
{



    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура


    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    // ============

    vuxyzrgb_temp = vuxyzrgb;
    vuxyzrgb_arma_temp = vuxyzrgb_arma;

    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============



    //    //=================================================
    //    // Расчет координат в системе координат робота
    //    //=================================================
    //    arma::Mat<double> RTcam2robot, RTcamRotation;

    //    RTcam2robot = vuxyzrgb_arma_temp.RTmatrix_cam2robot_arma;
    //    RTcamRotation  = vuxyzrgb_arma_temp.RTmatrix_camRotation_arma;


    //    for (int qc = 0; qc <  vuxyzrgb_temp.num_cl; qc++)
    //    {


    //        //===========================
    //        // Инициализация
    //        //===========================
    //       // vuxyzrgb_temp.objRect3D[qc] = {xyz_min.x, xyz_min.y, xyz_min.z, xyz_max.x, xyz_max.y, xyz_max.z, Center3D.x, Center3D.y,Center3D.z, radius3D };

    //        arma::Col<double> xyz_min_arma = {vuxyzrgb_arma_temp.objRect3D.row(qc)[0],
    //                                          vuxyzrgb_arma_temp.objRect3D.row(qc)[1],
    //                                          vuxyzrgb_arma_temp.objRect3D.row(qc)[2],
    //                                          1};

    //        arma::Col<double> xyz_max_arma = {vuxyzrgb_arma_temp.objRect3D.row(qc)[3],
    //                                          vuxyzrgb_arma_temp.objRect3D.row(qc)[4],
    //                                          vuxyzrgb_arma_temp.objRect3D.row(qc)[5],
    //                                          1};

    //        arma::Col<double> Center3D_arma = {vuxyzrgb_arma_temp.objRect3D.row(qc)[6],
    //                                           vuxyzrgb_arma_temp.objRect3D.row(qc)[7],
    //                                           vuxyzrgb_arma_temp.objRect3D.row(qc)[8],
    //                                           1};

    //        double radius3D = vuxyzrgb_arma_temp.objRect3D.row(qc)[9];


    //        RTcam2robot.save("./files/arma/RTcam2robot.txt", arma::raw_ascii);
    //        RTcamRotation.save("./files/arma/RTcamRotation.txt", arma::raw_ascii);

    //        xyz_min_arma.save("./files/arma/xyz_min_arma.txt", arma::raw_ascii);
    //        xyz_max_arma.save("./files/arma/xyz_max_arma.txt", arma::raw_ascii);
    //        Center3D_arma.save("./files/arma/Center3D_arma.txt", arma::raw_ascii);

    //        xyz_min_arma =  RTcam2robot * xyz_min_arma;
    ////        xyz_max_arma =  RTcam2robot * xyz_max_arma;
    ////        Center3D_arma = RTcam2robot * Center3D_arma;


    //        vuxyzrgb_temp.objRect3D_robot_vec[qc] = {xyz_min_arma(0), xyz_min_arma(1), xyz_min_arma(2),
    //                                                 xyz_max_arma(0), xyz_max_arma(1), xyz_max_arma(2),
    //                                                 Center3D_arma(0), Center3D_arma(1),Center3D_arma(2),
    //                                                 radius3D };



    //    }




    //=================================================
    // Расчет координат в системе координат робота
    //=================================================
    cv::Mat RTcam2robot, RTcamRotation;

    RTcam2robot = vuxyzrgb_temp.RTmatrix_cam2robot_cv;
    RTcamRotation  = vuxyzrgb_temp.RTmatrix_camRotation_cv;


    vuxyzrgb_temp.objRect3D_robot_vec.resize(vuxyzrgb_temp.num_cl);


    for (int qc = 0; qc <  vuxyzrgb_temp.num_cl; qc++)
    {


        //===========================
        // Инициализация
        //===========================
        // vuxyzrgb_temp.objRect3D[qc] = {xyz_min.x, xyz_min.y, xyz_min.z, xyz_max.x, xyz_max.y, xyz_max.z, Center3D.x, Center3D.y,Center3D.z, radius3D };

        cv:: Mat xyz_min (4, 1, CV_64F);
        cv:: Mat xyz_max (4, 1, CV_64F);
        cv:: Mat Center3D (4, 1, CV_64F);

        cv:: Mat xyz_min_robor (4, 1, CV_64F);
        cv:: Mat xyz_max_robor (4, 1, CV_64F);
        cv:: Mat Center3D_robor (4, 1, CV_64F);

        xyz_min.at<double>(0,0) = vuxyzrgb_temp.objRect3D[qc][0];
        xyz_min.at<double>(1,0) = vuxyzrgb_temp.objRect3D[qc][1];
        xyz_min.at<double>(2,0) = vuxyzrgb_temp.objRect3D[qc][2];
        xyz_min.at<double>(3,0) = 1;

        xyz_max.at<double>(0,0) = vuxyzrgb_temp.objRect3D[qc][3];
        xyz_max.at<double>(1,0) = vuxyzrgb_temp.objRect3D[qc][4];
        xyz_max.at<double>(2,0) = vuxyzrgb_temp.objRect3D[qc][5];
        xyz_max.at<double>(3,0) = 1;

        Center3D.at<double>(0,0) = vuxyzrgb_temp.objRect3D[qc][6];
        Center3D.at<double>(1,0) = vuxyzrgb_temp.objRect3D[qc][7];
        Center3D.at<double>(2,0) = vuxyzrgb_temp.objRect3D[qc][8];
        Center3D.at<double>(3,0) = 1;

        double radius3D = vuxyzrgb_arma_temp.objRect3D.row(qc)[9];




        xyz_min_robor =  RTcamRotation * RTcam2robot * xyz_min;
        xyz_max_robor =  RTcamRotation * RTcam2robot * xyz_max;
        Center3D_robor = RTcamRotation * RTcam2robot * Center3D;


        vuxyzrgb_temp.objRect3D_robot_vec[qc].resize(10);
        vuxyzrgb_temp.objRect3D_robot_vec[qc] = {xyz_min_robor.at<double>(0), xyz_min_robor.at<double>(1), xyz_min_robor.at<double>(2),
                                                 xyz_max_robor.at<double>(0), xyz_max_robor.at<double>(1), xyz_max_robor.at<double>(2),
                                                 Center3D_robor.at<double>(0), Center3D_robor.at<double>(1), Center3D_robor.at<double>(2),
                                                 radius3D };


        // vuxyzrgb_arma_temp.objRect3D_robot_arma.row(qc) = arma::conv_to<arma::drowvec> ::from( vuxyzrgb_temp.objRect3D_robot_vec [qc] );

    }





    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    // ============


    //  координаты в системе координат робота
    vuxyzrgb.objRect3D_robot_vec = vuxyzrgb_temp.objRect3D_robot_vec;

    //  координаты в системе координат робота
    //vuxyzrgb_arma.objRect3D_robot_arma = vuxyzrgb_arma_temp.objRect3D_robot_arma;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============

}



// Переменная для настроек прорисовки на изображении
// 0 - резерв
// 1 - точки: 0 - без, 1 - с точками
// 2 - центр кластера: 0 - без, 1 -  с геометрическим центром, 2 -  с 2D центром
// 3 - текс: 0 - без, 1 - только "z", 2 - "z" и номер кластера

void ADS::cl_DenseStereo::AddResultsToIm(vector<int> mode)
{




    // Объявление изображения для вывода результатов

    // imgResult01r_temp = getImgOpenCV_1left();
    vector<cv::Mat> StereoPair = ADS::cl_DenseStereo::getImgColorPreProcess_StereoPair();
    cv::Mat  imgResult01r_temp = StereoPair.at(0); // Объявление и инициализация изображения для вывода результатов


    if( imgResult01r_temp.empty() )  return;





    Counter_AddResultsToIm ++; // счётчик для вывода на изображение номера прорисовки


    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура
    int mouse_u_temp, mouse_v_temp;













    // ===========
    // Mutex (lock)
    std::lock(mouse_mutex, vuxyzrgb_mutex);
    //std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============

    vuxyzrgb_temp = vuxyzrgb;
    vuxyzrgb_arma_temp = vuxyzrgb_arma;


    mouse_u_temp = mouse_u;
    mouse_v_temp = mouse_v;

    // ===========
    // Mutex (unlock)
    mouse_mutex.unlock();
    vuxyzrgb_mutex.unlock();
    // ============




    // ==============================
    // При отсутсвии входных данных
    // ==============================
    if (imgResult01r_temp.empty())
    {

        //cv::Mat sample = cv::Mat::zeros(img_Y,img_X,CV_8UC3);
        double coef_size = img_X/1920;
        cv::putText	(sample,
                    "No Image from Cam",
                    {int(coef_size * 50), sample.rows - int(coef_size * 250)},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    coef_size * 2.0,              //double 	fontScale
                    cv::Scalar(48,172,119),
                    coef_size * 10.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin
        imgResult01r_temp = sample;


    }








//    cout << "\t\t\t||||||||||||| imageSize (AddResults) "
//         << cv::Size(imgResult01r_temp.cols,imgResult01r_temp.rows)
//         <<endl;



//    исправление на последний
//    if (vuxyzrgb_temp.num_cl < vuxyzrgb_temp.SelectedClaster) vuxyzrgb_temp.SelectedClaster = vuxyzrgb_temp.num_cl;
//    if (vuxyzrgb_temp.SelectedClaster > int(vuxyzrgb_temp.objRect3D.size()-1 ))  vuxyzrgb_temp.SelectedClaster = int (vuxyzrgb_temp.objRect3D.size()-1); // исправило вылеты, которые долго искали


//    исправление на -1 то есть на отмену выбора
    if (vuxyzrgb_temp.num_cl < vuxyzrgb_temp.SelectedClaster) vuxyzrgb_temp.SelectedClaster = vuxyzrgb_temp.num_cl;
    if (vuxyzrgb_temp.SelectedClaster > int(vuxyzrgb_temp.objRect3D.size()-1 ))  vuxyzrgb_temp.SelectedClaster = -1; // исправило вылеты, которые долго искали

    //    // ===========
    //    // Mutex (lock)
    //    img_mutex.lock();
    //    // ============


    //    imgResult01r_temp = getImgOpenCV_1left();
    //    // ===========
    //    // Mutex (unlock)
    //    img_mutex.unlock();
    //    // ============


    // cout << "\t\t\t ..>> (0) AddResultsToIm: BEGIN "  << endl;

    if (mode[4] == 1)
    {
        // ======================================
        //  Кнопка SHOW ALL
        // ======================================

        Point button01Size = {200,60};

        cv::rectangle (imgResult01r_temp,
                      {5,5},
                      button01Size,
                      cv::Scalar(0,0,255),
                      3,  // thickness,
                      cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA



        cv::putText	(imgResult01r_temp,
                    "SHOW ALL",
                    { 20, 40},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .7,              //double 	fontScale
                    cv::Scalar(0,0,255),
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin




        std::ostringstream ostr; //output string stream

        ostr << Counter_AddResultsToIm; //use the string stream just like cout,
        string Counter_str = ostr.str();

        // Вывод на изображение номера прорисовки
        cv::putText	(imgResult01r_temp,
                    Counter_str,
                    { 40, 85},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .7,              //double 	fontScale
                    cv::Scalar(0,0,255),
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin


        // ======================================

        //          // ======================================
        //          //  Кнопка Select Working Aria
        //          // ======================================



        //          cv::rectangle (imgResult01r_temp,
        //                        {5,120},
        //                        buttonSize,
        //                        cv::Scalar(0,0,255),
        //                        3,  // thickness,
        //                        cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA



        //          cv::putText	(imgResult01r_temp,
        //                      "Select Working Aria",
        //                      { 20, 80},
        //                      cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
        //                      .5,              //double 	fontScale
        //                      cv::Scalar(0,0,255),
        //                      2,               // thickness,
        //                      cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
        //                      false);       // bottomLeftOrigin

        //          // ======================================

    }








    //================
    // Проверка на пустоту с обновлением изображения
    //================

    if ( vuxyzrgb_temp.xyz.empty() ||  vuxyzrgb_temp.num_cl < 0)
    {

        // ===========
        // Mutex (lock)
        imgResult_mutex.lock();
        // ============

        imgResult01r = imgResult01r_temp.clone();

        // ===========
        // Mutex (unlock)
        imgResult_mutex.unlock();
        // ============


        return;
    }
    //     if ( vuxyzrgb_temp.xyz.empty() ) return;
    //     if ( vuxyzrgb_temp.num_cl == 0) return;

    //================








    //================
    // DrawResults()
    //================




    //===========================
    // Создание карты цветов
    //===========================


    //        cv::Scalar clr( rand()&255, rand()&255, rand()&255 );
    cv::Scalar clr;
    vector<cv::Scalar> clrA(8);

    clrA[0] = {47,      20,     162};    //  красный
    clrA[1] = { 25,     83,     216};    // оранжевый
    clrA[2] = { 32,     177,    237};    //  желтый
    clrA[3] = { 48,     172,    119};    // зеленый
    clrA[4] = { 238,	  190,     77};    // голубой
    clrA[5] = { 189,    114,      0};    // синий
    clrA[6] = { 142,    47,     126};    // фиолетовый
    clrA[7] = { 0,       0,       0};    // чернота




    //===========================
    // Отмечаем предыдущий центр кластера
    //===========================


    cv::drawMarker	(imgResult01r_temp,
                   {mouse_u_temp,mouse_v_temp},
                   {0,0,0},
                   cv::MARKER_STAR, //markerType
                   20, //markerSize
                   4,	//thickness
                   8 ); //line_type






    //===========================
    // Отмечаем указатель мыши
    //===========================


    if (mode[6] == 1)
    {
        cv::Point Mouse = {MouseClickCoordinates.at(0), MouseClickCoordinates.at(1)};

        cv::drawMarker	(imgResult01r_temp,
                       Mouse,
                       {0,0,0},
                       cv::MARKER_CROSS, //markerType
                       20, //markerSize
                       4,	//thickness
                       8 ); //line_type

    }



    //===========================
    // Вывод номера выделенного кластера
    //===========================
    if (mode[7] == 1)
    {

        //        if (MouseClickCoordinates.at(0) > button01Size.x &&
        //            MouseClickCoordinates.at(1) > button01Size.y)
        //        {
        stringstream text3;

        // text3 << "Selected Cluster #" << int(vuxyzrgb_temp.SelectedClaster) << "; ";

        //cout << "\t\t\t ..>> (1) AddResultsToIm: Selected Claster # " << int(vuxyzrgb_temp.SelectedClaster)  << endl;




        cv::putText	(imgResult01r_temp,
                    text3.str(),
                    { 30, 90},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    .8,              //double 	fontScale
                    clrA[0],
                    2,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);
        //        }

    }


    //cout << "\t\t --->>> (-1) AddResultsToIm: vuxyzrgb_temp.SelectedClaster: "<< vuxyzrgb_temp.SelectedClaster  << endl;


    //================
    // Выделяем кластеры
    //================
    for (int qc = 0; qc <  int( vuxyzrgb_temp.num_cl ); qc++)
    {

        // cout << "\t\t --->>> AddResultsToIm: all qc: "<< qc <<" "  << endl;

        //===========================
        //  Проверка попадания клика мышки внутрь 2D рамки выделяющей кластер
        //===========================
        // Если клик мыши не попадает в рамку кластера, то проресовка кластера пропускается
        // Если клик мыши попадает в кнопку сброса в углу экрана, то рисуются все кластеры




        if (MouseClickCoordinates.at(0) > button01Size.x &&
            MouseClickCoordinates.at(1) > button01Size.y &&
            vuxyzrgb_temp.SelectedClaster != qc) continue;

        //cout << "\t\t --->>> AddResultsToIm: Selected qc: "<< qc <<" "  << endl;





        // Крайние точки выделяющей объект рамки в 2D и 3D ( min. и max)
        cv::Point vu_min,vu_max, Center2D;
        int radius2D;
        vu_min.x = vuxyzrgb_temp.objRect2D[qc][0];
        vu_min.y = vuxyzrgb_temp.objRect2D[qc][1];

        vu_max.x = vuxyzrgb_temp.objRect2D[qc][2];
        vu_max.y = vuxyzrgb_temp.objRect2D[qc][3];

        Center2D.x = vuxyzrgb_temp.objRect2D[qc][4];
        Center2D.y = vuxyzrgb_temp.objRect2D[qc][5];

        radius2D =  vuxyzrgb_temp.objRect2D[qc][6];


        cv::Point3d xyz_min, xyz_max, Center3D;
        double radius3D;
        xyz_min.x = vuxyzrgb_temp.objRect3D[qc][0];
        xyz_min.y = vuxyzrgb_temp.objRect3D[qc][1];
        xyz_min.z = vuxyzrgb_temp.objRect3D[qc][2];

        xyz_max.x = vuxyzrgb_temp.objRect3D[qc][3];
        xyz_max.y = vuxyzrgb_temp.objRect3D[qc][4];
        xyz_max.z = vuxyzrgb_temp.objRect3D[qc][5];

        Center3D.x = vuxyzrgb_temp.objRect3D[qc][6];
        Center3D.y = vuxyzrgb_temp.objRect3D[qc][7];
        Center3D.z = vuxyzrgb_temp.objRect3D[qc][8];

        radius3D =  vuxyzrgb_temp.objRect3D[qc][9];


        //        cout << "\t \t \t >>>>> comparison1: " << (MouseClickCoordinates.at(0) > button01Size.x &&
        //                                                   MouseClickCoordinates.at(1) > button01Size.y &&
        //                                                   vuxyzrgb_temp.SelectedClaster != qc) << endl;

        //        cout << "\t \t \t >>>>> vuxyzrgb_temp.SelectedClaster1: " << vuxyzrgb_temp.SelectedClaster << endl;
        //        cout << "\t \t \t >>>>> new qc1: " << qc << endl;
        //        cout << "\t \t \t >>>>> new qc1: " << qc << endl;





        //===========================
        // Определение цвета
        //===========================

        //int distance_Z = mean(vuxyzrgb_arma_temp.clusters_xyz(qc).col(2));
        int distance_Z = Center3D.z;

        if      ( (distance_Z  > 250)  && (distance_Z < 500) )
            clr = clrA[0];   //  красный
        else if ( distance_Z  > 500  && distance_Z < 600)
            clr = clrA[1];  // оранжевый
        else if ( distance_Z  > 600  && distance_Z < 800)
            clr = clrA[2];  //  желтый
        else if ( distance_Z  > 800  && distance_Z < 1000)
            clr = clrA[3];   // зеленый
        else if ( distance_Z  > 1000  && distance_Z < 1200)
            clr = clrA[4];    // голубой
        else if ( distance_Z  > 1200  && distance_Z < 1500)
            clr = clrA[5];    // синий
        else if ( distance_Z  > 1500  && distance_Z < 2000)
            clr = clrA[6];    // фиолетовый
        else
            clr = clrA[7];    // чернота




        //        if      ( (distance_Z  > 250)  && (distance_Z < 600) )
        //            clr = clrA[0];   //  красный
        //        else if ( distance_Z  > 600  && distance_Z < 1000)
        //            clr = clrA[1];  // оранжевый
        //        else if ( distance_Z  > 1000  && distance_Z < 1200)
        //            clr = clrA[2];  //  желтый
        //        else if ( distance_Z  > 1200  && distance_Z < 1500)
        //            clr = clrA[3];   // зеленый
        //        else if ( distance_Z  > 1500  && distance_Z < 2000)
        //            clr = clrA[4];    // голубой
        //        else if ( distance_Z  > 2000  && distance_Z < 3000)
        //            clr = clrA[5];    // синий
        //        else if ( distance_Z  > 3000  && distance_Z < 4000)
        //            clr = clrA[6];    // фиолетовый
        //        else
        //            clr = clrA[7];    // чернота


        //        ===========================
        //         Наносим точки
        //        ===========================


        if (mode[1] == 1)
        {


            //              cout << "\t vuxyzrgb.num_p: " << vuxyzrgb.num_p << endl;
            //              cout << "\t vuxyzrgb.num_cl: " << vuxyzrgb.num_cl << endl;
            //               cout << "\t vuxyzrgb.clusters[qc].size(): " << vuxyzrgb.clusters[qc].size() << endl;

            //              cout << "\t vuxyzrgb.vu.size(): " << vuxyzrgb.vu.size() << endl;
            //              cout << "\t vuxyzrgb.xyz.size(): " << vuxyzrgb.xyz.size() << endl;



            //              cout << "\t vuxyzrgb_arma.num_p: " << vuxyzrgb_arma.num_p << endl;
            //              cout << "\t vuxyzrgb_arma.num_cl: " << vuxyzrgb_arma.num_cl << endl;


            //              cout << "\t vuxyzrgb_arma.num_p_cl(qc): " << vuxyzrgb_arma.num_p_cl(qc) << endl;
            //              cout << "\t vuxyzrgb_arma.clusters_vu.size(): " << vuxyzrgb_arma.clusters_vu.size() << endl;
            //              cout << "\t vuxyzrgb_arma.clusters_vu(qc).size(): " << vuxyzrgb_arma.clusters_vu(qc).size() << endl;
            //              cout << "\t vuxyzrgb_arma.clusters_xyz.size(): " << vuxyzrgb_arma.clusters_xyz.size() << endl;
            //              cout << "\t vuxyzrgb_arma.clusters_xyz(qc).size(): " << vuxyzrgb_arma.clusters_xyz(qc).size() << endl;




            for (int qp = 0; qp < vuxyzrgb_arma_temp.num_p_cl(qc)-1  ; qp++)
            {


                cv::Point centerPoint = {vuxyzrgb_arma_temp.clusters_vu(qc)(qp,1), vuxyzrgb_arma_temp.clusters_vu(qc)(qp,0)};
                int 	radius = 1;

                cv::circle	(imgResult01r_temp,
                           centerPoint,  // coordinate
                           radius,  //radius
                           clr,     // color
                           2,       // thickness,
                           cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA

            }

        }




        //===========================
        // Рисуем прямоугольник
        //===========================

        if (mode[0] == 1)
        {

        // Заносим координаты рамки

        cv::rectangle (imgResult01r_temp,
                      {vu_min.x - 10, vu_min.y - 10},
                      {vu_max.x + 10, vu_max.y + 10},
                      clr,
                      2,  // thickness,
                      cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA


        //        int mix = 3;
        //        vu_max = {vu_max.x + mix, vu_max.y + mix};
        //        vu_min = {vu_max.x - mix, vu_max.y - mix};

        //        cv::rectangle (imgResult01r_temp,
        //                      vu_min,
        //                      vu_max,
        //                      clr, // color
        //                      2,  // thickness,
        //                      cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA

        }

        //===========================
        // Отмечаем цент кластера
        //===========================

        if (mode[2] == 1)
        {

            cv::Point centerClusterGeometric = {  Center2D.x, Center2D.y};


            cv::circle	(imgResult01r_temp,
                       centerClusterGeometric,
                       9,   //radius
                       {0,0,0}, // color
                       3, // thickness,
                       cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
        }


        if (mode[2] == 2)
        {

            cv::Point centerCluster2Dpoints = { arma::mean( vuxyzrgb_arma_temp.clusters_vu(qc).col(1) ),
                                               arma::mean( vuxyzrgb_arma_temp.clusters_vu(qc).col(0) ) };

            cv::circle	(imgResult01r_temp,
                       centerCluster2Dpoints,
                       9,   //radius
                       {0,0,0}, // color
                       3, // thickness,
                       cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
        }



        //            //===========================
        //            // Рисуем выделяющий круг
        //            //===========================


        //            cv::circle	(imgResult01r_temp,
        //                       centerClusterGeometric,
        //                       radius2D,   //radius
        //                       clr, // color
        //                       3, // thickness,
        //                       cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA



        //===========================
        // Наносим текст
        //===========================
        //  координата по Z


        if (mode[3] == 1 || mode[3] == 2)
        {

            //        char text[1024];
            //        sprintf(text,"%i", int(arma::mean(vuxyzrgb_arma.clusters_xyz(qc).col(2))  ));

            stringstream text;
            text  << "z: " << int(Center3D.z) << " mm";


            cv::putText	(imgResult01r_temp,
                        text.str(),
                        {vu_min.x - 15, vu_min.y - 15},
                        cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                        0.6,              //double 	fontScale
                        clr,
                        2,               // thickness,
                        cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                        false);       // bottomLeftOrigin
        }

        if (mode[3] == 2)
        {

            // Номер кластерв
            //                sprintf(text,"%i", qc );
            //                stringstream text2;


            stringstream text;
            text  << "cl: " <<  qc;

            cv::putText	(imgResult01r_temp,
                        text.str(),
                        {vu_min.x - 15, vu_min.y - 45},
                        cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                        0.6,              //double 	fontScale
                        clr,
                        2,               // thickness,
                        cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                        false);       // bottomLeftOrigin

        }




        if (mode[5] == 1)
        {

            // ======================================
            // Вывод информации от выбраном кластере возле кнопки сброса
            // ======================================
            if (MouseClickCoordinates.at(0) > button01Size.x &&
                MouseClickCoordinates.at(1) > button01Size.y &&
                vuxyzrgb_temp.SelectedClaster > -1)
            {

                // ======================================
                // Вывод координат выделенного объекта на экран
                // ======================================
                stringstream text3;

                text3 << "   ";

                text3 << "x: " << int(Center3D.x) << " mm;   "
                      << "y: " << int(Center3D.y) << " mm;   "
                      << "z: " << int(Center3D.z) << " mm;   ";
                //<< "R: " << int(radius3D) << " mm.   ";



                cv::putText	(imgResult01r_temp,
                            text3.str(),
                            { 420, 30},
                            cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                            .75,              //double 	fontScale
                            clr,
                            2,               // thickness,
                            cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                            false);       // bottomLeftOrigin




                //          char textTemp[1024];

                //          sprintf( textTemp,"x: %i", int(arma::mean(vuxyzrgb_arma.clusters_xyz(qc).col(0))  ));

                //          cv::putText	(imgResult01r,
                //                      textTemp,
                //                      { 20, 100},
                //                      cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                //                      .6,              //double 	fontScale
                //                      cv::Scalar(0,0,255),
                //                      2,               // thickness,
                //                      cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                //                      false);       // bottomLeftOrigin
                //          sprintf( textTemp,"y: %i", int(arma::mean(vuxyzrgb_arma.clusters_xyz(qc).col(1))  ));

                //          cv::putText	(imgResult01r,
                //                      textTemp,
                //                      { 20, 120},
                //                      cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                //                      .6,              //double 	fontScale
                //                      cv::Scalar(0,0,255),
                //                      2,               // thickness,
                //                      cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                //                      false);       // bottomLeftOrigin

                //          sprintf( textTemp,"z: %i", int(arma::mean(vuxyzrgb_arma.clusters_xyz(qc).col(2))  ));

                //          cv::putText	(imgResult01r,
                //                      textTemp,
                //                      { 20, 140},
                //                      cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                //                      .6,              //double 	fontScale
                //                      cv::Scalar(0,0,255),
                //                      2,               // thickness,
                //                      cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                //                      false);       // bottomLeftOrigin



                // ======================================
                // Вывод на экран информации о приметиве выделенного объекта
                // ======================================
                //    // Plane:
                //    // z = ax + by + c: [a, b, c, err]
                //    // 0 - a
                //    // 1 - b
                //    // 2 - c
                //    // 3 - err


                //    //Sphere:
                //    //[xc, yc, zc, radius, err]
                //    // 0 - xc
                //    // 1 - yc
                //    // 2 - zc
                //    // 3 - radius
                //    // 4 - err


                //    // Cylinder:
                //    // 0 - xc1
                //    // 1 - yc1
                //    // 2 - zc1

                //    // 3 - xc2
                //    // 4 - yc2
                //    // 5 - zc2

                //    // 6 - radius
                //    // 7 - err







//                cout << "\t\t\t||||||||||||| (AddResults) 05 vuxyzrgb_temp.PrimitiveDetect.empty()"<< vuxyzrgb_temp.PrimitiveDetect.empty() <<endl;
//                cout << "\t\t\t||||||||||||| (AddResults) 05 vuxyzrgb_temp.PrimitiveDetect.size()"<< vuxyzrgb_temp.PrimitiveDetect.size() <<endl;
//                cout << "\t\t\t||||||||||||| (AddResults) 05 vuxyzrgb_temp.SelectedClaster "<< vuxyzrgb_temp.SelectedClaster <<endl;
//                cout << "\t\t\t||||||||||||| (AddResults) 05 int(vuxyzrgb_temp.PrimitiveDetect.size()) > vuxyzrgb_temp.SelectedClaster) "
//                     << (int(vuxyzrgb_temp.PrimitiveDetect.size()) > vuxyzrgb_temp.SelectedClaster) << endl;

//                cout << "\t\t\t||||||||||||| (AddResults) 05 if ---- "
//                     << (!vuxyzrgb_temp.PrimitiveDetect.empty()
//                         && int(vuxyzrgb_temp.PrimitiveDetect.size()) > vuxyzrgb_temp.SelectedClaster) << endl;






                if ( !vuxyzrgb_temp.PrimitiveDetect.empty()
                    && int(vuxyzrgb_temp.PrimitiveDetect.size()) > vuxyzrgb_temp.SelectedClaster)
                {


                    stringstream text4;
                    text4 << "  >>> ";

                    switch (vuxyzrgb_temp.PrimitiveDetect[vuxyzrgb_temp.SelectedClaster])
                    {
                    case 0:
                        text4  << "Non";
                        break;
                    case 1:
                        text4  << "Plane:     "
                              << " a: " << double(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][0]) << "    ;  "
                              << " b: " << double(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][1]) << "    ;  "
                              << " c: " << double(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][2]) << "    ;  ";
                        break;
                    case 2:
                        text4  << "Sphere:    "
                              << " x: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][0]) << " mm; "
                              << " y: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][1]) << " mm; "
                              << " z: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][2]) << " mm; "
                              << " R: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][3]) << " mm. ";
                        break;
                    case 3:
                        text4  << "Cylinder:  "
                              << " xc1: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][0]) << " mm; "
                              << " yc1: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][1]) << " mm; "
                              << " zc1: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][2]) << " mm; "
                              << " xc2: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][3]) << " mm; "
                              << " yc2: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][4]) << " mm; "
                              << " zc2: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][5]) << " mm; "
                              << " R: " << int(vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster][6]) << " mm. ";
                    }


                    cv::putText	(imgResult01r_temp,
                                text4.str(),
                                { 220, 60},
                                cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                                .75,              //double 	fontScale
                                clr,
                                2,               // thickness,
                                cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                                false);       // bottomLeftOrigin
                }
            }

        }
        // ======================================



    }



    // причина: неопределенная ошибка objRect
    // Convertion to armadillo type
    //vuxyzrgb_arma_temp = conv_std2arma (vuxyzrgb_temp);


    // cout << "\t\t\t ..>> (0) AddResultsToIm: END "  << endl;

    // ===========
    // Mutex (lock)
    imgResult_mutex.lock();
    // ============

    imgResult01r = imgResult01r_temp.clone();

    // ===========
    // Mutex (unlock)
    imgResult_mutex.unlock();
    // ============



}




void ADS::cl_DenseStereo::setRTmatrix_cam2robot_cv(cv::Mat RT)
{

    // arma::mat RT_arma( reinterpret_cast<double*>(RT.data), RT.rows, RT.cols );

    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    // ============

    vuxyzrgb.RTmatrix_cam2robot_cv = RT;
    // vuxyzrgb_arma.RTmatrix_cam2robot_arma = RT_arma;

    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============

}





void ADS::cl_DenseStereo::setRTmatrix_camRotation_angle(vector<double>)
{


    // Матрица вращения для пересчета координат при поворете камеры поворотным устройсвтвом
    // будет менятся в зависимости от углов поворота
    vector <vector<double>> R_camRotation_vec;
    cv::Mat RMatrix_camRotation = cv::Mat(4, 4, CV_64F);
    arma::dmat RMatrix_camRotation_arma;

    R_camRotation_vec.resize(4);
    R_camRotation_vec[0] = {1,  0,  0, 0};
    R_camRotation_vec[1] = {0,  1,  0, 0};
    R_camRotation_vec[2] = {0,  0,  1, 0};
    R_camRotation_vec[3] = {0,  0,  0, 1};




    //=================================================
    //  ТУТ БУДЕТ КОД ДЛЯ ПЕРЕВОДА УГЛОВ В МАТРИЦУ ТРАНСФОРМАЦИИ
    //=================================================


    for (int i =0; i<4 ; i++)
    {
        for (int j =0; j<4 ; j++)
        {
            RMatrix_camRotation.at<double>(i, j) = R_camRotation_vec[i][j];
            //RMatrix_camRotation_arma(i,j) =  R_camRotation_vec[i][j];
        }
    }






    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    // ============


    vuxyzrgb.RTmatrix_camRotation_cv = RMatrix_camRotation;
    //vuxyzrgb_arma.RTmatrix_camRotation_arma = RMatrix_camRotation_arma;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============



}



void ADS::cl_DenseStereo::saveFileResult()
{

    // Сохранение изображения с результатом
    //cv::imwrite("./files/imgLeftResult.png", imgResult01r);
    cv::imwrite("C:/files/imgLeftResult.png", imgResult01r);


    // Сохранение характеристических данных
    std::ofstream out4;          // поток для записи
    out4.open("C:/files/infoFile.txt", ios_base::out); // окрываем файл для записи

    if (out4.is_open())
    {
        out4
            << "\nAmount points: \n" <<  vuxyzrgb.num_p << endl
            << "\nAmount of clusters that is stored: \n" << vuxyzrgb.num_cl << endl


            << "\nAmount points (arma): \n" <<  vuxyzrgb_arma.num_p << endl
            << "\nAmount of clusters that is stored (arma): \n" << vuxyzrgb_arma.num_cl << endl


            << endl;

    }
    out4.close();



    // =====================================
    //  сохранение данных о точках в текстовый файл
    // =====================================

    std::ofstream out3;          // поток для записи
    out3.open("C:/files/points_cloud22222.txt", ios_base::out); // окрываем файл для записи

    if (out3.is_open())

        for (int q = 0; q <  vuxyzrgb.num_p; q++)
        {

            out3 << vuxyzrgb.vu.at(q)[0] << " \t " << vuxyzrgb.vu.at(q)[1] << " \t "
                 << vuxyzrgb.xyz.at(q)[0] << " \t " << vuxyzrgb.xyz.at(q)[1] << " \t " << vuxyzrgb.xyz.at(q)[2] << " \t "
                 << vuxyzrgb.rgb.at(q)[0] << " \t " << vuxyzrgb.rgb.at(q)[1] << " \t " << vuxyzrgb.rgb.at(q)[2] << " \t "
                 << vuxyzrgb.cluster.at(q)<< endl;

        }


    out3.close();




}






cv::Mat ADS::cl_DenseStereo::getResultImage()
{
    //    // ===========
    //    // Mutex (lock)
    imgResult_mutex.lock();
    //    // ============

    cv::Mat img_temp = imgResult01r.clone();


    //    // ===========
    //    // Mutex (unlock)
    imgResult_mutex.unlock();
    //    // ============


    if (img_temp.empty()  )
    {


        img_temp =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(img_temp,
                    "getResultImage:: Image is Empty",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);


    }





    return img_temp;



}

void ADS::cl_DenseStereo::setResultImage(cv::Mat img)
{
    // ===========
    // Mutex (lock)
    imgResult_mutex.lock();
    // ============

    imgResult01r = img.clone();

    // ===========
    // Mutex (unlock)
    imgResult_mutex.unlock();
    // ============
}

Mat ADS::cl_DenseStereo::getShowImg()
{


    //    // ===========
    //    // Mutex (lock)
    imgShow_mutex.lock();
    //    // ============



    cv::Mat img_temp = imgShow.clone();

    //    // ===========
    //    // Mutex (unlock)
    imgShow_mutex.unlock();
    //    // ============


    if (img_temp.empty()  )
    {


        img_temp =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(img_temp,
                    "getShowImg:: Image is Empty",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);


    }



    return img_temp;

}

void ADS::cl_DenseStereo::setShowImg(cv::Mat img_temp)
{

    // ===========
    // Mutex (lock)
    imgShow_mutex.lock();
    // ============

    imgShow = img_temp.clone();

    // ===========
    // Mutex (unlock)
    imgShow_mutex.unlock();
    // ============

}



cv::Mat ADS::cl_DenseStereo::getDisparityMapImage()
{
    //    // ===========
    //    // Mutex (lock)
    imgDisparity_mutex.lock();
    //    // ============

    cv::Mat disp_norm_temp = disp_norm.clone();

    //    // ===========
    //    // Mutex (unlock)
    imgDisparity_mutex.unlock();
    //    // ============

    return disp_norm_temp;



}

cv::Mat ADS::cl_DenseStereo::getDisparityMapImage02()
{
    //    // ===========
    //    // Mutex (lock)
    imgResultDisparity_mutex.lock();
    //    // ============

    cv::Mat disp_norm_temp = imgResult_disp02.clone();


    //    // ===========
    //    // Mutex (unlock)
    imgResultDisparity_mutex.unlock();
    //    // ============


    if (disp_norm_temp.empty()  )
    {


        disp_norm_temp =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(disp_norm_temp,
                    "getDisparityMapImage02:: Image is Empty",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);


    }




    return disp_norm_temp;

}


cv::Mat ADS::cl_DenseStereo::getDisparityMapImage03()
{
    //    // ===========
    //    // Mutex (lock)
    imgResultDisparity_mutex.lock();
    //    // ============

    cv::Mat disp_norm_temp = imgResult_disp03.clone();

    //    // ===========
    //    // Mutex (unlock)
    imgResultDisparity_mutex.unlock();
    //    // ============

    return disp_norm_temp;


}

cv::Mat ADS::cl_DenseStereo::getResultPrimitiveGeneralImage()
{
    //    // ===========
    //    // Mutex (lock)
    imgPrimitives_mutex.lock();
    //    // ============

    cv::Mat finalImage_temp = imgPrimitive01r_general.clone();

    //    // ===========
    //    // Mutex (unlock)
    imgPrimitives_mutex.unlock();
    //    // ============

    return finalImage_temp;

}




cv::Mat ADS::cl_DenseStereo::getResultPrimitivesXYImage()
{
    //    // ===========
    //    // Mutex (lock)
    imgPrimitives_mutex.lock();
    //    // ============

    cv::Mat imgPrimitivesXY_temp = imgPrimitivesXY.clone();

    //    // ===========
    //    // Mutex (unlock)
    imgPrimitives_mutex.unlock();
    //    // ============

    return imgPrimitivesXY_temp;

}



cv::Mat ADS::cl_DenseStereo::getResultPrimitivesYZImage()
{
    //    // ===========
    //    // Mutex (lock)
    imgPrimitives_mutex.lock();
    //    // ============

    cv::Mat imgPrimitivesYZ_temp = imgPrimitivesYZ.clone();

    //    // ===========
    //    // Mutex (unlock)
    imgPrimitives_mutex.unlock();
    //    // ============

    return imgPrimitivesYZ_temp;

}


cv::Mat ADS::cl_DenseStereo::getResultPrimitivesXZImage()
{
    //    // ===========
    //    // Mutex (lock)
    imgPrimitives_mutex.lock();
    //    // ============

    cv::Mat imgPrimitivesXZ_temp = imgPrimitivesXZ.clone();

    //    // ===========
    //    // Mutex (unlock)
    imgPrimitives_mutex.unlock();
    //    // ============

    return imgPrimitivesXZ_temp;

}




cv::Mat ADS::cl_DenseStereo::getResultColorSearchImage()
{
    //    // ===========
    //    // Mutex (lock)
    imgResultColorSearch_mutex.lock();
    //    // ============

    cv::Mat imgResultColorSearch_temp = imgResultColorSearch.clone();

    //    // ===========
    //    // Mutex (unlock)
    imgResultColorSearch_mutex.unlock();
    //    // ============

    return imgResultColorSearch_temp;



}





cv::Mat ADS::cl_DenseStereo::getImg3Dpoins()
{
    //    // ===========
    //    // Mutex (lock)
    img3Dpoins_mutex.lock();
    //    // ============

    cv::Mat img_temp = img3Dpoins.clone();

    //    // ===========
    //    // Mutex (unlock)
    img3Dpoins_mutex.unlock();
    //    // ============


    if (img_temp.empty()  )
    {


        img_temp =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(img_temp,
                    "getImg3Dpoins:: Image is Empty",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);


    }




    return img_temp;



}





void ADS::cl_DenseStereo::Rotation3Dpoints(double angx, double angy, double angz, double tx, double ty, double tz, double Scale, double Dz)
{



    // ===========
    // Mutex  (lock)
    vuxyzrgb_3Dpoints_mutex.lock();
    // ============

     t_vuxyzrgb vuxyzrgb_3Dpoints_temp = vuxyzrgb_3Dpoints;

    // ===========
    // Mutex  (unlock)
    vuxyzrgb_3Dpoints_mutex.unlock();
    // ============


    cv::Mat imgResult01r_temp = getImgOpenCV_1left();




    // ===========
    // Mutex  (lock)
    CalibParam_mutex.lock();
    // ============
    cv::Mat M1_temp = M1;
    // ===========
    // Mutex  (unlock)
    CalibParam_mutex.unlock();
    // ============

    cv::Size2i imgSize =  imgResult01r_temp.size();

    cv::Mat img3Dpoins_temp= cv::Mat::zeros(imgSize, CV_8UC3);

    cv::Mat Rx =  Mat(3, 3, CV_64F);
    cv::Mat Ry =  Mat(3, 3, CV_64F);
    cv::Mat Rz =  Mat(3, 3, CV_64F);
    cv::Mat R =  Mat(3, 3, CV_64F);

    cv::Mat T =  Mat(3, 1, CV_64F);
    cv::Mat xyz  =  Mat(3, 1, CV_64F);
    cv::Mat xyz1  =  Mat(4, 1, CV_64F);
    cv::Mat uv1 =  Mat(3, 1, CV_64F);
    cv::Mat uv =  Mat(2, 1, CV_64F);

    cv::Mat P =  Mat(3, 4, CV_64F);


    vector <vector<double>> Rvec;
    Rvec.resize(3);


    // ang перевод в радианы

    double coefPI = 3.1416/180;
    angx = coefPI * angx;
    angy = coefPI * angy;
    angz = coefPI * angz;



    // T

    T.at<double>(0, 0) = tx;
    T.at<double>(1, 0) = ty;
    T.at<double>(2, 0) = tz;

    // Rx
    Rvec[0] = {1,          0,           0};
    Rvec[1] = {0,  cos(angx),  -sin(angx)};
    Rvec[2] = {0,  sin(angx),   cos(angx)};

    for (int i =0; i<3 ; i++)
        for (int j =0; j<3 ; j++)
            Rx.at<double>(i, j) = Rvec[i][j];


    //Ry
    Rvec[0] = { cos(angy),  0,  sin(angy)};
    Rvec[1] = {         0,  1,          0};
    Rvec[2] = {-sin(angy),  0,  cos(angy)};

    for (int i =0; i<3 ; i++)
        for (int j =0; j<3 ; j++)
            Ry.at<double>(i, j) = Rvec[i][j];


    //Rz
    Rvec[0] = {cos(angz),  -sin(angz),   0};
    Rvec[1] = {sin(angz),   cos(angz),   0};
    Rvec[2] = {        0,           0,   1};

    for (int i =0; i<3 ; i++)
        for (int j =0; j<3 ; j++)
            Rz.at<double>(i, j) = Rvec[i][j];


    // ======
    R = Rx * Ry * Rz;
    // ======




    // P
    Rvec.resize(3);
    Rvec[0] = {1, 0, 0, 0};
    Rvec[1] = {0, 1, 0, 0};
    Rvec[2] = {0, 0, 1, -Dz};

    for (int i =0; i<3 ; i++)
        for (int j =0; j<4 ; j++)
            P.at<double>(i, j) = Rvec[i][j];

    P =  M1_temp * P;

//    cout << "\t\t\t ||||||||||| 3Dpoints vuxyzrgb_temp.xyz0.size() = " << vuxyzrgb_3Dpoints_temp.xyz0.size() <<endl;
//    cout << "\t\t\t ||||||||||| 3Dpoints vuxyzrgb_temp.xyz.size() = " << vuxyzrgb_3Dpoints_temp.xyz.size() <<endl;
//    cout << "\t\t\t ||||||||||| vuxyzrgb_temp.num_p = " << vuxyzrgb_3Dpoints_temp.num_p <<endl;



    for (int q =0; q < int (vuxyzrgb_3Dpoints_temp.xyz0.size()); q++)
    {
        xyz.at<double>(0, 0) = Scale * vuxyzrgb_3Dpoints_temp.xyz0.at(q).at(0);
        xyz.at<double>(1, 0) = Scale * vuxyzrgb_3Dpoints_temp.xyz0.at(q).at(1);
        xyz.at<double>(2, 0) = Scale * vuxyzrgb_3Dpoints_temp.xyz0.at(q).at(2);


        xyz = R * (xyz + T);

        xyz1.at<double>(0, 0) = xyz.at<double>(0, 0);
        xyz1.at<double>(1, 0) = xyz.at<double>(1, 0);
        xyz1.at<double>(2, 0) = xyz.at<double>(2, 0);
        xyz1.at<double>(3, 0) = 1;


        uv1 = P * xyz1;
        uv.at<double>(0, 0) = uv1.at<double>(0, 0) / uv1.at<double>(2, 0);
        uv.at<double>(1, 0) = uv1.at<double>(1, 0) / uv1.at<double>(2, 0);

        int r,c;
        r = round(uv.at<double>(1, 0));
        c = round(uv.at<double>(0, 0));




        if  (  (r>0) && (c>0) && (r<imgSize.height) && (c<imgSize.width)  )
        {


            int u,v;

            v = vuxyzrgb_3Dpoints_temp.vu0.at(q).at(0);
            u = vuxyzrgb_3Dpoints_temp.vu0.at(q).at(1);
            img3Dpoins_temp.at<Vec3b>(r, c) = imgResult01r_temp.at<Vec3b>(v,u);


//            cv::Scalar color;

//            color[0] = vuxyzrgb_3Dpoints_temp.rgb0.at(q).at(0);
//            color[1] = vuxyzrgb_3Dpoints_temp.rgb0.at(q).at(1);
//            color[2] = vuxyzrgb_3Dpoints_temp.rgb0.at(q).at(2);

//                cv::circle	(img3Dpoins_temp,
//                           {v, u},  // coordinate
//                           3,  //radius
//                           color,     // color
//                           2,       // thickness,
//                           cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA



        }


    }






    //    // ===========
    //    // Mutex (lock)
    img3Dpoins_mutex.lock();
    //    // ============



    img3Dpoins = img3Dpoins_temp.clone();

    //img3Dpoins = img3Dpoins_temp;
    //    // ===========
    //    // Mutex (unlock)
    img3Dpoins_mutex.unlock();
    //    // ============




}




ADS::t_vuxyzrgb ADS::cl_DenseStereo::get_vuxyzrgb()
{



    //    // ===========
    //    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    //    // ============

    t_vuxyzrgb vuxyzrgb_temp = vuxyzrgb;

//    cout << "\t\t --->>> (0) getResultVector: vuxyzrgb.SelectedClaster: "<< vuxyzrgb.SelectedClaster  << endl;
//    cout << "\t\t --->>> (0) getResultVector: vuxyzrgb_temp.SelectedClaster: "<< vuxyzrgb_temp.SelectedClaster  << endl;

    //    // ===========
    //    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    //    // ============



    return vuxyzrgb_temp;


}



ADS::type_arma_vuxyzrgb ADS::cl_DenseStereo::get_vuxyzrgb_arma()
{
    //    // ===========
    //    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    //    // ============

    type_arma_vuxyzrgb vuxyzrgb_arma_temp = vuxyzrgb_arma;

    //    // ===========
    //    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    //    // ============



    return vuxyzrgb_arma_temp;

}


void ADS::cl_DenseStereo::set_vuxyzrgb(t_vuxyzrgb vuxyzrgb_temp)
{
    //    // ===========
    //    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    //    // ============

    ADS::cl_DenseStereo::vuxyzrgb = vuxyzrgb_temp;

    //    cout << "\t\t --->>> (0) getResultVector: vuxyzrgb.SelectedClaster: "<< vuxyzrgb.SelectedClaster  << endl;
    //    cout << "\t\t --->>> (0) getResultVector: vuxyzrgb_temp.SelectedClaster: "<< vuxyzrgb_temp.SelectedClaster  << endl;

    //    // ===========
    //    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    //    // ============

    //    // ============
}

void ADS::cl_DenseStereo::set_vuxyzrgb_arma(type_arma_vuxyzrgb vuxyzrgb_arma_temp)
{
    //    // ===========
    //    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    //    // ============

    ADS::cl_DenseStereo::vuxyzrgb_arma = vuxyzrgb_arma_temp;

    //    // ===========
    //    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    //    // ============
}








void ADS::cl_DenseStereo::DrawResultsInImage()
{

    // ===========
    // Mutex (lock)
    imgResult_mutex.lock();
    // ===========

    cv::Mat imgResult01r_temp2 = imgResult01r.clone();

    // ===========
    // Mutex (unlock)
    imgResult_mutex.unlock();
    // ===========





    cv::Mat outResult01r_temp;

    if (!imgResult01r_temp2.empty())
    {



        cv::resize(imgResult01r_temp2, outResult01r_temp, cv::Size( double(imgResult01r_temp2.cols * CoefShowWindow),
                                                                   double(imgResult01r_temp2.rows) * CoefShowWindow), 0, 0, cv::INTER_LINEAR);


        cv::imshow( "Window_imgResult01Left", outResult01r_temp );


        outResult01r = outResult01r_temp;
    }





    // ################################
    cv::waitKey(10);
    // ################################

}


void ADS::cl_DenseStereo::DrawResultsInImage_AlLIm(int selectorImage)
{

    // Получение изображения
    // первый входной аргумент фунции - это номер камеры:

    // 0 - StereoCam00left	 - левая камеры стереокамеры
    // 1 - StereoCam01right	 - правая камеры стереокамеры
    // 2 - HandCam02left 	 - камерана левой руке манипулятора
    // 3 - HandCam03right	 - камера на првавой руке манипулятора

    // 10 - Изображение с результатами - левая камера стереокамеры: imgResult01r;
    // 11 - Изображение катры глубин в цвете: imgResultColorSearch

    // 20 - ОБЩЕЕ изображение с результатами построения примитивов объетков: imgPrimitive01r_general;
    // 21 - изображение отдельной проекции примитивов: imgPrimitivesXY;
    // 22 - изображение отдельной проекции примитивов: imgPrimitivesYZ;
    // 23 - изображение отдельной проекции примитивов: imgPrimitivesXZ;

    // 30 - Изображение результатов поиска по цвету




    cv::Mat m;
    switch ( selectorImage )
    {
    case 0:


        // ===========
        // Mutex (lock)
        //img_mutex.lock();
        // ============

        m = getImgOpenCV_1left();

        // ===========
        // Mutex (unlock)
        //img_mutex.unlock();
        // ============






        break;
    case 1:


        // ===========
        // Mutex (lock)
        img_mutex.lock();
        // ============

        m = getImgOpenCV_2right();

        // ===========
        // Mutex (unlock)
        img_mutex.unlock();
        // ============




        break;
    case 2:
        m = getImgOpenCV_3leftHand();
        break;
    case 3:
        m = getImgOpenCV_4rightHand();
        break;


    case 10:
        m = getResultImage();
        break;
    case 11:
        m = getDisparityMapImage();
        break;




    case 20:
        m = getResultPrimitiveGeneralImage();
        break;
    case 21:
        m = getResultPrimitivesXYImage();
        break;
    case 22:
        m = getResultPrimitivesXZImage();
        break;
    case 23:
        m = getResultPrimitivesYZImage();
        break;

    case 30:
        m = getResultColorSearchImage();
        break;


    default:
        cout<<"Error, bad input, quitting\n";
        break;

    }





    cv::Mat outResult01r_temp = m;

    if (!m.empty())
    {



        cv::resize(m, outResult01r_temp, cv::Size( double(m.cols * CoefShowWindow),
                                                  double(m.rows) * CoefShowWindow), 0, 0, cv::INTER_LINEAR);


        cv::imshow( "Window_imgResult01Left", outResult01r_temp );


        outResult01r = outResult01r_temp;
    }





    // ################################
    cv::waitKey(10);
    // ################################

}


vector<int> ADS::cl_DenseStereo::getMouseClickCoordinates02()
{
    return MouseClickCoordinates02;
}


void ADS::cl_DenseStereo::setMouseClickCoordinates02(int u, int v)
{
    MouseClickCoordinates02.at(0) = u ;
    MouseClickCoordinates02.at(1) = v ;

}


static void onMouse( int event, int x, int y, int, void* ptr)
{



    switch( event )
    {
    case EVENT_MOUSEMOVE:
        break;

    case EVENT_LBUTTONDOWN:

        //        cout << " <<<<<<<<<<<<<<< Mouse click >>>>>>>>>>>>>>>>> " << endl;
        //        cout << " x: "<< x << endl;
        //        cout << " y: "<< y << endl;



        static_cast<vector<int>*>(ptr)->at(0) = x;
        static_cast<vector<int>*>(ptr)->at(1) = y;

        break;

    case EVENT_LBUTTONUP:
        break;
    }


}


void ADS::cl_DenseStereo::eventMouse_SelectObject(int selector)
{



    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура
    int mouse_u_temp, mouse_v_temp;
    cv::Point3d previousCenter3D_temp;


    // ===========
    // Mutex (lock)
    std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============

    vuxyzrgb_temp = vuxyzrgb;
    vuxyzrgb_arma_temp = vuxyzrgb_arma;

    mouse_u_temp = mouse_u;
    mouse_v_temp = mouse_v;

    previousCenter3D_temp =  previousCenter3D;

    // ===========
    // Mutex (unlock)
    mouse_mutex.unlock();
    vuxyzrgb_mutex.unlock();
    // ============




    //================
    // Проверка
    //================
    if ( vuxyzrgb_temp.xyz.empty() ) return;




    if (selector == 0)
    {
        cv::setMouseCallback( "Window_imgResult01Left", onMouse, ptr_MouseClickCoordinates);
        MouseClickCoordinates.at(0) = ptr_MouseClickCoordinates->at(0) / CoefShowWindow;
        MouseClickCoordinates.at(1) = ptr_MouseClickCoordinates->at(1) / CoefShowWindow;
    }
    else if (selector == 1)
    {
        // ? исправлено ??  или "-" для корекции так как координаты вводятся в отрицательном значении - так реализована логигка алгоритма (временнно)
        MouseClickCoordinates.at(0) = MouseClickCoordinates02.at(0);
        MouseClickCoordinates.at(1) = MouseClickCoordinates02.at(1);

    }

    //================
    // Проверка на новый клик мыши
    //================

    if ( MouseCheck.at(0) != MouseClickCoordinates.at(0) || MouseCheck.at(1)  != MouseClickCoordinates.at(1) )
    {

        // Переписывае предидущие положение мышы
        MouseCheck.at(0) = MouseClickCoordinates.at(0);
        MouseCheck.at(1) = MouseClickCoordinates.at(1);


        //        cout << "\t >>>>> new MouseCheck.at(0): " << MouseCheck.at(0) << endl;
        //        cout << "\t >>>>> new MouseCheck.at(1): " << MouseCheck.at(1) << endl;

        // Переписывае предидущие положения центра кластера (объекта) в том месте, где кликнет мышка
        mouse_u_temp = MouseClickCoordinates.at(0);
        mouse_v_temp = MouseClickCoordinates.at(1);







        //================
        // Проверка на новый клик мыши
        //================





        //================
        // Выделяем кластеры после изменения объекта интереса - клик мыши на др. объект
        //================
        // Перенаходим индекс выделенного кластера
        // Переписываем предыдущего центра кластера



        // Сброс выбора кластера - после клика мышкой
        vuxyzrgb_temp.SelectedClaster = -1;
        vuxyzrgb_arma_temp.SelectedClaster = -1;

        // cout << "\t\t >>>>> eventMouse_SelectObject: 02 " << endl;
        //================
        // Проверка на пустоту
        //================
        if ( vuxyzrgb_temp.num_cl == 0) return;


        // for (int qc = 0; qc <  int( vuxyzrgb_temp.num_cl ); qc++)
        for (int qc = 0; qc <  int( vuxyzrgb_temp.objRect2D.size() ); qc++)
        {
            cv::Point vu_min,vu_max, Center2D;
            vu_min.x = vuxyzrgb_temp.objRect2D[qc][0];
            vu_min.y = vuxyzrgb_temp.objRect2D[qc][1];

            vu_max.x = vuxyzrgb_temp.objRect2D[qc][2];
            vu_max.y = vuxyzrgb_temp.objRect2D[qc][3];

            Center2D.x = vuxyzrgb_temp.objRect2D[qc][4];
            Center2D.y = vuxyzrgb_temp.objRect2D[qc][5];


            cv::Point3d xyz_min, xyz_max, Center3D;
            xyz_min.x = vuxyzrgb_temp.objRect3D[qc][0];
            xyz_min.y = vuxyzrgb_temp.objRect3D[qc][1];
            xyz_min.z = vuxyzrgb_temp.objRect3D[qc][2];

            xyz_max.x = vuxyzrgb_temp.objRect3D[qc][3];
            xyz_max.y = vuxyzrgb_temp.objRect3D[qc][4];
            xyz_max.z = vuxyzrgb_temp.objRect3D[qc][5];

            Center3D.x = vuxyzrgb_temp.objRect3D[qc][6];
            Center3D.y = vuxyzrgb_temp.objRect3D[qc][7];
            Center3D.z = vuxyzrgb_temp.objRect3D[qc][8];



            //================
            // Проверка на новый клик мыши по 2D кординатам
            //================


            if ((MouseClickCoordinates.at(0) > button01Size.x &&
                 MouseClickCoordinates.at(1) > button01Size.y) &&
                (mouse_u_temp > vu_min.x && mouse_u_temp < vu_max.x) &&
                (mouse_v_temp > vu_min.y && mouse_v_temp < vu_max.y)  )

            {
                // Перенаходим индекс выделенного кластера

                vuxyzrgb_temp.SelectedClaster = qc;
                vuxyzrgb_arma_temp.SelectedClaster = qc;

                // Коректировка указателя мышы - перенос его в центр кластера
                mouse_u_temp = Center2D.x;
                mouse_v_temp = Center2D.y;



                previousCenter3D_temp.x = Center3D.x;
                previousCenter3D_temp.y = Center3D.y;
                previousCenter3D_temp.z = Center3D.z;


                break;

            }
            else
            {
                // Сброс выбора кластера - после клика мышкой
                vuxyzrgb_temp.SelectedClaster = -1;
                vuxyzrgb_arma_temp.SelectedClaster = -1;
            }



            //================
            // Проверка на новый клик мыши по 3D кординатам
            //================

            if (MouseClickCoordinates.at(0) > button01Size.x &&
                MouseClickCoordinates.at(1) > button01Size.y &&

                (previousCenter3D_temp.x > Center3D.x - rad && previousCenter3D_temp.x < Center3D.x + rad) &&

                (previousCenter3D_temp.y > Center3D.y - rad && previousCenter3D_temp.y < Center3D.y + rad) &&

                (previousCenter3D_temp.z > Center3D.z - rad && previousCenter3D_temp.z < Center3D.z + rad) )

            {

                // Перенаходим индекс выделенного кластера

                vuxyzrgb_temp.SelectedClaster = qc;
                vuxyzrgb_arma_temp.SelectedClaster = qc;

                // Переписываем предыдущего центра кластера
                previousCenter3D_temp.x = Center3D.x;
                previousCenter3D_temp.y = Center3D.y;
                previousCenter3D_temp.z = Center3D.z;


            }
            else
            {
                // Сброс выбора кластера - после клика мышкой
                vuxyzrgb_temp.SelectedClaster = -1;
                vuxyzrgb_arma_temp.SelectedClaster = -1;
            }





        }

      //   cout << "\t\t >>>>> eventMouse_SelectObject: 04 " << endl;


    }

    //================
    // Проверка на пустоту
    //================
    if ( vuxyzrgb_temp.num_cl == 0) return;





    //================
    // Реализация слежения за кластерами (объектами)
    //================

    // for (int qc = 0; qc <  int( vuxyzrgb_temp.num_cl ); qc++)
    for (int qc = 0; qc <  int( vuxyzrgb_temp.objRect2D.size() ); qc++)
    {

        // Крайние точки выделяющей объект рамки в 2D и 3D ( min. и max)
        cv::Point vu_min,vu_max, Center2D;
        vu_min.x = vuxyzrgb_temp.objRect2D[qc][0];
        vu_min.y = vuxyzrgb_temp.objRect2D[qc][1];

        vu_max.x = vuxyzrgb_temp.objRect2D[qc][2];
        vu_max.y = vuxyzrgb_temp.objRect2D[qc][3];

        Center2D.x = vuxyzrgb_temp.objRect2D[qc][4];
        Center2D.y = vuxyzrgb_temp.objRect2D[qc][5];

        cv::Point3d xyz_min, xyz_max, Center3D;
        xyz_min.x = vuxyzrgb_temp.objRect3D[qc][0];
        xyz_min.y = vuxyzrgb_temp.objRect3D[qc][1];
        xyz_min.z = vuxyzrgb_temp.objRect3D[qc][2];

        xyz_max.x = vuxyzrgb_temp.objRect3D[qc][3];
        xyz_max.y = vuxyzrgb_temp.objRect3D[qc][4];
        xyz_max.z = vuxyzrgb_temp.objRect3D[qc][5];

        Center3D.x = vuxyzrgb_temp.objRect3D[qc][6];
        Center3D.y = vuxyzrgb_temp.objRect3D[qc][7];
        Center3D.z = vuxyzrgb_temp.objRect3D[qc][8];

        cv::Point3d Dxyz;
        Dxyz.x = xyz_max.x - xyz_min.x;
        Dxyz.y = xyz_max.y - xyz_min.y;
        Dxyz.z = xyz_max.z - xyz_min.z;


        //================
        // Слежение за кластером по 2D кординатам
        //================
        // Находим индекс выделенного кластера
        //  Проверка попадания клика мышки внутрь 2D рамки выделяющей кластер


        //        if ( (MouseClickCoordinates.at(0) > button01Size.x &&  MouseClickCoordinates.at(1) > button01Size.y ) &&
        //            ( mouse_u_temp > vu_min.x && mouse_u_temp < vu_max.x) &&
        //            ( mouse_v_temp > vu_min.y &&  mouse_v_temp < vu_max.y )  )

        //        {
        //            //================
        //            // Слежение за кластером по 2D кординатам
        //            //================
        //            // Переписываем предыдущего центра кластера
        //            mouse_u_temp = Center2D.x;
        //            mouse_v_temp = Center2D.y;

        //            vuxyzrgb_temp.SelectedClaster = qc;
        //            vuxyzrgb_arma_temp.SelectedClaster = qc;

        //            break;
        //        }
        //        else
        //        {
        //            // Сброс выбора кластера - после клика мышкой на кнопку на экране
        //            vuxyzrgb_temp.SelectedClaster = -1;
        //            vuxyzrgb_arma_temp.SelectedClaster = -1;

        //        }



        //================
        // Слежение за кластером по 3D кординатам
        //================

        //        cout << "\t\t >>>>>>>> qc: " << qc << endl;
        //        cout << "\t\t >>>>>>>> (MouseClickCoordinates.at(0) > button01Size.x &&  MouseClickCoordinates.at(1) > button01Size.y ): "
        //             <<  (MouseClickCoordinates.at(0) > button01Size.x &&  MouseClickCoordinates.at(1) > button01Size.y ) << endl;


        //        cout << "\t\t >>>>>>>> (previousCenter3D_temp.x > Center3D.x - rad && previousCenter3D_temp.x < Center3D.x + rad) : "
        //             << (previousCenter3D_temp.x > Center3D.x - rad && previousCenter3D_temp.x < Center3D.x + rad)  << endl;

        //        cout << "\t\t >>>>>>>> (previousCenter3D_temp.y > Center3D.y - rad && previousCenter3D_temp.y < Center3D.y + rad) "
        //             << (previousCenter3D_temp.y > Center3D.y - rad && previousCenter3D_temp.y < Center3D.y + rad)  << endl;

        //        cout << "\t\t >>>>>>>> (previousCenter3D_temp.z > Center3D.z - rad && previousCenter3D_temp.z < Center3D.z + rad) : "
        //             << (previousCenter3D_temp.z > Center3D.z - rad && previousCenter3D_temp.z < Center3D.z + rad)  << endl;







        //        if ((MouseClickCoordinates.at(0) > button01Size.x &&  MouseClickCoordinates.at(1) > button01Size.y ) &&
        //            (previousCenter3D_temp.x > Center3D.x - rad && previousCenter3D_temp.x < Center3D.x + rad) &&
        //            (previousCenter3D_temp.y > Center3D.y - rad && previousCenter3D_temp.y < Center3D.y + rad) &&
        //            (previousCenter3D_temp.z > Center3D.z - rad && previousCenter3D_temp.z < Center3D.z + rad) )

        if ((MouseClickCoordinates.at(0) > button01Size.x &&  MouseClickCoordinates.at(1) > button01Size.y ) &&
            (previousCenter3D_temp.x > Center3D.x -  Dxyz.x && previousCenter3D_temp.x < Center3D.x +  Dxyz.x) &&
            (previousCenter3D_temp.y > Center3D.y -  Dxyz.y && previousCenter3D_temp.y < Center3D.y +  Dxyz.y) &&
            (previousCenter3D_temp.z > Center3D.z -  Dxyz.z && previousCenter3D_temp.z < Center3D.z +  Dxyz.z) )
        {
            // Переписываем предыдущего центра кластера
            mouse_u_temp = Center2D.x;
            mouse_v_temp = Center2D.y;

            previousCenter3D_temp.x = Center3D.x;
            previousCenter3D_temp.y = Center3D.y;
            previousCenter3D_temp.z = Center3D.z;

            vuxyzrgb_temp.SelectedClaster = qc;
            vuxyzrgb_arma_temp.SelectedClaster = qc;

            break;

        }
        else
        {
            // Сброс выбора кластера - после клика мышкой
            vuxyzrgb_temp.SelectedClaster = -1;
            vuxyzrgb_arma_temp.SelectedClaster = -1;

        }




    }


    //        cout << "\t\t >>>>>>>> vuxyzrgb_temp.SelectedClaster: " << vuxyzrgb_temp.SelectedClaster << endl;
    //        cout << "\t\t >>>>>>>> vuxyzrgb_arma_temp.SelectedClaster: " << vuxyzrgb_arma_temp.SelectedClaster << endl;
    //        cout << "\t\t >>>>> mouse_u: " << mouse_u_temp << endl;
    //        cout << "\t\t >>>>> mouse_v: " << mouse_v_temp << endl;


    //    if (vuxyzrgb_temp.SelectedClaster > -1)

    //    {
    //                cout << "\t\t >>>>>>>> Center2D.x: " << vuxyzrgb_temp.objRect2D[vuxyzrgb_temp.SelectedClaster][4] << endl;
    //                cout << "\t\t >>>>>>>> Center2D.y: " << vuxyzrgb_temp.objRect2D[vuxyzrgb_temp.SelectedClaster][5] << endl;
    //                cout << "\t\t >>>>>>>> ---------: "  << endl;
    //                cout << "\t\t >>>>>>>> Center3D.x: " << vuxyzrgb_temp.objRect3D[vuxyzrgb_temp.SelectedClaster][6] << endl;
    //                cout << "\t\t >>>>>>>> Center3D.y: " << vuxyzrgb_temp.objRect3D[vuxyzrgb_temp.SelectedClaster][7] << endl;
    //                cout << "\t\t >>>>>>>> Center3D.z: " << vuxyzrgb_temp.objRect3D[vuxyzrgb_temp.SelectedClaster][8] << endl;





    //    }
    //        cout << "\t\t >>>>>>>> ---------: "  << endl;
    //        cout << "\t\t >>>>>>>> previousCenter3D_temp.x: " << previousCenter3D_temp.x << endl;
    //        cout << "\t\t >>>>>>>> previousCenter3D_temp.y: " << previousCenter3D_temp.y << endl;
    //        cout << "\t\t >>>>>>>> previousCenter3D_temp.z: " << previousCenter3D_temp.z << endl;

    //        cout << "\t\t >>>>> MouseClickCoordinates.at(0): " << MouseClickCoordinates.at(0) << endl;
    //        cout << "\t\t >>>>> MouseClickCoordinates.at(1): " << MouseClickCoordinates.at(1) << endl;

    //        cout << "\t\t >>>>> MouseCheck.at(0): " << MouseCheck.at(0) << endl;
    //        cout << "\t\t >>>>> MouseCheck.at(1): " << MouseCheck.at(1) << endl;






     //    cout << "\t\t >>>>> eventMouse_SelectObject: 05 " << endl;





     cout << "\t\t --->>> (-3) eventMouse_SelectObject: vuxyzrgb_temp.SelectedClaster: "<< vuxyzrgb_temp.SelectedClaster  << endl;


     if (vuxyzrgb_temp.SelectedClaster > vuxyzrgb_temp.num_cl) vuxyzrgb_temp.SelectedClaster = vuxyzrgb_temp.num_cl;

    // ===========
    // Mutex (lock)
    std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============


    int temp = vuxyzrgb_temp.SelectedClaster;
    vuxyzrgb.SelectedClaster = temp; //  to vector type
    vuxyzrgb_arma.SelectedClaster = temp; //  to armadillo type


    mouse_u = mouse_u_temp;
    mouse_v = mouse_v_temp;


    previousCenter3D =  previousCenter3D_temp;


    // ===========
    // Mutex (unlock)
    mouse_mutex.unlock();
    vuxyzrgb_mutex.unlock();
    // ============




}


void ADS::cl_DenseStereo::setWorkingAria(vector<double> limit)
{

    limit_WorkingArea = limit;


    // ===========
    // Mutex  (lock)
    vuxyzrgb_3Dpoints_mutex.lock();
    // ============


    limit_WorkingArea = limit;


    vuxyzrgb_3Dpoints.limit_WorkingArea = limit;


    // ===========
    // Mutex  (unlock)
    vuxyzrgb_3Dpoints_mutex.unlock();
    // ============


}


void ADS::cl_DenseStereo::setParametersSBM(
    int NumDisparities,
    int BlockSize,
    int PreFilterCap,
    int MinDisparity,
    int TextureThreshold,
    int UniquenessRatio,
    int SpeckleWindowSize,
    int SpeckleRange,
    int Disp12MaxDiff
    )
{

    smbNumDisparities = NumDisparities;   /**< Range of disparity */ // ndisparities = 16*43
    smbBlockSize = BlockSize; /**< Size of the block window. Must be odd */ //SADWindowSize = 21
    smbPreFilterCap = PreFilterCap;        // 31 -> 15 +++
    smbMinDisparity = MinDisparity;
    smbTextureThreshold = TextureThreshold;    // 10   +++
    smbUniquenessRatio = UniquenessRatio;     // 15   +++
    smbSpeckleWindowSize = SpeckleWindowSize;  // 100 +++
    smbSpeckleRange = SpeckleRange;        // 32
    smbDisp12MaxDiff = Disp12MaxDiff;

}



void ADS::cl_DenseStereo::Clustering()
{

    if ( vuxyzrgb.xyz.empty() ) return;



    // ===========
    // Mutex
    vuxyzrgb_mutex.lock();
    // ============

    // Ввод данных
    vector<vector<double>> xyz = vuxyzrgb.xyz;

    // ===========
    // Mutex
    vuxyzrgb_mutex.unlock();
    // ============





    // Инициализация
    double coef = 0.2;

    vector<double> x3D, y3D, z3D;

    size_t Len3D = int( xyz.size() );
    for (size_t i = 0; i < Len3D; i++) {
        x3D.push_back(xyz[i][0]);
        y3D.push_back(xyz[i][1]);
        z3D.push_back(xyz[i][2]);
    }


    // Создание Матрицы расстояний
    vector<vector<double>> S(Len3D, std::vector<double>(Len3D, 0));
    // Расчет расстояний
    double sumOfS = 0.0;
    vector<size_t> i1(Len3D, 0);
    for (size_t i = 0; i < Len3D; i++)
        i1[i] = i;




#if defined(_WIN64)
    // ================
    // Only Windows
    // ================

    combinable<double> sumS;
    Concurrency::parallel_for(size_t(0), Len3D, [&](size_t i) {
        std::for_each(i1.begin(), i1.end(), [&](size_t j) {
            S[i][j] = sqrt(pow((x3D[i] - x3D[j]), 2) + pow((y3D[i] - y3D[j]), 2) + pow((z3D[i] - z3D[j]), 2));
            sumS.local() += S[i][j];
        });
    });

    sumOfS = sumS.combine(plus<double>());




    // Нормировка матрицы
    double S2 = sumOfS / double(pow(Len3D, 2));
    vector<vector<double>> S3(Len3D, std::vector<double>(Len3D, 0.0));

    Concurrency::parallel_for_each(i1.begin(), i1.end(), [&](size_t i) {
        std::for_each(i1.begin(), i1.end(), [&](size_t j) {
            S3[i][j] = S[i][j] / S2;
        });
    });



    vector<vector<bool>> L(Len3D, std::vector<bool>(Len3D, false));

    Concurrency::parallel_for_each(i1.begin(), i1.end(), [&](size_t i) {
        std::for_each(i1.begin(), i1.end(), [&](size_t j) {
            if (S3[i][j] < coef)
                L[i][j] = true;
        });
    });



// ================
// Only Windows END
// ================

#else

    // ================
    // Only Linux
    // ================

    // 01
    double sumS = 0.0;

    for (size_t i = 0; i < Len3D; i++)
        for (size_t j = 0; j < Len3D; j++)
        {
            S[i][j] = sqrt(pow((x3D[i] - x3D[j]), 2) + pow((y3D[i] - y3D[j]), 2) + pow((z3D[i] - z3D[j]), 2));
            sumS += S[i][j];
        }


    // Нормировка матрицы
    double S2 = sumOfS / double(pow(Len3D, 2));
    vector<vector<double>> S3(Len3D, std::vector<double>(Len3D, 0.0));



    // 02

    for (size_t i = 0; i < Len3D; i++)
        for (size_t j = 0; j < Len3D; j++)
        {
            S3[i][j] = S[i][j] / S2;
        }



    vector<vector<bool>> L(Len3D, std::vector<bool>(Len3D, false));

    // 03


    for (size_t i = 0; i < Len3D; i++)
        for (size_t j = 0; j < Len3D; j++)
        {
            if (S3[i][j] < coef)
                L[i][j] = true;
        }



// ================
// Only Linux END
// ================

#endif






    vector<int> IDX(Len3D, -1);
    vector< vector<vector<double>>> clastersData;
    int numClaster = 0, ID_current = 0;
    vector<int> IDs_check, IDs_clusters, IDs_lost;
    IDs_check.push_back(0);

    for (int i = 0; i < int( Len3D ); i++) {
        if (L[0][i])
            IDs_clusters.push_back(i);
        else
            IDs_lost.push_back(i);
    }


    while (!IDs_lost.empty())
    {
        ID_current = IDs_lost[0];
        IDs_clusters.clear();

        for (int i = 0; i < int( Len3D) ; i++)
            if (L[ID_current][i])
                IDs_clusters.push_back(i);

        vector<int> VectDiff;
        for (int i = 0; i < int( IDs_clusters.size() ); i++) {
            vector<int>::iterator it;
            it = find(IDs_check.begin(), IDs_check.end(), IDs_clusters[i]);
            if (it == IDs_check.end())
                VectDiff.push_back(IDs_clusters[i]);
        }

        while (!VectDiff.empty())
        {
            ID_current = VectDiff[0];

            for (int i = 0; i < int( Len3D ); i++)
                if (L[ID_current][i]) {
                    vector<int>::iterator it;
                    it = find(IDs_clusters.begin(), IDs_clusters.end(), i);
                    if (it == IDs_clusters.end())
                        IDs_clusters.push_back(i);
                }


            vector<int>::iterator it1;
            it1 = find(IDs_check.begin(), IDs_check.end(), ID_current);
            if (it1 == IDs_check.end())
                IDs_check.push_back(ID_current);


            VectDiff.clear();
            for (int i = 0; i <int( IDs_clusters.size() ); i++) {
                vector<int>::iterator it;
                it = find(IDs_check.begin(), IDs_check.end(), IDs_clusters[i]);
                if (it == IDs_check.end())
                    VectDiff.push_back(IDs_clusters[i]);
            }
        }

        VectDiff.clear();
        for (int i = 0; i < int( IDs_lost.size() ); i++) {
            vector<int>::iterator it;
            it = find(IDs_clusters.begin(), IDs_clusters.end(), IDs_lost[i]);
            if (it == IDs_clusters.end())
                VectDiff.push_back(IDs_lost[i]);
        }
        IDs_lost.clear();
        IDs_lost = VectDiff;




        //        if ( int( IDs_clusters.size() ) > -1 )
        //        {

        vector<vector<double>> xyz2;

        for (int i = 0; i < int( IDs_clusters.size() ); i++)
        {
            IDX[IDs_clusters[i]] = numClaster;
            xyz2.push_back(xyz[IDs_clusters[i]]);

        }
        clastersData.push_back(xyz2);

        numClaster++;
        //        }

    }

    //==============================================
    // Дополнительно
    //==============================================

    // Определение количесва кластеров
    int numClusters = 0;
    for (int i = 0; i < int( IDX.size() ); i++)
        if (IDX[i] > numClusters)
            numClusters = IDX[i];


    //
    vector<vector<size_t>> numPoints_Into_ClusterID;
    vector<vector<vector<double>>> clastersData2;

    for (int q = 0; q < numClusters; q++)
    {

        vector<size_t> numPoints;
        vector<vector<double>> xyz2;

        for (int i = 0; i < int( IDX.size() ); i++)
            if (IDX[i] == q)
            {
                numPoints.push_back(i);
                xyz2.push_back( xyz[i] );

            }

        numPoints_Into_ClusterID.push_back(numPoints);
        clastersData2.push_back(xyz2);


    }


    // ===========
    // Mutex
    vuxyzrgb_mutex.lock();
    // ============


    // Вывод данных
    vuxyzrgb.num_cl = numClusters;
    vuxyzrgb.cluster = IDX;
    vuxyzrgb.clusters = numPoints_Into_ClusterID;

    // Convertion to armadillo type
    vuxyzrgb_arma = conv_std2arma (vuxyzrgb);


    // ===========
    // Mutex
    vuxyzrgb_mutex.unlock();
    // ============

}


void ADS::cl_DenseStereo::searchColor()
{


    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура



    // Параметры цвета по которым происходит поиск объектов по цвету
    vector<int> limit_searchColor {0, 110, 130, 25, 150, 170};
    vector<int>  MouseClickCoordinates_temp {0, 0};

    // Загрузка исходного изображения
    cv::Mat imgLeft_temp = getImgOpenCV_1left();
    cv::Mat imgRight_temp = getImgOpenCV_2right();

    // ===========
    // Mutex (lock)
    std::lock(mouse_mutex, vuxyzrgb_mutex);
    // ============

    vuxyzrgb_temp = vuxyzrgb;
    vuxyzrgb_arma_temp = vuxyzrgb_arma;

    MouseClickCoordinates_temp[0] = MouseClickCoordinates.at(0);
    MouseClickCoordinates_temp[1] = MouseClickCoordinates.at(1);





    // ===========
    // Mutex (unlock)
    mouse_mutex.unlock();
    vuxyzrgb_mutex.unlock();
    // ============


    // Ректификация (выравнивание) изображения- Rectify
    cv::Mat img01r_temp, img02r_temp;
    cv::remap( imgLeft_temp, img01r_temp, map11, map12,
              cv::INTER_LINEAR );
    cv::remap( imgRight_temp, img02r_temp, map21, map22,
              cv::INTER_LINEAR );

    // cout <<  "\t ////////////////  searchColor(): (0d) "<< endl;
    // Инициализация выходного изображения
    cv::Mat imgResultColorSearch_temp = img01r_temp.clone();


    // Перевод в пространсво HSV
    cv::Mat  HSV_left, HSV_right, thresholdL, thresholdR, blurred;
    cv::cvtColor(img01r_temp, HSV_left, COLOR_BGR2HSV);
    cv::cvtColor(img02r_temp, HSV_right, COLOR_BGR2HSV);

    //======================================
    // Выбор цвета объекта по клику мыши
    //======================================

    //  cout << "\t ////////////////  searchColor(): MouseClick: v =  " << MouseClickCoordinates_temp[0] << ", u = "<< MouseClickCoordinates_temp[1]   << endl;
    //  cout << "\t ////////////////  searchColor(): MouseCheck: v =  " << MouseCheck_searchColor.at(0) << ", u = "<<  MouseCheck_searchColor.at(1)  << endl;
    //  cout << "\t ////////////////  searchColor(): MouseCheck: " << ( MouseCheck_searchColor.at(0) != MouseClickCoordinates_temp[0] || MouseCheck_searchColor.at(1)  != MouseClickCoordinates_temp[1]  )  << endl;

    if ( MouseCheck_searchColor.at(0) != MouseClickCoordinates_temp[0]  || MouseCheck_searchColor.at(1)  != MouseClickCoordinates_temp[1]  )
    {
        SelectColor = HSV_left.at<cv::Vec3b>(MouseClickCoordinates_temp[1] ,MouseClickCoordinates_temp[0] );
        MouseCheck_searchColor.at(0) = MouseClickCoordinates_temp[0] ;
        MouseCheck_searchColor.at(1) = MouseClickCoordinates_temp[1] ;
    }


    //  cout << "\t ////////////////  searchColor(): SelectColor: " << int (SelectColor[0]) << ", "<< int (SelectColor[1]) << ", " << int (SelectColor[2]) << endl;

    // Параметры цвета по которым происходит поиск объектов по цвету
    // int min = 0, max = 1000;1
    int hmin = limit_searchColor[0];
    int smin = limit_searchColor[1];
    int vmin = limit_searchColor[2];
    int hmax = limit_searchColor[3];
    int smax = limit_searchColor[4];
    int vmax = limit_searchColor[5];


    // Ограничения по пространсвам HSV
    int h_rang = 10;
    int s_rang = 60;
    int v_rang = 60;

    if ( SelectColor[0] - h_rang < 0 ) hmin = 0 ;
    else hmin = SelectColor[0] - h_rang ;

    if ( SelectColor[1] - s_rang < 0 ) smin = 0 ;
    else smin = SelectColor[1] - s_rang ;

    if ( SelectColor[2] - v_rang < 0 ) vmin = 0 ;
    else vmin = SelectColor[2] - v_rang ;

    if ( SelectColor[0] + h_rang > 255 ) hmax = 255 ;
    else hmax = SelectColor[0] + h_rang ;

    if (  SelectColor[1] + s_rang > 255 ) smax = 255 ;
    else smax = SelectColor[1] + s_rang ;

    if ( SelectColor[2] + v_rang > 255 ) vmax = 255 ;
    else vmax = SelectColor[2] + v_rang ;

    limit_searchColor[0] = hmin;
    limit_searchColor[1] = smin;
    limit_searchColor[2] = vmin;
    limit_searchColor[3] = hmax;
    limit_searchColor[4] = smax;
    limit_searchColor[5] = vmax;



    //cout << "HSV_left: " << HSV_left.col(1058).row(854) << endl;

    // cout <<  "\t ////////////////  searchColor(): HSV: " <<  hmin << ", "<< smin << ", " << vmin << ", " << hmax << ", " << smax << ", " << vmax << ", " << endl;

    vector<vector<Point>> Contours_left, Contours_right;

    //===========================
    // Работа с левым изображением
    //===========================

    cv::inRange(HSV_left, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), thresholdL);
    cv::findContours(thresholdL, Contours_left, RETR_TREE, CHAIN_APPROX_SIMPLE);


    if (int( Contours_left.size() ) > 0)
    {
        int area = 0;
        int idx = 0;
        for (int i = 0; i < int( Contours_left.size() ); i++)
        {
            if (area < int( Contours_left[i].size() ) )
            {
                idx = i;
                area = Contours_left[i].size();
            }
        }

        // cout <<  "\t //////////////// searchColor(): Contours_left.size(): " <<  Contours_left.size() << endl;
        //  cout <<  "\t ////////////////  searchColor(): area " <<  area << endl;
        //  cout <<  "\t ////////////////  searchColor(): idx: " << idx << endl;

        // Получение рамки ограничивающей объект
        Rect rect = boundingRect(Contours_left[idx]);

        // координаты центра для левого изображения
        Point2d ptleft0;
        vector<Point2d> ptleft;


        ptleft0.x = rect.x + rect.width / 2;
        ptleft0.y = rect.y + rect.height / 2;
        ptleft.push_back(ptleft0);

        // координаты рамки
        Point2d pt1, pt2, ptc;
        pt1.x = rect.x;
        pt1.y = rect.y;
        pt2.x = rect.x + rect.width;
        pt2.y = rect.y + rect.height;


        //===========================
        // Прорисовка на изображении
        //===========================
        cv::rectangle(imgResultColorSearch_temp, pt1, pt2, CV_RGB(0, 255, 0), 3);
        cv::circle(imgResultColorSearch_temp, ptc, 1, Scalar(0, 255, 0), 2);

        //===========================
        //===========================

        //    //===========================
        //    // Работа с правым изображением
        //    //===========================


        //    cv::inRange(HSV_right, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), thresholdR);
        //    cv::findContours(thresholdR, Contours_right, RETR_TREE, CHAIN_APPROX_SIMPLE);



        //    area = 0;
        //    idx = 0;
        //    for (int i = 0;  i < int(Contours_right.size()); i++)
        //    {
        //        if (area < int(Contours_right[i].size() ) )
        //        {
        //            idx = i;
        //            area = Contours_right[i].size();
        //        }
        //    }


        //    // Получение рамки ограничивающей объект
        //    rect = boundingRect(Contours_right[idx]);



        //     // координаты центра для правого изображения
        //    Point2d ptright0;
        //    vector<Point2d> ptright;



        //    ptright0.x = rect.x + rect.width / 2;
        //    ptright0.y = rect.y + rect.height / 2;
        //    ptright.push_back(ptright0);




        //    // координаты рамки
        //    pt1.x = rect.x;
        //    pt1.y = rect.y;
        //    pt2.x = rect.x + rect.width;
        //    pt2.y = rect.y + rect.height;
        //    ptc.x = rect.x + rect.width / 2;
        //    ptc.y = rect.y + rect.height / 2;



        //Vec3d Coords3D;

        //    //===========================
        //    // Триангуляция
        //    //===========================
        //    Mat triangCoords4D;
        //    cv::triangulatePoints(P1, P2, ptleft, ptright, triangCoords4D);

        //    Vec4f triangCoords1 = triangCoords4D.col(0);
        //    for (unsigned int i = 0; i < 3; i++)
        //    {
        //        Coords3D[i] = triangCoords1[i] / triangCoords1[3];
        //    }



        //    // cout << "Coords3D: " << Coords3D << endl;
        //    //cout << "HSV_left: " << HSV_left.col(1058).row(854) << endl;

    }

    // ===========
    // Mutex (lock)
    imgResultColorSearch_mutex.lock();
    // ============

    imgResultColorSearch = imgResultColorSearch_temp.clone();


    // ===========
    // Mutex (unlock)
    imgResultColorSearch_mutex.unlock();
    // ============

}


void ADS::cl_DenseStereo::DrawSearchColor()
{


    // ===========
    // Mutex (lock)
    imgResultColorSearch_mutex.lock();
    // ===========

    cv::Mat imgResultColorSearch_temp = imgResultColorSearch.clone();


    // ===========
    // Mutex (unlock)
    imgResultColorSearch_mutex.unlock();
    // ===========



    cv::Mat outResult01r_temp;

    if (!imgResultColorSearch_temp.empty())
    {


        cv::resize(imgResultColorSearch_temp, outResult01r_temp, cv::Size( int(imgResultColorSearch_temp.cols * CoefShowWindow),
                                                                          int(imgResultColorSearch_temp.rows * CoefShowWindow)), 0, 0, cv::INTER_LINEAR);


        cv::imshow( "Window_imgResult01Left", outResult01r_temp );

    }

    // ################################
    cv::waitKey(10);
    // ################################


}


void ADS::cl_DenseStereo::WriteSourseStereoImg_as_FilesImgSequence()
{
    //=========================================
    // Получение изображений с камеры
    //=========================================


    // Оригиналы изображений
    cv::Mat image01, image02;
    image01 = getImgOpenCV_1left();
    image02 = getImgOpenCV_2right();



    //=========================================
    // Создание папки для калибровочных изображений
    //=========================================

    stringstream path_LeftCam, path_RightCam, path_LeftCam2, path_RightCam2;

    path_LeftCam <<  "./files/SaveImg";
    path_RightCam << "./files/SaveImg";

    path_LeftCam2 <<  "./files/SaveImg/01Left";
    path_RightCam2 << "./files/SaveImg/02Right";






#if defined(_WIN64)

    // ================
    // Only Windows
    // ================

    int result01b = _mkdir(path_LeftCam2.str().c_str() );
    if (result01b==0 )
        cout << "path_LeftCam2: Folder WriteOneStereoPair is made \n";
    else
        cout << "path_LeftCam2: Don't made Folder WriteOneStereoPair \n";

    int result02b = _mkdir(path_RightCam2.str().c_str() );
    if (result02b==0 )
        cout << "path_RightCam2: Folder WriteOneStereoPair is made \n";
    else
        cout << "path_RightCam2: Don't made Folder WriteOneStereoPair \n";

    int result01a = _mkdir(path_LeftCam.str().c_str() );
    if (result01a==0 )
        cout << "path_LeftCam: Folder WriteOneStereoPair is made \n";
    else
        cout << "path_LeftCam: Don't made Folder WriteOneStereoPair \n";

    int result02a = _mkdir(path_RightCam.str().c_str() );
    if (result02a==0 )
        cout << "path_RightCam: Folder WriteOneStereoPair is made \n";
    else
        cout << "path_RightCam: Don't made Folder WriteOneStereoPair \n";



// ================
// Only Windows END
// ================

#else

    // ================
    // Only Linux
    // ================


    if (mkdir(path_LeftCam2.str().c_str(), 0777) == -1  )
        cout << "path_LeftCam2: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "path_LeftCam2: Don't made Folder WriteOneStereoPair \n";


    if (mkdir(path_RightCam2.str().c_str(), 0777) == -1  )
        cout << "path_RightCam2: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "path_RightCam2: Don't made Folder WriteOneStereoPair \n";


    if (mkdir(path_LeftCam.str().c_str(), 0777) == -1  )
        cout << "path_LeftCam: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "path_LeftCam: Don't made Folder WriteOneStereoPair \n";


    if (mkdir(path_RightCam.str().c_str(), 0777) == -1  )
        cout << "path_RightCam: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "path_RightCam: Don't made Folder WriteOneStereoPair \n";


// ================
// Only Linux END
// ================

#endif











    //=========================================
    // Сохранение изображения с результатом
    //=========================================

    stringstream file_path01, file_path02;
    file_path01  << "./files/SaveImg/01Left/L" << count_SequenceImgGrabs << ".bmp";
    file_path02  << "./files/SaveImg/02Right/R" << count_SequenceImgGrabs << ".bmp";\


    cv::imwrite(file_path01.str(), image01);
    cv::imwrite(file_path02.str(), image02);



    // cout << file_path01.str() << endl;
    //cout << file_path02.str()  << endl;

    // счетчик для нумерации файлов
    count_SequenceImgGrabs++;

}


void ADS::cl_DenseStereo::SaveImgSequence()
{

    // Флаг для выхода из цикла
    bool exitCode = true;



    // Оригиналы изображений
    cv::Mat image01, image02;







#if defined(_WIN64)

    // ################################
    Sleep(2000);
    // ################################

#else

    // ################################
    usleep(2000*1000);
    // ################################

#endif

    do {



        // Координаты точки клика мыши
        int mouse_u = 0;
        int mouse_v = 0;



        //=========================================
        // Получение изображений с камеры
        //=========================================



        image01 = getImgOpenCV_1left();
        image02 = getImgOpenCV_2right();


        //=========================================
        // Создание стереопары из 2-х изображений
        //=========================================

        // Get dimension of final image
        int maxV = max(image01.rows, image02.rows);
        int maxU = image01.cols + image02.cols;


        // Create a black image
        cv::Mat3b outStereoImg(maxV, maxU, cv::Vec3b(0,0,0));


        // Copy images in correct position
        image01.copyTo(outStereoImg(cv::Rect(0, 0, image01.cols, image01.rows)));
        image02.copyTo(outStereoImg(cv::Rect(image01.cols, 0, image02.cols, image02.rows)));


        // ======================================
        //  Кнопка START Calibration
        // ======================================

        cv::Point buttonSize = {700,150};

        cv::rectangle (outStereoImg,
                      {5,5},
                      buttonSize,
                      cv::Scalar(47,20,162),
                      8,  // thickness,
                      cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA



        cv::putText	(outStereoImg,
                    "START Acquisition",
                    { 80, 100},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.2,              //double 	fontScale
                    cv::Scalar(47,20,162),
                    6,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin

        // ======================================


        //=========================================
        // Вывод изображения стереопары на экран
        //=========================================

        // Коэффициент масштабирования экрана
        double CoefShowWindow_temp = CoefShowWindow / 2;


        cv::resize(outStereoImg, outStereoImg, cv::Size( double(outStereoImg.cols * CoefShowWindow_temp),
                                                        double(outStereoImg.rows) * CoefShowWindow_temp), 0, 0, cv::INTER_LINEAR);

        cv::imshow( "outStereoIm", outStereoImg );


        // ################################
        cv::waitKey(10);
        // ################################

        //=========================================
        // Обработчик событий для мыши
        //=========================================

        cv::setMouseCallback( "outStereoIm", onMouse, ptr_MouseClickCoordinates);

        MouseClickCoordinates.at(0) = ptr_MouseClickCoordinates->at(0) / CoefShowWindow_temp;
        MouseClickCoordinates.at(1) = ptr_MouseClickCoordinates->at(1) / CoefShowWindow_temp;


        if ( MouseCheck.at(0) != MouseClickCoordinates.at(0) || MouseCheck.at(1)  != MouseClickCoordinates.at(1) )
        {
            mouse_u = MouseClickCoordinates.at(0);
            mouse_v = MouseClickCoordinates.at(1);
            MouseCheck.at(0) = MouseClickCoordinates.at(0) ;
            MouseCheck.at(1) = MouseClickCoordinates.at(1);

        }




        if ( (mouse_u < buttonSize.x && mouse_v < buttonSize.y) && !(mouse_u == 0  && mouse_v == 0) )
        {

            WriteSourseStereoImg_as_FilesImgSequence();
            exitCode = false;
        }






    } while (exitCode);



}





// =============================
// Получение снимков для калибровки
// =============================
void ADS::cl_DenseStereo::getCalibrateImages(const char* setPath_fileList,
                                             const char* setCalibrationImg_folderName, vector<bool>* exitCode_vec )
{


    // exitCode - Флаг для выхода из цикла
    bool exitCode = exitCode_vec->at(0);


    stringstream calibration_path_LeftCam_, calibration_path_RightCam_;

    calibration_path_LeftCam_ <<  "./files/" << setCalibrationImg_folderName << "_01left";
    calibration_path_RightCam_ << "./files/" << setCalibrationImg_folderName << "_02right";

    string calibration_path_LeftCam =   calibration_path_LeftCam_.str();
    string calibration_path_RightCam =   calibration_path_RightCam_.str();





#if defined(_WIN64)

    // ================
    // Only Windows
    // ================



    // Создание папки для калибровочных изображений
    int result01 = _mkdir(calibration_path_LeftCam.c_str());
    if (result01==0 )
        cout << "calibration_path_LeftCam: Folder WriteOneStereoPair is made \n";
    else
        cout << "calibration_path_LeftCam: Don't made Folder WriteOneStereoPair \n";




    int result02 = _mkdir(calibration_path_RightCam.c_str() );
    if (result02==0 )
        cout << "calibration_path_RightCam: Folder WriteOneStereoPair is made \n";
    else
        cout << "calibration_path_RightCam: Don't made Folder WriteOneStereoPair \n";




// ================
// Only Windows END
// ================

#else

    // ================
    // Only Linux
    // ================

    if (mkdir(calibration_path_LeftCam.c_str(), 0777) == -1  )
        cout << "calibration_path_LeftCam: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "calibration_path_LeftCam: Don't made Folder WriteOneStereoPair \n";


    if (mkdir(calibration_path_RightCam.c_str(), 0777) == -1  )
        cout << "calibration_path_RightCam: Folder WriteOneStereoPair is made \n \n";
    else
        cout << "calibration_path_RightCam: Don't made Folder WriteOneStereoPair \n";




// ================
// Only Linux END
// ================

#endif










    // Оригиналы изображений
    cv::Mat image01, image02;


    // Инкримент для нумерации файлов исходных изображений
    int countGetImg = 1001;


    // Открытие файла для записи путей файлов изображений
    std::ofstream out;          // поток для записи
    out.open(setPath_fileList, ios::out); // окрываем файл для записи



#if defined(_WIN64)

    // ################################
    Sleep(2000);
    // ################################

#else

    // ################################
    usleep(2000*1000);
    // ################################

#endif

    do {


        // exitCode - Флаг для выхода из цикла
        exitCode = exitCode_vec->at(0);

        // Координаты точки клика мыши
        int mouse_u = 0;
        int mouse_v = 0;



        //=========================================
        // Получение изображений с камеры
        //=========================================

        // captureContinuous(1);



        // ===========
        // Mutex (lock)
        //img_mutex.lock();
        // ===========
        // Загрузка исходного изображения
        vector<cv::Mat> StereoPair = getImgOpenCV_StereoPair();
        image01 = StereoPair.at(0);
        image02 = StereoPair.at(1);

        //        image01 = getImgOpenCV_1left();
        //        image02 = getImgOpenCV_2right();


        // ===========
        // Mutex (unlock)
        //img_mutex.unlock();
        // ===========


        //=========================================
        // Создание стереопары из 2-х изображений
        //=========================================

        // Get dimension of final image
        int maxV = max(image01.rows, image02.rows);
        int maxU = image01.cols + image02.cols;


        // Create a black image
        cv::Mat3b outStereoImg(maxV, maxU, cv::Vec3b(0,0,0));


        // Copy images in correct position
        image01.copyTo(outStereoImg(cv::Rect(0, 0, image01.cols, image01.rows)));
        image02.copyTo(outStereoImg(cv::Rect(image01.cols, 0, image02.cols, image02.rows)));


        // ======================================
        //  Кнопка START Calibration
        // ======================================

        cv::Point buttonSize = {700,150};

        cv::rectangle (outStereoImg,
                      {5,5},
                      buttonSize,
                      cv::Scalar(47,20,162),
                      8,  // thickness,
                      cv::LINE_8);    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA



        cv::putText	(outStereoImg,
                    "START Calibration",
                    { 80, 100},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.2,              //double 	fontScale
                    cv::Scalar(47,20,162),
                    6,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin

        // ======================================


        //=========================================
        // Вывод изображения стереопары на экран
        //=========================================

        // Коэффициент масштабирования экрана
        double CoefShowWindow_temp = CoefShowWindow / 2;


        cv::resize(outStereoImg, outStereoImg, cv::Size( int(outStereoImg.cols * CoefShowWindow_temp),
                                                        int(outStereoImg.rows * CoefShowWindow_temp)), 0, 0, cv::INTER_LINEAR);

        cv::imshow( "outStereoIm", outStereoImg );


        // ################################
        cv::waitKey(10);
        // ################################

        //=========================================
        // Обработчик событий для мыши
        //=========================================

        cv::setMouseCallback( "outStereoIm", onMouse, ptr_MouseClickCoordinates);

        MouseClickCoordinates.at(0) = ptr_MouseClickCoordinates->at(0) / CoefShowWindow_temp;
        MouseClickCoordinates.at(1) = ptr_MouseClickCoordinates->at(1) / CoefShowWindow_temp;


        if ( MouseCheck.at(0) != MouseClickCoordinates.at(0) || MouseCheck.at(1)  != MouseClickCoordinates.at(1) )
        {
            mouse_u = MouseClickCoordinates.at(0);
            mouse_v = MouseClickCoordinates.at(1);
            MouseCheck.at(0) = MouseClickCoordinates.at(0) ;
            MouseCheck.at(1) = MouseClickCoordinates.at(1);

        }


        if ( (mouse_u < buttonSize.x && mouse_v < buttonSize.y) && !(mouse_u == 0  && mouse_v == 0) ) exitCode = false;


        //=========================================
        // Сохранение изображения с результатом
        //=========================================


        if ( mouse_u > buttonSize.x && mouse_v > buttonSize.y  )
        {

            stringstream ss01, ss02;
            ss01 << calibration_path_LeftCam << "/L" << countGetImg << ".bmp";
            ss02 << calibration_path_RightCam << "/R" << countGetImg << ".bmp";


            cout << ss01.str() << endl;
            cout << ss02.str() << endl;
            cout << " |||||||||||| out.is_open() " << out.is_open() << endl;


            // out.open(setPath_fileList, ios::app); // окрываем файл для записи


            if (out.is_open())
            {
                out <<  ss01.str() << endl;
                out <<  ss02.str() << endl;
            }


            cv::imwrite(ss01.str(), image01);
            cv::imwrite(ss02.str(), image02);

            countGetImg ++;

        }




    } while (exitCode);

    out.close();
    cv::destroyWindow("outStereoIm");

}







// =============================
// Калибровка стереокамеры
// =============================
void ADS::cl_DenseStereo::CalibrateStereoCam(
    const char* setCalibration_fileName,
    const char* usedPath_fileList,
    int         nx, // board size
    int         ny, // board size
    float       squareSize)   // square size (mm))
{





    cout << "\n\n"   << endl;
    cout << "==============================================="   << endl;
    cout << "     Wait Calibration Process Begining .....   "   << endl;
    cout << "==============================================="   << endl;
    cout << "\n\n"   << endl;







    bool              useUncalibrated = false;
    bool              displayCorners   = true;
    bool              showUndistorted  = true;
    bool              isVerticalStereo = false; // horiz or vert cams

    FILE*             f                = fopen( usedPath_fileList, "rt" );
    // окрываем файл для записи


    int               i, j, lr, nframes, num_Im;
    int               N                = nx*ny;
    vector<string>                 imageNames[2];

    vector< cv::Point3f >          boardModel;
    vector< vector<cv::Point3f> >  objectPoints;
    vector< vector<cv::Point2f> >  points[2];
    vector< cv::Point2f >          corners[2];
    bool                           found[2]         = {false, false};
    cv::Size                       imageSize;





    // READ IN THE LIST OF CIRCLE GRIDS:
    //
    if( !f )
    {
        cout << "Cannot open file " << usedPath_fileList << endl;
        return;
    }

    for( i = 0; i < ny; i++ )
        for( j = 0; j < nx; j++ )
            boardModel.push_back(
                cv::Point3f((float)(i*squareSize), (float)(j*squareSize), 0.f)
                );
    i = 0;
    for(;;)
    {
        char buf[1024];
        lr = i % 2;
        if( lr == 0 ) found[0] = found[1] = false;

        if( !fgets( buf, sizeof(buf)-3, f )) break;
        size_t len = strlen(buf);
        while( len > 0 && isspace(buf[len-1]))  buf[--len] = '\0';
        if( buf[0] == '#') continue;

        cv::Mat img = cv::imread( buf, 0 );
        if( img.empty() ) break;
        imageSize = img.size();
        imageNames[lr].push_back(buf);
        i++;
        num_Im = i;


        // If we did not find board on the left image,
        // it does not make sense to find it on the right.
        //
        if( lr == 1 && !found[0] )
            continue;
        // Find  grids :



//         // !!!! только в особых случаях
//        // ======================================
//        // Сжатие изображения если нужно сделать калибровку под новое разрешение по старым изображеням
//        // ============================

//        img_X = 960;
//        img_Y = 768;
//        img_coeff = 1.0;
//        if (img.cols != img_X && img.rows != img_Y)
//            cv::resize(img, img,
//                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);
//           imageSize = img.size();
//        // ======================================




        // Для обычной калибровки
        found[lr] = cv::findChessboardCorners(
            img,
            cv::Size(nx, ny),
            corners[lr],
            cv::CALIB_CB_ADAPTIVE_THRESH +       // Используйте адаптивное пороговое значение для преобразования изображения в черно-белое, а не фиксированный пороговый уровень (вычисляемый из средней яркости изображения).
            //    cv::CALIB_CB_NORMALIZE_IMAGE +   // Нормализуйте гамму изображения с помощью "equalizeHist" перед применением фиксированного или адаптивного порогового значения
                cv::CALIB_CB_FILTER_QUADS        // Используйте дополнительные критерии (например, площадь контура, периметр, квадратная форма), чтобы отфильтровать ложные квадратуры, извлеченные на этапе извлечения контура
            // cv::CALIB_CB_FAST_CHECK           // Запустите быструю проверку изображения, которое ищет углы шахматной доски, и сократите вызов, если ничего не найдено
            );


//        cv::CALIB_CB_NORMALIZE_IMAGE - влияет на количесво определения (убрали - улучшилось)
//        Наверное нужна только для работы с темным фоном вокруг КД

        // ============================

        if(found[lr])
            cv::cornerSubPix(img, corners[lr], cv::Size(11, 11), cv::Size(-1, -1),
                             cv::TermCriteria(cv::TermCriteria:: MAX_ITER + cv::TermCriteria::EPS, 100, 0.001));
        // ============================



        if( displayCorners )
        {
            cout << buf << endl;
            cv::Mat cimg;
            cv::cvtColor( img, cimg, cv::COLOR_GRAY2BGR );
            // draw chessboard corners works for circle grids too

            cv::drawChessboardCorners(
                cimg, cv::Size(nx, ny), corners[lr], found[lr]
                );

            double Dx = 0.4;
            cv::resize(cimg, cimg, cv::Size(cimg.cols * Dx,cimg.rows * Dx), 0, 0, cv::INTER_LINEAR);

            cv::imshow( "Corners", cimg );
            cv::waitKey(100);

            //    if( (cv::waitKey(0)&255) == 27 ) // Allow ESC to quit
            //       exit(-1);
        }
        else
            cout << '.';


        if( lr == 1 && found[0] && found[1] )
        {
            objectPoints.push_back(boardModel);
            points[0].push_back(corners[0]);
            points[1].push_back(corners[1]);
        }
    }
    fclose(f);




    // ==================
    // Оценка внутренних параметров каждой камеры отдельно
    // ==================

    // CALIBRATE THE STEREO CAMERAS
    cv::Mat M1 = cv::Mat::eye( 3, 3, CV_64F );
    cv::Mat M2 = cv::Mat::eye( 3, 3, CV_64F );
    cv::Mat D1, D2, R, T, E, F, Q;
    cout <<"\nRunning stereo calibration ...\n";

    vector<cv::Mat> rvecs1, tvecs1, rvecs2, tvecs2;
    int flag = 0;
//    flag |= cv::CALIB_FIX_K4;
//    flag |= cv::CALIB_FIX_K5;

//    flag |= cv::CALIB_FIX_ASPECT_RATIO;
//    flag |= cv::CALIB_TILTED_MODEL;


    // ==================
    // Оценка внутренних параметров 01 камеры отдельно
    // ==================
//    double rms01 =  calibrateCamera(
//        objectPoints,
//        points[0],
//        imageSize,
//        M1,     // cameraMatrix
//        D1,     // distCoeffs
//        rvecs1,
//        tvecs1,
//        flag);



//    // ==================
//    // Оценка внутренних параметров 02камеры отдельно
//    // ==================
//    double rms02 =  calibrateCamera(
//        objectPoints,
//        points[1],
//        imageSize,
//        M2,     // cameraMatrix
//        D2,     // distCoeffs
//        rvecs2,
//        tvecs2,
//        flag);



//    objectPoints — Вектор векторов 3D точек.
//    Внешний вектор содержит столько же элементов, сколько число представлений образца.

//    imagePoints — Вектор векторов 2D точек.
//    imageSize — Размер изображения
//    cameraMatrix — Внутренняя матрица камеры
//    distCoeffs — Коэффициенты искажения объектива. Эти коэффициенты будут объяснены в будущем посте.
//    rvecs — Вращение указано как вектор 3 × 13×1. Направление вектора задает ось вращения, а величина вектора — угол поворота.
//    tvecs — 3×13×1 вектор смещения.


 // Оптимальная матрица
//    M1 = cv::getOptimalNewCameraMatrix(M1, // cameraMatrix
//                                       D1, // distCoeffs
//                                       imageSize,
//                                       1,
//                                       imageSize,
//                                       0);

//    // Оптимальная матрица
//    M2 = cv::getOptimalNewCameraMatrix(M2, // cameraMatrix
//                                       D2, // distCoeffs
//                                       imageSize,
//                                       1,
//                                       imageSize,
//                                       0);








    // ==================
    // Калибровка стереокамеры
    // ==================


//    double rmsStereo =  cv::stereoCalibrate(
//        objectPoints,
//        points[0],
//        points[1],
//        M1, D1, M2, D2,
//        imageSize, R, T, E, F,
//        cv::CALIB_FIX_ASPECT_RATIO +
//       // cv::CALIB_FIX_INTRINSIC +
//            //   cv::CALIB_USE_INTRINSIC_GUESS +
//            //   cv::CALIB_USE_EXTRINSIC_GUESS +
//            //   cv::CALIB_ZERO_TANGENT_DIST +
//            //   cv::CALIB_SAME_FOCAL_LENGTH +
//            //   cv::CALIB_RATIONAL_MODEL +
//            //   cv::CALIB_FIX_K3 +
//            //   cv::CALIB_FIX_K4 +
//            //   cv::CALIB_FIX_K5 +
//            //   cv::CALIB_THIN_PRISM_MODEL+
//            cv::CALIB_TILTED_MODEL,
//        cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 100, 1e-4)
//        );





    // CALIBRATE THE STEREO CAMERAS // https://russianblogs.com/article/2529263524/

    int flagStereo = 0;
//    flagStereo |= cv::CALIB_USE_INTRINSIC_GUESS;     //  матрица внутреннего параметра cameraMatrix вводится как начальное значение, и функция дополнительно оптимизирует его.
//      flagStereo |= cv::CALIB_FIX_ASPECT_RATIO;       // Исправьте соотношение fx / fy и используйте только fy в качестве переменной для выполнения расчетов оптимизации
//    flagStereo |= cv::CALIB_FIX_PRINCIPAL_POINT;   // Точка оптической оси будет зафиксирована во время оптимизации
//    flagStereo |= cv::CALIB_FIX_FOCAL_LENGTH;
//    flagStereo |= cv::CALIB_FIX_K1;               // коэффициент радиального искажения остается неизменным во время оптимизации
//    flagStereo |= cv::CALIB_FIX_K2;
//    flagStereo |= cv::CALIB_FIX_K3;
//    flagStereo |= cv::CALIB_FIX_K4;
//    flagStereo |= cv::CALIB_FIX_K5;
//    flagStereo |= cv::CALIB_RATIONAL_MODEL;      // (Идеальная модель) Включите три параметра искажения k4, k5 и k6. Заставьте функцию калибровки использовать рациональную модель и получите 8 коэффициентов
//    flagStereo |= cv::CALIB_THIN_PRISM_MODEL;    // (Модель искажения тонкой призмы): включить коэффициенты искажения S1, S2, S3 и S4
//    flagStereo |= cv::CALIB_FIX_S1_S2_S3_S4;     // Коэффициенты искажения S1, S2, S3 и S4 тонкой призмы не изменяются в процессе оптимизации
      flagStereo |= cv::CALIB_TILTED_MODEL;          // (Модель наклона): включить коэффициенты искажения tauX и tauY. Функция калибровки использует модель датчика наклона и возвращает 14 коэффициентов.
//    flagStereo |= cv::CALIB_FIX_TAUX_TAUY;       // В процессе оптимизации коэффициент модели датчика наклона не изменяется.
//    flagStereo |= cv::CALIB_USE_QR;
//    flagStereo |= cv::CALIB_FIX_TANGENT_DIST;
//    flagStereo |= cv::CALIB_FIX_INTRINSIC;
//    flagStereo |= cv::CALIB_SAME_FOCAL_LENGTH;
//    flagStereo |= cv::CALIB_ZERO_DISPARITY;
//    flagStereo |= cv::CALIB_USE_LU;
//    flagStereo |= cv::CALIB_USE_EXTRINSIC_GUESS;


    double rmsStereo =  cv::stereoCalibrate(
        objectPoints,
        points[0],
        points[1],
        M1, D1, M2, D2,
        imageSize, R, T, E, F,
        flagStereo,
//        cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-4)
        cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 500, 1e-4)
          );





    // CALIBRATION QUALITY CHECK
    // because the output fundamental matrix implicitly
    // includes all the output information,
    // we can check the quality of calibration using the
    // epipolar geometry constraint: m2^t*F*m1=0
    vector< cv::Point3f > lines[2];
    double avgErr = 0;

    nframes = (int)objectPoints.size();

    double avgErr_vec[100];


    for( i = 0; i < nframes; i++ )
    {
        vector< cv::Point2f >& pt0 = points[0][i];
        vector< cv::Point2f >& pt1 = points[1][i];
        cv::undistortPoints( pt0, pt0, M1, D1, cv::Mat(), M1 );
        cv::undistortPoints( pt1, pt1, M2, D2, cv::Mat(), M2 );
        cv::computeCorrespondEpilines( pt0, 1, F, lines[0] );
        cv::computeCorrespondEpilines( pt1, 2, F, lines[1] );
        for( j = 0; j < N; j++ )
        {
            double err = fabs(
                             pt0[j].x*lines[1][j].x + pt0[j].y*lines[1][j].y + lines[1][j].z
                             ) + fabs(
                             pt1[j].x*lines[0][j].x + pt1[j].y*lines[0][j].y + lines[0][j].z
                             );
            avgErr += err;
            avgErr_vec[i] = err;
        }
    }

    avgErr = avgErr/(nframes*N);
    cout << "avg err = " <<  avgErr << endl;


    cv::Mat R1, R2, P1, P2, map11, map12, map21, map22;
    cv::Mat H1, H2;
    cv::Rect validRoi[2];
    cv::Rect roi1, roi2;


    // COMPUTE AND DISPLAY RECTIFICATION
    //
    if( showUndistorted )
    {

        // IF BY CALIBRATED (BOUGUET'S METHOD)
        //

//        int flag_StereoRectify = 0;
       int flag_StereoRectify = cv::CALIB_ZERO_DISPARITY;


        if( !useUncalibrated )
        {
            cv::stereoRectify(
                M1, D1, M2, D2, imageSize, R, T,
                R1, R2, P1, P2, Q,
                flag_StereoRectify,
                -1,
                imageSize,
                &roi1, &roi2);

            // Precompute maps for cvRemap()
            cv::initUndistortRectifyMap(
                M1, D1, R1, P1, imageSize, CV_16SC2, map11, map12);
            cv::initUndistortRectifyMap(
                M2, D2, R2, P2, imageSize, CV_16SC2, map21, map22);

        }


        // OR ELSE HARTLEY'S METHOD
        //
        else {
            // use intrinsic parameters of each camera, but
            // compute the rectification transformation directly
            // from the fundamental matrix
            vector< cv::Point2f > allpoints[2];
            for( i = 0; i < nframes; i++ )
            {
                copy(
                    points[0][i].begin(),
                    points[0][i].end(),
                    back_inserter(allpoints[0])
                    );
                copy(
                    points[1][i].begin(),
                    points[1][i].end(),
                    back_inserter(allpoints[1])
                    );
            }

            cv::Mat F = findFundamentalMat(
                allpoints[0], allpoints[1], cv::FM_8POINT
                );

            cv::stereoRectifyUncalibrated(
                allpoints[0], allpoints[1],
                F,
                imageSize,
                H1, H2,
                3
                );
            R1 = M1.inv()*H1*M1;
            R2 = M2.inv()*H2*M2;
            // Precompute map for cvRemap()
            //
            cv::initUndistortRectifyMap(
                M1, D1, R1, P1,
                imageSize,
                CV_16SC2,
                map11, map12
                );
            cv::initUndistortRectifyMap(
                M2, D2, R2, P2,
                imageSize,
                CV_16SC2,
                map21, map22
                );
        }

    }



    cout << "\n nframes \n" << nframes
         << "\n num_Im \n" << num_Im
         << "\n rmsStereo \n" << rmsStereo
         << "\n avgErr \n" << avgErr
         << "\n cameraMatrix1_M1 \n"  << M1
         << "\n distCoeffs1_D1 \n" << D1
         << "\n cameraMatrix2_M2 \n" << M2
         << "\n distCoeffs2_D2 \n" << D2
         << "\n imageSize \n " << imageSize
         << "\n RotationMatrix_R \n" << R
         << "\n TranslationVector_T \n" << T
         << "\n EssentialMatrix_E \n " << E
         << "\n FundamentalMatrix_F \n" << F
         << "\n disparity-to-depth_mapping_matrix_Q \n" << Q
         << "\n" << endl;

    cout <<"Done\n\n";





    // SAVE

    cout<<"Start SAVE ..."<<endl;

    // File 01
    cv::FileStorage fs(setCalibration_fileName, cv::FileStorage::WRITE);
    if( fs.isOpened() )
    {
        fs << "M1" << M1     // cameraMatrix1
           << "D1" << D1     // distCoeffs1
           << "M2" << M2    // cameraMatrix2
           << "D2" << D2    // distCoeffs2
           << "imageSize" << imageSize

           << "R" << R  // Rotation matrix from the coordinate system of the first camera to the second.
           << "T" << T  // Translation vector from the coordinate system of the first camera to the second
           << "E" << E      // EssentialMatrix
           << "F" << F      // FundamentalMatrix
           << "R1" << R1 // Output 3x3 rectification transform (rotation matrix) for the first camera
           << "R2" << R2 // Output 3x3 rectification transform (rotation matrix) for the second camera
           << "P1" << P1 // Output 3x4 projection matrix in the new (rectified) coordinate systems for the first camera
           << "P2" << P2 // Output 3x4 projection matrix in the new (rectified) coordinate systems for the second camera
           << "H1" << H1 // Output rectification homography matrix for the first image
           << "H2" << H2 // Output rectification homography matrix for the second image
           << "Q" << Q  // Output 4×4 disparity-to-depth mapping matrix (see reprojectImageTo3D

           << "rmsStereo" << rmsStereo
           << "avgErr" << avgErr
           << "nframes" << nframes
           << "num_Im" << num_Im;
        fs.release();
    }
    else cout << "Error: can not save the intrinsic parameters\n";


    // File 02
    ofstream out;          // поток для записи


    out.open("./files/avgError.txt", ios::out); // окрываем файл для записи
    if (out.is_open())
    {

        for( i = 0; i < num_Im/2; i++ )
        {
            out << i << ": " <<  avgErr_vec[i] << "\n";
        }
        out  << endl;
    }
    out.close();



    cout << "\n\n"   << endl;
    cout << "==============================================="   << endl;
    cout << "    Calibration Process Finished    "   << endl;
    cout << "==============================================="   << endl;

}







void ADS::cl_DenseStereo::DetectPrimitive_old()
{

    cv::Mat imgLeft_temp, imgRight_temp;

    vector<cv::Mat> StereoPair = getImgGrayPreProc_StereoPair();
    imgLeft_temp = StereoPair.at(0);
    imgRight_temp = StereoPair.at(1);


    if ( (imgLeft_temp.empty() || imgRight_temp.empty() )  ) return;




    vector<vector<double>> figureModel;
    Mat finalImage = Mat::zeros(1200, 1920, CV_8UC3);
    vector<vector<Point3d>> clastersData;

    vector<int> IDX = vuxyzrgb.cluster;
    vector<Point3d> xyz;
    xyz.resize(int( vuxyzrgb.xyz.size() ) );


    // == Правка====

    for (int i = 0; i < int( vuxyzrgb.xyz.size() ); i++)
    {
        xyz.at(i).x = vuxyzrgb.xyz.at(i).at(0);
        xyz.at(i).y = vuxyzrgb.xyz.at(i).at(1);
        xyz.at(i).z = vuxyzrgb.xyz.at(i).at(2);
    }





    int numClusters = 0;
    for (int i = 0; i < int(IDX.size()); i++)
        if (IDX[i] > numClusters)
            numClusters = IDX[i];

    for (int j = 0; j < numClusters; j++) {
        vector<Point3d> xyz2;
        for (int i = 0; i < int(IDX.size()); i++)
            if (IDX[i] == (j + 1)) {
                xyz2.push_back(xyz[i]);
            }
        clastersData.push_back(xyz2);
    }

    // =====================================


    ofstream IDXout;
    IDXout.open("IDX.m");
    IDXout << "b = [";
    for (int i = 0; i < int(IDX.size()) ; i++)
        IDXout << IDX[i] << endl;
    IDXout << "];";
    IDXout.close();



    // =====================================
    //
    // =====================================

    Mat projectionXY = Mat::ones(1200, 1920, CV_8UC3);
    projectionXY = Scalar(255, 255, 255);
    Mat projectionYZ = Mat::ones(1200, 1920, CV_8UC3);
    projectionYZ = Scalar(255, 255, 255);
    Mat projectionXZ = Mat::ones(1200, 1920, CV_8UC3);
    projectionXZ = Scalar(255, 255, 255);


    // =====================================
    // Проверка на выбросы
    // =====================================
    double xLimMin = 1e+8;
    double xLimMax = -1e+8;
    double yLimMin = 1e+8;
    double yLimMax = -1e+8;
    double zLimMin = 1e+8;
    double zLimMax = -1e+8;
    for (int i = 0; i < int(xyz.size()); i++) {
        if (IDX[i] != 0) {
            if (xyz[i].x < xLimMin)
                xLimMin = xyz[i].x;
            if (xyz[i].x > xLimMax)
                xLimMax = xyz[i].x;
            if (xyz[i].y < yLimMin)
                yLimMin = xyz[i].y;
            if (xyz[i].y > yLimMax)
                yLimMax = xyz[i].y;
            if (xyz[i].z < zLimMin)
                zLimMin = xyz[i].z;
            if (xyz[i].z > zLimMax)
                zLimMax = xyz[i].z;
        }
    }

    // =====================================
    // Создание изображения с результатом (прорисовка сетки и др.)
    // =====================================

    double xyScale, yzScale, xzScale, xScale, yScale, zScale;
    xScale = min(abs(910 / xLimMax), abs(910 / xLimMin));
    yScale = min(abs(550 / yLimMax), abs(550 / yLimMin));
    xyScale = min(xScale, yScale);
    zScale = 1820 / zLimMax;
    yzScale = min(zScale, yScale);
    xScale = min(abs(550 / xLimMax), abs(550 / xLimMin));
    xzScale = min(xScale, zScale);
    Rect rect(49, 49, 1820, 1100);
    rectangle(projectionXY, rect, cv::Scalar(0, 0, 0));
    rectangle(projectionYZ, rect, cv::Scalar(0, 0, 0));
    rectangle(projectionXZ, rect, cv::Scalar(0, 0, 0));
    Point2d p1, p2;
    p1.x = 49; p1.y = 599.5;
    p2.x = 1869; p2.y = 599.5;
    line(projectionXY, p1, p2, Scalar(200, 200, 200));
    p1.x = 959.5; p1.y = 49;
    p2.x = 959.5; p2.y = 1149;
    line(projectionXY, p1, p2, Scalar(200, 200, 200));
    p1.x = 49; p1.y = 599.5;
    p2.x = 1869; p2.y = 599.5;
    line(projectionYZ, p1, p2, Scalar(200, 200, 200));
    line(projectionXZ, p1, p2, Scalar(200, 200, 200));
    char textZ[10];
    Point2d textPoint;
    textPoint.x = 49;
    textPoint.y = 40;
    putText(projectionXY, "Y", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionYZ, "Y", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionXZ, "X", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    textPoint.x = 1878;
    textPoint.y = 1149;
    putText(projectionXY, "X", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionYZ, "Z", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionXZ, "Z", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    textPoint.x = 20;
    textPoint.y = 610;
    putText(projectionXY, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionYZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionXZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    textPoint.x = 40;
    textPoint.y = 1180;
    putText(projectionYZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionXZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    textPoint.x = 950;
    textPoint.y = 1180;
    putText(projectionXY, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));

    // =====================================


    Mat res4 = imgLeft_temp.clone();

    // == Правка====
    vector<Point2d> kp1;
    kp1.resize(int( vuxyzrgb.xyz.size() ));

    for (int i = 0; i < int( vuxyzrgb.xyz.size() ); i++)
    {
        kp1.at(i).x = double(vuxyzrgb.vu.at(i).at(1) );
        kp1.at(i).y = double(vuxyzrgb.vu.at(i).at(0) );
    }

    // =====================================

    // =====================================
    // Прорисовка точек
    // =====================================

    for (int i = 0; i < int(clastersData.size()); i++) {
        Mat res3 = imgLeft_temp.clone();
       // char clasterFilename[100];
       // sprintf_s(clasterFilename, "Claster%d.jpg", i + 1);
        double xmin = 1000000, xmax = 0, ymin = 1000000, ymax = 0;
        double meanZ = 0.0;
        int countZ = 0;
        int tempIdx = 0;
        for (int j = 0; j < int(IDX.size()); j++)
            if (IDX[j] == (i + 1)) {
                circle(res3, kp1[j], 3, Scalar(0, 255, 0));
                circle(res4, kp1[j], 3, Scalar(0, 255, 0));
                if (kp1[j].x < xmin)
                    xmin = kp1[j].x;
                if (kp1[j].x > xmax)
                    xmax = kp1[j].x;
                if (kp1[j].y < ymin)
                    ymin = kp1[j].y;
                if (kp1[j].y > ymax)
                    ymax = kp1[j].y;
                meanZ += clastersData[i][tempIdx].z;
                countZ++;
                tempIdx++;


                Point2d pcPoint;
                pcPoint.x = 959.5 + xyz[j].x * xyScale;
                pcPoint.y = 599.5 + xyz[j].y * xyScale;
                circle(projectionXY, pcPoint, 3, Scalar(0, 255, 0));
                pcPoint.x = 49 + xyz[j].z * yzScale;
                pcPoint.y = 599.5 + xyz[j].y * yzScale;
                circle(projectionYZ, pcPoint, 3, Scalar(0, 255, 0));
                pcPoint.x = 49 + xyz[j].z * xzScale;
                pcPoint.y = 599.5 + xyz[j].x * xzScale;
                circle(projectionXZ, pcPoint, 3, Scalar(0, 255, 0));
            }
        meanZ /= double(countZ);
        Point2d lt, rt, lb, rb;
        lt.x = xmin; lt.y = ymin;
        rt.x = xmax; rt.y = ymin;
        lb.x = xmin; lb.y = ymax;
        rb.x = xmax; rb.y = ymax;
        line(res3, lt, rt, Scalar(0, 255, 255), 2);
        line(res3, lt, lb, Scalar(0, 255, 255), 2);
        line(res3, lb, rb, Scalar(0, 255, 255), 2);
        line(res3, rt, rb, Scalar(0, 255, 255), 2);
        line(res4, lt, rt, Scalar(0, 255, 255), 2);
        line(res4, lt, lb, Scalar(0, 255, 255), 2);
        line(res4, lb, rb, Scalar(0, 255, 255), 2);
        line(res4, rt, rb, Scalar(0, 255, 255), 2);
        //char textZ[100];
        Point2d textPoint;
        textPoint.x = (xmax + xmin) / 2;
        textPoint.y = ymax + 15;
        //sprintf_s(textZ, "Mean Z, mm: %7.2f", meanZ);
        putText(res3, textZ, textPoint, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
        //putText(res4, textZ, textPoint, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
        //cv::imwrite(clasterFilename, res3);
    }
    //cv::imwrite("AllClasters.jpg", res4);







    ofstream ClasterDataOut;
    ClasterDataOut.open("ClasterDataOut.m");
    ClasterDataOut << "clasters = [ ";

    for (int k = 0; k < int(clastersData.size()); k++) {
        //*****************************************   Fit plane   ************************
        vector<Point3d> planePoints;
        vector<double> PlaneModel = FitPlane(clastersData[k], planePoints);
        std::cout << endl << "Claster" << k + 1 << ":" << endl << endl;

        std::cout << endl << "Plane:" << endl << "[ ";

        for (int i = 0; i < int(PlaneModel.size()); i++) {
            std::cout << PlaneModel[i] << " ";

            ClasterDataOut << PlaneModel[i] << " ";
        }
        std::cout << "]" << endl << endl;

        ClasterDataOut << "0 0 0 0;" << endl;

        //*****************************************   Fit sphere   ************************
        vector<double> SphereModel = FitSphere(clastersData[k]);

        std::cout << endl << "Sphere:" << endl << "[ ";

        for (int i = 0; i < int(SphereModel.size()); i++) {
            std::cout << SphereModel[i] << " ";

            ClasterDataOut << SphereModel[i] << " ";
        }
        std::cout << "]" << endl << endl;

        ClasterDataOut << "0 0 0;" << endl;

        //*****************************************   Fit cylinder   ************************
        vector<Point3d> cylinderPoints;
        vector<double> CylinderModel = FitCylinder(clastersData[k], PlaneModel, cylinderPoints);

        std::cout << endl << "Cylinder:" << endl << "[ ";

        for (int i = 0; i < int (CylinderModel.size()); i++) {
            std::cout << CylinderModel[i] << " ";

            ClasterDataOut << CylinderModel[i] << " ";
        }
        std::cout << "]" << endl << endl;

        ClasterDataOut << ";" << endl;

        int figureInClaster = 0;
        double minErrorOfModel = 0.0;
        if (PlaneModel[PlaneModel.size() - 1] < SphereModel[SphereModel.size() - 1])
            minErrorOfModel = PlaneModel[PlaneModel.size() - 1];

        else {
            minErrorOfModel = SphereModel[SphereModel.size() - 1];
            figureInClaster = 1;
        }
        if (CylinderModel[CylinderModel.size() - 1] < minErrorOfModel) {
            minErrorOfModel = CylinderModel[CylinderModel.size() - 1];
            figureInClaster = 2;
        }
        std::cout << endl << "Figure in claster " << k + 1 << " is: ";

        Point2d pcPoint;
        switch (figureInClaster)
        {
        case 0:
            std::cout << "Plane" << endl;

            figureModel.push_back(PlaneModel);

            for (int i = 0; i < 6; i++) {
                p1.x = 959.5 + planePoints[0].x * xyScale + 0.2 * i * (planePoints[1].x * xyScale - planePoints[0].x * xyScale);
                p1.y = 599.5 + planePoints[0].y * xyScale + 0.2 * i * (planePoints[1].y * xyScale - planePoints[0].y * xyScale);
                p2.x = 959.5 + planePoints[2].x * xyScale + 0.2 * i * (planePoints[3].x * xyScale - planePoints[2].x * xyScale);
                p2.y = 599.5 + planePoints[2].y * xyScale + 0.2 * i * (planePoints[3].y * xyScale - planePoints[2].y * xyScale);
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                p1.x = 959.5 + planePoints[0].x * xyScale + 0.2 * i * (planePoints[2].x * xyScale - planePoints[0].x * xyScale);
                p1.y = 599.5 + planePoints[0].y * xyScale + 0.2 * i * (planePoints[2].y * xyScale - planePoints[0].y * xyScale);
                p2.x = 959.5 + planePoints[1].x * xyScale + 0.2 * i * (planePoints[3].x * xyScale - planePoints[1].x * xyScale);
                p2.y = 599.5 + planePoints[1].y * xyScale + 0.2 * i * (planePoints[3].y * xyScale - planePoints[1].y * xyScale);
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
            }
            for (int i = 0; i < 6; i++) {
                p1.x = 49 + planePoints[0].z * yzScale + 0.2 * i * (planePoints[1].z * yzScale - planePoints[0].z * yzScale);
                p1.y = 599.5 + planePoints[0].y * yzScale + 0.2 * i * (planePoints[1].y * yzScale - planePoints[0].y * yzScale);
                p2.x = 49 + planePoints[2].z * yzScale + 0.2 * i * (planePoints[3].z * yzScale - planePoints[2].z * yzScale);
                p2.y = 599.5 + planePoints[2].y * yzScale + 0.2 * i * (planePoints[3].y * yzScale - planePoints[2].y * yzScale);
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                p1.x = 49 + planePoints[0].z * yzScale + 0.2 * i * (planePoints[2].z * yzScale - planePoints[0].z * yzScale);
                p1.y = 599.5 + planePoints[0].y * yzScale + 0.2 * i * (planePoints[2].y * yzScale - planePoints[0].y * yzScale);
                p2.x = 49 + planePoints[1].z * yzScale + 0.2 * i * (planePoints[3].z * yzScale - planePoints[1].z * yzScale);
                p2.y = 599.5 + planePoints[1].y * yzScale + 0.2 * i * (planePoints[3].y * yzScale - planePoints[1].y * yzScale);
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
            }
            for (int i = 0; i < 6; i++) {
                p1.x = 49 + planePoints[0].z * xzScale + 0.2 * i * (planePoints[1].z * xzScale - planePoints[0].z * xzScale);
                p1.y = 599.5 + planePoints[0].x * xzScale + 0.2 * i * (planePoints[1].x * xzScale - planePoints[0].x * xzScale);
                p2.x = 49 + planePoints[2].z * xzScale + 0.2 * i * (planePoints[3].z * xzScale - planePoints[2].z * xzScale);
                p2.y = 599.5 + planePoints[2].x * xzScale + 0.2 * i * (planePoints[3].x * xzScale - planePoints[2].x * xzScale);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                p1.x = 49 + planePoints[0].z * xzScale + 0.2 * i * (planePoints[2].z * xzScale - planePoints[0].z * xzScale);
                p1.y = 599.5 + planePoints[0].x * xzScale + 0.2 * i * (planePoints[2].x * xzScale - planePoints[0].x * xzScale);
                p2.x = 49 + planePoints[1].z * xzScale + 0.2 * i * (planePoints[3].z * xzScale - planePoints[1].z * xzScale);
                p2.y = 599.5 + planePoints[1].x * xzScale + 0.2 * i * (planePoints[3].x * xzScale - planePoints[1].x * xzScale);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
            }

            break;
        case 1:
            std::cout << "Sphere" << endl;

            figureModel.push_back(SphereModel);
            pcPoint.x = 959.5 + SphereModel[0] * xyScale;
            pcPoint.y = 599.5 + SphereModel[1] * xyScale;
            circle(projectionXY, pcPoint, SphereModel[3] * xyScale, Scalar(50, 50, 50));
            double xLeft, xRight, yTop, yLeft, yRigth;
            yTop = pcPoint.y - SphereModel[3] * xyScale;
            for (int i = 0; i < 4; i++) {
                xLeft = pcPoint.x - sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * i * SphereModel[3] * xyScale, 2));
                xRight = pcPoint.x + sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * i * SphereModel[3] * xyScale, 2));
                yLeft = yRigth = yTop + 0.5 * i * SphereModel[3] * xyScale;
                p1.x = xLeft; p1.y = yLeft;
                p2.x = xRight; p2.y = yRigth;
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                for (int j = 0; j < 3; j++) {
                    p1.x = xLeft + (xRight - xLeft) * 0.25 * (j + 1);
                    p1.y = yLeft;
                    p2.x = pcPoint.x - sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * (i + 1) * SphereModel[3] * xyScale, 2)) + 2 * sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * (i + 1) * SphereModel[3] * xyScale, 2)) * 0.25 * (j + 1);
                    p2.y = yTop + 0.5 * (i + 1) * SphereModel[3] * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                }
            }

            pcPoint.x = 49 + SphereModel[2] * yzScale;
            pcPoint.y = 599.5 + SphereModel[1] * yzScale;
            circle(projectionYZ, pcPoint, SphereModel[3] * yzScale, Scalar(50, 50, 50));
            yTop = pcPoint.y - SphereModel[3] * yzScale;
            for (int i = 0; i < 4; i++) {
                xLeft = pcPoint.x - sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * i * SphereModel[3] * yzScale, 2));
                xRight = pcPoint.x + sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * i * SphereModel[3] * yzScale, 2));
                yLeft = yRigth = yTop + 0.5 * i * SphereModel[3] * yzScale;
                p1.x = xLeft; p1.y = yLeft;
                p2.x = xRight; p2.y = yRigth;
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                for (int j = 0; j < 3; j++) {
                    p1.x = xLeft + (xRight - xLeft) * 0.25 * (j + 1);
                    p1.y = yLeft;
                    p2.x = pcPoint.x - sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * (i + 1) * SphereModel[3] * yzScale, 2)) + 2 * sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * (i + 1) * SphereModel[3] * yzScale, 2)) * 0.25 * (j + 1);
                    p2.y = yTop + 0.5 * (i + 1) * SphereModel[3] * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                }
            }

            pcPoint.x = 49 + SphereModel[2] * xzScale;
            pcPoint.y = 599.5 + SphereModel[0] * xzScale;
            circle(projectionXZ, pcPoint, SphereModel[3] * xzScale, Scalar(50, 50, 50));
            circle(projectionXZ, pcPoint, 0.5 * SphereModel[3] * xzScale, Scalar(50, 50, 50));
            for (int i = 0; i < 4; i++) {
                p1.x = pcPoint.x + SphereModel[3] * xzScale * cos(i * CV_PI / 4); p1.y = pcPoint.y + SphereModel[3] * xzScale * sin(i * CV_PI / 4);
                p2.x = pcPoint.x + SphereModel[3] * xzScale * cos(i * CV_PI / 4 + CV_PI); p2.y = pcPoint.y + SphereModel[3] * xzScale * sin(i * CV_PI / 4 + CV_PI);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
            }
            break;
        case 2:
            std::cout << "Cylinder" << endl;

            figureModel.push_back(CylinderModel);
            int nearCenter, farCenter;
            if (CylinderModel[2] < CylinderModel[5]) {
                nearCenter = 0;
                farCenter = 1;
            }
            else {
                nearCenter = 1;
                farCenter = 0;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 959.5 + cylinderPoints[2 * i + nearCenter].x * xyScale; p1.y = 599.5 + cylinderPoints[2 * i + nearCenter].y * xyScale;
                p2.x = 959.5 + cylinderPoints[2 * (i + 1) + nearCenter].x * xyScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].y * xyScale;
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].z < CylinderModel[2 + 3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].z < CylinderModel[2 + 3 * farCenter])) {
                    p1.x = 959.5 + cylinderPoints[2 * i + farCenter].x * xyScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].y * xyScale;
                    p2.x = 959.5 + cylinderPoints[2 * (i + 1) + farCenter].x * xyScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                    p1.x = 959.5 + cylinderPoints[2 * i + farCenter].x * xyScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].y * xyScale;
                    p2.x = 959.5 + cylinderPoints[2 * i + nearCenter].x * xyScale; p2.y = 599.5 + cylinderPoints[2 * i + nearCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                    p1.x = 959.5 + cylinderPoints[2 * (i + 1) + farCenter].x * xyScale; p1.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].y * xyScale;
                    p2.x = 959.5 + cylinderPoints[2 * (i + 1) + nearCenter].x * xyScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                }
            }
            if (CylinderModel[0] < CylinderModel[3]) {
                nearCenter = 1;
                farCenter = 0;
            }
            else {
                nearCenter = 0;
                farCenter = 1;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 49 + cylinderPoints[2 * i + nearCenter].z * yzScale; p1.y = 599.5 + cylinderPoints[2 * i + nearCenter].y * yzScale;
                p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * yzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].y * yzScale;
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].x > CylinderModel[3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].x > CylinderModel[3 * farCenter])) {
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * yzScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * yzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * yzScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * i + nearCenter].z * yzScale; p2.y = 599.5 + cylinderPoints[2 * i + nearCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * yzScale; p1.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * yzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                }
            }
            if (CylinderModel[1] < CylinderModel[4]) {
                nearCenter = 0;
                farCenter = 1;
            }
            else {
                nearCenter = 1;
                farCenter = 0;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 49 + cylinderPoints[2 * i + nearCenter].z * xzScale; p1.y = 599.5 + cylinderPoints[2 * i + nearCenter].x * xzScale;
                p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * xzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].x * xzScale;
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].y < CylinderModel[1 + 3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].y < CylinderModel[1 + 3 * farCenter])) {
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * xzScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * xzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * xzScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * i + nearCenter].z * xzScale; p2.y = 599.5 + cylinderPoints[2 * i + nearCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * xzScale; p1.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * xzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                }
            }
            break;
        }

    }


    //    imwrite("projectionXY.jpg", projectionXY);
    //    imwrite("projectionYZ.jpg", projectionYZ);
    //    imwrite("projectionXZ.jpg", projectionXZ);
    resize(projectionXY, projectionXY, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(projectionYZ, projectionYZ, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(projectionXZ, projectionXZ, Size(), 0.5, 0.5, INTER_LINEAR);

    Rect r1(0, 0, 960, 600);
    Rect r2(960, 0, 960, 600);
    Rect r3(0, 600, 960, 600);
    Rect r4(960, 600, 960, 600);
    resize(res4, res4, Size(), 0.5, 0.5, INTER_LINEAR);
    res4.copyTo(finalImage(r1));
    projectionXY.copyTo(finalImage(r2));
    projectionYZ.copyTo(finalImage(r3));
    projectionXZ.copyTo(finalImage(r4));
    p1.x = 0; p1.y = 599.5;
    p2.x = 1919; p2.y = 599.5;
    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    p1.x = 959.5; p1.y = 0;
    p2.x = 959.5; p2.y = 1199;
    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    Rect rectF(0, 0, 1920, 1200);
    rectangle(finalImage, rectF, cv::Scalar(0, 0, 0), 2);
    //cv::imwrite("FinalImage.jpg", finalImage);

    ClasterDataOut << "];" << endl;

    imgResult01r = finalImage.clone();



}


void ADS::cl_DenseStereo::DetectPrimitive_single()
{


    //    // ===========
    //    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    //    // ============

    t_vuxyzrgb vuxyzrgb_temp = vuxyzrgb;

    //    // ===========
    //    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    //    // ============



    vector<vector<double>> figureModel;

    vector<vector<Point3d>> clastersData;

    vector<int> IDX = vuxyzrgb_temp.cluster;
    vector<Point3d> xyz;
    xyz.resize( vuxyzrgb_temp.num_p);

    int numClusters = vuxyzrgb_temp.num_cl;


    // == Правка====

    for (int i = 0; i < vuxyzrgb_temp.num_p ; i++)
    {
        xyz.at(i).x = vuxyzrgb_temp.xyz.at(i).at(0);
        xyz.at(i).y = vuxyzrgb_temp.xyz.at(i).at(1);
        xyz.at(i).z = vuxyzrgb_temp.xyz.at(i).at(2);
    }




    for (int j = 0; j < numClusters; j++)
    {
        vector<Point3d> xyz2;
        for (int i = 0; i < int(IDX.size()); i++)
            if (IDX[i] == (j ))
            {
                xyz2.push_back(xyz[i]);
            }
        clastersData.push_back(xyz2);
    }



    for (int k = 0; k < vuxyzrgb_temp.num_cl; k++)
    {
        cout << "\t ||| DetectPrimitive02(): clastersData["<< k
             <<"].size(): " << clastersData[k].size()<< endl;

    }





    // =====================================
    // Определение параметров модели объекта
    // =====================================

    if (vuxyzrgb_temp.SelectedClaster > -1 )
    {
        int k = vuxyzrgb_temp.SelectedClaster;


        //*****************************************   Fit plane   ************************
        vector<Point3d> planePoints;
        vector<double> PlaneModel = ADS::FitPlane(clastersData[k], planePoints);



        //*****************************************   Fit sphere   ************************
        vector<double> SphereModel = FitSphere(clastersData[k]);

        //*****************************************   Fit cylinder   ************************
        vector<Point3d> cylinderPoints;
        vector<double> CylinderModel = FitCylinder(clastersData[k], PlaneModel, cylinderPoints);



        // =====================================
        // Определение вида примитива
        // =====================================


        vuxyzrgb_temp.PrimitiveDetect[vuxyzrgb_temp.SelectedClaster] = 0;
        vuxyzrgb_temp.PrimitiveParameter.resize(8);

        double minErrorOfModel = 0.0;
        if (PlaneModel[PlaneModel.size() - 1] < SphereModel[SphereModel.size() - 1])
        {
            minErrorOfModel = PlaneModel[PlaneModel.size() - 1];
            vuxyzrgb_temp.PrimitiveDetect[vuxyzrgb_temp.SelectedClaster] = 1;
            vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster] = PlaneModel;
        }
        else
        {
            minErrorOfModel = SphereModel[SphereModel.size() - 1];
            vuxyzrgb_temp.PrimitiveDetect[vuxyzrgb_temp.SelectedClaster] = 2;
            vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster] = SphereModel;
        }
        if (CylinderModel[CylinderModel.size() - 1] < minErrorOfModel)
        {
            minErrorOfModel = CylinderModel[CylinderModel.size() - 1];
            vuxyzrgb_temp.PrimitiveDetect[vuxyzrgb_temp.SelectedClaster] = 3;
            vuxyzrgb_temp.PrimitiveParameter[vuxyzrgb_temp.SelectedClaster] = CylinderModel;
        }




        //        std::cout << endl << "\t ++++>>> Primitive: " <<  vuxyzrgb_temp.PrimitiveDetect[vuxyzrgb_temp.SelectedClaster] << " is: ";
        //        switch (vuxyzrgb_temp.PrimitiveDetect[vuxyzrgb_temp.SelectedClaster])
        //        {
        //        case 0:
        //            std::cout << "Non" << endl;
        //            break;
        //        case 1:
        //            std::cout  << "\t ++++>>> Primitive: Claster" << k  << ":" << endl << endl;
        //            std::cout  << "\t ++++>>> Primitive: Plane:" << endl << "[ ";

        //            for (int i = 0; i < int(PlaneModel.size()); i++)
        //            {
        //                std::cout << PlaneModel[i] << " ";
        //            }
        //            std::cout << "]" << endl << endl;

        //            break;
        //        case 2:
        //            std::cout  << "\t ++++>>> Primitive: Claster" << k  << ":" << endl << endl;
        //            std::cout  << "\t ++++>>> Primitive: Sphere:" << endl << "[ ";

        //            for (int i = 0; i < int(SphereModel.size()); i++)
        //            {
        //                std::cout << SphereModel[i] << " ";
        //            }
        //            std::cout << "]" << endl << endl;

        //            break;
        //        case 3:
        //            std::cout  << "\t ++++>>> Primitive: Claster" << k  << ":" << endl << endl;
        //            std::cout  << "\t ++++>>> Primitive: Cylinder:" << endl << "[ ";

        //            for (int i = 0; i < int (CylinderModel.size()); i++)
        //            {
        //                std::cout << CylinderModel[i] << " ";
        //            }
        //            std::cout << "]" << endl << endl;
        //        }



        // ===========
        // Mutex (lock)
        vuxyzrgb_mutex.lock();
        // ============



        vuxyzrgb.PrimitiveDetect = vuxyzrgb_temp.PrimitiveDetect;
        vuxyzrgb.PrimitiveParameter = vuxyzrgb_temp.PrimitiveParameter;


        // ===========
        // Mutex (unlock)
        vuxyzrgb_mutex.unlock();
        // ============

    }



}


void ADS::cl_DenseStereo::DetectPrimitive_all()
{


    //    // ===========
    //    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    //    // ============

    t_vuxyzrgb vuxyzrgb_temp = vuxyzrgb;

    //    // ===========
    //    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    //    // ============

    //    cout << "\t |||>> (0) DetectPrimitive_all(): BIGIN "  << endl;

    vector<vector<double>> figureModel;

    vector<vector<Point3d>> clastersData;

    vector<int> IDX = vuxyzrgb_temp.cluster;
    vector<Point3d> xyz;
    xyz.resize( vuxyzrgb_temp.num_p);


    // Пересчет размеров массивов для хранения данных о приметивах
    vuxyzrgb_temp.PrimitiveDetect.resize(vuxyzrgb_temp.num_cl);
    vuxyzrgb_temp.PrimitiveParameter.resize(vuxyzrgb_temp.num_cl);
    vuxyzrgb_temp.PrimitivePoints.resize(vuxyzrgb_temp.num_cl);



    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveDetect.size() " << vuxyzrgb_temp.PrimitiveDetect.size()  << endl;

    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveParameter.size() " << vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveParameter.size() " << vuxyzrgb_temp.PrimitiveParameter.size()  << endl;

    //    cout << "\t |||>> vuxyzrgb_temp.PrimitivePoints.size() " << vuxyzrgb_temp.PrimitivePoints.size()  << endl;


    int numClusters = vuxyzrgb_temp.num_cl;


    // == Правка====

    for (int i = 0; i < vuxyzrgb_temp.num_p ; i++)
    {
        xyz.at(i).x = vuxyzrgb_temp.xyz.at(i).at(0);
        xyz.at(i).y = vuxyzrgb_temp.xyz.at(i).at(1);
        xyz.at(i).z = vuxyzrgb_temp.xyz.at(i).at(2);
    }




    for (int j = 0; j < numClusters; j++)
    {
        vector<Point3d> xyz2;
        for (int i = 0; i < int(IDX.size()); i++)
            if (IDX[i] == (j ))
            {
                xyz2.push_back(xyz[i]);
            }
        clastersData.push_back(xyz2);
    }



    //    for (int k = 0; k < vuxyzrgb_temp.num_cl; k++)
    //    {
    //        cout << "\t ++>>> DetectPrimitive_all(): clastersData["<< k
    //             <<"].size(): " << clastersData[k].size()<< endl;

    //    }





    // =====================================
    // Определение модели объекта
    // =====================================


    for (int k = 0; k < int(clastersData.size()); k++)
    {

        //*****************************************   Fit plane   ************************
        vector<Point3d> planePoints;


        vector<double> PlaneModel = ADS::FitPlane(clastersData[k], planePoints);



        //*****************************************   Fit sphere   ************************
        vector<double> SphereModel = FitSphere(clastersData[k]);

        //*****************************************   Fit cylinder   ************************
        vector<Point3d> cylinderPoints;
        vector<double> CylinderModel = FitCylinder(clastersData[k], PlaneModel, cylinderPoints);



        // =====================================
        // Определение вида примитива
        // =====================================


        int figureInClaster = 0;
        double minErrorOfModel = 0.0;
        if (PlaneModel[PlaneModel.size() - 1] < SphereModel[SphereModel.size() - 1])
        {
            minErrorOfModel = PlaneModel[PlaneModel.size() - 1];
            figureInClaster = 1;
        }
        else
        {
            minErrorOfModel = SphereModel[SphereModel.size() - 1];
            figureInClaster = 2;
        }
        if (CylinderModel[CylinderModel.size() - 1] < minErrorOfModel) {
            minErrorOfModel = CylinderModel[CylinderModel.size() - 1];
            figureInClaster = 3;
        }


        switch (figureInClaster)
        {
        case 0:
            std::cout << "Non" << endl;
            break;
        case 1: // Plane


            vuxyzrgb_temp.PrimitiveDetect[k] = 1;
            vuxyzrgb_temp.PrimitiveParameter[k] = PlaneModel;
            vuxyzrgb_temp.PrimitivePoints[k] = planePoints;

            //            std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Plane: " <<  " [ ";

            //            for (int i = 0; i < int(PlaneModel.size()); i++)
            //            {
            //                std::cout << PlaneModel[i] << " ";
            //            }
            //            std::cout << "]" <<  endl;

            break;
        case 2: // Sphere

            // if (R>l) {}


            vuxyzrgb_temp.PrimitiveDetect[k] = 2;
            vuxyzrgb_temp.PrimitiveParameter[k] = SphereModel;
            vuxyzrgb_temp.PrimitivePoints[k] = {};


            //            std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Sphere: " <<  " [ ";
            //            for (int i = 0; i < int(SphereModel.size()); i++)
            //            {
            //                std::cout << SphereModel[i] << " ";
            //            }
            //            std::cout << "]" <<  endl;

            // end if

            break;
        case 3:  // Cylinder


            // if (R>l) {}



            vuxyzrgb_temp.PrimitiveDetect[k] = 3;
            vuxyzrgb_temp.PrimitiveParameter[k] = CylinderModel;
            vuxyzrgb_temp.PrimitivePoints[k] = cylinderPoints;




            //            std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Cylinder:"  << " [ ";
            //            for (int i = 0; i < int (CylinderModel.size()); i++)
            //            {
            //                std::cout << CylinderModel[i] << " ";
            //            }
            //            std::cout << "]" <<  endl;

            // end if


        }

    }


    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitiveDetect.size() " <<  vuxyzrgb_temp.PrimitiveDetect.size()  << endl;
    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitiveParameter.size() " <<  vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitivePoints.size() " <<  vuxyzrgb_temp.PrimitivePoints.size()  << endl;

    //    cout << "\t |||>> (9999) DetectPrimitive_all(): END "  << endl;

    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    // ============



    vuxyzrgb.PrimitiveDetect = vuxyzrgb_temp.PrimitiveDetect;
    vuxyzrgb.PrimitiveParameter = vuxyzrgb_temp.PrimitiveParameter;
    vuxyzrgb.PrimitivePoints = vuxyzrgb_temp.PrimitivePoints;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============






}


void ADS::cl_DenseStereo::AddPrimitiveToIm()
{

    cv::Mat imgLeft01r_temp; // Инициализация изображения для вывода результатов
    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура

    imgLeft01r_temp = getImgOpenCV_1left();

    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();

    // ============

    vuxyzrgb_temp = vuxyzrgb;
    vuxyzrgb_arma_temp = vuxyzrgb_arma;

    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============




    //  cout << "\t |||>> (0) AddPrimitiveToIm(): BIGIN "  << endl;


    // =====================================
    // Проверка на пустоту
    // =====================================

    if (vuxyzrgb_temp.PrimitiveDetect.empty()) return;
    if (imgLeft01r_temp.empty()) return;

    // =====================================

    //  cout << "\t |||>> (1) AddPrimitiveToIm() "  << endl;

    cv::Size imgSize = imgLeft01r_temp.size();

    vector<vector<double>> figureModel;
    Mat finalImage = Mat::zeros(imgSize, CV_8UC3);
    vector<vector<Point3d>> clastersData;

    vector<int> IDX = vuxyzrgb_temp.cluster;
    vector<Point3d> xyz;
    xyz.resize( vuxyzrgb_temp.num_p);

    int numClusters = vuxyzrgb_temp.num_cl;


    // == Правка====

    for (int i = 0; i < int( vuxyzrgb_temp.xyz.size() ); i++)
    {
        xyz.at(i).x = vuxyzrgb_temp.xyz.at(i).at(0);
        xyz.at(i).y = vuxyzrgb_temp.xyz.at(i).at(1);
        xyz.at(i).z = vuxyzrgb_temp.xyz.at(i).at(2);
    }




    for (int j = 0; j < numClusters; j++) {
        vector<Point3d> xyz2;
        for (int i = 0; i < int(IDX.size()); i++)
            if (IDX[i] == (j )) {
                xyz2.push_back(xyz[i]);
            }
        clastersData.push_back(xyz2);
    }

    // =====================================



    //   cout << "\t |||>> (2) AddPrimitiveToIm() "  << endl;


    // =====================================
    //
    // =====================================

    Mat projectionXY = Mat::ones(imgSize, CV_8UC3);
    projectionXY = Scalar(255, 255, 255);
    Mat projectionYZ = Mat::ones(imgSize, CV_8UC3);
    projectionYZ = Scalar(255, 255, 255);
    Mat projectionXZ = Mat::ones(imgSize, CV_8UC3);
    projectionXZ = Scalar(255, 255, 255);


    // =====================================
    // Проверка на выбросы
    // =====================================
    double xLimMin = 1e+8;
    double xLimMax = -1e+8;
    double yLimMin = 1e+8;
    double yLimMax = -1e+8;
    double zLimMin = 1e+8;
    double zLimMax = -1e+8;
    for (int i = 0; i < int(xyz.size()); i++) {
        //if (IDX[i] != 0) {
        if (xyz[i].x < xLimMin)
            xLimMin = xyz[i].x;
        if (xyz[i].x > xLimMax)
            xLimMax = xyz[i].x;
        if (xyz[i].y < yLimMin)
            yLimMin = xyz[i].y;
        if (xyz[i].y > yLimMax)
            yLimMax = xyz[i].y;
        if (xyz[i].z < zLimMin)
            zLimMin = xyz[i].z;
        if (xyz[i].z > zLimMax)
            zLimMax = xyz[i].z;
        //}
    }


    //   cout << "\t |||>> (3) AddPrimitiveToIm() "  << endl;


    // =====================================
    // Создание изображения с результатом (прорисовка сетки и др.)
    // =====================================

    double xyScale, yzScale, xzScale, xScale, yScale, zScale;
    xScale = min(abs(910 / xLimMax), abs(910 / xLimMin));
    yScale = min(abs(550 / yLimMax), abs(550 / yLimMin));
    xyScale = min(xScale, yScale);
    zScale = 1820 / zLimMax;
    yzScale = min(zScale, yScale);
    xScale = min(abs(550 / xLimMax), abs(550 / xLimMin));
    xzScale = min(xScale, zScale);
    Rect rect(49, 49, 1820, 1100);
    rectangle(projectionXY, rect, cv::Scalar(0, 0, 0));
    rectangle(projectionYZ, rect, cv::Scalar(0, 0, 0));
    rectangle(projectionXZ, rect, cv::Scalar(0, 0, 0));
    Point2d p1, p2;
    p1.x = 49; p1.y = 599.5;
    p2.x = 1869; p2.y = 599.5;
    line(projectionXY, p1, p2, Scalar(200, 200, 200));
    p1.x = 959.5; p1.y = 49;
    p2.x = 959.5; p2.y = 1149;
    line(projectionXY, p1, p2, Scalar(200, 200, 200));
    p1.x = 49; p1.y = 599.5;
    p2.x = 1869; p2.y = 599.5;
    line(projectionYZ, p1, p2, Scalar(200, 200, 200));
    line(projectionXZ, p1, p2, Scalar(200, 200, 200));
    char textZ[10];
    Point2d textPoint;
    textPoint.x = 49;
    textPoint.y = 40;
    putText(projectionXY, "Y", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionYZ, "Y", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionXZ, "X", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    textPoint.x = 1878;
    textPoint.y = 1149;
    putText(projectionXY, "X", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionYZ, "Z", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionXZ, "Z", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    textPoint.x = 20;
    textPoint.y = 610;
    putText(projectionXY, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionYZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionXZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    textPoint.x = 40;
    textPoint.y = 1180;
    putText(projectionYZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionXZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    textPoint.x = 950;
    textPoint.y = 1180;
    putText(projectionXY, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));

    // =====================================


    //   cout << "\t |||>> (4) AddPrimitiveToIm() "  << endl;




    Mat res4 = imgLeft01r_temp.clone();


    // == Правка====
    vector<Point2d> kp1;
    kp1.resize(int( vuxyzrgb_temp.xyz.size() ));

    for (int i = 0; i < int( vuxyzrgb_temp.xyz.size() ); i++)
    {
        kp1.at(i).x = double(vuxyzrgb_temp.vu.at(i).at(1) );
        kp1.at(i).y = double(vuxyzrgb_temp.vu.at(i).at(0) );
    }

    // =====================================

    // =====================================
    // Прорисовка точек
    // =====================================

    for (int i = 0; i < int(clastersData.size()); i++) {
        Mat res3 = imgLeft01r_temp.clone();
        //char clasterFilename[100];
        //sprintf_s(clasterFilename, "Claster%d.jpg", i + 1);
        double xmin = 1000000, xmax = 0, ymin = 1000000, ymax = 0;
        double meanZ = 0.0;
        int countZ = 0;
        int tempIdx = 0;
        for (int j = 0; j < int(IDX.size()); j++)
            if (IDX[j] == (i)) {                                        //!!!!!!!!!!!!!!!!!!!!!!!!!!!
                circle(res3, kp1[j], 3, Scalar(0, 255, 0));
                circle(res4, kp1[j], 3, Scalar(0, 255, 0));
                if (kp1[j].x < xmin)
                    xmin = kp1[j].x;
                if (kp1[j].x > xmax)
                    xmax = kp1[j].x;
                if (kp1[j].y < ymin)
                    ymin = kp1[j].y;
                if (kp1[j].y > ymax)
                    ymax = kp1[j].y;
                meanZ += clastersData[i][tempIdx].z;
                countZ++;
                tempIdx++;


                Point2d pcPoint;
                pcPoint.x = 959.5 + xyz[j].x * xyScale;
                pcPoint.y = 599.5 + xyz[j].y * xyScale;
                circle(projectionXY, pcPoint, 3, Scalar(0, 255, 0));
                pcPoint.x = 49 + xyz[j].z * yzScale;
                pcPoint.y = 599.5 + xyz[j].y * yzScale;
                circle(projectionYZ, pcPoint, 3, Scalar(0, 255, 0));
                pcPoint.x = 49 + xyz[j].z * xzScale;
                pcPoint.y = 599.5 + xyz[j].x * xzScale;
                circle(projectionXZ, pcPoint, 3, Scalar(0, 255, 0));
            }
        meanZ /= double(countZ);
        Point2d lt, rt, lb, rb;
        lt.x = xmin; lt.y = ymin;
        rt.x = xmax; rt.y = ymin;
        lb.x = xmin; lb.y = ymax;
        rb.x = xmax; rb.y = ymax;
        line(res3, lt, rt, Scalar(0, 255, 255), 2);
        line(res3, lt, lb, Scalar(0, 255, 255), 2);
        line(res3, lb, rb, Scalar(0, 255, 255), 2);
        line(res3, rt, rb, Scalar(0, 255, 255), 2);
        line(res4, lt, rt, Scalar(0, 255, 255), 2);
        line(res4, lt, lb, Scalar(0, 255, 255), 2);
        line(res4, lb, rb, Scalar(0, 255, 255), 2);
        line(res4, rt, rb, Scalar(0, 255, 255), 2);
       // char textZ[100];
        Point2d textPoint;
        textPoint.x = (xmax + xmin) / 2;
        textPoint.y = ymax + 15;
        //sprintf_s(textZ, "Mean Z, mm: %7.2f", meanZ);
        putText(res3, textZ, textPoint, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
        //putText(res4, textZ, textPoint, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
        //cv::imwrite(clasterFilename, res3);
    }
    //cv::imwrite("AllClasters.jpg", res4);



    //    cout << "\t |||>> (5) AddPrimitiveToIm() "  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitiveDetect.size() " <<  vuxyzrgb_temp.PrimitiveDetect.size()  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitiveParameter.size() " <<  vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitivePoints.size() " <<  vuxyzrgb_temp.PrimitivePoints.size()  << endl;
    //    cout << "\t |||>> (5) clastersData.size():   " << clastersData.size()  << endl;
    //    cout << "\t |||>> (5) vuxyzrgb_temp.num_cl:   " << vuxyzrgb_temp.num_cl  << endl;


    vector<Point3d> planePoints;
    vector<Point3d> cylinderPoints;
    vector<double> PlaneModel;
    vector<double> SphereModel;
    vector<double> CylinderModel;


    Point2d pcPoint;





    for (int k = 0; k < int(clastersData.size()); k++) {



        // cout << "\t |||>> (6) AddPrimitiveToIm() k =  " << k  << "  vuxyzrgb_temp.PrimitiveDetect[k]: " << vuxyzrgb_temp.PrimitiveDetect[k]  << endl;


        switch (vuxyzrgb_temp.PrimitiveDetect[k])
        {
        case 1: // ========== Fit plane  =========



            PlaneModel = vuxyzrgb_temp.PrimitiveParameter[k];
            planePoints = vuxyzrgb_temp.PrimitivePoints[k];

            for (int i = 0; i < 6; i++) {
                p1.x = 959.5 + planePoints[0].x * xyScale + 0.2 * i * (planePoints[1].x * xyScale - planePoints[0].x * xyScale);
                p1.y = 599.5 + planePoints[0].y * xyScale + 0.2 * i * (planePoints[1].y * xyScale - planePoints[0].y * xyScale);
                p2.x = 959.5 + planePoints[2].x * xyScale + 0.2 * i * (planePoints[3].x * xyScale - planePoints[2].x * xyScale);
                p2.y = 599.5 + planePoints[2].y * xyScale + 0.2 * i * (planePoints[3].y * xyScale - planePoints[2].y * xyScale);
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                p1.x = 959.5 + planePoints[0].x * xyScale + 0.2 * i * (planePoints[2].x * xyScale - planePoints[0].x * xyScale);
                p1.y = 599.5 + planePoints[0].y * xyScale + 0.2 * i * (planePoints[2].y * xyScale - planePoints[0].y * xyScale);
                p2.x = 959.5 + planePoints[1].x * xyScale + 0.2 * i * (planePoints[3].x * xyScale - planePoints[1].x * xyScale);
                p2.y = 599.5 + planePoints[1].y * xyScale + 0.2 * i * (planePoints[3].y * xyScale - planePoints[1].y * xyScale);
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
            }
            for (int i = 0; i < 6; i++) {
                p1.x = 49 + planePoints[0].z * yzScale + 0.2 * i * (planePoints[1].z * yzScale - planePoints[0].z * yzScale);
                p1.y = 599.5 + planePoints[0].y * yzScale + 0.2 * i * (planePoints[1].y * yzScale - planePoints[0].y * yzScale);
                p2.x = 49 + planePoints[2].z * yzScale + 0.2 * i * (planePoints[3].z * yzScale - planePoints[2].z * yzScale);
                p2.y = 599.5 + planePoints[2].y * yzScale + 0.2 * i * (planePoints[3].y * yzScale - planePoints[2].y * yzScale);
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                p1.x = 49 + planePoints[0].z * yzScale + 0.2 * i * (planePoints[2].z * yzScale - planePoints[0].z * yzScale);
                p1.y = 599.5 + planePoints[0].y * yzScale + 0.2 * i * (planePoints[2].y * yzScale - planePoints[0].y * yzScale);
                p2.x = 49 + planePoints[1].z * yzScale + 0.2 * i * (planePoints[3].z * yzScale - planePoints[1].z * yzScale);
                p2.y = 599.5 + planePoints[1].y * yzScale + 0.2 * i * (planePoints[3].y * yzScale - planePoints[1].y * yzScale);
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
            }
            for (int i = 0; i < 6; i++) {
                p1.x = 49 + planePoints[0].z * xzScale + 0.2 * i * (planePoints[1].z * xzScale - planePoints[0].z * xzScale);
                p1.y = 599.5 + planePoints[0].x * xzScale + 0.2 * i * (planePoints[1].x * xzScale - planePoints[0].x * xzScale);
                p2.x = 49 + planePoints[2].z * xzScale + 0.2 * i * (planePoints[3].z * xzScale - planePoints[2].z * xzScale);
                p2.y = 599.5 + planePoints[2].x * xzScale + 0.2 * i * (planePoints[3].x * xzScale - planePoints[2].x * xzScale);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                p1.x = 49 + planePoints[0].z * xzScale + 0.2 * i * (planePoints[2].z * xzScale - planePoints[0].z * xzScale);
                p1.y = 599.5 + planePoints[0].x * xzScale + 0.2 * i * (planePoints[2].x * xzScale - planePoints[0].x * xzScale);
                p2.x = 49 + planePoints[1].z * xzScale + 0.2 * i * (planePoints[3].z * xzScale - planePoints[1].z * xzScale);
                p2.y = 599.5 + planePoints[1].x * xzScale + 0.2 * i * (planePoints[3].x * xzScale - planePoints[1].x * xzScale);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
            }

            break;


        case 2:  // ======== Fit sphere==========

            SphereModel = vuxyzrgb_temp.PrimitiveParameter[k];

            figureModel.push_back(SphereModel);
            pcPoint.x = 959.5 + SphereModel[0] * xyScale;
            pcPoint.y = 599.5 + SphereModel[1] * xyScale;
            circle(projectionXY, pcPoint, SphereModel[3] * xyScale, Scalar(50, 50, 50));
            double xLeft, xRight, yTop, yLeft, yRigth;
            yTop = pcPoint.y - SphereModel[3] * xyScale;
            for (int i = 0; i < 4; i++) {
                xLeft = pcPoint.x - sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * i * SphereModel[3] * xyScale, 2));
                xRight = pcPoint.x + sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * i * SphereModel[3] * xyScale, 2));
                yLeft = yRigth = yTop + 0.5 * i * SphereModel[3] * xyScale;
                p1.x = xLeft; p1.y = yLeft;
                p2.x = xRight; p2.y = yRigth;
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                for (int j = 0; j < 3; j++) {
                    p1.x = xLeft + (xRight - xLeft) * 0.25 * (j + 1);
                    p1.y = yLeft;
                    p2.x = pcPoint.x - sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * (i + 1) * SphereModel[3] * xyScale, 2)) + 2 * sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * (i + 1) * SphereModel[3] * xyScale, 2)) * 0.25 * (j + 1);
                    p2.y = yTop + 0.5 * (i + 1) * SphereModel[3] * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                }
            }

            pcPoint.x = 49 + SphereModel[2] * yzScale;
            pcPoint.y = 599.5 + SphereModel[1] * yzScale;
            circle(projectionYZ, pcPoint, SphereModel[3] * yzScale, Scalar(50, 50, 50));
            yTop = pcPoint.y - SphereModel[3] * yzScale;
            for (int i = 0; i < 4; i++) {
                xLeft = pcPoint.x - sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * i * SphereModel[3] * yzScale, 2));
                xRight = pcPoint.x + sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * i * SphereModel[3] * yzScale, 2));
                yLeft = yRigth = yTop + 0.5 * i * SphereModel[3] * yzScale;
                p1.x = xLeft; p1.y = yLeft;
                p2.x = xRight; p2.y = yRigth;
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                for (int j = 0; j < 3; j++) {
                    p1.x = xLeft + (xRight - xLeft) * 0.25 * (j + 1);
                    p1.y = yLeft;
                    p2.x = pcPoint.x - sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * (i + 1) * SphereModel[3] * yzScale, 2)) + 2 * sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * (i + 1) * SphereModel[3] * yzScale, 2)) * 0.25 * (j + 1);
                    p2.y = yTop + 0.5 * (i + 1) * SphereModel[3] * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                }
            }

            pcPoint.x = 49 + SphereModel[2] * xzScale;
            pcPoint.y = 599.5 + SphereModel[0] * xzScale;
            circle(projectionXZ, pcPoint, SphereModel[3] * xzScale, Scalar(50, 50, 50));
            circle(projectionXZ, pcPoint, 0.5 * SphereModel[3] * xzScale, Scalar(50, 50, 50));
            for (int i = 0; i < 4; i++) {
                p1.x = pcPoint.x + SphereModel[3] * xzScale * cos(i * CV_PI / 4); p1.y = pcPoint.y + SphereModel[3] * xzScale * sin(i * CV_PI / 4);
                p2.x = pcPoint.x + SphereModel[3] * xzScale * cos(i * CV_PI / 4 + CV_PI); p2.y = pcPoint.y + SphereModel[3] * xzScale * sin(i * CV_PI / 4 + CV_PI);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
            }
            break;


        case 3:  // ========== Fit cylinder =============


            CylinderModel = vuxyzrgb_temp.PrimitiveParameter[k];
            cylinderPoints = vuxyzrgb_temp.PrimitivePoints[k];

            figureModel.push_back(CylinderModel);
            int nearCenter, farCenter;
            if (CylinderModel[2] < CylinderModel[5]) {
                nearCenter = 0;
                farCenter = 1;
            }
            else {
                nearCenter = 1;
                farCenter = 0;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 959.5 + cylinderPoints[2 * i + nearCenter].x * xyScale; p1.y = 599.5 + cylinderPoints[2 * i + nearCenter].y * xyScale;
                p2.x = 959.5 + cylinderPoints[2 * (i + 1) + nearCenter].x * xyScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].y * xyScale;
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].z < CylinderModel[2 + 3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].z < CylinderModel[2 + 3 * farCenter])) {
                    p1.x = 959.5 + cylinderPoints[2 * i + farCenter].x * xyScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].y * xyScale;
                    p2.x = 959.5 + cylinderPoints[2 * (i + 1) + farCenter].x * xyScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                    p1.x = 959.5 + cylinderPoints[2 * i + farCenter].x * xyScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].y * xyScale;
                    p2.x = 959.5 + cylinderPoints[2 * i + nearCenter].x * xyScale; p2.y = 599.5 + cylinderPoints[2 * i + nearCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                    p1.x = 959.5 + cylinderPoints[2 * (i + 1) + farCenter].x * xyScale; p1.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].y * xyScale;
                    p2.x = 959.5 + cylinderPoints[2 * (i + 1) + nearCenter].x * xyScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                }
            }
            if (CylinderModel[0] < CylinderModel[3]) {
                nearCenter = 1;
                farCenter = 0;
            }
            else {
                nearCenter = 0;
                farCenter = 1;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 49 + cylinderPoints[2 * i + nearCenter].z * yzScale; p1.y = 599.5 + cylinderPoints[2 * i + nearCenter].y * yzScale;
                p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * yzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].y * yzScale;
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].x > CylinderModel[3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].x > CylinderModel[3 * farCenter])) {
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * yzScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * yzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * yzScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * i + nearCenter].z * yzScale; p2.y = 599.5 + cylinderPoints[2 * i + nearCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * yzScale; p1.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * yzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                }
            }
            if (CylinderModel[1] < CylinderModel[4]) {
                nearCenter = 0;
                farCenter = 1;
            }
            else {
                nearCenter = 1;
                farCenter = 0;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 49 + cylinderPoints[2 * i + nearCenter].z * xzScale; p1.y = 599.5 + cylinderPoints[2 * i + nearCenter].x * xzScale;
                p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * xzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].x * xzScale;
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].y < CylinderModel[1 + 3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].y < CylinderModel[1 + 3 * farCenter])) {
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * xzScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * xzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * xzScale; p1.y = 599.5 + cylinderPoints[2 * i + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * i + nearCenter].z * xzScale; p2.y = 599.5 + cylinderPoints[2 * i + nearCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * xzScale; p1.y = 599.5 + cylinderPoints[2 * (i + 1) + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * xzScale; p2.y = 599.5 + cylinderPoints[2 * (i + 1) + nearCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                }
            }
            break;
        }

    }


    //    imwrite("projectionXY.jpg", projectionXY);
    //    imwrite("projectionYZ.jpg", projectionYZ);
    //    imwrite("projectionXZ.jpg", projectionXZ);
    resize(projectionXY, projectionXY, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(projectionYZ, projectionYZ, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(projectionXZ, projectionXZ, Size(), 0.5, 0.5, INTER_LINEAR);

    Rect r1(0, 0, 960, 600);
    Rect r2(960, 0, 960, 600);
    Rect r3(0, 600, 960, 600);
    Rect r4(960, 600, 960, 600);
    resize(res4, res4, Size(), 0.5, 0.5, INTER_LINEAR);
    res4.copyTo(finalImage(r1));
    projectionXY.copyTo(finalImage(r2));
    projectionYZ.copyTo(finalImage(r3));
    projectionXZ.copyTo(finalImage(r4));
    p1.x = 0; p1.y = 599.5;
    p2.x = 1919; p2.y = 599.5;
    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    p1.x = 959.5; p1.y = 0;
    p2.x = 959.5; p2.y = 1199;
    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    Rect rectF(0, 0, 1920, 1200);
    rectangle(finalImage, rectF, cv::Scalar(0, 0, 0), 2);
    //cv::imwrite("FinalImage.jpg", finalImage);


    // cout << "\t |||>> (9999) AddPrimitiveToIm(): END "  << endl;

    // ===========
    // Mutex (lock)
    imgResult_mutex.lock();
    // ============

    imgPrimitive01r_general = finalImage.clone();


    // ===========
    // Mutex (unlock)
    imgResult_mutex.unlock();
    // ============




}


void ADS::cl_DenseStereo::DetectAddPrimitive()
{

    cv::Mat imgLeft01r_temp; // Инициализация изображения для вывода результатов
    cv::Mat imgResult01r_temp;
    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура


    imgLeft01r_temp = getImgOpenCV_1left();

    // ===========
    // Mutex (lock)
    std::lock(vuxyzrgb_mutex,imgResult_mutex);

    // ============

    imgResult01r_temp = imgResult01r.clone();

    vuxyzrgb_temp = vuxyzrgb;
    vuxyzrgb_arma_temp = vuxyzrgb_arma;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    imgResult_mutex.unlock();
    // ============

    // ============
    // Если точек нет то выход
    // ============
    if (vuxyzrgb_temp.num_p < 1) return;





    cv::Size imgSize = imgResult01r_temp.size();
    cout << "\t\t\t\t >>>>>>>>>>>>>>>>>>>>imgSize = " << imgSize <<endl;


    int h = imgSize.height;
    int w = imgSize.width;

    int ho = (h/2)-0.5;
    int wo = (w/2)-0.5;

//    p2.y = 599.5;
//    p2.x = 959.5;



    if (vuxyzrgb_temp.num_cl < vuxyzrgb_temp.SelectedClaster) vuxyzrgb_temp.SelectedClaster = vuxyzrgb_temp.num_cl;


     cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 01 "  <<endl;


    // cout << "\t |||>> (0) DetectPrimitive_all(): BIGIN "  << endl;

    vector<vector<double>> figureModel;
    Mat finalImage = Mat::zeros(imgSize, CV_8UC3);
    vector<vector<Point3d>> clastersData;




    vector<Point3d> xyz;
    xyz.resize( vuxyzrgb_temp.num_p);




    // Используемые переменные
    vector<int> IDX = vuxyzrgb_temp.cluster;;
    int numClusters= vuxyzrgb_temp.num_cl;;




    // Пересчет размеров массивов для хранения данных о приметивах
    vuxyzrgb_temp.PrimitiveDetect.resize(vuxyzrgb_temp.num_cl);
    vuxyzrgb_temp.PrimitiveParameter.resize(vuxyzrgb_temp.num_cl);
    vuxyzrgb_temp.PrimitivePoints.resize(vuxyzrgb_temp.num_cl);


    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveDetect.size() " << vuxyzrgb_temp.PrimitiveDetect.size()  << endl;

    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveParameter.size() " << vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveParameter.size() " << vuxyzrgb_temp.PrimitiveParameter.size()  << endl;

    //    cout << "\t |||>> vuxyzrgb_temp.PrimitivePoints.size() " << vuxyzrgb_temp.PrimitivePoints.size()  << endl;


    cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 02 "  <<endl;


    // == Правка====

    for (int i = 0; i < vuxyzrgb_temp.num_p ; i++)
    {
        xyz.at(i).x = vuxyzrgb_temp.xyz.at(i).at(0);
        xyz.at(i).y = vuxyzrgb_temp.xyz.at(i).at(1);
        xyz.at(i).z = vuxyzrgb_temp.xyz.at(i).at(2);
    }




    for (int j = 0; j < numClusters; j++)
    {
        vector<Point3d> xyz2;
        for (int i = 0; i < int(IDX.size()); i++)
            if (IDX[i] == (j ))
            {
                xyz2.push_back(xyz[i]);
            }
        clastersData.push_back(xyz2);
    }



    //    for (int k = 0; k < vuxyzrgb_temp.num_cl; k++)
    //    {
    //        cout << "\t ++>>> DetectPrimitive_all(): clastersData["<< k
    //             <<"].size(): " << clastersData[k].size()<< endl;

    //    }





    cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 03 "  <<endl;


    // =====================================
    // Определение модели объекта
    // =====================================


    for (int k = 0; k < int(clastersData.size()); k++)
    {




        // =====================================
        // Прерыватель. Для снятия нагрузки с процессора
        // Определяет только выделенный объект (кластер)
        // =====================================
        if (k!=vuxyzrgb_temp.SelectedClaster)
        {
            vuxyzrgb_temp.PrimitiveDetect[k] = 2;
            vuxyzrgb_temp.PrimitiveParameter[k] = {0, 0, 0, 0, 0};
            vuxyzrgb_temp.PrimitivePoints[k] = {};
            continue;
        }




        //*****************************************   Fit plane   ************************
        vector<Point3d> planePoints;


        vector<double> PlaneModel = ADS::FitPlane(clastersData[k], planePoints);



        //*****************************************   Fit sphere   ************************
        vector<double> SphereModel = FitSphere(clastersData[k]);

        //*****************************************   Fit cylinder   ************************
        vector<Point3d> cylinderPoints;
        vector<double> CylinderModel = FitCylinder(clastersData[k], PlaneModel, cylinderPoints);


    cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 04 "  <<endl;
        // =====================================
        // Определение вида примитива
        // =====================================


        int figureInClaster = 0;
        double minErrorOfModel = 0.0;
        if (PlaneModel[PlaneModel.size() - 1] < SphereModel[SphereModel.size() - 1])
        {
            minErrorOfModel = PlaneModel[PlaneModel.size() - 1];
            figureInClaster = 1;
        }
        else
        {
            minErrorOfModel = SphereModel[SphereModel.size() - 1];
            figureInClaster = 2;
        }
        if (CylinderModel[CylinderModel.size() - 1] < minErrorOfModel) {
            minErrorOfModel = CylinderModel[CylinderModel.size() - 1];
            figureInClaster = 3;
        }

        double l1, l2, lMin;
        l1 = sqrt(pow((planePoints[0].x - planePoints[1].x),2)+pow((planePoints[0].y - planePoints[1].y),2)+pow((planePoints[0].z - planePoints[1].z),2));
        l2 = sqrt(pow((planePoints[0].x - planePoints[2].x),2)+pow((planePoints[0].y - planePoints[2].y),2)+pow((planePoints[0].z - planePoints[2].z),2));
        if (l1 < l2)
            lMin = l1;
        else
            lMin = l2;

        switch (figureInClaster)
        {
        case 0:
            std::cout << "Non" << endl;
            break;
        case 1: // Plane


            vuxyzrgb_temp.PrimitiveDetect[k] = 1;
            vuxyzrgb_temp.PrimitiveParameter[k] = PlaneModel;
            vuxyzrgb_temp.PrimitivePoints[k] = planePoints;

            //            std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Plane: " <<  " [ ";

            //            for (int i = 0; i < int(PlaneModel.size()); i++)
            //            {
            //                std::cout << PlaneModel[i] << " ";
            //            }
            //            std::cout << "]" <<  endl;

            break;
        case 2: // Sphere

            if (SphereModel[3] < lMin) {


                vuxyzrgb_temp.PrimitiveDetect[k] = 2;
                vuxyzrgb_temp.PrimitiveParameter[k] = SphereModel;
                vuxyzrgb_temp.PrimitivePoints[k] = {};


                //                std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Sphere: " <<  " [ ";
                //                for (int i = 0; i < int(SphereModel.size()); i++)
                //                {
                //                    std::cout << SphereModel[i] << " ";
                //                }
                //                std::cout << "]" <<  endl;

            } else {
                vuxyzrgb_temp.PrimitiveDetect[k] = 1;
                vuxyzrgb_temp.PrimitiveParameter[k] = PlaneModel;
                vuxyzrgb_temp.PrimitivePoints[k] = planePoints;

                //                std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Plane: " <<  " [ ";

                //                for (int i = 0; i < int(PlaneModel.size()); i++)
                //                {
                //                    std::cout << PlaneModel[i] << " ";
                //                }
                //                std::cout << "]" <<  endl;
            }

            break;
        case 3:  // Cylinder


            if (CylinderModel[6]<lMin)
            {



                vuxyzrgb_temp.PrimitiveDetect[k] = 3;
                vuxyzrgb_temp.PrimitiveParameter[k] = CylinderModel;
                vuxyzrgb_temp.PrimitivePoints[k] = cylinderPoints;




                //                std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Cylinder:"  << " [ ";
                //                for (int i = 0; i < int (CylinderModel.size()); i++)
                //                {
                //                    std::cout << CylinderModel[i] << " ";
                //                }
                //                std::cout << "]" <<  endl;

            } else {
                vuxyzrgb_temp.PrimitiveDetect[k] = 1;
                vuxyzrgb_temp.PrimitiveParameter[k] = PlaneModel;
                vuxyzrgb_temp.PrimitivePoints[k] = planePoints;

                //                std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Plane: " <<  " [ ";

                //                for (int i = 0; i < int(PlaneModel.size()); i++)
                //                {
                //                    std::cout << PlaneModel[i] << " ";
                //                }
                //                std::cout << "]" <<  endl;
            }


        }

    }


    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitiveDetect.size() " <<  vuxyzrgb_temp.PrimitiveDetect.size()  << endl;
    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitiveParameter.size() " <<  vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitivePoints.size() " <<  vuxyzrgb_temp.PrimitivePoints.size()  << endl;

    //    cout << "\t |||>> (9999) DetectPrimitive_all(): END "  << endl;





    cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 08 "  <<endl;




    //  cout << "\t |||>> (0) AddPrimitiveToIm(): BIGIN "  << endl;


    // =====================================
    // Проверка на пустоту
    // =====================================

    if (vuxyzrgb_temp.PrimitiveDetect.empty()) return;
    if (imgResult01r_temp.empty()) return;
    if (imgLeft01r_temp.empty()) return;
    // =====================================

    // cout << "\t |||>> (1) AddPrimitiveToIm() "  << endl;









    // =====================================
    //
    // =====================================

    Mat projectionXY = Mat::ones(imgSize, CV_8UC3);
    projectionXY = Scalar(255, 255, 255);
    Mat projectionYZ = Mat::ones(imgSize, CV_8UC3);
    projectionYZ = Scalar(255, 255, 255);
    Mat projectionXZ = Mat::ones(imgSize, CV_8UC3);
    projectionXZ = Scalar(255, 255, 255);



    // =====================================
    // Проверка на выбросы
    // =====================================
    double xLimMin = 1e+8;
    double xLimMax = -1e+8;
    double yLimMin = 1e+8;
    double yLimMax = -1e+8;
    double zLimMin = 1e+8;
    double zLimMax = -1e+8;
    for (int i = 0; i < int(xyz.size()); i++) {
        //if (IDX[i] != 0) {
        if (xyz[i].x < xLimMin)
            xLimMin = xyz[i].x;
        if (xyz[i].x > xLimMax)
            xLimMax = xyz[i].x;
        if (xyz[i].y < yLimMin)
            yLimMin = xyz[i].y;
        if (xyz[i].y > yLimMax)
            yLimMax = xyz[i].y;
        if (xyz[i].z < zLimMin)
            zLimMin = xyz[i].z;
        if (xyz[i].z > zLimMax)
            zLimMax = xyz[i].z;
        //}
    }


    //  cout << "\t |||>> (3) AddPrimitiveToIm() "  << endl;


    // =====================================
    // Создание изображения с результатом (прорисовка сетки и др.)
    // =====================================

    double xyScale, yzScale, xzScale, xScale, yScale, zScale;
    xScale = min(abs((w/2-50) / xLimMax), abs((w/2-50) / xLimMin));
    yScale = min(abs((h/2-50) / yLimMax), abs((h/2-50) / yLimMin));
    xyScale = min(xScale, yScale);
    zScale = (w-100) / zLimMax;
    yzScale = min(zScale, yScale);
    xScale = min(abs( (h/2-50) / xLimMax), abs((h/2-50) / xLimMin));
    xzScale = min(xScale, zScale);
    Rect rect(49, 49, (w-100), (h-100));
    rectangle(projectionXY, rect, cv::Scalar(0, 0, 0));
    rectangle(projectionYZ, rect, cv::Scalar(0, 0, 0));
    rectangle(projectionXZ, rect, cv::Scalar(0, 0, 0));
    Point2d p1, p2;
    p1.x = 49; p1.y = ho;
    p2.x = (w-51); p2.y = ho;
    line(projectionXY, p1, p2, Scalar(200, 200, 200));
    p1.x = wo; p1.y = 49;
    p2.x = wo; p2.y = (h-100);
    line(projectionXY, p1, p2, Scalar(200, 200, 200));
    p1.x = 49; p1.y = ho;
    p2.x = (w-100); p2.y = ho;
    line(projectionYZ, p1, p2, Scalar(200, 200, 200));
    line(projectionXZ, p1, p2, Scalar(200, 200, 200));
    char textZ[10];
    Point2d textPoint;
    textPoint.x = 49;
    textPoint.y = 40;
    putText(projectionXY, "Y", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionYZ, "Y", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionXZ, "X", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    textPoint.x = (w-42);
    textPoint.y = (h-51);
    putText(projectionXY, "X", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionYZ, "Z", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionXZ, "Z", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    textPoint.x = 20;
    textPoint.y = (h/2-10);
    putText(projectionXY, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionYZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionXZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    textPoint.x = 40;
    textPoint.y = (h-20);
    putText(projectionYZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionXZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    textPoint.x = (h/2-10);
    textPoint.y = (h-20);
    putText(projectionXY, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));

    // =====================================


    // cout << "\t |||>> (4) AddPrimitiveToIm() "  << endl;

    cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 09 "  <<endl;


    Mat res4;


    // == Правка====
    vector<Point2d> kp1;
    kp1.resize(int( vuxyzrgb_temp.xyz.size() ));

    for (int i = 0; i < int( vuxyzrgb_temp.xyz.size() ); i++)
    {
        kp1.at(i).x = double(vuxyzrgb_temp.vu.at(i).at(1) );
        kp1.at(i).y = double(vuxyzrgb_temp.vu.at(i).at(0) );
    }

    // =====================================

    // =====================================
    // Прорисовка точек
    // =====================================

    int Selector = 1 ;


    if (Selector == 1)

    {
        res4 =  imgResult01r_temp;
    }
    else
    {
        res4 = imgLeft01r_temp.clone();

        for (int i = 0; i < int(clastersData.size()); i++) {
            Mat res3 = imgLeft01r_temp.clone();
            //char clasterFilename[100];
            //sprintf_s(clasterFilename, "Claster%d.jpg", i + 1);
            double xmin = 1000000, xmax = 0, ymin = 1000000, ymax = 0;
            double meanZ = 0.0;
            int countZ = 0;
            int tempIdx = 0;
            for (int j = 0; j < int(IDX.size()); j++)
                if (IDX[j] == (i)) {
                    circle(res3, kp1[j], 3, Scalar(0, 255, 0));
                    circle(res4, kp1[j], 3, Scalar(0, 255, 0));
                    if (kp1[j].x < xmin)
                        xmin = kp1[j].x;
                    if (kp1[j].x > xmax)
                        xmax = kp1[j].x;
                    if (kp1[j].y < ymin)
                        ymin = kp1[j].y;
                    if (kp1[j].y > ymax)
                        ymax = kp1[j].y;
                    meanZ += clastersData[i][tempIdx].z;
                    countZ++;
                    tempIdx++;


                    Point2d pcPoint;
                    pcPoint.x = wo + xyz[j].x * xyScale;
                    pcPoint.y = ho + xyz[j].y * xyScale;
                    circle(projectionXY, pcPoint, 3, Scalar(0, 255, 0));
                    pcPoint.x = 49 + xyz[j].z * yzScale;
                    pcPoint.y = ho + xyz[j].y * yzScale;
                    circle(projectionYZ, pcPoint, 3, Scalar(0, 255, 0));
                    pcPoint.x = 49 + xyz[j].z * xzScale;
                    pcPoint.y = ho + xyz[j].x * xzScale;
                    circle(projectionXZ, pcPoint, 3, Scalar(0, 255, 0));
                }
            meanZ /= double(countZ);
            Point2d lt, rt, lb, rb;
            lt.x = xmin; lt.y = ymin;
            rt.x = xmax; rt.y = ymin;
            lb.x = xmin; lb.y = ymax;
            rb.x = xmax; rb.y = ymax;
            line(res3, lt, rt, Scalar(0, 255, 255), 2);
            line(res3, lt, lb, Scalar(0, 255, 255), 2);
            line(res3, lb, rb, Scalar(0, 255, 255), 2);
            line(res3, rt, rb, Scalar(0, 255, 255), 2);
            line(res4, lt, rt, Scalar(0, 255, 255), 2);
            line(res4, lt, lb, Scalar(0, 255, 255), 2);
            line(res4, lb, rb, Scalar(0, 255, 255), 2);
            line(res4, rt, rb, Scalar(0, 255, 255), 2);
            char textZ[100];
            Point2d textPoint;
            textPoint.x = (xmax + xmin) / 2;
            textPoint.y = ymax + 15;
           // sprintf_s(textZ, "Mean Z, mm: %7.2f", meanZ);
            putText(res3, textZ, textPoint, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
            //putText(res4, textZ, textPoint, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
            //cv::imwrite(clasterFilename, res3);
        }
        //cv::imwrite("AllClasters.jpg", res4);

    }

    //    cout << "\t |||>> (5) AddPrimitiveToIm() "  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitiveDetect.size() " <<  vuxyzrgb_temp.PrimitiveDetect.size()  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitiveParameter.size() " <<  vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitivePoints.size() " <<  vuxyzrgb_temp.PrimitivePoints.size()  << endl;
    //    cout << "\t |||>> (5) clastersData.size():   " << clastersData.size()  << endl;
    //    cout << "\t |||>> (5) vuxyzrgb_temp.num_cl:   " << vuxyzrgb_temp.num_cl  << endl;




    cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 10 "  <<endl;



    for (int k = 0; k < int(clastersData.size()); k++)
    {


        // условие для прористовки примитивов только выбранного кластера
        if (vuxyzrgb_temp.SelectedClaster > -1)
            if (!(vuxyzrgb_temp.SelectedClaster == k)) continue;




        vector<Point3d> planePoints;
        vector<Point3d> cylinderPoints;
        vector<double> PlaneModel;
        vector<double> SphereModel;
        vector<double> CylinderModel;


        Point2d pcPoint;


        //cout << "\t |||>> (6) AddPrimitiveToIm() k =  " << k  << "  vuxyzrgb_temp.PrimitiveDetect[k]: " << vuxyzrgb_temp.PrimitiveDetect[k]  << endl;


        switch (vuxyzrgb_temp.PrimitiveDetect[k])
        {
        case 1: // ========== Fit plane  =========



            PlaneModel = vuxyzrgb_temp.PrimitiveParameter[k];
            planePoints = vuxyzrgb_temp.PrimitivePoints[k];

            for (int i = 0; i < 6; i++) {
                p1.x = wo + planePoints[0].x * xyScale + 0.2 * i * (planePoints[1].x * xyScale - planePoints[0].x * xyScale);
                p1.y = ho + planePoints[0].y * xyScale + 0.2 * i * (planePoints[1].y * xyScale - planePoints[0].y * xyScale);
                p2.x = wo + planePoints[2].x * xyScale + 0.2 * i * (planePoints[3].x * xyScale - planePoints[2].x * xyScale);
                p2.y = ho + planePoints[2].y * xyScale + 0.2 * i * (planePoints[3].y * xyScale - planePoints[2].y * xyScale);
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                p1.x = wo + planePoints[0].x * xyScale + 0.2 * i * (planePoints[2].x * xyScale - planePoints[0].x * xyScale);
                p1.y = ho + planePoints[0].y * xyScale + 0.2 * i * (planePoints[2].y * xyScale - planePoints[0].y * xyScale);
                p2.x = wo + planePoints[1].x * xyScale + 0.2 * i * (planePoints[3].x * xyScale - planePoints[1].x * xyScale);
                p2.y = ho + planePoints[1].y * xyScale + 0.2 * i * (planePoints[3].y * xyScale - planePoints[1].y * xyScale);
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
            }
            for (int i = 0; i < 6; i++) {
                p1.x = 49 + planePoints[0].z * yzScale + 0.2 * i * (planePoints[1].z * yzScale - planePoints[0].z * yzScale);
                p1.y = ho + planePoints[0].y * yzScale + 0.2 * i * (planePoints[1].y * yzScale - planePoints[0].y * yzScale);
                p2.x = 49 + planePoints[2].z * yzScale + 0.2 * i * (planePoints[3].z * yzScale - planePoints[2].z * yzScale);
                p2.y = ho + planePoints[2].y * yzScale + 0.2 * i * (planePoints[3].y * yzScale - planePoints[2].y * yzScale);
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                p1.x = 49 + planePoints[0].z * yzScale + 0.2 * i * (planePoints[2].z * yzScale - planePoints[0].z * yzScale);
                p1.y = ho + planePoints[0].y * yzScale + 0.2 * i * (planePoints[2].y * yzScale - planePoints[0].y * yzScale);
                p2.x = 49 + planePoints[1].z * yzScale + 0.2 * i * (planePoints[3].z * yzScale - planePoints[1].z * yzScale);
                p2.y = ho + planePoints[1].y * yzScale + 0.2 * i * (planePoints[3].y * yzScale - planePoints[1].y * yzScale);
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
            }
            for (int i = 0; i < 6; i++) {
                p1.x = 49 + planePoints[0].z * xzScale + 0.2 * i * (planePoints[1].z * xzScale - planePoints[0].z * xzScale);
                p1.y = ho + planePoints[0].x * xzScale + 0.2 * i * (planePoints[1].x * xzScale - planePoints[0].x * xzScale);
                p2.x = 49 + planePoints[2].z * xzScale + 0.2 * i * (planePoints[3].z * xzScale - planePoints[2].z * xzScale);
                p2.y = ho + planePoints[2].x * xzScale + 0.2 * i * (planePoints[3].x * xzScale - planePoints[2].x * xzScale);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                p1.x = 49 + planePoints[0].z * xzScale + 0.2 * i * (planePoints[2].z * xzScale - planePoints[0].z * xzScale);
                p1.y = ho + planePoints[0].x * xzScale + 0.2 * i * (planePoints[2].x * xzScale - planePoints[0].x * xzScale);
                p2.x = 49 + planePoints[1].z * xzScale + 0.2 * i * (planePoints[3].z * xzScale - planePoints[1].z * xzScale);
                p2.y = ho + planePoints[1].x * xzScale + 0.2 * i * (planePoints[3].x * xzScale - planePoints[1].x * xzScale);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
            }

            break;


        case 2:  // ======== Fit sphere==========

            SphereModel = vuxyzrgb_temp.PrimitiveParameter[k];

            figureModel.push_back(SphereModel);
            pcPoint.x = wo + SphereModel[0] * xyScale;
            pcPoint.y = ho + SphereModel[1] * xyScale;
            circle(projectionXY, pcPoint, SphereModel[3] * xyScale, Scalar(50, 50, 50));
            double xLeft, xRight, yTop, yLeft, yRigth;
            yTop = pcPoint.y - SphereModel[3] * xyScale;
            for (int i = 0; i < 4; i++) {
                xLeft = pcPoint.x - sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * i * SphereModel[3] * xyScale, 2));
                xRight = pcPoint.x + sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * i * SphereModel[3] * xyScale, 2));
                yLeft = yRigth = yTop + 0.5 * i * SphereModel[3] * xyScale;
                p1.x = xLeft; p1.y = yLeft;
                p2.x = xRight; p2.y = yRigth;
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                for (int j = 0; j < 3; j++) {
                    p1.x = xLeft + (xRight - xLeft) * 0.25 * (j + 1);
                    p1.y = yLeft;
                    p2.x = pcPoint.x - sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * (i + 1) * SphereModel[3] * xyScale, 2)) + 2 * sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * (i + 1) * SphereModel[3] * xyScale, 2)) * 0.25 * (j + 1);
                    p2.y = yTop + 0.5 * (i + 1) * SphereModel[3] * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                }
            }

            pcPoint.x = 49 + SphereModel[2] * yzScale;
            pcPoint.y = ho + SphereModel[1] * yzScale;
            circle(projectionYZ, pcPoint, SphereModel[3] * yzScale, Scalar(50, 50, 50));
            yTop = pcPoint.y - SphereModel[3] * yzScale;
            for (int i = 0; i < 4; i++) {
                xLeft = pcPoint.x - sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * i * SphereModel[3] * yzScale, 2));
                xRight = pcPoint.x + sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * i * SphereModel[3] * yzScale, 2));
                yLeft = yRigth = yTop + 0.5 * i * SphereModel[3] * yzScale;
                p1.x = xLeft; p1.y = yLeft;
                p2.x = xRight; p2.y = yRigth;
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                for (int j = 0; j < 3; j++) {
                    p1.x = xLeft + (xRight - xLeft) * 0.25 * (j + 1);
                    p1.y = yLeft;
                    p2.x = pcPoint.x - sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * (i + 1) * SphereModel[3] * yzScale, 2)) + 2 * sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * (i + 1) * SphereModel[3] * yzScale, 2)) * 0.25 * (j + 1);
                    p2.y = yTop + 0.5 * (i + 1) * SphereModel[3] * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                }
            }

            pcPoint.x = 49 + SphereModel[2] * xzScale;
            pcPoint.y = ho + SphereModel[0] * xzScale;
            circle(projectionXZ, pcPoint, SphereModel[3] * xzScale, Scalar(50, 50, 50));
            circle(projectionXZ, pcPoint, 0.5 * SphereModel[3] * xzScale, Scalar(50, 50, 50));
            for (int i = 0; i < 4; i++) {
                p1.x = pcPoint.x + SphereModel[3] * xzScale * cos(i * CV_PI / 4); p1.y = pcPoint.y + SphereModel[3] * xzScale * sin(i * CV_PI / 4);
                p2.x = pcPoint.x + SphereModel[3] * xzScale * cos(i * CV_PI / 4 + CV_PI); p2.y = pcPoint.y + SphereModel[3] * xzScale * sin(i * CV_PI / 4 + CV_PI);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
            }
            break;


        case 3:  // ========== Fit cylinder =============


            CylinderModel = vuxyzrgb_temp.PrimitiveParameter[k];
            cylinderPoints = vuxyzrgb_temp.PrimitivePoints[k];

            figureModel.push_back(CylinderModel);
            int nearCenter, farCenter;
            if (CylinderModel[2] < CylinderModel[5]) {
                nearCenter = 0;
                farCenter = 1;
            }
            else {
                nearCenter = 1;
                farCenter = 0;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = wo + cylinderPoints[2 * i + nearCenter].x * xyScale; p1.y = ho + cylinderPoints[2 * i + nearCenter].y * xyScale;
                p2.x = wo + cylinderPoints[2 * (i + 1) + nearCenter].x * xyScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].y * xyScale;
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].z < CylinderModel[2 + 3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].z < CylinderModel[2 + 3 * farCenter])) {
                    p1.x = wo + cylinderPoints[2 * i + farCenter].x * xyScale; p1.y = ho + cylinderPoints[2 * i + farCenter].y * xyScale;
                    p2.x = wo + cylinderPoints[2 * (i + 1) + farCenter].x * xyScale; p2.y = ho + cylinderPoints[2 * (i + 1) + farCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                    p1.x = wo + cylinderPoints[2 * i + farCenter].x * xyScale; p1.y = ho + cylinderPoints[2 * i + farCenter].y * xyScale;
                    p2.x = wo + cylinderPoints[2 * i + nearCenter].x * xyScale; p2.y = ho + cylinderPoints[2 * i + nearCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                    p1.x = wo + cylinderPoints[2 * (i + 1) + farCenter].x * xyScale; p1.y = ho + cylinderPoints[2 * (i + 1) + farCenter].y * xyScale;
                    p2.x = wo + cylinderPoints[2 * (i + 1) + nearCenter].x * xyScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                }
            }
            if (CylinderModel[0] < CylinderModel[3]) {
                nearCenter = 1;
                farCenter = 0;
            }
            else {
                nearCenter = 0;
                farCenter = 1;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 49 + cylinderPoints[2 * i + nearCenter].z * yzScale; p1.y = ho + cylinderPoints[2 * i + nearCenter].y * yzScale;
                p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * yzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].y * yzScale;
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].x > CylinderModel[3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].x > CylinderModel[3 * farCenter])) {
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * yzScale; p1.y = ho + cylinderPoints[2 * i + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * yzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + farCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * yzScale; p1.y = ho + cylinderPoints[2 * i + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * i + nearCenter].z * yzScale; p2.y = ho + cylinderPoints[2 * i + nearCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * yzScale; p1.y = ho + cylinderPoints[2 * (i + 1) + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * yzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                }
            }
            if (CylinderModel[1] < CylinderModel[4]) {
                nearCenter = 0;
                farCenter = 1;
            }
            else {
                nearCenter = 1;
                farCenter = 0;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 49 + cylinderPoints[2 * i + nearCenter].z * xzScale; p1.y = ho + cylinderPoints[2 * i + nearCenter].x * xzScale;
                p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * xzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].x * xzScale;
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].y < CylinderModel[1 + 3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].y < CylinderModel[1 + 3 * farCenter])) {
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * xzScale; p1.y = ho + cylinderPoints[2 * i + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * xzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + farCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * xzScale; p1.y = ho + cylinderPoints[2 * i + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * i + nearCenter].z * xzScale; p2.y = ho + cylinderPoints[2 * i + nearCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * xzScale; p1.y = ho + cylinderPoints[2 * (i + 1) + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * xzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                }
            }
            break;
        }

    }


    //    imwrite("projectionXY.jpg", projectionXY);
    //    imwrite("projectionYZ.jpg", projectionYZ);
    //    imwrite("projectionXZ.jpg", projectionXZ);



        cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 11 "  <<endl;


    Mat imgPrimitivesXY_temp = projectionXY;
    Mat imgPrimitivesYZ_temp = projectionYZ;
    Mat imgPrimitivesXZ_temp = projectionXZ;


    resize(projectionXY, projectionXY, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(projectionYZ, projectionYZ, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(projectionXZ, projectionXZ, Size(), 0.5, 0.5, INTER_LINEAR);








    Rect r1(0,   0,   w/2, h/2);
    Rect r2(w/2, 0,   w/2, h/2);
    Rect r3(0,   h/2, w/2, h/2);
    Rect r4(w/2, h/2, w/2, h/2);
    resize(res4, res4, Size(), 0.5, 0.5, INTER_LINEAR);
    res4.copyTo(finalImage(r1));
    projectionXY.copyTo(finalImage(r2));
    projectionYZ.copyTo(finalImage(r3));
    projectionXZ.copyTo(finalImage(r4));
    p1.x = 0; p1.y = (h/2)-0.5;
    p2.x = (w-1); p2.y = (h/2)-0.5;
    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    p1.x = (w/2)-0.5; p1.y = 0;
    p2.x = (w/2)-0.5; p2.y = (h-1);
    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    Rect rectF(0, 0, w, h);
    rectangle(finalImage, rectF, cv::Scalar(0, 0, 0), 2);







//    Rect r1(0, 0, 960, 600);
//    Rect r2(960, 0, 960, 600);
//    Rect r3(0, 600, 960, 600);
//    Rect r4(960, 600, 960, 600);
//    resize(res4, res4, Size(), 0.5, 0.5, INTER_LINEAR);
//    res4.copyTo(finalImage(r1));
//    projectionXY.copyTo(finalImage(r2));
//    projectionYZ.copyTo(finalImage(r3));
//    projectionXZ.copyTo(finalImage(r4));
//    p1.x = 0; p1.y = ho;
//    p2.x = 1919; p2.y = ho;
//    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
//    p1.x = wo; p1.y = 0;
//    p2.x = wo; p2.y = 1199;
//    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
//    Rect rectF(0, 0, 1920, 1200);
//    rectangle(finalImage, rectF, cv::Scalar(0, 0, 0), 2);





    //cv::imwrite("FinalImage.jpg", finalImage);
    // cout << "\t |||>> (9999) AddPrimitiveToIm(): END "  << endl;

        cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 12 "  <<endl;



    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    // ============


    vuxyzrgb.num_prim = vuxyzrgb_temp.PrimitiveDetect.size();
    vuxyzrgb.PrimitiveDetect = vuxyzrgb_temp.PrimitiveDetect;
    vuxyzrgb.PrimitiveParameter = vuxyzrgb_temp.PrimitiveParameter;
    vuxyzrgb.PrimitivePoints = vuxyzrgb_temp.PrimitivePoints;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============




    // ===========
    // Mutex (lock)
    imgPrimitives_mutex.lock();
    // ============



    imgPrimitivesXY = imgPrimitivesXY_temp;
    imgPrimitivesYZ = imgPrimitivesYZ_temp;
    imgPrimitivesXZ = imgPrimitivesXZ_temp;
    imgPrimitive01r_general = finalImage.clone();


    // ===========
    // Mutex (unlock)
    imgPrimitives_mutex.unlock();
    // ============





        cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 13 "  <<endl;







}





// ============
// Определение приметивов для контуров
// ============

void ADS::cl_DenseStereo::DetectAddPrimitiveForContour()
{

    cv::Mat imgLeft01r_temp; // Инициализация изображения для вывода результатов
    cv::Mat imgResult01r_temp;
    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    //type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура


    imgLeft01r_temp = getImgOpenCV_1left();
    imgResult01r_temp = getImgFindColorObj();




    vuxyzrgb_temp = get_vuxyzrgbColorObj();
    //vuxyzrgb_arma_temp = vuxyzrgb_arma;





    // ============
    // Если точек нет то выход
    // ============
    if (vuxyzrgb_temp.num_p < 20) return;


    // Вручную принудительно назначение выбранного кластера
    vuxyzrgb_temp.SelectedClaster = 0;


    cv::Size imgSize = imgResult01r_temp.size();
    cout << "\t\t\t\t >>>>>>>>>>>>>>>>>>>>imgSize = " << imgSize <<endl;


    int h = imgSize.height;
    int w = imgSize.width;

    int ho = (h/2)-0.5;
    int wo = (w/2)-0.5;

    //    p2.y = 599.5;
    //    p2.x = 959.5;



    //if (vuxyzrgb_temp.num_cl < vuxyzrgb_temp.SelectedClaster) vuxyzrgb_temp.SelectedClaster = vuxyzrgb_temp.num_cl;


  //  cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 01 "  <<endl;


    // cout << "\t |||>> (0) DetectPrimitive_all(): BIGIN "  << endl;

    vector<vector<double>> figureModel;
    Mat finalImage = Mat::zeros(imgSize, CV_8UC3);
    vector<vector<Point3d>> clastersData;




    vector<Point3d> xyz;
    xyz.resize( vuxyzrgb_temp.num_p);




    // Используемые переменные
    vector<int> IDX = vuxyzrgb_temp.cluster;;
    int numClusters= vuxyzrgb_temp.num_cl;;




    // Пересчет размеров массивов для хранения данных о приметивах
    vuxyzrgb_temp.PrimitiveDetect.resize(vuxyzrgb_temp.num_cl);
    vuxyzrgb_temp.PrimitiveParameter.resize(vuxyzrgb_temp.num_cl);
    vuxyzrgb_temp.PrimitivePoints.resize(vuxyzrgb_temp.num_cl);


    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveDetect.size() " << vuxyzrgb_temp.PrimitiveDetect.size()  << endl;

    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveParameter.size() " << vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>> vuxyzrgb_temp.PrimitiveParameter.size() " << vuxyzrgb_temp.PrimitiveParameter.size()  << endl;

    //    cout << "\t |||>> vuxyzrgb_temp.PrimitivePoints.size() " << vuxyzrgb_temp.PrimitivePoints.size()  << endl;


  //  cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 02 "  <<endl;


    // == Правка====

    for (int i = 0; i < vuxyzrgb_temp.num_p ; i++)
    {
        xyz.at(i).x = vuxyzrgb_temp.xyz.at(i).at(0);
        xyz.at(i).y = vuxyzrgb_temp.xyz.at(i).at(1);
        xyz.at(i).z = vuxyzrgb_temp.xyz.at(i).at(2);
    }




    for (int j = 0; j < numClusters; j++)
    {
        vector<Point3d> xyz2;
        for (int i = 0; i < int(IDX.size()); i++)
            if (IDX[i] == (j ))
            {
                xyz2.push_back(xyz[i]);
            }
        clastersData.push_back(xyz2);
    }



    //    for (int k = 0; k < vuxyzrgb_temp.num_cl; k++)
    //    {
    //        cout << "\t ++>>> DetectPrimitive_all(): clastersData["<< k
    //             <<"].size(): " << clastersData[k].size()<< endl;

    //    }




   // cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 03 "  <<endl;


    // =====================================
    // Определение модели объекта
    // =====================================


    for (int k = 0; k < int(clastersData.size()); k++)
    {




        // =====================================
        // Прерыватель. Для снятия нагрузки с процессора
        // Определяет только выделенный объект (кластер)
        // =====================================
//        if (k!=vuxyzrgb_temp.SelectedClaster)
//        {
//            vuxyzrgb_temp.PrimitiveDetect[k] = 2;
//            vuxyzrgb_temp.PrimitiveParameter[k] = {0, 0, 0, 0, 0};
//            vuxyzrgb_temp.PrimitivePoints[k] = {};
//            continue;
//        }




        //*****************************************   Fit plane   ************************
        vector<Point3d> planePoints;


        vector<double> PlaneModel = ADS::FitPlane(clastersData[k], planePoints);



        //*****************************************   Fit sphere   ************************
        vector<double> SphereModel = FitSphere(clastersData[k]);

        //*****************************************   Fit cylinder   ************************
        vector<Point3d> cylinderPoints;
        vector<double> CylinderModel = FitCylinder(clastersData[k], PlaneModel, cylinderPoints);


       // cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 04 "  <<endl;
        // =====================================
        // Определение вида примитива
        // =====================================


        int figureInClaster = 0;
        double minErrorOfModel = 0.0;
        if (PlaneModel[PlaneModel.size() - 1] < SphereModel[SphereModel.size() - 1])
        {
            minErrorOfModel = PlaneModel[PlaneModel.size() - 1];
            figureInClaster = 1;
        }
        else
        {
            minErrorOfModel = SphereModel[SphereModel.size() - 1];
            figureInClaster = 2;
        }
        if (CylinderModel[CylinderModel.size() - 1] < minErrorOfModel) {
            minErrorOfModel = CylinderModel[CylinderModel.size() - 1];
            figureInClaster = 3;
        }

        double l1, l2, lMin;
        l1 = sqrt(pow((planePoints[0].x - planePoints[1].x),2)+pow((planePoints[0].y - planePoints[1].y),2)+pow((planePoints[0].z - planePoints[1].z),2));
        l2 = sqrt(pow((planePoints[0].x - planePoints[2].x),2)+pow((planePoints[0].y - planePoints[2].y),2)+pow((planePoints[0].z - planePoints[2].z),2));
        if (l1 < l2)
            lMin = l1;
        else
            lMin = l2;

        switch (figureInClaster)
        {
        case 0:
            std::cout << "Non" << endl;
            break;
        case 1: // Plane


            vuxyzrgb_temp.PrimitiveDetect[k] = 1;
            vuxyzrgb_temp.PrimitiveParameter[k] = PlaneModel;
            vuxyzrgb_temp.PrimitivePoints[k] = planePoints;

            //            std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Plane: " <<  " [ ";

            //            for (int i = 0; i < int(PlaneModel.size()); i++)
            //            {
            //                std::cout << PlaneModel[i] << " ";
            //            }
            //            std::cout << "]" <<  endl;

            break;
        case 2: // Sphere

            if (SphereModel[3] < lMin) {


                vuxyzrgb_temp.PrimitiveDetect[k] = 2;
                vuxyzrgb_temp.PrimitiveParameter[k] = SphereModel;
                vuxyzrgb_temp.PrimitivePoints[k] = {};


                //                std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Sphere: " <<  " [ ";
                //                for (int i = 0; i < int(SphereModel.size()); i++)
                //                {
                //                    std::cout << SphereModel[i] << " ";
                //                }
                //                std::cout << "]" <<  endl;

            } else {
                vuxyzrgb_temp.PrimitiveDetect[k] = 1;
                vuxyzrgb_temp.PrimitiveParameter[k] = PlaneModel;
                vuxyzrgb_temp.PrimitivePoints[k] = planePoints;

                //                std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Plane: " <<  " [ ";

                //                for (int i = 0; i < int(PlaneModel.size()); i++)
                //                {
                //                    std::cout << PlaneModel[i] << " ";
                //                }
                //                std::cout << "]" <<  endl;
            }

            break;
        case 3:  // Cylinder


            if (CylinderModel[6]<lMin)
            {



                vuxyzrgb_temp.PrimitiveDetect[k] = 3;
                vuxyzrgb_temp.PrimitiveParameter[k] = CylinderModel;
                vuxyzrgb_temp.PrimitivePoints[k] = cylinderPoints;




                //                std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Cylinder:"  << " [ ";
                //                for (int i = 0; i < int (CylinderModel.size()); i++)
                //                {
                //                    std::cout << CylinderModel[i] << " ";
                //                }
                //                std::cout << "]" <<  endl;

            } else {
                vuxyzrgb_temp.PrimitiveDetect[k] = 1;
                vuxyzrgb_temp.PrimitiveParameter[k] = PlaneModel;
                vuxyzrgb_temp.PrimitivePoints[k] = planePoints;

                //                std::cout  << "\t ++++>>> Primitive: Claster" << k  << ", Primitive: Plane: " <<  " [ ";

                //                for (int i = 0; i < int(PlaneModel.size()); i++)
                //                {
                //                    std::cout << PlaneModel[i] << " ";
                //                }
                //                std::cout << "]" <<  endl;
            }


        }

    }


    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitiveDetect.size() " <<  vuxyzrgb_temp.PrimitiveDetect.size()  << endl;
    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitiveParameter.size() " <<  vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>>  vuxyzrgb_temp.PrimitivePoints.size() " <<  vuxyzrgb_temp.PrimitivePoints.size()  << endl;

    //    cout << "\t |||>> (9999) DetectPrimitive_all(): END "  << endl;





   // cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 08 "  <<endl;




    //  cout << "\t |||>> (0) AddPrimitiveToIm(): BIGIN "  << endl;


    // =====================================
    // Проверка на пустоту
    // =====================================

    if (vuxyzrgb_temp.PrimitiveDetect.empty()) return;
    if (imgResult01r_temp.empty()) return;
    if (imgLeft01r_temp.empty()) return;
    // =====================================

    // cout << "\t |||>> (1) AddPrimitiveToIm() "  << endl;









    // =====================================
    //
    // =====================================

    Mat projectionXY = Mat::ones(imgSize, CV_8UC3);
    projectionXY = Scalar(255, 255, 255);
    Mat projectionYZ = Mat::ones(imgSize, CV_8UC3);
    projectionYZ = Scalar(255, 255, 255);
    Mat projectionXZ = Mat::ones(imgSize, CV_8UC3);
    projectionXZ = Scalar(255, 255, 255);



    // =====================================
    // Проверка на выбросы
    // =====================================
    double xLimMin = 1e+8;
    double xLimMax = -1e+8;
    double yLimMin = 1e+8;
    double yLimMax = -1e+8;
    double zLimMin = 1e+8;
    double zLimMax = -1e+8;
    for (int i = 0; i < int(xyz.size()); i++) {
        //if (IDX[i] != 0) {
        if (xyz[i].x < xLimMin)
            xLimMin = xyz[i].x;
        if (xyz[i].x > xLimMax)
            xLimMax = xyz[i].x;
        if (xyz[i].y < yLimMin)
            yLimMin = xyz[i].y;
        if (xyz[i].y > yLimMax)
            yLimMax = xyz[i].y;
        if (xyz[i].z < zLimMin)
            zLimMin = xyz[i].z;
        if (xyz[i].z > zLimMax)
            zLimMax = xyz[i].z;
        //}
    }


    //  cout << "\t |||>> (3) AddPrimitiveToIm() "  << endl;


    // =====================================
    // Создание изображения с результатом (прорисовка сетки и др.)
    // =====================================

    double xyScale, yzScale, xzScale, xScale, yScale, zScale;
    xScale = min(abs((w/2-50) / xLimMax), abs((w/2-50) / xLimMin));
    yScale = min(abs((h/2-50) / yLimMax), abs((h/2-50) / yLimMin));
    xyScale = min(xScale, yScale);
    zScale = (w-100) / zLimMax;
    yzScale = min(zScale, yScale);
    xScale = min(abs( (h/2-50) / xLimMax), abs((h/2-50) / xLimMin));
    xzScale = min(xScale, zScale);
    Rect rect(49, 49, (w-100), (h-100));
    rectangle(projectionXY, rect, cv::Scalar(0, 0, 0));
    rectangle(projectionYZ, rect, cv::Scalar(0, 0, 0));
    rectangle(projectionXZ, rect, cv::Scalar(0, 0, 0));
    Point2d p1, p2;
    p1.x = 49; p1.y = ho;
    p2.x = (w-51); p2.y = ho;
    line(projectionXY, p1, p2, Scalar(200, 200, 200));
    p1.x = wo; p1.y = 49;
    p2.x = wo; p2.y = (h-100);
    line(projectionXY, p1, p2, Scalar(200, 200, 200));
    p1.x = 49; p1.y = ho;
    p2.x = (w-100); p2.y = ho;
    line(projectionYZ, p1, p2, Scalar(200, 200, 200));
    line(projectionXZ, p1, p2, Scalar(200, 200, 200));
    char textZ[10];
    Point2d textPoint;
    textPoint.x = 49;
    textPoint.y = 40;
    putText(projectionXY, "Y", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionYZ, "Y", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionXZ, "X", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    textPoint.x = (w-42);
    textPoint.y = (h-51);
    putText(projectionXY, "X", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionYZ, "Z", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    putText(projectionXZ, "Z", textPoint, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 2);
    textPoint.x = 20;
    textPoint.y = (h/2-10);
    putText(projectionXY, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionYZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionXZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    textPoint.x = 40;
    textPoint.y = (h-20);
    putText(projectionYZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    putText(projectionXZ, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
    textPoint.x = (h/2-10);
    textPoint.y = (h-20);
    putText(projectionXY, "0", textPoint, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));

    // =====================================


    // cout << "\t |||>> (4) AddPrimitiveToIm() "  << endl;

  //  cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 09 "  <<endl;


    Mat res4;


    // == Правка====
    vector<Point2d> kp1;
    kp1.resize(int( vuxyzrgb_temp.xyz.size() ));

    for (int i = 0; i < int( vuxyzrgb_temp.xyz.size() ); i++)
    {
        kp1.at(i).x = double(vuxyzrgb_temp.vu.at(i).at(1) );
        kp1.at(i).y = double(vuxyzrgb_temp.vu.at(i).at(0) );
    }

    // =====================================

    // =====================================
    // Прорисовка точек
    // =====================================

    int Selector = 1 ;


    if (Selector == 1)

    {
        res4 =  imgResult01r_temp;
    }
    else
    {
        res4 = imgLeft01r_temp.clone();

        for (int i = 0; i < int(clastersData.size()); i++) {
            Mat res3 = imgLeft01r_temp.clone();
            //char clasterFilename[100];
            //sprintf_s(clasterFilename, "Claster%d.jpg", i + 1);
            double xmin = 1000000, xmax = 0, ymin = 1000000, ymax = 0;
            double meanZ = 0.0;
            int countZ = 0;
            int tempIdx = 0;
            for (int j = 0; j < int(IDX.size()); j++)
                if (IDX[j] == (i)) {
                    circle(res3, kp1[j], 3, Scalar(0, 255, 0));
                    circle(res4, kp1[j], 3, Scalar(0, 255, 0));
                    if (kp1[j].x < xmin)
                        xmin = kp1[j].x;
                    if (kp1[j].x > xmax)
                        xmax = kp1[j].x;
                    if (kp1[j].y < ymin)
                        ymin = kp1[j].y;
                    if (kp1[j].y > ymax)
                        ymax = kp1[j].y;
                    meanZ += clastersData[i][tempIdx].z;
                    countZ++;
                    tempIdx++;


                    Point2d pcPoint;
                    pcPoint.x = wo + xyz[j].x * xyScale;
                    pcPoint.y = ho + xyz[j].y * xyScale;
                    circle(projectionXY, pcPoint, 3, Scalar(0, 255, 0));
                    pcPoint.x = 49 + xyz[j].z * yzScale;
                    pcPoint.y = ho + xyz[j].y * yzScale;
                    circle(projectionYZ, pcPoint, 3, Scalar(0, 255, 0));
                    pcPoint.x = 49 + xyz[j].z * xzScale;
                    pcPoint.y = ho + xyz[j].x * xzScale;
                    circle(projectionXZ, pcPoint, 3, Scalar(0, 255, 0));
                }
            meanZ /= double(countZ);
            Point2d lt, rt, lb, rb;
            lt.x = xmin; lt.y = ymin;
            rt.x = xmax; rt.y = ymin;
            lb.x = xmin; lb.y = ymax;
            rb.x = xmax; rb.y = ymax;
            line(res3, lt, rt, Scalar(0, 255, 255), 2);
            line(res3, lt, lb, Scalar(0, 255, 255), 2);
            line(res3, lb, rb, Scalar(0, 255, 255), 2);
            line(res3, rt, rb, Scalar(0, 255, 255), 2);
            line(res4, lt, rt, Scalar(0, 255, 255), 2);
            line(res4, lt, lb, Scalar(0, 255, 255), 2);
            line(res4, lb, rb, Scalar(0, 255, 255), 2);
            line(res4, rt, rb, Scalar(0, 255, 255), 2);
            char textZ[100];
            Point2d textPoint;
            textPoint.x = (xmax + xmin) / 2;
            textPoint.y = ymax + 15;
            // sprintf_s(textZ, "Mean Z, mm: %7.2f", meanZ);
            putText(res3, textZ, textPoint, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
            //putText(res4, textZ, textPoint, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
            //cv::imwrite(clasterFilename, res3);
        }
        //cv::imwrite("AllClasters.jpg", res4);

    }

    //    cout << "\t |||>> (5) AddPrimitiveToIm() "  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitiveDetect.size() " <<  vuxyzrgb_temp.PrimitiveDetect.size()  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitiveParameter.size() " <<  vuxyzrgb_temp.PrimitiveParameter.size()  << endl;
    //    cout << "\t |||>> (5)  vuxyzrgb_temp.PrimitivePoints.size() " <<  vuxyzrgb_temp.PrimitivePoints.size()  << endl;
    //    cout << "\t |||>> (5) clastersData.size():   " << clastersData.size()  << endl;
    //    cout << "\t |||>> (5) vuxyzrgb_temp.num_cl:   " << vuxyzrgb_temp.num_cl  << endl;




   // cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 10 "  <<endl;



    for (int k = 0; k < int(clastersData.size()); k++)
    {


        // условие для прористовки примитивов только выбранного кластера
        if (vuxyzrgb_temp.SelectedClaster > -1)
            if (!(vuxyzrgb_temp.SelectedClaster == k)) continue;




        vector<Point3d> planePoints;
        vector<Point3d> cylinderPoints;
        vector<double> PlaneModel;
        vector<double> SphereModel;
        vector<double> CylinderModel;


        Point2d pcPoint;


        //cout << "\t |||>> (6) AddPrimitiveToIm() k =  " << k  << "  vuxyzrgb_temp.PrimitiveDetect[k]: " << vuxyzrgb_temp.PrimitiveDetect[k]  << endl;


        switch (vuxyzrgb_temp.PrimitiveDetect[k])
        {
        case 1: // ========== Fit plane  =========



            PlaneModel = vuxyzrgb_temp.PrimitiveParameter[k];
            planePoints = vuxyzrgb_temp.PrimitivePoints[k];

            for (int i = 0; i < 6; i++) {
                p1.x = wo + planePoints[0].x * xyScale + 0.2 * i * (planePoints[1].x * xyScale - planePoints[0].x * xyScale);
                p1.y = ho + planePoints[0].y * xyScale + 0.2 * i * (planePoints[1].y * xyScale - planePoints[0].y * xyScale);
                p2.x = wo + planePoints[2].x * xyScale + 0.2 * i * (planePoints[3].x * xyScale - planePoints[2].x * xyScale);
                p2.y = ho + planePoints[2].y * xyScale + 0.2 * i * (planePoints[3].y * xyScale - planePoints[2].y * xyScale);
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                p1.x = wo + planePoints[0].x * xyScale + 0.2 * i * (planePoints[2].x * xyScale - planePoints[0].x * xyScale);
                p1.y = ho + planePoints[0].y * xyScale + 0.2 * i * (planePoints[2].y * xyScale - planePoints[0].y * xyScale);
                p2.x = wo + planePoints[1].x * xyScale + 0.2 * i * (planePoints[3].x * xyScale - planePoints[1].x * xyScale);
                p2.y = ho + planePoints[1].y * xyScale + 0.2 * i * (planePoints[3].y * xyScale - planePoints[1].y * xyScale);
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
            }
            for (int i = 0; i < 6; i++) {
                p1.x = 49 + planePoints[0].z * yzScale + 0.2 * i * (planePoints[1].z * yzScale - planePoints[0].z * yzScale);
                p1.y = ho + planePoints[0].y * yzScale + 0.2 * i * (planePoints[1].y * yzScale - planePoints[0].y * yzScale);
                p2.x = 49 + planePoints[2].z * yzScale + 0.2 * i * (planePoints[3].z * yzScale - planePoints[2].z * yzScale);
                p2.y = ho + planePoints[2].y * yzScale + 0.2 * i * (planePoints[3].y * yzScale - planePoints[2].y * yzScale);
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                p1.x = 49 + planePoints[0].z * yzScale + 0.2 * i * (planePoints[2].z * yzScale - planePoints[0].z * yzScale);
                p1.y = ho + planePoints[0].y * yzScale + 0.2 * i * (planePoints[2].y * yzScale - planePoints[0].y * yzScale);
                p2.x = 49 + planePoints[1].z * yzScale + 0.2 * i * (planePoints[3].z * yzScale - planePoints[1].z * yzScale);
                p2.y = ho + planePoints[1].y * yzScale + 0.2 * i * (planePoints[3].y * yzScale - planePoints[1].y * yzScale);
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
            }
            for (int i = 0; i < 6; i++) {
                p1.x = 49 + planePoints[0].z * xzScale + 0.2 * i * (planePoints[1].z * xzScale - planePoints[0].z * xzScale);
                p1.y = ho + planePoints[0].x * xzScale + 0.2 * i * (planePoints[1].x * xzScale - planePoints[0].x * xzScale);
                p2.x = 49 + planePoints[2].z * xzScale + 0.2 * i * (planePoints[3].z * xzScale - planePoints[2].z * xzScale);
                p2.y = ho + planePoints[2].x * xzScale + 0.2 * i * (planePoints[3].x * xzScale - planePoints[2].x * xzScale);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                p1.x = 49 + planePoints[0].z * xzScale + 0.2 * i * (planePoints[2].z * xzScale - planePoints[0].z * xzScale);
                p1.y = ho + planePoints[0].x * xzScale + 0.2 * i * (planePoints[2].x * xzScale - planePoints[0].x * xzScale);
                p2.x = 49 + planePoints[1].z * xzScale + 0.2 * i * (planePoints[3].z * xzScale - planePoints[1].z * xzScale);
                p2.y = ho + planePoints[1].x * xzScale + 0.2 * i * (planePoints[3].x * xzScale - planePoints[1].x * xzScale);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
            }

            break;


        case 2:  // ======== Fit sphere==========

            SphereModel = vuxyzrgb_temp.PrimitiveParameter[k];

            figureModel.push_back(SphereModel);
            pcPoint.x = wo + SphereModel[0] * xyScale;
            pcPoint.y = ho + SphereModel[1] * xyScale;
            circle(projectionXY, pcPoint, SphereModel[3] * xyScale, Scalar(50, 50, 50));
            double xLeft, xRight, yTop, yLeft, yRigth;
            yTop = pcPoint.y - SphereModel[3] * xyScale;
            for (int i = 0; i < 4; i++) {
                xLeft = pcPoint.x - sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * i * SphereModel[3] * xyScale, 2));
                xRight = pcPoint.x + sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * i * SphereModel[3] * xyScale, 2));
                yLeft = yRigth = yTop + 0.5 * i * SphereModel[3] * xyScale;
                p1.x = xLeft; p1.y = yLeft;
                p2.x = xRight; p2.y = yRigth;
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                for (int j = 0; j < 3; j++) {
                    p1.x = xLeft + (xRight - xLeft) * 0.25 * (j + 1);
                    p1.y = yLeft;
                    p2.x = pcPoint.x - sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * (i + 1) * SphereModel[3] * xyScale, 2)) + 2 * sqrt(pow(SphereModel[3] * xyScale, 2) - pow(SphereModel[3] * xyScale - 0.5 * (i + 1) * SphereModel[3] * xyScale, 2)) * 0.25 * (j + 1);
                    p2.y = yTop + 0.5 * (i + 1) * SphereModel[3] * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                }
            }

            pcPoint.x = 49 + SphereModel[2] * yzScale;
            pcPoint.y = ho + SphereModel[1] * yzScale;
            circle(projectionYZ, pcPoint, SphereModel[3] * yzScale, Scalar(50, 50, 50));
            yTop = pcPoint.y - SphereModel[3] * yzScale;
            for (int i = 0; i < 4; i++) {
                xLeft = pcPoint.x - sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * i * SphereModel[3] * yzScale, 2));
                xRight = pcPoint.x + sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * i * SphereModel[3] * yzScale, 2));
                yLeft = yRigth = yTop + 0.5 * i * SphereModel[3] * yzScale;
                p1.x = xLeft; p1.y = yLeft;
                p2.x = xRight; p2.y = yRigth;
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                for (int j = 0; j < 3; j++) {
                    p1.x = xLeft + (xRight - xLeft) * 0.25 * (j + 1);
                    p1.y = yLeft;
                    p2.x = pcPoint.x - sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * (i + 1) * SphereModel[3] * yzScale, 2)) + 2 * sqrt(pow(SphereModel[3] * yzScale, 2) - pow(SphereModel[3] * yzScale - 0.5 * (i + 1) * SphereModel[3] * yzScale, 2)) * 0.25 * (j + 1);
                    p2.y = yTop + 0.5 * (i + 1) * SphereModel[3] * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                }
            }

            pcPoint.x = 49 + SphereModel[2] * xzScale;
            pcPoint.y = ho + SphereModel[0] * xzScale;
            circle(projectionXZ, pcPoint, SphereModel[3] * xzScale, Scalar(50, 50, 50));
            circle(projectionXZ, pcPoint, 0.5 * SphereModel[3] * xzScale, Scalar(50, 50, 50));
            for (int i = 0; i < 4; i++) {
                p1.x = pcPoint.x + SphereModel[3] * xzScale * cos(i * CV_PI / 4); p1.y = pcPoint.y + SphereModel[3] * xzScale * sin(i * CV_PI / 4);
                p2.x = pcPoint.x + SphereModel[3] * xzScale * cos(i * CV_PI / 4 + CV_PI); p2.y = pcPoint.y + SphereModel[3] * xzScale * sin(i * CV_PI / 4 + CV_PI);
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
            }
            break;


        case 3:  // ========== Fit cylinder =============


            CylinderModel = vuxyzrgb_temp.PrimitiveParameter[k];
            cylinderPoints = vuxyzrgb_temp.PrimitivePoints[k];

            figureModel.push_back(CylinderModel);
            int nearCenter, farCenter;
            if (CylinderModel[2] < CylinderModel[5]) {
                nearCenter = 0;
                farCenter = 1;
            }
            else {
                nearCenter = 1;
                farCenter = 0;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = wo + cylinderPoints[2 * i + nearCenter].x * xyScale; p1.y = ho + cylinderPoints[2 * i + nearCenter].y * xyScale;
                p2.x = wo + cylinderPoints[2 * (i + 1) + nearCenter].x * xyScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].y * xyScale;
                line(projectionXY, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].z < CylinderModel[2 + 3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].z < CylinderModel[2 + 3 * farCenter])) {
                    p1.x = wo + cylinderPoints[2 * i + farCenter].x * xyScale; p1.y = ho + cylinderPoints[2 * i + farCenter].y * xyScale;
                    p2.x = wo + cylinderPoints[2 * (i + 1) + farCenter].x * xyScale; p2.y = ho + cylinderPoints[2 * (i + 1) + farCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                    p1.x = wo + cylinderPoints[2 * i + farCenter].x * xyScale; p1.y = ho + cylinderPoints[2 * i + farCenter].y * xyScale;
                    p2.x = wo + cylinderPoints[2 * i + nearCenter].x * xyScale; p2.y = ho + cylinderPoints[2 * i + nearCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                    p1.x = wo + cylinderPoints[2 * (i + 1) + farCenter].x * xyScale; p1.y = ho + cylinderPoints[2 * (i + 1) + farCenter].y * xyScale;
                    p2.x = wo + cylinderPoints[2 * (i + 1) + nearCenter].x * xyScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].y * xyScale;
                    line(projectionXY, p1, p2, Scalar(50, 50, 50));
                }
            }
            if (CylinderModel[0] < CylinderModel[3]) {
                nearCenter = 1;
                farCenter = 0;
            }
            else {
                nearCenter = 0;
                farCenter = 1;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 49 + cylinderPoints[2 * i + nearCenter].z * yzScale; p1.y = ho + cylinderPoints[2 * i + nearCenter].y * yzScale;
                p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * yzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].y * yzScale;
                line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].x > CylinderModel[3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].x > CylinderModel[3 * farCenter])) {
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * yzScale; p1.y = ho + cylinderPoints[2 * i + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * yzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + farCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * yzScale; p1.y = ho + cylinderPoints[2 * i + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * i + nearCenter].z * yzScale; p2.y = ho + cylinderPoints[2 * i + nearCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * yzScale; p1.y = ho + cylinderPoints[2 * (i + 1) + farCenter].y * yzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * yzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].y * yzScale;
                    line(projectionYZ, p1, p2, Scalar(50, 50, 50));
                }
            }
            if (CylinderModel[1] < CylinderModel[4]) {
                nearCenter = 0;
                farCenter = 1;
            }
            else {
                nearCenter = 1;
                farCenter = 0;
            }
            for (int i = 0; i < 10; i++) {
                p1.x = 49 + cylinderPoints[2 * i + nearCenter].z * xzScale; p1.y = ho + cylinderPoints[2 * i + nearCenter].x * xzScale;
                p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * xzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].x * xzScale;
                line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                if ((cylinderPoints[2 * i + farCenter].y < CylinderModel[1 + 3 * farCenter]) || (cylinderPoints[2 * (i + 1) + farCenter].y < CylinderModel[1 + 3 * farCenter])) {
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * xzScale; p1.y = ho + cylinderPoints[2 * i + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * xzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + farCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * i + farCenter].z * xzScale; p1.y = ho + cylinderPoints[2 * i + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * i + nearCenter].z * xzScale; p2.y = ho + cylinderPoints[2 * i + nearCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                    p1.x = 49 + cylinderPoints[2 * (i + 1) + farCenter].z * xzScale; p1.y = ho + cylinderPoints[2 * (i + 1) + farCenter].x * xzScale;
                    p2.x = 49 + cylinderPoints[2 * (i + 1) + nearCenter].z * xzScale; p2.y = ho + cylinderPoints[2 * (i + 1) + nearCenter].x * xzScale;
                    line(projectionXZ, p1, p2, Scalar(50, 50, 50));
                }
            }
            break;
        }

    }


    //    imwrite("projectionXY.jpg", projectionXY);
    //    imwrite("projectionYZ.jpg", projectionYZ);
    //    imwrite("projectionXZ.jpg", projectionXZ);



  //  cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 11 "  <<endl;


    Mat imgPrimitivesXY_temp = projectionXY;
    Mat imgPrimitivesYZ_temp = projectionYZ;
    Mat imgPrimitivesXZ_temp = projectionXZ;


    resize(projectionXY, projectionXY, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(projectionYZ, projectionYZ, Size(), 0.5, 0.5, INTER_LINEAR);
    resize(projectionXZ, projectionXZ, Size(), 0.5, 0.5, INTER_LINEAR);








    Rect r1(0,   0,   w/2, h/2);
    Rect r2(w/2, 0,   w/2, h/2);
    Rect r3(0,   h/2, w/2, h/2);
    Rect r4(w/2, h/2, w/2, h/2);
    resize(res4, res4, Size(), 0.5, 0.5, INTER_LINEAR);
    res4.copyTo(finalImage(r1));
    projectionXY.copyTo(finalImage(r2));
    projectionYZ.copyTo(finalImage(r3));
    projectionXZ.copyTo(finalImage(r4));
    p1.x = 0; p1.y = (h/2)-0.5;
    p2.x = (w-1); p2.y = (h/2)-0.5;
    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    p1.x = (w/2)-0.5; p1.y = 0;
    p2.x = (w/2)-0.5; p2.y = (h-1);
    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    Rect rectF(0, 0, w, h);
    rectangle(finalImage, rectF, cv::Scalar(0, 0, 0), 2);







    //    Rect r1(0, 0, 960, 600);
    //    Rect r2(960, 0, 960, 600);
    //    Rect r3(0, 600, 960, 600);
    //    Rect r4(960, 600, 960, 600);
    //    resize(res4, res4, Size(), 0.5, 0.5, INTER_LINEAR);
    //    res4.copyTo(finalImage(r1));
    //    projectionXY.copyTo(finalImage(r2));
    //    projectionYZ.copyTo(finalImage(r3));
    //    projectionXZ.copyTo(finalImage(r4));
    //    p1.x = 0; p1.y = ho;
    //    p2.x = 1919; p2.y = ho;
    //    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    //    p1.x = wo; p1.y = 0;
    //    p2.x = wo; p2.y = 1199;
    //    line(finalImage, p1, p2, Scalar(0, 0, 0), 2);
    //    Rect rectF(0, 0, 1920, 1200);
    //    rectangle(finalImage, rectF, cv::Scalar(0, 0, 0), 2);





    //cv::imwrite("FinalImage.jpg", finalImage);
    // cout << "\t |||>> (9999) AddPrimitiveToIm(): END "  << endl;

  //  cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 12 "  <<endl;



    // ===========
    // Mutex (lock)
    vuxyzrgb_mutex.lock();
    // ============


    vuxyzrgb.num_prim = vuxyzrgb_temp.PrimitiveDetect.size();
    vuxyzrgb.PrimitiveDetect = vuxyzrgb_temp.PrimitiveDetect;
    vuxyzrgb.PrimitiveParameter = vuxyzrgb_temp.PrimitiveParameter;
    vuxyzrgb.PrimitivePoints = vuxyzrgb_temp.PrimitivePoints;


    // ===========
    // Mutex (unlock)
    vuxyzrgb_mutex.unlock();
    // ============




    // ===========
    // Mutex (lock)
    imgPrimitives_mutex.lock();
    // ============



    imgPrimitivesXY = imgPrimitivesXY_temp;
    imgPrimitivesYZ = imgPrimitivesYZ_temp;
    imgPrimitivesXZ = imgPrimitivesXZ_temp;
    imgPrimitive01r_general = finalImage.clone();


    // ===========
    // Mutex (unlock)
    imgPrimitives_mutex.unlock();
    // ============





   // cout << "\t\t\t\t >>>>>>>>>>>>>>>> DetectAddPrimitive 13 "  <<endl;



}








void ADS::cl_DenseStereo::DrawPrimitive()
{


    // ===========
    // Mutex (lock)
    imgPrimitives_mutex.lock();
    // ===========

    cv::Mat finalImage_temp = imgPrimitive01r_general.clone();


    // ===========
    // Mutex (unlock)
    imgPrimitives_mutex.unlock();
    // ===========



    cv::Mat outResult01r_temp;

    if (!finalImage_temp.empty())
    {


        cv::resize(finalImage_temp, outResult01r_temp, cv::Size( int(finalImage_temp.cols * CoefShowWindow),
                                                                int(finalImage_temp.rows * CoefShowWindow)), 0, 0, cv::INTER_LINEAR);

        //cv::imshow( "Window_DrawPrimitive", outResult01r_temp );
        cv::imshow( "Window_imgResult01Left", outResult01r_temp );

    }

    // ################################
    cv::waitKey(10);
    // ################################


}


void ADS::cl_DenseStereo::setCoefShowWindow(double CoefShowWindow_)
{
    CoefShowWindow = CoefShowWindow_;

}




//int ADS::cl_DenseStereo::Odometry(bool exitCode)
//{

//    //=================================================

//    //=================================================

//    auto now = chrono::system_clock::now();
//    auto in_time_t = chrono::system_clock::to_time_t(now);
//    stringstream datetime;
//    datetime << put_time(localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
//    stringstream logPath;
//    logPath << "./files_Odometry/logs/log_" << datetime.str() << ".txt";
//    ofstream logout;
//    logout.open(logPath.str());
//    if (!logout.is_open())
//        cout << "Error: can not open log file" << endl;

//    string intrinsicsPath = calibration_filename;
//    string extrinsicsPath = calibration_filename;
//    double distanceBetweenMatchedPoints = 30.0;
//    double minRange, maxRange;
//    string leftVideoFilename, rightVideoFilename;
//    double clusteringCoefficient, imageScaleFactor, maxCoordinatDuration;
//    double akazeThreshold = 0.001;
//    int clusterMinimumNumberOfPoints;
//    int imageStart, imageEnd, imageStep, minimumPointsForPnP;
//    bool saveTrajectory;



//    //=================================================

//    //=================================================


//    FileStorage fsconfig("./files_Odometry/config.xml", FileStorage::READ);
//    if (fsconfig.isOpened())
//    {
////        fsconfig["intrinsicsPath"] >> intrinsicsPath;
////        fsconfig["extrinsicsPath"] >> extrinsicsPath;
//        fsconfig["minRange"] >> minRange;
//        fsconfig["maxRange"] >> maxRange;
////        fsconfig["leftVideo"] >> leftVideoFilename;
////        fsconfig["rightVideo"] >> rightVideoFilename;
//        fsconfig["clusteringCoefficient"] >> clusteringCoefficient;
//        fsconfig["clusterMinimumNumberOfPoints"] >> clusterMinimumNumberOfPoints;
//        fsconfig["imageStart"] >> imageStart;
//        fsconfig["imageEnd"] >> imageEnd;
//        fsconfig["imageStep"] >> imageStep;
//        fsconfig["minimumPointsForPnP"] >> minimumPointsForPnP;
//        fsconfig["imageScaleFactor"] >> imageScaleFactor;
//        fsconfig["maxCoordinatDuration"] >> maxCoordinatDuration;
//        fsconfig["saveTrajectory"] >> saveTrajectory;

////        logout << "intrinsicsPath: " << intrinsicsPath << endl;
////        logout << "extrinsicsPath: " << extrinsicsPath << endl;

//        logout << "minRange: " << minRange << endl;
//        logout << "maxRange: " << maxRange << endl;
////        logout << "leftVideoFilename: " << leftVideoFilename << endl;
////        logout << "rightVideoFilename: " << rightVideoFilename << endl;
//        logout << "clusteringCoefficient: " << clusteringCoefficient << endl;
//        logout << "clusterMinimumNumberOfPoints: " << clusterMinimumNumberOfPoints << endl;
//        logout << "imageStart: " << imageStart << endl;
//        logout << "imageEnd: " << imageEnd << endl;
//        logout << "imageStep: " << imageStep << endl;
//        logout << "minimumPointsForPnP: " << minimumPointsForPnP << endl;
//        logout << "imageScaleFactor: " << imageScaleFactor << endl;
//        logout << "maxCoordinatDuration: " << maxCoordinatDuration << endl;
//        logout << "saveTrajectory: " << saveTrajectory << endl;

//    } else {
//        cout << "Error: can not read the intrinsic parameters" << endl;
//        logout << "Error: can not read the intrinsic parameters" << endl;
//        waitKey(0);
//        return 0;
//    }
//    cout << "Data loaded from config.xml" << endl;



//    StereoParams stereoParams;
//    stereoParams.init(intrinsicsPath, extrinsicsPath);

//    //=================================================

//    //=================================================

////    VideoCapture leftVideo(leftVideoFilename);
////    VideoCapture rightVideo(rightVideoFilename);

//    StereoImage previousStereoImage;
//    PointCloud previousPointCloud;
//    Mat previousLeftImage;
//    Trajectory trajectory;
////    int frameNumberLeft = leftVideo.get(CAP_PROP_FRAME_COUNT);
////    int frameNumberRight = rightVideo.get(CAP_PROP_FRAME_COUNT);
////    int lastFrameNumber = frameNumberLeft < frameNumberRight ? frameNumberLeft - 1 : frameNumberRight - 1;

////    logout << "count of images in video: " << lastFrameNumber + 1 << endl;
////    cout << "count of images in video: " << lastFrameNumber + 1 << endl;
////    if (imageEnd > lastFrameNumber) {
////        imageEnd = lastFrameNumber;
////        logout << "imageEnd changed to: " << imageEnd << endl;
////    }



//    //=================================================

//    //=================================================


//    for (int imageNumber = 0; imageNumber <= imageEnd; imageNumber++) {
//        StereoImage stereoImage;
//        PointCloud pointCloud;
//        Mat leftImage;
//        Mat rightImage;



//        // Загрузка исходного изображения
//        vector<cv::Mat> StereoPair = getImgOpenCV_StereoPair();
//        leftImage = StereoPair.at(0);
//        rightImage = StereoPair.at(1);

////        leftVideo.read(leftImage);
////        rightVideo.read(rightImage);



//        // Условие для шага
//        if (imageNumber < imageStart || imageNumber != imageStart && (imageNumber - imageStart) % imageStep != 0)
//            continue;


//        auto stepBegin = chrono::steady_clock::now();

//        cout << "Image number: " << imageNumber << endl;
//        logout << "Image number: " << imageNumber << endl;
//        stereoImage.init(leftImage, rightImage, stereoParams);

//        auto begin = chrono::steady_clock::now();
//        stereoImage.preprocessing(imageScaleFactor);
//        auto end = std::chrono::steady_clock::now();
//        auto preprocessingTime = chrono::duration_cast<chrono::milliseconds>(end - begin);

//        begin = chrono::steady_clock::now();
//        stereoImage.detectKeyPoints(distanceBetweenMatchedPoints, akazeThreshold);
//        end = std::chrono::steady_clock::now();
//        auto detectKeyPointsTime = chrono::duration_cast<chrono::milliseconds>(end - begin);

//        cout << "Indices Of Truthy Left KeyPoints: " << stereoImage.getMatchedLeftImagePoints().size() << endl;
//        logout << "Indices Of Truthy Left KeyPoints: " << stereoImage.getMatchedLeftImagePoints().size() << endl;
//        logout << "Key points detecting: " << detectKeyPointsTime.count() << "ms" << endl;
//        cout << "Key points detecting: " << detectKeyPointsTime.count() << "ms" << endl;

//        cout << "Indices Of Truthy Left KeyPoints: " << stereoImage.getIndicesOfTruthyLeftKeyPoints().size() << endl;
//        logout << "Indices Of Truthy Left KeyPoints: " << stereoImage.getIndicesOfTruthyLeftKeyPoints().size() << endl;

//        begin = chrono::steady_clock::now();
//        stereoImage.calculateWorldPoints();
//        end = std::chrono::steady_clock::now();
//        auto calculateWorldPointsTime = chrono::duration_cast<chrono::milliseconds>(end - begin);

//        pointCloud.init(stereoImage.getWorldPoints());
//        pointCloud.limitWorldPointsByRange(minRange, maxRange);
//        pointCloud.clustering(clusteringCoefficient, clusterMinimumNumberOfPoints);

//        cout << "number of clustered points: " << pointCloud.getClusteredPointsRelationToClusters().size() << endl;
//        logout << "number of clustered points: " << pointCloud.getClusteredPointsRelationToClusters().size() << endl;
//        logout << "Preprocessing: " << preprocessingTime.count() << "ms" << endl;
//        cout << "Preprocessing: " << preprocessingTime.count() << "ms" << endl;
//        logout << "World points calculation: " << calculateWorldPointsTime.count() << "ms" << endl;
//        cout << "World points calculation: " << calculateWorldPointsTime.count() << "ms" << endl;

//        if (imageNumber == imageStart || pointCloud.getClusteredPointsRelationToClusters().size() < minimumPointsForPnP) {
//            trajectory.addPoint(Mat::eye(3, 3, CV_64F), Mat::zeros(3, 1, CV_64F), imageNumber, false);
//            previousStereoImage = stereoImage;
//            previousPointCloud = pointCloud;
//            previousLeftImage = leftImage.clone();
//            continue;
//        }

//        vector<Point3d> worldPoints;
//        vector<Point2d> keyPoints;
//        vector<Point2f> p0, p1;
//        vector<int> previousLimitedIndex = previousPointCloud.getIndecesOfLimitedWorldPoints();
//        vector<int> indecesOfPreviousClusteredPoints = previousPointCloud.getIndecesOfClusteredPointsToOriginal();
//        for (uint i = 0; i < indecesOfPreviousClusteredPoints.size(); i++)
//        {
//            p0.push_back(previousStereoImage.getMatchedLeftImagePoints()[previousLimitedIndex[indecesOfPreviousClusteredPoints[i]]]);
//        }
//        vector<int> currentLimitedIndex = pointCloud.getIndecesOfLimitedWorldPoints();
//        vector<int> indecesOfCurrentClusteredPoints = pointCloud.getIndecesOfClusteredPointsToOriginal();
//        for (uint i = 0; i < indecesOfCurrentClusteredPoints.size(); i++)
//        {
//            p1.push_back(stereoImage.getMatchedLeftImagePoints()[currentLimitedIndex[indecesOfCurrentClusteredPoints[i]]]);
//        }
//        if (p0.size() < 1) {
//            trajectory.addPoint(Mat::eye(3, 3, CV_64F), Mat::zeros(3, 1, CV_64F), imageNumber, false);
//            previousStereoImage = stereoImage;
//            previousPointCloud = pointCloud;
//            previousLeftImage = leftImage.clone();
//            continue;
//        }
//        vector<uchar> status;
//        vector<float> err;
//        calcOpticalFlowPyrLK(previousStereoImage.getLeftProcessedImage(), stereoImage.getLeftProcessedImage(), p0, p1, status, err, Size(11, 11), 5);
//        vector<Point3d> previousWorldPoints = previousPointCloud.getWorldPointsClustered();
//        for (uint i = 0; i < p0.size(); i++)
//        {
//            if (status[i] == 1) {
//                worldPoints.push_back(previousWorldPoints[i]);
//                keyPoints.push_back(p1[i]);
//            }
//        }


//        if (keyPoints.size() < minimumPointsForPnP) {
//            trajectory.addPoint(Mat::eye(3, 3, CV_64F), Mat::zeros(3, 1, CV_64F), imageNumber, false);
//            previousStereoImage = stereoImage;
//            previousPointCloud = pointCloud;
//            previousLeftImage = leftImage.clone();
//            continue;
//        }

//        Mat rvec, tvec, R3, inliers;
//        solvePnPRansac(worldPoints, keyPoints, stereoParams.getLeftCameraMatrix(), stereoParams.getLeftDistortionCoefficients(), rvec, tvec, false, 1000, 0.5, 0.99, inliers, 1);
//        Rodrigues(rvec, R3);
//        Mat Rnew, Tnew;
//        trajectoryPoint point = trajectory.getLastPoint(saveTrajectory);
//        Rnew = R3.inv() * point.rotation;
//        Tnew = point.transition + Rnew * (-tvec);
//        if (abs(tvec.at<double>(0)) > maxCoordinatDuration || abs(tvec.at<double>(1)) > maxCoordinatDuration || abs(tvec.at<double>(2)) > maxCoordinatDuration) {
//            trajectory.addPoint(Mat::eye(3, 3, CV_64F), Mat::zeros(3, 1, CV_64F), imageNumber, false);
//        }
//        else {
//            trajectory.addPoint(Rnew, Tnew, imageNumber, true);
//        }

//        trajectoryPoint lastPoint = trajectory.getLastPoint(saveTrajectory);
//        cout << endl << "R3:" << endl << R3 << endl;
//        cout << endl << "tvec:" << endl << tvec << endl;
//        cout << endl << "Trajectory: " << endl;
//        cout << "R: " << endl << lastPoint.rotation << endl;
//        cout << "T: " << endl << lastPoint.transition << endl;
//        cout << "inliers: " << inliers.size() << endl << endl;
//        logout << endl << "R3:" << endl << R3 << endl;
//        logout << endl << "tvec:" << endl << tvec << endl;
//        logout << endl << "Trajectory: " << endl;
//        logout << "R: " << lastPoint.rotation << endl;
//        logout << "T: " << endl << lastPoint.transition << endl;
//        logout << "inliers: " << inliers.size() << endl << endl;

//        previousStereoImage = stereoImage;
//        previousPointCloud = pointCloud;
//        previousLeftImage = leftImage.clone();

//        auto stepEnd = std::chrono::steady_clock::now();
//        auto stepTime = chrono::duration_cast<chrono::milliseconds>(stepEnd - stepBegin);
//        logout << "Total step duration: " << stepTime.count() << "ms" << endl;
//        cout << "Total step duration: " << stepTime.count() << "ms" << endl;
//    }




//    //=================================================

//    //=================================================





//    stringstream matlabDatetime;
//    matlabDatetime << put_time(localtime(&in_time_t), "%Y_%m_%d_%H_%M_%S");
//    stringstream matlabPath;
//    matlabPath << "../logs/trajectory_" << matlabDatetime.str() << ".m";
//    ofstream mOut;
//    mOut.open(matlabPath.str());
//    if (!mOut.is_open())
//        cout << "Error: can not open .m file" << endl;

//    mOut << "clc; clear; close all;" << endl;
//    int trajectoryIndex = 0;
//    vector<trajectoryPoint> accumulatedTrajectory = trajectory.getTrajectory();
//    int i = 0;
//    while (i < accumulatedTrajectory.size()) {
//        mOut << "trajectory_" << trajectoryIndex << " = [" << endl;
//        while (accumulatedTrajectory[i].isObservable) {
//            mOut << accumulatedTrajectory[i].transition.at<double>(0) << " " << accumulatedTrajectory[i].transition.at<double>(1) << " " << accumulatedTrajectory[i].transition.at<double>(2) << endl;
//            i++;
//            if (i >= accumulatedTrajectory.size()) {
//                break;
//            }
//        }
//        mOut << "];" << endl;
//        mOut << "if size(trajectory_" << trajectoryIndex << ", 1) > 0" << endl;
//        mOut << "plot3(trajectory_" << trajectoryIndex;
//        mOut << "(:,1), trajectory_" << trajectoryIndex;
//        mOut << "(:,2), trajectory_" << trajectoryIndex;
//        mOut << "(:,3)); hold on; end" << endl;
//        trajectoryIndex++;
//        i++;
//        if (i >= accumulatedTrajectory.size()) {
//            break;
//        }
//    }
//    mOut << "xlabel('X'); ylabel('Y'), zlabel('Z');";

//    mOut.close();

////    leftVideo.release();
////    rightVideo.release();

//    logout.close();
//    waitKey(0);
//    return 0;
//}



//===================================
// Функции для определения габоритов объектов
//===================================


void ADS::cl_DenseStereo::DetectObjSize()
{

    t_vuxyzrgb vuxyzrgb_temp; // Временная Структура в формате std:vector
    type_arma_vuxyzrgb vuxyzrgb_arma_temp; // Временная Структура

//    // ===========
//    // Mutex (lock)
//    vuxyzrgb_mutex.lock();
//    // ============

//    vuxyzrgb_temp = vuxyzrgb;

//    // ===========
//    // Mutex (unlock)
//    vuxyzrgb_mutex.unlock();
//    // ============


    vuxyzrgb_temp = get_vuxyzrgbColorObj();


    if (vuxyzrgb_temp.num_p < 5)
    {

//        // ===========
//        // Mutex (lock)
//        vuxyzrgb_mutex.lock();
//        // ============

//        vuxyzrgb.ObjSize_circumscribedLength =  -2;
//        vuxyzrgb.ObjSize_circumscribedWidth =  -2;
//        vuxyzrgb.ObjSize_circumscribedHeight =  -2;

//        vuxyzrgb.ObjSize_ObjLength =  -2;
//        vuxyzrgb.ObjSize_ObjWidth =  -2;

//        // ===========
//        // Mutex (unlock)
//        vuxyzrgb_mutex.unlock();
//        // ============


        vuxyzrgb_temp.ObjSize_circumscribedLength = -2;
        vuxyzrgb_temp.ObjSize_circumscribedWidth =  -2;
        vuxyzrgb_temp.ObjSize_circumscribedHeight =  -2;

        vuxyzrgb_temp.ObjSize_ObjLength =  -2;
        vuxyzrgb_temp.ObjSize_ObjWidth =  -2;



        set_vuxyzrgbColorObj(vuxyzrgb_temp);






       return;
    }




    std::vector<double> X = vuxyzrgb_temp.xyz.at(0);
    std::vector<double> Y = vuxyzrgb_temp.xyz.at(1);
    std::vector<double> Z = vuxyzrgb_temp.xyz.at(2);


    //=====================================
    /////////// Отсеивание точек
    //Нахождение кол-во данных в массиве
    //=====================================
    double n = std::distance(std::begin(Z), std::end(Z));
    //std::cout << "Quantity v massive " << n;
    // Сумма всех элеметов в массиве
    double sum = std::accumulate(std::begin(Z), std::end(Z), 0, std::plus<int>());
    //std::cout <<"Summa v massive " <<sum;
    // Математическое ожидание
    double MatO = sum / n;
    //std::cout << "MatO " << MatO;
    // Дисперсия

    double s2n = 0;
    for (size_t i = 0; i < n; i++)
    {
        s2n += std::pow(Z[i] - MatO, 2);
    }
    s2n /= n;
    //std::cout << "dispersia " << s2n;
    // Среднее квадратичное отклонение
    double SKO = std::sqrt(s2n);
    //std::cout << "SKO " << SKO;
    double sigma = SKO * 2;
    //std::cout << "Sigma " << sigma;

    std::vector<int> del_idx;
    for (int idx = 0; idx < (int) Z.size(); ++idx)
    {
        if ((Z[idx] >= (MatO + sigma)) || (Z[idx] <= (MatO - sigma)))
        {
            del_idx.push_back(idx);

           // std::cout << "idx = " << idx << " Z = " << Z[idx] << std::endl;


        }

    }


    for (int q = 0; q < (int) del_idx.size(); q++)
    {
//        std::cout << "Delete = " << std::endl;
//        std::cout << del_idx[q] << std::endl;

        int del = del_idx[q] - q;


//        if (del > 0) std::cout  << del - 1 << " >> " << Z[del - 1] << std::endl;
//        std::cout << del << " >> " << Z[del] << std::endl;
//        if (del < Z.size()) std::cout << del + 1 << " >> " << Z[del + 1] << std::endl ;
//        std::cout << std::endl;


        X.erase(X.begin() + del);
        Y.erase(Y.begin() + del);
        Z.erase(Z.begin() + del);



//        if (del > 0) std::cout << std::endl << del - 1 << " >> " << Z[del - 1] << std::endl;
//        std::cout << del << " >> " << Z[del] << std::endl;
//        if (del < Z.size()) std::cout << del + 1 << " >> " << Z[del + 1] << std::endl ;
//        std::cout << del << " >> " << Z[del] << std::endl;

    }



//=====================================



    unsigned int start_time = clock(); // начальное время

    for (size_t i = 0; i < X.size(); i++) {
        std::cout << X[i] << '\n';
    }
    // std::cout << max_element(X.begin(), X.end());

    double Xmin = *min_element(X.begin(), X.end());
    double Xmax = *max_element(X.begin(), X.end());

    double Ymin = *min_element(Y.begin(), Y.end());
    double Ymax = *max_element(Y.begin(), Y.end());

    double Zmin = *min_element(Z.begin(), Z.end());
    double Zmax = *max_element(Z.begin(), Z.end());

    double X0 = Xmin + (Xmax - Xmin) / 2.0;
    double Y0 = Ymin + (Ymax - Ymin) / 2.0;
    double Z0 = Zmin + (Zmax - Zmin) / 2.0;

    Point3D M0 = Point3D(X0, Y0, Z0, "M0");
    Point3D M1 = Point3D(Xmin, Ymin, Zmin, "M1");
    Point3D M2 = Point3D(Xmax, Ymin, Zmin, "M2");
    Point3D M3 = Point3D(Xmax, Ymax, Zmin, "M3");
    Point3D M4 = Point3D(Xmin, Ymax, Zmin, "M4");
    Point3D M5 = Point3D(Xmin, Ymin, Zmax, "M5");
    Point3D M6 = Point3D(Xmax, Ymin, Zmax, "M6");
    Point3D M7 = Point3D(Xmax, Ymax, Zmax, "M7");
    Point3D M8 = Point3D(Xmin, Ymax, Zmax, "M8");

    size_t m1 = get_sum_count(X, Y, Z, M1, M0);
    size_t m2 = get_sum_count(X, Y, Z, M2, M0);
    size_t m3 = get_sum_count(X, Y, Z, M3, M0);
    size_t m4 = get_sum_count(X, Y, Z, M4, M0);
    size_t m5 = get_sum_count(X, Y, Z, M5, M0);
    size_t m6 = get_sum_count(X, Y, Z, M6, M0);
    size_t m7 = get_sum_count(X, Y, Z, M7, M0);
    size_t m8 = get_sum_count(X, Y, Z, M8, M0);


    // Устанавливаем кол-во точек около габаритной точки
    M1.setNumberOfPoint(m1);
    M2.setNumberOfPoint(m2);
    M3.setNumberOfPoint(m3);
    M4.setNumberOfPoint(m4);
    M5.setNumberOfPoint(m5);
    M6.setNumberOfPoint(m6);
    M7.setNumberOfPoint(m7);
    M8.setNumberOfPoint(m8);

    // Формируем список
    std::list<Point3D> M;
    M.push_back(M1);
    M.push_back(M2);
    M.push_back(M3);
    M.push_back(M4);
    M.push_back(M5);
    M.push_back(M6);
    M.push_back(M7);
    M.push_back(M8);
    // Вывод неотсортированного списка
//    std::cout << std::endl;
//    std::cout << std::endl << "M[] points:" << std::endl;
//    for (auto it = M.begin(); it != M.end(); it++)
// std::cout << it->Name << ": [" << it->NumberOfPoint << "]" << std::endl;

    // Сортировка
    M.sort();
    M.reverse();
    // Вывод отсортированного списка
//    std::cout << std::endl;
//    std::cout << std::endl << "M[] points (sorted):" << std::endl;
//    for (auto it = M.begin(); it != M.end(); it++)
//        std::cout << it->Name << ": [" << it->NumberOfPoint << "]" << std::endl;

//    std::cout << std::endl;
    // Формируем список осевых точек
    std::list<Point3D> P;
    // Первая точка берется из отсортированного списка MD
    P.push_back(M.front());
    auto& P0 = M.front();

    double m0_dist;
    double p0_dist;
    bool same_x;
    bool same_y;
    bool same_z;
    bool same_xyz;

    // Начинаем проверку со второй точки
    for (auto it = std::next(M.begin()); it != M.end(); ++it)
    {
        m0_dist = get_distance(*it, M0);  // Расстояние от текущей точки до центра масс
        p0_dist = get_distance(*it, P0);  // Расстояние между точками P0 и текущей

        // Проверяем, не лежат ли точки в одной плоскости
        same_x = it->X == P0.X;
        same_y = it->Y == P0.Y;
        same_z = it->Z == P0.Z;
        same_xyz = !(same_x || same_y || same_z);
        // if (m0_dist < p0_dist)
        if ((m0_dist < p0_dist) && same_xyz)
        {
            // Добавляем найденную точку в список
            P.push_back(*it);
            // Если нужная точка найдена, прекращаем перебор
            break;
        }
    }
    auto& P1 = P.back();

    // Выводим найденые осевые точки на экран
//    std::cout << std::endl;
//    std::cout << P0.Name << "[" << P0.NumberOfPoint << "] (" << P0.X << ", " << P0.Y << ", " << P0.Z << ")" << std::endl;
//    std::cout << P1.Name << "[" << P1.NumberOfPoint << "] (" << P1.X << ", " << P1.Y << ", " << P1.Z << ")" << std::endl;





    // Вывод габаритов параллелепипеда, где (L-длина W-ширина H-высота параллелепипид и габариты объекты: Length-высота, Width-ширина)
    double L, W, H, Length, Width;
    get_sizes(X, Y, Z, &L, &W, &H, &Length, &Width);



    vuxyzrgb_temp.ObjSize_circumscribedLength =  L;
    vuxyzrgb_temp.ObjSize_circumscribedWidth =  W;
    vuxyzrgb_temp.ObjSize_circumscribedHeight =  H;

    vuxyzrgb_temp.ObjSize_ObjLength =  Length;
    vuxyzrgb_temp.ObjSize_ObjWidth =  Width;



    set_vuxyzrgbColorObj(vuxyzrgb_temp);








//    // ===========
//    // Mutex (lock)
//    vuxyzrgb_mutex.lock();
//    // ============

//    vuxyzrgb.ObjSize_circumscribedLength =  L;
//    vuxyzrgb.ObjSize_circumscribedWidth =  W;
//    vuxyzrgb.ObjSize_circumscribedHeight =  H;

//    vuxyzrgb.ObjSize_ObjLength =  Length;
//    vuxyzrgb.ObjSize_ObjWidth =  Width;

//    // ===========
//    // Mutex (unlock)
//    vuxyzrgb_mutex.unlock();
//    // ============




    std::cout << "L = " << L << std::endl;
    std::cout << "W = " << W << std::endl;
    std::cout << "H = " << H << std::endl;
    std::cout << "Length = " << Length << std::endl;
    std::cout << "Width = " << Width << std::endl;

    unsigned int end_time = clock(); // конечное время
    unsigned int search_time = end_time - start_time;
    std::cout << "search_time = " << search_time << std::endl;

    std::cout << "X = " << X.size() << std::endl;




}


