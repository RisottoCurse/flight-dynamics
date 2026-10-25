#include "IO/TrajectoryWriter.h"

#include <iomanip>
#include <stdexcept>

TrajectoryWriter::TrajectoryWriter(const std::string& filepath):
    mFile(filepath) {
        if (!mFile.is_open()) {
            throw std::runtime_error("Unable to open trajectory file: " + filepath);
        }

        mFile << "time,x,y,z,u,v,w,phase\n";
        mFile << std::setprecision(16);
    }

void TrajectoryWriter::write(double time, const StateVector& state, int phase) {

    const auto& r = state.getPosition();
    const auto& v = state.getVelocity();

    mFile
        << time << ","
        << r.x() << ","
        << r.y() << ","
        << r.z() << ","
        << v.x() << ","
        << v.y() << ","
        << v.z() << ","
        << phase << "\n";

}
