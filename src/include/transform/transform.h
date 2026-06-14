#include "ast.h"
#include "option/option.h"
#include <chrono>
#include <functional>

namespace {
  using namespace std::chrono;
}

namespace transform {

struct PassConfig {
  // Reduce the number of pass printed
  bool hidden;
  // Avoid error throwing before the last CSE
  bool allowUnresolvedExpression;
};

// Symbol redefinition or use before define will be checked
bool symbolChecking(const std::unique_ptr<ModuleNode> &moduleNode,
                    PassConfig config);
// Positional Arg will be converted into Named Arg
bool argBinding(const std::unique_ptr<ModuleNode> &moduleNode,
                PassConfig config);
// The block function will be inlined
bool blockInling(const std::unique_ptr<ModuleNode> &moduleNode,
                 PassConfig config);
// Number of actions, checks, and triggers in a block will be checked
bool blockLegalization(const std::unique_ptr<ModuleNode> &moduleNode,
                       PassConfig config);
// The actions, checks, and triggers function will be inlined
bool functionInling(const std::unique_ptr<ModuleNode> &moduleNode,
                    PassConfig config);
// Propagate the constant and fold the constant
bool constantFolding(const std::unique_ptr<ModuleNode> &moduleNode,
                     PassConfig config);
// Unroll for loop
bool forLoopUnrolling(const std::unique_ptr<ModuleNode> &moduleNode,
                      PassConfig config);
// Flatten the if-statement and remove the dead condition branch
bool ifStatementPropagation(const std::unique_ptr<ModuleNode> &moduleNode,
                            PassConfig config);
// Promote type `A` to `list[A]` implicitly
bool implicitListPromotion(const std::unique_ptr<ModuleNode> &moduleNode,
                           PassConfig config);

namespace pass {
const std::string symbolChecking = "symbolChecking";
const std::string argBinding = "argBinding";
const std::string blockInling = "blockInling";
const std::string blockLegalization = "blockLegalization";
const std::string functionInling = "functionInling";
const std::string forLoopUnrolling = "forLoopUnrolling";
const std::string constantFolding = "constantFolding";
const std::string ifStatementPropagation = "ifStatementPropagation";
const std::string implicitListPromotion = "implicitListPromotion";
} // namespace pass

class PassManager {
private:
  const std::unique_ptr<ModuleNode> &moduleNode;
  Option opt;

  map<std::string,
      std::function<bool(const std::unique_ptr<ModuleNode> &, PassConfig)>>
      passMap = {
          {pass::symbolChecking, symbolChecking},
          {pass::argBinding, argBinding},
          {pass::blockInling, blockInling},
          {pass::blockLegalization, blockLegalization},
          {pass::functionInling, functionInling},
          {pass::forLoopUnrolling, forLoopUnrolling},
          {pass::constantFolding, constantFolding},
          {pass::ifStatementPropagation, ifStatementPropagation},
          {pass::implicitListPromotion, implicitListPromotion},
  };

  bool singlePass(std::string passName, PassConfig config) {
    const auto start = steady_clock::now();
    std::function<bool(const std::unique_ptr<ModuleNode> &, PassConfig)>
        passFunc = passMap[passName];
    if (opt.printASTBefore && !config.hidden)
      moduleNode->print(std::string("AST Before ") + passName);
    if (!passFunc(std::move(moduleNode), config)) {
      if (opt.profilePass) {
        const auto end = steady_clock::now();
        const auto ms = duration_cast<milliseconds>(end - start)
                .count();
        std::cerr << "[pass-profile] " << passName << " failed in " << ms
                  << " ms\n";
      }
      return false;
    }
    if (opt.printASTAfter && !config.hidden)
      moduleNode->print(std::string("AST After ") + passName);
    if (opt.profilePass) {
      const auto end = steady_clock::now();
      const auto ms = duration_cast<milliseconds>(end - start).count();
      std::cerr << "[pass-profile] " << passName << ": " << ms << " ms\n";
    }
    return true;
  }

  bool pipeline() {
    // Before expression propagation
    PassConfig config = {.hidden = false, .allowUnresolvedExpression = true};
    if (!(singlePass(pass::symbolChecking, config) &&
          singlePass(pass::argBinding, config) &&
          singlePass(pass::blockInling, config) &&
          singlePass(pass::blockLegalization, config) &&
          singlePass(pass::functionInling, config)))
      return false;
    // Expression propagation
    PassConfig CSEConfig = {.hidden = true, .allowUnresolvedExpression = true};
    PassConfig lastCSEConfig = {.hidden = true,
                                .allowUnresolvedExpression = false};
    if (!(singlePass(pass::constantFolding, CSEConfig) &&
          singlePass(pass::forLoopUnrolling, config) &&
          singlePass(pass::constantFolding, CSEConfig) &&
          singlePass(pass::ifStatementPropagation, config) &&
          singlePass(pass::constantFolding, lastCSEConfig)))
      return false;
    // After expression propagation
    config = {.hidden = false, .allowUnresolvedExpression = false};
    if (!singlePass(pass::implicitListPromotion, config))
      return false;
    if (opt.printASTBefore)
      moduleNode->print("AST Before Code Generation");
    return true;
  }

public:
  bool execute() {
    const auto start = steady_clock::now();
    bool ret = true;
    if (opt.runOnly.empty()) {
      ret = pipeline();
    } else {
      // TODO: Allow loopUnrolling with constandFolding OTF
      PassConfig config = {.hidden = false, .allowUnresolvedExpression = false};
      for (std::string pass : opt.runOnly)
        if (!singlePass(pass, config)) {
          ret = false;
          break;
        }
    }
    if (opt.profilePass) {
      const auto end = steady_clock::now();
      const auto ms = duration_cast<milliseconds>(end - start).count();
      std::cerr << "[pass-profile] total: " << ms << " ms\n";
    }
    return ret;
  }

  PassManager(const std::unique_ptr<ModuleNode> &moduleNode, Option opt)
      : moduleNode(moduleNode), opt(opt){};
};
} // namespace transform