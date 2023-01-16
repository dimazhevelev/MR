//
// Created by Dima Zhevelev on 15/01/2023.
//

#ifndef MRSIM__MRBRAIN_H_
#define MRSIM__MRBRAIN_H_

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <thread>
#include <vector>
#include <unordered_map>
#include <map>
#include "MRTimedThread.h"
#include <mutex>
#include <fstream>

typedef std::vector<float> (*NetFunction_t)(const std::vector<float>, const std::vector<float>);
namespace py = pybind11;
class MRBrain
{
 public:
    MRBrain(){}
    ~MRBrain() {}

    void initialize();
    py::list getPositions();
    py::list getMotorPositions();
    void deinitialize();

    void printStatus();
    void addMotor(float i_initial_motor_position);
    void initPositions(py::list i_initial_positions);
    int addNetFunction(std::string &i_functionName, std::string &i_functionTag, std::string &i_object_path);
    void initLog(const std::string &i_log);

 private:
    std::map<std::string, void*> _dl_handles;
    std::unordered_map<std::string, NetFunction_t> _net_functions;

    std::string _log_file;
    std::ofstream _log_stream;

    std::vector<float> _motor_positions;
    std::vector<float> _positions;
    std::vector<std::string> _net_function_tags;
    MRTimedThread *_mainThread;
    MRTimedThread *_noiseThread;
    MRTimedThread *_motorsThread;
    std::mutex _motors_lock;
    std::mutex _positions_lock;



    void runNetFunctions();
    void makeNoise();
    void tickMotors();
    void setMotors(const std::vector<float> i_motor_positions);

};




PYBIND11_MODULE(MRSim, m) {
// optional module docstring
m.doc() = "pybind11  plugin";

// bindings to Pet class
py::class_<MRBrain>(m, "MRBrain")
.def(py::init<>())
.def("deinitialize", &MRBrain::deinitialize)
.def("addMotor", &MRBrain::addMotor)
.def("addNetFunction", &MRBrain::addNetFunction)
.def("initPositions", &MRBrain::initPositions)
.def("getPositions", &MRBrain::getPositions)
.def("getMotorPositions", &MRBrain::getMotorPositions)
.def("initialize", &MRBrain::initialize);
}


#endif //MRSIM__MRBRAIN_H_
