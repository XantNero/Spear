#ifndef _IMGUILAYER_H_
#define _IMGUILAYER_H_

class ImguiLayer
{
public:
    ImguiLayer();
    ~ImguiLayer();
    void onAttach();
    void onDetach();
    void begin();
    void end();
    void onImguiRender();
};

#endif