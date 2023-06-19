#pragma once

// Project: This filter is designed for Joy Inclinometer with P/N 100613522 Issue 01 SN 13/1585
// 1. Initial version, Feb. 2023, Xuanwen Luo

#include "Gaussian.h"
#include <vector>
#include "dataType.h"
#include <Eigen/Dense>

using namespace std;


class Filter2D
{
public:

    Filter2D() {};
      
   static vector<dataType> filter2D(vector<double> data);

   /* Filter2D(const Eigen::VectorXd& x, const Eigen::MatrixXd& P, const Eigen::MatrixXd& F, const Eigen::MatrixXd& Q, double dt)     
    {
        this->_I = Eigen::MatrixXd::Identity(x.rows(), x.rows());
        this->_state.resize(x.rows());
        this->_state = x;
        this->_P = P;
        this->_F = F;
        this->_Q = Q;
        this->_dt = dt;
    };*/

    ~Filter2D() {};

private:
    void static predict(
        const Eigen::VectorXd& x,
        Eigen::VectorXd& x_hat,
        const Eigen::MatrixXd& P,
        Eigen::MatrixXd& P_hat,
        const Eigen::MatrixXd& F,
        const Eigen::MatrixXd& Q);

    void static update(
        Eigen::VectorXd& x,
        const Eigen::VectorXd& x_hat,
        Eigen::MatrixXd& P,
        const Eigen::MatrixXd& P_hat,
        const Eigen::VectorXd& Z,
        const Eigen::RowVectorXd& H, 
        const Eigen::MatrixXd& R,
        Eigen::VectorXd& c_var);

    //double _dt;
    //Eigen::MatrixXd _F; // process transition
    //Eigen::MatrixXd _P; // process covaraince
    //Eigen::MatrixXd _Q; // process noise covariance
    //Eigen::MatrixXd _H; // measurment transition
    //Eigen::MatrixXd _R; // measurement noise covariance
    //Eigen::MatrixXd _I; // Identity matrix
    //Eigen::MatrixXd _S; // Intermediate Matrix in update part
    //Eigen::MatrixXd _K; // Kalman Gain
    //Eigen::VectorXd _state; // State
};

