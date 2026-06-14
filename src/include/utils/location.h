#ifndef LOCATION_H
#define LOCATION_H

#include <iostream>
#include <string>
#include <vector>

struct CallFrame {
  std::string symbol;
  std::string filename;
  int line;
  int column;

  CallFrame(std::string symbol, std::string filename, int line, int column)
      : symbol(symbol), filename(filename), line(line), column(column) {}
};

struct Location {
  std::string filename;
  int line;
  int column;
  std::string ownerSymbol;
  std::vector<CallFrame> callFrames;

  Location(std::string filename, int line, int column)
      : filename(filename), line(line), column(column) {}

  static bool &stackTraceEnabledFlag() {
    static bool enabled = false;
    return enabled;
  }

  static void setStackTraceEnabled(bool enabled) {
    stackTraceEnabledFlag() = enabled;
  }

  static bool isStackTraceEnabled() { return stackTraceEnabledFlag(); }

  void pushCallFrame(const std::string &symbol, const Location &callSite) {
    if (!isStackTraceEnabled())
      return;
    callFrames.emplace_back(symbol, callSite.filename, callSite.line,
                            callSite.column);
  }

  void printCallTrace(std::ostream &os) const {
    if (!isStackTraceEnabled())
      return;
    os << "# frame 0: " << ownerSymbol << " at " << filename << ":" << line
       << ":" << column << "\n";
    int frameNo = 1;
    for (int idx = static_cast<int>(callFrames.size()) - 1; idx >= 0; --idx) {
      const auto &frame = callFrames[idx];
      os << "# frame " << frameNo << ": "
         << frame.symbol << " at " << frame.filename << ":" << frame.line
         << ":" << frame.column << "\n";
      ++frameNo;
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const Location &location) {
    os << location.filename << ":" << location.line << ":" << location.column;
    return os;
  }
};

#endif