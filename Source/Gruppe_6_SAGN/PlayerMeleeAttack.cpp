// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "PlayerMeleeAttack.h"


// Sets default values
APlayerMeleeAttack::APlayerMeleeAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerMeleeAttack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerMeleeAttack::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Follows the player.
	SetActorLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

	TimeToDestroy += DeltaTime;
	if (TimeToDestroy > 0.3f)
	{
		Destroy();
	}

}
