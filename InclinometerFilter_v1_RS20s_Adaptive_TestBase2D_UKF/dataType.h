#pragma once

#include "Gaussian.h"
#include <iostream>
#include <Eigen/Dense>

using  namespace std;

class dataType {

public:
    struct {
        Gaussian filterData;
        double normRersidual;
    } sensorData;

    struct {
        double filterData = 0;
        double normRersidual = 0;
    } sensorData2D;

    struct {
        double filterData = 0;
        double gx = 0;
        double gy = 0;
        double normRersidual = 0;
    } sensorDataGxy;

   /* struct {
        vector <vector<double>> F;
    } predictData;

    struct {
        vector <vector<double>> F;
    } updateData;*/


};

