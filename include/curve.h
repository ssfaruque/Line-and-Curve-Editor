#ifndef CURVE_H
#define CURVE_H

#include <vector>

#include "types.h"

class Curve
{
protected:
    std::vector<Vertex> m_points;
    int m_numSegments;
    int m_id;

public:
    Curve();
    Curve(const std::vector<Vertex>& points, int numSegments);
    void addPoint(const Vertex& addedPoint);
    void insertPoint(int pointNum, const Vertex& insertedPoint);
    void deletePoint(int pointNum);
    void modifyPoint(int pointNum, const Vertex& newPoint);

    void print() const;

    virtual void draw() const = 0;


};


#endif  // CURVE_H
