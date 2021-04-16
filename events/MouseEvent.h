#ifndef _MOUSEEVENT_H_
#define _MOUSEEVENT_H_

#include "Event.h"

class MouseEvent : public Event
{
public:
    inline virtual int getCategory() const override
        { return  EventCategory::Input | EventCategory::Mouse; };
    inline float getPosX() const { return m_PosX; } 
    inline float getPosY() const { return m_PosY; }
protected:
    MouseEvent(float x, float y) : m_PosX(x), m_PosY(y) { }
    ~MouseEvent() { }
protected:
    float m_PosX;
    float m_PosY;
};

class MouseButtonEvent : public MouseEvent
{
public:
    inline int getButton() const { return m_Button; }
protected:
    MouseButtonEvent(int button, float x, float y)
        : MouseEvent(x, y), m_Button(button) { }
    ~MouseButtonEvent() { }
protected:
    int m_Button;
};

class MousePressEvent : public MouseButtonEvent
{
public:
    MousePressEvent(int button, float x, float y)
        : MouseButtonEvent(button, x, y) { }
    ~MousePressEvent() { }
    static inline EventType getStaticType() 
        { return EventType::MousePress; }
    inline virtual EventType getType() const override
        { return getStaticType(); }    
};

class MouseReleaseEvent : public MouseButtonEvent
{
public:
    MouseReleaseEvent(int button, float x, float y)
        : MouseButtonEvent(button, x, y) { }
    ~MouseReleaseEvent();
    static inline EventType getStaticType() 
        { return EventType::MouseRelease; }
    inline virtual EventType getType() const override
        { return getStaticType(); }  
};

class MouseMoveEvent : public MouseEvent
{
public:
    MouseMoveEvent(float x, float y) : MouseEvent(x, y) { }
    ~MouseMoveEvent() { }
    static inline EventType getStaticType() 
        { return EventType::MouseMove; }
    inline virtual EventType getType() const override
        { return getStaticType(); }  
};

class MouseScrollEvent : public MouseEvent
{
public:
    MouseScrollEvent(float xOffset, float yOffset, float x = 0, float y = 0) 
        : MouseEvent(x, y), m_XOffset(xOffset), m_YOffset(yOffset) { }
    ~MouseScrollEvent();
    static inline EventType getStaticType() 
        { return EventType::MouseScroll; }
    inline virtual EventType getType() const override
        { return getStaticType(); }  
    inline float getXOffset() const { return m_XOffset; }
    inline float getYOffset() const { return m_YOffset; }
private:
    float m_XOffset;
    float m_YOffset;
};

#endif