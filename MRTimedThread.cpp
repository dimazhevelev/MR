//
// Created by Dima Zhevelev on 15/01/2023.
//

#include "MRTimedThread.h"
#include <chrono>
#include <iostream>

using namespace std::chrono;


MRTimedThread::~MRTimedThread(){
    _stop_thread = true;
    _main_thread->join();
    delete(_main_thread);
}

void MRTimedThread::mainThread(){
    while(!_stop_thread){
        milliseconds entry_time_ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch());
        _handler();
        milliseconds time_now = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch());
        milliseconds wake_up_in_ms(entry_time_ms.count() + _duty_cycle_ms - time_now.count());
        std::this_thread::sleep_for(std::chrono::milliseconds(wake_up_in_ms));
    }
}