
#include <func_DenseStereo.h>



 ADS::type_arma_vuxyzrgb ADS::conv_std2arma (const ADS::t_vuxyzrgb& vuxyzPoints_std)
{

    ADS::type_arma_vuxyzrgb vuxyzPoints_arma;
//    vuxyzPoints_arma.num_p = int(vuxyzPoints_std.vu.size());
//    vuxyzPoints_arma.num_cl = int(vuxyzPoints_std.clusters.size());
    vuxyzPoints_arma.num_p = vuxyzPoints_std.num_p;
    vuxyzPoints_arma.num_cl = vuxyzPoints_std.num_cl;


    // Инициализация
    vuxyzPoints_arma.num_p_cl.set_size(vuxyzPoints_arma.num_cl);



    vuxyzPoints_arma.vu.set_size(vuxyzPoints_arma.num_p,2);
    vuxyzPoints_arma.xyz.set_size(vuxyzPoints_arma.num_p,3);
    vuxyzPoints_arma.rgb.set_size(vuxyzPoints_arma.num_p,3);
    vuxyzPoints_arma.cluster.set_size(vuxyzPoints_std.cluster.size(),1);

    vuxyzPoints_arma.clusters_indx.set_size(vuxyzPoints_arma.num_cl,1);
    vuxyzPoints_arma.clusters_vu.set_size(vuxyzPoints_arma.num_cl,1);
    vuxyzPoints_arma.clusters_xyz.set_size(vuxyzPoints_arma.num_cl,1);
    vuxyzPoints_arma.clusters_rgb.set_size(vuxyzPoints_arma.num_cl,1);




    // причина: неопределенная ошибка objRect
    vuxyzPoints_arma.objRect2D.set_size(vuxyzPoints_arma.num_cl,7);
    vuxyzPoints_arma.objRect3D.set_size(vuxyzPoints_arma.num_cl,10);
    vuxyzPoints_arma.objRect3D_robot_arma.set_size(vuxyzPoints_arma.num_cl,10);



    for(int q = 0; q < vuxyzPoints_arma.num_p; q++)
    {
        vuxyzPoints_arma.vu.row(q)  = arma::conv_to<arma::Row<int>>::from( vuxyzPoints_std.vu [q] );
        vuxyzPoints_arma.xyz.row(q) = arma::conv_to<arma::drowvec> ::from( vuxyzPoints_std.xyz [q] );
        vuxyzPoints_arma.rgb.row(q) = arma::conv_to<arma::Row<int>>::from( vuxyzPoints_std.rgb [q] );

        //        vuxyzPoints_arma.vu(q,0) =  vuxyzPoints_std.vu [q][0];
        //        vuxyzPoints_arma.vu(q,1) =  vuxyzPoints_std.vu [q][1];

        //        vuxyzPoints_arma.xyz(q,0) =  vuxyzPoints_std.xyz [q][0];
        //        vuxyzPoints_arma.xyz(q,1) =  vuxyzPoints_std.xyz [q][1];
        //        vuxyzPoints_arma.xyz(q,2) =  vuxyzPoints_std.xyz [q][2];

        //        vuxyzPoints_arma.rgb(q,0) =  vuxyzPoints_std.rgb [q][0];
        //        vuxyzPoints_arma.rgb(q,1) =  vuxyzPoints_std.rgb [q][1];
        //        vuxyzPoints_arma.rgb(q,2) =  vuxyzPoints_std.rgb [q][2];



    }


    vuxyzPoints_arma.cluster = arma::conv_to<arma::Col<int>>::from (vuxyzPoints_std.cluster);



    for(int qc = 0; qc < vuxyzPoints_arma.num_cl; qc++)
    {
        vuxyzPoints_arma.clusters_indx(qc) = arma::conv_to<arma::Row<int>>::from( vuxyzPoints_std.clusters.at(qc) );
        vuxyzPoints_arma.num_p_cl(qc) = int(vuxyzPoints_arma.clusters_indx(qc).size());



        for(int qp = 0; qp < vuxyzPoints_arma.num_p_cl(qc); qp++)
        {




            vuxyzPoints_arma.clusters_vu(qc).set_size(vuxyzPoints_arma.num_p_cl(qc),2);
            vuxyzPoints_arma.clusters_xyz(qc).set_size(vuxyzPoints_arma.num_p_cl(qc),3);
            vuxyzPoints_arma.clusters_rgb(qc).set_size(vuxyzPoints_arma.num_p_cl(qc),3);



     // временно - поже можно удалить
//            cout << "\t \t  >>> vuxyzPoints_arma.clusters_indx(qc)(qp): " << vuxyzPoints_arma.clusters_indx(qc)(qp) << endl;
//            cout << "\t \t  >>> vuxyzPoints_arma.xyz.row(qp): " << vuxyzPoints_arma.xyz.row(vuxyzPoints_arma.clusters_indx(qc)(qp) ) << endl;


//            cout << "\t \t  >>> vuxyzPoints_std.xyz [qp]: "
//                 << vuxyzPoints_std.xyz [vuxyzPoints_arma.clusters_indx(qc)(qp)][0]
//                 << "\t  " << vuxyzPoints_std.xyz [vuxyzPoints_arma.clusters_indx(qc)(qp)][1]
//                 << "\t  " << vuxyzPoints_std.xyz [vuxyzPoints_arma.clusters_indx(qc)(qp)][2] << endl;




            vuxyzPoints_arma.clusters_vu(qc).row(qp) = vuxyzPoints_arma.vu.row( vuxyzPoints_arma.clusters_indx(qc)(qp) );
            vuxyzPoints_arma.clusters_xyz(qc).row(qp) = vuxyzPoints_arma.xyz.row( vuxyzPoints_arma.clusters_indx(qc)(qp) );
            vuxyzPoints_arma.clusters_rgb(qc).row(qp) = vuxyzPoints_arma.rgb.row( vuxyzPoints_arma.clusters_indx(qc)(qp) );




            // причина: ошибки из-за непродуманности, поэтому перенос в метод отвечающий за кластеризацию
           //vuxyzPoints_arma.objRect2D.row(qc) = arma::conv_to<arma::Row<int>>::from( vuxyzPoints_std.objRect2D [qc] );
          // vuxyzPoints_arma.objRect3D.row(qc) = arma::conv_to<arma::drowvec> ::from( vuxyzPoints_std.objRect3D [qc] );
        }

    }



    return vuxyzPoints_arma;
}





//static void saveXYZ(const char* filename, const cv::Mat& mat, const cv::Mat& img)
//{
//    const double max_z = 1.0e4;
//    FILE* fp = fopen(filename, "wt");
//    for(int y = 0; y < mat.rows; y++)
//    {
//        for(int x = 0; x < mat.cols; x++)
//        {
//            cv::Vec3f point = mat.at<cv::Vec3f>(y, x);
//            cv::Vec3b color = img.at<cv::Vec3b>(y, x);
//            if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
//            fprintf(fp, "%i %i %f %f %f %u %u %u\n", x, y, point[0], point[1], point[2], color[0], color[1], color[2]);
//        }
//    }
//    fclose(fp);
//}

 ADS::t_vuxyzrgb ADS::get_uvxyz_02 (const cv::Mat& mat, const cv::Mat& img, int limit_OutPoints, vector<double> limit_WorkingArea )
{





    // limit - максимальное количество точек

    int c = 0;     // общее количество точек
    int k = 0;     // количество выбранных точек
    int step = 0;  // счетчик для прореживания точек
    ADS::t_vuxyzrgb vuxyzPoints;




    double min_x = limit_WorkingArea[0];
    double min_y = limit_WorkingArea[1];
    double min_z = limit_WorkingArea[2];

    double max_x = limit_WorkingArea[3];
    double max_y = limit_WorkingArea[4];
    double max_z = limit_WorkingArea[5];






    // ============== old =====================
    //    for(int v = 0; v < mat.rows; v++)
    //    {
    //        for(int u = 0; u < mat.cols; u++)
    //        {

    //            cv::Vec3f xyz = mat.at<cv::Vec3f>(v, u);
    //            cv::Vec3b rgb = img.at<cv::Vec3b>(v, u);

    //            if(fabs(xyz[2] - max_z) < FLT_EPSILON || fabs(xyz[2]) > max_z) continue;
    //            if(fabs(xyz[0] - max_x) < FLT_EPSILON || fabs(xyz[0]) > max_x) continue;
    //            if(fabs(xyz[1] - max_y) < FLT_EPSILON || fabs(xyz[1]) > max_y) continue;

    //            c++;
    //        }
    //    }
    // ============== old =====================


    for(int v = 0; v < mat.rows; v++)
    {
        for(int u = 0; u < mat.cols; u++)
        {

            cv::Vec3f xyz = mat.at<cv::Vec3f>(v, u);
            cv::Vec3b rgb = img.at<cv::Vec3b>(v, u);


            if( xyz[0] < min_x ) continue;
            if( xyz[1] < min_y ) continue;
            if( xyz[2] < min_z ) continue;

            if( xyz[0] > max_x ) continue;
            if( xyz[1] > max_y ) continue;
            if( xyz[2] > max_z ) continue;

            c++;
        }
    }




    int churn = c/limit_OutPoints;
    if (churn < 1) churn = 1;


    for(int v = 0; v < mat.rows; v++)
    {
        for(int u = 0; u < mat.cols; u++)
        {

            cv::Vec3f xyz = mat.at<cv::Vec3f>(v, u);
            cv::Vec3b rgb = img.at<cv::Vec3b>(v, u);

            if( xyz[0] < min_x ) continue;
            if( xyz[1] < min_y ) continue;
            if( xyz[2] < min_z ) continue;

            if( xyz[0] > max_x ) continue;
            if( xyz[1] > max_y ) continue;
            if( xyz[2] > max_z ) continue;

            step++;
            if (step < churn) continue;
            step = 0;

            vuxyzPoints.vu.push_back({v, u});
            vuxyzPoints.xyz.push_back(vector<double> ({xyz[0], xyz[1], xyz[2]}));
            vuxyzPoints.rgb.push_back({rgb[0], rgb[1], rgb[2]});

         // изменено (удалить в будущем)
         // vuxyzPoints.cluster.push_back(-1);

            k++;

        }
    }

    vuxyzPoints.num_p = k;


    //cout << "Points: " << k << " of " << c << "\t step: "  << churn << "\t limit_OutPoints: " << limit_OutPoints << endl;

    return vuxyzPoints;

}














vector<double> ADS::FitPlane(vector<Point3d> clastersData, vector<Point3d>& planePoints) {

    vector<double> abd(4, 0);   // z = ax + by + c: [a, b, c, err]
    int MatSize = int (clastersData.size());
    cv::Mat xyzClaster = cv::Mat::zeros(3, MatSize, CV_64F);

    for (int i = 0; i < MatSize; i++)
    {
        xyzClaster.at<double>(0, i) = clastersData[i].x;
        xyzClaster.at<double>(1, i) = clastersData[i].y;
        xyzClaster.at<double>(2, i) = clastersData[i].z;
    }


    cv::Mat A = cv::Mat::zeros(MatSize, 3, CV_64F);
    cv::Mat b = cv::Mat::zeros(MatSize, 1, CV_64F);
    cv::Mat resX = cv::Mat::zeros(3, 1, CV_64F);
    for (int i = 0; i < MatSize; i++) {
        A.at<double>(i, 0) = xyzClaster.at<double>(0, i);
        A.at<double>(i, 1) = xyzClaster.at<double>(1, i);
        A.at<double>(i, 2) = 1;
        b.at<double>(i) = xyzClaster.at<double>(2, i);
    }
    resX = ADS::MNK(A, b);



    abd[0] = resX.at<double>(0);
    abd[1] = resX.at<double>(1);
    abd[2] = resX.at<double>(2);

    double yRad = atan2(abd[0], 1.0);
    cv::Mat Ry = cv::Mat::eye(3, 3, CV_64F);
    Ry.at<double>(0, 0) = cos(yRad);
    Ry.at<double>(0, 2) = sin(yRad);
    Ry.at<double>(2, 0) = -sin(yRad);
    Ry.at<double>(2, 2) = cos(yRad);
    Mat planeRotated0 = Ry * xyzClaster;

    for (int i = 0; i < MatSize; i++) {
        A.at<double>(i, 0) = planeRotated0.at<double>(0, i);
        A.at<double>(i, 1) = planeRotated0.at<double>(1, i);
        A.at<double>(i, 2) = 1;
        b.at<double>(i) = planeRotated0.at<double>(2, i);
    }
    resX =ADS:: MNK(A, b);


    double xRad = -atan2(resX.at<double>(1), 1.0);
    cv::Mat Rx = cv::Mat::eye(3, 3, CV_64F);
    Rx.at<double>(1, 1) = cos(xRad);
    Rx.at<double>(1, 2) = -sin(xRad);
    Rx.at<double>(2, 1) = sin(xRad);
    Rx.at<double>(2, 2) = cos(xRad);
    Mat planeRotated1 = Rx * planeRotated0;
    double meanZ = 0.0;
    for (int i = 0; i < MatSize; i++)
        meanZ += planeRotated1.at<double>(2, i);
    meanZ /= MatSize;
    double meanErr = 0.0;
    for (int i = 0; i < MatSize; i++)
        meanErr += abs(planeRotated1.at<double>(2, i) - meanZ);
    meanErr /= MatSize;
    abd[3] = meanErr;

    double zRad = CV_PI / 360;


    cv::Mat Rz = cv::Mat::eye(3, 3, CV_64F);
    Rz.at<double>(0, 0) = cos(zRad);
    Rz.at<double>(0, 1) = -sin(zRad);
    Rz.at<double>(1, 0) = sin(zRad);
    Rz.at<double>(1, 1) = cos(zRad);
    vector<vector<double>> rotationResults;
    Mat planeRotatedStep = planeRotated1.clone();
    for (int i = 0; i < 180; i++) {
        vector<double> rotationStep(6, 0);
        planeRotatedStep = Rz * planeRotatedStep;
        double xmin = ADS::minMatElement(planeRotatedStep.row(0));
        double xmax = ADS::maxMatElement(planeRotatedStep.row(0));
        double ymin = ADS::minMatElement(planeRotatedStep.row(1));
        double ymax = ADS::maxMatElement(planeRotatedStep.row(1));
        double stepSq = (xmax - xmin) * (ymax - ymin);
        rotationStep[0] = stepSq;
        rotationStep[1] = xmin;
        rotationStep[2] = xmax;
        rotationStep[3] = ymin;
        rotationStep[4] = ymax;
        double stepRad = zRad * (i + 1);
        rotationStep[5] = stepRad;
        rotationResults.push_back(rotationStep);
    }


    double errMin = 1e+8;
    int minErrPlaneIdx = 0;
    for (int i = 0; i < 180; i++)
        if (rotationResults[i][0] < errMin) {
            errMin = rotationResults[i][0];
            minErrPlaneIdx = i;
        }
    Mat pPoints = Mat::zeros(3, 4, CV_64F);
    for (int i = 0; i < 4; i++) {
        pPoints.at<double>(0, i) = rotationResults[minErrPlaneIdx][floor(i / 2) + 1];
        pPoints.at<double>(1, i) = rotationResults[minErrPlaneIdx][i % 2 + 3];
        pPoints.at<double>(2, i) = meanZ;
    }
    cv::Mat Rxb = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat Ryb = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat Rzb = cv::Mat::eye(3, 3, CV_64F);
    Rxb.at<double>(1, 1) = cos(-xRad);
    Rxb.at<double>(1, 2) = -sin(-xRad);
    Rxb.at<double>(2, 1) = sin(-xRad);
    Rxb.at<double>(2, 2) = cos(-xRad);
    Ryb.at<double>(0, 0) = cos(-yRad);
    Ryb.at<double>(0, 2) = sin(-yRad);
    Ryb.at<double>(2, 0) = -sin(-yRad);
    Ryb.at<double>(2, 2) = cos(-yRad);
    Rzb.at<double>(0, 0) = cos(-rotationResults[minErrPlaneIdx][5]);
    Rzb.at<double>(0, 1) = -sin(-rotationResults[minErrPlaneIdx][5]);
    Rzb.at<double>(1, 0) = sin(-rotationResults[minErrPlaneIdx][5]);
    Rzb.at<double>(1, 1) = cos(-rotationResults[minErrPlaneIdx][5]);
    pPoints = Ryb * Rxb * Rzb * pPoints;
    for (int i = 0; i < 4; i++) {
        Point3d pp;
        pp.x = pPoints.at<double>(0, i);
        pp.y = pPoints.at<double>(1, i);
        pp.z = pPoints.at<double>(2, i);
        planePoints.push_back(pp);
    }



    return abd;
}


vector<double> ADS::FitSphere(vector<Point3d> clastersData) {
    vector<double>  SphereModel(5, 0);  //[xc, yc, zc, radius, err]
    int MatSize = int (clastersData.size());
    cv::Mat xClaster = cv::Mat::zeros(MatSize, 1, CV_64F);
    cv::Mat yClaster = cv::Mat::zeros(MatSize, 1, CV_64F);
    cv::Mat zClaster = cv::Mat::zeros(MatSize, 1, CV_64F);
    double xMean = 0.0, yMean = 0.0, zMean = 0.0;
    for (int i = 0; i < MatSize; i++)
    {
        xClaster.at<double>(i) = clastersData[i].x;
        yClaster.at<double>(i) = clastersData[i].y;
        zClaster.at<double>(i) = clastersData[i].z;
        xMean += clastersData[i].x;
        yMean += clastersData[i].y;
        zMean += clastersData[i].z;
    }
    xMean /= double(MatSize);
    yMean /= double(MatSize);
    zMean /= double(MatSize);
    cv::Mat A = cv::Mat::zeros(3, 3, CV_64F);
    cv::Mat b = cv::Mat::zeros(3, 1, CV_64F);
    cv::Mat resX = cv::Mat::zeros(3, 1, CV_64F);
    for (int i = 0; i < MatSize; i++) {
        A.at<double>(0, 0) += xClaster.at<double>(i) * (xClaster.at<double>(i) - xMean) / MatSize;
        A.at<double>(0, 1) += xClaster.at<double>(i) * (yClaster.at<double>(i) - yMean) / MatSize;
        A.at<double>(0, 2) += xClaster.at<double>(i) * (zClaster.at<double>(i) - zMean) / MatSize;
        A.at<double>(1, 0) += yClaster.at<double>(i) * (xClaster.at<double>(i) - xMean) / MatSize;
        A.at<double>(1, 1) += yClaster.at<double>(i) * (yClaster.at<double>(i) - yMean) / MatSize;
        A.at<double>(1, 2) += yClaster.at<double>(i) * (zClaster.at<double>(i) - zMean) / MatSize;
        A.at<double>(2, 0) += zClaster.at<double>(i) * (xClaster.at<double>(i) - xMean) / MatSize;
        A.at<double>(2, 1) += zClaster.at<double>(i) * (yClaster.at<double>(i) - yMean) / MatSize;
        A.at<double>(2, 2) += zClaster.at<double>(i) * (zClaster.at<double>(i) - zMean) / MatSize;
        b.at<double>(0) += (pow(xClaster.at<double>(i), 2) + pow(yClaster.at<double>(i), 2) + pow(zClaster.at<double>(i), 2)) * (xClaster.at<double>(i) - xMean) / MatSize;
        b.at<double>(1) += (pow(xClaster.at<double>(i), 2) + pow(yClaster.at<double>(i), 2) + pow(zClaster.at<double>(i), 2)) * (yClaster.at<double>(i) - yMean) / MatSize;
        b.at<double>(2) += (pow(xClaster.at<double>(i), 2) + pow(yClaster.at<double>(i), 2) + pow(zClaster.at<double>(i), 2)) * (zClaster.at<double>(i) - zMean) / MatSize;
    }
    A *= 2;

    resX = ADS::MNK(A, b);

    double Radius = 0.0;
    for (int i = 0; i < MatSize; i++)
        Radius += (pow((xClaster.at<double>(i) - resX.at<double>(0)), 2) + pow((yClaster.at<double>(i) - resX.at<double>(1)), 2) + pow((zClaster.at<double>(i) - resX.at<double>(2)), 2)) / MatSize;
    Radius = sqrt(Radius);

    SphereModel[0] = resX.at<double>(0);
    SphereModel[1] = resX.at<double>(1);
    SphereModel[2] = resX.at<double>(2);
    SphereModel[3] = Radius;

    double meanErr = 0.0;
    for (int i = 0; i < MatSize; i++)
        meanErr += abs(sqrt(pow((xClaster.at<double>(i) - resX.at<double>(0)), 2) + pow((yClaster.at<double>(i) - resX.at<double>(1)), 2) + pow((zClaster.at<double>(i) - resX.at<double>(2)), 2)) - Radius);
    meanErr /= MatSize;
    SphereModel[4] = meanErr;

    return SphereModel;
}


vector<double> ADS::FitCylinder(vector<Point3d> clastersData, vector<double> planeModel, vector<Point3d>& cylinderPoints) {
    vector<double> CylinderModel(8, 0.0);    //[xc1, yc1, zc1, xc2, yc2, zc2, radius, err]
    int MatSize = int (clastersData.size());
    cv::Mat xyzClaster = cv::Mat::zeros(3, MatSize, CV_64F);
    for (int i = 0; i < MatSize; i++)
    {
        xyzClaster.at<double>(0, i) = clastersData[i].x;
        xyzClaster.at<double>(1, i) = clastersData[i].y;
        xyzClaster.at<double>(2, i) = clastersData[i].z;
    }
    double yRad = atan2(planeModel[0], 1.0);
    cv::Mat Ry = cv::Mat::eye(3, 3, CV_64F);
    Ry.at<double>(0, 0) = cos(yRad);
    Ry.at<double>(0, 2) = sin(yRad);
    Ry.at<double>(2, 0) = - sin(yRad);
    Ry.at<double>(2, 2) = cos(yRad);
    Mat cylinderRotated0 = Ry * xyzClaster;

    vector<Point3d> planeData;
    Point3d pointXYZ;
    for (int i = 0; i < MatSize; i++)
    {
        pointXYZ.x = cylinderRotated0.at<double>(0, i);
        pointXYZ.y = cylinderRotated0.at<double>(1, i);
        pointXYZ.z = cylinderRotated0.at<double>(2, i);
        planeData.push_back(pointXYZ);
    }
    vector<Point3d> planePoints;
    vector<double> planeModel2 = FitPlane(planeData, planePoints);

    double xRad = - atan2(planeModel2[1], 1.0);
    cv::Mat Rx = cv::Mat::eye(3, 3, CV_64F);
    Rx.at<double>(1, 1) = cos(xRad);
    Rx.at<double>(1, 2) = - sin(xRad);
    Rx.at<double>(2, 1) = sin(xRad);
    Rx.at<double>(2, 2) = cos(xRad);
    Mat cylinderRotated1 = Rx * cylinderRotated0;

    double zRad = CV_PI / 360;

    cv::Mat Rz = cv::Mat::eye(3, 3, CV_64F);
    Rz.at<double>(0, 0) = cos(zRad);
    Rz.at<double>(0, 1) = -sin(zRad);
    Rz.at<double>(1, 0) = sin(zRad);
    Rz.at<double>(1, 1) = cos(zRad);
    vector<vector<double>> rotationResults;
    Mat cylinderRotatedStep = cylinderRotated1.clone();
    for (int i = 0; i < 360; i++) {
        vector<double> rotationStep(7, 0);
        cylinderRotatedStep = Rz * cylinderRotatedStep;
        cv::Mat circleKoef = cv::Mat::zeros(3, 1, CV_64F);
        cv::Mat Ac = cv::Mat::zeros(MatSize, 3, CV_64F);
        cv::Mat b = cv::Mat::zeros(MatSize, 1, CV_64F);
        for (int i = 0; i < MatSize; i++) {
            Ac.at<double>(i, 0) = cylinderRotatedStep.at<double>(0, i);
            Ac.at<double>(i, 1) = cylinderRotatedStep.at<double>(2, i);
            Ac.at<double>(i, 2) = 1;
            b.at<double>(i) = -(pow(cylinderRotatedStep.at<double>(0, i), 2) + pow(cylinderRotatedStep.at<double>(2, i), 2));
        }
        circleKoef = ADS::MNK(Ac, b);
        double xc1 = -circleKoef.at<double>(0) / 2;
        double zc1 = -circleKoef.at<double>(1) / 2;
        double radius1 = sqrt((pow(xc1, 2) + pow(zc1, 2)) - circleKoef.at<double>(2));
        double ymin = ADS::minMatElement(cylinderRotatedStep.row(1));
        double ymax = ADS::maxMatElement(cylinderRotatedStep.row(1));
        double stepRad = zRad * (i + 1);
        double meanErr = 0.0;
        for (int i = 0; i < MatSize; i++)
            meanErr += abs(sqrt(pow((cylinderRotatedStep.at<double>(0, i) - xc1), 2) + pow((cylinderRotatedStep.at<double>(2, i) - zc1), 2)) - radius1);
        meanErr /= MatSize;
        rotationStep[0] = meanErr;
        rotationStep[1] = stepRad;
        rotationStep[2] = xc1;
        rotationStep[3] = zc1;
        rotationStep[4] = ymin;
        rotationStep[5] = ymax;
        rotationStep[6] = radius1;
        rotationResults.push_back(rotationStep);
    }
    double errMin = 1e+8;
    int minErrCylinderIdx = 0;
    for (int i = 0; i < 360; i++)
        if (rotationResults[i][0] < errMin) {
            errMin = rotationResults[i][0];
            minErrCylinderIdx = i;
        }
    Mat osn1 = Mat::zeros(3, 1, CV_64F);
    Mat osn2 = Mat::zeros(3, 1, CV_64F);
    osn1.at<double>(0) = rotationResults[minErrCylinderIdx][2];
    osn1.at<double>(1) = rotationResults[minErrCylinderIdx][4];
    osn1.at<double>(2) = rotationResults[minErrCylinderIdx][3];
    osn2.at<double>(0) = rotationResults[minErrCylinderIdx][2];
    osn2.at<double>(1) = rotationResults[minErrCylinderIdx][5];
    osn2.at<double>(2) = rotationResults[minErrCylinderIdx][3];

    cv::Mat Rxb = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat Ryb = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat Rzb = cv::Mat::eye(3, 3, CV_64F);
    Rxb.at<double>(1, 1) = cos(-xRad);
    Rxb.at<double>(1, 2) = -sin(-xRad);
    Rxb.at<double>(2, 1) = sin(-xRad);
    Rxb.at<double>(2, 2) = cos(-xRad);
    Ryb.at<double>(0, 0) = cos(-yRad);
    Ryb.at<double>(0, 2) = sin(-yRad);
    Ryb.at<double>(2, 0) = -sin(-yRad);
    Ryb.at<double>(2, 2) = cos(-yRad);
    Rzb.at<double>(0, 0) = cos(-rotationResults[minErrCylinderIdx][1]);
    Rzb.at<double>(0, 1) = -sin(-rotationResults[minErrCylinderIdx][1]);
    Rzb.at<double>(1, 0) = sin(-rotationResults[minErrCylinderIdx][1]);
    Rzb.at<double>(1, 1) = cos(-rotationResults[minErrCylinderIdx][1]);
    osn1 = Ryb * Rxb * Rzb * osn1;
    osn2 = Ryb * Rxb * Rzb * osn2;
    CylinderModel[0] = osn1.at<double>(0);
    CylinderModel[1] = osn1.at<double>(1);
    CylinderModel[2] = osn1.at<double>(2);
    CylinderModel[3] = osn2.at<double>(0);
    CylinderModel[4] = osn2.at<double>(1);
    CylinderModel[5] = osn2.at<double>(2);
    CylinderModel[6] = rotationResults[minErrCylinderIdx][6];
    CylinderModel[7] = rotationResults[minErrCylinderIdx][0];

    Mat pPoints = Mat::zeros(3, 22, CV_64F);
    for (int i = 0; i <= 10; i++) {
        pPoints.at<double>(0, 2 * i) = rotationResults[minErrCylinderIdx][2] + rotationResults[minErrCylinderIdx][6] * cos(i * CV_PI / 5);
        pPoints.at<double>(1, 2 * i) = rotationResults[minErrCylinderIdx][4];
        pPoints.at<double>(2, 2 * i) = rotationResults[minErrCylinderIdx][3] + rotationResults[minErrCylinderIdx][6] * sin(i * CV_PI / 5);
        pPoints.at<double>(0, 2 * i + 1) = rotationResults[minErrCylinderIdx][2] + rotationResults[minErrCylinderIdx][6] * cos(i * CV_PI / 5);
        pPoints.at<double>(1, 2 * i + 1) = rotationResults[minErrCylinderIdx][5];
        pPoints.at<double>(2, 2 * i + 1) = rotationResults[minErrCylinderIdx][3] + rotationResults[minErrCylinderIdx][6] * sin(i * CV_PI / 5);
    }
    pPoints = Ryb * Rxb * Rzb * pPoints;
    for (int i = 0; i < 22; i++) {
        Point3d pp;
        pp.x = pPoints.at<double>(0, i);
        pp.y = pPoints.at<double>(1, i);
        pp.z = pPoints.at<double>(2, i);
        cylinderPoints.push_back(pp);
    }

    return CylinderModel;
}





Mat ADS::MNK(Mat A, Mat b) {
    Mat resX = cv::Mat::zeros(3, 1, CV_64F);
    Mat At, Ainv, Aleft;
    cv::transpose(A, At);
    Ainv = At * A;
    Ainv = Ainv.inv();
    Aleft = Ainv * At;
    resX = Aleft * b;
    return resX;
}


double ADS::minMatElement(Mat data) {
    double temp = 1e+8;
    for (int i = 0; i < data.cols; i++)
        if (data.at<double>(i) < temp)
            temp = data.at<double>(i);
    return temp;
}
double ADS::maxMatElement(Mat data) {
    double temp = -1e+8;
    for (int i = 0; i < data.cols; i++)
        if (data.at<double>(i) > temp)
            temp = data.at<double>(i);
    return temp;
}



QImage Mat2QImage(cv::Mat const& src)
{
    cv::Mat temp; // make the same cv::Mat
    cvtColor(src, temp, COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); // enforce deep copy, see documentation

    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}

cv::Mat QImage2Mat(QImage const& src)
{
    cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    cvtColor(tmp, result,COLOR_BGR2RGB);
    return result;
}


QPixmap QImage2QPixmap( QImage const& src )
{
    return QPixmap::fromImage( src );
}


bool FindPoint(double PoitsF[4][4], double x,double y)
{
    double d,xNew,yNew;
    double x1,x2,x3,x4,y1,y2,y3,y4;
    x1=PoitsF[0][0];y1=PoitsF[0][1];
    x2=PoitsF[1][0];y2=PoitsF[1][1];
    x3=PoitsF[2][0];y3=PoitsF[2][1];
    x4=PoitsF[3][0];y4=PoitsF[3][1];
    // формула поиска точки внутри прямоугольника
    d=((x3-x2)*(y1-y2))-((x1-x2)*(y3-y2));
    xNew=(((y1-y2)*(x-x2))-((x1-x2)*(y-y2)))/d;
    yNew=((-(y3-y2)*(x-x2))+((x3-x2)*(y-y2)))/d;
    // cout <<"x= "<< xNew << " y= "<< yNew << "\n";

    if (xNew>=0 && xNew<=1 && yNew>=0 && yNew<=1)
        return true;
    else
        return false;
}






//===================================
// Функции и класс для определения габоритов объектов (начало)
//===================================

ADS::Point3D::Point3D(double x, double y, double z, std::string name)
{
    X = x;
    Y = y;
    Z = z;
    Name = name;
}

// Метод для установки кол-ва точек
void ADS::Point3D::setNumberOfPoint(int numberofpoints)
{
    NumberOfPoint = numberofpoints;
}

// Переопределяем оператор для возможности сортировки
bool ADS::Point3D::operator< (const Point3D& p)
{
    return NumberOfPoint < p.NumberOfPoint;
}





    // Формула Расстояния между двумя точками 3D
double ADS::get_distance(ADS::Point3D p1, ADS::Point3D p2)
{
    return std::sqrt((p2.X - p1.X) * (p2.X - p1.X) + (p2.Y - p1.Y) * (p2.Y - p1.Y) + (p2.Z - p1.Z) * (p2.Z - p1.Z));
}
// =====================
double get_sum_distance(std::vector<double> X,
                        std::vector<double> Y,
                        std::vector<double> Z,
                        ADS::Point3D MN, ADS::Point3D M0)
{
    double max_dist = ADS::get_distance(MN, M0);
    std::vector<double> distances;
    for (size_t i = 0; i < X.size(); i++) {
        double dist = ADS::get_distance(MN, ADS::Point3D(X[i], Y[i], Z[i], "M"));
        if (dist < max_dist / 2)
            distances.push_back(1);
    }
    return std::accumulate(distances.begin(), distances.end(), 0);
}
// =====================
size_t ADS::get_sum_count(std::vector<double> X,
                     std::vector<double> Y,
                     std::vector<double> Z,
                     ADS::Point3D MN, ADS::Point3D M0) {
    double max_dist = ADS::get_distance(MN, M0);
    size_t N = 0;
    for (size_t i = 0; i < X.size(); i++) {
        double dist = ADS::get_distance(MN, ADS::Point3D(X[i], Y[i], Z[i], "M"));
        if (dist < max_dist / 2)
            N += 1;
    }
    return N;
}
// =====================
std::vector<double> ADS::sum(std::vector<double> a, std::vector<double> b) {
    std::vector<double> res = a;
    for (size_t i = 0; i < a.size(); i++) {
        res[i] += b[i];
    }
    return res;
}
// =====================
std::vector<double> ADS::substr(std::vector<double> a, std::vector<double> b) {
    std::vector<double> res = a;
    for (size_t i = 0; i < a.size(); i++) {
        res[i] -= b[i];
    }
    return res;
}
// =====================
std::vector<double> ADS::cross(std::vector<double> a, std::vector<double> b) {
    double x1 = a[1] * b[2] - a[2] * b[1];
    double y1 = a[2] * b[0] - a[0] * b[2];
    double z1 = a[0] * b[1] - a[1] * b[0];
    std::vector<double> res = { x1, y1, z1 };
    return res;
}
// =====================
std::vector<double> ADS::np_divide(std::vector<double> a, std::vector<double> b) {
    std::vector<double> res = a;
    for (size_t i = 0; i < a.size(); i++) {
        res[i] /= b[i];
    }
    return res;
}
// =====================
double ADS::np_linalg_norm(std::vector<double> a) {
    double res = 0;
    for (size_t i = 0; i < a.size(); i++) {
        res += std::pow(a[i], 2);
    }
    return std::sqrt(std::abs(res));
}
// =====================
int np_dot(std::vector<double> v1, std::vector<double> v2) {
    double product = 0;
    for (size_t i = 0; i < v1.size(); i++)
        product += v1[i] * v2[i];
    return product;
}
// нахождение самого длинного перпендикуляра к прямой
double ADS::lineseg_dist(std::vector<double> p, std::vector<double> a, std::vector<double> b) {
    std::vector<double> AB = substr(b, a);
    std::vector<double> AC = substr(p, a);
    double area = np_linalg_norm(cross(AB, AC));
    double CD = area / np_linalg_norm(AB);
    return CD;
}
// Нахождение минимальных и максимальных элементов в массивах
void ADS::get_sizes(std::vector<double> X, std::vector<double> Y, std::vector<double> Z, double* L, double* W, double* H, double* Length, double* Width)
{
    double Xmin = *min_element(X.begin(), X.end());
    double Xmax = *max_element(X.begin(), X.end());

    double Ymin = *min_element(Y.begin(), Y.end());
    double Ymax = *max_element(Y.begin(), Y.end());

    double Zmin = *min_element(Z.begin(), Z.end());
    double Zmax = *max_element(Z.begin(), Z.end());

    double X0 = Xmin + (Xmax - Xmin) / 2.0;
    double Y0 = Ymin + (Ymax - Ymin) / 2.0;
    double Z0 = Zmin + (Zmax - Zmin) / 2.0;

    // Построение параллелепипеда

    ADS::Point3D M0 = ADS::Point3D(X0, Y0, Z0, "M0");
    ADS::Point3D M1 = ADS::Point3D(Xmin, Ymin, Zmin, "M1");
    ADS::Point3D M2 = ADS::Point3D(Xmax, Ymin, Zmin, "M2");
    ADS::Point3D M3 = ADS::Point3D(Xmax, Ymax, Zmin, "M3");
    ADS::Point3D M4 = ADS::Point3D(Xmin, Ymax, Zmin, "M4");
    ADS::Point3D M5 = ADS::Point3D(Xmin, Ymin, Zmax, "M5");
    ADS::Point3D M6 = ADS::Point3D(Xmax, Ymin, Zmax, "M6");
    ADS::Point3D M7 = ADS::Point3D(Xmax, Ymax, Zmax, "M7");
    ADS::Point3D M8 = ADS::Point3D(Xmin, Ymax, Zmax, "M8");

    size_t m1 = ADS::get_sum_count(X, Y, Z, M1, M0);
    size_t m2 = ADS::get_sum_count(X, Y, Z, M2, M0);
    size_t m3 = ADS::get_sum_count(X, Y, Z, M3, M0);
    size_t m4 = ADS::get_sum_count(X, Y, Z, M4, M0);
    size_t m5 = ADS::get_sum_count(X, Y, Z, M5, M0);
    size_t m6 = ADS::get_sum_count(X, Y, Z, M6, M0);
    size_t m7 = ADS::get_sum_count(X, Y, Z, M7, M0);
    size_t m8 = ADS::get_sum_count(X, Y, Z, M8, M0);

#pragma region Расчет осевых точек
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
    std::list<ADS::Point3D> M;
    M.push_back(M1);
    M.push_back(M2);
    M.push_back(M3);
    M.push_back(M4);
    M.push_back(M5);
    M.push_back(M6);
    M.push_back(M7);
    M.push_back(M8);

    // Сортировка
    M.sort();
    M.reverse();

    std::cout << std::endl;
    // Формируем список осевых точек
    std::list<ADS::Point3D> P;
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
        m0_dist = ADS::get_distance(*it, M0);  // Расстояние от текущей точки до центра масс
        p0_dist = ADS::get_distance(*it, P0);  // Расстояние между точками P0 и текущей

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






#pragma endregion

    // Вычисление длины
    double length = sqrt(pow(P1.X - P0.X, 2) +
                         pow(P1.Y - P0.Y, 2) +
                         pow(P1.Z - P0.Z, 2));

    // Вычисление ширины
    std::vector<double> dists;
    for (size_t i = 0; i < X.size(); i++) {
        std::vector<double> a = { P0.X, P0.Y, P0.Z };
        std::vector<double> b = { P1.X, P1.Y, P1.Z };
        std::vector<double> p = { X[i], Y[i], Z[i] };
        dists.push_back(lineseg_dist(p, a, b));
    }
    double max_dist = *max_element(dists.begin(), dists.end()) * 2;
     // Вывод габаритов параллелепипеда, где (L-длина W-ширина H-высота параллелепипид и габариты объекты: Length-высота, Width-ширина)
    *L = ADS::get_distance(M1, M2);
    *W = ADS::get_distance(M2, M3);
    *H = ADS::get_distance(M3, M7);

    *Length = length;
    *Width = max_dist;
}


//===================================
// Функции и класс для определения габоритов объектов (конец)
//===================================
//===================================
//===================================





//===================================
// Экспоненциальное бегущее среднее
//===================================

// бегущее среднее
float ADS::expRunningAverage(float newVal, int q)
{
    float k = 0.005; // коэффициент фильтрации, 0.0-1.0
    static float filVal[40];
    filVal[q] += (newVal - filVal[q]) * k;
    return filVal[q];
}








//===================================
//  DarkNet-test-simple (начало)
//===================================




ADS::NeuralNetDetector::NeuralNetDetector(const std::string model, const std::string classes) {
    if (!init_network(model, classes)) {
        std::cout << "Neural network been inited!" << std::endl;
        std::cout << "Input width: " << input_width << std::endl;
        std::cout << "Input height: " << input_height << std::endl;
    } else {
        std::cerr << "Failed to init neural network!" << std::endl;
    }
}

ADS::NeuralNetDetector::NeuralNetDetector(const std::string model, const std::string classes,
                                     int width, int height) {
    input_width = width;
    input_height = height;
    if (!init_network(model, classes)) {
        std::cout << "Neural network been inited!" << std::endl;
        std::cout << "Input width: " << input_width << std::endl;
        std::cout << "Input height: " << input_height << std::endl;
    } else {
        std::cerr << "Failed to init neural network!" << std::endl;
    }
}

errno_t ADS::NeuralNetDetector::read_classes(const std::string file_path) {
    std::ifstream classes_file(file_path);
    std::string line;

    if (!classes_file) {
        std::cerr << "Failed to open classes names!\n";
        return ENOENT;
    }
    while (std::getline(classes_file, line)) {
        classes.push_back(line);
    }
    classes_file.close();

    return 0;
}

errno_t ADS::NeuralNetDetector::init_network(const std::string model_path,
                                        const std::string classes_path) {
    errno_t err = read_classes(classes_path);

    if (err == 0) {
        std::cout << model_path << std::endl;
        ADS::NeuralNetDetector::network = cv::dnn::readNet(model_path);
        if (network.empty()) {
            return ENETDOWN;
        } else {
            network.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
            network.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }
    }

    return err;
}

// Draw the predicted bounding box.
void ADS::NeuralNetDetector::draw_label(cv::Mat& img, std::string label, int left, int top) {
    // Display the label at the top of the bounding box.
    int baseline;
    cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX,
                                          FONT_SCALE, THICKNESS, &baseline);
    top = std::max(top, label_size.height);
    // Top left corner.
    cv::Point tlc = cv::Point(left, top);
    // Bottom right corner.
    cv::Point brc = cv::Point(left + label_size.width,
                              top + label_size.height + baseline);
    // Draw black rectangle.
    cv::rectangle(img, tlc, brc, BLACK, cv::FILLED);
    // Put the label on the black rectangle.
    cv::putText(img, label, cv::Point(left, top + label_size.height),
                cv::FONT_HERSHEY_SIMPLEX, FONT_SCALE, YELLOW, THICKNESS);
}

std::vector<cv::Mat> ADS::NeuralNetDetector::pre_process(cv::Mat &img, cv::dnn::Net &net) {
    // Convert to blob.
    cv::Mat blob;
    cv::dnn::blobFromImage(img, blob, 1.0 / 255.0, cv::Size(input_width, input_height),
                           cv::Scalar(), true, false);
    net.setInput(blob);
    // Forward propagate.
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());
    return outputs;
}

cv::Mat ADS::NeuralNetDetector::post_process(cv::Mat &img, std::vector<cv::Mat> &outputs,
                                        const std::vector<std::string> &class_name) {
    // Initialize vectors to hold respective outputs while unwrapping detections.
    // TODO Очистка векторов
    cv::Mat ret = img.clone(); // тормозит на 0.1 сек
    classes_id_set.clear();
    confidences_set.clear();
    boxes_set.clear();
    classes_set.clear();
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    // Resizing factor.
    float x_factor = img.cols / (float)input_width;
    float y_factor = img.rows / (float)input_height;

    float *data = (float *)outputs[0].data;

    const int dimensions = class_name.size() + 5;
    const int rows = 25200;
    // Iterate through 25200 detections.
    for (int i = 0; i < rows; ++i) {
        float confidence = data[4];
        // Discard bad detections and continue.
        if (confidence >= CONFIDENCE_THRESHOLD) {
            float * classes_scores = data + 5;
            // Create a 1x85 Mat and store class scores of 80 classes.
            cv::Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
            // Perform minMaxLoc and acquire index of best class score.
            cv::Point class_id;
            double max_class_score;
            cv::minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            // Continue if the class score is above the threshold.
            if (max_class_score > SCORE_THRESHOLD) {
                // Store class ID and confidence in the pre-defined respective vectors.
                confidences.push_back(confidence);
                class_ids.push_back(class_id.x);
                // Center.
                float cx = data[0];
                float cy = data[1];
                // Box dimension.
                float w = data[2];
                float h = data[3];
                // Bounding box coordinates.
                int left = int((cx - 0.5 * w) * x_factor);
                int top = int((cy - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);
                // Store good detections in the boxes vector.
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
        // Jump to the next column.
        data += dimensions;
    }

    // Perform Non Maximum Suppression and draw predictions.
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
    for (int i = 0; i < indices.size(); i++) {
        int idx = indices[i];
        cv::Rect box = boxes[idx];

        boxes_set.push_back(box);
        confidences_set.push_back(confidences[idx]);
        classes_id_set.push_back(class_ids[idx]);
        classes_set.push_back(class_name[class_ids[idx]]);

        int left = box.x;
        int top = box.y;
        int width = box.width;
        int height = box.height;
        // Draw bounding box.
        cv::rectangle(ret, cv::Point(left, top), cv::Point(left + width, top + height), GREEN, 3*THICKNESS);
        // Get the label for the class name and its confidence.
        std::string label = cv::format("%.2f", confidences[idx]);
        label = class_name[class_ids[idx]] + ": " + label;
        // Draw class labels.
        draw_label(ret, label, left, top);
    }

    processed_image = ret; // TODO

    return ret;
}

cv::Mat ADS::NeuralNetDetector::process(cv::Mat &img) {
    std::vector<cv::Mat> detections;
    detections = pre_process(img, network);
    cv::Mat res = post_process(img, detections, NeuralNetDetector::classes);
    // Put efficiency information.
    // The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
    std::vector<double> layersTimes;
    double freq = cv::getTickFrequency();
    NeuralNetDetector::inference_time = network.getPerfProfile(layersTimes) / freq;
    return res;
}

std::string ADS::NeuralNetDetector::get_info(void) {
    std::string str = "";
    for (int i = 0; i < classes_id_set.size(); i++) {
        str += classes_set[i];
        str += ": ";
        str += std::to_string(confidences_set[i]);
        str += "\n";
    }
    return str;
}

cv::Mat ADS::NeuralNetDetector::get_image(void) {
    return processed_image;
}


//===================================
//  DarkNet-test-simple (конец)
//===================================
//===================================



ADS::NeuralNetSegmentator::NeuralNetSegmentator(const std::string model, const std::string classes,
                                           int width = INPUT_WIDTH, int height = INPUT_HEIGHT) {
    input_width = width;
    input_height = height;
    if (!init_network(model, classes)) {
        std::cout << "Neural network been inited!" << std::endl;
        std::cout << "Input width: " << input_width << std::endl;
        std::cout << "Input height: " << input_height << std::endl;
    } else {
        std::cerr << "Failed to init neural network!" << std::endl;
    }
}

int ADS::NeuralNetSegmentator::read_classes(const std::string file_path) {
    std::ifstream classes_file(file_path);
    std::string line;
    srand(time(0));

    if (!classes_file) {
        std::cerr << "Failed to open classes names!\n";
        return ENOENT;
    }
    while (std::getline(classes_file, line)) {
        classes.push_back(line);
        masks_colors_set.push_back(cv::Scalar(rand() % 256, rand() % 256, rand() % 256));
    }
    classes_file.close();

    return 0;
}

int ADS::NeuralNetSegmentator::init_network(const std::string model_path,
                                           const std::string classes_path) {
    int err = read_classes(classes_path);

    if (err == 0) {
        network = cv::dnn::readNetFromONNX(model_path);
        if (network.empty()) {
            return ENETDOWN;
        } else {
            network.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
            network.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }
    }

    return err;
}

void ADS::NeuralNetSegmentator::letter_box(const cv::Mat& img, cv::Mat& out,
                                      cv::Vec4d& params,
                                      const cv::Size& newShape = cv::Size(INPUT_WIDTH, INPUT_HEIGHT),
                                      bool autoShape = false, bool scaleFill = false,
                                      bool scaleUp = true, int stride = 32) {
    cv::Size shape = img.size();
    float r = std::min((float)newShape.height / (float)shape.height, (float)newShape.width / (float)shape.width);
    if (!scaleUp) {
        r = std::min(r, 1.0f);
    }

    float ratio[2] = { r, r };
    int new_un_pad[2] = { (int)std::round((float)shape.width * r),
                         (int)std::round((float)shape.height * r) };
    auto dw = (float)(newShape.width - new_un_pad[0]);
    auto dh = (float)(newShape.height - new_un_pad[1]);

    if (autoShape) {
        dw = (float)((int)dw % stride);
        dh = (float)((int)dh % stride);
    } else if (scaleFill) {
        dw = 0.0f;
        dh = 0.0f;
        new_un_pad[0] = newShape.width;
        new_un_pad[1] = newShape.height;
        ratio[0] = (float)newShape.width / (float)shape.width;
        ratio[1] = (float)newShape.height / (float)shape.height;
    }

    if (shape.width != new_un_pad[0] && shape.height != new_un_pad[1]) {
        cv::resize(img, out, cv::Size(new_un_pad[0], new_un_pad[1]));
    } else {
        out = img.clone();
    }

    dw /= 2.0f;
    dh /= 2.0f;
    int top = int(std::round(dh - 0.1f));
    int bottom = int(std::round(dh + 0.1f));
    int left = int(std::round(dw - 0.1f));
    int right = int(std::round(dw + 0.1f));
    params[0] = ratio[0];
    params[1] = ratio[1];
    params[2] = left;
    params[3] = top;
    cv::copyMakeBorder(out, out, top, bottom, left, right, cv::BORDER_CONSTANT, BLACK);
}

std::vector<cv::Mat> ADS::NeuralNetSegmentator::pre_process(cv::Mat& img, cv::Vec4d& params) {
    cv::Mat input;
    cv::Mat blob;
    letter_box(img, input, params, cv::Size(INPUT_WIDTH, INPUT_HEIGHT));
    cv::dnn::blobFromImage(input, blob, 1.0 / 255.0, cv::Size(INPUT_WIDTH, INPUT_HEIGHT),
                           cv::Scalar(), true, false);
    network.setInput(blob);
    std::vector<std::string> output_layer_names{ "output0", "output1" };
    std::vector<cv::Mat> outputs;
    network.forward(outputs, output_layer_names);
    return outputs;
}

void ADS::NeuralNetSegmentator::get_mask(const cv::Mat& mask_proposals, const cv::Mat& mask_protos,
                                    OutputSeg& output, const MaskParams& maskParams) {
    int seg_channels = maskParams.segChannels;
    int net_width = maskParams.netWidth;
    int seg_width = maskParams.segWidth;
    int net_height = maskParams.netHeight;
    int seg_height = maskParams.segHeight;
    float mask_threshold = maskParams.maskThreshold;
    cv::Vec4f params = maskParams.params;
    cv::Size src_img_shape = maskParams.srcImgShape;
    cv::Rect temp_rect = output.box;

    //crop from mask_protos
    int rang_x = floor((temp_rect.x * params[0] + params[2]) / net_width * seg_width);
    int rang_y = floor((temp_rect.y * params[1] + params[3]) / net_height * seg_height);
    int rang_w = ceil(((temp_rect.x + temp_rect.width) * params[0] + params[2]) / net_width * seg_width) - rang_x;
    int rang_h = ceil(((temp_rect.y + temp_rect.height) * params[1] + params[3]) / net_height * seg_height) - rang_y;

    rang_w = MAX(rang_w, 1);
    rang_h = MAX(rang_h, 1);
    if (rang_x + rang_w > seg_width) {
        if (seg_width - rang_x > 0) {
            rang_w = seg_width - rang_x;
        } else {
            rang_x -= 1;
        }
    }
    if (rang_y + rang_h > seg_height) {
        if (seg_height - rang_y > 0) {
            rang_h = seg_height - rang_y;
        } else {
            rang_y -= 1;
        }
    }

    std::vector<cv::Range> roi_rangs;
    roi_rangs.push_back(cv::Range(0, 1));
    roi_rangs.push_back(cv::Range::all());
    roi_rangs.push_back(cv::Range(rang_y, rang_h + rang_y));
    roi_rangs.push_back(cv::Range(rang_x, rang_w + rang_x));

    //crop
    cv::Mat temp_mask_protos = mask_protos(roi_rangs).clone();
    cv::Mat protos = temp_mask_protos.reshape(0, { seg_channels,rang_w * rang_h });
    cv::Mat matmul_res = (mask_proposals * protos).t();
    cv::Mat masks_feature = matmul_res.reshape(1, { rang_h,rang_w });
    cv::Mat dest, mask;

    //sigmoid
    cv::exp(-masks_feature, dest);
    dest = 1.0 / (1.0 + dest);

    int left = floor((net_width / seg_width * rang_x - params[2]) / params[0]);
    int top = floor((net_height / seg_height * rang_y - params[3]) / params[1]);
    int width = ceil(net_width / seg_width * rang_w / params[0]);
    int height = ceil(net_height / seg_height * rang_h / params[1]);

    cv::resize(dest, mask, cv::Size(width, height), cv::INTER_NEAREST);
    mask = mask(temp_rect - cv::Point(left, top)) > mask_threshold;
    output.boxMask = mask;
}

// Draw the predicted bounding box.
void ADS::NeuralNetSegmentator::draw_label(cv::Mat& img, std::string label, int left, int top) {
    // Display the label at the top of the bounding box.
    int baseline;
    cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX,
                                          FONT_SCALE, THICKNESS, &baseline);
    top = std::max(top, label_size.height);
    // Top left corner.
    cv::Point tlc = cv::Point(left, top);
    // Bottom right corner.
    cv::Point brc = cv::Point(left + label_size.width,
                              top + label_size.height + baseline);
    // Draw black rectangle.
    cv::rectangle(img, tlc, brc, BLACK, cv::FILLED);
    // Put the label on the black rectangle.
    cv::putText(img, label, cv::Point(left, top + label_size.height),
                cv::FONT_HERSHEY_SIMPLEX, FONT_SCALE, YELLOW, THICKNESS);
}

void ADS::NeuralNetSegmentator::draw_result(cv::Mat& img, std::vector<OutputSeg> result, std::vector<std::string> class_name) {
    cv::Mat mask = img.clone();
    for (int i = 0; i < result.size(); i++) {
        cv::rectangle(img, result[i].box, GREEN, 3*THICKNESS);
        mask(result[i].box).setTo(masks_colors_set[result[i].id], result[i].boxMask);
        std::string label = cv::format("%.2f", result[i].confidence);
        label = class_name[result[i].id] + ": " + label;
        int left = result[i].box.x;
        int top = result[i].box.y;
        draw_label(img, label, left, top);
    }
    cv::addWeighted(img, 0.5, mask, 0.5, 0, img);
}

cv::Mat ADS::NeuralNetSegmentator::post_process(cv::Mat &img, std::vector<cv::Mat> &outputs,
                                           const std::vector<std::string> &class_name,
                                           cv::Vec4d& params) {
    classes_id_set.clear();
    confidences_set.clear();
    boxes_set.clear();
    classes_set.clear();
    masks_set.clear();
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    std::vector<std::vector<float>> picked_proposals;

    float* data = (float*)outputs[0].data;

    const int dimensions = class_name.size() + 5 + 32;
    const int rows = 25200;
    for (int i = 0; i < rows; ++i) {
        float confidence = data[4];
        if (confidence >= CONFIDENCE_THRESHOLD) {
            float* classes_scores = data + 5;
            cv::Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double max_class_score;
            cv::minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
            if (max_class_score > SCORE_THRESHOLD) {
                float x = (data[0] - params[2]) / params[0];
                float y = (data[1] - params[3]) / params[1];
                float w = data[2] / params[0];
                float h = data[3] / params[1];
                int left = std::max(int(x - 0.5 * w), 0);
                int top = std::max(int(y - 0.5 * h), 0);
                int width = int(w);
                int height = int(h);
                boxes.push_back(cv::Rect(left, top, width, height));
                confidences.push_back(confidence);
                class_ids.push_back(class_id.x);

                std::vector<float> temp_proto(data + class_name.size() + 5, data + dimensions);
                picked_proposals.push_back(temp_proto);
            }
        }
        data += dimensions;
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
    std::vector<OutputSeg> output;
    std::vector<std::vector<float>> temp_mask_proposals;
    cv::Rect holeImgRect(0, 0, img.cols, img.rows);
    for (int i = 0; i < indices.size(); ++i) {
        int idx = indices[i];
        cv::Rect box = boxes[idx];

        boxes_set.push_back(box);
        confidences_set.push_back(confidences[idx]);
        classes_id_set.push_back(class_ids[idx]);
        classes_set.push_back(class_name[class_ids[idx]]);

        OutputSeg result;
        result.id = class_ids[idx];
        result.confidence = confidences[idx];
        result.box = boxes[idx] & holeImgRect;
        temp_mask_proposals.push_back(picked_proposals[idx]);
        output.push_back(result);
    }

    MaskParams mask_params;
    mask_params.params = params;
    mask_params.srcImgShape = img.size();
    for (int i = 0; i < temp_mask_proposals.size(); ++i) {
        get_mask(cv::Mat(temp_mask_proposals[i]).t(), outputs[1], output[i], mask_params);
        masks_set.push_back(output[i].boxMask);
    }

    draw_result(img, output, class_name);

    return img;
}

cv::Mat ADS::NeuralNetSegmentator::process(cv::Mat &img) {
    cv::Mat input = img.clone();
    std::vector<cv::Mat> detections;
    cv::Vec4d params;
    detections = pre_process(input, params);
    cv::Mat res = post_process(input, detections, NeuralNetSegmentator::classes, params);
    // Put efficiency information.
    // The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
    std::vector<double> layersTimes;
    double freq = cv::getTickFrequency();
    NeuralNetSegmentator::inference_time = network.getPerfProfile(layersTimes) / freq;
    processed_image = res;

    return res;
}

std::string ADS::NeuralNetSegmentator::get_info(void) {
    std::string str = "";
    for (int i = 0; i < classes_id_set.size(); i++) {
        str += classes_set[i];
        str += ": ";
        str += std::to_string(confidences_set[i]);
        str += "\n";
    }
    return str;
}




