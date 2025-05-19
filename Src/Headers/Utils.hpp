#pragma once

#define COMPILER_ASSERT(statement, messege) COMPILER_ASSERT_(statement, messege, __func__)

#define COMPILER_ASSERT_(statement, messege, func)      \
    if (!(statement)) {                                 \
        std::cerr << func << ": " << messege << "\n";   \
        return nullptr;                                 \
    }

#define TYPE_INF_ASSERT(statement, messege) TYPE_INFERENCE_ASSERT_(statement, messege, __func__)

#define TYPE_INFERENCE_ASSERT_(statement, messege, func)      \
    if (!(statement)) {                                 \
        std::cerr << func << ": " << messege << "\n";   \
        return;                                 \
    }