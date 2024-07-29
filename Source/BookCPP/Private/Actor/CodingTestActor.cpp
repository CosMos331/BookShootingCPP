// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CodingTestActor.h"

// Sets default values
ACodingTestActor::ACodingTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACodingTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	

}

// Called every frame
void ACodingTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ACodingTestActor::Add(int32 a, int32 b)
{
	int32 result = a + b;
	return result;
}

void ACodingTestActor::InputSpaceKey(bool d)
{
	bPressedKey = d;
}

void ACodingTestActor::MoveUp(float DeltaTime)
{
	if (bPressedKey)
	{
		FVector  location = GetActorLocation();
		location.Z += 1000 * DeltaTime;
		SetActorLocation(location);
	}
}

