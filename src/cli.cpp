#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include <iostream>
#include <sstream>
#include <GL/gl.h>


#include "cli.h"
#include "window.h"
#include "scene.h"
#include "frameBuffer.h"
#include "object.h"
#include "camera.h"


Cli::Cli(Scene* scene):
m_scene(scene)
{}


std::vector<std::string> Cli::getTokensFromLine(const std::string& line) const
{
    std::vector<std::string> tokens{};

    std::istringstream iss(line);
    std::string token;

    while(std::getline(iss, token, ' '))
        tokens.push_back(token);

    return tokens;
}

void Cli::processInput()
{
    std::string inputLine;
    std::cout << ">> ";
    std::getline(std::cin, inputLine);

    std::vector<std::string> tokens = getTokensFromLine(inputLine);

    std::string command = !tokens.empty() ? tokens[0] : "";
    std::stringstream ss;

    if(command == "load" && tokens.size() == 2)
        cmdLoad(tokens[1]);    

    else if(command == "save" && tokens.size() == 2)
        cmdSave(tokens[1]); 


    else if(command == "translate" && tokens.size() == 5)
        cmdTranslate(std::stoi(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]), std::stof(tokens[4]));

    else if(command == "scale" && tokens.size() == 5)
        cmdScale(std::stoi(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]), std::stof(tokens[4]));

    else if(command == "rotate" && tokens.size() == 9)
        cmdRotate(std::stoi(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]), std::stof(tokens[4]), std::stof(tokens[5]), std::stof(tokens[6]), std::stof(tokens[7]), std::stof(tokens[8]));

    else if(command == "remove" && tokens.size() == 2)
        cmdRemove(std::stoi(tokens[1]));

    else if(command == "help" && tokens.size() == 1)
        cmdHelp();

    else if(command == "displayContents" && tokens.size() == 1)
        cmdDisplayObjects();

    else if(command == "displayVertices" && tokens.size() == 2)
        cmdDisplayVertices(std::stof(tokens[1]));





    else if(command == "setLightPos" && tokens.size() == 4)
        cmdSetLightPos(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

    else if(command == "setLightIntensity" && tokens.size() == 4)
        cmdSetLightIntensity(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

    else if(command == "setLightAmbIntensity" && tokens.size() == 4)
        cmdSetLightAmbIntensity(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

    else if(command == "setK" && tokens.size() == 2)
        cmdSetK(std::stof(tokens[1]));

    else if(command == "setPhongConstant" && tokens.size() == 2)
        cmdSetPhongConstant(std::stof(tokens[1]));

    else if(command == "setKA" && tokens.size() == 4)
        cmdSetKA(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

    else if(command == "setKD" && tokens.size() == 4)
        cmdSetKD(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

    else if(command == "setKS" && tokens.size() == 4)
        cmdSetKS(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));

    else if(command == "setFrom" && tokens.size() == 4)
        cmdSetFrom(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
    


    
    else if(command == "exit")
        exit(0);

    else if(tokens.size() > 0)
        std::cout << "Invalid command: " << inputLine << std::endl;
}


void Cli::cmdLoad(const std::string fileName)
{
    m_scene->load(fileName);
}


void Cli::cmdSave(const std::string fileName)
{
    m_scene->save(fileName);
}



void Cli::cmdTranslate(int id, float xTrans, float yTrans, float zTrans)
{
    for (Object *object : m_scene->m_objects)
    {
        if (object->getId() == uint(id))
        {
            object->translate(Vector3f(xTrans, yTrans, zTrans));
            break;
        }
    }
}



void Cli::cmdScale(int id, float xScale, float yScale, float zScale)
{
    for (Object *object : m_scene->m_objects)
    {
        if (object->getId() == uint(id))
        {
            object->scale(Vector3f(xScale, yScale, zScale));
            break;
        }
    }    
}

void Cli::cmdRotate(int id, float x1, float y1, float z1, float x2, float y2, float z2, float angle)
{
    for (Object *object : m_scene->m_objects)
    {
        if (object->getId() == uint(id))
        {
            object->rotate(Vector3f(x1, y1, z1), Vector3f(x2, y2, z2), angle);
            break;
        }
    }
}


void Cli::cmdRemove(int id)
{
}


void Cli::cmdDisplayObjects() const
{
    m_scene->displayObjects();
}


void screenToNormalCoords(float* x, float* y)
{
    Window *window = Window::getInstance();
    *x = ((2.0f / window->getWidth())  * (*x)) - 1;
    *y = ((2.0f / window->getHeight()) * (*y)) - 1;
}


void Cli::cmdHelp() const
{
    std::cout << "---------- LIST OF COMMANDS ----------" << std::endl;

    std::cout << "load: 'load fileName'" << std::endl;
    std::cout << "save: 'save fileName'" << std::endl;
    std::cout << "translate: 'translate id xTrans yTrans zTrans'" << std::endl;
    std::cout << "scale: 'scale id xScale yScale zScale'" << std::endl;
    std::cout << "rotate: 'rotate id p0.x p0.y p0.z p1.x p1.y p1.z angle'" << std::endl;
    std::cout << "displayContents: 'displayContents'" << std::endl;
    std::cout << "displayVertices: 'displayVertices id'" << std::endl;
    std::cout << "setLightPos: 'setLightPos x y z'" << std::endl;
    std::cout << "setLightIntensity: 'setLightPos r g b'" << std::endl;
    std::cout << "setLightAmbIntensity: 'setLightAmbIntensity r g b'" << std::endl;
    std::cout << "setK: 'setK val'" << std::endl;
    std::cout << "setPhongConstant: 'setPhongConstant val'" << std::endl;
    std::cout << "setKA: 'setKA r g b'" << std::endl;
    std::cout << "setKD: 'setKD r g b'" << std::endl;
    std::cout << "setKS: 'setKS r g b'" << std::endl;
    std::cout << "setFrom: 'setFrom x y z'" << std::endl;
    std::cout << "exit: 'exit'" << std::endl;

    std::cout << "--------------------------------------" << std::endl;
}


void Cli::cmdSetLightPos(float x, float y, float z)
{
    Vector3f vec(x, y, z);
    m_scene->m_light->m_pos = vec;
}


void Cli::cmdSetLightIntensity(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_sourceIntensity = vec;
}


void Cli::cmdSetLightAmbIntensity(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_ambIntensity = vec;
}


void Cli::cmdSetK(float K)
{
    m_scene->m_light->m_K = K;
}


void Cli::cmdSetPhongConstant(float phongConstant)
{
    m_scene->m_light->m_phongConstant = phongConstant;
}


void Cli::cmdSetKA(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_ka = vec;
}


void Cli::cmdSetKD(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_kd = vec;
}


void Cli::cmdSetKS(float r, float g, float b)
{
    Vector3f vec(r, g, b);
    m_scene->m_light->m_ks = vec;
}


void Cli::cmdSetFrom(float x, float y, float z)
{
    Vector3f vec(x, y, z);
    m_scene->m_camera->m_pos = vec;
}


void Cli::cmdDisplayVertices(int id) const
{
    m_scene->displayVertices(id);
}