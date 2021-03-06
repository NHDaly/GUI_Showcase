//
//  cat_face.h
//  Deep
//
//  Created by Nathan Daly on 12/16/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_cat_face_h
#define Deep_cat_face_h

#include "GUI/GUIImageView.h"

using GUI::DispPoint;

extern SDL_Color cat_green;

class Eye : public GUI::ImageView {
public:
    
    Eye() 
    : GUI::ImageView(GUIImage("images/cat_eye_bg.bmp")),
    pupil(new GUI::ImageView(GUIImage("images/cat_eye_pupil.bmp"))) {
        set_clear_color(cat_green);
        capture_focus();
        pupil->set_clear_color(cat_green);
        attach_subview(pupil, DispPoint());
    }
    
protected:
    virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);
    
private:
    GUI::ImageView *pupil;
                       
};

class CatFace : public GUI::View {
public:
    
    CatFace()
    : GUI::View(200,200)
    {
        set_clear_color(cat_green);
        draw_onto_self(GUIImage("images/cat_face_bg.bmp"), DispPoint());
        attach_subview(new Eye, DispPoint(30, 50));
        attach_subview(new Eye, DispPoint(130, 50));
    }
    
    
    virtual bool handle_mouse_up(DispPoint coord) {
        
        if (get_w() == 200) 
            resize(coord.x,coord.y);
        else 
            resize(200, 200);
        mark_changed();
        draw_onto_self(GUIImage("images/cat_face_bg.bmp"), DispPoint());
               
        return true;
    }
    
};


#endif
