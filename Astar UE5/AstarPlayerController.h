// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AstarPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CRYPTRAIDER_API AAstarPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AAstarPlayerController();
    bool IsRightMouseButtonDown() const;

protected:
    virtual void SetupInputComponent() override;

private:
    void OnRightMouseButtonDown();
    void OnRightMouseButtonUp();
    bool bIsRightMouseButtonDown;
};
