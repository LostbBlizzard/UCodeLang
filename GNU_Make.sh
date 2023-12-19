if [ "$(uname)" = "Darwin" ];  
then
././Dependencies/bin/premake/premake5.app gmake2
else
././Dependencies/bin/premake/premake5 gmake2
fi