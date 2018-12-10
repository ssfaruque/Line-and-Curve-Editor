#ifndef APP_H
#define APP_H

class Window;
class Scene;
class Cli;

class App
{
    private:
        bool m_running;
        Window* m_window;
        Scene* m_scene;
        Cli* m_cli;
        static App* m_app;

    private:
        App(const char* title, const int width, const int height,
            int* argc, char** argv);

    public:
        static void create(const char* title, const int width, const int height,
                           int* argc, char** argv);
        static App* getInstance();

        ~App();
        inline bool isRunning() const {return m_running;};
        void run();

        friend void renderScene();
        friend void renderProjections();

};


#endif  // APP_H