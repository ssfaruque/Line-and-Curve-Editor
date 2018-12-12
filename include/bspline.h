#ifndef BSPLINE_H
#define BSPLINE_H

#include "curve.h"

class Bspline : public Curve
{
public:
    int m_k;

public:

    void draw() const;
    void printInfo() const;

    inline void setK(int k) {m_k = k;} 

    Bspline(const std::vector<Vertex>& points, int k, int numSegments);
};




#endif  // BSPLINE_H