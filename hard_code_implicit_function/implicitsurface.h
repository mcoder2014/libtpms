#ifndef IMPLICITSURFACE_H
#define IMPLICITSURFACE_H

#include <QString>

class ImplicitSurface
{
public:
    ImplicitSurface();

    int m_type_flag;
    QString m_type;
    int setType(QString type);

    double f(double x, double y, double z);     // f(x,y,z) = 0

    double P(double x, double y, double z);     // P type
    double D(double x, double y, double z);     // D type
    double G(double x, double y, double z);     // G type
    double I_WP(double x, double y, double z);  // I-WP type
    double F_RD(double x, double y, double z);  // F-RD type
    double L(double x, double y, double z);     // L type
    double Tubular_P(double x, double y, double z); // Tubular P type
    double Tubular_G(double x, double y, double z); // Tubular G type
    double I2_Y(double x, double y,double z);       // I2 - Y **


};

#endif // IMPLICITSURFACE_H
