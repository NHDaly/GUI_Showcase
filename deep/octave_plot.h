//
//  octave_plot.h
//  Deep
//
//  Created by Nathan Daly on 12/29/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_octave_plot_h
#define Deep_octave_plot_h

#include "gui/GUIView.h"
#include "gui/GUIButton.h"
#include "NathanSocket.h"

class OctavePlot : public GUI::Button
{
public:
    
    OctavePlot();
    
    void rand();
    
private:
    Socket_Client client;
    
    virtual void operation() {
        rand();
    }

};

#endif
