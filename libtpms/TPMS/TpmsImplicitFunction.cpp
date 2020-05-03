#include "TpmsImplicitFunction.h"

#include <math.h>
#include <algorithm>

using namespace std::placeholders;

std::unordered_map<TpmsType, std::function<double(double,double,double)>>
    TpmsImplicitFunction::implicitFuncitonTable{
        {TpmsType::D, std::bind(&TpmsImplicitFunction::D,_1,_2,_3)},
        {TpmsType::G, std::bind(&TpmsImplicitFunction::G,_1,_2,_3)},
        {TpmsType::P, std::bind(&TpmsImplicitFunction::P,_1,_2,_3)},
        {TpmsType::I_WP, std::bind(&TpmsImplicitFunction::I_WP,_1,_2,_3)},
        {TpmsType::F_RD, std::bind(&TpmsImplicitFunction::F_RD,_1,_2,_3)},
        {TpmsType::L, std::bind(&TpmsImplicitFunction::L,_1,_2,_3)},
        {TpmsType::TUBULAR_G, std::bind(&TpmsImplicitFunction::TUBULAR_G,_1,_2,_3)},
        {TpmsType::TUBULAR_P, std::bind(&TpmsImplicitFunction::TUBULAR_P,_1,_2,_3)},
        {TpmsType::I2_Y, std::bind(&TpmsImplicitFunction::I2_Y,_1,_2,_3)}};

std::unordered_map<std::string, TpmsType> TpmsImplicitFunction::typeTable{
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

TpmsType TpmsImplicitFunction::getType() const
{
    return type;
}

void TpmsImplicitFunction::setType(const TpmsType &value)
{
    type = value;
}

void TpmsImplicitFunction::setType(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    if(typeTable.find(value) != typeTable.end()) {
        type = typeTable[value];
    }
}

double TpmsImplicitFunction::f(double x, double y, double z)
{
    return implicitFuncitonTable[type](x,y,z);
}

double TpmsImplicitFunction::P(double x, double y, double z)
{
    toRadian(x,y,z);
    return cos(x) + cos(y) + cos(z);
}

double TpmsImplicitFunction::D(double x, double y, double z)
{
    toRadian(x,y,z);
    return cos(x) * cos(y) * cos(z)
            + sin(x) * sin(y) * sin(z);
}

double TpmsImplicitFunction::G(double x, double y, double z)
{
    toRadian(x, y, z);
    return sin(x) * cos(y) + sin(z)*cos(x) + sin(y) * cos(z);
}

double TpmsImplicitFunction::I_WP(double x, double y, double z)
{
    toRadian(x,y,z);
    return 2*(cos(x)*cos(y) + cos(y)*cos(z) + cos(z)*cos(x))
            - (cos(2*x) + cos(2*y) + cos(2*z));
}

double TpmsImplicitFunction::F_RD(double x, double y, double z)
{
    toRadian(x, y, z);
    return 4*cos(x)*cos(y)*cos(z)
            - (cos(2*x)*cos(2*y) + cos(2*x)*cos(2*z) + cos(2*y)*cos(2*z));
}

double TpmsImplicitFunction::L(double x, double y, double z)
{
    toRadian(x, y, z);
    return 0.5 * (sin(2*x)*cos(y)*cos(z)
                 + sin(2*y)*cos(z)*sin(x)
                 + sin(2*z)*cos(x)*sin(y))
           - 0.5 * (cos(2*x)*cos(2*y)
                   + cos(2*y)*cos(2*z)
                   + cos(2*z)*cos(2*x));
}

double TpmsImplicitFunction::TUBULAR_P(double x, double y, double z)
{
    toRadian(x, y, z);
    return 10 * (cos(x) + cos(y) + cos(z))
            - 5.1 * (cos(x)*cos(y) + cos(y)*cos(z) + cos(z)*cos(x))
            - 14.6;
}

double TpmsImplicitFunction::TUBULAR_G(double x, double y, double z)
{
    toRadian(x, y, z);
    return 10 * (cos(x)*sin(y) + cos(y)*sin(z) +cos(z)+sin(x))
            - 0.5 * (cos(2*x)*cos(2*y) + cos(2*y)*cos(2*z) + cos(2*z)*cos(2*x))
            - 14;
}

double TpmsImplicitFunction::I2_Y(double x, double y, double z)
{
    toRadian(x, y, z);
    return -2 * (sin(2*x)*cos(y)*sin(z)
                 + sin(x)*sin(2*y)*cos(z)
                 + cos(x)*sin(y)*sin(2*z))
            + cos(2*x)*cos(2*y)
            + cos(2*y)*cos(2*z)
            + cos(2*x)*cos(x*z);
}

void TpmsImplicitFunction::toRadian(double &x, double &y, double &z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;
}

std::string tpmsTypeToString(TpmsType tpmsType)
{
    static std::unordered_map<TpmsType, std::string> tpmsType2String{
        {TpmsType::D, "d"},
        {TpmsType::G, "g"},
        {TpmsType::P, "p"},
        {TpmsType::I_WP, "i-wp"},
        {TpmsType::F_RD, "f-rd"},
        {TpmsType::L, "l"},
        {TpmsType::TUBULAR_G, "tubular-g"},
        {TpmsType::TUBULAR_P, "tubular-p"},
        {TpmsType::I2_Y, "i2-y"}};

    return tpmsType2String[tpmsType];
}
