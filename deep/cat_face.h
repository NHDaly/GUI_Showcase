//
//  cat_face.h
//  Deep
//
//  Created by Nathan Daly on 12/16/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_cat_face_h
#define Deep_cat_face_h

#include "gui/NewGUIImageView.h"

extern SDL_Color cat_green;

class Eye : public NewGUIImageView {
public:
    
    Eye() 
    : NewGUIImageView(GUIImage("images/cat_eye_bg.bmp")),
    pupil(new NewGUIImageView(GUIImage("images/cat_eye_pupil.bmp"))) {
        set_clear_color(cat_green);
        capture_focus();
        pupil->set_clear_color(cat_green);
        attach_subview(pupil, DispPoint());
    }
    
protected:
    virtual bool handle_mouse_motion(DispPoint coord, DispPoint rel_motion);
    
private:
    NewGUIImageView *pupil;
                       
};

class CatFace : public NewGUIView {
public:
    
    CatFace()
    : NewGUIView(200,200)
    {
        set_clear_color(cat_green);
        draw_onto_self(GUIImage("images/cat_face_bg.bmp"), DispPoint());
        attach_subview(new Eye, DispPoint(30, 50));
        attach_subview(new Eye, DispPoint(130, 50));
    }
    
    
    
    
};


#endif