#ifndef BSPLINE_H
#define BSPLINE_H

#include "curve.h"

class Bspline : public Curve
{
private:

public:

    void draw() const;

    Bspline(const std::vector<Vertex>& points, int numSegments);
};




#endif  // BSPLINE_H