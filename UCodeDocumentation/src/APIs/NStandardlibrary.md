# NStandardlibrary(API)

The NStandardlibrary is the API that Standardlibrary uses to talk to operating system to use files,sockets and memory.

The NStandardlibrary uses [conditional compliation](../SyntaxAndSeamantics/Concepts/conditional%20compliation.md) to use to call the correct API specific function or use the API Object.

The NStandardlibrary is currently maintained by LostbBlizzard On [Github](https://github.com/LostbBlizzard).


On Windows NStandardlibrary uses [Win32](./Win32.md) API.


For embedded systems.developers should make there own variant NStandardlibrary or use an existing one for their specific purpose.

