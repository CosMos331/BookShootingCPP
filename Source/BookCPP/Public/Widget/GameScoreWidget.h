// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOOKCPP_API UGameScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreData;

};
