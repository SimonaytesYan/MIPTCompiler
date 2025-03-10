#pragma once

#include "Grammar.hpp"

#include <fstream>

class GraphicDumpPass {
  public:
    GraphicDumpPass();
    void graphicDump(const GrammarUnit* node);

    void createPngFromDot();

  private:

    void dumpOperator();
    void printNodeInFormat(const char* color);
    void writeNodeAndEdge();

  private:
    const GrammarUnit* node_;
    std::ofstream out_;

    size_t dumpCounter_;
};
