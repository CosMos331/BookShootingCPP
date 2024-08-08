// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"

#include "Components/Button.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btRestart->OnClicked.AddDynamic(this, &UMenuWidget::Restart);
	btQuit->OnClicked.AddDynamic(this, &UMenuWidget::Quit);
}

void UMenuWidget::Restart()
{
	UGameplayStatics::OpenLevel(GetWorld(), "GameMap");
}

void UMenuWidget::Quit()
{
	UWorld* currentWorld = GetWorld();

	UKismetSystemLibrary::QuitGame(currentWorld, currentWorld->GetFirstPlayerController(), EQuitPreference::Quit, false);

}
