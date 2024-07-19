// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/StaticMeshActor.h"
#include "Tile.h"
#include "AStar.h"
#include "AstarModeBase.generated.h"

UCLASS()
class CRYPTRAIDER_API AAstarModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AAstarModeBase();

	virtual void BeginPlay() override;

	UFUNCTION()
	void AddPoint(FVector2D Point);

	UFUNCTION()
	void RemovePoint(FVector2D Point);

	UFUNCTION(BlueprintCallable)
	void StartAstarAlgorithm();

	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION()
	bool CanAddPoint();

	UPROPERTY(EditAnywhere)
	int32 TileSize;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ATile> TileClass;

	UPROPERTY(EditAnywhere)
	TArray<FVector2D> ArrayPoints;

	TArray<TArray<ATile*>> TileArray;
	TArray<TArray<int32>> Map;
};
