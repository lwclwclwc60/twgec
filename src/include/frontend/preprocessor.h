#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include <vector>

class Preprocessor {
public:
  Preprocessor(const std::string &filename, const std::string &content)
      : filename(filename), content(content) {}
  bool
  preprocess(std::vector<std::pair<std::string, std::string>> &sourceFiles);

private:
  std::string filename;
  std::string content;
};

#endif