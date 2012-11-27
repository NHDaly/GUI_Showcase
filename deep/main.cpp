//
//  main.cpp
//  Deep
//
//  Created by Nathan Daly on 8/23/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "SDL/SDL.h"
#include "gui/GUIWin_Ctrl.h"
#include "gui/GUIWindow.h"
#include "gui/Compatibility.h"
#include "gui/GUIView.h"
#include "gui/GUIButton.h"
#include "gui/GUIValBox.h"

#include "gui/NewGUIView.h"

#include <iostream>
using namespace std;

int user_main (int argc, char **argv);

class QuitAction {
public:
    void operator()() {
        throw QuitAction();
    }
};

int user_main (int argc, char **argv) {
    
    GUIWin_Ctrl& win_ctrl = *GUIWin_Ctrl::get();

    GUIView_shptr_t button(new GUIButton<QuitAction>(178,22,
                                                     "images/button.bmp"));
    win_ctrl.get_window()->attach(button, DispPoint(20,20));
    
    GUIView_shptr_t slider(new GUIValue_Horiz_Slider(100));
    win_ctrl.get_window()->attach(slider, DispPoint(300,300));
    
    GUIView_shptr_t slider_view(new GUIValue_Display(200, 100,
                                                     static_cast<GUIValue_Box*>(slider.get())));
    win_ctrl.get_window()->attach(slider_view, DispPoint(300,350));

    
    win_ctrl.run();

    return 0;
}

#include "gui/NewGUIView.h"
#include "gui/NewGUIImageView.h"
#include "gui/NewGUIWindow.h"


struct FakeGUIView {
public:
    // Returns true if the mouse_down is finished being handled.
    // If returns false, handling will continue up the chain.
    // May optionally call capture_focus() to become the target for keypresses.
    virtual bool handle_mouse_down(DispPoint coord) { return false; }
    
    // These functions will be called by capture/lose focus, and may be
    // overridden to provide behavior on focus gain/loss.
    virtual void got_focus() { }
    virtual void lost_focus() { }

    bool changed;
    int w,h;
    DispPoint pos;
    
    NewGUIView* background;
    SDL_Surface* image;
    SDL_Surface* display;   // includes children drawn on.
    
    // Hierarchy
    NewGUIView* parent;
    typedef std::list<NewGUIView*> Subview_list_t;
    Subview_list_t children;
};

int test(int argc, char **argv) {
    
    NewGUIWindow window(600,600, "Window 1");
    
    GUIImage bg = GUIImage::create_blank(500,500);
    SDL_FillRect(bg, 0, SDL_MapRGB(bg->format, 155, 155, 155));

    GUIImage bg2 = GUIImage::create_blank(200,200);
    SDL_FillRect(bg2, 0, SDL_MapRGB(bg2->format, 255, 100, 100));

    NewGUIView* nv = new NewGUIImageView(bg);
    NewGUIView* nv1 = new NewGUIView(50,50);
    NewGUIView* nv2 = new NewGUIImageView(bg2);
    NewGUIView* nv3 = new NewGUIView(20,20);
    
    window.set_main_view(nv);
    
    nv->attach_subview(nv1, DispPoint(10,10));
    //    nv->attach_subview(nv2, DispPoint(30,10));
    
    cout << "refresh one: should draw nv1 onto nv" << endl;
    window.refresh();
    
    nv2->draw_onto_self(bg2, DispPoint());
    cout << "refresh two ought to do nothing.." << endl;
    window.refresh();
    
    nv->attach_subview(nv2, DispPoint(100,100));
    nv->move_subview(nv2, DispPoint(30,30));
    cout << "refresh three: should draw nv1 AND nv2 onto nv" << endl;
    window.refresh();
    
    
    
//    display_image(nv->image, GUIWin_Ctrl::get()->get_window()->screen,
//                  DispPoint(), true);
    window.refresh();
//    SDL_Delay(2000);
    
    
    NewGUIImageView *bubble = new NewGUIImageView(GUIImage("images/slider_bubble.bmp"));
    nv2->attach_subview(bubble, DispPoint(120, 100));

    nv2->attach_subview(nv3, DispPoint(180, 150));

    window.refresh();
//    display_image(nv->image, GUIWin_Ctrl::get()->get_window()->screen,
//                  DispPoint(), true);
    window.refresh();
   
    
    bool running = true;
    while(running) {
        SDL_Event event;
        
        try {
            while (SDL_PollEvent(&event) && running){
                switch (event.type) {
                    case SDL_MOUSEBUTTONDOWN:{
                        
                        SDL_MouseButtonEvent click = event.button;
                        NewGUIView* clicked_view =
                        window.get_main_view()->
                        get_view_from_point(DispPoint(click.x, click.y));
                        
                        if (clicked_view) {
                            ((FakeGUIView*)bubble)->parent->remove_subview(bubble);
                            //                        NewGUIImageView *bubble = new NewGUIImageView(GUIImage("images/slider_bubble.bmp"));
                            clicked_view->attach_subview(bubble, DispPoint(click.x, click.y));
                        }
                        cout << clicked_view << endl;
                        window.refresh();
                        
                    }
                }
            }
        }
        catch(const Error& e) {
            cout << e.msg << endl;
        }
    }
    
    
    
    nv->remove_subview(nv2);
    
    cout << "refresh three: should draw nv1 onto nv" << endl;
    window.refresh();
    cout << "refresh 4-7 should do nothing" << endl;
    window.refresh();
    
    window.refresh();
    window.refresh();
//    display_image(nv->image, GUIWin_Ctrl::get()->get_window()->screen,
//                  DispPoint(), true);
    window.refresh();
    SDL_Delay(2000);
    
    delete nv2;
    delete nv;
    
    return 0;
}


int main (int argc, char **argv) {
 
    try {
        
        initGame();
//        GUIWin_Ctrl& win_ctrl = *GUIWin_Ctrl::get();
//        GUIView_shptr_t main_screen(new GUIView);
//        win_ctrl.get_window()->attach(main_screen, DispPoint(0,0));
        
        return test(argc, argv);
    }
    catch (const QuitAction& q) {
        cout << "Goodbye!" << endl;
    }
//    catch (const Error& e) {
//        cout << "Error: " << e.msg << endl;
//        throw;
//    }
//    catch (...) {
//        cout << "Unkown Error." << endl;
//        throw;
//    }
    
    return 0;
}
