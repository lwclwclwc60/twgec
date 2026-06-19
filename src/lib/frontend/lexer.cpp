#include "frontend/lexer.h"
#include <algorithm>
#include <map>

#define LEXER_MATCH_KEYWORD_AND_RETURN(keyword)                                \
  else if (matchKeyword(keyword)) {                                            \
    return keywordToken(keyword);                                              \
  }

namespace keyword {
std::string _true = "true";
std::string _false = "false";
std::string block = "block";
std::string actions = "actions";
std::string triggers = "triggers";
std::string checks = "checks";
std::string def = "def";
std::string _const = "const";
std::string _if = "if";
std::string _else = "else";
std::string _for = "for";
std::string in = "in";
std::string Point = "Point";
std::string ActorMatch = "ActorMatch";
std::string Button = "Button";
std::string CustomWeapon = "CustomWeapon";
std::string IntrinsicToString = "twge::to_string";
std::string IntrinsicToInt = "twge::to_int";
std::string IntrinsicToBool = "twge::to_bool";
std::string IntrinsicGetIndex = "twge::get_index";
std::string IntrinsicGetLength = "twge::get_length";
std::string IntrinsicGetSlice = "twge::get_slice";
std::string IntrinsicAssert = "twge::assert";

const std::map<std::string, TokenType> initMap = {
    {_true, TokenType::TRUE},
    {_false, TokenType::FALSE},
    {block, TokenType::BLOCK},
    {actions, TokenType::ACTIONS},
    {triggers, TokenType::TRIGGERS},
    {checks, TokenType::CHECKS},
    {def, TokenType::DEF},
    {_const, TokenType::CONST},
    {_if, TokenType::IF},
    {_else, TokenType::ELSE},
    {_for, TokenType::FOR},
    {in, TokenType::IN},
    {Point, TokenType::POINT},
    {ActorMatch, TokenType::ACTOR_MATCH},
    {Button, TokenType::BUTTON},
    {CustomWeapon, TokenType::CUSTOM_WEAPON},
    {IntrinsicToString, TokenType::INTRINSIC_TO_STRING},
    {IntrinsicToInt, TokenType::INTRINSIC_TO_INT},
    {IntrinsicToBool, TokenType::INTRINSIC_TO_BOOL},
    {IntrinsicGetIndex, TokenType::INTRINSIC_GET_INDEX},
    {IntrinsicGetLength, TokenType::INTRINSIC_GET_LENGTH},
    {IntrinsicGetSlice, TokenType::INTRINSIC_GET_SLICE},
    {IntrinsicAssert, TokenType::INTRINSIC_ASSERT},
};
} // namespace keyword

// Helper Function
inline bool isValidIdentifierLeadingCharacter(char c) {
  return isalpha(c) || c == '_' || (c & 0x80);
}

inline bool isValidIdentifierCharacter(char c) {
  return isalnum(c) || c == '_' || (c & 0x80);
}

int Lexer::line = 1;
int Lexer::column = 1;

std::vector<Token> Lexer::getTokens() {
  std::vector<Token> output;
  for (const auto &file : inputFiles) {
    filename = file.first;
    input = file.second;
    pos = 0;
    line = 1;
    column = 1;
    std::vector<Token> tokens = getSingleFileTokens();
    output.insert(output.end(), tokens.begin(), tokens.end());
  }
  return output;
}

std::vector<Token> Lexer::getSingleFileTokens() {
  std::vector<Token> tokens;
  Token token = nextToken();

  while (token.type != TokenType::END) {
    tokens.push_back(token);
    token = nextToken();
  }

  tokens.erase(std::remove_if(tokens.begin(), tokens.end(),
                              [](const Token &token) {
                                return token.type == TokenType::COMMENT;
                              }),
               tokens.end());

  return tokens;
}

bool Lexer::raiseLexicalError(std::vector<Token> tokens) {
  bool hasLexicalError = false;
  for (const auto &t : tokens)
    if (t.type == TokenType::UNKNOWN) {
      hasLexicalError = true;
      std::vector<int> intArray;
      for (unsigned char c : t.value)
        intArray.push_back(static_cast<int>(c));
      std::cerr << "InvalidCharacterError: Unexpected character \'" << t.value
                << " (";
      for (int v : intArray)
        std::cerr << v << ", ";
      std::cerr << ")"
                << "\' at " << t.location << "\n";
    }
  return hasLexicalError;
}

void Lexer::print(std::vector<Token> tokens) {
  std::cout << "===== Print Tokens =====\n";
  for (const auto &t : tokens)
    std::cout << t << "\n";
}

Token Lexer::nextToken() {
  while (pos < input.length()) {
    char current = input[pos];
    if (current == ' ') {
      Lexer::column++;
      pos++;
    } else if (current == '\t') {
      Lexer::column += 4;
      pos++;
    } else if (current == '\n') {
      Lexer::column = 1;
      Lexer::line++;
      pos++;
    } else if (current == '"') {
      return stringToken();
    } else if (current == '=' && next('=')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::EQUAL, filename, Lexer::line, Lexer::column - 2,
                   "==");
    } else if (current == '=') {
      Lexer::column++;
      pos++;
      return Token(TokenType::ASSIGN, filename, Lexer::line, Lexer::column - 1,
                   "=");
    } else if (current == '>' && next('=')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::GREATER_THAN_EQUAL, filename, Lexer::line,
                   Lexer::column - 2, ">=");
    } else if (current == '>') {
      Lexer::column++;
      pos++;
      return Token(TokenType::GREATER_THAN, filename, Lexer::line,
                   Lexer::column - 1, ">");
    } else if (current == '<' && next('=')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::LESS_THAN_EQUAL, filename, Lexer::line,
                   Lexer::column - 2, "<=");
    } else if (current == '<') {
      Lexer::column++;
      pos++;
      return Token(TokenType::LESS_THAN, filename, Lexer::line,
                   Lexer::column - 1, "<");
    } else if (current == '!' && next('=')) {
      // TODO: Support not operation
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::NOT_EQUAL, filename, Lexer::line,
                   Lexer::column - 2, "!=");
    } else if (current == '&' && next('&')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::AND, filename, Lexer::line, Lexer::column - 2,
                   "&&");
    } else if (current == '|' && next('|')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::OR, filename, Lexer::line, Lexer::column - 2,
                   "||");
    } else if (current == '{') {
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENCUR, filename, Lexer::line, Lexer::column - 1,
                   "{");
    } else if (current == '}') {
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSECUR, filename, Lexer::line,
                   Lexer::column - 1, "}");
    } else if (current == '(') {
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENPAR, filename, Lexer::line, Lexer::column - 1,
                   "(");
    } else if (current == ')') {
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSEPAR, filename, Lexer::line,
                   Lexer::column - 1, ")");
    } else if (current == '[') {
      Lexer::column++;
      pos++;
      return Token(TokenType::OPENSQR, filename, Lexer::line, Lexer::column - 1,
                   "[");
    } else if (current == ']') {
      Lexer::column++;
      pos++;
      return Token(TokenType::CLOSESQR, filename, Lexer::line,
                   Lexer::column - 1, "]");
    } else if (current == '.' && next('.') && next('.', 2)) {
      Lexer::column += 3;
      pos += 3;
      return Token(TokenType::ELLIPSIS, filename, Lexer::line,
                   Lexer::column - 3, "...");
    } else if (current == '.') {
      Lexer::column++;
      pos++;
      return Token(TokenType::DOT, filename, Lexer::line, Lexer::column - 1,
                   ".");
    } else if (current == ',') {
      Lexer::column++;
      pos++;
      return Token(TokenType::COMMA, filename, Lexer::line, Lexer::column - 1,
                   ",");
    } else if (current == ';') {
      Lexer::column++;
      pos++;
      return Token(TokenType::SEMICOLON, filename, Lexer::line,
                   Lexer::column - 1, ";");
    } else if (current == ':' && next(':')) {
      Lexer::column += 2;
      pos += 2;
      return Token(TokenType::SCOPE, filename, Lexer::line, Lexer::column - 2,
                   "::");
    } else if (current == ':') {
      Lexer::column++;
      pos++;
      return Token(TokenType::COLON, filename, Lexer::line, Lexer::column - 1,
                   ":");
    } else if (current == '+') {
      Lexer::column++;
      pos++;
      return Token(TokenType::ADD, filename, Lexer::line, Lexer::column - 1,
                   "+");
    } else if (current == '-') {
      Lexer::column++;
      pos++;
      return Token(TokenType::SUB, filename, Lexer::line, Lexer::column - 1,
                   "-");
    } else if (current == '*') {
      Lexer::column++;
      pos++;
      return Token(TokenType::MUL, filename, Lexer::line, Lexer::column - 1,
                   "*");
    } else if (current == '/' && next('/')) {
      return commentToken();
    } else if (current == '/' && next('*')) {
      return blockCommentToken();
    } else if (current == '/') {
      Lexer::column++;
      pos++;
      return Token(TokenType::DIV, filename, Lexer::line, Lexer::column - 1,
                   "/");
    } else if (current == '%') {
      Lexer::column++;
      pos++;
      return Token(TokenType::MOD, filename, Lexer::line, Lexer::column - 1,
                   "%");
    } else if (current == '_' && next('_')) {
      return metadataToken();
    } else if (std::isdigit(current)) {
      // TODO: Support Float
      return integerToken();
    }
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::_true)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::_false)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::actions)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::block)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::checks)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::triggers)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::def)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::_const)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::_if)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::_else)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::_for)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::in)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::Point)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::ActorMatch)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::Button)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::CustomWeapon)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::IntrinsicToString)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::IntrinsicToInt)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::IntrinsicToBool)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::IntrinsicGetIndex)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::IntrinsicGetLength)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::IntrinsicGetSlice)
    LEXER_MATCH_KEYWORD_AND_RETURN(keyword::IntrinsicAssert)
    else if (isValidIdentifierLeadingCharacter(current)) {
      return identifierToken();
    }
    else {
      return Token(TokenType::UNKNOWN, filename, Lexer::line, Lexer::column++,
                   std::string(1, input[pos++]));
    }
  }
  return Token(TokenType::END, filename, Lexer::line, Lexer::column - 1);
}

Token Lexer::metadataToken() {
  int startColumn = column;
  size_t start = pos;
  if (pos + 1 >= input.length() || input[pos++] != '_' || input[pos++] != '_') {
    column += 3;
    return Token(TokenType::UNKNOWN, filename, Lexer::line, startColumn,
                 input.substr(start, (pos++) - start));
  }
  column += 2;
  while (pos < input.length() && (isalnum(input[pos]) && input[pos] != '_')) {
    column++;
    pos++;
  }
  if (pos + 1 >= input.length() || input[pos++] != '_' || input[pos++] != '_') {
    column += 3;
    return Token(TokenType::UNKNOWN, filename, Lexer::line, startColumn,
                 input.substr(start, (pos++) - start));
  }
  column += 2;
  return Token(TokenType::METADATA, filename, Lexer::line, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::stringToken() {
  int startColumn = column;
  size_t start = pos++;
  column++;
  while (pos < input.length() &&
         (input[pos] != '"' || input[pos - 1] == '\\')) {
    column++;
    pos++;
  }
  if (pos < input.length()) {
    column++;
    pos++; // Skip closing quote
  }
  return Token(TokenType::STRING, filename, Lexer::line, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::identifierToken() {
  size_t start = pos;
  int startColumn = column;
  while (pos < input.length() && isValidIdentifierCharacter(input[pos])) {
    column++;
    bool isValid = true;
    if ((isalnum(input[pos]) || input[pos] == '_')) {
      pos++;
    } else if (input[pos] & 0x80) {
      if ((input[pos] & 0xE0) == 0xC0)
        pos += 2; // leading bits are 110
      else if ((input[pos] & 0xF0) == 0xE0)
        pos += 3; // leading bits are 1110
      else if ((input[pos] & 0xF8) == 0xF0)
        pos += 4; // leading bits are 11110
      else
        isValid = false;
    } else {
      isValid = false;
    }
    if (!isValid)
      return Token(TokenType::UNKNOWN, filename, Lexer::line, Lexer::column++,
                   input.substr(start, (pos++) - start));
  }
  return Token(TokenType::IDENTIFIER, filename, Lexer::line, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::integerToken() {
  size_t start = pos;
  int startColumn = column;
  while (pos < input.length() && isdigit(input[pos])) {
    column++;
    pos++;
  }
  return Token(TokenType::INT, filename, Lexer::line, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::commentToken() {
  size_t start = pos;
  int startColumn = column;
  column += 2;
  while (pos < input.length() && input[pos++] != '\n')
    column++;
  column = 1;
  return Token(TokenType::COMMENT, filename, Lexer::line++, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::blockCommentToken() {
  size_t start = pos;
  int startLine = line;
  int startColumn = column;

  pos += 2;
  column += 2;

  while (pos < input.length()) {
    if (input[pos] == '*' && next('/')) {
      pos += 2;
      column += 2;
      return Token(TokenType::COMMENT, filename, startLine, startColumn,
                   input.substr(start, pos - start));
    }
    if (input[pos] == '\n') {
      pos++;
      line++;
      column = 1;
      continue;
    }
    pos++;
    column++;
  }

  return Token(TokenType::UNKNOWN, filename, startLine, startColumn,
               input.substr(start, pos - start));
}

Token Lexer::keywordToken(std::string keyword) {
  int startColumn = Lexer::column;
  Lexer::column += (keyword).length();
  (pos) += (keyword).length();
  return Token(keyword::initMap.at(keyword), filename, Lexer::line, startColumn,
               keyword);
}

inline bool Lexer::matchKeyword(std::string keyword) {
  unsigned len = keyword.length();
  return pos + len < input.length() && input.substr(pos, len) == keyword &&
         !isValidIdentifierCharacter(input[pos + len]);
}

inline bool Lexer::next(char character, unsigned offset) {
  return pos + offset < input.length() && input[pos + offset] == character;
}