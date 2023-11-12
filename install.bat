echo off
cd %UserProfile%
mkdir .ucodelang

cd ./.ucodelang

mkdir "bin"

mkdir "modules"

cd ./bin

curl.exe -LJO https://github.com/LostbBlizzard/UCodeLang/releases/download/Release-0.0.5/uclang-win64.exe

ren uclang-win64.exe  uclang.exe

curl.exe -LJO https://github.com/LostbBlizzard/UCodeLang/releases/download/Release-0.0.5/uclanglsp-win64.exe

ren uclanglsp-win64.exe  uclanglsp.exe

cd ./../modules

git clone https://github.com/LostbBlizzard/UCodeLang ./gitprojecttep
move ./gitprojecttep/UCodeAPI/StandardLibrary ./
move ./gitprojecttep/UCodeAPI/NStandardLibrary ./

RMDIR /S /Q "gitprojecttep/"


call "%UserProfile%/.ucodelang/bin/uclang" index "%UserProfile%/.ucodelang/modules/StandardLibrary

call "%UserProfile%/.ucodelang/bin/uclang" index "%UserProfile%/.ucodelang/modules/NStandardLibrary

echo ""
echo ""

echo add environment variable "%UserProfile%/.ucodelang/bin" to your PATH to complete
echo installation of ucodelang. then try using \"uclang -help\";
pause