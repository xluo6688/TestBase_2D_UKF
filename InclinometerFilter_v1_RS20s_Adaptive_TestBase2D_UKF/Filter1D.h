#pragma once

// Project: This filter is designed for Joy Inclinometer with P/N 100613522 Issue 01 SN 13/1585
// 1. Initial version, Feb. 2023, Xuanwen Luo

#include "Gaussian.h"
#include <vector>
#include "dataType.h"

using namespace std;

class Filter1D
{
public:   
  
   static vector<dataType> filter(vector<double>);

private:
    static Gaussian gaussian_multiply(Gaussian g1, Gaussian g2);
    static Gaussian update(Gaussian prior, Gaussian likelihood);
    static Gaussian predict(Gaussian state, Gaussian control);
};

