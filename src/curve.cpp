#include <iostream>
#include <algorithm>

#include "curve.h"

int ID = 0;

Curve::Curve():
m_points({}), m_numSegments(0), m_id(ID++)
{
}


Curve::Curve(const std::vector<Vertex>& points, int numSegments):
m_points(points), m_numSegments(numSegments), m_id(ID++)
{
}


void Curve::addPoint(const Vertex& addedPoint)
{
    m_points.push_back(addedPoint);
}



void Curve::insertPoint(int pointNum, const Vertex& insertedPoint)
{
    if(pointNum < 0 || pointNum > int(m_points.size()) + 1)
    {
        std::cout << "pointNum " << pointNum << "is outside appropriate range for insertion" << std::endl;
        std::cout << "There are " << m_points.size() << " points in the curve" << std::endl;
    }

    m_points.insert(m_points.begin() + pointNum, insertedPoint);
}


void Curve::deletePoint(int pointNum)
{
    m_points.erase(m_points.begin() + pointNum);
}


void Curve::modifyPoint(int pointNum, const Vertex& newPoint)
{
    if(pointNum < 0 || pointNum >= int(m_points.size()))
    {
        std::cout << "pointNum " << pointNum << "is outside appropriate range for modification" << std::endl;
        std::cout << "There are " << m_points.size() << " points in the curve" << std::endl;
    }

    m_points[pointNum] = newPoint;
}


void Curve::print() const
{
    std::cout << "ID: " << m_id << std::endl;
    std::cout << "numSegments: " << m_numSegments << std::endl;

    for(int i = 0; i < int(m_points.size()); ++i)
    {
        Vertex p = m_points[i];
        std::cout << "Point " << i << ": " << "(" << p.position.x << ", " << p.position.y << ", " << p.position.z << ")" << std::endl;
    }
}
