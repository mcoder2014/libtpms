#include "implicitsurface.h"

#include <QtMath>

ImplicitSurface::ImplicitSurface()
{

}

double ImplicitSurface::G(double x, double y, double z)
{
    x = 2 * M_PI * x;
    y = 2 * M_PI * y;
    z = 2 * M_PI * z;

    return qSin(x)*qCos(y) + qSin(z)*qCos(x) + qSin(y)*qCos(z);
}
