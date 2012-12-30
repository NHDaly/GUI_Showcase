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

#include "gui/NewGUIApp.h"
#include "gui/NewGUIView.h"
#include "gui/NewGUIButton.h"

#include "cat_face.h"
#include "animation.h"

#include <iostream>
#include <tr1/functional>
using namespace std;
using namespace std::tr1;

int user_main (int argc, char **argv);
int test(int argc, char **argv);


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
    

//    GUIView_shptr_t int_text_box(new GUIInteger_Text_Box(200, 100));
//    win_ctrl.get_window()->attach(int_text_box, DispPoint(500,350));
    
//    SDL_ShowCursor(1 - SDL_ShowCursor(SDL_QUERY));
//    cout << "SDL_ShowCursor(SDL_QUERY) " << SDL_ShowCursor(SDL_QUERY) << endl;

    win_ctrl.run();

    return 0;
}

#include "gui/NewGUIView.h"
#include "gui/NewGUIImageView.h"
#include "gui/NewGUIWindow.h"
#include "gui/NewGUITextBox.h"
#include "gui/NewGUIValBox.h"

#include "SDL/SDL_video.h"

class ReturnButton : public NewGUITextButton {
public:
    ReturnButton(NewGUIWindow *window_) 
    :NewGUITextButton("Return! Yay!"), window(window_) { }
protected:
    virtual void operation() {
        window->remove_last_subview();
    }
private:
    NewGUIWindow *window;
};


struct Call {
    Call(NewGUIValue_Slider *slider, NewGUIValue_Box *text)
    :sl(slider), tx(text) {}
    
    void operator()() {
        cout << "SETTING VALUE" << endl;
        sl->set_new_value(tx->get_value());
    }
private:
    NewGUIValue_Slider *sl;
    NewGUIValue_Box *tx;
    
};

class NewGUIQuitButton : public NewGUITextButton {
public:
    NewGUIQuitButton() : NewGUITextButton("QUIT") {}
    
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
    void operator()(QuitAction q) {
        cout << "Goodbye!" << endl;
        throw GUIQuit();
    }
};

int test(int argc, char **argv) {
    
    
    NewGUIApp::get()->register_error_handler<QuitAction>(&print_goodbye<QuitAction>);
    
    
    NewGUIWindow window(600,600, "Window 1");


    GUIImage bg = GUIImage::create_blank(500,500);
    SDL_FillRect(bg, 0, SDL_MapRGB(bg->format, 155, 155, 155));

    GUIImage bg2 = GUIImage::create_blank(200,200);
    SDL_FillRect(bg2, 0, SDL_MapRGB(bg2->format, 255, 100, 100));

    /*** CREATE NV ***/
    NewGUIView* nv = new NewGUIImageView(bg);
    NewGUIView* nv1 = new NewGUIView(50,50);
    NewGUIView* nv2 = new NewGUIImageView(bg2);
    NewGUIView* nv3 = new NewGUIView(20,20);
        
    
//    GUIWindow_Stub stub(0, 500, 500, "test");
//    
//    //    stub.rename("NEW NAME");
//    
//    stub.render_on_screen(nv);
//    
//    return 0;
//    
//    //    NewGUIApp::get()->run(&stub);

    
    nv->attach_subview(nv1, DispPoint(10,10));
    nv->attach_subview(nv2, DispPoint(100,100));
    nv->move_subview(nv2, DispPoint(30,30));
    
    
    nv2->attach_subview(new NewGUIImageView(GUIImage("images/slider_bubble.bmp")),
                        DispPoint(120, 100));

    nv2->attach_subview(nv3, DispPoint(180, 150));
   
    nv->attach_subview(new NewGUIQuitButton, DispPoint(440,200));
    
    nv->attach_subview(new Anim, DispPoint(150,400));

    /*** CREATE SECOND_VIEW ***/
    
    GUIImage bg_full = GUIImage::create_blank(SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_FillRect(bg_full, 0, SDL_MapRGB(bg->format, 155, 155, 155));
    NewGUIView *scd_view = new NewGUIImageView(bg_full);
    

    scd_view->attach_subview(new ReturnButton(&window), DispPoint(300,300));
    
    scd_view->attach_subview(new NewGUITextBox(200, 300), DispPoint(20, 200));
    
    NewGUIValue_Horiz_Slider *val0 = new NewGUIValue_Horiz_Slider(100);
    val0->set_range(10, 40);
    scd_view->attach_subview(val0, DispPoint(20,30));
    scd_view->attach_subview(new NewGUIValue_Display(400, 30, val0), DispPoint(20,100));

    NewGUIValue_Box *tb = new NewGUIValue_Text_Box(400, 30);
    scd_view->attach_subview(tb, DispPoint(300,100));

    
    scd_view->attach_subview(NewGUI_create_button(bind(&NewGUIValue_Slider::set_new_value, val0,
                                                       bind(&NewGUIValue_Box::get_value, tb))), DispPoint(300,200));

    scd_view->attach_subview(new CatFace, DispPoint(400,400));

    
    /*** Link the views ***/

    class GoToViewButton : public NewGUITextButton {
    public:
        GoToViewButton(NewGUIWindow *window_, NewGUIView *view_, DispPoint point_ = DispPoint(0,0))
        : NewGUITextButton("Go To View"), window(window_), view(view_), point(point_) { }
    protected:
        virtual void operation() {
            window->attach_subview(view, point);
        }
    private:
        NewGUIWindow *window;
        NewGUIView *view;
        DispPoint point;
    };

    nv->attach_subview(new GoToViewButton(&window, scd_view), DispPoint(200,400));
    
    /*** Run nv ***/
    window.attach_subview(nv, DispPoint(10,10));

    NewGUIApp::get()->run(&window);
    
    
    nv->remove_subview(nv2);
    window.remove_subview(nv);

    delete nv2;
    delete nv;

    return 0;
}


int main (int argc, char **argv) {
    initGame();
    initSDL(SDL_INIT_EVERYTHING);
//  return user_main(argc, argv);
    
    return test(argc, argv);

}
