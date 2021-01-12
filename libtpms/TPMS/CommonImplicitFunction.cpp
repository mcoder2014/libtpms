#include "CommonImplicitFunction.h"


double ImplicitFunciton::P(double x, double y, double z)
{
    toRadian(x,y,z);
    return cos(x) + cos(y) + cos(z);
}

double ImplicitFunciton::D(double x, double y, double z)
{
    toRadian(x,y,z);
    return cos(x) * cos(y) * cos(z)
            + sin(x) * sin(y) * sin(z);
}

double ImplicitFunciton::G(double x, double y, double z)
{
    toRadian(x, y, z);
    return sin(x) * cos(y) + sin(z)*cos(x) + sin(y) * cos(z);
}

double ImplicitFunciton::I_WP(double x, double y, double z)
{
    toRadian(x,y,z);
    return 2*(cos(x)*cos(y) + cos(y)*cos(z) + cos(z)*cos(x))
            - (cos(2*x) + cos(2*y) + cos(2*z));
}

double ImplicitFunciton::F_RD(double x, double y, double z)
{
    toRadian(x, y, z);
    return 4*cos(x)*cos(y)*cos(z)
            - (cos(2*x)*cos(2*y) + cos(2*x)*cos(2*z) + cos(2*y)*cos(2*z));
}

double ImplicitFunciton::L(double x, double y, double z)
{
    toRadian(x, y, z);
    return 0.5 * (sin(2*x)*cos(y)*cos(z)
                 + sin(2*y)*cos(z)*sin(x)
                 + sin(2*z)*cos(x)*sin(y))
           - 0.5 * (cos(2*x)*cos(2*y)
                   + cos(2*y)*cos(2*z)
                   + cos(2*z)*cos(2*x));
}

double ImplicitFunciton::TUBULAR_P(double x, double y, double z)
{
    toRadian(x, y, z);
    return 10 * (cos(x) + cos(y) + cos(z))
            - 5.1 * (cos(x)*cos(y) + cos(y)*cos(z) + cos(z)*cos(x))
            - 14.6;
}

double ImplicitFunciton::TUBULAR_G(double x, double y, double z)
{
    toRadian(x, y, z);
    return 10 * (cos(x)*sin(y) + cos(y)*sin(z) +cos(z)+sin(x))
            - 0.5 * (cos(2*x)*cos(2*y) + cos(2*y)*cos(2*z) + cos(2*z)*cos(2*x))
            - 14;
}

double ImplicitFunciton::I2_Y(double x, double y, double z)
{
    toRadian(x, y, z);
    return -2 * (sin(2*x)*cos(y)*sin(z)
                 + sin(x)*sin(2*y)*cos(z)
                 + cos(x)*sin(y)*sin(2*z))
            + cos(2*x)*cos(2*y)
            + cos(2*y)*cos(2*z)
            + cos(2*x)*cos(x*z);
}

double ImplicitFunciton::ONE(double, double, double) {return 1.0;}

double ImplicitFunciton::ZERO(double, double, double) {return 0.0;}
