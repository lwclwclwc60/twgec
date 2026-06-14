#include "codegen/codegen.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "frontend/preprocessor.h"
#include "option/option.h"
#include "transform/transform.h"
#include "utils/location.h"
#include <fstream>

int main(int argc, char *argv[]) {
  Option option = parseCommand(argc, argv);
  Location::setStackTraceEnabled(option.stackTrace);
  if (option.argFilePath == "")
    return 0;

  std::ifstream inputFile(option.argFilePath);
  if (!inputFile.is_open()) {
    std::cerr << "Error: Could not open file " << option.argFilePath << "\n";
    return 1;
  }

  std::string input((std::istreambuf_iterator<char>(inputFile)),
                    (std::istreambuf_iterator<char>()));
  inputFile.close();
  // Preprocessor
  Preprocessor preprocessor(option.argFilePath, input);
  std::vector<std::pair<std::string, std::string>> sourceFiles;
  if (!preprocessor.preprocess(sourceFiles))
    return 1;
  // Lexer
  Lexer lexer(sourceFiles);
  std::vector<Token> tokens = lexer.getTokens();
  if (Lexer::raiseLexicalError(tokens))
    return 1;
  if (option.printToken)
    Lexer::print(tokens);
  // Parser
  Parser parser(tokens);
  std::unique_ptr<ModuleNode> moduleNode = parser.parse();
  if (moduleNode == nullptr) {
    std::cerr << "SyntaxError: Cannot convert the token in a valid AST\n";
    return 1;
  }
  // Lowering
  transform::PassManager passManager(moduleNode, option);
  if (!passManager.execute())
    return 1;
  if (!option.runOnly.empty())
    return 0;
  // Codegen
  codegen::CodeGenerator generator(moduleNode);
  generator.codegen(option.outputFile);

  return 0;
}