#include "implicitsurface.h"

#include <QtMath>

ImplicitSurface::ImplicitSurface()
    :m_type_flag(0), m_type("p")
{

}

///
/// \brief ImplicitSurface::setType
/// \param type p, d, p, i-wp, f-rd, l, tubular-p, tubular-g, i2-y
/// \return
///
int ImplicitSurface::setType(QString type)
{
    if(type.toLower() == "p")
    {
        this->m_type_flag =0;
        this->m_type = "P";
    }
    else if(type.toLower() == "d")
    {
        this->m_type_flag = 1;
        this->m_type = "d";
    }
    else if (type.toLower() == "g")
    {
        this->m_type_flag = 2;
        this->m_type = "p";
    }
    else if (type.toLower() == "i-wp")
    {
        this->m_type_flag = 3;
        this->m_type = "i-wp";
    }
    else if (type.toLower() == "f-rd")
    {
        this->m_type_flag = 4;
        this->m_type = "f-rd";
    }
    else if (type.toLower() == "l")
    {
        this->m_type_flag = 5;
        this->m_type = "l";
    }
    else if (type.toLower() == "tubular-p")
    {
        this->m_type_flag = 6;
        this->m_type = "tubular-p";
    }
    else if (type.toLower() == "tubular-g")
    {
        this->m_type_flag = 7;
        this->m_type = "tubular-g";
    }
    else if (type.toLower() == "i2-y")
    {
        this->m_type_flag = 8;
        this->m_type = "i2-y";
    }
    return this->m_type_flag;
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
    switch (this->m_type_flag)
    {
    case 0:
        return P(x,y,z);
    case 1:
        return D(x,y,z);
    case 2:
        return G(x,y,z);
    case 3:
        return I_WP(x,y,z);
    case 4:
        return F_RD(x,y,z);
    case 5:
        return L(x,y,z);
    case 6:
        return Tubular_P(x,y,z);
    case 7:
        return Tubular_G(x,y,z);
    case 8:
        return I2_Y(x,y,z);
    default:
        return P(x,y,z);
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
