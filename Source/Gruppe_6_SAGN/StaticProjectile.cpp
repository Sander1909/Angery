// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "StaticProjectile.h"


// Sets default values
AStaticProjectile::AStaticProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStaticProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AStaticProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	TimeToDestroy += DeltaTime;

	if (TimeToDestroy > 6.5f)
	{
		Destroy();
	}
}

