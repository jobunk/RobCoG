// Copyright 2018, Institute for Artificial Intelligence - University of Bremen


#include "ROSJointStatePub.h"
#include "RobCoG.h"
#include "sensor_msgs/JointState.h"

// Sets default values
AROSJointStatePub::AROSJointStatePub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PrePhysics;

    WebsocketIPAddr = TEXT("127.0.0.1");
    WebsocketPort = 9001;
    RobotName = TEXT("pr2_base");
    RobotJointStateTopic = TEXT("unreal_joint_state");
    RobotEffortTopic = TEXT("unreal_force");

    TickCount = 0;
}

// Called when the game starts or when spawned
void AROSJointStatePub::BeginPlay()
{
	Super::BeginPlay();

    bool bActorFound = false;

    Robot = nullptr;

    for (TActorIterator<ARRobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        UE_LOG(LogTemp, Log, TEXT("Actor Label = [%s]"), *ActorItr->GetActorLabel());
        if (ActorItr->GetActorLabel() == RobotName)
        {
            Robot = *ActorItr;
            bActorFound = true;
            break;
        }
    }

    checkf(bActorFound, TEXT("Robot Actor with Name [%s] not found!"), *RobotName);

    Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));

    RobotJointStatePublisher = MakeShareable<FROSBridgePublisher>
            (new FROSBridgePublisher(RobotJointStateTopic, TEXT("sensor_msgs/JointState")));

    RobotForceSubscriber = MakeShareable<FRobotForceSubscriberCallback>
            (new FRobotForceSubscriberCallback(Robot, RobotEffortTopic));

    Handler->Connect();

    Handler->AddPublisher(RobotJointStatePublisher);

    Handler->AddSubscriber(RobotForceSubscriber);
}

// Called when actor destroyed or game ended
void AROSJointStatePub::EndPlay(const EEndPlayReason::Type Reason)
{
	// Disconnect the handler before parent ends
	Handler->Disconnect();

	Super::EndPlay(Reason);
}


// Called every frame
void AROSJointStatePub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Process messages
	Handler->Process();

    TArray<FString> ListJointName;
    TArray<double> ListJointPosition, ListJointVelocity;

    for (auto &JointElement : Robot->JointComponents)
    {
        FString JointName = JointElement.Value->GetName();
        float JointPosition = Robot->GetJointPosition(JointElement.Key);
        float JointVelocity = Robot->GetJointVelocity(JointElement.Key);

        ListJointName.Add(JointName);
        ListJointPosition.Add(JointPosition);
        ListJointVelocity.Add(JointVelocity);
    }
    
    TSharedPtr<sensor_msgs::JointState> JointState =
            MakeShareable(new sensor_msgs::JointState());
    JointState->SetHeader(std_msgs::Header(++TickCount, FROSTime(), TEXT("0")));
    JointState->SetName(ListJointName);
    JointState->SetPosition(ListJointPosition);
    JointState->SetVelocity(ListJointVelocity);

    Handler->PublishMsg(RobotJointStateTopic, JointState);

    UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
}

