// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVAL_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected: //Replicated Properties
	UPROPERTY(Replicated) //Replicate in serverside
		float Hunger;
	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
		float HungerDecrementValue;

	UPROPERTY(Replicated)
		float Thirst;
	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
		float ThirstDecrementValue;

	UPROPERTY(Replicated)
		float Stamina;
	UPROPERTY(EditAnywhere, Category = "S|PlayerStats")
		float StaminaDecrementValue;


	FTimerHandle TimerHandle;
	FTimerHandle StaminaTimerHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void HandleHungerAndThirst();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerLowerHunger(float value);
	bool ServerLowerHunger_Validate(float value);
	void ServerLowerHunger_Implementation(float value);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerLowerThirst(float value);
	bool ServerLowerThirst_Validate(float value);
	void ServerLowerThirst_Implementation(float value);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerLowerStamina(float value);
	bool ServerLowerStamina_validate(float value);
	void ServerLowerStamina_Implementation(float value);
	void RegenerateStamina();

public:
	void LowerHunger(float value);
	void LowerThirst(float value);
	void LowerStamina(float value);
	float GetHunger();
	float GetThirst();
	float GetStamina();
};
