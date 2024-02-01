// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

//forward declarations
class UParticleSystem14;

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//visual effects
	UPROPERTY(EditAnywhere, Category="Effects")
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, Category="Effects")
	UParticleSystem* ImpactEffect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//components
	UPROPERTY(VisibleAnywhere, Category="Components")
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category="Components")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundBase* MuzzleSound;
	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundBase* ImpactSound;
	UPROPERTY(EditAnywhere, Category="Sounds")
	class USoundBase* HeadShotSound;

	//Variables
	UPROPERTY(EditAnywhere, Category="Weapon")
	float MaxRange = 100000;
	UPROPERTY(EditAnywhere, Category="Weapon")
	float Damage = 40;
	UPROPERTY(EditAnywhere, Category="Weapon")
	float HeadShotMultiplier = 2.5;
	UPROPERTY(EditAnywhere, Category="Weapon")
	float RateOfFire = 1;

	float TimeBetweenShots;
	bool CanFire = true;
	double CanFireTime;

	//Functions
	bool GunTrace(FHitResult& HitResult, FVector& ShotDirection);
	void ApplyDamage(FHitResult& HitResult, FVector& ShotDirection);
	void MuzzleEffects();
	void ImpactEffects(FHitResult& HitResult, FVector& ShotDirection);
	void TriggerDelay();
	class APawn* GetOwnerPawn() const;
	class AController* GetOwnerController() const;

};
