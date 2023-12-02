if [ "$(uname)" = "Darwin" ];  
then
././Dependencies/bin/premake/premake5.app install
else
././Dependencies/bin/premake/premake5 install
fi