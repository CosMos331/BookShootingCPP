// Fill out your copyright notice in the Description page of Project Settings.


#include "GmaeMode/MainGameModeBase.h"
#include "Actor/Bullet.h"

AMainGameModeBase::AMainGameModeBase()
{

	static ConstructorHelpers::FClassFinder<AActor> ACTOR_PLAYER(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/BP_ShooterPlayer.BP_ShooterPlayer_C'"));

	if (ACTOR_PLAYER.Succeeded())
	{
		DefaultPawnClass = ACTOR_PLAYER.Class;
	}

}

void AMainGameModeBase::FireBullet(FVector playerLocation)
{
	/*ABullet* bullet;
	bullet->SetActorLocation(playerLocation);*/

}
