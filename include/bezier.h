#ifndef BEZIER_H
#define BEZIER_H

#include "curve.h"

class Bezier : public Curve
{
private:

public:

    void draw() const;

    Bezier(const std::vector<Vertex>& points, int numSegments);
};




#endif  // BEZIER_H
