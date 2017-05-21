// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "IFrameShield.generated.h"

UCLASS()
class GRUPPE_6_SAGN_API AIFrameShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIFrameShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float DestructionTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
