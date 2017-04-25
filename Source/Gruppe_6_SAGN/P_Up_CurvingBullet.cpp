// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "P_Up_CurvingBullet.h"


// Sets default values
AP_Up_CurvingBullet::AP_Up_CurvingBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AP_Up_CurvingBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP_Up_CurvingBullet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FRotator Temp = FRotator(0.0f, 1.0f, 0.0f) + GetActorRotation();
	SetActorRotation(Temp);

}

