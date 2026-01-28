#pragma once

#define COMPILER_ASSERT(statement, message) COMPILER_ASSERT_(statement, message, __func__)

#define COMPILER_ASSERT_(statement, message, func)      \
    if (!(statement)) {                                 \
        std::cerr << func << ": " << message << "\n";   \
        return nullptr;                                 \
    }

#define TYPE_INF_ASSERT(statement, message) TYPE_INFERENCE_ASSERT_(statement, message, __func__)

#define TYPE_INFERENCE_ASSERT_(statement, message, func)    \
    if (!(statement)) {                                     \
        std::cerr << func << ": " << message << "\n";       \
        finished_successfully_ = false;                     \
        return;                                             \
    }
