// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CodingTestActor.generated.h"

UCLASS()
class BOOKCPP_API ACodingTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACodingTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	UFUNCTION(BlueprintCallable)
	int32 Add(int32 a, int32 b);

public :
	bool bPressedKey;

	UFUNCTION(BlueprintCallable, Category = "Move")
	void InputSpaceKey(bool d);
	UFUNCTION(BlueprintCallable, Category = "Move")
	void MoveUp(float DeltaTime);
};
