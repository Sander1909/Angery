// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "IFrameShield.h"


// Sets default values
AIFrameShield::AIFrameShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIFrameShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIFrameShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Follows the player.
	SetActorLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	
	DestructionTimer += DeltaTime;

	if (DestructionTimer > 1.0f)
	{
		Destroy();
	}
}

