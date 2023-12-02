if [ "$(uname)" = "Darwin" ];
then
././Dependencies/bin/premake/premake5.app $1
else
././Dependencies/bin/premake/premake5 $1
fi