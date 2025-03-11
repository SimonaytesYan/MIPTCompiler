#pragma once

#include "Grammar.hpp"

#include <fstream>

class GraphicDumpPass {
  public:
    GraphicDumpPass() = default;
    GraphicDumpPass(const std::string file_name_prefix);

    void graphicDump(const GrammarUnit* node);

    void createPngFromDot();

  private:

    void dumpOperator();
    void printNodeInFormat(const char* color);
    void writeNodeAndEdge();
    std::string fullDumpFileName();

  private:
    const GrammarUnit* node_{nullptr};
    std::ofstream out_;

    std::string file_name_prefix_{"dump"};
    size_t dump_counter_{0};
};
