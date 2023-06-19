#pragma once
// Project: This filter is designed for Joy Inclinometer with P/N 100613522 Issue 01 SN 13/1585
// 1. Initial version, Feb. 2023, Xuanwen Luo

#include <iostream>

class Gaussian
{
public: 
    Gaussian() {
        this->mean = 0;
        this->var = 0;
    }

    Gaussian(double mean, double var) {
        this->mean = mean;
        this->var = var;
    }
    double getMean() {
        return mean;
    }
    double getVar() {
        return var;
    }      

private:
    double mean;
    double var;
};

