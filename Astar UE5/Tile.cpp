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
}

void ATile::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if(PlayerController)
    {
        AstarPlayerController = Cast<AAstarPlayerController>(PlayerController);
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

void ATile::SetPathTile()
{
    UStaticMeshComponent* mesh = this->GetStaticMeshComponent();
    if(mesh)
    {
        mesh->SetMaterial(0, m_PathMaterial);
    }
}
