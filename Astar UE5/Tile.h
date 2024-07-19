// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "AstarPlayerController.h"
#include "Tile.generated.h"

/**
 * 
 */
UCLASS()
class CRYPTRAIDER_API ATile : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	ATile();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void Reset();

	UFUNCTION()
	void OnMouseClicked();

	UFUNCTION()
	void OnLeftClicked();

	UFUNCTION()
	void OnRightClicked();

	virtual void NotifyActorOnClicked(FKey PressedButton) override;
	virtual void NotifyActorBeginCursorOver() override;

	UFUNCTION()
	void SetPathTile();
	
	UPROPERTY(EditAnywhere)
	int32 Row;

	UPROPERTY(EditAnywhere)
	int32 Col;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Materials")
	UMaterialInterface* m_DefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Materials")
	UMaterialInterface* m_WallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Materials")
	UMaterialInterface* m_PathMaterial;

	UPROPERTY(EditAnywhere)
	bool IsPointTile;

	UPROPERTY(EditAnywhere)
	bool IsWallTile;

private:
	AAstarPlayerController* AstarPlayerController;
};
