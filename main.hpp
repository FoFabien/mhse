#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

// compilation flag ****************
#define ENGINE_OS 1 // 1 = windows, 2 = linux, 3 = mac, others = undefined

// macro ***************************
#define cdelete(x) {delete x; x = NULL;}
#define adelete(x) {delete [] x; x = NULL;}
#define ABS(x) ((x)<0 ? -(x) : (x))
#define SGN(x) ((x)<0 ? -1 : 1)

// lib *****************************
#include <iostream>
#include <iterator>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <limits>
#include <locale>
#include <vector>
#include <map>
#include <stack>
#include <bitset>
#include <queue>
#include <set>

//important define *****************
#define LOGFILE_NAME "log.txt"

struct XYZ
{
    XYZ(): x(0), y(0), z(0){};
    XYZ(float a, float b, float c): x(a), y(b), z(c){};
    XYZ(const XYZ &arg): x(arg.x), y(arg.y), z(arg.z){};
    inline bool operator==(const XYZ& rhs){return (x == rhs.x && y == rhs.y && z == rhs.z);}
    inline bool operator!=(const XYZ& rhs){return !(x == rhs.x && y == rhs.y && z == rhs.z);}
    inline XYZ& operator=(XYZ arg) {x = arg.x; y = arg.y; z = arg.z; return *this;}
    float x;
    float y;
    float z;
};

#endif // MAIN_HPP_INCLUDED
