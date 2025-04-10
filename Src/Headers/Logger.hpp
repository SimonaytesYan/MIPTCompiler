#pragma once

// #define DUMP

#ifdef DUMP
    #define log std::cout
#else
    #include <ostream>
    #include <sstream>

    // Creating empty buffer
    class MyBuf : public std::stringbuf
    {
        public:
            virtual int sync() { return 0; }
    };
    static MyBuf buffer;

    static std::ostream log(&buffer);
#endif  // DEBUG
