// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/ShooterPlayer.h"


// 생성 위치
// Y 1000 -1000
// Z 600

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH_COMP"));
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX_COMP"));
	
	// 컴포넌트 설정
	BoxComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	// 컴포넌트 계층 지정
	SetRootComponent(BoxComp);
	MeshComp->SetupAttachment(BoxComp);

	// 메시 컴포넌트에 스태택 매시 지정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MESH(
		TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (MESH.Succeeded())
	{
		MeshComp->SetStaticMesh(MESH.Object);
	}

	// 멤버 초기화
	Speed = 800.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	float rrr = FMath::RandRange(1, 100);

	if (rrr < 50)
	{
		Direction.Z = -1;
	}
	else
	{
		AShooterPlayer* p = Cast<AShooterPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		if (p != nullptr) 
		{
			Direction = p->GetActorLocation() - GetActorLocation();
			Direction.Normalize();
			
		}

	}

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);

	EnemyRemove();
}

void AEnemy::Move(float dt)
{
	FVector CurrentLocation = GetActorLocation();

	CurrentLocation += Direction * Speed * dt;

	SetActorLocation(CurrentLocation);

}

void AEnemy::EnemyRemove()
{
	FVector CurrentLocation = GetActorLocation();
	if (CurrentLocation.Z <= -650)
	{
		Destroy();
	}
}

