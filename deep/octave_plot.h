//
//  octave_plot.h
//  Deep
//
//  Created by Nathan Daly on 12/29/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef Deep_octave_plot_h
#define Deep_octave_plot_h

#include "GUIView.h"
#include "GUIButton.h"
#include "NathanSocket.h"

class OctavePlot : public GUIButton
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
