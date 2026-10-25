/**
* @file Trajectory.h
* @author Ashutosh Madhu
* @brief This file contains the writer to log trajectories
* @date 2026-10-25
*/
#pragma once

#include <fstream>
#include <string>

#include "State/State.h"

class TrajectoryWriter {

    std::ofstream mFile;

public:
    explicit TrajectoryWriter(const std::string& filepath);
    void write(double time, const StateVector& state, int phase);
    
};