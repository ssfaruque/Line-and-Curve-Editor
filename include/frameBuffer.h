#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "types.h"

class FrameBuffer
{
    private:
        u32* m_buffer;  // index 0 starts at lower left hand corner of screen
        float* m_depthBuffer;
        uint m_width;
        uint m_height;

        uint m_viewportX;
        uint m_viewportY;
        uint m_viewportWidth;
        uint m_viewportHeight;

    public:
        FrameBuffer(uint width, uint height);
        ~FrameBuffer();
        
        void setPixel(uint x, uint y, u32 data);
        void setDepth(uint x, uint y, float depth);
        float getDepth(uint x, uint y);
        inline u32* getBuffer() const {return m_buffer;}
        inline uint getWidth()  const {return m_width;}
        inline uint getHeight() const {return m_height;}
        void clear(Color color = {0});
        void resizeBuffer(uint width, uint height);

        void setViewport(uint x, uint y, uint w, uint h);
        inline uint getViewportWidth()  const {return m_viewportWidth;}
        inline uint getViewportHeight() const {return m_viewportHeight;}

    friend class Window;
};



#endif  // FRAME_BUFFER_H