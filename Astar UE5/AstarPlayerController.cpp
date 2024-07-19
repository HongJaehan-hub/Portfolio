// Fill out your copyright notice in the Description page of Project Settings.


#include "AstarPlayerController.h"

AAstarPlayerController::AAstarPlayerController()
{
    bIsRightMouseButtonDown = false;
    UE_LOG(LogTemp, Warning, TEXT("AAstarPlayerController"));
}

void AAstarPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("RightClick", IE_Pressed, this, &AAstarPlayerController::OnRightMouseButtonDown);
    InputComponent->BindAction("RightClick", IE_Released, this, &AAstarPlayerController::OnRightMouseButtonUp);
}

void AAstarPlayerController::OnRightMouseButtonDown()
{
    bIsRightMouseButtonDown = true;
}

void AAstarPlayerController::OnRightMouseButtonUp()
{
    bIsRightMouseButtonDown = false;
}

bool AAstarPlayerController::IsRightMouseButtonDown() const
{
    return bIsRightMouseButtonDown;
}
