// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"


UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    //returns early if the AI controller is null
    if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

    //Gets the pawn that the AI controller is controlling
    AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

    //returns early if the pawn is null
    if(Character == nullptr) return EBTNodeResult::Failed;

    //calls the Shoot function on the pawn
    Character->Shoot();

    return EBTNodeResult::Succeeded;
}
