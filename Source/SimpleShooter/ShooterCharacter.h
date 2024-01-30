// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

//forward declarations
class AGun;
UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void PullTrigger();

	UPROPERTY(EditAnywhere, Category="Movement")
	float RotationRate = 75;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditDefaultsOnly, Category="Stats")
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category="Stats")
	float Health;

	UPROPERTY()
	AGun* Gun;
	
};










/*
formula to calculate animation speed
foot_speed = (y_finish - y_start) / (t_finish - t_start)
t = time
y = y axis
*/



/*
FS = (YF - YS) / (TF - TS)
FS = (-39.774193 - 19.034456) / (.35 - .15)
FS = -58.808649 / 0.2
Run FS = -294.043245

FS = (-26.927351 - 35.112503) / (.71 - .22)
FS = -62.039854 / .49
Walk FS = -126.6119469387755

Walk = 150 
Run = 300
Divisions = 6 (300 / 50)
*/