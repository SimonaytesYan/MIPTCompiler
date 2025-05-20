#pragma once

// #define DUMP

#ifdef DUMP
    #define LOG std::cerr
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

    static std::ostream LOG(&buffer);
#endif  // DEBUG
