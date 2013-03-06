//
//  main.cpp
//  Deep
//
//  Created by Nathan Daly on 8/23/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "SDL/SDL.h"
#include "gui/GUIWindow.h"
#include "gui/Compatibility.h"
#include "gui/GUIView.h"
#include "gui/GUIButton.h"
#include "gui/GUIValBox.h"

#include "gui/GUIApp.h"
#include "gui/GUIView.h"
#include "gui/GUIButton.h"
#include "gui/GUIScrollView.h"

#include "cat_face.h"
#include "animation.h"

#include <iostream>
#include <tr1/functional>
using namespace std;
using namespace std::tr1;

int user_main (int argc, char **argv);
int test(int argc, char **argv);


#include "gui/GUIView.h"
#include "gui/GUIImageView.h"
#include "gui/GUIWindow.h"
#include "gui/GUITextViews.h"
#include "gui/GUIValBox.h"

#include "SDL/SDL_video.h"

class ReturnButton : public GUITextButton {
public:
    ReturnButton(GUIWindow *window_) 
    :GUITextButton("Return! Yay!"), window(window_) { }
protected:
    virtual void operation() {
        window->remove_last_subview();
    }
private:
    GUIWindow *window;
};


struct Call {
    Call(GUIValue_Slider *slider, GUIValue_Box *text)
    :sl(slider), tx(text) {}
    
    void operator()() {
        cout << "SETTING VALUE" << endl;
        sl->set_new_value(tx->get_value());
    }
private:
    GUIValue_Slider *sl;
    GUIValue_Box *tx;
    
};

class GUIQuitButton : public GUITextButton {
public:
    GUIQuitButton() : GUITextButton("QUIT") {}
    
    virtual void operation() {
        throw QuitAction();
    }
};

void print_hello() {
    cout << "Hello!" << endl;
}

struct Quit_ {};
template <typename T>
void print_goodbye(T t) {
    cout << "Goodbye!" << endl;
    throw GUIQuit();
}
struct PrintGoodbye {
    void operator()(Quit_ q) {
        cout << "Goodbye!" << endl;
        throw GUIQuit();
    }
};

//#include "octave_plot.h"

int test(int argc, char **argv) {
    
    
    GUIApp::get()->register_exception_handler<Quit_>(&print_goodbye<Quit_>);
    
    GUIApp::get()->set_framerate_cap(40);
    
    GUIWindow window(600,600, "Window 1");
    
    
    GUIImage bg = GUIImage::create_blank(500,500);
    SDL_FillRect(bg, 0, SDL_MapRGB(bg->format, 155, 155, 155));

    GUIImage bg2 = GUIImage::create_blank(200,200);
    SDL_FillRect(bg2, 0, SDL_MapRGB(bg2->format, 255, 100, 100));

    /*** CREATE NV ***/
    GUIView* nv = new GUIImageView(bg);
    GUIView* nv1 = new GUIView(50,50);
    GUIView* nv2 = new GUIImageView(bg2);
    GUIView* nv3 = new GUIView(20,20);
        
    
//    GUIWindow_Stub stub(0, 500, 500, "test");
//    
//    //    stub.rename("NEW NAME");
//    
//    stub.render_on_screen(nv);
//    
//    return 0;
//    
//    //    GUIApp::get()->run(&stub);

//    OctavePlot * plot = new OctavePlot;
//    plot->rand();
//    
//    nv->attach_subview(plot, DispPoint(50,300));
    
    
    nv->attach_subview(nv1, DispPoint(10,10));
    nv->attach_subview(nv2, DispPoint(100,100));
    nv->move_subview(nv2, DispPoint(30,30));
    
    
    nv2->attach_subview(new GUIImageView(GUIImage("images/slider_bubble.bmp")),
                        DispPoint(120, 100));

    nv2->attach_subview(nv3, DispPoint(180, 150));
   
    nv->attach_subview(new GUIQuitButton, DispPoint(440,200));
    
    nv->attach_subview(new Anim, DispPoint(150,400));

    nv->attach_subview(new GUIScrollView(100,300,
                                         new GUIImageView(GUIImage("images/coins_screen_shot.bmp"))), DispPoint(250,50));
    nv->attach_subview(new GUIScrollView(100,300,
                                         new GUIImageView(GUIImage("images/cat_face_bg.bmp"))), DispPoint(375,0));
    
    // A view can capture focus without ever being attached to a window!
    GUIView view(100,100);
    view.capture_focus();

    
    /*** CREATE SECOND_VIEW ***/
    
    GUIImage bg_full = GUIImage::create_blank(window.get_dim().x, window.get_dim().y);
    SDL_FillRect(bg_full, 0, SDL_MapRGB(bg->format, 155, 155, 155));
    GUIView *scd_view = new GUIImageView(bg_full);
    

    scd_view->attach_subview(new ReturnButton(&window), DispPoint(300,300));
    
    SDL_Color light = {0xdd, 0xdd, 0xdd};
    GUIView *text_view = new GUIImageView(GUIImage::create_filled(200, 300, light));
//    text_view->attach_subview(new GUIScrollView(200,300, new GUITextBox(200, 310, true)), DispPoint());
    text_view->attach_subview(new GUIScrollView(200,300, new GUITextBox(200, 310)), DispPoint());
//    text_view->attach_subview(new GUITextBox(150, 150, false, true), DispPoint());
    
    scd_view->attach_subview(text_view, DispPoint(20, 200));
    
    GUIValue_Horiz_Slider *val0 = new GUIValue_Horiz_Slider(100);
    val0->set_range(10, 40);
    scd_view->attach_subview(val0, DispPoint(20,30));
    scd_view->attach_subview(new GUIValue_Display(400, 30, val0), DispPoint(20,100));

    GUIValue_Box *tb = new GUIValue_Text_Box(400, 30);
    scd_view->attach_subview(tb, DispPoint(300,100));

    
    scd_view->attach_subview(GUI_create_button(bind(&GUIValue_Slider::set_new_value, val0,
                                                       bind(&GUIValue_Box::get_value, tb))), DispPoint(300,200));

    scd_view->attach_subview(new CatFace, DispPoint(400,400));

    
    /*** Link the views ***/

    class GoToViewButton : public GUITextButton {
    public:
        GoToViewButton(GUIWindow *window_, GUIView *view_, DispPoint point_ = DispPoint(0,0))
        : GUITextButton("Go To View"), window(window_), view(view_), point(point_) { }
    protected:
        virtual void operation() {
            window->attach_subview(view, point);
        }
    private:
        GUIWindow *window;
        GUIView *view;
        DispPoint point;
    };

    nv->attach_subview(new GoToViewButton(&window, scd_view), DispPoint(200,400));
    
    
    /*** Run nv ***/
    window.attach_subview(nv, DispPoint(10,10));

    GUIApp::get()->run(&window);
    
    
    nv->remove_subview(nv2);
    window.remove_subview(nv);

    delete nv2;
    delete nv;

    return 0;
}


int main (int argc, char **argv) {

    initGUI();
    initSDL(SDL_INIT_EVERYTHING);
//  return user_main(argc, argv);
    
    return test(argc, argv);

}
