/**
 * @file StatusCode.h
 * @author Ashutosh Madhu
 * @brief 
 * @version 0.1
 * @date 2026-11-07
 * 
 * @copyright Copyright (c) 2026
 * 
 */

 #pragma once 

 #include <cstdint>
 #include <ostream>

 enum class StatusCode : std::uint16_t {

   // General 0xxx
   
   INVALID_INPUT                 = 0001,
    
    // StateVector 1xxx

    // Dynamics 2xxx

    // Propagator 3xxx

    // Manoeuvre 4xxx

    // Lambert 5xxx
    A_MAX_LESS_THAN_MIN          = 5000,
    NO_CONVERGENCE_IN_BISECTION  = 5001,

 };

 inline std::ostream& operator<<(std::ostream& os, StatusCode status) {
    return os << static_cast<uint16_t>(status);
 };