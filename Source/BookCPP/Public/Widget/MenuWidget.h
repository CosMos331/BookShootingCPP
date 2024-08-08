// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOOKCPP_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	virtual void NativeConstruct() override;

public :

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btRestart;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* btQuit;

public :
	UFUNCTION()
	void Restart();

	UFUNCTION()
	void Quit();
};
