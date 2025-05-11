#ifndef XBOX_H
#define XBOX_H

#include <SDL2/SDL.h>

struct POVState {
    float forward = 0.f;
    float strafe = 0.f;
    float vertical = 0.f;
    float yaw = 0.f;
    float pitch = 0.f;
    float roll = 0.f;
};

class XboxController {
public:
    XboxController();
    ~XboxController();
    bool init();
    void update();
    void printState() const;
    bool isRunning() const;

private:
    float normalize_axis(Sint16 value);
    SDL_GameController* controller = nullptr;
    SDL_Event event;
    bool running = true;
    POVState state;
};

#endif

