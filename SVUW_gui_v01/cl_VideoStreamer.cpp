#include "cl_VideoStreamer.h"


//cl_VideoStreamer::cl_VideoStreamer(QObject *parent)
//    : QObject{parent}

 cl_VideoStreamer::cl_VideoStreamer()

{


    // By default "/shutdown" is the target to graceful shutdown the streamer
    // if you want to change the target to graceful shutdown:
    //      streamer.setShutdownTarget("/stop");

    // By default std::thread::hardware_concurrency() workers is used for streaming
    // if you want to use 4 workers instead:
    //      streamer.start(8080, 4);
    G_streamer.start(8080);

    // http://10.32.31.37:8080/bgr
    // http://localhost:8080/bgr


    // ++++++++++++++++++
    // старт таймера
    // ++++++++++++++++++
   // G_timer.start();
    // ++++++++++++++++++




}




void cl_VideoStreamer::Start_VideoStream_CycleIn()
{


    // By default "/shutdown" is the target to graceful shutdown the streamer
    // if you want to change the target to graceful shutdown:
    //      streamer.setShutdownTarget("/stop");

    // By default std::thread::hardware_concurrency() workers is used for streaming
    // if you want to use 4 workers instead:
    //      streamer.start(8080, 4);
    G_streamer.start(8080);

    int FrameCounter = 0;   // Количество кадров
    int StreamSpeed = 0;    // Количество кадров в сек
    int StreamSpeed_out = 0;    // Количество кадров в сек



    cv::Mat frame_temp;

    // ++++++++++++++++++
    // старт таймера
    // ++++++++++++++++++
   // G_timer.start();
    // ++++++++++++++++++

    // http://http://10.32.31.37:8080/bgr
    // http://localhost:8080/bgr
    std::vector<uchar> buff_bgr;



    // Visit /shutdown or another defined target to stop the loop and graceful shutdown
    while (G_streamer.isRunning() && G_ExitCode)
    {


        frame_temp = get_imgStream();





//        FrameCounter ++ ;

//        // ==========================
//        // Номер кадра
//        // ==========================
//        cv::putText(frame_temp, //target image
//                    std::to_string(FrameCounter), //text
//                    cv::Point(10, 100), //top-left position
//                    cv::FONT_HERSHEY_DUPLEX,
//                    0.8,
//                    CV_RGB(0, 0, 0), //font color
//                    2);



//        // ++++++++++++++++++
//        //  Расчёт скорости
//        // ++++++++++++++++++

//        StreamSpeed ++;

//        if (G_timer.elapsed() > 1000)
//        {
//            StreamSpeed_out = StreamSpeed;
//            StreamSpeed = 0;
//            G_timer.restart();

//        }
//        // ++++++++++++++++++


//        // ==========================
//        // Скорость стрима
//        // ==========================

//        //        stringstream text;
//        //        text  << "z: " << int(Center3D.z) << " mm";

//        cv::putText(frame_temp, //target image
//                    std::to_string(StreamSpeed_out), //text
//                    cv::Point(10, 150), //top-left position
//                    cv::FONT_HERSHEY_DUPLEX,
//                    0.8,
//                    CV_RGB(255, 255, 0), //font color
//                    2);



        cv::imencode(".jpg", frame_temp, buff_bgr, G_params);
        G_streamer.publish("/bgr", std::string(buff_bgr.begin(), buff_bgr.end()));


        std::this_thread::sleep_for(std::chrono::milliseconds(40));



    }

    G_streamer.stop();



}




void cl_VideoStreamer::StartStreamer()

{


    // By default "/shutdown" is the target to graceful shutdown the streamer
    // if you want to change the target to graceful shutdown:
    //      streamer.setShutdownTarget("/stop");

    // By default std::thread::hardware_concurrency() workers is used for streaming
    // if you want to use 4 workers instead:
    //      streamer.start(8080, 4);
    G_streamer.start(8080);

    // http://http://10.32.31.37:8080/bgr
    // http://localhost:8080/bgr


    // ++++++++++++++++++
    // старт таймера
    // ++++++++++++++++++
    // G_timer.start();
    // ++++++++++++++++++




}



void cl_VideoStreamer::VideoStream()
{

     G_params = {cv::IMWRITE_JPEG_QUALITY, G_StreamQualiy};


     if (!G_streamer.isRunning() ) return;

     std::vector<uchar> buff_bgr;
//    std::vector<uchar> buff_result;
//    std::vector<uchar> buff_disp;
//    std::vector<uchar> buff_3Dpoint;
//    std::vector<uchar> buff_stereo;
//    std::vector<uchar> buff_operator;


    cv::Mat frame_temp = get_imgStream();


//    G_FrameCounter ++ ;


//    // ==========================
//    // Номер кадра
//    // ==========================
//    cv::putText(frame_temp, //target image
//                std::to_string(G_FrameCounter), //text
//                cv::Point(10, 100), //top-left position
//                cv::FONT_HERSHEY_DUPLEX,
//                0.8,
//                CV_RGB(0, 0, 0), //font color
//                2);



//    // ++++++++++++++++++
//    //  Расчёт скорости
//    // ++++++++++++++++++

//    G_StreamSpeed ++;

//    if (G_timer.elapsed() > 1000)
//    {
//        G_StreamSpeed_out =G_StreamSpeed;
//        G_StreamSpeed = 0;
//        G_timer.restart();

//    }
//    // ++++++++++++++++++


//    // ==========================
//    // Скорость стрима
//    // ==========================

//    //        stringstream text;
//    //        text  << "z: " << int(Center3D.z) << " mm";

//    cv::putText(frame_temp, //target image
//                std::to_string(G_StreamSpeed_out), //text
//                cv::Point(10, 150), //top-left position
//                cv::FONT_HERSHEY_DUPLEX,
//                0.8,
//                CV_RGB(255, 255, 0), //font color
//                2);



//    cv::imencode(".jpg", frame_temp, G_buff_bgr, G_params);
//    G_streamer.publish("/bgr", std::string(G_buff_bgr.begin(), G_buff_bgr.end()));




    cv::imencode(".jpg", frame_temp, buff_bgr, G_params);
    G_streamer.publish(G_StreamPublishPath, std::string(buff_bgr.begin(), buff_bgr.end()));

    //  G_streamer.publish("/result", std::string(buff_result.begin(), buff_result.end()));





    std::this_thread::sleep_for(std::chrono::milliseconds(10));





}




void cl_VideoStreamer::Stop_VideoStream()
{

    G_ExitCode = false;
    G_streamer.stop();

}

void cl_VideoStreamer::set_imgStream(cv::Mat img)
{

    // ===========
    // Mutex (lock)
    G_imgStream_mutex.lock();
    // ============

    G_frameStream = img.clone();

    // ===========
    // Mutex (unlock)
    G_imgStream_mutex.unlock();
    // ============


}

cv::Mat cl_VideoStreamer::get_imgStream()
{


    // ===========
    // Mutex (lock)
    G_imgStream_mutex.lock();
    // ============

    cv::Mat img = G_frameStream.clone();

    // ===========
    // Mutex (unlock)
    G_imgStream_mutex.unlock();
    // ============

    return img;
}

void cl_VideoStreamer::setStreamQualiy(int StreamQualiy_temp)
{
    G_StreamQualiy = StreamQualiy_temp;
}

void cl_VideoStreamer::setSreamPort(int SreamPort_temp)
{
    G_SreamPort = SreamPort_temp;
}

void cl_VideoStreamer::setStreamPublishPath(std::string StreamPublishPath_temp)
{
    G_StreamPublishPath = StreamPublishPath_temp;
}
