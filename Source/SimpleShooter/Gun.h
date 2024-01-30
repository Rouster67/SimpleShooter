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

	//Variables
	UPROPERTY(EditAnywhere, Category="Weapon")
	float MaxRange = 100000;
	UPROPERTY(EditAnywhere, Category="Weapon")
	float Damage = 40;
};
