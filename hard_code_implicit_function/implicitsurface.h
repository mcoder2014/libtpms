#ifndef IMPLICITSURFACE_H
#define IMPLICITSURFACE_H

#include <unordered_map>
#include <string>

#include <QString>

class ImplicitSurface
{
public:
    ImplicitSurface();

//    QString m_type;
    std::string m_type;
    void setType(QString type);
    void setType(std::string type);
    void initFunctionMap();

    double f(double x, double y, double z);     // f(x,y,z) = 0
//    double f(QString type, double x, double y, double z);
    double f(std::string type, double x, double y, double z);

    double P(double x, double y, double z);     // P type
    double D(double x, double y, double z);     // D type
    double G(double x, double y, double z);     // G type
    double I_WP(double x, double y, double z);  // I-WP type
    double F_RD(double x, double y, double z);  // F-RD type
    double L(double x, double y, double z);     // L type
    double Tubular_P(double x, double y, double z); // Tubular P type
    double Tubular_G(double x, double y, double z); // Tubular G type
    double I2_Y(double x, double y,double z);       // I2 - Y **

private:
//    using pFunc = double (ImplicitSurface::*)(double, double, double);
//    typedef  double (ImplicitSurface::*pFunc)(double, double, double);
    std::unordered_map<std::string, double (ImplicitSurface::*)(double x, double y, double z)> m_funcMap;
};

#endif // IMPLICITSURFACE_H
