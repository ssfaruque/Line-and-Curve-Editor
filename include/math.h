#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>

struct Vertex;
struct BoundingBox;

using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Vector4f = glm::vec4;

using Matrix2f = glm::mat2;
using Matrix3f = glm::mat3;
using Matrix4f = glm::mat4;


#define dotProduct glm::dot
#define crossProduct glm::cross
#define normalize glm::normalize
#define inverse glm::inverse
#define reflect glm::reflect
#define magnitude glm::length

#define transpose glm::transpose


inline float toRadians(float angle) {return glm::radians(angle);}

// 2D transformations
Matrix3f createTranslationMatrix3f(const Vector2f& vector);
Matrix3f createScaleMatrix3f(const Vector2f& vector);
Matrix3f create2DRotateMatrix(const Vertex& centroid, float angle);


// 3D transformations
Matrix4f createTranslationMatrix4f(const Vector3f& vector);
Matrix4f createScaleMatrix4f(const Vector3f& vector);
Matrix4f create3DRotateXMatrix(float angle);
Matrix4f create3DRotateYMatrix(float angle);
Matrix4f create3DRotateZMatrix(float angle);
Matrix4f create3DRotateMatrix(const Vector3f& p0, const Vector3f& p1, float angle);


// Projections
Matrix4f createOrthoXYProj();
Matrix4f createOrthoYZProj();
Matrix4f createOrthoXZProj();

Matrix4f createOrthoMatrix(const BoundingBox& boundingBox);


#endif  // MATH_H