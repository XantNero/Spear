#ifndef _WINDOWEVENT_H_
#define _WINDOWEVENT_H_

#include "Event.h"

class WindowEvent : public Event
{
public:
    inline virtual int getCategory() const override
        { return  EventCategory::Window; };
    inline unsigned int getWidth() const { return m_Width; }
    inline unsigned int getHeight() const { return m_Height; }
protected:
    WindowEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height) { }
    ~WindowEvent() { }
protected:
    unsigned int m_Width;
    unsigned int m_Height;
};

class WindowResizeEvent : public WindowEvent
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : WindowEvent(width, height) { }
    ~WindowResizeEvent() { }
    static inline EventType getStaticType() 
        { return EventType::WindowResize; }
    inline virtual EventType getType() const override
        { return getStaticType(); }    
};

class WindowCloseEvent : public WindowEvent
{
public:
    WindowCloseEvent() : WindowEvent(0, 0) { }
    ~WindowCloseEvent() { }
    static inline EventType getStaticType() 
        { return EventType::WindowClose; }
    inline virtual EventType getType() const override
        { return getStaticType(); }  
};

#endif