# Parser
### Symbol and Naming
**Logical Symbol**

`|` means or

`?` means optional

`*` means zero to many

`[]` defines the parsing priority

**Named Symbol**

`<xxx>Node` is the AST node that is declared in AST.h

`<xxx>Token` is the token that is declared in Token.h

`<XXX>` with all uppercase characters is an intermediate expression

### DSL Parsing Logic

##### Overview

![AST image](/assets/frontend/ast.png)

##### Regular Expression
```c
// Global Scope
ModuleNode := [ MetadataNode | ConstDefNode | FunDefNode | BlockNode | SemicolonToken ]*
- MetadataNode := MetadataToken AssignToken ExpNode SemicolonToken
- ConstDefNode := ConstToken ScopedIdentifierToken AssignToken ExpNode SemicolonToken
- FunDefNode := DefToken ScopedIdentifierToken ParamDefsNode ColonToken FunDefBody
  - ParamDefsNode := OpenParToken [ NonDefaultParamDefNode [CommaToken NonDefaultParamDefNode]* [CommaToken DefaultParamDefNode [CommaToken DefaultParamDefNode]* ]? | DefaultParamDefNode [CommaToken DefaultParamDefNode]* ]? CloseParToken
    - NonDefaultParamDefNode := IdentifierToken
    - DefaultParamDefNode := IdentifierToken AssignToken ExpNode
  - FunDefBody := BlockFunDefBody | TypedFunDefBody
    - BlockFunDefBody := BlockToken [ BlockBodyNode | [AssignToken InstructionNode] ]
    - TypedFunDefBody := TypedDefToken InstrSetNode
- BlockNode := BlockToken IdentifierToken [ BlockBodyNode | [AssignToken InstructionNode] ]
- BlockBodyNode := OpenCurToken [ MetadataNode | TypedInstrSetNode ]* CloseCurToken
// Instruction Set Scope
TypedInstrSetNode := TypedDefToken InstrSetNode
- TypedDefToken := ActionsToken | ChecksToken | TriggersToken
- InstrSetNode := OpenCurToken CompositeInstrNode* CloseCurToken
  - CompositeInstrNode := BranchNode | ForLoopNode | InstructionNode
    - BranchNode := IfRegionNode [ElseToken IfRegionNode]* [ElseToken InstrSetNode]?
      - IfRegionNode := IfToken OpenParToken ExpNode CloseParToken InstrSetNode
    - ForLoopNode := ForToken OpenParToken IdentifierToken InToken ExpNode [EllipsisToken ExpNode]? CloseParToken InstrSetNode
// Instruction Scope
InstructionNode := [ScopedIdentifierToken | IntrinsicAssertToken] ParamAppsNode SemicolonToken
- ParamAppsNode := OpenParToken [ PositionalArgNode [CommaToken PositionalArgNode]* [CommaToken NamedArgNode [CommaToken NamedArgNode]* ]? | NamedArgNode [CommaToken NamedArgNode]* ]? CloseParToken
  - PositionalArgNode := ExpNode
  - NamedArgNode := IdentifierToken AssignToken ExpNode
// Expression
ExpNode := LogicalOrExpNode
- LogicalOrExpNode = LogicalAndExpNode [ OrToken LogicalAndExpNode ]*
- LogicalAndExpNode = EqualityExpNode [ AndToken EqualityExpNode ]*
- EqualityExpNode = RelationalExpNode [ [ EqualToken | NotEqualToken] RelationalExpNode ]*
- RelationalExpNode = AdditiveExpNode [ [LessThanToken | GreaterThanToken | LessThanEqualToken | GreaterThanEqualToken] AdditiveExpNode ]*
- AdditiveExpNode = MultiplicativeExpNode [ [AddToken | SubToken] MultiplicativeExpNode ]*
- MultiplicativeExpNode = IntrinsicExpNode [ [MulToken | DivToken | ModToken] IntrinsicExpNode ]*
- IntrinsicExpNode = IntrinsicNode | PrimaryExpNode
  - IntrinsicNode = IntrinsicCallNode OpenParToken ExpNode [CommaToken ExpNode]* CloseParToken
  - IntrinsicCallNode = IntrinsicToString | IntrinsicToInt | IntrinsicToBool | IntrinsicGetIndex | IntrinsicGetLength | IntrinsicGetSlice
- PrimaryExpNode = ValueNode | [OpenParToken ExpNode CloseParToken]
ValueNode := StringValueNode | IntValueNode | NegativeIntValueNode | BoolValueNode | VariableValueNode | PointValueNode | ActorMatchValueNode | ButtonValueNode | CustomWeaponValueNode | ListValueNode
- StringValueNode := StringToken
- IntValueNode := IntToken
- NegativeIntValueNode := SubToken IntToken
- BoolValueNode := TrueToken | FalseToken
- VariableValueNode := ScopedIdentifierToken
- PointValueNode := PointToken OpenParToken ExpNode CommaToken ExpNode CloseParToken
- ActorMatchValueNode := ActorMatchToken ParamAppsNode
- ButtonValueNode := ButtonToken ParamAppsNode
- CustomWeaponValueNode := CustomWeaponToken ParamAppsNode
- ListValueNode := OpenSqrToken [ExpNode [CommaToken ExpNode]*]? CloseSqrToken
// Other
- ScopedIdentifierToken := IdentifierToken [ScopeToken IdentifierToken]*
```
