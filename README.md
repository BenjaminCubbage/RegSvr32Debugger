# RegSvr32Debugger

Mimic regsvr32.exe to print out all of a COM-enabled DLL or OCX file's attempts to read and write from the Windows Registry.

### Introduction

This was a fun little project I made while experimenting with creating my own COM Classes in C++. COM Objects are essentially small pieces of data that contain VTable pointers and, optionally, private fields. The COM API is designed to be language- and location-agnostic, so that a COM object can be created and utilized no matter the target architecture or runtime. 

The idea is that these COM Classes can be loaded from anywhere by looking up their location in the Windows Registry. The DLL/OCX files are registered in the Windows System Registry using [regsvr32](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/regsvr32). All regsvr32 does to register a PE—as far as I can tell, at least—is call into the DllRegisterServer entrypoint and print the status code result.

RegSvr32Debugger mimics this behavior by doing exactly the same thing. The only difference is that each call to change the Windows System Registry is rerouted and logged using Microsoft Detours. This can be useful for finding out the COM CLSID of a DLL/OCX, or for debugging a misbehaving DllRegisterServer entrypoint.

Most of all, though, this was built as a learning experience for me. COM is still used extensively when referencing old windows libraries, but it's rarely used in modern native application development.

### Building this project

RegSvr32Debugger requires [Microsoft Detours](https://github.com/microsoft/Detours) as a dependency. It can be installed anywhere on your machine, but must be built for your target architecture via its Makefile. RegSvr32Debugger is currently only configured to run on x86 or x64.

To build RegSvr32Debugger, first clone the repository into the Visual Studio solution of your choosing. Then set the DETOURS_DIR config variable like:

```sh 
setx DETOURS_DIR "C:\path\to\Detours-4.x.x"
```

At that point, the project should be ready to build and run. If running in Visual Studio's debugger, make sure to set the command line arguments in `Project Properties > Configuration Properties > Command Line Arguments`.

### Usage

The built executable has the name regsvr32. This is because some components will not allow themselves to be registered if the calling process name is not regsvr32. We essentially need to "trick" them into registering themselves by spoofing the real exe.

```sh 
regsvr32 "path\to\dll.dll"
```

- `/?` Help
- `/v` Verbose. Print status codes of passed-through registry edit events
- `/guids` Only print out the pattern-matched GUIDs (CLSID/IID/etc.) of registry paths that were opened, created, or set
- `/setonly` Don't log RegOpen* or RegCreate* operations.
