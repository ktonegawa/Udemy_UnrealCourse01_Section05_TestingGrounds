// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
    GENERATED_USTRUCT_BODY()
    
    FVector SpawnPoint;
    float Rotation;
    float Scale;
};

class UActorPool;

UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

    UFUNCTION(BlueprintCallable, Category = "Character")
    void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius = 500, float MinScale = 1, float MaxScale = 1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // To delete all invalid props
	virtual void Destroyed() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, Category="Spawining")
    FVector MinExtent;
    UPROPERTY(EditDefaultsOnly, Category = "Spawining")
    FVector MaxExtent;

    UPROPERTY(EditDefaultsOnly, Category = "Navigation")
    FVector NavigationBoundsOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Pool")
    void SetPool(UActorPool* Pool);

private:
    void PositionNavMeshBoundsVolume();

    TArray<FSpawnPosition> RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale);

    bool FindEmptyLocation(FVector& OutLocation, float Radius);

    void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);
    
    TArray<AActor*> SpawnedActors;

    bool CanSpawnAtLocation(FVector Location, float Radius);
	
    UActorPool* Pool;

    AActor* NavMeshBoundsVolume;
};
