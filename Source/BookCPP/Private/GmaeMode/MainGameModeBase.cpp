// Fill out your copyright notice in the Description page of Project Settings.


#include "GmaeMode/MainGameModeBase.h"
#include "Actor/Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/ShooterPlayer.h"


#include "Widget/GameScoreWidget.h"
#include "Widget/MenuWidget.h"
#include "Components/TextBlock.h"

AMainGameModeBase::AMainGameModeBase()
{

	static ConstructorHelpers::FClassFinder<AActor> ACTOR_PLAYER(
		TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Actor/BP_ShooterPlayer.BP_ShooterPlayer_C'"));

	if (ACTOR_PLAYER.Succeeded())
	{
        
		DefaultPawnClass = ACTOR_PLAYER.Class;
	}

   
    static ConstructorHelpers::FClassFinder<UGameScoreWidget> SCORE_WIDGET(
        TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widget/BP_ScoreWidget.BP_ScoreWidget_C'"));
   
    if (SCORE_WIDGET.Succeeded())
    {
        ScoreWidget = SCORE_WIDGET.Class;
    }

    static ConstructorHelpers::FClassFinder<UMenuWidget> MENU_WIDGET(
        TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Widget/BP_MenuWidget.BP_MenuWidget_C'"));
    if (MENU_WIDGET.Succeeded())
    {
        MenuWidget = MENU_WIDGET.Class;
    }

}

void AMainGameModeBase::BeginPlay()
{
    // 첫 번째 플레이어 컨트롤러를 가져옵니다
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {


        // 현재 컨트롤 중인 폰을 가져옵니다
        APawn* PlayerPawn = PlayerController->GetPawn();
        if (PlayerPawn)
        {
            // AShooterPlayer로 캐스팅합니다
            AShooterPlayer* ShooterPlayer = Cast<AShooterPlayer>(PlayerPawn);
            if (ShooterPlayer)
            {
                UE_LOG(LogTemp, Warning, TEXT("Found ShooterPlayer"));
                // 델리게이트에 메서드를 바인딩합니다
                ShooterPlayer->OnShootings.AddUniqueDynamic(this, &ThisClass::OnShooting);

            }

            
        }


    }

    // TODO : 위젯 마무리

    if (ScoreWidget != nullptr)
    {
        // 위젯 블루프린트 파일을 메모리에 로드
        MainUI = CreateWidget<UGameScoreWidget>(GetWorld(), ScoreWidget);

        // 위젯 로드시 뷰포트에 출력
        if (MainUI != nullptr)
        {
            MainUI->ScoreText->SetText(FText::FromString("Score : "));
            MainUI->ScoreData->SetText(FText::AsNumber(Score));

            MainUI->AddToViewport();
        }
    }
}

void AMainGameModeBase::OnShooting()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting"));
}

void AMainGameModeBase::SetScoreText()
{
    MainUI->ScoreData->SetText(FText::AsNumber(Score));
}

void AMainGameModeBase::ShowWidget()
{
    if (MenuWidget != nullptr)
    {
        MenuUI = CreateWidget<UMenuWidget>(GetWorld(), MenuWidget);

        if (MenuUI != nullptr)
        {
            MenuUI->AddToViewport();

            // 게임 일시 정지
            UGameplayStatics::SetGamePaused(GetWorld(), true);
            // 마우스 표시
            GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
        }
    }

}


