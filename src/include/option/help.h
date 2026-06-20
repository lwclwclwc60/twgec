#ifndef HELP_H
#define HELP_H

#include "codegen/instruction/metadata.h"
#include "instruction.h"
#include <sstream>

namespace {

using namespace codegen::metadata;

const std::string introduction =
    "twgec (Twilight game.events compiler) is a transpiler that can generate "
    "`game.events` file from `*.twge` file.";

const std::string sectionOption = R"(
Usage: twgec <option>* file
Options:
    --help                                    Display this information.
    -o=<file>                                 Set the output file path.
    --run-only=<pass>(,<pass>)*               Run specific pass(es).
                                              Please refer to the pass in the next section.
    --print-ast-before                        Print the AST before each transformation.
    --print-ast-after                         Print the AST after each transformation.
    --print-token                             Print the Token of the language.
    --stack-trace                             Enable stack trace collection and printing.
    --profile-pass                            Print elapsed time of each transform pass.
    
Passes:
    symbolChecking
    argBinding
    blockInling
    blockLegalization
    functionInling
    constantFolding
    forLoopUnrolling
    ifStatementPropagation
    implicitListPromotion)";

const std::string sectionBuiltinStruct = R"(
Builtin Struct:
    Point
        x: int, y: int
    ActorMatch
        controller: string, id: string, matchKind: string, group: int
    Button
        id: string, text: string
    CustomWeapon
        reference: string, code: string, scaleOnGround: int, scaleOnIcon: int, weight: int, damage: int, swapTime: int, fireTime: int, fireType: int, pivotOnHandX: int, pivotOnHandXScale: int, pivotOnHandY: int, pivotOnHandYScale: int, pivotOnHandDegree: int, pivotOnIconX: int, pivotOnIconXScale: int, pivotOnIconY: int, pivotOnIconYScale: int, pivotOnIconDegree)";

std::string getGlobalMetadata() {
  std::string ret = "Supported Global Metadata:\n";
  const std::map<std::string, codegen::DefaultMapValue> defaultMap =
      ConfigSetup::defaultMap.defaultMap;
  for (auto metadata : defaultMap)
    ret += inden(4) + "__" + metadata.first +
           "__ : " + config::toString(metadata.second.astType) + "\n";
  return ret;
}

std::string getBlockMetadata() {
  std::string ret = "Supported Block Metadata:\n";
  const std::map<std::string, codegen::DefaultMapValue> defaultMap =
      BlockSetup::defaultMap.defaultMap;
  for (auto metadata : defaultMap)
    ret += inden(4) + "__" + metadata.first +
           "__ : " + config::toString(metadata.second.astType) + "\n";
  return ret;
}

std::string getInstructions() {
  std::string ret = "Supported Actions:\n";
  for (auto config : config::actionList)
    ret += config.toString() + "\n";
  ret += "\nSupported Checks:\n";
  for (auto config : config::checkList)
    ret += config.toString() + "\n";
  ret += "\nSupported Triggers:\n";
  for (auto config : config::triggerList)
    ret += config.toString() + "\n";
  return ret;
}
} // namespace

static const std::string helpMessage =
    introduction + "\n" + sectionOption + "\n" + sectionBuiltinStruct + "\n\n" +
    getGlobalMetadata() + "\n" + getBlockMetadata() + "\n" + getInstructions();

#endif