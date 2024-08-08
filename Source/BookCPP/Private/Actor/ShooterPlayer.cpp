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
#include "GmaeMode/MainGameModeBase.h"

#include "Actor/Bullet.h"
#include "Actor/Enemy.h"


// Sets default values
AShooterPlayer::AShooterPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// �ڽ� �ݶ��̴� ������Ʈ ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("MY_BOX_COMP"));
	// boxComp�� RootComponent�� ����
	SetRootComponent(boxComp);

	// Arrow Component ���� 
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("FIRE_POS"));
	arrowComp->SetupAttachment(boxComp);

	arrowComp->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	arrowComp->SetWorldLocation(FVector(0.f, 0.f, 80.f));

	// ����ƽ �޽� ������Ʈ ����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY_MESH_COMP"));
	// meshComp�� boxComp�� �ڽ����� ����
	meshComp->SetupAttachment(boxComp);
	meshComp->SetWorldScale3D(FVector(5, 5, 5));
	meshComp->SetWorldRotation(FRotator(0, 90, 90));


	// ������ �޽�������Ʈ�� StaticMesh ����, ��ο� �ִ� �޽ø� ����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(
		TEXT("/Script/Engine.StaticMesh'/Game/Model2/Spaceship_ARA.Spaceship_ARA'"));
	if (SM_BOX.Succeeded())
	{
		meshComp->SetStaticMesh(SM_BOX.Object);
	}

	FVector boxSize = FVector(50.0f, 50.0f, 50.0f);
	boxComp->SetBoxExtent(boxSize);

	// ����� Input Mapping Context, Input Action ����
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

	// TSubclassOf �ʱ�ȭ
	static ConstructorHelpers::FClassFinder<ABullet> BP_BULLET(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/BP_Bullet.BP_Bullet_C'"));
	if (BP_BULLET.Succeeded())
	{
		bulletFactory = BP_BULLET.Class;
	}

	// ����
	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND_ATT(
		TEXT("/Script/Engine.SoundWave'/Game/Audio/star-wars-blaster.star-wars-blaster'"));
	if (SOUND_ATT.Succeeded())
	{
		attackSound = SOUND_ATT.Object;
	}
	attackSound->Volume = 0.2f;

	static ConstructorHelpers::FObjectFinder<USoundWave> SOUND_DEATH(
		TEXT("/Script/Engine.SoundWave'/Game/Audio/Remove.Remove'"));
	if (SOUND_DEATH.Succeeded())
	{
		DeathSound = SOUND_DEATH.Object;
	}
	DeathSound->Volume = 0.5f;

	// ���� �ʱ�ȭ
	Speed = 100;
	MoveDirection = FVector::ZeroVector;

	// Collision
	boxComp->SetCollisionProfileName(TEXT("Player"));
	
	//// ������ �̺�Ʈ�� �Ҵ�.
	//boxComp->SetGenerateOverlapEvents(true);
	//// �浹 ������ Query And Physics�� �����Ѵ�.
	//boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//// Object Type�� 1�� ä��(Player)�� ����
	//boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	//boxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//// ������ ä���� Collision���� �浹 ó�� ����
	//boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);



}

// Called when the game starts or when spawned
void AShooterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// ���� ������ ù��° ��Ʈ�ѷ�
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	// pc�� �޾ƿ��µ� �����ߴٸ�
	if (pc != nullptr)
	{
		// �÷��̾� ��Ʈ�ѷ��κ��� �Է� ���� �ý��� ������ �����´�.
		UEnhancedInputLocalPlayerSubsystem* subsys =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr)
		{
			// �Է� ���� �ý��ۿ� IMC ���� ������ �����Ѵ�.
			subsys->AddMappingContext(imc_playerInput, 0);
		}
	}


	boxComp->OnComponentBeginOverlap.AddUniqueDynamic(
		this, &ThisClass::OnPalyerBeginOverlap);

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

	// PlayerInputComponent�� EnhancedInputComponent�� �����Ͽ� EnhancedInput ����� ���
	UEnhancedInputComponent* enhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// InputAction�� ���ϴ� Ű �Է� �̺�Ʈ��, �̺�Ʈ �߻��� ȣ���� �Լ��� ���ε�
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
	// �̺�Ʈ
	OnShootings.Broadcast();


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

void AShooterPlayer::OnPalyerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* e = Cast<AEnemy>(OtherActor);

	AMainGameModeBase* gameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	int32 hp = gameMode->GetHp();

	

	if (e != nullptr)
	{
		e->Destroy();
		hp--;
		if (hp <= 0)
		{

			UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);
			gameMode->ShowWidget();
			Destroy();
		}
	}
	gameMode->SetHp(hp);



}



