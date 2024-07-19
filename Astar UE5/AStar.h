// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CRYPTRAIDER_API AStar
{
public:
	AStar();
	~AStar();

	static TArray<FVector2D> FindPath(TArray<TArray<int>> map, FVector2D Start, FVector2D End);
};
