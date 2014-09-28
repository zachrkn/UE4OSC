// UE4OSC created by Zach Duer
// see readme for copyright, permissions and credits
// oscpack created by Ross Bencina, see oscpack files for applicable copyright and permissions

#if defined(__cplusplus) || defined(_cplusplus)
extern "C" {
#endif

#pragma once
//#include "includes/targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h> // Windows Header Files:

// ======================= VARIABLES =====================================
float MyFloat = 0.1;

float * MyFloatArray;

int MyArraySize;

int ReceivePort;

// ======================= DLL GET FUNCTIONS (called to update UDK with external program info) =====================================
__declspec(dllexport) float GetOSCFloat();
__declspec(dllexport) float * GetOSCFloatArray();
__declspec(dllexport) int GetOSCArraySize();

// ======================= DLL SEND FUNCTIONS (called to update external program via UDP) =====================================
__declspec(dllexport) void SendOSCStringMessage(char* Host, int SendPort);
__declspec(dllexport) void SendOSCFloat(char* AddressPattern, float FloatMsg, char* Host, int SendPort);

// ======================= OSC STRUCTURAL FUNCTIONS =====================================
// Initialize the OSC Receiver
__declspec(dllexport) void StartOSCReceiver(int ReceivePort);
__declspec(dllexport) void StopOSCReceiver();
unsigned int __stdcall ReceiveOSCMessage(void *arg);

// ======================= MISCELLANEOUS FUNCTIONS =====================================


#if defined(__cplusplus) || defined(_cplusplus)
}
#endif
