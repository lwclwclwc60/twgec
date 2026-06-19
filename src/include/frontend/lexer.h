#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <cctype>
#include <string>
#include <vector>

class Lexer {
public:
  Lexer(std::vector<std::pair<std::string, std::string>> &inputFiles)
      : inputFiles(inputFiles) {}
  std::vector<Token> getTokens();
  static bool raiseLexicalError(std::vector<Token> tokens);
  static void print(std::vector<Token> tokens);

private:
  std::vector<std::pair<std::string, std::string>> inputFiles;

  std::string filename;
  std::string input;
  static int line;
  static int column;
  size_t pos;

  std::vector<Token> getSingleFileTokens();
  Token nextToken();
  Token metadataToken();
  Token commentToken();
  Token blockCommentToken();
  Token integerToken();
  Token stringToken();
  Token identifierToken();
  Token keywordToken(std::string keyword);

  inline bool matchKeyword(std::string keyword);
  inline bool next(char character, unsigned offset = 1);
};

#endif