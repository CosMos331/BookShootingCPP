// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BOOKCPP_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public :
	AMainGameModeBase();

	virtual void BeginPlay() override;


public :
	UFUNCTION()
	void OnShooting();

	void SetScore(int32 s) { Score = s; }
	int32 GetScore() { return Score; }

	void SetHp(int32 hp) { Hp = hp; }
	int32 GetHp() { return Hp; }

	void SetScoreText();

	void ShowWidget();

public :
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameScoreWidget> ScoreWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMenuWidget> MenuWidget;



private  :
	int32 Score = 0;
	int32 Hp = 3;

	class UGameScoreWidget* MainUI;
	class UMenuWidget* MenuUI;

};
