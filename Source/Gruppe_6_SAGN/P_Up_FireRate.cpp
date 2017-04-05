// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "P_Up_FireRate.h"


// Sets default values
AP_Up_FireRate::AP_Up_FireRate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AP_Up_FireRate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP_Up_FireRate::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FRotator NewRotation = GetActorRotation() + FRotator(0.0f, 1.0f, 0.0f);
	SetActorRotation(NewRotation);

}

