// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"

#include "Actor/Enemy.h"

#include "Kismet/GameplayStatics.h"
#include "GmaeMode/MainGameModeBase.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컴포넌트 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX_COMP"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMESH_COMP"));

	// 컴포넌트간 계층 지정
	SetRootComponent(boxComp);
	meshComp->SetupAttachment(boxComp);

	// meshComp에 원하는 Static Mesh 지정
	ConstructorHelpers::FObjectFinder<UStaticMesh> SBOX_MESH(
		TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (SBOX_MESH.Succeeded())
	{
		meshComp->SetStaticMesh(SBOX_MESH.Object);
	}

	// boxComp의 크기 지정
	boxComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	// boxComp의 크기 배율 지정
	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));


	// 기본적인 요소 초기화
	BulletSpeed = 800.f;
	
	// 총알 액터의 Material 생성 및 적용
	// 1)

	static ConstructorHelpers::FObjectFinder<UMaterial> MT_BULLET(
		TEXT("/Script/Engine.Material'/Game/Material/MT_Bullet.MT_Bullet'"));
	if (MT_BULLET.Succeeded())
	{
		Material = MT_BULLET.Object;
	}
	meshComp->SetMaterial(0, Material);

	// collision setting

	boxComp->SetCollisionProfileName(TEXT("Bullet"));

	/*boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel3);

	boxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);*/

	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddUniqueDynamic(
		this, &ThisClass::OnBulletOverlap);	

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BulletMove(DeltaTime);

	BulletRemove();
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AEnemy* e = Cast<AEnemy>(OtherActor);

	// 충돌한 Actor가 Enemy이면 삭제
	if (e != nullptr)
	{
		
		AMainGameModeBase* gameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		int32 Score = gameMode->GetScore();

		Score++;
		gameMode->SetScore(Score);


		gameMode->SetScoreText();

		OtherActor->Destroy();
	}

	// 총알을 자체 충돌시 삭제
	Destroy();
}

void ABullet::BulletMove(float dt)
{
	FVector currentLocation = GetActorLocation();

	currentLocation.Z += BulletSpeed * dt;

	SetActorLocation(currentLocation);

}

void ABullet::BulletRemove()
{
	FVector currentLocation = GetActorLocation();

	if (currentLocation.Z >= 650)
	{
		Destroy();
	}
}

