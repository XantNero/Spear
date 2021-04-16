#ifndef _KEYEVENT_H_
#define _KEYEVENT_H_

#include "Event.h"


class KeyEvent : public Event
{
public:
    inline virtual int getCategory() const override
        { return  EventCategory::Input | EventCategory::Keyboard; };
    inline int getKey() const { return m_KeyCode; }
protected:
    KeyEvent(int keyCode) : m_KeyCode(keyCode) { }
    ~KeyEvent() { }
protected:
    int m_KeyCode;
};

class KeyPressEvent : public KeyEvent
{
public:
    KeyPressEvent(int keyCode, int repeatCount)
        : KeyEvent(keyCode), m_RepeatCount(repeatCount) { }
    ~KeyPressEvent() { }
    static inline EventType getStaticType() 
        { return EventType::KeyPress; }
    inline virtual EventType getType() const override
        { return getStaticType(); }  
    inline int getRepeatCount() const { return m_RepeatCount; }  
private:
    int m_RepeatCount;
};

class KeyReleaseEvent : public KeyEvent
{
public:
    KeyReleaseEvent(int keyCode) : KeyEvent(keyCode) { }
    ~KeyReleaseEvent() { }
    static inline EventType getStaticType() 
        { return EventType::KeyRelease; }
    inline virtual EventType getType() const override
        { return getStaticType(); }  
};

class KeyTypeEvent : public KeyEvent
{
public: //TODO:set unknown keycode
    KeyTypeEvent(unsigned int codePoint, int keyCode = 0)
        : KeyEvent(keyCode), m_CodePoint(codePoint) { }
    ~KeyTypeEvent() { }
    static inline EventType getStaticType() 
        { return EventType::KeyType; }
    inline virtual EventType getType() const override
        { return getStaticType(); }  
    inline unsigned int getCodePoint() const { return m_CodePoint; }
private:
    unsigned int m_CodePoint;
};

#endif