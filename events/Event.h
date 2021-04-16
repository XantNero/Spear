#ifndef _EVENT_H_
#define _EVENT_H_

enum class EventType {
    None = 0,
    WindowClose, WindowResize,
    KeyPress, KeyRelease, KeyType,
    MousePress, MouseRelease, MouseMove, MouseScroll
};

struct EventCategory {
    enum {
        None        = 0,
        Input       = (1 << 0),
        Keyboard    = (1 << 1),
        Mouse       = (1 << 2),
        Window      = (1 << 3)
        
    };
};

class Event
{
public:
    virtual EventType getType() const = 0;
    virtual int getCategory() const = 0;
protected:
    Event() : m_Handled(false) { }
    virtual ~Event() { }
    virtual bool isHandled() const { return m_Handled; }
    virtual void handle() { m_Handled = true; }
protected:
    bool m_Handled;
};

class EventDispatcher
{
public:
    template<typename T>
	using EventFn = std::function<bool(T&)>;
    EventDispatcher(Event &event) : m_Event(event) { }
    template<typename T>
    bool dispatch(EventFn<T> func) {
        if (m_Event.getType() == T::getStaticType()) {
            func(m_Event);
            return true;
        }
        return false;
    }
private:
    Event& m_Event;
};

#endif