// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    
    if(AIBehavior && PlayerPawn)
    {
        RunBehaviorTree(AIBehavior);
        
        Blackboard = GetBlackboardComponent();
        Blackboard->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if(PlayerPawn == nullptr || Blackboard == nullptr) return;

    if(LineOfSightTo(PlayerPawn))
    {
        Blackboard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        Blackboard->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    }else
    {
        Blackboard->ClearValue(TEXT("PlayerLocation"));
    }
}
