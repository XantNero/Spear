#include "ModelApplication.h"

//TODO: timestep
//TODO: proper zoom and camera movement

int main(int argc, const char* argv[])
{
    srand(time(NULL));
    ModelApplication app(argv[1]);
    app.run();
    return 0;
}