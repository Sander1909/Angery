// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "Gruppe_6_SAGNGameModeBase.h"
#include "StandardEnemy.h"
#include "SpinningMeleeEnemy.h"
#include "PacmanEnemy.h"
#include "StrayEnemy.h"
#include "BossEnemy.h"

AGruppe_6_SAGNGameModeBase::AGruppe_6_SAGNGameModeBase()
{

	PrimaryActorTick.bCanEverTick = true;

}

void AGruppe_6_SAGNGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGruppe_6_SAGNGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	


}

void AGruppe_6_SAGNGameModeBase::debugFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello debug"));
}

void AGruppe_6_SAGNGameModeBase::ShowVictoryHUD_Implementation()
{

}

void AGruppe_6_SAGNGameModeBase::NextWaveAnnouncement_Implementation()
{

}

