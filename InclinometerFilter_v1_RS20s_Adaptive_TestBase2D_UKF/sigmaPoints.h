#pragma once
#include <Eigen/Dense>
#include <vector>

using namespace std;

class sigmaPoints
{
public:
	sigmaPoints();

	void createSigmaPoints(Eigen::VectorXd mean, 
		        Eigen::MatrixXd var, 
		        double alpha, 
		        int k, 
				double beta,
		        Eigen::MatrixXd& sigmaPoints,
		        Eigen::VectorXd& weight);

};

