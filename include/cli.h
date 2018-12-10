#ifndef CLI_H
#define CLI_H

#include <vector>
#include <string>
#include "types.h"

class Scene;

class Cli
{
    private:
        Scene* m_scene;

    private:
        // All possible commands
        void cmdLoad(const std::string fileName);
        void cmdSave(const std::string fileName);
        void cmdTranslate(int id, float xTrans, float yTrans, float zTrans);
        void cmdScale(int id, float xScale, float yScale, float zScale);
        void cmdRotate(int id, float x1, float y1, float z1, float x2, float y2, float z2, float angle);
        void cmdRemove(int id);
        void cmdDisplayObjects() const;
        void cmdDisplayVertices(int id) const;
        void cmdHelp() const;

        void cmdSetLightPos(float x, float y, float z);
        void cmdSetLightIntensity(float r, float g, float b);
        void cmdSetLightAmbIntensity(float r, float g, float b);
        void cmdSetK(float K);
        void cmdSetPhongConstant(float phongConstant);
        void cmdSetKA(float r, float g, float b);
        void cmdSetKD(float r, float g, float b);
        void cmdSetKS(float r, float g, float b);
        void cmdSetFrom(float x, float y, float z);

        std::vector<std::string> getTokensFromLine(const std::string& line) const;

    public:
        Cli(Scene* scene);
        ~Cli() = default;

        void processInput();
};



#endif  // CLI_H