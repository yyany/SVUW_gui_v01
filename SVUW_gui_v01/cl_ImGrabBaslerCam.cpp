#include "cl_ImGrabBaslerCam.h"

std::vector <void*> handle1__ (24, NULL);;   // дискриптор устройства
MV_CC_DEVICE_INFO_LIST HR_stDeviceList_ = { 0 };         // 1-ый список всех устройств в сети, которые есть в наличии



int HR_nBufSize__ = 3840*2880*3;;
unsigned char* HR_pFrameBuf__  = (unsigned char*)malloc(HR_nBufSize__);
MV_FRAME_OUT_INFO_EX HR_stInfo_;





Basler::cl_ImGrabBaslerCam::cl_ImGrabBaslerCam()

{

    // Pylon::PylonTerminate();

    // =====================================
    //  Инициализация некоторых параметров
    // =====================================

    ImgOpenCV_StereoPair.resize(2);


    // =====================================
    //  No Signal изображение
    // =====================================

    //            cv::Scalar clr;
    //            vector<cv::Scalar> clrA(8);

    //            clrA[0] = {47,      20,     162};    //  красный
    //            clrA[1] = { 25,     83,     216};    // оранжевый
    //            clrA[2] = { 32,     177,    237};    //  желтый
    //            clrA[3] = { 48,     172,    119};    // зеленый
    //            clrA[4] = { 238,	  190,     77};    // голубой
    //            clrA[5] = { 189,    114,      0};    // синий
    //            clrA[6] = { 142,    47,     126};    // фиолетовый
    //            clrA[7] = { 0,       0,       0};    // чернота



    sample = cv::Mat::zeros(img_Y,img_X,CV_8UC3);
    double coef_size = img_X/1920;
    cv::putText	(sample,
                "No Signal",
                {int(coef_size * 50), sample.rows - int(coef_size * 150)},
                cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                coef_size * 5.0,              //double 	fontScale
                cv::Scalar(47, 20, 162),
                coef_size * 20.0,               // thickness,
                cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                false);       // bottomLeftOrigin



//    // =====================================
//    //    Проверка
//    // =====================================
//    cv::Mat sample2 = cv::Mat::zeros(img_Y,img_X,CV_8UC3);
//    double CoefShowWindow = 0.2;

//    cv::resize(sample, sample2, cv::Size( int(sample.cols * CoefShowWindow),
//                                         int(sample.rows * CoefShowWindow)), 0, 0, cv::INTER_LINEAR);


//    // Запуст окна для вывода результатов
//    //  cv::namedWindow("sample", cv::WINDOW_AUTOSIZE); // Создаем  окна
//    imshow("sample",sample2);
//    // ################################
//    cv::waitKey(5);
//    // ################################




//    //==============================
//    // Инициализация полей класса для  камеры HikRobot
//    //==============================

//    //    #define MAX_BUF_SIZE    (2048*1536*3)
//    //  #define MAX_BUF_SIZE    (960*720*3)
//    HR_nBufSize__ = 960*720*3;

//    fill(handle1__.begin(), handle1__.end(), NULL); // дискриптор устройства
//    HR_stDeviceList_ = { 0 };                        // 1-ый список всех устройств в сети, которые есть в наличии
//    HR_pFrameBuf__ = (unsigned char*)malloc(HR_nBufSize__);

}


//Basler::cl_ImGrabBaslerCam::~cl_ImGrabBaslerCam()
//{
//    // Releases all pylon resources.
//    PylonTerminate();

//    // Releases VideoWriter
//    video_01.release();
//    video_02.release();
//}




void Basler::cl_ImGrabBaslerCam::InitializeBaslerCam()


{

    cout << "\nINITIALIZE Basler Cam\n";
    // Releases all pylon resources.
    // PylonTerminate();

    // Before using any pylon methods, the pylon runtime must be initialized.
    Pylon::PylonInitialize();

    // Инициализация параметра cameras
    cameras.Initialize(c_maxCamerasToUse);



}


bool Basler::cl_ImGrabBaslerCam::openBaslerCam()
{



    cout << "\nOPEN Basler Cam\n";



    if(cameras.IsCameraDeviceRemoved())
    {
        cout << "\nCamera is IsCameraDeviceRemoved\n";
        return false;
    }









    // Get the transport layer factory.
    CTlFactory& tlFactory = CTlFactory::GetInstance();

    m_transportLayer = dynamic_cast<IGigETransportLayer*>(tlFactory.CreateTl(BaslerGigEDeviceClass));
    if (m_transportLayer == NULL)
    {
       // throw RUNTIME_EXCEPTION("No GigE transport layer available.");

        cout << "No GigE transport layer available."<< endl;

    }

    // In this sample we use the transport layer directly to enumerate cameras.
    // By calling EnumerateDevices on the TL we get get only GigE cameras.
    // You could also accomplish this by using a filter and
    // let the Transport Layer Factory enumerate.

    // Get all attached devices and exit application if no device is found.


    DeviceInfoList_t devices;


    cout <<" EnumerateDevices: " <<m_transportLayer->EnumerateDevices(devices) << endl;
    cout <<" EnumerateAllDevices: " << m_transportLayer->EnumerateAllDevices(devices) << endl;


    if ( m_transportLayer->EnumerateDevices(devices) == 0 )
    {
       // throw RUNTIME_EXCEPTION( "No camera present.");

        cout << "No camera present."<< endl;


        // Выход если камеры не подключены
        return false;
    }


    // Сортировка списка камер по ...
    m_transportLayer->EnumerateDevices(devices);


    for (size_t i = 0; i < devices.size(); ++i)
    {
        cout <<" devices["<< i <<"].GetSerialNumber():  " << devices[i].GetSerialNumber() << endl;
    }




    for (size_t q = 0; q < devices.size(); ++q)
    {
        if (devices[q].GetSerialNumber() == SerialNumber_Cams[0])
        {
            usableDeviceInfos.push_back(devices[q]);
        }


    }

    // Вместо цикла
    // usableDeviceInfos.push_back(devices[0]);





    cout <<" devices.size():  " << devices.size() << endl;
    cout <<" usableDeviceInfos.size():  " << usableDeviceInfos.size() << endl;


    const String_t subnet(devices[0].GetSubnetAddress());
    cout << "subnet:  " << subnet << endl;




    // Проверка (выбор) камер по серийному номеру и подсети
    // Start with index 1 as we have already added the first one above.
    // We will also limit the number of cameras to c_maxCamerasToUse.
    for (size_t i = 1; i < devices.size() && usableDeviceInfos.size() < c_maxCamerasToUse; ++i)
    {
        for (size_t q = 0; q < devices.size(); ++q)
        {
            if (devices[q].GetSerialNumber() == SerialNumber_Cams[i])
            {
                if (subnet == devices[q].GetSubnetAddress())
                {

                    // Add this deviceInfo to the ones we will be using.
                    usableDeviceInfos.push_back(devices[q]);
                }
                else
                {
                    cerr << "Camera will not be used because it is in a different subnet "
                         << subnet << "!" << endl;
                }
            }


        }
    }


    cout <<" usableDeviceInfos.size(2):  " << usableDeviceInfos.size() << endl;


    for (size_t q = 0; q < usableDeviceInfos.size(); ++q)
    {
        cout <<" usableDeviceInfos["<<q<<"].GetSerialNumber():  " << usableDeviceInfos[q].GetSerialNumber() << endl;
    }


    //    for (size_t i = 0; i < devices.size() && usableDeviceInfos.size() < c_maxCamerasToUse; ++i)
    //    {
    //        // Add this deviceInfo to the ones we will be using.
    //            usableDeviceInfos.push_back(devices[i]);
    //    }


    // Create an array of instant cameras for the found devices and avoid exceeding a maximum number of devices.

    //CInstantCameraArray cameras( usableDeviceInfos.size());
    //CBaslerUniversalInstantCameraArray cameras( usableDeviceInfos.size());




    // Seed the random number generator and generate a random device key value.
    //        srand((unsigned)time(NULL));
    //        const uint32_t DeviceKey = rand();

    // For this sample we configure all cameras to be in the same group.
    //        const uint32_t GroupKey = 0x112233;


    //   const uint32_t DeviceKey = 0x29;
    //  const uint32_t GroupKey  = 0x112233;
    //        const uint32_t GroupMask = 0xffffffff;





    // For the following sample we use the CActionTriggerConfiguration to configure the camera.
    // It will set the DeviceKey, GroupKey and GroupMask features. It will also
    // configure the camera FrameTrigger and set the TriggerSource to the action command.
    // You can look at the implementation of CActionTriggerConfiguration in <pylon/gige/ActionTriggerConfiguration.h>
    // to see which features are set.

    // Create all GigE cameras and attach them to the InstantCameras in the array.
    // Create and attach all Pylon Devices.

    //    usableDeviceInfos[0].SetUserDefinedName("left");
    //    usableDeviceInfos[1].SetUserDefinedName("right");

    cout << "cameras.GetSize(): " << cameras.GetSize() << endl;

    for ( std::size_t i = 0; i < cameras.GetSize(); ++i)
    {



        // cameras[i].Attach(tlFactory.CreateDevice(usableDeviceInfos[i]));
        //cameras[i].Attach(CTlFactory::GetInstance().CreateFirstDevice(usableDeviceInfos[i]));
        cameras[i].Attach(m_transportLayer->CreateFirstDevice(usableDeviceInfos[i]));

        // We'll use the CActionTriggerConfiguration, which will set up the cameras to wait for an action command.
        cameras[i].RegisterConfiguration(new CActionTriggerConfiguration(DeviceKey, GroupKey, AllGroupMask), RegistrationMode_Append, Cleanup_Delete);
        // Set the context. This will help us later to correlate the grab result to a camera in the array.
        cameras[i].SetCameraContext(i);

        const CBaslerGigEDeviceInfo& di = cameras[i].GetDeviceInfo();


        //cameras[i].TriggerMode.SetValue(TriggerMode_On);

        // Print the model name of the camera.
        cout << "Using camera " << i << ": "
             << di.GetFriendlyName() << " {"
             << di.GetModelName() <<  "} "
             << di.GetIpAddress() << " ["
             << di.GetSerialNumber() << "] "
             << di.GetMacAddress() <<  " <"
             << di.GetDeviceFactory()  <<  "> " << endl;


    }




    if(!cameras.IsPylonDeviceAttached())
    {
        cout << "\nCamera is not attached\n";






        return false;
    }



    if(cameras.IsOpen())
    {
        cout << "\nCamera already open\n";
        cameras.Close();







        return false;
    }

    if(!cameras.IsOpen()) cameras.Open();

//    if(cameras.IsOpen())
//    {
//        cout << "\nCamera already open\n";
//        return false;
//    }



    for ( std::size_t i = 0; i < cameras.GetSize(); ++i)
    {

        cout << "\n Reset camera to default \n";
        // "Reset camera to default";
        //cameras[i].UserSetSelector.SetValue(UserSetSelectorEnums::UserSetSelector_Default);
        //        cameras[i].UserSetSelector.SetValue(UserSetSelectorEnums::UserSetSelector_UserSet2);
        //        cameras[i].UserSetLoad.Execute();





        cout << "\n Set camera image format \n";
        // "Set camera image format";

        if (cameras[i].Width.GetMax() < cam_X[i])
        {
            cout << "\n Incorrect camera frame size\n";

            // =====================================
            //  изображение ошибки
            // =====================================

//            cv::Scalar clr;
//            vector<cv::Scalar> clrA(8);

//            clrA[0] = {47,      20,     162};    //  красный
//            clrA[1] = { 25,     83,     216};    // оранжевый
//            clrA[2] = { 32,     177,    237};    //  желтый
//            clrA[3] = { 48,     172,    119};    // зеленый
//            clrA[4] = { 238,	  190,     77};    // голубой
//            clrA[5] = { 189,    114,      0};    // синий
//            clrA[6] = { 142,    47,     126};    // фиолетовый
//            clrA[7] = { 0,       0,       0};    // чернота

            double coef_size = img_X/1920;
            cv::putText	(sample,
                        "Incorrect frame size",
                        {int(coef_size * 50), sample.rows - int(coef_size * 250)},
                        cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                        coef_size * 2.0,              //double 	fontScale
                        cv::Scalar(32, 177, 237),
                        coef_size * 5.0,               // thickness,
                        cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                        false);       // bottomLeftOrigin
           // =====================================
           // =====================================


            return false;

        }


        cameras[i].Width.SetValue(cam_X[i]);
        cameras[i].Height.SetValue(cam_Y[i]);

        cameras[i].CenterX.SetValue((bool)cam_CX[i]);
        cameras[i].CenterY.SetValue((bool)cam_CY[i]);

        if (!cam_CX[i])cameras[i].OffsetX.SetValue(cam_dX[i]);
        if (!cam_CY[i])cameras[i].OffsetY.SetValue(cam_dY[i]);





        cameras[i].PixelFormat.SetValue(PixelFormatEnums::PixelFormat_BayerRG8);

        //        cameras[i].ChunkModeActive.SetValue(true);
        //        cameras[i].ChunkSelector.SetValue(ChunkSelectorEnums::ChunkSelector_Timestamp);
        //        cameras[i].ChunkEnable.SetValue(true);
        //        cameras[i].ChunkSelector.SetValue(ChunkSelectorEnums::ChunkSelector_Framecounter);
        //        cameras[i].ChunkEnable.SetValue(true);
        //        cameras[i].CounterSelector.SetValue(CounterSelector_Counter2);

        //        cameras[i].CounterResetSource.SetValue(CounterResetSourceEnums::CounterResetSource_Software);
        //        cameras[i].CounterReset.Execute(); // reset the framecounter

        cout << "\n Set camera image format \n";
        // "Activate PTP mode";

        // cameras[i].GevIEEE1588.SetValue(true);

        // cameras[i].TriggerMode.SetValue(TriggerMode_Off);
        // Make sure that the Frame Start trigger is set to Off to enable free run
        // cameras[i].TriggerSelector.SetValue(TriggerSelector_FrameStart);
        //cameras[i].TriggerSource.SetValue(TriggerSource_Action1);
        // cameras[i].TriggerSource.SetValue(TriggerSource_Line1);


        // Let the free run start immediately without a specific start time
        //        cameras[i].SyncFreeRunTimerStartTimeLow.SetValue(0);
        //        cameras[i].SyncFreeRunTimerStartTimeHigh.SetValue(0);
        //        // Specify a trigger rate of 25 frames per second
        //        cameras[i].SyncFreeRunTimerTriggerRateAbs.SetValue(25.0);
        //        // Apply the changes
        //        cameras[i].SyncFreeRunTimerUpdate.Execute();
        //        // Enable Synchronous Free Run
        //        cameras[i].SyncFreeRunTimerEnable.SetValue(true);



        cout << "\n Network settings \n";
        // "Network settings";

        //        cameras[i].GevSCPSPacketSize.SetValue(1500); // Packet Size
        //        cameras[i].GevSCPD.SetValue(0);        // Inter-Packet Delay
        //        cameras[i].GevSCFTD.SetValue(0);        // Frame transmission delay
        //        cameras[i].GevSCBWR.SetValue(10);        // Bandwidth Reserve
        //        cameras[i].GevSCBWRA.SetValue(10);        // Bandwidth Reserve Accumulation

        //cameras[i].GevHeartbeatTimeout.SetValue(5000);   // Heartbeat Timeout



        // cameras[i].MaxNumBuffer.SetValue(16);
        // cameras[i].MaxNumQueuedBuffer.SetValue(1);



        //        cameras[i].GetStreamGrabberParams().AutoPacketSize.SetValue(true);// Auto Packet Size
        //        cameras[i].GetStreamGrabberParams().MaxBufferSize.SetValue(131072);// Maximum Buffer Size
        //        cameras[i].GetStreamGrabberParams().MaxNumBuffer.SetValue(16);// Maximum Number of Buffers
        //        cameras[i].GetStreamGrabberParams().MaxTransferSize.SetValue(1048568);// Maximum Transfer Size
        //        cameras[i].GetStreamGrabberParams().NumMaxQueuedUrbs.SetValue(64);// Num Max Queued Urbs
        //        cameras[i].GetStreamGrabberParams().ReceiveThreadPriorityOverride.SetValue(true);// Receive Thread Priority Override
        //        cameras[i].GetStreamGrabberParams().ReceiveThreadPriority.SetValue(15);// Receive Thread Priority
        //        cameras[i].GetStreamGrabberParams().SocketBufferSize.SetValue(2048);// Socket Buffer Size (socket driver only)




        //       cameras[i].GetStreamGrabberParams().EnableResend.SetValue(true); // Enable Resends
        //        cameras[i].GetStreamGrabberParams().PacketTimeout.SetValue(40);// Packet Timeout (Filter Driver only)
        //        cameras[i].GetStreamGrabberParams().FrameRetention.SetValue(200);// Frame Retention (Filter Driver only)
        //        cameras[i].GetStreamGrabberParams().ReceiveWindowSize.SetValue(16);// Receive Window Size (Performance Driver only)
        //        cameras[i].GetStreamGrabberParams().ResendRequestThreshold.SetValue(5);// Resend Request Threshold (Performance Driver only)
        //        cameras[i].GetStreamGrabberParams().ResendRequestBatching.SetValue(10);// Resend Request Batching (Performance Driver only)
        //        cameras[i].GetStreamGrabberParams().MaximumNumberResendRequests.SetValue(25); // Maximum Number of Resend Requests (Performance Driver only)
        //        cameras[i].GetStreamGrabberParams().ResendTimeout.SetValue(2);// Resend Timeout (Performance Driver only)
        //        cameras[i].GetStreamGrabberParams().ResendRequestResponseTimeout.SetValue(2);// Resend Request Response Timeout (Performance Driver only)





        cout << "\n Auto functions settings \n";
        // "Auto functions settings";
        cameras[i].AutoFunctionAOISelector.SetValue(AutoFunctionAOISelector_AOI2);
        cameras[i].AutoFunctionAOIUsageWhiteBalance.SetValue(true);
        cameras[i].AutoTargetValue.SetValue(128);
        cameras[i].AutoFunctionAOISelector.SetValue(AutoFunctionAOISelector_AOI1);
        cameras[i].AutoFunctionAOIUsageIntensity.SetValue(true);

        cameras[i].BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Continuous);
        cameras[i].ExposureAuto.SetValue(ExposureAuto_Continuous);
        //cameras[i].ExposureTimeAbs.SetValue(20000);// .SetValue(Basler_GigECamera::ExposureAuto_Off);
        cameras[i].GainAuto.SetValue(GainAuto_Continuous);

        cout << "\n Apply settings complete \n";
        //  "Apply settings complete";


        cameras[i].ActionDeviceKey.SetValue(0x29);
        //cameras[i].ActionSelector.SetValue(1);
        cameras[i].ActionGroupKey.SetValue(0x112233);
        cameras[i].ActionGroupMask.SetValue(0xffffffff);

        //cameras[i].ActionDeviceKey.SetValue(DeviceKey);
        // cameras[i].ActionGroupKey.SetValue(GroupKey);

        //
        //        cameras[i].TriggerMode.SetValue(TriggerMode_On);
        //        cameras[i].GevIEEE1588DataSetLatch();
        cameras[i].TriggerMode.SetValue(TriggerMode_Off);

    }


    //    for ( std::size_t i = 0; i < cameras.GetSize(); ++i)
    //    {
    //        cameras[i].TriggerMode.SetValue(TriggerMode_On);
    //        cameras[i].GevIEEE1588DataSetLatch();
    //    }

    cameras.StartGrabbing();




    return true;
}



void Basler::cl_ImGrabBaslerCam::stopCapture()
{

    if(cameras.IsGrabbing())
    {

        cameras.StopGrabbing();


#if defined(_WIN64)

        // ################################
        Sleep(1000);
        // ################################

#else

        // ################################
        usleep(1000*1000);
        // ################################

#endif


    }

}


bool Basler::cl_ImGrabBaslerCam::closeBaslerCam()
{
    if(cameras.IsOpen())
    {
        //cameras.StopGrabbing();
        cameras.Close();

        // Releases all pylon resources.
        // PylonTerminate();

        //

        m_transportLayer = NULL;
        usableDeviceInfos.clear();
        subnet.empty();
        cameras.DestroyDevice();
        //cameras.DetachDevice();
        // cameras.DestroyDevice();
        // m_canceled = false;

        return true;
    }


    cout << "\nCamera is CLOSED\n";
    return false;


}



void Basler::cl_ImGrabBaslerCam::capture()
{


    // This smart pointer will receive the grab result data.
    //CBaslerUniversalGrabResultPtr ptrGrabResult;
    // vector<Pylon::CPylonImage> pylonImages;   //me


    CImageFormatConverter formatConverter;   //me
    formatConverter.OutputPixelFormat = Pylon::PixelType_BGR8packed;   //me

    bool errorFlag = false;



    // Starts grabbing for all cameras starting with index 0. The grabbing
    // is started for one camera after the other. That's why the images of all
    // cameras are not taken at the same time.
    // However, a hardware trigger setup can be used to cause all cameras to grab images synchronously.
    // According to their default configuration, the cameras are
    // set up for free-running continuous acquisition.

    //cameras.StartGrabbing();


    // Now we issue the action command to all devices in the subnet.
    // The devices with a matching DeviceKey, GroupKey and valid GroupMask will grab an image.
    m_transportLayer->IssueActionCommand(DeviceKey, GroupKey, AllGroupMask, subnet);


    do
    {

        CBaslerUniversalGrabResultPtr ptrGrabResult;

        // Grab c_countOfImagesToGrab from the cameras.
        for( std::uint32_t i = 0; i < usableDeviceInfos.size() && cameras.IsGrabbing(); ++i)
        {
            cameras.RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);

            errorFlag = !ptrGrabResult->GrabSucceeded();

           // cout << "ID, result#"<< i << "\t" << ptrGrabResult->GetID() << endl;
            //            cout << "ImageNumber, result#"<< i << "\t" << ptrGrabResult->GetImageNumber() << endl;
            //            cout << "FrameNumberr, result#"<< i << "\t" << ptrGrabResult->GetFrameNumber() << endl;
            //            cout << "CameraContext, result #"<< i << "\t" <<  ptrGrabResult->GetCameraContext() << endl;
            //cout << "TimeStamp, result #"<< i << "\t" << ptrGrabResult->GetTimeStamp() << endl;



            if (  ptrGrabResult->GetCameraContext()  == 0)
            {
                //pylonImage_0.AttachGrabResultBuffer(ptrGrabResult);
                formatConverter.Convert(pylonImage_0,ptrGrabResult);

                ImgOpenCV_2right = cv::Mat (ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(),
                                           CV_8UC3, (cv::uint8_t*) pylonImage_0.GetBuffer() );
            }

            if (ptrGrabResult->GetCameraContext() == 1)
            {

                //pylonImage_1.AttachGrabResultBuffer(ptrGrabResult);
                formatConverter.Convert(pylonImage_1,ptrGrabResult);

                ImgOpenCV_1left = cv::Mat (ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(),
                                          CV_8UC3, (cv::uint8_t*) pylonImage_1.GetBuffer() );

            }







            // Show the image acquired by each camera in the window related to each camera.
            //            std::intptr_t cameraContextValue = ptrGrabResult->GetCameraContext();
            //            DisplayImage(cameraContextValue, ptrGrabResult);

            //cv::waitKey(100);

            // ################################
            //            Sleep(20);
            // ################################

        }
    }
    while(errorFlag);


    cameras.StopGrabbing();


}



bool Basler::cl_ImGrabBaslerCam::checkConection()
{

    //if (!cameras.IsOpen()) return false;
    if (cameras.IsCameraDeviceRemoved()) return false;

    return true;
}


void Basler::cl_ImGrabBaslerCam::captureContinuous(bool exitCode)
{

    //  if (!cameras.IsOpen()) return;




    if(!cameras.IsOpen())
    {
        cout << "\n captureContinuous: Camera is not Open\n";
        return;
    }





    try
    {


      // Остановка камер внутри метода


//        cout << ">> exitCode: " << exitCode << endl;

//        if (!exitCode)
//        {
//            cout << ">> exitCode: " << exitCode << endl;
//            cout << ">> exitCode: " << exitCode << endl;
//            cout << ">> exitCode: " << exitCode << endl;

//            cameras.StopGrabbing();
//            cameras.Close();


//            // Releases all pylon resources.
//            // PylonTerminate();

//            //

//            m_transportLayer = NULL;
//            usableDeviceInfos.clear();
//            subnet.empty();
//            // cameras.DestroyDevice();
//            // m_canceled = false;


//            // ################################
//            Sleep(5000);
//            // ################################


//        }


//        // =====================================
//        // Время (начало)
//        // =====================================
//        int64 time = cv::getTickCount();
//        // =====================================



        // This smart pointer will receive the grab result data.
        //CBaslerUniversalGrabResultPtr ptrGrabResult;
        // vector<Pylon::CPylonImage> pylonImages;   //me

        m_transportLayer->IssueActionCommand(DeviceKey, GroupKey, AllGroupMask, subnet);
        CImageFormatConverter formatConverter;   //me
        formatConverter.OutputPixelFormat = Pylon::PixelType_BGR8packed;   //me

        //bool errorFlag = false;




        // Starts grabbing for all cameras starting with index 0. The grabbing
        // is started for one camera after the other. That's why the images of all
        // cameras are not taken at the same time.
        // However, a hardware trigger setup can be used to cause all cameras to grab images synchronously.
        // According to their default configuration, the cameras are
        // set up for free-running continuous acquisition.





        // cameras.StartGrabbing();

        // cameras.StopGrabbing();

        //    cout << "\n  Grabbing ... \n";
        //    cout << "\n (4) cameras.IsPylonDeviceAttached(): " << cameras.IsPylonDeviceAttached()  << endl;
        //    cout << "\n (4) cameras.IsOpen() : " << cameras.IsOpen() << endl;
        //    cout << "\n (4) cameras.IsGrabbing() : " << cameras.IsGrabbing() << endl;


        // Now we issue the action command to all devices in the subnet.
        // The devices with a matching DeviceKey, GroupKey and valid GroupMask will grab an image.


        cv::Mat ImgOpenCV_1left_temp, ImgOpenCV_2right_temp, ImgOpenCV_3leftHand_temp, ImgOpenCV_4rightHand_temp;


        int captured = 0;
        int qk = 0;
        int count = 10;


       // while( (!m_canceled && captured < count) && ( (ImgOpenCV_2right_temp.empty() || ImgOpenCV_1left_temp.empty())  ))
       // while( exitCode &&  (captured < count) && ( (ImgOpenCV_2right_temp.empty() || ImgOpenCV_1left_temp.empty())  ))
        while( (captured < count) &&  (ImgOpenCV_2right_temp.empty() || ImgOpenCV_1left_temp.empty())  )
        {

            qk++;

            //if(qk == 10) cout << "StartGrabbing: (qk == 10) " << endl;
//            cout << "StartGrabbing: (qk == " << qk << endl;
//            cout << "exitCode: " << exitCode << endl;
//            cout << "captured: " << captured << endl;
//            cout << "qk: " << qk << endl;
//            cout << "count: " << count << endl;
//            cout << "BaslerCams: ImgOpenCV_1left_temp.empty() " << ImgOpenCV_1left_temp.empty() << endl;
//            cout << "BaslerCams: ImgOpenCV_2right_temp.empty() " << ImgOpenCV_2right_temp.empty() << endl;





            CBaslerUniversalGrabResultPtr ptrGrabResult;

            // Grab c_countOfImagesToGrab from the cameras.
            for( std::uint32_t i = 0; i < usableDeviceInfos.size() && cameras.IsGrabbing(); ++i)
            {
                cameras[i].RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);


                if(!ptrGrabResult->GrabSucceeded() )
                {
                    cout << "Error. Captured" << captured << "/" << count << "frames";
                    continue;
                }



                //cout << "ID, result#"<< i << "\t" << ptrGrabResult->GetID() << endl;
                //            cout << "ImageNumber, result#"<< i << "\t" << ptrGrabResult->GetImageNumber() << endl;
                //            cout << "FrameNumberr, result#"<< i << "\t" << ptrGrabResult->GetFrameNumber() << endl;
                //            cout << "CameraContext, result #"<< i << "\t" <<  ptrGrabResult->GetCameraContext() << endl;
                //cout << "\t |||||||||| TimeStamp, result #"<< i << "\t" << ptrGrabResult->GetTimeStamp() << endl;

                // Coef - коэффициент уменьшения изображения
                double Coef = 1.0;

                if (  ptrGrabResult->GetCameraContext()  == 0)
                {
                    //pylonImage_0.AttachGrabResultBuffer(ptrGrabResult);
                    formatConverter.Convert(pylonImage_0,ptrGrabResult);

                    ImgOpenCV_1left_temp = cv::Mat (ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(),
                                                   CV_8UC3, (cv::uint8_t*) pylonImage_0.GetBuffer() );

//                    resize(ImgOpenCV_1left_temp, ImgOpenCV_1left_temp, cv::Size( int(ImgOpenCV_1left_temp.cols * Coef),
//                                 int(ImgOpenCV_1left_temp.rows * Coef)), 0, 0, cv::INTER_LINEAR);




//          убраннно так как перенесено в set
//                    // =====================================
//                    // Cжатие изображения до нужного размера
//                    // =====================================
//                    if(ImgOpenCV_1left_temp.cols != img_X && ImgOpenCV_1left_temp.rows != img_Y)
//                    cv::resize(ImgOpenCV_1left_temp, ImgOpenCV_1left_temp,
//                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);


                    cout << "\t\t\t||||||||||||| imageSize (Basler) "
                         << cv::Size(ImgOpenCV_1left_temp.cols,ImgOpenCV_1left_temp.rows)
                         <<endl;

                }


                if (ptrGrabResult->GetCameraContext() == 1)
                {

                    //pylonImage_1.AttachGrabResultBuffer(ptrGrabResult);
                    formatConverter.Convert(pylonImage_1,ptrGrabResult);

                    ImgOpenCV_2right_temp = cv::Mat (ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(),
                                                    CV_8UC3, (cv::uint8_t*) pylonImage_1.GetBuffer() );


//                    resize(ImgOpenCV_2right_temp, ImgOpenCV_2right_temp, cv::Size( int(ImgOpenCV_2right_temp.cols * Coef),
//                                                 int(ImgOpenCV_2right_temp.rows * Coef)), 0, 0, cv::INTER_LINEAR);

//          убраннно так как перенесено в set
                    // =====================================
                    // Cжатие изображения до нужного размера
                    // =====================================
//                    if(ImgOpenCV_2right_temp.cols != img_X && ImgOpenCV_2right_temp.rows != img_Y)
//                    cv::resize(ImgOpenCV_2right_temp, ImgOpenCV_2right_temp,
//                               cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);

                }


                if (ptrGrabResult->GetCameraContext() == 2)
                {

                    //pylonImage_1.AttachGrabResultBuffer(ptrGrabResult);
                    formatConverter.Convert(pylonImage_1,ptrGrabResult);

                    ImgOpenCV_3leftHand_temp = cv::Mat (ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(),
                                                       CV_8UC3, (cv::uint8_t*) pylonImage_1.GetBuffer() );
                }


                if (ptrGrabResult->GetCameraContext() == 3)
                {

                    //pylonImage_1.AttachGrabResultBuffer(ptrGrabResult);
                    formatConverter.Convert(pylonImage_1,ptrGrabResult);

                    ImgOpenCV_4rightHand_temp = cv::Mat (ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(),
                                                        CV_8UC3, (cv::uint8_t*) pylonImage_1.GetBuffer() );
                }

            }

            captured++;



//            // =====================================
//            // Время (Конец)
//            // =====================================
//            time = cv::getTickCount() - time;

//            printf("\n  >    captureContinuous:(03) RetrieveResult:      %fsec\n", time*1/cv::getTickFrequency());
//            // =====================================
//            cout <<"\n  >    captureContinuous:(03) captured: " << captured << endl;




        }


//        cout << "BaslerCams: ImgOpenCV_1left_temp.empty() " << ImgOpenCV_1left_temp.empty() << endl;
//        cout << "BaslerCams: ImgOpenCV_2right_temp.empty() " << ImgOpenCV_2right_temp.empty() << endl;



//        // =====================================
//        //  No Signal изображение
//        // =====================================
//        cv::Mat sample = cv::Mat::zeros(img_Y,img_X,CV_8UC3);
//        cv::putText	(sample,
//                    "No Signal",
//                    {200, sample.rows - 200},
//                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
//                    5.0,              //double 	fontScale
//                    cv::Scalar(47, 20, 162),
//                    20.0,               // thickness,
//                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
//                    false);       // bottomLeftOrigin

//        // =====================================
//        //    Проверка
//        // =====================================
//        cv::Mat sample2;
//        double CoefShowWindow = 0.3;

//        cv::resize(sample, sample2, cv::Size( double(sample.cols * CoefShowWindow),
//                                             double(sample.rows * CoefShowWindow)), 0, 0, cv::INTER_LINEAR);

//        imshow("sample",sample2);
//        // ################################
//        cv::waitKey(5);
//        // ################################


        if( ImgOpenCV_2right_temp.empty() || ImgOpenCV_1left_temp.empty() )
        {
            ImgOpenCV_3leftHand_temp = sample.clone();
            ImgOpenCV_4rightHand_temp = sample.clone();
        }

        if( ImgOpenCV_1left_temp.empty() ) ImgOpenCV_1left_temp = sample.clone();
        if( ImgOpenCV_2right_temp.empty()) ImgOpenCV_2right_temp = sample.clone();



        setImgOpenCV_1left(ImgOpenCV_1left_temp);
        setImgOpenCV_2right(ImgOpenCV_2right_temp);

        setImgOpenCV_StereoPair(ImgOpenCV_1left_temp,ImgOpenCV_2right_temp);



        // ===========
        // Mutex (lock)
        img_mutex.lock();
        // ===========


        //ImgOpenCV_1left = ImgOpenCV_1left_temp.clone();
        //ImgOpenCV_2right = ImgOpenCV_2right_temp.clone();
        ImgOpenCV_3leftHand = ImgOpenCV_3leftHand_temp.clone();
        ImgOpenCV_4rightHand = ImgOpenCV_4rightHand_temp.clone();

       // ImgOpenCV_StereoPair.resize(2);
       // ImgOpenCV_1left_temp.copyTo(ImgOpenCV_StereoPair.at(0));
       // ImgOpenCV_2right_temp.copyTo(ImgOpenCV_StereoPair.at(1));


        // ===========
        // Mutex (unlock)
        img_mutex.unlock();
        // ===========





        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        // Занесение информации о состоянии для передачи по TCP
        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // Наличие соединения с камерами
        if(cameras.IsOpen())  ConnectToCams_Cam = 1;
        else  ConnectToCams_Cam = 0;

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // Получение изображения камерами
        if( !ImgOpenCV_2right_temp.empty() && !ImgOpenCV_1left_temp.empty() ) ImageAquisition_Cam = 1;
        else  ImageAquisition_Cam = 0;

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        // Факт успешной работы  (только в начале работы)
        if( SecondaryPowerSupply_Cam == 1 &&
            ConnectToCams_Cam == 1 &&
            ImageAquisition_Cam == 1 &&
            ReadyForOperationCompartmentLamp_Cam== 1)
        {
            FuctSuccessfulOperation_Cam = 1;
        }
        else
        {
            FuctSuccessfulOperation_Cam = 0;
        }

        // информация о состоянии StatusStateInformation (отсылается постоянно)
        StatusSuccessfulOperation_Cam = FuctSuccessfulOperation_Cam;

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



    }
    catch (const GenericException &e)
    {
        if ( cameras.IsCameraDeviceRemoved())
        {
            // The camera device has been removed. This caused the exception.
            cout << endl;
            cout << "The camera has been removed from the computer." << endl;
            cout << "The camera device removal triggered an exception:" << endl
                 << e.GetDescription() << endl;
        }
        else
        {
            // An unexpected error has occurred.
            // In this example it is handled by exiting the program.
            throw;
        }
    }

    if ( !cameras.IsCameraDeviceRemoved())
        cout << endl << "Timeout expired" << endl;




}


cv::Mat Basler::cl_ImGrabBaslerCam::getImgOpenCV_1left()
{




    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============

    cv::Mat ImgOpenCV_1left_temp = ImgOpenCV_1left.clone();
    if (ImgOpenCV_1left_temp.empty()) ImgOpenCV_1left_temp = sample.clone();

    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============

    return ImgOpenCV_1left_temp;


}

cv::Mat Basler::cl_ImGrabBaslerCam::getImgOpenCV_2right()
{

    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============

    cv::Mat ImgOpenCV_2right_temp = ImgOpenCV_2right.clone();
    if (ImgOpenCV_2right_temp.empty()) ImgOpenCV_2right_temp = sample.clone();

    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============

    return ImgOpenCV_2right_temp;

}

vector<cv::Mat> Basler::cl_ImGrabBaslerCam::getImgOpenCV_StereoPair()
{


    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============

    vector<cv::Mat> StereoPair_temp = ImgOpenCV_StereoPair;

    if (StereoPair_temp.at(0).empty()) StereoPair_temp.at(0) = sample.clone();
    if (StereoPair_temp.at(1).empty()) StereoPair_temp.at(1) = sample.clone();


    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============

return StereoPair_temp;

}


cv::Mat Basler::cl_ImGrabBaslerCam::getImgOpenCV_DoubleStereoImg()
{

    cv::Mat image01, image02;
    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============
    image01 = ImgOpenCV_StereoPair.at(0);
    image02 = ImgOpenCV_StereoPair.at(1);

    if (image01.empty()) image01 = sample.clone();
    if (image02.empty()) image02 = sample.clone();

    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============

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




cv::Mat Basler::cl_ImGrabBaslerCam::getImgOpenCV_3leftHand()
{

    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============

    return ImgOpenCV_3leftHand.clone();

    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============

}

cv::Mat Basler::cl_ImGrabBaslerCam::getImgOpenCV_4rightHand()
{

    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============

    return ImgOpenCV_4rightHand.clone();

    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============

}


void Basler::cl_ImGrabBaslerCam:: setImgOpenCV_1left(cv::Mat image)
{

    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============



    if ( (image.cols != img_X && image.rows != img_Y) && (img_X > 0 && img_Y > 0) )
        cv::resize(image, image,
                   cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);




    ImgOpenCV_1left = image.clone();

    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============


}

void Basler::cl_ImGrabBaslerCam:: setImgOpenCV_2right(cv::Mat image)
{


    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============

    if ( (image.cols != img_X && image.rows != img_Y) && (img_X > 0 && img_Y > 0) )
        cv::resize(image, image,
                   cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);

    ImgOpenCV_2right = image.clone();

    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============



}


void Basler::cl_ImGrabBaslerCam:: setImgOpenCV_StereoPair(cv::Mat image01, cv::Mat image02)
{

    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============

    if (img_X > 0 && img_Y > 0)
    {
        if (image01.cols != img_X && image01.rows != img_Y)
            cv::resize(image01, image01,
                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);

        if (image02.cols != img_X && image02.rows != img_Y)
            cv::resize(image02, image02,
                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);
    }

    ImgOpenCV_StereoPair.resize(2);

    image01.copyTo(ImgOpenCV_StereoPair.at(0));
    image02.copyTo(ImgOpenCV_StereoPair.at(1));


    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============


}



void Basler::cl_ImGrabBaslerCam:: setImgOpenCV_StereoPair_SingleImg(unsigned int idxImg, cv::Mat image)
{

    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ============

    if (img_X > 0 && img_Y > 0)
    {
        if (image.cols != img_X && image.rows != img_Y)
            cv::resize(image, image,
                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);


    }

    ImgOpenCV_StereoPair.resize(2);


    switch (idxImg)
    {
    case 0:

        image.copyTo(ImgOpenCV_StereoPair.at(0));

        break;

    case 1:

        image.copyTo(ImgOpenCV_StereoPair.at(1));
        break;
    } // switch



    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ============


}




int Basler::cl_ImGrabBaslerCam:: get_c_maxCamerasToUse()
{
    return c_maxCamerasToUse;
}


void Basler::cl_ImGrabBaslerCam:: set_c_maxCamerasToUse(int idx)
{
    c_maxCamerasToUse = idx;
}


vector<String_t> Basler::cl_ImGrabBaslerCam:: get_SerialNumber_Cams()
{
    return SerialNumber_Cams;
}


void Basler::cl_ImGrabBaslerCam:: set_SerialNumber_Cams(vector<String_t> vec)
{
    SerialNumber_Cams = vec;
}



vector<string> Basler::cl_ImGrabBaslerCam:: get_StreamAddress_IPcams()
{
    return StreamAddress_IPcams;
}


void Basler::cl_ImGrabBaslerCam:: set_StreamAddress_IPcams(vector<string> vec)
{
    StreamAddress_IPcams = vec;

    videoStreamAddressLeft =      vec[0];
    videoStreamAddressRight =     vec[1];
    videoStreamAddressLeftHand =  vec[2];
    videoStreamAddressRightHand = vec[3];


}


double Basler::cl_ImGrabBaslerCam::framerate() const
{
    return m_framerate;
}


void Basler::cl_ImGrabBaslerCam::openIPcam()
{



//    cout << "IPCam: Start openIPcam .... " << endl;

//    cout << "IPCam: c_maxCamerasToUse == 2 .... " << endl;
//    cout << "IPCam: videoStreamAddressLeft  " << videoStreamAddressLeft << endl;
//    cout << "IPCam: videoStreamAddressRight  " << videoStreamAddressRight << endl;

//    cout << "IPCam: vcap01  " << endl;
//    // vcap01.open( (const cv::String) videoStreamAddressLeft);

//            if(!vcap01.open( (const cv::String) videoStreamAddressLeft) )
//                std::cout << "Error opening Cam 01" << std::endl;

//    cout << "IPCam: vcap02  " << endl;
//    vcap02.open(videoStreamAddressRight);



    //open the video stream and make sure it's opened

    if (c_maxCamerasToUse == 1)
    {
        if(!vcap01.open(videoStreamAddressLeft) )
            std::cout << "Error opening Cam 01" << std::endl;
    }


    else if (c_maxCamerasToUse == 2)

    {

        cout << "IPCam: c_maxCamerasToUse == 2 .... " << endl;
        cout << "IPCam: videoStreamAddressLeft  " << videoStreamAddressLeft << endl;
        cout << "IPCam: videoStreamAddressRight  " << videoStreamAddressRight << endl;





        if ( vcap01.isOpened() )
        {
            vcap01.release();

            cerr << "Cam 01:: was open, released \n";

        }





        if ( vcap02.isOpened() )
        {
            vcap02.release();

            cerr << "Cam 02:: was open, released \n";


        }




        cout << "IPCam: vcap01  " << endl;
        if(!vcap01.open( (const cv::String)  videoStreamAddressLeft, cv::CAP_ANY ) )
            std::cout << "Error opening Cam 01" << std::endl;


#if defined(_WIN64)
        // ################################
        Sleep(100);
        // ################################
#else \
    // ################################
        usleep(100*1000);
        // ################################
#endif



        cout << "IPCam: vcap02  " << endl;
        if(!vcap02.open( (const cv::String) videoStreamAddressRight, cv::CAP_ANY ))
            std::cout << "Error opening Cam 02" << std::endl;


    }

    else if (c_maxCamerasToUse == 3)
    {
        if(!vcap01.open(videoStreamAddressLeft) )
            std::cout << "Error opening Cam 01" << std::endl;

        if(!vcap02.open(videoStreamAddressRight))
            std::cout << "Error opening Cam 02" << std::endl;

        if(!vcap03.open(videoStreamAddressLeftHand))
            std::cout << "Error opening Cam 03" << std::endl;
    }
    else if (c_maxCamerasToUse == 4)
    {
        if(!vcap01.open(videoStreamAddressLeft) )
            std::cout << "Error opening Cam 01" << std::endl;


        if(!vcap02.open(videoStreamAddressRight))
            std::cout << "Error opening Cam 02" << std::endl;

        if(!vcap03.open(videoStreamAddressLeftHand))
            std::cout << "Error opening Cam 03" << std::endl;

        if(!vcap04.open(videoStreamAddressRightHand))
            std::cout << "Error opening Cam 04" << std::endl;
    }



    cout << "IPCam: END openIPcam .... " << endl;

}


void Basler::cl_ImGrabBaslerCam::CaptureFromIP()
{


    cout << "IPCam: Start CaptureFromIP .... " << endl;


    cv::Mat image01, image02, image03, image04;

    // ================



    if (c_maxCamerasToUse == 1)
    {
        if(vcap01.grab())
        {
            vcap01.retrieve(image01);

        }
        else
        {
           std::cout << "No frame Cam 01" << std::endl;
        }
//        if (image01.cols != img_X && image01.rows != img_Y)
//            cv::resize(image01, image01,
//                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);

    }


    else if (c_maxCamerasToUse == 2)

    {

        if(vcap01.grab())
        {
            vcap01.retrieve(image01);

        }
        else
        {
           std::cout << "No frame Cam 01" << std::endl;
        }

        if(vcap02.grab())
        {
            vcap02.retrieve(image02);
        }
        else
        {
           std::cout << "No frame Cam 02" << std::endl;
        }


// Перенесено в setImgOpenCV_1left ...
//        if (image01.cols != img_X && image01.rows != img_Y)
//            cv::resize(image01, image01,
//                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);


//        if (image02.cols != img_X && image02.rows != img_Y)
//            cv::resize(image02, image02,
//                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);


       cout << "IPCam: .... " << endl;

    }
    else if (c_maxCamerasToUse == 3)
    {
        if(!vcap01.read(image01))
            std::cout << "No frame Cam 01" << std::endl;


        if(!vcap02.read(image02))
            std::cout << "No frame Cam 02" << std::endl;


        if(!vcap03.read(image03))
            std::cout << "No frame Cam 02" << std::endl;
    }
    else if (c_maxCamerasToUse == 4)
    {
        if(!vcap01.read(image01))
            std::cout << "No frame Cam 01" << std::endl;


        if(!vcap02.read(image02))
            std::cout << "No frame Cam 02" << std::endl;


        if(!vcap03.read(image03))
            std::cout << "No frame Cam 02" << std::endl;


        if(!vcap04.read(image04))
            std::cout << "No frame Cam 02" << std::endl;
    }




//    cout << "IPCam: image01.empty() " << image01.empty() << endl;
//    cout << "IPCam: image02.empty() " << image02.empty() << endl;
//    cout << "IPCam: image03.empty() " << image03.empty() << endl;
//    cout << "IPCam: image04.empty() " << image04.empty() << endl;






    if (!image01.empty()) setImgOpenCV_1left(image01);
    if (!image02.empty()) setImgOpenCV_2right(image02);
    if (!image01.empty() && !image02.empty()) setImgOpenCV_StereoPair(image01,image02);



    // ===========
    // Mutex (lock)
    img_mutex.lock();
    // ===========


    ImgOpenCV_3leftHand = image03;
    ImgOpenCV_4rightHand = image04;



    // ===========
    // Mutex (unlock)
    img_mutex.unlock();
    // ===========


}



void Basler::cl_ImGrabBaslerCam::CaptureFromIP_SingleCam(unsigned int CamIndex, bool flag_AlternateGrabCam, bool flag_setImgOpenCV)
{

    cv::Mat image01, image02;

    // ================

    if( flag_AlternateGrabCam ) if (!(flag_CurrentIPCam == CamIndex) ) return;

    switch (CamIndex)
    {

    case 0:
        flag_CurrentIPCam = 1;
        break;

    case 1:
        flag_CurrentIPCam = 0;
        break;

    }  // switch


    if (CamIndex == 0)
    {

        if(vcap01.grab())
        {
            vcap01.retrieve(image01);
//            std::cout << "Thread:: Get frame Cam 01" << std::endl;
        }
        else
        {
           std::cout << "No frame Cam 01" << std::endl;
        }
        if (!image01.empty()) setImgOpenCV_1left(image01);
       // image02 = getImgOpenCV_2right();
       // if (!image01.empty() && !image02.empty()) setImgOpenCV_StereoPair(image01, image02);
        if (!image01.empty()) setImgOpenCV_StereoPair_SingleImg(0,image01);


    }


    else if (CamIndex == 1)

    {
        if(vcap02.grab())
        {
            vcap02.retrieve(image02);
//            std::cout << "Thread:: Get frame Cam 01" << std::endl;
        }
        else
        {
           std::cout << "No frame Cam 02" << std::endl;
        }

        if (!image02.empty()) setImgOpenCV_2right(image02);


//        image01 = getImgOpenCV_1left();
//        if (!image01.empty() && !image02.empty()) setImgOpenCV_StereoPair(image01, image02);

        if (!image02.empty()) setImgOpenCV_StereoPair_SingleImg(1,image02);

    }


    //    setImgOpenCV_1left(image01);
    //    setImgOpenCV_2right(image02);
    //    setImgOpenCV_StereoPair(image01,image02);





}


void Basler::cl_ImGrabBaslerCam::CaptureFromIP_SingleCam02(unsigned int CamIndex)
{

    cv::Mat image01, image02;

    constexpr int64 kTimeoutNs = 1000;
    std::vector<int> ready_index;

    // ================


    if (CamIndex == 0)
    {

        if( VideoCapture::waitAny({vcap01, vcap02}, ready_index, kTimeoutNs) )
        {
            vcap01.retrieve(image01);
//            std::cout << "Thread:: Get frame Cam 01" << std::endl;
        }
        else
        {
           std::cout << "No frame Cam 01" << std::endl;
        }
        if (!image01.empty()) setImgOpenCV_1left(image01);
        image02 = getImgOpenCV_2right();
        if (!image01.empty() && !image02.empty()) setImgOpenCV_StereoPair(image01, image02);

    }


    else if (CamIndex == 1)

    {
        if( VideoCapture::waitAny({vcap01, vcap02}, ready_index, kTimeoutNs) )
        {
            vcap02.retrieve(image02);
//            std::cout << "Thread:: Get frame Cam 01" << std::endl;
        }
        else
        {
           std::cout << "No frame Cam 02" << std::endl;
        }

        if (!image02.empty()) setImgOpenCV_2right(image02);
        image01 = getImgOpenCV_1left();
        if (!image01.empty() && !image02.empty()) setImgOpenCV_StereoPair(image01, image02);
    }


    //    setImgOpenCV_1left(image01);
    //    setImgOpenCV_2right(image02);
    //    setImgOpenCV_StereoPair(image01,image02);





}


void Basler::cl_ImGrabBaslerCam::CaptureFromFile(const char* img1Path, const char* img2Path)
{



    cv::Mat imgLeft = cv::imread(img1Path, cv::IMREAD_UNCHANGED);
    cv::Mat imgRight = cv::imread(img2Path, cv::IMREAD_UNCHANGED);



// Перенесено в setImgOpenCV_1left ...
//    if (imgLeft.cols != img_X && imgLeft.rows != img_Y)
//        cv::resize(imgLeft, imgLeft,
//                   cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);


//    if (imgRight.cols != img_X && imgRight.rows != img_Y)
//        cv::resize(imgRight, imgRight,
//                   cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);


// ======================
//
// ======================
#if defined(_WIN64)
    // ################################
    Sleep(2000);
    // ################################
#else
    // ################################
    usleep(2000*1000);
    // ################################
#endif


//    // ===========
//    // Mutex (lock)
//    img_mutex.lock();
//    // ===========

    setImgOpenCV_1left(imgLeft);
    setImgOpenCV_2right(imgRight);
    setImgOpenCV_StereoPair(imgLeft,imgRight);

//    // ===========
//    // Mutex (unlock)
//    img_mutex.unlock();
//    // ===========


}


void Basler::cl_ImGrabBaslerCam::CaptureFromVideo( string img1Path, string img2Path, vector<bool>* exitCode )
{


    cap_left.open(img1Path, cv::CAP_ANY);
    cap_right.open(img2Path, cv::CAP_ANY);


    cout << "img1Path  " << img1Path << endl;
    cout << "img2Path  " << img2Path << endl;

    cout << "cap_left.isOpened()" << cap_left.isOpened() << endl;
    cout << "cap_right.isOpened()" << cap_right.isOpened() << endl;


    int k = 0;
    while (exitCode->at(1))
    {

        cout << "#  " << k <<  ", exitCode = " << exitCode->at(1)  << endl;

        k++;

        cv::Mat frame_left, frame_right;

        cap_left >> frame_left;
        cap_right >> frame_right;


        //        cap_left.read(frame_left);
        //        cap_right.read(frame_right);

// Перенесено в setImgOpenCV_1left ...
//        if (frame_left.cols != img_X && frame_left.rows != img_Y)
//            cv::resize(frame_left, frame_left,
//                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);
//        if (frame_right.cols != img_X && frame_right.rows != img_Y)
//            cv::resize(frame_right, frame_right,
//                       cv::Size( double(img_X * img_coeff), double(img_Y * img_coeff) ), 0, 0, cv::INTER_LINEAR);




        // If the frame is empty, break immediately
        if (frame_left.empty())
            break;




//        // ===========
//        // Mutex (lock)
//        img_mutex.lock();
//        // ===========

        setImgOpenCV_1left(frame_left);
        setImgOpenCV_2right(frame_right);

        setImgOpenCV_StereoPair(frame_left, frame_right);

//        // ===========
//        // Mutex (unlock)
//        img_mutex.unlock();
//        // ===========


    }

    // When everything done, release the video capture object
    cap_left.release();
    cap_right.release();



}

void Basler::cl_ImGrabBaslerCam::InitializeStereoVideo(const char* videoFilesName)
{


    cv::Size frameSize = ImgOpenCV_1left.size();


    // Формирование путей для видео файлов
    stringstream videoFileName_01, videoFileName_02;

    videoFileName_01 << "./file/" << videoFilesName << "_left_01.mp4"  ;
    videoFileName_02 << "./file/" << videoFilesName << "_right_02.mp4"  ;

    //int fcc = cv::VideoWriter::fourcc('M', 'P', '4', '2');
    int fcc = 0;
    double fps = 0;

    video_01.open(videoFileName_01.str(), fcc, fps, frameSize,1);
    video_02.open(videoFileName_02.str(), fcc, fps, frameSize,1);

    if ( !video_01.isOpened() || !video_02.isOpened() )
    {
        cerr << "001: Could not open the output video file for write\n";
    }





}

void Basler::cl_ImGrabBaslerCam::WriteSourseStereoVideo()
{


    if ( !video_01.isOpened() || !video_02.isOpened() )
    {
        cerr << "002: Could not open the output video file for write\n";
    }




    // encode the frame into the videofile stream
    video_01.write(ImgOpenCV_1left);
    video_02.write(ImgOpenCV_2right);



//    //=========================================
//    // Создание стереопары из 2-х изображений
//    //=========================================

//    // Get dimension of final image
//    int maxV = max(ImgOpenCV_1left.rows, ImgOpenCV_2right.rows);
//    int maxU = ImgOpenCV_1left.cols + ImgOpenCV_2right.cols;


//    // Create a black image
//    cv::Mat3b outStereoImg(maxV, maxU, cv::Vec3b(0,0,0));


//    // Copy images in correct position
//    ImgOpenCV_1left.copyTo(outStereoImg(cv::Rect(0, 0, ImgOpenCV_1left.cols, ImgOpenCV_1left.rows)));
//    ImgOpenCV_2right.copyTo(outStereoImg(cv::Rect(ImgOpenCV_1left.cols, 0, ImgOpenCV_2right.cols, ImgOpenCV_2right.rows)));

//    //=========================================
//    // show live and wait for a key with timeout long enough to show images
//    //=========================================

//    double CoefShowWindow_temp = 0.5;

//    cv::resize(outStereoImg, outStereoImg, cv::Size( double(outStereoImg.cols * CoefShowWindow_temp),
//                                                    double(outStereoImg.rows) * CoefShowWindow_temp), 0, 0, cv::INTER_LINEAR);

//    cv::imshow("Live", outStereoImg);


}


//void Basler::cl_ImGrabBaslerCam::CaptureFromIP()
//{


//    cout << "IPCam: Start CaptureFromIP .... " << endl;


//    cv::Mat image01, image02, image03, image04;


//    std::cout << "(1) open the default camera using default API" << std::endl;
//     vcap01.open(0);
//    std::cout << "(2) open" << std::endl;


//   //open the video stream and make sure it's opened

//    if (c_maxCamerasToUse == 1)
//        {
//            if(!vcap01.open(videoStreamAddressLeft) )
//                std::cout << "Error opening Cam 01" << std::endl;
//        }


//    else if (c_maxCamerasToUse == 2)

//    {
//        if(!vcap01.open(videoStreamAddressLeft) )
//            std::cout << "Error opening Cam 01" << std::endl;


//        if(!vcap02.open(videoStreamAddressRight))
//            std::cout << "Error opening Cam 02" << std::endl;

//     }


//    else if (c_maxCamerasToUse == 3)
//        {
//        if(!vcap01.open(videoStreamAddressLeft) )
//            std::cout << "Error opening Cam 01" << std::endl;


//        if(!vcap02.open(videoStreamAddressRight))
//            std::cout << "Error opening Cam 02" << std::endl;


//        if(!vcap03.open(videoStreamAddressLeftHand))
//            std::cout << "Error opening Cam 02" << std::endl;
//        }
//    else if (c_maxCamerasToUse == 4)
//    {
//            if(!vcap01.open(videoStreamAddressLeft) )
//                std::cout << "Error opening Cam 01" << std::endl;


//            if(!vcap02.open(videoStreamAddressRight))
//                std::cout << "Error opening Cam 02" << std::endl;


//            if(!vcap03.open(videoStreamAddressLeftHand))
//                std::cout << "Error opening Cam 02" << std::endl;

//            if(!vcap04.open(videoStreamAddressRightHand))
//                std::cout << "Error opening Cam 02" << std::endl;
//    }

////    if(!vcap01.open(videoStreamAddressLeft) )
////        std::cout << "Error opening Cam 01" << std::endl;


////    if(!vcap02.open(videoStreamAddressRight))
////        std::cout << "Error opening Cam 02" << std::endl;


////    if(!vcap03.open(videoStreamAddressLeftHand))
////        std::cout << "Error opening Cam 02" << std::endl;

////    if(!vcap04.open(videoStreamAddressRightHand))
////        std::cout << "Error opening Cam 02" << std::endl;


//      // ================

//    if(!vcap01.read(image01))
//            std::cout << "No frame Cam 01" << std::endl;


//    if(!vcap02.read(image02))
//        std::cout << "No frame Cam 02" << std::endl;


//    if(!vcap03.read(image03))
//        std::cout << "No frame Cam 02" << std::endl;


//    if(!vcap04.read(image04))
//        std::cout << "No frame Cam 02" << std::endl;



//    cout << "IPCam: image01.empty() " << image01.empty() << endl;
//    cout << "IPCam: image02.empty() " << image02.empty() << endl;
//    cout << "IPCam: image03.empty() " << image03.empty() << endl;
//    cout << "IPCam: image04.empty() " << image04.empty() << endl;



//    // ===========
//    // Mutex (lock)
//    img_mutex.lock();
//    // ===========

//    setImgOpenCV_1left(image01);
//    setImgOpenCV_2right(image02);


//    ImgOpenCV_3leftHand = image03;
//    ImgOpenCV_4rightHand = image04;




//    // ===========
//    // Mutex (unlock)
//    img_mutex.unlock();
//    // ===========


//}


void Basler::cl_ImGrabBaslerCam::set_BaslerСamsCrop (
                    vector<int> cam_X_in,
                    vector<int> cam_Y_in,
                    vector<int> cam_CX_in,
                    vector<int> cam_CY_in,
                    vector<int> cam_dX_in,
                    vector<int> cam_dY_in)
{

    cam_X = cam_X_in;
    cam_Y = cam_Y_in;
    cam_CX = cam_CX_in;
    cam_CY = cam_CY_in;
    cam_dX = cam_dX_in;
    cam_dY = cam_dY_in;






}
void Basler::cl_ImGrabBaslerCam::set_ImgCompress (int  img_X_in, int  img_Y_in, double  img_coeff_in)
{
    img_X = img_X_in;
    img_Y =  img_Y_in;
    img_coeff =  img_coeff_in;
}







// ==============================
// HikRobot (HikVision)
// ==============================




// Print the discovered devices' information
bool Basler::cl_ImGrabBaslerCam::PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    if (NULL == pstMVDevInfo)
    {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
    {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // print current ip and user defined name
        printf("\t CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
        printf("\t UserDefinedName: %s\n" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
        printf("\t Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
        printf("\t Device chManufacturerName: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chManufacturerName);
        printf("\t Device chDeviceVersion: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chDeviceVersion);
        printf("\t Device chSerialNumber: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber);
        //  printf("\t Device chManufacturerSpecificInfo: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chManufacturerSpecificInfo);


        //        std::cout << " UserDefinedName: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName << std::endl;
        //        std::cout << " Device Model Name: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName << std::endl;
        //        std::cout << " Device chManufacturerName: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chManufacturerName << std::endl;
        //        std::cout << " Device chManufacturerSpecificInfo: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chManufacturerSpecificInfo << std::endl;
        //        std::cout << " Device chDeviceVersion: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chDeviceVersion << std::endl;
        std::cout << " Serial Number: " << pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber << std::endl;


        stringstream ff;
        ff << pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber;
        std::cout << " Serial Number2: " << ff.str() << std::endl;

    }
    else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
        printf("Device Number: %d\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}




int Basler::cl_ImGrabBaslerCam::HRCam_GetCamList()
{

    int nRet = -1;       //



    // eEnum device
    // Перечислим все устройства, соответствующие указанному протоколу передачи в подсети
    unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;

    // Функция memset заполняет первые n байт области памяти, на которую указывает аргумент destination,
    // символом, код которого указывается аргументом c.
    //        Синтаксис:

    //#include < string.h >
    //            void *memset (void *destination, int c, size_t n);

    //        Аргументы:

    //                destination – указатель на заполняемый массив
    //            с – код символа для заполнения
    //            n – размер заполняемой части массива в байтах


    memset(&HR_stDeviceList_, 0, sizeof(MV_CC_DEVICE_INFO_LIST));



    // Enum device
    nRet = MV_CC_EnumDevices(nTLayerType, &HR_stDeviceList_);


    if (nRet != 0)
    {
        printf("Enum Devices fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    if (HR_stDeviceList_.nDeviceNum == 0)
    {
        printf("no camera found!\n");
        return nRet;
    }


    // +++++++++++++++++++++++++++++
    std::cout << " \t >>> 3 Enum device" << std::endl;
    // +++++++++++++++++++++++++++++





    // Show devices
    // Вывод информации об устройсвах в терминал
    if (HR_stDeviceList_.nDeviceNum > 0)
    {
        for (unsigned int i = 0; i < HR_stDeviceList_.nDeviceNum; i++)
        {
            printf("[device %d]:\n", i);
            MV_CC_DEVICE_INFO* pDeviceInfo = HR_stDeviceList_.pDeviceInfo[i];
            if (NULL == pDeviceInfo)
            {
                break;
            }


            // Печать информации об устройсве
            PrintDeviceInfo(pDeviceInfo);
        }
    }
    else
    {
        qDebug() << ("Find No Devices!\n");
        return -1;
    }




    return nRet;
}


// Найти устройство (камеру) по серийному номеру. Возращает номер  камеры в списке камер

unsigned int Basler::cl_ImGrabBaslerCam::HRCam_FindCamBySN(std::string CamSN) // CamSN - серийный номер камеры
{

    unsigned int CamIndex = 9999; // индекс камеры

    for (unsigned int q = 0; q < (unsigned int)HR_stDeviceList_.nDeviceNum; ++q)
    {


        stringstream ff;
        ff << HR_stDeviceList_.pDeviceInfo[q]->SpecialInfo.stGigEInfo.chSerialNumber;


        //        // +++++++++++++++++++++++++++++
        //        std::cout << " \t >>> ff.str() == CamSN" << (ff.str() == CamSN ) << std::endl;
        //        std::cout << " \t >>> ff.str() = " << ff.str()   << std::endl;
        //        std::cout << " \t >>> CamSN = " << CamSN  << std::endl;
        //        // +++++++++++++++++++++++++++++

        if ( ff.str() == CamSN)
        {
            CamIndex = q;

            // +++++++++++++++++++++++++++++
            std::cout << " \t >>> Device has found by Serial Number :: CamIndex = " << CamIndex << std::endl;
            // +++++++++++++++++++++++++++++

            return CamIndex;

        }


        if (CamIndex < 0)
        {
            // +++++++++++++++++++++++++++++
            std::cout << " \t >>> Device has NOT found by Serial Number :: " << CamSN << std::endl;
            // +++++++++++++++++++++++++++++

        }


    }


    return CamIndex;

}



int Basler::cl_ImGrabBaslerCam::HRCam_CreateHandle(unsigned int CamIndex)
// CamIndex - номер устройства
{
    int nRet = -1;         // статус выполнения



    // Create handle
    // создайте дескриптор устройства

    //        MV_CC_DEVICE_INFO stDevInfo = {0};
    //        memcpy(&stDevInfo, stDeviceList.pDeviceInfo[nIndex], sizeof(MV_CC_DEVICE_INFO));
    //        nRet[nIndex] = MV_CC_CreateHandle(&handle[nIndex], &stDevInfo);


    //    void *  memcpy ( void *  dest,  const void *  src,  std:: size_t  count  ) ;
    //    Копирует count байты из объекта, на который указывает , srcв объект, на который указывает dest.
    //        Оба объекта интерпретируются как массивы символов без знака.
    //            Параметры
    //            dest	-	указатель на ячейку памяти для копирования в
    //            Src	-	указатель на ячейку памяти для копирования
    //            количество	-	количество байтов для копирования




    nRet = MV_CC_CreateHandle(&handle1__[CamIndex], HR_stDeviceList_.pDeviceInfo[CamIndex]);


    if (MV_OK != nRet)
    {
        cout << "DeviceNum = " << CamIndex << "  ";
        printf("Create Handle fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // +++++++++++++++++++++++++++++
    std::cout << " \t >>>  Create Handle, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++

    return nRet;
}



int Basler::cl_ImGrabBaslerCam::HRCam_CreateHandleAllCam()
{

    int nRet = -1;         // статус выполнения



    // Create handle
    // создайте дескриптор устройства

    //        MV_CC_DEVICE_INFO stDevInfo = {0};
    //        memcpy(&stDevInfo, stDeviceList.pDeviceInfo[nIndex], sizeof(MV_CC_DEVICE_INFO));
    //        nRet[nIndex] = MV_CC_CreateHandle(&handle[nIndex], &stDevInfo);



    for(size_t q = 0; q < HR_stDeviceList_.nDeviceNum; ++q)
    {





        nRet = MV_CC_CreateHandle(&handle1__[q], HR_stDeviceList_.pDeviceInfo[q]);

        //        std::cout << handle2[q]->SpecialInfo.stGigEInfo.chSerialNumber;
        //        std::cout << stDeviceList2.pDeviceInfo[q]->SpecialInfo.stGigEInfo.chSerialNumber;


        if (MV_OK != nRet)
        {
            cout << "DeviceNum = " << q << "  ";
            printf("Create Handle fail! nRet [0x%x]\n", nRet);
            return nRet;
        }


        // +++++++++++++++++++++++++++++
        std::cout << " \t >>>  Create Handles for All Cam = " << q <<  std::endl;
        // +++++++++++++++++++++++++++++

    }




    return nRet;

}




int Basler::cl_ImGrabBaslerCam::HRCam_ConnectToCam(unsigned int CamIndex)
{

    int nRet = -1;         // статус выполнения

    // Open device
    // Подключаем устройство
    unsigned int nAccessMode = MV_ACCESS_Exclusive;
    unsigned short nSwitchoverKey = 0;

    if (CamIndex < 0 || CamIndex > HR_stDeviceList_.nDeviceNum)
    {
        cout << "CamIndex = " << CamIndex << ", ConnectToCam::  Input Device Num error!" << endl;
        return nRet;
    }


    nRet = MV_CC_OpenDevice(handle1__[CamIndex], nAccessMode, nSwitchoverKey);
    // nRet = MV_CC_OpenDevice(handle1[CamIndex] );


    if (MV_OK != nRet)
    {
        cout << "CamIndex = " << CamIndex << "  ";
        printf("ConnectToCam:: Open Device fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // +++++++++++++++++++++++++++++
    std::cout << " \t >>>>>  Device is Open, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++



//    // Закоментировано так как введено в func_main.cpp
//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//    // Занесение информации о состоянии для передачи по TCP
//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//    // Наличие соединения с камерами
//    if(MV_OK == nRet)  ConnectToCams_Cam = 1;

//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>




    return nRet;

}

int Basler::cl_ImGrabBaslerCam::HRCam_SetCamSettings(unsigned int CamIndex)
{

    int nRet = -1;         // статус выполнения


    if (CamIndex < 0 || CamIndex > HR_stDeviceList_.nDeviceNum)
    {
        cout << "CamIndex = " << CamIndex << ", SetCamSettings:: Input Device Num error!" << endl;
        return nRet;
    }



    // Detection network optimal package size (It only works for the GigE camera)
    if (HR_stDeviceList_.pDeviceInfo[CamIndex]->nTLayerType == MV_GIGE_DEVICE)
    {
        int nPacketSize = MV_CC_GetOptimalPacketSize(handle1__[CamIndex]);


        if (nPacketSize > 0)
        {
            nRet = MV_CC_SetIntValue(handle1__[CamIndex],"GevSCPSPacketSize",nPacketSize);

            // +++++++++++++++++++++++++++++
            std::cout << " \t >> SetCamSettings:: Set Int Value, CamIndex = " <<  CamIndex << std::endl;
            // +++++++++++++++++++++++++++++

            if(nRet != MV_OK)
            {
                cout << "CamIndex = " << CamIndex << "  ";
                printf("SetCamSettings:: Warning: Set Packet Size fail nRet [0x%x]!", nRet);
            }
        }
        else
        {
            cout << "CamIndex = " << CamIndex << "  ";
            printf("SetCamSettings:: Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
        }
    }






    // Set trigger mode as off
    nRet = MV_CC_SetEnumValue(handle1__[CamIndex], "TriggerMode", 0);


    if (MV_OK != nRet)
    {
        cout << "CamIndex = " << CamIndex << "  ";
        printf("SetCamSettings:: Set Trigger Mode fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // +++++++++++++++++++++++++++++
    std::cout << " \t >> SetCamSettings:: Set Trigger Mode, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++




    //  unsigned char * pData = NULL;


    //        // Get payload size
    //        // Получить размер кадра данных
    //        MVCC_INTVALUE stParam;
    //        memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    //        nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
    //        if (MV_OK != nRet)
    //        {
    //            printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
    //            break;
    //        }
    //        unsigned int nPayloadSize = stParam.nCurValue;


    //        // int nBufSize = stParam.nCurValue; // Размер данных одного кадра


    //        // +++++++++++++++++++++++++++++
    //        std::cout << " \t >>> 10 Get payload size" << std::endl;
    //        // +++++++++++++++++++++++++++++


    //        // Init image info
    //        MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
    //        memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    //        pData = (unsigned char *)malloc(sizeof(unsigned char)* (nPayloadSize));
    //        if (NULL == pData)
    //        {
    //            printf("Allocate memory failed.\n");
    //            break;
    //        }
    //        memset(pData, 0, sizeof(pData));


    //        // +++++++++++++++++++++++++++++
    //        std::cout << " \t >>> 11 Init image info" << std::endl;
    //        // +++++++++++++++++++++++++++++


    return nRet;

}

int Basler::cl_ImGrabBaslerCam::HRCam_StartGrabbing(unsigned int CamIndex)
{


    int nRet = -1;         // статус выполнения


    if (CamIndex < 0 || CamIndex > HR_stDeviceList_.nDeviceNum)
    {
        cout << "HRCam_StartGrabbing:: CamIndex = " << CamIndex << ", Input Device Num error!" << endl;
        return nRet;
    }



    // Start grab image
    // Начало сбора изображений

    nRet = MV_CC_StartGrabbing(handle1__[CamIndex]);



    if (MV_OK != nRet)
    {
        printf("HRCam_StartGrabbing:: Start Grabbing fail! nRet [0x%x]\n", nRet);
        return nRet;
    }



    // +++++++++++++++++++++++++++++
    std::cout << " \t >>>>>>>  StartGrabbing, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++


    return nRet;
}






cv::Mat Basler::cl_ImGrabBaslerCam::HRCam_GetImage(unsigned int CamIndex)
{


    int nRet = -1;         // статус выполнения
    unsigned int    nTestFrameSize = 0;
    cv::Mat HR_Img_;   // Изображение с камеры


    if (!MV_CC_IsDeviceConnected(handle1__[CamIndex]) )
    {
        cout << "CamIndex = " << CamIndex << ", HRCam_GetImage:: Input Device is not connected!" << endl;



        HR_Img_ =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(HR_Img_,
                    "HikR:: Input Device is not connected",
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

        return  HR_Img_;
    }


    if (CamIndex < 0 || CamIndex > HR_stDeviceList_.nDeviceNum)
    {
        cout << "CamIndex = " << CamIndex << ", HRCam_GetImage:: Input Device Num error!" << endl;



        HR_Img_ =  cv::Mat::zeros(cam_Y[0],cam_X[0],CV_8UC3);
        double coef_size = cam_X[0]/1920;
        cv::putText	(HR_Img_,
                    "Input Device Num error",
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


        return  HR_Img_;
    }


    memset(&HR_stInfo_, 0, sizeof(MV_FRAME_OUT_INFO_EX));


    nRet = MV_CC_GetImageForBGR(handle1__[CamIndex], HR_pFrameBuf__, HR_nBufSize__, &HR_stInfo_, 1000);



    // Закоментировано так как введено в func_main.cpp
//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//    // Занесение информации о состоянии для передачи по TCP
//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//    // Получение изображения камерами
//    if( nRet == 0 ) ImageAquisition_Cam = 1;

//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



    unsigned short width = HR_stInfo_.nWidth;
    unsigned short  height = HR_stInfo_.nHeight;

    std::cout << "Image size:: " << width << "x" << height << std::endl;


    // Фильтрация на ошибки потом добавить

    if (nRet != 0)
    {
        std::cout << "error:: GetImageForRGB:" << setbase(16) << nRet << std::endl;



        // =====================================
        //  No Signal изображение
        // =====================================

        HR_Img_ =  cv::Mat::zeros(cam_X[0],cam_Y[0],CV_8UC3);
        double coef_size = width/1920;
        cv::putText	(HR_Img_,
                    "No Signal",
                    //                    {int(coef_size * 50), HR_Img_.rows - int(coef_size * 150)},
                    {50, 150},
                    cv::FONT_HERSHEY_SIMPLEX ,              // int 	fontFace
                    2.0,              //double 	fontScale
                    cv::Scalar(47, 20, 162),
                    5.0,               // thickness,
                    cv::LINE_8,    // lineType = //cv::LINE_4 //cv::LINE_8 //cv::LINE_AA
                    false);       // bottomLeftOrigin




//        //=========================================
//        // Вывод в окно OpenCV
//        //=========================================
//        double img_coeff = 0.45;
//        cv::Mat image;


//        cv::resize(HR_Img_, image,
//                   cv::Size( double(HR_Img_.cols * img_coeff), double(HR_Img_.rows * img_coeff) ), 0, 0, cv::INTER_LINEAR);
//        cv::imshow("01", image);
//        cv::waitKey(10);


    }
    else
    {
        //... Обработка данных изображения
        //std::cout << "imagetype:" << setbase(16) << stInfo.enPixelType << endl;

        if (HR_stInfo_.enPixelType == PixelType_Gvsp_BGR8_Packed)
        {

            cv::Mat(height, width, CV_8UC3, HR_pFrameBuf__).copyTo(HR_Img_); // возращаемое изображение

        }// if

    } // if





//    // Закоментировано так как введено в func_main.cpp
//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//    // Занесение информации о состоянии для передачи по TCP
//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


//    // Факт успешной работы  (только в начале работы)
//    if( SecondaryPowerSupply_Cam == 1 &&
//        ConnectToCams_Cam == 1 &&
//        ImageAquisition_Cam == 1 &&
//        ReadyForOperationCompartmentLamp_Cam== 1)
//    {
//        FuctSuccessfulOperation_Cam = 1;
//    }
//    else
//    {
//        FuctSuccessfulOperation_Cam = 0;
//    }

//    // информация о состоянии StatusStateInformation (отсылается постоянно)
//    StatusSuccessfulOperation_Cam = FuctSuccessfulOperation_Cam;

//    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>






    return HR_Img_;

}

int Basler::cl_ImGrabBaslerCam::HRCam_StopGrabbing(unsigned int CamIndex)
{

    int nRet = -1;         // статус выполнения

    // Stop grab image
    nRet = MV_CC_StopGrabbing(handle1__[CamIndex]);
    if (MV_OK != nRet)
    {
        cout << "CamIndex = " << CamIndex << "  ";
        printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
        return nRet;
    }


    // +++++++++++++++++++++++++++++
    std::cout << " \t >>> Stop Grabbing is done, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++


    return nRet;

}

int Basler::cl_ImGrabBaslerCam::HRCam_DisconnecCam(unsigned int CamIndex)
{
    int nRet = -1;         // статус выполнения
    // Close device
    nRet = MV_CC_CloseDevice(handle1__[CamIndex]);
    if (MV_OK != nRet)
    {
        cout << "CamIndex = " << CamIndex << "  ";
        printf("Clos Device fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // +++++++++++++++++++++++++++++
    std::cout << " \t >>> Clos Device is done, CamIndex = " <<  CamIndex << std::endl;
    // +++++++++++++++++++++++++++++

    return nRet;

}




void Basler::cl_ImGrabBaslerCam::HRCam_DestroyHandle()
{

    // Destroy handle
    // Уничтожить дескриптор и освободить ресурсы


    for(size_t q = 0; q < handle1__.size(); ++q)
    {
        if (handle1__[q])
        {
            MV_CC_DestroyHandle(handle1__[q]);
            handle1__[q] = NULL;
        }
    }

//    // +++++++++++++++++++++++++++++
//    std::cout << " \t >>> Destroy Handle is done, CamIndex = " <<  CamIndex << std::endl;
//    // +++++++++++++++++++++++++++++

//    //    // Free memery
//    if (HR_pFrameBuf__)
//    {
//        free(HR_pFrameBuf__);
//        HR_pFrameBuf__ = NULL;
//    }

    //system("pause");




}

MV_CC_DEVICE_INFO_LIST Basler::cl_ImGrabBaslerCam::HRCam_getDeviceList()
{
    return HR_stDeviceList_;
}

bool Basler::cl_ImGrabBaslerCam::HRCam_IsDeviceConnected(unsigned int CamIndex)
{
   bool f =  MV_CC_IsDeviceConnected(handle1__[CamIndex]);

    return f;
}




