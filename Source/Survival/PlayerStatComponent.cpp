// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	Hunger = 100;
	HungerDecrementValue = 0.3f;
	Thirst = 100;
	ThirstDecrementValue = 0.5f;
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	//Make properties replicate on the server.
	SetIsReplicated(true); 

	//Hook stat handling functions to the world timer, and set them to repeat.
	GetWorld()->GetTimerManager().SetTimer
	(TimerHandle, this, &UPlayerStatComponent::HandleHungerAndThirst, 3.0f, true);
	GetWorld()->GetTimerManager().SetTimer
	(StaminaHandle, this, &UPlayerStatComponent::RegenerateStamina, 1.0f, true);
}

//This method handles server replication of properties for this component.
void UPlayerStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate to everyone.
	DOREPLIFETIME(UPlayerStatComponent, Hunger);
	DOREPLIFETIME(UPlayerStatComponent, Thirst);
	DOREPLIFETIME(UPlayerStatComponent, Stamina);
}

void UPlayerStatComponent::HandleHungerAndThirst()
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		LowerHunger(HungerDecrementValue);
		LowerThirst(ThirstDecrementValue);
	}
}
void UPlayerStatComponent::LowerHunger(float value)
{
	if (GetOwnerRole() < ROLE_Authority)
	{ //If we are the client, and not the server.
		ServerLowerHunger(value);
	}
	else
	{ //We do have authority.
		Hunger -= value;
	}
}
void UPlayerStatComponent::LowerThirst(float value)
{
	if (GetOwnerRole() < ROLE_Authority)
		ServerLowerThirst(value);
	else
		Thirst -= value;
}

bool UPlayerStatComponent::ServerLowerHunger_Validate(float value)
{
	return true;
}
void UPlayerStatComponent::ServerLowerHunger_Implementation(float value)
{
	if (GetOwnerRole() == ROLE_Authority)
		LowerHunger(value);
}
bool UPlayerStatComponent::ServerLowerThirst_Validate(float value)
{
	return true;
}
void UPlayerStatComponent::ServerLowerThirst_Implementation(float value)
{
	if (GetOwnerRole() == ROLE_Authority)
		LowerThirst(value);
}

void UPlayerStatComponent::LowerStamina(float value)
{

}

void UPlayerStatComponent::RegenerateStamina()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (Stamina > 100.0f)
			Stamina = 100.0f;
		else
			++Stamina;
	}
}

float UPlayerStatComponent::GetHunger()
{
	return this->Hunger;
}

float UPlayerStatComponent::GetThirst()
{
	return this->Thirst;
}

float UPlayerStatComponent::GetStamina()
{
	return Stamina;
}
