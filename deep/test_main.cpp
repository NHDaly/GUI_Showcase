//
//  test_main.cpp
//  Deep
//
//  Created by Nathan Daly on 12/29/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "NathanSocket.h"

#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>

#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

#define ERR(x) { int res = x; assert(!res); }

#define kOctavePlotSocket 12123

void listen();
void start(Socket_Server *s);


octave_value_list call_oct_fun(string func, int num_args, char**args);

void listen() {
    
    Socket_Server listener(kOctavePlotSocket);
    listener.listen();
	while (true) {
		Socket_Server *s = listener.accept(); // s -- socket for requst thread
        
		// initializing a new thread
        pthread_attr_t attr;   // use &attr as a parameter to pthread_create
        ERR(pthread_attr_init(&attr));       // ERR macro asserts on failure
        ERR(pthread_attr_setstacksize(&attr, 1024*1024));
        ERR(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
        
        // creating a new thread
        pthread_t ID;
        ERR(pthread_create(&ID, &attr, (void*(*)(void *))&start, (void*)s));
    }
}


int get_int(Socket_Server *s) {
    
    string size;
    do {
        size.append(s->recv(1));
    }
    while (*size.rbegin() != '\0');
    
    return atoi(size.c_str());
 }

void start(Socket_Server *s) {
    
    cout << "Started!" << endl;
    
    int size_int = get_int(s);
    
    cout << "about to receive size: " << size_int << endl;

    string msg = s->recv_all(size_int);
    
    cout << "received msg: " << msg << endl;
    
    int num_args = get_int(s);

    string_vector args(num_args);
    
    for (int i = 0; i < num_args; i++) {
        int size_int = get_int(s);
        
        cout << "about to receive size: " << size_int << endl;
        
        args[i] = s->recv_all(size_int);        
    }

    octave_value_list in = octave_value(args);
    octave_value_list out = feval (msg.c_str(), in, num_args);
    
    cout << "out: " << out.length() << endl;
    
    if (!error_state && out.length () > 0)
    {
        double res = out(0).double_value();
        cout << "res: " << res << endl;
        
    }

}


int main(int, char**) {

    char *argv[2] = {(char*)"embedded", (char*)"-q"};
    octave_main(1, argv, true);  

    char *args[2] = {(char*)"GNUTERM", (char*)"x11"};
    call_oct_fun("setenv", 2, args);
    
    Array<double> args2(3);
    args2(0) = -10;
    args2(1) = 0.1;
    args2(2) = 10;
    
    octave_value_list in = octave_value(args2);
    octave_value_list out = feval ("plot", in, 1);

    
    if (out.length () > 0)
    {
        double res = out(0).double_value();
        cout << "res: " << res << endl;
        
    }
    
    
    
    listen();
    
    return 0;
}


octave_value_list call_oct_fun(string func, int num_args, char**vargs) {
    
    
    string_vector args(num_args);
    
    for (int i = 0; i < num_args; i++) {
        args[i] = vargs[i];        
    }
    
    octave_value_list in = octave_value(args);
    octave_value_list out = feval (func.c_str(), in, num_args);
    
    cout << "out: " << out.length() << endl;
    
    assert(!error_state);
    
    return out;
}

