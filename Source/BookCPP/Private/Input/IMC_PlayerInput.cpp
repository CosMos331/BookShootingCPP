// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/IMC_PlayerInput.h"
#include "InputAction.h"

UIMC_PlayerInput::UIMC_PlayerInput()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_HORIZONTAL(
		TEXT("/Script/BookCPP.IA_Horizontal'/Game/Input/IA_Horiaontal.IA_Horiaontal'"));

	//UInputAction* Horizontal;

	//if (IA_HORIZONTAL.Succeeded())
	//{
	//	Horizontal = IA_HORIZONTAL.Object;

	//	FInputActionKeyMapping HorizontalMapping;
	//	HorizontalMapping.Action = Horizontal;
	//	HorizontalMapping.Key = EKeys::A; // ���÷� A Ű�� ����߽��ϴ�.

	//	// UInputMappingContext�� ���� �߰�
	//	AddMapping(HorizontalMapping);
	//}


}
