DCEConfig.yaml serves as a blueprint instruction for both DCELoader and DCEAPIHook on how to operate.
This document contains:
* a list of some suggested software to gain information about your target process.
* some configuration examples.
* a list of allowed keyword/commands in a DCEConfiguration file.
* a list of the currently hookable APIs and their supported parameters.

## Suggested software

To be able to successfully write a DCE configuration file you need to gain knowledge about the specific game/software requirements.
This can be as easy as reading the volume label in file manager or more complex up to debugging the target process to understand its requirements.
Here are some ideas/techniques/software to help you to gain such knowledge.

### File Manager (explorer.exe)

As already said the most obvious thing to do is to obtain the volume name. Insert the original game disc and go to "My PC", then take note of the label used by the disc volume.

### API Monitors

To be able to easily obtain a list of the API called by the game/software you are writing a rule for, you will need a way to monitor its behavior.
Many software are able to record API access from a target process.
Here are some easy-to-use tools:

* [Process Monitor](https://learn.microsoft.com/it-it/sysinternals/downloads/procmon): Freeware.
* [API Monitor](http://www.rohitab.com/apimonitor): Freeware.
* APIS32 (AKA API SPY): Commercial. No longer being developed or sold.

Please refer to the documentation for each of these software tools.

### Debuggers

This is the most powerful way to gain knowledge from a process.
Using this software requires knowledge of both low-level programming and OS internals, but such not-so-easy-to-obtain skills are paid in full by the control you will get from EVERY binary running on your machine.
Some well known debuggers:

* [x64dbg/x32dbg](https://x64dbg.com/): Open source.
* [Ollydbg x32/x64](https://www.ollydbg.de/): Freeware. No longer developed.
* [IDA Free](https://hex-rays.com/ida-free/): Freeware. It is a disassembler but comes with debugging facilities too. Also proposes cloud-based x86/x64 decompiler.
* [Binary Ninja](https://binary.ninja/): Commercial. Like IDA, this is a disassembler but features debugging capabilities too.
* [Binary Ninja Free](https://binary.ninja/free/): Free edition of Binary Ninja.
* [BugChecker](https://github.com/vitoplantamura/BugChecker): Open source. RING 0 DEBUGGER working on Windows 11! If you was a SoftICE fan (like me) this is the way to go!

Using a debugger is surely the most complex way to gain the required information needed to write a DCE configuration, but sometimes is the only way. And, for what it's worth, is my favorite way :)

### DCE itself (Debug Version)

The debug version of DCE features a debug prompt with logs of each intercepted API.
This is especially useful when debugging your DCEConfig file.
Please use the debug version *ONLY* for debugging purposes.
Once you are satisfied with your DCEConfig file, you should run the performance-optimized release version.
When running the debug version, keep in mind that:
- "--->" means: data from the executable.
- "<---" means: data passed to the executable.

## Configuration Examples

### Example 1: an easy example 

Scenario: Let's suppose to have installed a game that during launch check the presence of a CD-ROM labeled "AWESOME_GAME". The main executable of such game is named "AG.exe". The game is not going to load any file from the CD-ROM.

Let's write a DCEConfig.yaml for this scenario:

```
loader:
  target: "AG.exe" 

virtual_drives: ['L']

hooks:
  - api: "GetDriveTypeA"
    arg1: "L:\\"
    return: 5 # DRIVE_CDROM
  - api: "GetVolumeInformationA"
    arg1: "L:\\"
    arg2: "AWESOME_GAME"
    return: true
```

So, we told to DCELoader to create a process using AG.exe. DCEAPIHook.dll will be injected in this process right after it creation.
We then defined a virtual drive, L. Defining a virtual drive will automatically create an hook to the GetLogicalDrives API and the presence of such defined drive will be spoofed.
We then have requested two hooks. The first one is hooking GetDriveTypeA: once the software will call this API passing the value "L:\\", DCE will intercept it and returns the value 5, that is DRIVE_CDROM, causing the game to think that this drive is actually a CD-ROM drive. The second hook is against GetVolumeInformationA: this API is used to retrieve the label of the disc inserted in the corresponding drive (if any), so once the game will call this api passing the value "L:\\", DCE will write "AWSOME_GAME" to the second param (lpVolumeNameBuffer) making the software to think that such labeled disc is actually inserted in the 'L' drive. Finally "true" value is returned.

Writing configurations for this type of software is super easy, as you only need to know the label of the required disc.

### Example 2: File redirection

Scenario: Let's suppose to have installed a game that during launch check the presence of a CD-ROM labeled "INCREDIBLE_GAME". The main executable of such game is named "IG.exe". The game will load two important files from the CD-ROM: "X:\\Stuff\\DATA0.DAT" and "X:\\Stuff\\DATA1.DAT" (where X is the drive letter).

We can use the file-redirection feature of DCE to easily write a DCEConfig.yaml file for this game:

```
# Remember to copy \Stuff\DATA0.DAT
# and \Stuff\DATA1.DAT from the
# original game disc to the 
# installation directory!

loader:
  target: "IG.exe" 

virtual_drives: ['L']

hooks:
  - api: "GetDriveTypeA"
    arg1: "L:\\"
    return: 5 # DRIVE_CDROM
  - api: "GetVolumeInformationA"
    arg1: "L:\\"
    arg2: "INCREDIBLE_GAME"
    return: true

file_redirections:
  - source: "L:\\Stuff\\DATA0.DAT"
    destination: ".\\Stuff\\DATA0.DAT"
  - source: "L:\\Stuff\\DATA1.DAT"
    destination: ".\\Stuff\\DATA1.DAT"
``` 

We already covered in the previous example the "loader", "virtual_drive" and "hooks" stuff. Let's focus on the new entry: "file_redirections". This list is used to let DCE know what file access to redirect and where.
We know that the game will try to access these two files located to the CD-ROM, but since we don't want to keep the original disc inserted, we copied to the game installation directory (we also added a comment to our configuration file to keep it in mind for the future). When we add a source and destination parameters for each of these files , DCE will automatically create an hook against the CreateFileA API, altering on the fly the path of the file being accessed (source) with the destination string. When the game will try to access these two files on the disc, it will actually access the ones copied to the game directory on the HDD.

### Example 3: hooking more APIs

Scenario: Let's suppose to have installed a game that during launch check the presence of a CD-ROM labeled "SUPER_GAME". The main executable of such game is named "SG.exe". This game is not loading any data from the disc, but it will use a somewhat weird method to check the presence of the original game disc: the mciSendCommand API.

Despite this hacky way to carry a cd-check, writing a config file is pretty easy:

```
loader:
  target: "SG.exe" 

virtual_drives: ['L']

hooks:
  - api: "GetDriveTypeA"
    arg1: "L:\\"
    return: 5 # DRIVE_CDROM
  - api: "mciSendCommand"
    arg2: 0x803 #MCI_OPEN
    return: 0
  - api: "mciSendCommand"
    arg2: 0x814 #MCI_STATUS
    status_return: 1 # SUCCESS
    return: 0
```

The new entry here are the two mciSendCommand hooks. Due to the way mciSendCommand works (please refer to the official Microsoft documentation) the hook implementation for this API is a bit hacky too. The first hook will be triggered when arg2 is 0x803 (ie, the MCI_OPEN command), bypassing the API call and returning 0.
The second mciSendCommand hook will be fired when the second argument is equals to 0x814 (MCI_STATUS): after calling this API the game is passing a MCI_STATUS_PARMS as the fourth parameter to this API and it expects to receive a valid and filled MCI_STATUS_PARMS to that address. The "status_return" is instructing the hook to put 1 as the value of dwReturn filed of this MCI_STATUS_PARMS. This is enough to make the game happy.

## DCEConfig commands

In your DCEConfig.yaml files you can use the following commands:

loader->target: defines the process to be created. DCEAPIHook.dll will be injected there.

loader->args: defines the arguments to pass to the process to be created. This is an optional field.

virtual_drives: this is an array of desired fake drives. Adding a lected to this list will automatically reports the presence of such drive when the software calls GetLogicalDrives API.

hooks: this is the list of the desired hooks. Once DCEAPIHook.dll in injected in the target process, every API configured here will be hooked. Multiple hooks for each API are allowed. Please consult the "Supported APIs" part of this document for a list of supported APIs and their allowed parameters and return values.

file_redirections: this is a list of the desired file redirections. Each entry in this list is composed by a source value, ie the original file path that the game uses, and a destination value, ie where the CreateFileA API will be redirected when trying to access such file.

## Supported API Hooks

This is a list of the currently supported APIs. Each API have its own customizable arguments and return value. Please note that the argument used as trigger is different for each API.
You should refer to the official Microsoft documentation for a full description of each API argument.

### GetDiskFreeSpaceA

GetDiskFreeSpaceA signature:

```
BOOL GetDiskFreeSpaceA(
  [in]  LPCSTR  lpRootPathName,
  [out] LPDWORD lpSectorsPerCluster,
  [out] LPDWORD lpBytesPerSector,
  [out] LPDWORD lpNumberOfFreeClusters,
  [out] LPDWORD lpTotalNumberOfClusters
);
```

Hook configuration:

- arg1 (lpRootPathName): when this string matches the one passed by the calling code, the hook will be triggered.
- arg2 (lpSectorsPerCluster): the number of sectors per cluster.
- arg3 (lpBytesPerSector): the number of bytes per sector.
- arg4 (lpNumberOfFreeClusters): the total number of free clusters on the disk.
- arg5 (lpTotalNumberOfClusters):  the total number of clusters on the disk.
- return: when the hook is triggered, this value will be returned to the calling code.

### GetDriveTypeA

GetDriveTypeA signature:

```
UINT GetDriveTypeA(
  [in, optional] LPCSTR lpRootPathName
);
```

Hook configuration:

- arg1 (lpRootPathName): when this string matches the one passed by the calling code, the hook will be triggered.
- return: when the hook is triggered, this value will be returned to the calling code.

### GetFileAttributesA

GetFileAttributesA signature:

```
DWORD GetFileAttributesA(
  [in] LPCSTR lpFileName
);
```

Hook configuration:

- arg1 (lpFileName): when this string matches the one passed by the calling code, the hook will be triggered.
- return: when the hook is triggered, this value will be returned to the calling code. For a list of valid File Attribute Constants see the official [Microsoft Documentation](https://learn.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants).

### GetVolumeInformationA

GetVolumeInformationA signature:

```
BOOL GetVolumeInformationA(
  [in, optional]  LPCSTR  lpRootPathName,
  [out, optional] LPSTR   lpVolumeNameBuffer,
  [in]            DWORD   nVolumeNameSize,
  [out, optional] LPDWORD lpVolumeSerialNumber,
  [out, optional] LPDWORD lpMaximumComponentLength,
  [out, optional] LPDWORD lpFileSystemFlags,
  [out, optional] LPSTR   lpFileSystemNameBuffer,
  [in]            DWORD   nFileSystemNameSize
);
```

Hook configuration:

- arg1 (lpRootPathName): when this string matches the one passed by the calling code, the hook will be triggered.
- arg2 (lpVolumeNameBuffer): this is a string that will be copied to the buffer passed to the function.
- arg3 (nVolumeNameSize): OPTIONAL. When not specified will be used the same value passed to the function.
- arg4 (lpVolumeSerialNumber): OPTIONAL. Default to 0.
- arg5 (lpMaximumComponentLength): OPTIONAL. Default to 0.
- arg6 (lpFileSystemFlags): OPTIONAL. Default to 0.
- arg7 (lpFileSystemNameBuffer): OPTIONAL. Default to empty string. This string will be copied to the buffer passed to the function, only if it isn't NULL.
- arg8 (nFileSystemNameSize): OPTIONAL. When not specified will be used the same value passed to the function.
- return: the boolean value returned to the calling code.

### mciSendCommand

Due to the way this API works, its hooking implementation is a bit hacky.

mciSendCommand signature:

```
MCIERROR mciSendCommand(
   MCIDEVICEID IDDevice,
   UINT        uMsg,
   DWORD_PTR   fdwCommand,
   DWORD_PTR   dwParam
);
```

ATTENTION: the dwParam argument is a pointer to a structure that contains parameters for the command message. This structure is different for each uMsg value. Please check the official Microsoft documentation for a full list of supported values and structures.

Hook configuration:

- arg2 (uMsg): when this value (uint32_t) matches the one passed to this API, the hook will be triggered.
- return: this is the value (uint32_t) returned to the calling code.
- status_return: *ONLY USED IF uMsg is 0x814 (MCI_STATUS).* This value (uint32_t) will be assigned to the dwReturn field of the MCI_STATUS_PARMS structure passed (as dwParam) to the mciSendCommand API. 

### RegEnumValueA

RegEnumValueA signature:

```
LSTATUS RegEnumValueA(
  [in]                HKEY    hKey,
  [in]                DWORD   dwIndex,
  [out]               LPSTR   lpValueName,
  [in, out]           LPDWORD lpcchValueName,
                      LPDWORD lpReserved,
  [out, optional]     LPDWORD lpType,
  [out, optional]     LPBYTE  lpData,
  [in, out, optional] LPDWORD lpcbData
);
```

Due to its nature, the configuration of this hook is a bit different:

- reg_path: this is the (OPTINONAL) path of the target key in the registry.
- key_name: this is the name of the targeted key.
- key_type: this is the type of the targeted key. Supported values: 0x1 (REG_SZ), 0x4 (REG_DWORD) and 0xB (REG_QWORD).
- value: this is the desired value that you want to be written in the buffer.
- value_size: this is the size, in bytes, of the desired value.
- return: this is the value returned to the calling code. Use 0 for ERROR_SUCCESS.

A complete and working example:
```
  - api: "RegEnumValueA"
    reg_path: "\\REGISTRY\\MACHINE\\SOFTWARE\\WOW6432Node\\TEST\\123\\R2"
    key_name: "Drive"
    key_type: 0x1
    value: "L:\\DIR\\"
    value_size: 0x8
    return: 0
 ```
 In this example, the target key is named "Drive", is located in "HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\TEST\123\R2",
 is of type REG_SZ (0x1, i.e., string), the desired value is "L:\DIR\" and the size is 8 bytes (don't forget the '\0' at the end).

 Some things to keep in mind:
 * Make sure that you use the SAME key type as the one expected by the software you are targeting. 
 * Make sure that the value size doesn't exceed the buffer size allocated by the software you are targeting.
 * If you don't want to check the full registry path, you can pass an empty string ("") as reg_path.

### RegQueryValueExA

RegQueryValueExA signature:

```
LSTATUS RegQueryValueExA(
  [in]                HKEY    hKey,
  [in, optional]      LPCSTR  lpValueName,
                      LPDWORD lpReserved,
  [out, optional]     LPDWORD lpType,
  [out, optional]     LPBYTE  lpData,
  [in, out, optional] LPDWORD lpcbData
);
```

Due to its nature, the configuration of this hook is a bit different:

- reg_path: this is the (OPTINONAL) path of the target key in the registry.
- key_name: this is the name of the targeted key.
- key_type: this is the type of the targeted key. Supported values: 0x1 (REG_SZ), 0x4 (REG_DWORD) and 0xB (REG_QWORD).
- value: this is the desired value that you want to be written in the buffer.
- value_size: this is the size, in bytes, of the desired value.
- return: this is the value returned to the calling code. Use 0 for ERROR_SUCCESS.

A complete and working example:
```
  - api: "RegQueryValueExA"
    reg_path: "\\REGISTRY\\MACHINE\\SOFTWARE\\WOW6432Node\\TEST\\123\\R2"
    key_name: "Drive"
    key_type: 0x1
    value: "L:\\DIR\\"
    value_size: 0x8
    return: 0
 ```
 In this example, the target key is named "Drive", is located in "HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\TEST\123\R2",
 is of type REG_SZ (0x1, i.e., string), the desired value is "L:\DIR\" and the size is 8 bytes (don't forget the '\0' at the end).

 Some things to keep in mind:
 * Make sure that you use the SAME key type as the one expected by the software you are targeting. 
 * Make sure that the value size doesn't exceed the buffer size allocated by the software you are targeting.
 * If you don't want to check the full registry path, you can pass an empty string ("") as reg_path.

### Other Hook

This is a list of hooks automatically created by DCE:

* CreateFileA: this hook will be automatically created when using the file redirection feature.
* FindFirstFileA: this hook will be automatically created when using the file redirection feature.
* GetFileAttributesA: this hook will be automatically created when using the file redirection feature.
* GetLogicalDrives: this hook will be created when at least one virtual drive is specified in virtual_drives array.

## Patches / Hacks

Since version 0.4.0 DCE supports applying patches/hacks to the target process memory.
This feature is mainly useful in three cases:
1. to fix bugs that causes the target process to crash or misbehave on recent Windows versions.
2. to alter some values that are hardcoded in the executable (e.g., to change the supported resolution etc.) 
3. in some rare cases, when you need to alter the behavior of the target process in a way that can't be achieved using the hooks (e.g., to bypass a check that is not an API call).
The patches are loaded from the "patches" section of DCEConfig file and are applied to the target process memory after the hooks are set.
ATTENTION: Patches can make the target process unstable or crash. Use them with caution. Patches are often not compatible with different versions of the same software.

### Patch structure

Each patch is composed by the following fields:
- rva: the relative virtual address in the target process memory where the patch will be applied. This address will be added to the base address of the target process.
- patch: an array of bytes that will be written to the target process memory.
OR:
- offset: the physical file offset where the patch will be applied. This offset will be automatically converted in a virtual address by DCE.
- patch: an array of bytes that will be written to the target process memory.

For each patch you must specify either the offset or the rva field.

### Patch examples

#### Example 1: Patching using file offset

This is an example of a patch that changes 3 bytes starting from file offset 0x178C.
In this case the offset is automatically converted in a virtual address by DCE.
```
patches:
  - offset: 0x178C
    patch: [0x41, 0x42, 0x43]
```

#### Example 2: Patching using a virtual address

This is an example of a patch that changes 3 bytes starting from relative virtual address 0x0061318C:
```
patches:
  - rva: 0x318D
    patch: [0x41, 0x42, 0x43]
```

## Cheats

Since version 0.5.0 DCE supports cheats. Cheats are like patches, but they are applied to the target process memory at a specific time.
There are two types of cheats: "SingleShot" and "Toggables".
SingleShot cheats will just overwrite a memory area once triggered, while Toggables cheats can be enabled and disabled multiple times.
You can use tools like Cheat Engine to find the memory addresses you want to change and then write the corresponding cheats in the DCEConfig file.
When a cheat is enabled/disabled, you will hear a sound notification.

### Cheats structure

Cheats are loaded from the "cheats" section of DCEConfig file.

A SingleShot cheat is composed by the following fields:
- vkey: the virtual key code of the key that will trigger the cheat. You can find a list of virtual key codes [here](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes).
- rva: the relative virtual address in the target process memory where the patch will be applied. This address will be added to the base address of the target process.
- cheat: an array of bytes that will be written to the target process memory.

- A Toggable cheat is composed by the following fields:
- vkey: the virtual key code of the key that will trigger the cheat. You can find a list of virtual key codes [here](https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes).
- rva: the relative virtual address in the target process memory where the patch will be applied. This address will be added to the base address of the target process.
- original_bytes: the original bytes that will be restored when the cheat is disabled.
- cheat: an array of bytes that will be written to the target process memory.

Please note that the only difference between a SingleShot cheat and a Toggable cheat is the presence of the original_bytes field in the Toggable cheat.
This field is used to store the original bytes that will be restored when the cheat is disabled.
If you don't specify the original_bytes field, the cheat will be assumed to be SingleShot.

### Cheats examples

#### Example 1: SingleShot cheat

This is an example of a SingleShot cheat that changes 2 bytes starting from relative virtual address 0x64F22C when the "F1" key is pressed:
```
  - vkey: 0x70  # F1 key
    rva: 0x64F22C
    cheat: [0xAA, 0xBB]
```
When the "F1" key is pressed, the bytes at the address 0x64F22C will be changed to 0xAA and 0xBB.

#### Example 2: Toggable cheat

This is an example of a Toggable cheat that changes 6 bytes starting from relative virtual address 0x43F15 when the "F2" key is pressed:
```
  - vkey: 0x71  # F2 key
	rva: 0x43F15
	original_bytes: [0xFF, 0x0D, 0x2C, 0xF2, 0xA4, 0x00]
	cheat: [0x90, 0x90, 0x90, 0x90, 0x90, 0x90]
```
When the "F2" key is pressed, the bytes at the address 0x43F15 will be changed to 0x90, 0x90, 0x90, 0x90, 0x90 and 0x90.
When the "F2" key is pressed again, the bytes at the address 0x43F15 will be restored to 0xFF, 0x0D, 0x2C, 0xF2, 0xA4 and 0x00.