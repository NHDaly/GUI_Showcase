//
//  animation.h
//  Deep
//
//  Created by Nathan Daly on 12/16/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_animation_h
#define Deep_animation_h

#include "gui/GUIImageView.h"
#include "gui/GUIApp.h"

#include <vector>
#include <tr1/functional>

class Anim : public GUIImageView {
public:  
    
    Anim()
    : GUIImageView(GUIImage("images/anim0.bmp")), frame(0)
    {
        images.push_back(GUIImage("images/anim0.bmp"));
        images.push_back(GUIImage("images/anim1.bmp"));
        images.push_back(GUIImage("images/anim2.bmp"));
        images.push_back(GUIImage("images/anim3.bmp"));
        
        SDL_Color clear = {0,0xff,0};
        set_clear_color(clear);
        GUIApp::get()->repeat_on_timer(std::tr1::bind(&Anim::update, this), 0.25);
    }
    
    void update() {
        
        draw_onto_self(images[frame++], DispPoint());
        frame = frame % (int)images.size();
    }
  
private:
    std::vector<GUIImage> images;
    int frame;
};


#endif
