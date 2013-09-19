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
#include "GUI/GUIView.h"
#include "GUI/GUIButton.h"
#include "GUI/GUIScrollView.h"

#include "cat_face.h"
#include "animation.h"

#include <iostream>
#include <tr1/functional>
using namespace std;
using namespace std::tr1;

int user_main (int argc, char **argv);
int test(int argc, char **argv);
void print_bye(GUI::Quit);


#include "gui/GUIView.h"
#include "gui/GUIImageView.h"
#include "gui/GUIWindow.h"
#include "gui/GUITextViews.h"
#include "gui/GUIValBox.h"
#include "gui/NewNewGUITextView.h"

#include "SDL/SDL_video.h"

class ReturnButton : public GUI::TextButton {
public:
    ReturnButton(GUI::Window *window_) 
    :GUI::TextButton("Return! Yay!"), window(window_) { }
protected:
    virtual void operation() {
        window->remove_last_subview();
    }
private:
    GUI::Window *window;
};


struct Call {
    Call(GUI::Value_Slider *slider, GUI::Value_Box *text)
    :sl(slider), tx(text) {}
    
    void operator()() {
        cout << "SETTING VALUE" << endl;
        sl->set_new_value(tx->get_value());
    }
private:
    GUI::Value_Slider *sl;
    GUI::Value_Box *tx;
    
};

class GUIQuitButton : public GUI::TextButton {
public:
    GUIQuitButton() : GUI::TextButton("QUIT") {}
    
    virtual void operation() {
        throw GUI::Quit();
    }
};

void print_hello() {
    cout << "Hello!" << endl;
}

void print_bye(GUI::Quit) {
    cout << "Goodbye!" << endl;
}

template <typename T>
void print_goodbye(T) {
    cout << "Goodbye!" << endl;
}
struct PrintGoodbye {
    void operator()(GUI::Quit) {
        cout << "Goodbye!" << endl;
    }
};

//#include "octave_plot.h"

int test(int argc, char **argv) {
    
    
    GUI::App::get()->register_exception_handler<GUI::Quit>(&print_bye);
    
    GUI::App::get()->set_framerate_cap(40);
    
    GUI::Window window(600,600, "Window 1");
    
    
    GUIImage bg = GUIImage::create_blank(500,500);
    SDL_FillRect(bg, 0, SDL_MapRGB(bg->format, 155, 155, 155));

    GUIImage bg2 = GUIImage::create_blank(200,200);
    SDL_FillRect(bg2, 0, SDL_MapRGB(bg2->format, 255, 100, 100));

    /*** CREATE NV ***/
    GUI::View* nv = new GUI::ImageView(bg);
    GUI::View* nv1 = new GUI::View(50,50);
    GUI::View* nv2 = new GUI::ImageView(bg2);
    GUI::View* nv3 = new GUI::View(20,20);
        
    
//    GUI::Window_Stub stub(0, 500, 500, "test");
//    
//    //    stub.rename("NEW NAME");
//    
//    stub.render_on_screen(nv);
//    
//    return 0;
//    
//    //    GUI::App::get()->run(&stub);

//    OctavePlot * plot = new OctavePlot;
//    plot->rand();
//    
//    nv->attach_subview(plot, DispPoint(50,300));
    
    
    nv->attach_subview(nv1, DispPoint(10,10));
    nv->attach_subview(nv2, DispPoint(100,100));
    nv->move_subview(nv2, DispPoint(30,30));
    
    
    nv2->attach_subview(new GUI::ImageView(GUIImage("images/slider_bubble.bmp")),
                        DispPoint(120, 100));

    nv2->attach_subview(nv3, DispPoint(180, 150));
   
    nv->attach_subview(new GUIQuitButton, DispPoint(440,200));
    
    nv->attach_subview(new Anim, DispPoint(150,400));

    nv->attach_subview(new GUI::ScrollView(100,300,
                                         new GUI::ImageView(GUIImage("images/coins_screen_shot.bmp"))), DispPoint(250,50));
    nv->attach_subview(new GUI::ScrollView(100,300,
                                         new GUI::ImageView(GUIImage("images/cat_face_bg.bmp"))), DispPoint(375,0));
    
    // A view can capture focus without ever being attached to a window!
    GUI::View view(100,100);
    view.capture_focus();

    
    /*** CREATE SECOND_VIEW ***/
    
    GUIImage bg_full = GUIImage::create_blank(window.get_dim().x, window.get_dim().y);
    SDL_FillRect(bg_full, 0, SDL_MapRGB(bg->format, 155, 155, 155));
    GUI::View *scd_view = new GUI::ImageView(bg_full);
    

    scd_view->attach_subview(new ReturnButton(&window), DispPoint(300,300));
    
    int text_box_h = 200;
    SDL_Color light = {0xdd, 0xdd, 0xdd};

    GUI::View *text_bg_view = new GUI::ScrollView(200,text_box_h, new GUI::TextBox(200, text_box_h+10));
                                                  
//    GUI::View *text_bg_view = new GUI::ImageView(GUIImage::create_filled(200, text_box_h, light));
////    text_view->attach_subview(new GUI::ScrollView(200,300, new GUI::TextBox(200, 310, true)), DispPoint());
//    text_bg_view->attach_subview(new GUI::ScrollView(200,text_box_h, new GUI::TextBox(200, text_box_h+10)), DispPoint());
////    text_view->attach_subview(new GUI::TextBox(150, 150, false, true), DispPoint());
    
    scd_view->attach_subview(text_bg_view, DispPoint(20, 200));

    // New New TextView Test
    {
        GUI::View *text_bg_2 = new GUI::ImageView(GUIImage::create_filled(100, 100, light));
        GUI::NewTextView *new_text_view = new GUI::NewTextView(100,100);
        text_bg_2->attach_subview(new_text_view, DispPoint());
        
        new_text_view->set_text("HI!");
        
        scd_view->attach_subview(text_bg_2, DispPoint(20, 420));
    }
    
    // vs. Old TextView
    {
        GUI::View *text_bg_2 = new GUI::ImageView(GUIImage::create_filled(100, 100, light));
        GUI::TextView *new_text_view = new GUI::TextView(100,100);
        text_bg_2->attach_subview(new_text_view, DispPoint());
        
        new_text_view->set_text("HI!");
        
        scd_view->attach_subview(text_bg_2, DispPoint(220, 420));
    }
    
    
    GUI::Value_Horiz_Slider *val0 = new GUI::Value_Horiz_Slider(100);
    val0->set_range(10, 40);
    scd_view->attach_subview(val0, DispPoint(20,30));
    scd_view->attach_subview(new GUI::Value_Display(400, 30, val0), DispPoint(20,100));

    GUI::Value_Box *tb = new GUI::Value_Text_Box(400, 30);
    scd_view->attach_subview(tb, DispPoint(300,100));

    
    scd_view->attach_subview(GUI::create_button(bind(&GUI::Value_Slider::set_new_value, val0,
                                                       bind(&GUI::Value_Box::get_value, tb))), DispPoint(300,200));

    scd_view->attach_subview(new CatFace, DispPoint(400,400));

    
    /*** Link the views ***/

    class GoToViewButton : public GUI::TextButton {
    public:
        GoToViewButton(GUI::Window *window_, GUI::View *view_, DispPoint point_ = DispPoint(0,0))
        : GUI::TextButton("Go To View"), window(window_), view(view_), point(point_) { }
    protected:
        virtual void operation() {
            window->attach_subview(view, point);
        }
    private:
        GUI::Window *window;
        GUI::View *view;
        DispPoint point;
    };

    nv->attach_subview(new GoToViewButton(&window, scd_view), DispPoint(200,400));
    
    
    /*** Run nv ***/
    window.attach_subview(nv, DispPoint(10,10));

    GUI::App::get()->run(&window);
    
    
    nv->remove_subview(nv2);
    window.remove_subview(nv);

    delete nv2;
    delete nv;

    return 0;
}


int main (int argc, char **argv) {

    initGUI();
//  return user_main(argc, argv);
    
    return test(argc, argv);

}
