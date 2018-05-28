// Copyright 2018, Institute for Artificial Intelligence - University of Bremen

#include "ROSAddTwoIntsServiceClient.h"
#include "ROSAddTwoIntsClient.h"

// Sets default values
AROSAddTwoIntsServiceClient::AROSAddTwoIntsServiceClient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Default IP localhost
	IPAddress = TEXT("127.0.0.1");

	// Set Port to 9090
	Port = 9090;

	// Set ROS topic name which publish strings
	Name = TEXT("add_two_ints");
}

// Called when the game starts or when spawned
void AROSAddTwoIntsServiceClient::BeginPlay()
{
	Super::BeginPlay();
	
	// Set websocket server address to default
	ROSHandler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(IPAddress, Port));

	// Connect to rosbridge
	ROSHandler->Connect();

	TSharedPtr<FROSAddTwoIntsClient> ServiceClient =
		MakeShareable<FROSAddTwoIntsClient>(new FROSAddTwoIntsClient(this, Name, TEXT("rospy_tutorials/AddTwoInts")));

	int NumA = FMath::RandRange(1, 10000);
	int NumB = FMath::RandRange(1, 10000);

	// Create a request instance with request parameters
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request = 
		MakeShareable(new rospy_tutorials::AddTwoInts::Request(NumA, NumB));
	
	// Create an empty response instance 
	TSharedPtr<FROSBridgeSrv::SrvResponse> Response =
		MakeShareable(new rospy_tutorials::AddTwoInts::Response());

	// Sleep a few ms to make sure the thread is spun
	FPlatformProcess::Sleep(0.01);

	ROSHandler->CallService(ServiceClient, Request, Response);
}

// Called when game ends or actor deleted
void AROSAddTwoIntsServiceClient::EndPlay(const EEndPlayReason::Type Reason)
{
	// Disconnect the handler before parent ends
	ROSHandler->Disconnect();
	Super::EndPlay(Reason);
}

// Called every frame
void AROSAddTwoIntsServiceClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

