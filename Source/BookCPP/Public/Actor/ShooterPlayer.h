// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "ShooterPlayer.generated.h"

UCLASS()
class BOOKCPP_API AShooterPlayer : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "MoveValue")
	float Speed;

	FVector MoveDirection;
public :
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "Fire")
	class UArrowComponent* arrowComp;

	// BP_Bullet을 할당하기 위함
	UPROPERTY(EditAnywhere, Category = "Fire")
	TSubclassOf<class ABullet> bulletFactory;

	// 인풋 매핑
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* imc_playerInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ia_Horizontal;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ia_Vertical;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ia_Fire;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* attackSound;

public:
	// Sets default values for this pawn's properties
	AShooterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private :
	void OnHorizontal(const struct FInputActionValue& value);
	void OnVertical(const struct FInputActionValue& value);
	void OnFire();


	void Move(float dt);
	
};
