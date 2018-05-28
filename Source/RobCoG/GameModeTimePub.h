// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "GameModeTimePub.generated.h"

UCLASS()
class ROBCOG_API AGameModeTimePub : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGameModeTimePub();

    void BeginPlay() override;

    void Tick(float DeltaSeconds) override;

    void Logout(AController *Exiting) override;
	
	UPROPERTY()
	FString ROSBridgeServerIPAddr;

	UPROPERTY()
	uint32 ROSBridgeServerPort;

private:
	TSharedPtr<FROSBridgeHandler> Handler;

	TSharedPtr<FROSBridgePublisher> TimePublisher;	
};
