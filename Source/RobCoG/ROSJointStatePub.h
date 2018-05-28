// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "RRobot.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "RobotForceSubscriberCallback.h"
#include "ROSJointStatePub.generated.h"

UCLASS()
class ROBCOG_API AROSJointStatePub : public AActor
{
    GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AROSJointStatePub();

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when actor destroyed or game ended
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
	FString RobotName;

	UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
	FString RobotJointStateTopic;

	UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
	FString RobotEffortTopic;

	UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
	FString WebsocketIPAddr;

	UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
	uint32 WebsocketPort;

	// Counter
    uint32 TickCount;

	// Pointer to robot
    ARRobot* Robot;

	// ROS Handler
    TSharedPtr<FROSBridgeHandler> Handler;

	// ROS Publisher
    TSharedPtr<FROSBridgePublisher> RobotJointStatePublisher;

	// ROS Subscriber
    TSharedPtr<FRobotForceSubscriberCallback> RobotForceSubscriber;
};
