//
//  octave_plot.cpp
//  Deep
//
//  Created by Nathan Daly on 12/29/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//


#include "octave_plot.h"

#include <iostream>


using namespace std;

#define kOctavePlotSocket 12123


OctavePlot::OctavePlot() 
: client(kOctavePlotSocket)
{
    client.connect();
}

void OctavePlot::rand() 
{   
    // message
    string size = "4";
    size.append("\0", 1);
    client.send(size);
    client.send("rand");

    // num args
    size = "1";
    size.append("\0", 1);
    client.send(size);
    
    // args
    size = "5";
    size.append("\0", 1);
    client.send(size);
    client.send("state");
    
    
    
}
