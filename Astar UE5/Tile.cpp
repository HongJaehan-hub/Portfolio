// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AstarModeBase.h"
#include "Components/StaticMeshComponent.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoReceiveInput = EAutoReceiveInput::Player0;
    IsPointTile = false;
    IsWallTile = false;

    int32 row[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
    int32 col[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    
    for(int32 i = 0; i < 8; ++i)
    {
        FIntPoint Key(row[i], col[i]);
        RotationMap.Add(Key, i * 45.0f);
    }
}

void ATile::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if(PlayerController)
    {
        AstarPlayerController = Cast<AAstarPlayerController>(PlayerController);
    }

    TArray<UActorComponent*> Components;
    GetComponents<UActorComponent>(Components);

    for (UActorComponent* Component : Components)
    {
        if (Component && Component->GetName() == TEXT("ArrowScene"))
        {
            ArrowComponent = Cast<USceneComponent>(Component);
            break;
        }
    }

    if(ArrowComponent)
    {
        ArrowComponent->SetVisibility(false, true);
    }
}

void ATile::Reset()
{
    IsPointTile = false;
    IsWallTile = false;

    UStaticMeshComponent* mesh = this->GetStaticMeshComponent();
    if(mesh)
    {
        mesh->SetMaterial(0, m_DefaultMaterial);
    }

    
    if(ArrowComponent)
    {
        ArrowComponent->SetVisibility(false, true);
    }
}

void ATile::NotifyActorOnClicked(FKey ButtonPressed)
{    
    Super::NotifyActorOnClicked(ButtonPressed);

    if (ButtonPressed == EKeys::LeftMouseButton)
    {
        OnLeftClicked();
    }
    else if (ButtonPressed == EKeys::RightMouseButton)
    {
        OnRightClicked();
    }
}

void ATile::OnLeftClicked()
{
    // 마우스 왼쪽 버튼은 시작, 끝 지점 선택
    if(IsWallTile)
        return;

    UWorld* World = GetWorld();
    if(World)
    {
        AAstarModeBase* AstarGameMode = World->GetAuthGameMode<AAstarModeBase>();
        UStaticMeshComponent* mesh = this->GetStaticMeshComponent();

        if(mesh == nullptr)
            return;

        if(AstarGameMode)
        {
            // 이미 선택된 시작 or 끝 지점이라면 제거
            if(IsPointTile)
            {
                AstarGameMode->RemovePoint(FVector2D(Row, Col));
                mesh->SetMaterial(0, m_DefaultMaterial);
                IsPointTile = false;
            }
            else
            {
                // 시작 끝 지점을 추가 할 수 있는지 여부 확인
                if(AstarGameMode->CanAddPoint())
                {
                    IsPointTile = true;
                    AstarGameMode->AddPoint(FVector2D(Row, Col));
                    mesh->SetMaterial(0, m_PathMaterial);
                }
            }
        }
    }
}

void ATile::OnRightClicked()
{
    // 마우스 오른쪽 클릭은 벽 설정
    if(IsPointTile)
        return;

    UWorld* World = GetWorld();
    if(World)
    {
        AAstarModeBase* AstarGameMode = World->GetAuthGameMode<AAstarModeBase>();
        UStaticMeshComponent* mesh = this->GetStaticMeshComponent();

        if(mesh == nullptr)
            return;

        if(AstarGameMode)
        {
            // 이미 선택된 벽인 경우 제거
            if(IsWallTile)
            {
                IsWallTile = false;
                mesh->SetMaterial(0, m_DefaultMaterial);
            }
            else
            {
                IsWallTile = true;
                mesh->SetMaterial(0, m_WallMaterial);
            }
        }
    }
}

void ATile::NotifyActorBeginCursorOver()
{
    // 마우스 커서 오른쪽 클릭 상태에서 커서 오버 시 벽 설치
    if(IsPointTile)
        return;

    if(AstarPlayerController)
    {
        if(AstarPlayerController->IsRightMouseButtonDown())
        {
            UStaticMeshComponent* mesh = this->GetStaticMeshComponent();
            if(mesh)
            {
                mesh->SetMaterial(0, m_WallMaterial);
            }
            IsWallTile = true;
        }
    }
}

void ATile::SetPathTile(FVector2D Diff)
{
    UStaticMeshComponent* mesh = this->GetStaticMeshComponent();
    if(mesh)
    {
        mesh->SetMaterial(0, m_PathMaterial);
    }

    if(ArrowComponent)
    {
        ArrowComponent->SetVisibility(true, true);
        FRotator Rotator;

        const float* ValPtr = RotationMap.Find(FIntPoint((int32)Diff.X, (int32)Diff.Y));
        if(ValPtr)
        {
            ArrowComponent->SetRelativeRotation(FRotator(0.0f, *ValPtr, 0.0f));
        }
    }
}
