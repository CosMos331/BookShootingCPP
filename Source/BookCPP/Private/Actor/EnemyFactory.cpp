// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EnemyFactory.h"
#include "Actor/Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyFactory::AEnemyFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	static ConstructorHelpers::FClassFinder<AEnemy> BP_ENEMY(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/BP_Enemy.BP_Enemy_C'"));
	if (BP_ENEMY.Succeeded())
	{
		Enemy = BP_ENEMY.Class;
	} 
	

}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SumDelay += DeltaTime;
	
	if (SumDelay >= SpawnDelay)
	{
		
		SumDelay = 0.f;
		EnemySpawn();
	}

}

void AEnemyFactory::EnemySpawn()
{
	
	
	UE_LOG(LogTemp, Warning, TEXT("asdasd"));

	GetWorld()->SpawnActor<AEnemy>(Enemy, GetActorLocation(), GetActorRotation());
}

