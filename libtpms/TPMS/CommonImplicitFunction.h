#ifndef COMMONIMPLICITFUNCTION_H
#define COMMONIMPLICITFUNCTION_H

#include <math.h>

namespace ImplicitFunciton {

/**
 * @brief The BaseImplicitFunction class
 * 基类，仿函数，抽象类
 */
class BaseImplicitFunction
{
public:
    virtual double operator() (double x, double y, double z) = 0;
};

// 标准 TPMS 隐函数 implicit function
// Ones 固定返回 1
double ONE(double, double, double);

// ZERO 固定返回 0
double ZERO(double, double, double);

// P type
double P(double x, double y, double z);

// D type
double D(double x, double y, double z);

// G type
double G(double x, double y, double z);

// I-WP type
double I_WP(double x, double y, double z);

// F-RD type
double F_RD(double x, double y, double z);

// L type
double L(double x, double y, double z);

// Tubular P type
double TUBULAR_P(double x, double y, double z);

// Tubular G type
double TUBULAR_G(double x, double y, double z);

// I2 - Y
double I2_Y(double x, double y,double z);

inline void toRadian(double& x, double& y, double& z){
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;
}

}

#endif // COMMONIMPLICITFUNCTION_H
