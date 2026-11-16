/**
 * @file global.h
 * @author Ashutosh Madhu
 * @brief Helpers for propagating std::expected errors.
 * @version 0.1
 * @date 2026-11-16
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once



// Requires GCC/Clang statement expressions and a non-void expected value.
// Evaluate expr once, returning its error or moving out its value.
#define TRY(expr)                                                        \
    __extension__({                                                       \
        auto _temp = (expr);                         \
        if (!_temp) {                                \
            return std::unexpected(                                      \
                std::move(_temp).error());           \
        }                                                                \
        std::move(*_temp);                           \
    })
