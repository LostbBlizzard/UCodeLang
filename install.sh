#!/usr/bin/env bash
ucodelangdir = "$HOME/.ucodelang"
ucodebindir = "$ucodelangdir/bin"
ucodemoddir = "$ucodelangdir/modules"
ucodedocdir = "$ucodelangdir/doc"

ucodelangtepdir ="$ucodelangdir/tep"
packeddir = "$ucodelangtepdir/packed"


mkdir -p "$ucodelangdir"
mkdir -p "$ucodebindir"
mkdir -p "$ucodelangtepdir"
mkdir -p "$HOME/.ucodelang/modules"

if [ "$(uname)" = "Darwin" ];  
then

elif [ "$(uname)" = "Linux" ]; 
then

fi
# download ULangPacked to ~/.ucodelang/tep/ULangPacked.tar.gz


tar -xvf $ucodelangtepdir/ULangPacked.tar.gz -C $packeddir

mv $packeddir/LICENSE.txt $ucodelangdir/LICENSE.txt

# Standard Librarys

mv $packed/UCodeAPI/StandardLibrary $ucodemoddir/StandardLibrary

mv $packed/UCodeAPI/NStandardLibrary $ucodemoddir/NStandardLibrary

mv $packed/UCodeAPI/BuildSystem $ucodemoddir/BuildSystem

mv $packed/UCodeAPI/CompilerAPI $ucodemoddir/CompilerAPI

# docs

mv $packed/Output/UCodeDocumentation $ucodedocdir

# bin

if [ "$(uname)" = "Darwin" ];  
then

mv $packed/Output/UCodelangCL/MacOS/Published/uclang $ucodebindir/uclang

mv $packed/Output/UCodeLanguageSever/MacOS/Published/uclanglsp $ucodebindir/uclanglsp

elif [ "$(uname)" = "Linux" ]; 
then

mv $packed/Output/UCodelangCL/linux64/Published/uclang $ucodebindir/uclang

mv $packed/Output/UCodeLanguageSever/linux64/Published/uclanglsp $ucodebindir/uclanglsp

fi

# add to path

if ! grep -Fxq "export PATH=$HOME/.ucodelang/bin" $HOME/.bashrc
then
 echo "export PATH=$HOME/.ucodelang/bin" >> $HOME/.bashrc
fi


if ! grep -Fxq "export PATH=$HOME/.ucodelang/bin" $HOME/.profile
then
 echo "export PATH=$HOME/.ucodelang/bin" >> $HOME/.profile
fi

#setup StandardLibrarys

$ucodebindir/uclang index $ucodemoddir/StandardLibrary

$ucodebindir/uclang index $ucodemoddir/NStandardLibrary

$ucodebindir/uclang index $ucodemoddir/BuildSystem

$ucodebindir/uclang index $ucodemoddir/CompilerAPI

rmdir $ucodelangtepdir

echo "installation of 'ucodelang' is complete try using \"uclang -help\"";
