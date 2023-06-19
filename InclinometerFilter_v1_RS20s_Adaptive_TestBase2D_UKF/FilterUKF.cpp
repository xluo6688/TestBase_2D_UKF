#include "FilterUKF.h"
#include <vector>
#include "dataType.h"
#include <Eigen/Dense>
#include <iostream>
#include <cmath>

void FilterUKF::predict_UKF(
    const Eigen::MatrixXd sigmaPoints,
    const Eigen::VectorXd weights,
    Eigen::VectorXd& x_hat,
    Eigen::MatrixXd& P_hat,
    const Eigen::MatrixXd Q)
{
    x_hat = weights(0) * sigmaPoints.col(0);
    for (int i = 1; i < sigmaPoints.cols(); i++) {
        x_hat += weights(2) * sigmaPoints.col(i);
    }
    
    P_hat = weights(1) * (sigmaPoints.col(0) - x_hat) * (sigmaPoints.col(0) - x_hat).transpose();
    for (int i = 1; i < sigmaPoints.cols(); i++) {
        P_hat += weights(2) * (sigmaPoints.col(i) - x_hat) * (sigmaPoints.col(i) - x_hat).transpose();
    }

    P_hat += Q;
}

void FilterUKF::update_UKF(
    const Eigen::MatrixXd sPoints,
    const Eigen::MatrixXd ZZ,
    const Eigen::VectorXd z,
    const Eigen::VectorXd weights,
    const Eigen::VectorXd x_hat,
    const Eigen::MatrixXd P_hat,
    Eigen::VectorXd& x,
    Eigen::MatrixXd& P,
    const Eigen::MatrixXd R,
    double& c_var)
{
    Eigen::VectorXd mean_z;
    Eigen::VectorXd y;
    Eigen::MatrixXd Pz;
    Eigen::MatrixXd K;
    Eigen::MatrixXd Pz_inv;  
    Eigen::VectorXd c_var_t;

    mean_z = weights(0) * ZZ.col(0);
    for (int i = 1; i < ZZ.cols(); i++) {
        mean_z += weights(2) * ZZ.col(i);
    }

    y = z - mean_z;

    Pz = weights(1) * (ZZ.col(0) - mean_z) * (ZZ.col(0) - mean_z).transpose();
    for (int i = 1; i < ZZ.cols(); i++) {
        Pz += weights(2) * (ZZ.col(i) - mean_z) * (ZZ.col(i) - mean_z).transpose();
    }

    Pz += R;
    Pz_inv = Pz.inverse();
    c_var_t = y.transpose() * Pz_inv * y;
    c_var = c_var_t[0]; 

    K = weights(1) * (sPoints.col(0) - x_hat) * (ZZ.col(0) - mean_z).transpose();
    for (int i = 1; i < ZZ.cols(); i++) {
        K += weights(2) * (sPoints.col(i) - x_hat) * (ZZ.col(i) - mean_z).transpose();
    }

    K *= Pz.inverse();        
    x = x_hat + K * y;  
    P = P_hat - K * Pz * K.transpose();
}

vector<dataType> FilterUKF::filterUKF(vector<dataType> data) {

    vector<dataType> filteredData;
    dataType outputData;
    int stateDim = 4;
    int sensorDim = 1;
    Eigen::VectorXd mean(stateDim, 1);
    Eigen::VectorXd x(stateDim, 1);
    Eigen::VectorXd x_hat(stateDim, 1);
    Eigen::MatrixXd var(stateDim, stateDim);
    Eigen::MatrixXd P(stateDim, stateDim);
    Eigen::MatrixXd P_hat(stateDim, stateDim);
    Eigen::MatrixXd A(stateDim, stateDim);
    Eigen::MatrixXd ZZ(sensorDim, 2*stateDim + 1);
    double angle;

    float dt = 0.05;
 
    float alpha = 0.1;
    int k = -1;

    float beta = 2.0;
    Eigen::MatrixXd sPoints(stateDim, 2*stateDim + 1);
    Eigen::VectorXd weights(3, 1);
    Eigen::MatrixXd sigmaPoints_y(stateDim, 2* stateDim + 1);
    Eigen::MatrixXd Q(stateDim, stateDim);
    double process_std = 3.5;
    double measurement_std = 2;
    
    Eigen::MatrixXd R(sensorDim, sensorDim);
    Eigen::VectorXd z(sensorDim);
    double normRersidual;
    double process_var;
    double c_var;

    mean << data[0].sensorDataGxy.gx, 0, data[0].sensorDataGxy.gy, 0;
    /*var << 5, 1, 2, 1,
           1, 2, 1, 1,
           2, 1, 5, 1,
           1, 1, 1, 2;*/

    var << 20, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 20, 0,
           0, 0, 0, 0;

    A << 1, dt, 0, 0,
         0, 1, 0, 0,
         0, 0, 1, dt,
         0, 0, 0, 1;

    R << pow(measurement_std, 2);

    sigmaPoints sigmaPointsUKF;

   /* cout << " UKF start .... " << endl;
    sigmaPointsUKF.createSigmaPoints(mean, var, alpha, k, beta, sPoints, weights);
    cout << "sigmaPoints = " << sPoints << endl;
    cout << "weight = " << weights << endl;*/

    sigmaPoints_y = A * sPoints;

    for (int i = 0; i < (int)data.size(); i++)
    {
        process_var = pow(process_std, 2);

        //Q << 0.25 * pow(dt, 4) * process_var, 0.5 * pow(dt, 3) * process_var, -0.1 * pow(dt, 4), -0.1 * pow(dt, 2),
        //    0.5 * pow(dt, 3) * process_var, pow(dt, 2)* process_var, -0.1 * pow(dt, 2), -0.1 * pow(dt, 4),
        //    -0.1 * pow(dt, 4), -0.1 * pow(dt, 2), 0.25 * pow(dt, 4) * process_var, 0.5 * pow(dt, 3) * process_var,
        //    -0.1 * pow(dt, 2), -0.1 * pow(dt, 4), 0.5 * pow(dt, 3) * process_var, pow(dt, 2)* process_var;

       /* Q << 0.25 * pow(dt, 4) * process_var, 0.5 * pow(dt, 3) * process_var, 0, 0,
            0.5 * pow(dt, 3) * process_var, pow(dt, 2)* process_var, 0, 0,
            0, 0, 0.25 * pow(dt, 4) * process_var, 0.5 * pow(dt, 3) * process_var,
            0, 0, 0.5 * pow(dt, 3) * process_var, pow(dt, 2)* process_var;*/

        Q << 0.25 * pow(dt, 4) * process_var, 0, 0, 0,
            0, pow(dt, 2)* process_var, 0, 0,
            0, 0, 0.25 * pow(dt, 4) * process_var, 0,
            0, 0, 0, pow(dt, 2)* process_var;

        z << atan2(data[i].sensorDataGxy.gx, data[i].sensorDataGxy.gy) * 180 / 3.1416;
       // z << atan(data[i].sensorDataGxy.filterData);
       
        sigmaPointsUKF.createSigmaPoints(mean, var, alpha, k, beta, sPoints, weights);
        sigmaPoints_y = A * sPoints;
     
        predict_UKF(sigmaPoints_y, weights, x_hat, P_hat, Q);

        for (int i = 0; i < ZZ.rows(); i++) {
            for (int j = 0; j < ZZ.cols(); j++) {
                //ZZ(i, j) = atan(sigmaPoints_y(0, j) / sigmaPoints_y(2, j)) * 180 / 3.1416;
                ZZ(i, j) = atan2(sigmaPoints_y(0, j), sigmaPoints_y(2, j)) * 180 / 3.1416;
            }
        }

        update_UKF(sigmaPoints_y, ZZ, z, weights, x_hat, P_hat, x, P, R, c_var);
        mean = x;
        var = P;
        normRersidual = c_var;
        

        angle = atan(x[0] / x[2]) * 180 / 3.1416;

        outputData.sensorDataGxy.filterData = angle;
        outputData.sensorDataGxy.gx = x[0];
        outputData.sensorDataGxy.gy = x[2];
        outputData.sensorDataGxy.normRersidual = normRersidual;
        filteredData.push_back(outputData);
    }

    return filteredData;
}

  





