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
	//checks if gun can fire
	if(!CanFire) return;

	//plays muzzle effects
	MuzzleEffects();

	//creates hit result and shot direction
	FHitResult HitResult;
	FVector ShotDirection;
	bool Hit = GunTrace(HitResult, ShotDirection);
	//if you hit something, apply damage and spawn impact effect
	if(Hit)
	{

		ApplyDamage(HitResult, ShotDirection);

		//plays impact effects
		ImpactEffects(HitResult, ShotDirection);
	}

	//sets up next time to fire
	TriggerDelay();
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

	if(!CanFire && GetWorld()->GetTimeSeconds() > CanFireTime)
	{
		CanFire = true;
	}
}

bool AGun::GunTrace(FHitResult& HitResult, FVector& ShotDirection)
{
	//gets start of line trace
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetOwnerController()->GetPlayerViewPoint(
		ViewPointLocation,
		ViewPointRotation
	);
	//gets end of line trace
	FVector ViewPointEnd = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	//Gets the direction of the shot
	ShotDirection = -ViewPointRotation.Vector();

	//gets hit result
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwnerPawn());
	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		ViewPointLocation,
		ViewPointEnd,
		ECollisionChannel::ECC_GameTraceChannel1,
		Params
	);
}

void AGun::ApplyDamage(FHitResult& HitResult, FVector& ShotDirection)
{
	//gets the actor
	AActor* HitActor = HitResult.GetActor();
	//Gets the bone hit
	FString BoneName = HitResult.BoneName.ToString();

	UE_LOG(LogTemp, Display, TEXT("Bone Hit: %s"), *BoneName);

	float thisDamage = Damage;
	if(BoneName == "head")
	{
		thisDamage *= HeadShotMultiplier;

		if(HeadShotSound)
	{
		UGameplayStatics::SpawnSoundAttached(
			HeadShotSound,
			Mesh,
			TEXT("MuzzleFlashSocket")
		);
	}
	}

	//applies damage to hit actor
	if(HitActor)
	{
		//creates damage event
		FPointDamageEvent DamageEvent(thisDamage, HitResult, ShotDirection, nullptr);
		//applies damage
		HitActor->TakeDamage(thisDamage, DamageEvent, Cast<APawn>(GetOwner())->GetController(), this);
	}
}

void AGun::MuzzleEffects()
{
	//Plays Sound
	if(MuzzleSound)
	{
		UGameplayStatics::SpawnSoundAttached(
			MuzzleSound,
			Mesh,
			TEXT("MuzzleFlashSocket")
		);
	}

	//Spawn Visual Effect
	if(MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(
			MuzzleFlash,
			Mesh,
			TEXT("MuzzleFlashSocket")
		);
	}
}

void AGun::ImpactEffects(FHitResult& HitResult, FVector& ShotDirection)
{
	//Plays Sound
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ImpactSound,
			HitResult.Location
		);
	}

	//Spawn Visual Effect
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

void AGun::TriggerDelay()
{
	CanFire = false;
	TimeBetweenShots = 1 / RateOfFire;
	CanFireTime = GetWorld()->GetTimeSeconds() + TimeBetweenShots;
}

class APawn* AGun::GetOwnerPawn() const
{
	return Cast<APawn>(GetOwner());
}

class AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr)
		return nullptr;
	else
		return OwnerPawn->GetController();
}

