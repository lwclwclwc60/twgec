# twgec
`twgec` stands for the `twilight game.events compiler`. It compiles the `.twge` DSL into the `game.events` that can be used in the [Twilight War Game Creation](https://code.gamelet.com/).

# Setup the twgec

## Setup Syntax Highlighter

Developers can install the prebuilt VSIX directly from `tool/bin`.

1. Open VS Code.
2. Run `Extensions: Install from VSIX...`.
3. Select the latest `tool/bin/twge-syntax-highlighter-<version>.vsix`.

For extension development, packaging, and local build flow, see `tool/twge-syntax-highlighter/README.md`.

With the syntax highlighter. the `.twge` will have syntax highlighting as shown below:

![The highlighted .twge syntax](/assets/readme/syntax-highlighting.png)

## Build twgec

To build the twgec (twge compiler), you have to execute the `build.sh` as below:
```
./bash.sh
```
After, the build directory will be created. To run the `twgec` with a simple testcase. You might run
```
build/twgec test/codegen/actions/success/console/test.twge
```
to see if the `game.events` can be generated (`ls game.events`)

# Start to code

## Help

By running the following command:
```
build/twgec --help
```
guidelines on how to use the `twgec` will be shown.

## Simple Example

![A simple example of twge](/assets/readme/simple-twge.png)

As the code snippet above, `block` is the basic unit of the game event.

Under a `block`, you can add the `triggers`, `checks`, and `actions` here these have the exact same meanings as those in the [Twilight War Game Creation](https://code.gamelet.com/) have.

Under a `block`, some metadata such as `__repeat__` and `__delay__` etc. can be set.

With this simple example, the `game.events` can be generated.

## Advanced Feature

### Function Definition

#### Block function

By referring to the `Simple Example` Section, the implementation of the block `chooseJobS1` and `chooseJobS2` are highly repetitive. In order to increase the usability, function definition should be used.

![The twge example with block function definition](/assets/readme/fun-def-block.png)

As the code snippet above, `def <functionDefinitionName>(<param>...): block {}` can be used here.  `def` means that is a function definition. `: block` means that is the function definition of the class `block`. The `block`\'s implementation can be completed inside the bracket (`{}`).

In order to call the function, the syntax should be `block <blockName> = <functionDefinitionName>(<arg>...);` as the code snippet above.

#### Actions function

![The twge example with action function definition](/assets/readme/fun-def-action.png)

Sometimes, the `actions`, `triggers`, and `checks` might have some complicated and duplicated logic. In order to encapsulate and reuse the code implementation, function can be used as above (see `biMapWarp`). It is similar to the block function definition. But remember, the type of the definition should be `: actions`, `: triggers`, or `: checks` instead of `: block`.