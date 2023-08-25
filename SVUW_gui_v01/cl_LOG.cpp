
# include <cl_LOG.h>


ADS::cl_DenseStereo *obj_ptrl; // указатель оъект класса
str_Settings *settings_ptrl;  // указатель на структуру настроек




cl_LOG::cl_LOG()
{

}



// ===================================
// Слот для принятия данных с главной формы по сигналу от неё
// ===================================
void cl_LOG::slot_MainFormToLOG(ADS::cl_DenseStereo *obj_ptr_slot, str_Settings *settings_ptr_slot)
{

        obj_ptrl = obj_ptr_slot;
        settings_ptrl = settings_ptr_slot;

}

void cl_LOG::slot_MainFormToLOG_AirCraftData(int counter_AirCraft_in,
                                             double x_AirCraft_in,
                                             double y_AirCraft_in,
                                             double z_AirCraft_in,
                                             double x_GlidePath_in,
                                             double y_GlidePath_in,
                                             double z_GlidePath_in,
                                             double dx_AirCraft_in,
                                             double dy_AirCraft_in,
                                             double dz_AirCraft_in,
                                             double ro_AirCraft_in,
                                             double theta_AirCraft_in,
                                             double phi_AirCraft_in,
                                             double ro_GlidePath_in,
                                             double theta_GlidePath_in,
                                             double phi_GlidePath_in,
                                             double dro_AirCraft_in,
                                             double dtheta_AirCraft_in,
                                             double dphi_AirCraft_in)
{







    // AirCraft
    counter_AirCraft = counter_AirCraft_in;        // переменная для номера самёта (если по z произошел большой разрыв)

    x_AirCraft = x_AirCraft_in;          // Координата самолёта по x
    y_AirCraft = y_AirCraft_in;          // Координата самолёта по y
    z_AirCraft = z_AirCraft_in;          // Координата самолёта по z

    x_GlidePath = x_GlidePath_in;         // Координата глисcады по x
    y_GlidePath = y_GlidePath_in;         // Координата глиcсады по y
    z_GlidePath = z_GlidePath_in;         // Координата глисcады по z

    dx_AirCraft = dx_AirCraft_in;         // Отклонение самолёто от глисcады по x
    dy_AirCraft = dy_AirCraft_in;         // Отклонение самолёто от глисcады по y
    dz_AirCraft = dz_AirCraft_in;         // Отклонение самолёто от глисcады по z

    ro_AirCraft = ro_AirCraft_in;         // радиус AirCraft
    theta_AirCraft = theta_AirCraft_in;      // полярный угол AirCraft
    phi_AirCraft = phi_AirCraft_in;        // азимут  AirCraft

    ro_GlidePath = ro_GlidePath_in;        // радиус глисcады
    theta_GlidePath = theta_GlidePath_in;     // полярный угол глисcады
    phi_GlidePath = phi_GlidePath_in;       // азимут глисcады

    dro_AirCraft = dro_AirCraft_in;        // Отклонение самолёто от глисcады по радиусу
    dtheta_AirCraft = dtheta_AirCraft_in;     // Отклонение самолёто от глисcады по полярному углу
    dphi_AirCraft = dphi_AirCraft_in;       // Отклонение самолёто от глисcады по азимуту



}

void cl_LOG::WiriteLogAirCraft3DCoordinates()
{


        // =====================================
        // =====================================


        // =====================================
        // Создание папки
        // =====================================



        std::string path_Log1 =   "./files/Aircraft/";
        std::string path_Log2 =   "./files/Aircraft/Log/";


    #if defined(_WIN64)

        // ================
        // Only Windows
        // ================

        int result1 = _mkdir(path_Log1.c_str());
        int result2 = _mkdir(path_Log2.c_str());


        if (result1==0 )
           std:: cout << "AirCraft::Log:: Folder 'Aircraft' is made \n";
        else
            std::cout << "AirCraft::Log:: Don't made Folder 'Aircraft'  \n";

        if (result2==0 )
            std:: cout << "AirCraft::Log:: Folder '/Aircraft/Log/' is made \n";
        else
            std::cout << "AirCraft::Log:: Don't made Folder '/Aircraft/Log/'  \n";


    // ================
    // Only Windows END
    // ================

    #else

        // ================
        // Only Linux
        // ================

        if (mkdir(path_Log1.c_str(), 0777) == -1  )
            cout << "AirCraft::Log:: Folder 'Aircraft' is made \n \n";
        else
            cout << "AirCraft::Log:: Don't made Folder 'Aircraft' \n";

        if (mkdir(path_Log2.c_str(), 0777) == -1  )
            cout << "AirCraft::Log:: Folder '/Aircraft/Log/' is made \n \n";
        else
            cout << "AirCraft::Log:: Don't made Folder  '/Aircraft/Log/' \n";


    // ================
    // Only Linux END
    // ================

    #endif






        // =====================================
        // Пути к файлам
        // =====================================
        std::stringstream filename_Log_OjectDetection;

        filename_Log_OjectDetection << path_Log2  << "Log_AircraftCoordinates.txt";



        // =====================================
        //  Сохранение данных об обнаруженном объекте в текстовый файл
        // =====================================

        std::ofstream out3;          // поток для записи
        out3.open(filename_Log_OjectDetection.str(), std::ios_base::app); // окрываем файл для записи



        // ==================================
        // Как-то нужно переделать
        // ==================================
        ADS::t_vuxyzrgb vuxyzrgb = obj_ptrl->get_vuxyzrgb();
        if (vuxyzrgb.num_cl <= 0) return;
        // ==================================

        QDateTime DateTime = QDateTime::currentDateTime();

        // Потом востановить и переделать под модульность
       // vector<double> ClasterCoordinate3D =  obj_ptrl->getClasterCoordinate3D(0);


        if (out3.is_open() )
        {

            out3
                << DateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString() <<  ","

                << " counter_AirCraft: " << counter_AirCraft <<  ";"   << std::endl


                << " x_AirCraft: " << x_AirCraft <<  ","
                << " y_AirCraft: " << y_AirCraft <<  ","
                << " z_AirCraft: " << z_AirCraft <<  ";"    << std::endl


                << " x_GlidePath: " << x_GlidePath <<  ","
                << " y_GlidePath: " << y_GlidePath <<  ","
                << " z_GlidePath: " << z_GlidePath <<  ";"  << std::endl


                << " dx_AirCraft: " << dx_AirCraft <<  ","
                << " dy_AirCraft: " << dy_AirCraft <<  ","
                << " dz_AirCraft: " << dz_AirCraft <<  ";"   << std::endl

                << " ro_AirCraft: " << ro_AirCraft <<  ","
                << " theta_AirCraft: " << theta_AirCraft <<  ","
                << " phi_AirCraft: " << phi_AirCraft <<  ";"   << std::endl

                << " ro_GlidePath: " << ro_GlidePath <<  ","
                << " theta_GlidePath: " << theta_GlidePath <<  ","
                << " phi_GlidePath: " << phi_GlidePath <<  ";"   << std::endl

                << " dro_AirCraft: " << dro_AirCraft <<  ","
                << " dtheta_AirCraft: " << dtheta_AirCraft <<  ","
                << " dphi_AirCraft: " << dphi_AirCraft <<  ";"

                << std::endl;

        }

        out3.close();




        // =====================================

        // =====================================
        // Сохранение информация об обнаруженном объекте в csv Log файл
        // =====================================

        // Пути к файлам
        std::stringstream filename_Log_CSV;
        filename_Log_CSV << path_Log2  << "Log_OjectDetection.csv";

        std::ofstream out4;          // поток для записи
        out4.open(filename_Log_CSV.str(), std::ios_base::app); // окрываем файл для записи


        if (out4.is_open() )
        {

            out4
                << DateTime.toString("yyyy-MM-dd; hh:mm:ss").toStdString() <<  "; "

                << counter_AirCraft <<  "; "

                << x_AirCraft <<  "; "
                << y_AirCraft <<  "; "
                << z_AirCraft <<  "; "

                << x_GlidePath <<  "; "
                << y_GlidePath <<  "; "
                << z_GlidePath <<  "; "

                << dx_AirCraft <<  "; "
                << dy_AirCraft <<  "; "
                << dz_AirCraft <<  "; "

                << ro_AirCraft     <<  "; "
                << theta_AirCraft  <<  "; "
                << phi_AirCraft    <<  "; "

                << ro_GlidePath      <<  "; "
                << theta_GlidePath   <<  "; "
                << phi_GlidePath     <<  "; "

                << dro_AirCraft    <<  "; "
                << dtheta_AirCraft <<  "; "
                << dphi_AirCraft   <<  "; "





                << std::endl;


        }

        out4.close();


}


