// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class BOOKCPP_API ABullet : public AActor
{
	GENERATED_BODY()
private: 
	UPROPERTY(EditAnywhere, Category = "move")
	float BulletSpeed;

	// »ç¿ëÇÒ  Material
	UPROPERTY(VisibleDefaultsOnly)
	UMaterial* Material;

public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

private :

	void BulletMove(float dt);

	void BulletRemove();

};
