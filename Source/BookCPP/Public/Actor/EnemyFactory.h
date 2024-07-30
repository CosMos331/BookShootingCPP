// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFactory.generated.h"

UCLASS()
class BOOKCPP_API AEnemyFactory : public AActor
{
	GENERATED_BODY()
	
private :
	UPROPERTY(EditAnywhere)
	float SpawnDelay;

	float SumDelay = 0.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> Enemy;

public:	
	// Sets default values for this actor's properties
	AEnemyFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private :
	void EnemySpawn();
};
