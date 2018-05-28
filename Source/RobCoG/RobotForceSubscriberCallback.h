// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RRobot.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "sensor_msgs/JointState.h"

class FRobotForceSubscriberCallback : public FROSBridgeSubscriber 
{
public:
    FRobotForceSubscriberCallback(ARRobot* InRobot, const FString& InTopic) :
        FROSBridgeSubscriber(InTopic, TEXT("sensor_msgs/JointState"))
    {
        Robot = InRobot;
    }

    ~FRobotForceSubscriberCallback() override {};

    TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override
    {
        TSharedPtr<sensor_msgs::JointState> JointStateMessage =
            MakeShareable<sensor_msgs::JointState>(new sensor_msgs::JointState());
        JointStateMessage->FromJson(JsonObject);

        return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
    }

    void Callback(TSharedPtr<FROSBridgeMsg> msg) override
    {
        TSharedPtr<sensor_msgs::JointState> JointStateMessage = StaticCastSharedPtr<sensor_msgs::JointState>(msg);

        TArray<FString> ListJointName = JointStateMessage->GetName();
        TArray<double> ListJointForce = JointStateMessage->GetEffort();
        UE_LOG(LogTemp, Warning, TEXT("ListJointName.Num() = %d"), ListJointName.Num());

        checkf(ListJointName.Num() == ListJointForce.Num(), TEXT("Error: Length of JointName and JointForce aren't equal."));

        for (int i = 0; i < ListJointName.Num(); i++)
        {
            FString JointName = ListJointName[i];
            double JointEffort = ListJointForce[i];
            Robot->AddForceToJoint(JointName, JointEffort * 10000);
        }
        return;
    }

private:
	ARRobot * Robot;
};
