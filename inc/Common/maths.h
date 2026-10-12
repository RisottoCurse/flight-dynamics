/**
* @file maths.h
* @author Ashutosh Madhu
* @brief This file contains the definition of the Maths namespace and its associated types.
* @date 2026-10-12
*/
#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

#define RAD2DEG 180.0 / M_PI
#define DEG2RAD M_PI / 180.0

using Vector2 = Eigen::Vector2d;
using Vector3 = Eigen::Vector3d;
using Velocity = Eigen::Vector3d;
using Vector6 = Eigen::Matrix<double, 6, 1>;
using VectorX = Eigen::VectorXd;


