// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "S05_TestingGroundsGameMode.h"
#include "InfiniteTerrainGameModeCpp.generated.h"

/**
 * 
 */
UCLASS()
class S05_TESTINGGROUNDS_API AInfiniteTerrainGameModeCpp : public AS05_TestingGroundsGameMode
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
    void PopulateBoundsVolumePool();
	
private:
    void AddToPool(class ANavMeshBoundsVolume *VolumeToAdd);
	
};
