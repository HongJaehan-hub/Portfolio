// Fill out your copyright notice in the Description page of Project Settings.

#include "AstarModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

AAstarModeBase::AAstarModeBase()
{
    static ConstructorHelpers::FClassFinder<ATile> TileBPClass(TEXT("/Game/Astar/BP_Tile"));
    if(TileBPClass.Class != nullptr)
    {
        TileClass = TileBPClass.Class;
    }
}

void AAstarModeBase::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if(PlayerController != nullptr)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }

    if(TileClass != nullptr)
    {
        TileArray.SetNum(TileSize);
        for(int32 i = 0; i < TileSize; ++i)
        {
            TileArray[i].SetNum(TileSize);
        }

        UWorld* world = GetWorld();
        if(world)
        {
            for(int i = 0; i < TileSize; ++i)
            {
                for(int j = 0; j < TileSize; ++j)
                {
                    float x = i * 120;
                    float y = j * 120;
                    FVector Location = FVector(x, y, 0);
                    FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
                    ATile* spawnTile = world->SpawnActor<ATile>(TileClass, Location, Rotation);
                    spawnTile->Row = i;
                    spawnTile->Col = j;
                    TileArray[i][j] = spawnTile;
                }
            }
        }
    }

    Map.SetNum(TileSize);
    for(int i = 0; i < TileSize; ++i)
    {
        Map[i].SetNum(TileSize);
        for(int32 j = 0; j < TileSize; ++j)
        {
            Map[i][j] = 0;
        }
    }
}

bool AAstarModeBase::CanAddPoint()
{
    return ArrayPoints.Num() < 2;
} 

void AAstarModeBase::AddPoint(FVector2D Point)
{
    ArrayPoints.Push(Point);
}

void AAstarModeBase::RemovePoint(FVector2D Point)
{
    int32 index;
    ArrayPoints.Find(Point, index);
    ArrayPoints.RemoveAt(index);
}

void AAstarModeBase::Reset()
{
    for(int i = 0; i < TileSize; ++i)
    {
        for(int j = 0; j < TileSize; ++j)
        {
            TileArray[i][j]->Reset();
            Map[i][j] = 0;
        }
    }
    ArrayPoints.Empty();
}

void AAstarModeBase::StartAstarAlgorithm()
{
    if(ArrayPoints.Num() < 2)
        return;

    // Tile Array를 순회하면서 Wall을 찾아서 Map에 설정
    for(int i = 0; i < TileSize; ++i)
    {
        for(int j = 0; j < TileSize; ++j)
        {
            Map[i][j] = TileArray[i][j]->IsWallTile ? 1 : 0;
        }
    }

    TArray<FVector2D> Path = AStar::FindPath(Map, ArrayPoints[0], ArrayPoints[1]);
    Path.Push(ArrayPoints[0]);

    for(int i = 0; i < Path.Num(); ++i)
    {
        int32 row = (int32)Path[i].X;
        int32 col = (int32)Path[i].Y;
        
        FVector2D Diff;
        if(i >= 1)
        {
            Diff = FVector2D((int32)Path[i - 1].X - row, (int32)Path[i - 1].Y - col);
        }
        TileArray[row][col]->SetPathTile(Diff);
    }
}
