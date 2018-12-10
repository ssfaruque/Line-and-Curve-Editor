#include "math.h"
#include "types.h"
#include <cmath>


Matrix3f createTranslationMatrix3f(const Vector2f& vector)
{
    Matrix3f matrix(1.0f);
    matrix[0][2] = vector[0];
    matrix[1][2] = vector[1];
    return transpose(matrix);
}


Matrix3f createScaleMatrix3f(const Vector2f& vector)
{
    Matrix3f matrix(1.0f);
    matrix[0][0] = vector[0];
    matrix[1][1] = vector[1];
    return transpose(matrix);
}



Matrix4f createTranslationMatrix4f(const Vector3f& vector)
{
    Matrix4f matrix(1.0f);
    matrix[0][3] = vector.x;
    matrix[1][3] = vector.y;
    matrix[2][3] = vector.z;
    return transpose(matrix);
}


Matrix4f createScaleMatrix4f(const Vector3f& vector)
{
    Matrix4f matrix(1.0f);
    matrix[0][0] = vector.x;
    matrix[1][1] = vector.y;
    matrix[2][2] = vector.z;
    return transpose(matrix);
}


Matrix4f create3DRotateXMatrix(float angle)
{
    Matrix4f matrix(1.0f);
    matrix[1][1] = cos(angle);
    matrix[1][2] = -sin(angle);
    matrix[2][1] = sin(angle);
    matrix[2][2] = cos(angle);
    return transpose(matrix);
}


Matrix4f create3DRotateYMatrix(float angle)
{
    Matrix4f matrix(1.0f);
    matrix[0][0] = cos(angle);
    matrix[0][2] = sin(angle);
    matrix[2][0] = -sin(angle);
    matrix[2][2] = cos(angle);
    return transpose(matrix);
}


Matrix4f create3DRotateZMatrix(float angle)
{
    Matrix4f matrix(1.0f);
    matrix[0][0] = cos(angle);
    matrix[0][1] = -sin(angle);
    matrix[1][0] = sin(angle);
    matrix[1][1] = cos(angle);
    return transpose(matrix);
}


/* Rotation matrix created using quaternions */
Matrix4f create3DRotateMatrix(const Vector3f& p0, const Vector3f& p1, float angle)
{
    Matrix4f rotationMatrix(1.0f);

    float l = sqrt((p1.x - p0.x) * (p1.x - p0.x) +
                   (p1.y - p0.y) * (p1.y - p0.y) +
                   (p1.z - p0.z) * (p1.z - p0.z));

    float cosA = cos(angle);
    float oneC = 1 - cosA;
    float sinA = sin(angle);
    float ux = (p1.x - p0.x) / l;
    float uy = (p1.y - p0.y) / l;
    float uz = (p1.z - p0.z) / l;

    Matrix4f translateNegP0 = createTranslationMatrix4f(Vector3f(-p0.x, -p0.y, -p0.z));
    Matrix4f translatePosP0 = createTranslationMatrix4f(Vector3f(p0.x, p0.y, p0.z));

    rotationMatrix[0][0] = ux * ux * oneC + cosA;
    rotationMatrix[0][1] = ux * uy * oneC - uz * sinA;
    rotationMatrix[0][2] = ux * uz * oneC + uy * sinA;

    rotationMatrix[1][0] = uy * ux * oneC + uz * sinA;
    rotationMatrix[1][1] = uy * uy * oneC + cosA;
    rotationMatrix[1][2] = uy * uz * oneC - ux * sinA;

    rotationMatrix[2][0] = uz * ux * oneC - uy * sinA;
    rotationMatrix[2][1] = uz * uy * oneC + ux * sinA;
    rotationMatrix[2][2] = uz * uz * oneC + cosA;
    
    rotationMatrix = transpose(rotationMatrix);

    return translatePosP0 * rotationMatrix * translateNegP0;

    return rotationMatrix;
}




Matrix4f createOrthoXYProj()
{
    Matrix4f matrix(1.0f);
    matrix[2][2] = 0;
    return transpose(matrix);
}


Matrix4f createOrthoYZProj()
{
    Matrix4f matrix(1.0f);
    matrix[0][0] = 0;
    return transpose(matrix);
}


Matrix4f createOrthoXZProj()
{
    Matrix4f matrix(1.0f);
    matrix[1][1] = 0;
    return transpose(matrix);
}



Matrix4f createOrthoMatrix(const BoundingBox& boundingBox)
{
    Matrix4f matrix(1.0f);

    matrix[0][0] = 2.0f / (boundingBox.xmax - boundingBox.xmin);
    matrix[0][3] = -(boundingBox.xmax + boundingBox.xmin) / (boundingBox.xmax - boundingBox.xmin);

    matrix[1][1] = 2.0f / (boundingBox.ymax - boundingBox.ymin);
    matrix[1][3] = -(boundingBox.ymax + boundingBox.ymin) / (boundingBox.ymax - boundingBox.ymin);

    matrix[2][2] = 1.0f / (boundingBox.zmax - boundingBox.zmin);
    matrix[2][3] = -boundingBox.zmin / (boundingBox.zmax - boundingBox.zmin);

    return transpose(matrix);
}
