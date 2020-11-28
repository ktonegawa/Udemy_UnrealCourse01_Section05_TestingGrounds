// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
//#include "PatrollingGuard.h" // TODO remove coupling
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    // Get patrol points
    auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
    auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
    if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

    // warn about empty patrol routes
    //auto PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
    auto PatrolPoints = PatrolRoute->GetPatrolPoints();
    if (PatrolPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("a gaurd is missing patrol points"));
        return EBTNodeResult::Failed;
    }

    // Set next waypoint
    //UE_LOG(LogTemp, Warning, TEXT("AI in C++"));
    auto BlackboardComp = OwnerComp.GetBlackboardComponent();
    auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
    BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
    
    // Cycle index
    auto NextIndex = Index + 1;
    //UE_LOG(LogTemp, Warning, TEXT("NextIndex: %i"), NextIndex);
    //UE_LOG(LogTemp, Warning, TEXT("PatrolPoints.Num(): %i"), PatrolPoints.Num());
    if (NextIndex >= PatrolPoints.Num())
    {
        //UE_LOG(LogTemp, Warning, TEXT("going to new waypoint"));
        BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, 0);
    }
    else
    {
        BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
    }
    //auto NextIndex = (Index + 1) % PatrolPoints.Num();
    //BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
    //UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
    
    return EBTNodeResult::Succeeded;
}
