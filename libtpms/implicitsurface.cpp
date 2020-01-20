#include "implicitsurface.h"

#include <map>
#include <algorithm>

#include <QtMath>

ImplicitSurface::ImplicitSurface()
    :m_type("p")
{
    initFunctionMap();
}

void ImplicitSurface::setType(std::string type)
{
    std::transform(type.begin(),type.end(),type.begin(), tolower);
    if(this->m_funcMap.find(type) != m_funcMap.end())
        this->m_type = type;
}

void ImplicitSurface::initFunctionMap()
{
    m_funcMap["p"] = &ImplicitSurface::P;
    m_funcMap["d"] = &ImplicitSurface::D;
    m_funcMap["g"] = &ImplicitSurface::G;
    m_funcMap["i-wp"] = &ImplicitSurface::I_WP;
    m_funcMap["f-rd"] = &ImplicitSurface::F_RD;
    m_funcMap["l"] = &ImplicitSurface::L;
    m_funcMap["tubular-g"] = &ImplicitSurface::Tubular_G;
    m_funcMap["tubular-p"] = &ImplicitSurface::Tubular_P;
    m_funcMap["i2-y"] = &ImplicitSurface::I2_Y;
}

///
/// \brief ImplicitSurface::f
/// \param x
/// \param y
/// \param z
/// \return
/// Get the value about f(x,y,z), which can be used to judge
/// if one point is inside the implicit surface.
///
double ImplicitSurface::f(double x, double y, double z)
{
    return this->f(m_type, x, y, z);
}

double ImplicitSurface::f(std::string type, double x, double y, double z)
{
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if(m_funcMap.find(type) != m_funcMap.end())
    {
        auto func = m_funcMap[type];
        return (this->*func)(x,y,z);
    }
    else
    {
        return (this->*(m_funcMap.begin())->second)(x,y,z);
    }
}

double ImplicitSurface::P(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return qCos(x) + qCos(y) + qCos(z);
}

double ImplicitSurface::D(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return qCos(x) * qCos(y) * qCos(z) - qSin(x) * qSin(y) * qSin(z);
}

double ImplicitSurface::G(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return qSin(x)*qCos(y) + qSin(z)*qCos(x) + qSin(y)*qCos(z);
}

double ImplicitSurface::I_WP(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return 2*(qCos(x) * qCos(y) + qCos(y) * qCos(z) + qCos(z) * qCos(x))
            - (qCos(2 * x) + qCos(2 * y) + qCos(2 * z));
}

double ImplicitSurface::F_RD(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return 4 * qCos(x) * qCos(y) * qCos(z)
            - (qCos(2 * x) * qCos(2 * z) + qCos(2 * x) * qCos(2 * z) + qCos(2 * y) * qCos(2 * z));

}

double ImplicitSurface::L(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return 0.5 * (qSin(2 * x) * qCos(y) * qSin(z)
                  + qSin(2 * y) * qCos(z) * qSin(x)
                  + qSin(2 * z) * qCos(x) * qSin(y))
            - 0.5 * (qCos(2 * x) * qCos(2 * y)
                     +qCos(2 * y) * qCos(2 * z)
                     +qCos(2 * z) * qCos(2 * x));
}

double ImplicitSurface::Tubular_P(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return 10 * (qCos(x) + qCos(y) + qCos(z))
            - 5.1 * (qCos(x) * qCos(y)
                     + qCos(y) * qCos(z)
                     + qCos(x) * qCos(z))
            -14.6;
}

double ImplicitSurface::Tubular_G(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return 10 * (qCos(x) * qSin(y) + qCos(y) * qSin(z) + qCos(z) * qSin(x))
            - 0.5 * (qCos(2 * x) * qCos(2 * y)
                     + qCos(2 * y) * qCos(2 * z)
                     + qCos(2 * z) * qCos(2 * x))
            -14;
}

double ImplicitSurface::I2_Y(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return -2 * (qSin(2 * x) * qCos(y) * qSin(z)
                 + qSin(x) * qSin(2 * y) * qCos(z)
                 + qCos(x) * qSin(y) * sin(2 * z))
            + qCos(2 * x) * qCos(2 * y)
            + qCos(2 * y) * qCos(2 * z)
            + qCos(2 * x) * qCos(2 * z);
}
