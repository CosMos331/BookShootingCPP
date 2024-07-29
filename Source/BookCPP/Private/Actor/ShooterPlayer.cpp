// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ShooterPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "Sound/SoundWave.h"

#include "Kismet/GameplayStatics.h"

#include "Actor/Bullet.h"


// Sets default values
AShooterPlayer::AShooterPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 박스 콜라이더 컴포넌트 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MY_BOX_COMP"));
	// boxComp를 RootComponent로 지정
	SetRootComponent(boxComp);

	// Arrow Component 생성 
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("FIRE_POS"));
	arrowComp->SetupAttachment(boxComp);

	arrowComp->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	arrowComp->SetWorldLocation(FVector(0.f, 0.f, 80.f));

	// 스테틱 메시 컴포넌트 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY_MESH_COMP"));
	// meshComp를 boxComp의 자식으로 지정
	meshComp->SetupAttachment(boxComp);

	// 스테팃 메시컴포넌트의 StaticMesh 지정, 경로에 인는 메시를 지정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(
		TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (SM_BOX.Succeeded())
	{
		meshComp->SetStaticMesh(SM_BOX.Object);
	}

	FVector boxSize = FVector(50.0f, 50.0f, 50.0f);
	boxComp->SetBoxExtent(boxSize);

	// 사용할 Input Mapping Context, Input Action 지정
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IA_MAPPING(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_PlayerInput.IMC_PlayerInput'"));
	if (IA_MAPPING.Succeeded())
	{
		imc_playerInput = IA_MAPPING.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_HORIZONTAL(
		TEXT("/Script/BookCPP.IA_Horizontal'/Game/Input/IA_Horiaontal.IA_Horiaontal'"));
	if (IA_HORIZONTAL.Succeeded())
	{
		ia_Horizontal = IA_HORIZONTAL.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_VERTICAL(
		TEXT("/Script/BookCPP.IA_Horizontal'/Game/Input/IA_Vertical.IA_Vertical'"));
	if (IA_VERTICAL.Succeeded())
	{
		ia_Vertical = IA_VERTICAL.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_FIRE(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Fire.IA_Fire'"));
	if (IA_FIRE.Succeeded())
	{
		ia_Fire = IA_FIRE.Object;
	}

	// TSubclassOf 초기화
	static ConstructorHelpers::FClassFinder<ABullet> BP_BULLET(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/BP_Bullet.BP_Bullet_C'"));
	if (BP_BULLET.Succeeded())
	{
		bulletFactory = BP_BULLET.Class;
	}

	// 사운드
	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND_ATT(
		TEXT("/Script/Engine.SoundWave'/Game/Audio/star-wars-blaster.star-wars-blaster'"));
	if (SOUND_ATT.Succeeded())
	{
		attackSound = SOUND_ATT.Object;
	}
	attackSound->Volume = 0.2f;
	

	// 액터 초기화
	Speed = 100;
	MoveDirection = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AShooterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 현재 월드의 첫번째 컨트롤러
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	// pc를 받아오는데 성공했다면
	if (pc != nullptr)
	{
		// 플레이어 컨트롤러로부터 입력 서브 시스템 정보를 가져온다.
		UEnhancedInputLocalPlayerSubsystem* subsys =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr)
		{
			// 입력 서브 시스템에 IMC 파일 변수를 연결한다.
			subsys->AddMappingContext(imc_playerInput, 0);
		}
	}


}

// Called every frame
void AShooterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);

}

// Called to bind functionality to input
void AShooterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent를 EnhancedInputComponent로 변경하여 EnhancedInput 방식을 사용
	UEnhancedInputComponent* enhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// InputAction에 원하는 키 입력 이벤트와, 이벤트 발생시 호출할 함수를 바인딩
	if(enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_Horizontal, ETriggerEvent::Triggered, this, &AShooterPlayer::OnHorizontal);
		enhancedInputComponent->BindAction(ia_Horizontal, ETriggerEvent::Completed, this, &AShooterPlayer::OnHorizontal);
		enhancedInputComponent->BindAction(ia_Vertical, ETriggerEvent::Triggered, this, &AShooterPlayer::OnVertical);
		enhancedInputComponent->BindAction(ia_Vertical, ETriggerEvent::Completed, this, &AShooterPlayer::OnVertical);
		enhancedInputComponent->BindAction(ia_Fire, ETriggerEvent::Started, this, &AShooterPlayer::OnFire);
	}


}

void AShooterPlayer::OnHorizontal(const FInputActionValue& value)
{
	float hor = value.Get<float>();
	MoveDirection.Y = hor;


}

void AShooterPlayer::OnVertical(const FInputActionValue& value)
{
	float hor = value.Get<float>();
	MoveDirection.Z = hor;

}

void AShooterPlayer::OnFire()
{
	GetWorld()->SpawnActor<ABullet>(
		bulletFactory,
		arrowComp->GetComponentLocation(),
		arrowComp->GetComponentRotation());
	UGameplayStatics::PlaySound2D(GetWorld(), attackSound);
}

void AShooterPlayer::Move(float dt)
{
	FVector currentLocation = GetActorLocation();
	MoveDirection.Normalize();
	currentLocation.Z += MoveDirection.Z * Speed * dt;
	currentLocation.Y += MoveDirection.Y * Speed * dt;
	SetActorLocation(currentLocation);
}



