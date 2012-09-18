//
//  test.cpp
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include <iostream>
using namespace std;

#include "gui/NewGUIView.h"
#include "SDL/SDL.h"

int main(int argc, char **argv) {
    
    
    NewGUIView* nv = new NewGUIView(10,100);
    NewGUIView* nv1 = new NewGUIView(50,50);
    NewGUIView* nv2 = new NewGUIView(200,200);
    
    nv->attach_subview(nv1, DispPoint(10,10));
    //    nv->attach_subview(nv2, DispPoint(30,10));
    
    nv->print_children();
    cout << "refresh one: should draw nv1 onto nv" << endl;
    nv->refresh();
    
    nv2->draw_onto_self(nv2, DispPoint());
    cout << "refresh two ought to do nothing.." << endl;
    nv->refresh();
    
    nv->attach_subview(nv2, DispPoint(100,100));
    nv->move_subview(nv2, DispPoint(0,0));
    cout << "refresh three: should draw nv1 AND nv2 onto nv" << endl;
    nv->refresh();

    nv->print_children();

    nv->remove_subview(nv2);

    cout << "refresh three: should draw nv1 onto nv" << endl;
    nv->refresh();
    cout << "refresh 4-7 should do nothing" << endl;
    nv->refresh();

    nv->refresh();
    nv->refresh();

    delete nv2;
    delete nv;
    
    return 0;
}
