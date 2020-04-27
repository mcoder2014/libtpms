#include "TpmsImplicitSurface.h"
#include <math.h>

using namespace std::placeholders;

std::unordered_map<TpmsType, std::function<double(double,int,int)>>
    TpmsImplicitSurface::implicitFuncitonTable{
        {TpmsType::D, std::bind(&TpmsImplicitSurface::D,_1,_2,_3)},
        {TpmsType::G, std::bind(&TpmsImplicitSurface::G,_1,_2,_3)},
        {TpmsType::P, std::bind(&TpmsImplicitSurface::P,_1,_2,_3)},
        {TpmsType::I_WP, std::bind(&TpmsImplicitSurface::I_WP,_1,_2,_3)},
        {TpmsType::F_RD, std::bind(&TpmsImplicitSurface::F_RD,_1,_2,_3)},
        {TpmsType::L, std::bind(&TpmsImplicitSurface::L,_1,_2,_3)},
        {TpmsType::TUBULAR_G, std::bind(&TpmsImplicitSurface::TUBULAR_G,_1,_2,_3)},
        {TpmsType::TUBULAR_P, std::bind(&TpmsImplicitSurface::TUBULAR_P,_1,_2,_3)},
        {TpmsType::I2_Y, std::bind(&TpmsImplicitSurface::I2_Y,_1,_2,_3)}};

std::unordered_map<std::string, TpmsType> TpmsImplicitSurface::typeTable{
    {"d", TpmsType::D},
    {"g", TpmsType::G},
    {"p", TpmsType::P},
    {"i-wp", TpmsType::I_WP},
    {"f-rd", TpmsType::F_RD},
    {"l", TpmsType::L},
    {"tubular-g", TpmsType::TUBULAR_G},
    {"tubular-p", TpmsType::TUBULAR_P},
    {"i2-y", TpmsType::I2_Y}
};

TpmsType TpmsImplicitSurface::getType() const
{
    return type;
}

void TpmsImplicitSurface::setType(const TpmsType &value)
{
    type = value;
}

double TpmsImplicitSurface::f(double x, double y, double z)
{
    return implicitFuncitonTable[type](x,y,z);
}

double TpmsImplicitSurface::P(double x, double y, double z)
{
    toRadian(x,y,z);
    return cos(x) + cos(y) + cos(z);
}

double TpmsImplicitSurface::D(double x, double y, double z)
{
    toRadian(x,y,z);
    return cos(x) * cos(y) * cos(z)
            + sin(x) * sin(y) * sin(z);
}

double TpmsImplicitSurface::G(double x, double y, double z)
{
    toRadian(x, y, z);
    return sin(x) * cos(y) + sin(z)*cos(x) + sin(y) * cos(z);
}

double TpmsImplicitSurface::I_WP(double x, double y, double z)
{
    toRadian(x,y,z);
    return 2*(cos(x)*cos(y) + cos(y)*cos(z) + cos(z)*cos(x))
            - (cos(2*x) + cos(2*y) + cos(2*z));
}

double TpmsImplicitSurface::F_RD(double x, double y, double z)
{
    toRadian(x, y, z);
    return 4*cos(x)*cos(y)*cos(z)
            - (cos(2*x)*cos(2*y) + cos(2*x)*cos(2*z) + cos(2*y)*cos(2*z));
}

double TpmsImplicitSurface::L(double x, double y, double z)
{
    toRadian(x, y, z);
    return 0.5 * (sin(2*x)*cos(y)*cos(z)
                 + sin(2*y)*cos(z)*sin(x)
                 + sin(2*z)*cos(x)*sin(y))
           - 0.5 * (cos(2*x)*cos(2*y)
                   + cos(2*y)*cos(2*z)
                   + cos(2*z)*cos(2*x));
}

double TpmsImplicitSurface::TUBULAR_P(double x, double y, double z)
{
    toRadian(x, y, z);
    return 10 * (cos(x) + cos(y) + cos(z))
            - 5.1 * (cos(x)*cos(y) + cos(y)*cos(z) + cos(z)*cos(x))
            - 14.6;
}

double TpmsImplicitSurface::TUBULAR_G(double x, double y, double z)
{
    toRadian(x, y, z);
    return 10 * (cos(x)*sin(y) + cos(y)*sin(z) +cos(z)+sin(x))
            - 0.5 * (cos(2*x)*cos(2*y) + cos(2*y)*cos(2*z) + cos(2*z)*cos(2*x))
            - 14;
}

double TpmsImplicitSurface::I2_Y(double x, double y, double z)
{
    toRadian(x, y, z);
    return -2 * (sin(2*x)*cos(y)*sin(z)
                 + sin(x)*sin(2*y)*cos(z)
                 + cos(x)*sin(y)*sin(2*z))
            + cos(2*x)*cos(2*y)
            + cos(2*y)*cos(2*z)
            + cos(2*x)*cos(x*z);
}

void TpmsImplicitSurface::toRadian(double &x, double &y, double &z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;
}
