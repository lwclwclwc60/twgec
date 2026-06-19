#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <string>

namespace codegen {

struct UserDefinedMetadata {
  // It will be handled in the `preCodegen`
  std::map<std::string, std::string> customWeaponsKeywordEnum;
};

class CodeGenerator {
private:
  // Variable
  const std::unique_ptr<ModuleNode> &moduleNode;
  UserDefinedMetadata userDefinedMeta;
  // Functions
  void preCodegen();
  void codegenModuleNode(std::ofstream &of);
  void codegenBlocks(std::ofstream &of);
  void codegenAction(std::ofstream &of, std::unique_ptr<InstructionNode> &);
  void codegenTypedInstrSet(std::ofstream &of,
                            std::unique_ptr<TypedInstrSetNode> &);
  void codegenCheck(std::ofstream &of, std::unique_ptr<InstructionNode> &);
  void codegenTrigger(std::ofstream &of, std::unique_ptr<InstructionNode> &);

public:
  // Constructor
  CodeGenerator(const std::unique_ptr<ModuleNode> &moduleNode)
      : moduleNode(moduleNode) {};
  // Functions
  void codegen(std::string filepath);
};
} // namespace codegen
#endif