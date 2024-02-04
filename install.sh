#!/usr/bin/env bash


ucodelangdir="$HOME/.ucodelang"
ucodebindir="$ucodelangdir/bin"
ucodemoddir="$ucodelangdir/modules"
ucodedocdir="$ucodelangdir/doc"

ucodelangtepdir="$ucodelangdir/tep"
packeddir="$ucodelangtepdir/packed"

mkdir -p "$ucodelangdir"
mkdir -p "$ucodebindir"
mkdir -p "$ucodelangtepdir"
mkdir -p "$HOME/.ucodelang/modules"

if [ "$(uname)" = "Darwin" ];  
then

echo "download"

elif [ "$(uname)" = "Linux" ]; 
then

echo "download"

fi
# download ULangPacked to ~/.ucodelang/tep/ULangPacked.tar.gz

mkdir -p $packeddir

tar -xvf $ucodelangtepdir/ULangPacked.tar.gz -C $packeddir

mv $packeddir/LICENSE.txt $ucodelangdir/LICENSE.txt

# Standard Librarys

mv $packeddir/UCodeAPI/StandardLibrary $ucodemoddir/StandardLibrary

mv $packeddir/UCodeAPI/NStandardLibrary $ucodemoddir/NStandardLibrary

mv $packeddir/UCodeAPI/BuildSystem $ucodemoddir/BuildSystem

mv $packeddir/UCodeAPI/CompilerAPI $ucodemoddir/CompilerAPI

# docs

mv $packeddir/Output/UCodeDocumentation $ucodedocdir

# bin

if [ "$(uname)" = "Darwin" ];  
then

mv $packeddir/Output/UCodelangCL/MacOS/Published/uclang $ucodebindir/uclang

mv $packeddir/Output/UCodeLanguageSever/MacOS/Published/uclanglsp $ucodebindir/uclanglsp

elif [ "$(uname)" = "Linux" ]; 
then

mv $packeddir/Output/UCodelangCL/linux64/Published/uclang $ucodebindir/uclang

mv $packeddir/Output/UCodeLanguageSever/linux64/Published/uclanglsp $ucodebindir/uclanglsp

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

rmdir -r -f $ucodelangtepdir

echo "installation of 'ucodelang' is complete try using \"uclang -help\"";
