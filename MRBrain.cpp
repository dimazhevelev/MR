//
// Created by Dima Zhevelev on 15/01/2023.
//

#include "MRBrain.h"
#include <iostream>
#include <time.h>
#include <sstream>
#include <dlfcn.h>



void MRBrain::initialize(){
    std::cout << "initializing Brain" << std::endl;
    initLog("log.log");
    _mainThread = new MRTimedThread(1, [this] () { runNetFunctions(); });
    _noiseThread = new MRTimedThread(250, [this] () { makeNoise(); });
    _motorsThread = new MRTimedThread(2, [this] () { tickMotors(); });

}

py::list MRBrain::getPositions(){
    std::lock_guard<std::mutex> guard(_positions_lock);

    return py::cast(_positions);
}

py::list MRBrain::getMotorPositions(){
    std::lock_guard<std::mutex> guard(_motors_lock);

    return py::cast(_motor_positions);
}

void MRBrain::deinitialize(){
    _mainThread->stopThread();
    _noiseThread->stopThread();
    _motorsThread->stopThread();

    _mainThread->joinThread();
    _noiseThread->joinThread();
    _motorsThread->joinThread();


    // close the library
    std::cout << "Closing library...\n";
    for(auto handle : _dl_handles){
        dlclose(handle.second);
    }
    _log_stream.close();
}


void MRBrain::addMotor(float i_initial_position){
    _motor_positions.push_back(i_initial_position);
}

void MRBrain::initPositions(py::list i_initial_positions){
    auto positions = i_initial_positions.cast<std::vector<float>>();
    for(auto position : positions){
        _positions.push_back(position);
    }
}

void MRBrain::runNetFunctions(){
    std::stringstream ss;
    ss << "Positions : ";
    for(auto pos : _positions)
        ss << pos << ", ";
    _log_stream << ss.str() << std::endl;
    ss.str("");
    ss << "Motors : ";
    for(auto pos : _motor_positions)
        ss << pos << ", ";
    _log_stream << ss.str() << std::endl;
    for(auto net_function : _net_function_tags){
        _motors_lock.lock();
        _positions_lock.lock();
        auto new_positions = _net_functions[net_function](_positions, _motor_positions);
        ss.str("");
        for(auto pos : new_positions)
            ss << pos << ", ";
        _log_stream << "F " << net_function << " : " << ss.str() << std::endl;
        setMotors(new_positions);
        _positions_lock.unlock();
        _motors_lock.unlock();
    }
    _log_stream << std::endl;
}

int MRBrain::addNetFunction(std::string &i_functionName, std::string &i_functionTag, std::string &i_object_path) {
    auto new_handle_iter = _dl_handles.find(i_object_path);
    void *new_handle;
    if(new_handle_iter == _dl_handles.end()){
        new_handle = dlopen(i_object_path.c_str(), RTLD_LAZY);
        if (!new_handle) {
            std::cout << "Unexpected error, couldn't load plugin (" << i_object_path << ")";
            return -1;
        }
        _dl_handles.insert(std::make_pair(i_object_path, new_handle));
    }
    else
    {
        new_handle = _dl_handles.find(i_object_path)->second;
    }

    // reset errors
    dlerror();

    NetFunction_t function = (NetFunction_t) dlsym(new_handle, i_functionName.c_str());
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol " << i_functionName << " : " << dlsym_error <<
             '\n';
        dlclose(new_handle);
        return -1;
    }

    _net_functions.insert(std::make_pair(i_functionTag, function));
    _net_function_tags.push_back(i_functionTag);

    return 0;
}

void MRBrain::makeNoise(){
    std::lock_guard<std::mutex> guard(_positions_lock);

    for(int i = 0; i < _positions.size(); i++){
        float randomFactor = ((float)rand() - (RAND_MAX/2)) / RAND_MAX;
        _positions[i] = _positions[i] * (1 + 0.001 * randomFactor);
    }
}

void MRBrain::tickMotors(){
    std::lock_guard<std::mutex> guard(_motors_lock);
    for(int i = 0; i < _motor_positions.size(); i++){
        _motor_positions[i] = _motor_positions[i] + 1;
    }
}

void MRBrain::setMotors(const std::vector<float> i_motor_positions){
    for(int i = 0; i < _motor_positions.size(); i++){
        _motor_positions[i] = i_motor_positions[i];
    }
}

void MRBrain::initLog(const std::string &i_log){
    _log_stream.open(i_log);
}