#pragma once
#include <unordered_map>


class Inputs{
private:
public:
    static Keys sdl_to_keys(const SDL_Keycode& keyCode);

    std::unordered_map<Keys, bool> pressedMap;
    std::unordered_map<Keys, bool> upMap;
    std::unordered_map<Keys, bool> downMap;
    std::unordered_map<Keys, double> downTimeMap;

    r2::Vector2D<int> mousePosOnScreen;

    r2::Vec wasd_vec = r2::Vec(1,0);
    r2::Vec arrows_vec = r2::Vec(1,0);
    r2::Vec mousePosInGame;
    r2::Vec zero_vec;

    std::vector<Gamepad> gamepads;

    Inputs();

    ~Inputs();

    void keyDown(const Keys& key, double time);

    void keyUp(const Keys& key);

    void mouseMotion();

    void update(double dt, Game& game);

    void reset();
};

enum class Keys {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,
    up,down,left,right,
    space,backspace,escape,l_shift,r_shift,l_control,r_control,tab,left_button,right_button,middle_button,
    start, back, left_trigger, right_trigger, left_stick, right_stick,        //gamepad only
    none, other}