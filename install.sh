#!/usr/bin/env bash

cd "$HOME"
mkdir -p "$HOME/.ucodelang"

mkdir -p "$HOME/.ucodelang/bin"

cd "$HOME/.ucodelang/bin"

if [[ "$(uname)" = "Darwin" ]];  
then
curl -LJO https://github.com/LostbBlizzard/UCodeLang/releases/download/Release-0.0.5/uclang-macos

mv uclang-macos uclang
chmod +x uclang

curl -LJO https://github.com/LostbBlizzard/UCodeLang/releases/download/Release-0.0.5/uclanglsp-macos

mv uclanglsp-macos uclanglsp

chmod +x uclanglsp

elif [[ "$(uname)" = "Linux" ]]; 
then

curl -LJO https://github.com/LostbBlizzard/UCodeLang/releases/download/Release-0.0.5/uclang-linux64

mv uclang-linux64 uclang
chmod +x uclang

curl -LJO https://github.com/LostbBlizzard/UCodeLang/releases/download/Release-0.0.5/uclanglsp-linux64

mv uclanglsp-linux64 uclanglsp

chmod +x uclanglsp

echo ""

fi

mkdir -p "$HOME/.ucodelang/modules"

cd "$HOME/.ucodelang/modules"

mkdir ./gitprojecttep/
git clone https://github.com/LostbBlizzard/UCodeLang ./gitprojecttep
cp -r ./gitprojecttep/UCodeAPI/StandardLibrary ./
cp -r ./gitprojecttep/UCodeAPI/NStandardLibrary ./

rm -f -r ./gitprojecttep/ 

if ! grep -Fxq "export PATH=$HOME/.ucodelang/bin" $HOME/.bashrc
then
 echo "export PATH=$HOME/.ucodelang/bin" >> $HOME/.bashrc
fi


if ! grep -Fxq "export PATH=$HOME/.ucodelang/bin" $HOME/.profile
then
 echo "export PATH=$HOME/.ucodelang/bin" >> $HOME/.profile
fi

if ! grep -Fxq "export PATH=$HOME/.ucodelang/bin" $HOME/.bash_profile
then
 echo "export PATH=$HOME/.ucodelang/bin" >> $HOME/.bash_profile
fi

cd "$HOME/.ucodelang/bin"

./uclang index "$HOME/.ucodelang/modules/StandardLibrary"

./uclang index "$HOME/.ucodelang/modules/NStandardLibrary"


echo "installation of 'ucodelang' is complete try using \"uclang -help\"";
