#ifndef TPMSIMPLICITSURFACE_H
#define TPMSIMPLICITSURFACE_H

#include <string>
#include <unordered_map>
#include <functional>

enum TpmsType
{
    P = 0,
    D = 1,
    G = 2,
    I_WP = 3,
    F_RD = 4,
    L = 5,
    TUBULAR_P = 6,
    TUBULAR_G = 7,
    I2_Y = 8
};


class TpmsImplicitSurface
{
public:
    TpmsType getType() const;
    void setType(const TpmsType &value);
    void setType(std::string value);

    double f(double x, double y, double z);

    // 标准 TPMS 隐函数 implicit function
    static double P(double x, double y, double z);         // P type
    static double D(double x, double y, double z);         // D type
    static double G(double x, double y, double z);         // G type
    static double I_WP(double x, double y, double z);      // I-WP type
    static double F_RD(double x, double y, double z);      // F-RD type
    static double L(double x, double y, double z);         // L type
    static double TUBULAR_P(double x, double y, double z); // Tubular P type
    static double TUBULAR_G(double x, double y, double z); // Tubular G type
    static double I2_Y(double x, double y,double z);       // I2 - Y

private:
    TpmsType type;
    static std::unordered_map<std::string, TpmsType> typeTable;
    static std::unordered_map<TpmsType, std::function<double(double,int,int)>> implicitFuncitonTable;

    static void toRadian(double& x, double& y, double& z);
};

#endif // TPMSIMPLICITSURFACE_H
