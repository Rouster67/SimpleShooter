// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components\SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creates root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	//creates components
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger()
{
	//Spawn Muzzle Flash
	if(MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleFlash,
			Mesh,
			TEXT("MuzzleFlashSocket")
		);
	}

	//get owner pawn
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	//get controller from owner pawn
	AController* OwnerController = OwnerPawn->GetController();
	if(OwnerController == nullptr) return;

	//gets start of line trace
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(
		ViewPointLocation,
		ViewPointRotation
	);
	//gets end of line trace
	FVector ViewPointEnd = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	//gets hit result
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerPawn);
	bool Hit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		ViewPointLocation,
		ViewPointEnd,
		ECollisionChannel::ECC_GameTraceChannel1,
		Params
	);

	if(Hit)
	{
		//Gets the direction of the shot
		FVector ShotDirection = -ViewPointRotation.Vector();
		//gets the actor
		AActor* HitActor = HitResult.GetActor();

		//applies damage to hit actor
		if(HitActor)
		{
			//creates damage event
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			//applies damage
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}

		//spawns impact effect
		if(ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ImpactEffect,
				HitResult.Location,
				ShotDirection.Rotation()
			);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

