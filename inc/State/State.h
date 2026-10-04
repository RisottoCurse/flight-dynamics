#pragma once 

#include <Eigen/Dense>


class StateVector {

    Eigen::Vector3d mPosition;
    Eigen::Vector3d mVelocity;

public: 
    StateVector(const Eigen::Vector3d& position, const Eigen::Vector3d& velocity) : 
    mPosition(position), mVelocity(velocity) 
    {};

    // getters
    [[nodiscard]] auto getPosition() const -> const Eigen::Vector3d& { return mPosition; }
    [[nodiscard]] auto getVelocity() const -> const Eigen::Vector3d& { return mVelocity; }

};