// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ROSBridgeSrvClient.h"
#include "rospy_tutorials/AddTwoInts.h"

class FROSAddTwoIntsClient final : public FROSBridgeSrvClient
{
public:
    FROSAddTwoIntsClient(AActor* InOwner, const FString& InName, const FString& InType):
        FROSBridgeSrvClient(InName, InType), Owner(InOwner)
    {
    }

    void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override
    {
        TSharedPtr<rospy_tutorials::AddTwoInts::Response> Response =
            StaticCastSharedPtr<rospy_tutorials::AddTwoInts::Response>(InResponse);

        UE_LOG(LogTemp, Log, TEXT("[%s] In actor %s: Add Two Ints response: %d"),
			*FString(__FUNCTION__), *Owner->GetName(), Response->GetSum());
    }

private:
	AActor* Owner;
};
