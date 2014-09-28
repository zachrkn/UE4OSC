// UE4OSC created by Zach Duer
// see readme for copyright, permissions and credits

#include "UE4OSCPluginPrivatePCH.h"

DEFINE_LOG_CATEGORY(YourLog);

typedef void(*_StartOSCReceiver)(int ReceivePort); //the typedef is must be the same as the dll function, exactly matching name, return type, and argument types
typedef void(*_StopOSCReceiver)();
typedef float(*_GetOSCFloat)(); 
typedef float*(*_GetOSCFloatArray)();
typedef int(*_GetOSCArraySize)();
typedef void(*_SendOSCFloat)(char* AddressPattern, float FloatMsg, char* Host, int SendPort);

AUE4OSCPluginOSCInterface::AUE4OSCPluginOSCInterface(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUE4OSCPluginOSCInterface::BeginPlay()
{
	//UE_LOG(YourLog, Warning, TEXT("OSCInterface Begin Play!"));
	Super::BeginPlay();
}

void AUE4OSCPluginOSCInterface::Tick(float DeltaTime)
{
	//UE_LOG(YourLog, Warning, TEXT("OSCInterface Tick!"));
	Super::Tick(DeltaTime);
}

/**
OSCSetHostnameAndPort MUST be called before OSCStartReceiver, because OSCReceivePort needs to be set.
*/
void AUE4OSCPluginOSCInterface::OSCSetHostnameAndPort(FString Hostname, int32 ReceivePort, int32 SendPort)
{
	OSCHostname = Hostname;
	OSCReceivePort = ReceivePort;
	OSCSendPort = SendPort;
}

void AUE4OSCPluginOSCInterface::OSCStartReceiver()
{
	FString filePath = FPaths::Combine(*FPaths::GamePluginsDir(), TEXT("UE4OSCPlugin/Binaries/Win64"), TEXT("UE4OSC.dll")); // get the plugin path, add the folder to contain the dll, and add the dll name
	if (FPaths::FileExists(filePath))
	{
		void *DLLHandle;
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath); // get reference to the dll
		if (DLLHandle != NULL)
		{
			_StartOSCReceiver DLLStartOSCReceiver = NULL; // declare the container for the dll function
			FString procName = "StartOSCReceiver"; // set the exact name of the dll function to recover
			DLLStartOSCReceiver = (_StartOSCReceiver)FPlatformProcess::GetDllExport(DLLHandle, *procName); // get reference to the dll function
			if (DLLStartOSCReceiver != NULL)
			{
				DLLStartOSCReceiver(OSCReceivePort); // call the dll function
			}
		}
	}
}

/* *** Not yet functioning in the DLL ***
void AUE4OSCPluginOSCInterface::OSCStopReceiver()
{
	FString filePath = FPaths::Combine(*FPaths::GamePluginsDir(), TEXT("UE4OSCPlugin/Binaries/Win64"), TEXT("UE4OSC.dll"));
	if (FPaths::FileExists(filePath))
	{
		void *DLLHandle;
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath);
		if (DLLHandle != NULL)
		{
			_StopOSCReceiver DLLStopOSCReceiver = NULL;
			FString procName = "StopOSCReceiver";
			DLLStopOSCReceiver = (_StopOSCReceiver)FPlatformProcess::GetDllExport(DLLHandle, *procName);
			if (DLLStopOSCReceiver != NULL)
			{
				DLLStopOSCReceiver();
			}
		}
	}
}
*/

float AUE4OSCPluginOSCInterface::OSCGetFloat()
{
	FString filePath = FPaths::Combine(*FPaths::GamePluginsDir(), TEXT("UE4OSCPlugin/Binaries/Win64"), TEXT("UE4OSC.dll"));
	if (FPaths::FileExists(filePath))
	{
		void *DLLHandle;
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath);
		if (DLLHandle != NULL)
		{
			_GetOSCFloat DLLGetOSCFloat = NULL;
			FString procName = "GetOSCFloat";
			DLLGetOSCFloat = (_GetOSCFloat)FPlatformProcess::GetDllExport(DLLHandle, *procName);
			if (DLLGetOSCFloat != NULL)
			{
				float out = DLLGetOSCFloat();
				return out;
			}
		}
	}
	return 1.00f; // return 1.00 to UE4 if the UE4OSC DLL is not found
}

TArray<float> AUE4OSCPluginOSCInterface::OSCGetFloatArray()
{
	TArray<float> OutArray;

	FString filePath = FPaths::Combine(*FPaths::GamePluginsDir(), TEXT("UE4OSCPlugin/Binaries/Win64"), TEXT("UE4OSC.dll"));
	if (FPaths::FileExists(filePath))
	{
		void *DLLHandle;
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath);
		if (DLLHandle != NULL)
		{
			/**
			the UE4OSC dll passes a pointer to an array of floating point numbers, which has been dynamically allocated
			that means it does not store the size of the array anywhere to be retrieved with a convenient .SizeOf(), for example
			but for the UE4 function GetOSCFloatArray to be available to Blueprint, it must return a TArray
			so, I have to get the size of the array first, then get the array, then convert it to a TArray to return
			*/
			int ArraySize;

			// get the size of the array
			_GetOSCArraySize DLLGetOSCArraySize = NULL;
			FString procName = "GetOSCArraySize";
			DLLGetOSCArraySize = (_GetOSCArraySize)FPlatformProcess::GetDllExport(DLLHandle, *procName);
			if (DLLGetOSCArraySize != NULL)
			{
				ArraySize = DLLGetOSCArraySize(); // call our dll function
			}
			
			if (ArraySize > 0)
			{ 
				// get the array itself
				_GetOSCFloatArray DLLGetOSCFloatArray = NULL;
				procName = "GetOSCFloatArray";
				DLLGetOSCFloatArray = (_GetOSCFloatArray)FPlatformProcess::GetDllExport(DLLHandle, *procName);
				if (DLLGetOSCFloatArray != NULL)
				{
					float * PointerArray = new float[ArraySize];
					PointerArray = DLLGetOSCFloatArray();
					
					// convert the dynamically allocated array to a TArray
					for (int i = 0; i < ArraySize; i++)
					{
						OutArray.Add(PointerArray[i]);
					}
					
					return OutArray;
				}
			}
		}
	}
	return OutArray; 
}

void AUE4OSCPluginOSCInterface::OSCSendFloat(float FloatMsg, FString AddressPattern)
{
	FString filePath = FPaths::Combine(*FPaths::GamePluginsDir(), TEXT("UE4OSCPlugin/Binaries/Win64"), TEXT("UE4OSC.dll"));
	if (FPaths::FileExists(filePath))
	{
		void *DLLHandle;
		DLLHandle = FPlatformProcess::GetDllHandle(*filePath);
		if (DLLHandle != NULL)
		{
			_SendOSCFloat DLLSendOSCFloat = NULL;
			FString procName = "SendOSCFloat";
			DLLSendOSCFloat = (_SendOSCFloat)FPlatformProcess::GetDllExport(DLLHandle, *procName);
			if (DLLSendOSCFloat != NULL)
			{
				AddressPattern = "/" + AddressPattern;
				char* AddressPatternAsChar = TCHAR_TO_ANSI(*AddressPattern);
				char* HostnameAsChar = TCHAR_TO_ANSI(*OSCHostname);
				int Port = 7400;
				UE_LOG(YourLog, Warning, TEXT("Sending float!"));
				DLLSendOSCFloat(AddressPatternAsChar, FloatMsg, HostnameAsChar, OSCSendPort);
			}
		}
	}
}