//
//  octave_plot.cpp
//  Deep
//
//  Created by Nathan Daly on 12/29/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include <iostream>
#include "octave_plot.h"

#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>


using namespace std;

OctavePlot::OctavePlot() 
//: NewGUIView(10,10)
{
    char *argv[2] = {"embedded", "-q"};
//    char *argv[0] = {"embedded", "-q"};
    
    octave_main(2,argv, 1);  
//    return embedded;
    
    cout << "HI!" << endl;

}

