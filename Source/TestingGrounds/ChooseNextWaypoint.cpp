// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{


	// Get the controlled pawn to get his waypoints
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolComp = ControlledPawn->FindComponentByClass<UPatrolComponent>();

	if (!PatrolComp) { return EBTNodeResult::Failed; }
	auto PatrolPoints = PatrolComp->GetPatrolPoints();

	if (PatrolPoints.Num() == 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No patrol points on guard %s"), *ControlledPawn->GetName());
		return EBTNodeResult::Aborted; 
	}
	
	// set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);


	//Cycle index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
