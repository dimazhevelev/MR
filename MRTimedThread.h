//
// Created by Dima Zhevelev on 15/01/2023.
//

#ifndef EXAMPLE__MRTIMEDTHREAD_H_
#define EXAMPLE__MRTIMEDTHREAD_H_

#include <thread>
#include <functional>

class MRTimedThread {
 public:
    MRTimedThread(int i_frequency, std::function<void(void)> i_handler){
        _duty_cycle_ms = 1000 / i_frequency;
        _handler = i_handler;
        _main_thread = new std::thread(&MRTimedThread::mainThread, this);
    }
    ~MRTimedThread();

    void mainThread();
    void stopThread() {_stop_thread = true;};
    void joinThread() { _main_thread->join();};
 private:
    std::atomic<bool> _stop_thread;
    std::thread *_main_thread;
    long _duty_cycle_ms;
    std::function<void(void)> _handler;
};

#endif //EXAMPLE__MRTIMEDTHREAD_H_
