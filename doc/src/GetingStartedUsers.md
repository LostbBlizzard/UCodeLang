# Geting Started Users

Despite that UCodeLang was designed to embed into applications. You can use Directly using uclang command line tool.

## Download

You may use the [Playground](https://lostbblizzard.github.io/UCodeLang/UCodeIDE/UCodeIDE.html) to try it without Downloading.

Run The following in your terminal to install on linux/MacOs.

```
curl -L https://github.com/LostbBlizzard/UCodeLang/releases/latest/download/install.sh | sh
```

Or Download the [Installer](https://github.com/LostbBlizzard/UCodeLang/releases/latest/download/UCodeLangSetup.exe) on the [latest release](https://github.com/LostbBlizzard/UCodeLang/releases/latest) for Windows.

To test that it was installed in your command prompt try runing.

```
uclang help
```

## Extension

lets also download the [VsCode extension](https://marketplace.visualstudio.com/items?itemName=Lostblizzard.ucode-language) to make our lives easier.

just search up UCodeLang by Lost blizzard in your extension tab.

## Hello main

after geting your extension. run

```
uclang new
```

This will ask you about the Project name and author's name.

You can put HelloMain as the ProjectName.
and MySelf as the AuthorName.

Inside the new folder.You will Have this folder structure.

```
- HelloMain

  - src
    - main.uc

  - ULangModule.ucm

  - Build.uc

```

The 'src' Folder is were all your code is going to be.
UCodeLang uses the .uc file extension for all source files.

main.uc is were your main funcion is.This is were your program will start executing.

[Build.ucm](./Users/BuildScript.md) Is your build script. this allows you to do stuff before and after you build your project. you can ignore this file or even delete it for now.

Lasly [ULangModule.uc](./Users/ULangModule.md) Is were you place your dependencies.

## build and runing

Now lets build our Hello World Program
by runing uclang build.

```
uclang build
```

And run to program by using uclang run

```
uclang run
```

You should see Hello World On your Screen.

Also See [Syntax And Semantics](./SyntaxAndSemantics.md)

Also See [Standardlibrary](./Standardlibrary.md)
