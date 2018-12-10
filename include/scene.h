#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>

#include "math.h"
#include "types.h"
#include "light.h"

class Object;
class Camera;

/* Everything is drawn to the scene and at the
 * end of each iteration of the render loop,
 * the entire scene will be written to the
 * pixel buffer
 */
class Scene
{
    private:
        std::vector<Object*> m_objects;
        std::vector<Vertex> m_clippingWindow;
        Light* m_light;
        Camera* m_camera;
        Matrix4f m_projection;
        static BoundingBox m_boundingBox;
        float m_clipXmin;
        float m_clipXmax;
        float m_clipYmin;
        float m_clipYmax;
        float m_clipZmin;
        float m_clipZmax;

    private:
        void calcVertexIntensities();

    public:
        Scene();
        Scene(std::string loadFileName);
        ~Scene();

        void addObject(Object* object);
        void clean();
        void draw();
        void drawWithProjectionXY();
        void drawWithProjectionXZ();
        void drawWithProjectionYZ();
        void displayObjects() const;
        void load(std::string loadFileName);
        void save(std::string saveFileName) const;
        void setClippingWindow(float clipXmin, float clipXmax, float clipYmin, float clipYmax);
        void setBoundingBox(float clipXmin, float clipXmax, float clipYmin, float clipYmax, float clipZmin, float clipZmax);
        void removeObject(uint id);
        void loadProjectionMatrix(Matrix4f projection);
        void resizeBoundingBox(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
        void displayVertices(uint id);

        inline static BoundingBox getBoundingBox() {return m_boundingBox;}

        friend class Cli;
};


#endif  // SCENE_H