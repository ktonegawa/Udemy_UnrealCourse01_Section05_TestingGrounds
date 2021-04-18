// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "ActorPool.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

#define ECC_Spawn ECC_GameTraceChannel2

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    NavigationBoundsOffset = FVector(2000, 0, 0);

    MinExtent = GetActorTransform().TransformPosition(FVector(200, -2000, 0));
    MaxExtent = GetActorTransform().TransformPosition(FVector(4000, 2000, 0));

}

void ATile::SetPool(UActorPool* InPool)
{
    UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));
    Pool = InPool;

    PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
    NavMeshBoundsVolume = Pool->Checkout();
    if (NavMeshBoundsVolume == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] Not enough actors in pool."), *GetName());
        return;
    }
    UE_LOG(LogTemp, Error, TEXT("[%s] checked out: {%s} at %s"), *GetName(), *NavMeshBoundsVolume->GetName(), *GetActorLocation().ToString());
    NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
    GetWorld()->GetNavigationSystem()->Build();
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
    int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
    for (size_t i = 0; i < NumberToSpawn; i++)
    {
        //FVector SpawnPoint;
        FSpawnPosition SpawnPosition;
        SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);

        bool found = FindEmptyLocation(SpawnPosition.SpawnPoint, Radius * SpawnPosition.Scale);
        if (found) {
            SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
            PlaceActor(ToSpawn, SpawnPosition);
        }
    }
}

//void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
    //// ############################## start ################################## //
    ////FVector Min(0, -2000, 0);
    ////FVector Max(4000, 2000, 0);
    //// ################################ end ################################ //
    //FVector Min = GetActorTransform().TransformPosition(FVector(0, -2000, 0));
    //FVector Max = GetActorTransform().TransformPosition(FVector(4000, 2000, 0));
    //FBox Bounds(Min, Max);
    //int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
    //for (size_t i = 0; i < NumberToSpawn; i++)
    //{
    //    FVector SpawnPoint = FMath::RandPointInBox(Bounds);
    //    UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s"), *SpawnPoint.ToCompactString());
    //    AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
    //    Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
    //    // ############################## start ################################## //
    //    //FVector StartTrace = Spawned->GetActorLocation();
    //    //GetWorld()->LineTraceSingleByChannel(*hitResult, SpawnPoint, EndTrace, ECC_Visibility);
    //    //Spawned->SetActorRelativeLocation(SpawnPoint);
    //    //Trying to automatically detect the ground via hitResult
    //    //FTransform Pos1 = GetActorTransform();
    //    //FVector Pos2 = GetActorTransform().TransformPosition(Min);
    //    //UE_LOG(LogTemp, Warning, TEXT("Pos1: %s"), *Pos1.ToString());
    //    //UE_LOG(LogTemp, Warning, TEXT("Pos2: %s"), *Pos2.ToString());
    //    // ################################ end ################################ //
    //    FVector StartTrace = Spawned->GetActorLocation();
    //    //FVector EndTrace = StartTrace - FVector(0, 0, 200);
    //    FVector EndTrace = SpawnPoint - FVector(0, 0, 200);
    //    FHitResult hitResult;
    //    // ############################## start ################################## //
    //    //FHitResult* hitResult2 = new FHitResult();
    //    //DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green, true, -1, 0, 10);
    //    //DrawDebugLine(GetWorld(), SpawnPoint, SpawnPoint-FVector(0, 0, 200), FColor::Green, true, -1, 0, 10);
    //    //FCollisionQueryParams CollisionParameters;
    //    ////TArray<AActor*> ignoredActors;
    //    ////ignoredActors.Add(Spawned);
    //    ////CollisionParameters.AddIgnoredActors(ignoredActors);
    //    //Spawned->ActorLineTraceSingle(hitResult, SpawnPoint, SpawnPoint - FVector(0, 0, 200), ECC_Visibility, CollisionParameters);
    //    // ################################ end ################################ //
    //    GetWorld()->LineTraceSingleByChannel(hitResult, SpawnPoint, EndTrace, ECC_Visibility);
    //    UE_LOG(LogTemp, Warning, TEXT("HitPoint: %s"), *hitResult.ToString());
    //    // ############################## start ################################## //
    //    //UE_LOG(LogTemp, Warning, TEXT("HitPoint: %s"), *hitResult2->ToString());
    //    //Spawned->SetActorRelativeLocation(hitResult.Location);
    //    //Spawned->SetActorRelativeLocation(hitResult2->Location);
    //    //Spawned->SetActorLocation(hitResult2->Location);
    //    // ################################ end ################################ //
    //    Spawned->SetActorLocation(hitResult.Location);
    //    SpawnedActors.Add(Spawned);
    //    // ############################## start ################################## //
    //    //Spawned->AddActorLocalOffset(FVector(0, 0, -120));
    //    // ################################ end ################################ //
    //}

    //TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
    //for (FSpawnPosition SpawnPosition : SpawnPositions) 
    //{
    //    PlaceActor(ToSpawn, SpawnPosition);
    //}

    RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
    //TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, 1, 1);
    //for (FSpawnPosition SpawnPosition : SpawnPositions)
    //{
    //    PlaceAIPawn(ToSpawn, SpawnPosition);
    //}
    RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}

//TArray<FSpawnPosition> ATile::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
//{
//    TArray<FSpawnPosition> SpawnPositions;
//    int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
//    for (size_t i = 0; i < NumberToSpawn; i++)
//    {
//        //FVector SpawnPoint;
//        FSpawnPosition SpawnPosition;
//        SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
//
//        bool found = FindEmptyLocation(SpawnPosition.SpawnPoint, Radius * SpawnPosition.Scale);
//        if (found) {
//            SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
//            SpawnPositions.Add(SpawnPosition);
//        }
//    }
//
//    return SpawnPositions;
//}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
    //FVector Min = GetActorTransform().TransformPosition(MinExtent);
    FVector Min = GetActorLocation() + MinExtent;
    //FVector Max = GetActorTransform().TransformPosition(MaxExtent);
    FVector Max = GetActorLocation() + MaxExtent;
    //UE_LOG(LogTemp, Warning, TEXT("Min: %s Max: %s"), *Min.ToString(), *Max.ToString());
    //UE_LOG(LogTemp, Warning, TEXT("GetActorLocation: %s GetActorTransform: %s"), *GetActorLocation().ToString(), *GetActorTransform().ToString());
    FBox Bounds(Min, Max);
    const int MAX_ATTEMPTS = 100;
    for (size_t i = 0; i < MAX_ATTEMPTS; i++)
    {
        FVector CandidatePoint = FMath::RandPointInBox(Bounds);
        if (CanSpawnAtLocation(CandidatePoint, Radius)) {
            OutLocation = CandidatePoint;
            return true;
        }
    }
    return false;
}


void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
    AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
    if (Spawned) {
        Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
        Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
        Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
        FVector StartTrace = Spawned->GetActorLocation();
        FVector EndTrace = SpawnPosition.SpawnPoint - FVector(0, 0, 200);
        FHitResult hitResult;
        GetWorld()->LineTraceSingleByChannel(hitResult, SpawnPosition.SpawnPoint, EndTrace, ECC_Visibility);
        //UE_LOG(LogTemp, Warning, TEXT("HitPoint: %s"), *hitResult.ToString());
        Spawned->SetActorLocation(hitResult.Location);
        SpawnedActors.Add(Spawned);
    }
}

void ATile::PlaceActor(TSubclassOf<APawn> &ToSpawn, FSpawnPosition SpawnPosition)
{
    APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
    if (Spawned) {
        Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
        Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
        Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
        FVector StartTrace = Spawned->GetActorLocation();
        FVector EndTrace = SpawnPosition.SpawnPoint - FVector(0, 0, 200);
        FHitResult hitResult;
        GetWorld()->LineTraceSingleByChannel(hitResult, SpawnPosition.SpawnPoint, EndTrace, ECC_Visibility);
        //UE_LOG(LogTemp, Warning, TEXT("HitPoint: %s"), *hitResult.ToString());
        Spawned->SetActorLocation(hitResult.Location);
        Spawned->SpawnDefaultController();
        Spawned->Tags.Add(FName("Enemy"));
        SpawnedActors.Add(Spawned);
    }
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
    //CastSphere(GetActorLocation(), 300);
    //CastSphere(GetActorLocation() + FVector(0, 0, 1000), 300);
    //CanSpawnAtLocation(GetActorLocation() + FVector(4000, 0, 0), 300);

}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    UE_LOG(LogTemp, Warning, TEXT("[%s]: EndPlay"), *GetName());
    Pool->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
    FHitResult HitResult;
    bool HasHit = GetWorld()->SweepSingleByChannel(
                            HitResult,
                            Location,
                            Location,
                            FQuat::Identity,
                            ECollisionChannel::ECC_Spawn,
                            FCollisionShape::MakeSphere(Radius));
    FColor ResultColor = HasHit ? FColor::Red : FColor::Green; // this reads "if HasHit then Red, else Green"
    //DrawDebugSphere(GetWorld(), Location, Radius, 26, ResultColor, true, -1, 0, 2);
    return !HasHit;
}

void ATile::Destroyed()
{
    Super::Destroyed();
    if (SpawnedActors.Num() > 0)
    {
        for (AActor* Actor : SpawnedActors)
        {
            Actor->Destroy();
        }
    }
    
}