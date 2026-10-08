cmake_minimum_required(VERSION 3.10)
project(twgec VERSION 1.0)
add_executable(twgec
    lib/frontend/ast.cpp
    lib/frontend/lexer.cpp
    lib/frontend/parser.cpp
    lib/frontend/preprocessor.cpp
    lib/transform/blockExtend.cpp
    lib/transform/mutableVarExpansion.cpp
    lib/transform/argBinding.cpp
    lib/transform/blockInlining.cpp
    lib/transform/blockLegalization.cpp
    lib/transform/constantFolding.cpp
    lib/transform/forLoopUnrolling.cpp
    lib/transform/functionInlining.cpp
    lib/transform/ifStatementPropagation.cpp
    lib/transform/implicitListPromotion.cpp
    lib/transform/symbolChecking.cpp
    lib/codegen/codegen.cpp
    lib/codegen/instruction/metadata.cpp
    lib/codegen/instruction/enhff/action.cpp
    lib/codegen/instruction/std/action.cpp
    lib/codegen/instruction/std/check.cpp
    lib/codegen/instruction/std/trigger.cpp
    lib/codegen/utils/builtin.cpp
    lib/codegen/utils/defaultMap.cpp
    lib/utils/keyword.cpp
    lib/main.cpp
)

target_include_directories(twgec PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/frontend
    ${CMAKE_CURRENT_SOURCE_DIR}/include/transform
    ${CMAKE_CURRENT_SOURCE_DIR}/include/codegen
    ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/include/config
    ${CMAKE_CURRENT_SOURCE_DIR}/include/option
)