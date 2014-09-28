// UE4OSC created by Zach Duer
// see readme for copyright, permissions and credits

#pragma once

#include "UE4OSCPluginOSCInterface.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(YourLog, Log, All);
/**
*
*/
UCLASS()
class AUE4OSCPluginOSCInterface : public AActor
{
	GENERATED_UCLASS_BODY()

	FString OSCHostname;
	int32 OSCReceivePort;
	int32 OSCSendPort;

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);


	UFUNCTION(BlueprintCallable, Category = "UE4OSC")
	void OSCSetHostnameAndPort(FString Hostname, int32 ReceivePort, int32 SendPort);

	UFUNCTION(BlueprintCallable, Category = "UE4OSC")
	void OSCStartReceiver();

	//UFUNCTION(BlueprintCallable, Category = "UE4OSC")
	//void OSCStopReceiver();

	UFUNCTION(BlueprintCallable, Category = "UE4OSC")
	float OSCGetFloat();

	UFUNCTION(BlueprintCallable, Category = "UE4OSC")
	TArray<float> OSCGetFloatArray();

	UFUNCTION(BlueprintCallable, Category = "UE4OSC")
	void OSCSendFloat(float FloatMsg, FString AddressPattern);


};