//
//  cat_face.cpp
//  Deep
//
//  Created by Nathan Daly on 12/16/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include <iostream>
#include "cat_face.h"

SDL_Color cat_green = {0, 250, 0, 0};


bool Eye::handle_mouse_motion(DispPoint coord, DispPoint rel_motion) {
    
    DispPoint new_pos = coord;
    if (new_pos.x < 0) new_pos.x = 0;
    if (new_pos.y < 0) new_pos.y = 0;
    if (new_pos.x >= get_w() - pupil->get_w()) new_pos.x = get_w() - pupil->get_w();
    if (new_pos.y >= get_h() - pupil->get_h()) new_pos.y = get_h() - pupil->get_h();
    
    move_subview(pupil, new_pos);
    
    return false;
}

