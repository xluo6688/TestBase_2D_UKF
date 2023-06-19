#pragma once

// Project: This filter is designed for Joy Inclinometer with P/N 100613522 Issue 01 SN 13/1585
// 1. Initial version, Feb. 2023, Xuanwen Luo

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Gaussian.h"
#include "dataType.h"


using namespace std;

class fileApp
{	public:
		//static vector<double>  readCsvData(string, int);
		static vector<dataType>  readCsvData(string, int, int);
		static bool  writeCsvData(string, vector<double>, vector<dataType>);
		static bool  writeCsvData2D(string, vector<dataType>, vector<dataType>);
};

