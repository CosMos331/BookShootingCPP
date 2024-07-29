// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ������Ʈ ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX_COMP"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMESH_COMP"));

	// ������Ʈ�� ���� ����
	SetRootComponent(boxComp);
	meshComp->SetupAttachment(boxComp);

	// meshComp�� ���ϴ� Static Mesh ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> SBOX_MESH(
		TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (SBOX_MESH.Succeeded())
	{
		meshComp->SetStaticMesh(SBOX_MESH.Object);
	}

	// boxComp�� ũ�� ����
	boxComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	// boxComp�� ũ�� ���� ����
	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));


	// �⺻���� ��� �ʱ�ȭ
	BulletSpeed = 800.f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BulletMove(DeltaTime);
}

void ABullet::BulletMove(float dt)
{
	FVector currentLocation = GetActorLocation();

	currentLocation.Z += BulletSpeed * dt;

	SetActorLocation(currentLocation);

}

