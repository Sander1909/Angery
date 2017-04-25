// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe_6_SAGN.h"
#include "Gruppe_6_SAGNGameModeBase.h"
#include "StandardEnemy.h"
#include "SpinningMeleeEnemy.h"
#include "PacmanEnemy.h"
#include "RandomEnemy.h"
#include "StrayEnemy.h"
#include "BossEnemy.h"

AGruppe_6_SAGNGameModeBase::AGruppe_6_SAGNGameModeBase()
{

	PrimaryActorTick.bCanEverTick = true;

}

void AGruppe_6_SAGNGameModeBase::BeginPlay()
{

	Super::BeginPlay();
	//Wave 7 gjør at spillet kan testes uten spawning.
	//WaveNumber = 5;


}

void AGruppe_6_SAGNGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	


	if (!WaveIntermission)
	{
		switch (WaveNumber)
		{
		case 1:

			StandardSpawnTimer += DeltaTime;

			if (StandardSpawnTimer > 1.5f && EnemiesSpawned < 15)
			{
				SpawnStandardEnemy();
				EnemiesSpawned++;
				StandardSpawnTimer = 0.0f;
			}
			//Kontrollerer hvor mange enemies som skal spawnes i waven, starter waveintermission.
			if (EnemiesSpawned >= 15)
			{
				WaveIntermission = true;
				UE_LOG(LogTemp, Warning, TEXT("First Round is OVER!"));
			}

			break;

		case 2:

			StraySpawnTimer += DeltaTime;
			StandardSpawnTimer += DeltaTime;

			if (StraySpawnTimer > 5.0f && EnemiesSpawned < 20)
			{
				SpawnStrayEnemy();
				EnemiesSpawned++;
				StraySpawnTimer = 0.0f;
			}

			if (StandardSpawnTimer > 1.5f && EnemiesSpawned < 20)
			{
				SpawnStandardEnemy();
				EnemiesSpawned++;
				StandardSpawnTimer = 0.0f;
			}

			if (EnemiesSpawned >= 20)
			{
				WaveIntermission = true;
				UE_LOG(LogTemp, Warning, TEXT("Second Round is OVER!"));
			}

			break;

		case 3:

			SpinningMeleeSpawnTimer += DeltaTime;
			StandardSpawnTimer += DeltaTime;

			if (SpinningMeleeSpawnTimer > 3.5f && EnemiesSpawned < 20)
			{
				SpawnSpinningMeleeEnemy();
				EnemiesSpawned++;
				SpinningMeleeSpawnTimer = 0.0f;
			}

			if (StandardSpawnTimer > 1.5f && EnemiesSpawned < 20)
			{
				SpawnStandardEnemy();
				EnemiesSpawned++;
				StandardSpawnTimer = 0.0f;
			}

			if (EnemiesSpawned >= 20)
			{
				WaveIntermission = true;
				UE_LOG(LogTemp, Warning, TEXT("Third Round is OVER!"));
			}

			break;

		case 4:

			PacmanSpawnTimer += DeltaTime;
			StandardSpawnTimer += DeltaTime;

			if (PacmanSpawnTimer > 3.0f && EnemiesSpawned < 20)
			{
				SpawnPacmanEnemy();
				EnemiesSpawned++;
				PacmanSpawnTimer = 0.0f;
			}

			if (StandardSpawnTimer > 1.5f && EnemiesSpawned < 20)
			{
				SpawnStandardEnemy();
				EnemiesSpawned++;
				StandardSpawnTimer = 0.0f;
			}

			if (EnemiesSpawned >= 20)
			{
				WaveIntermission = true;
				UE_LOG(LogTemp, Warning, TEXT("Fourth Round is OVER!"));
			}

			break;

		case 5:

			PacmanSpawnTimer += DeltaTime;
			SpinningMeleeSpawnTimer += DeltaTime;
			StraySpawnTimer += DeltaTime;

			if (SpinningMeleeSpawnTimer > 4.0f && EnemiesSpawned < 40)
			{
				SpawnSpinningMeleeEnemy();
				EnemiesSpawned++;
				SpinningMeleeSpawnTimer = 0.0f;
			}

			if (PacmanSpawnTimer > 5.0f && EnemiesSpawned < 40)
			{
				SpawnPacmanEnemy();
				EnemiesSpawned++;
				PacmanSpawnTimer = 0.0f;
			}

			if (StraySpawnTimer > 6.0f && EnemiesSpawned < 40)
			{
				SpawnStrayEnemy();
				EnemiesSpawned++;
				StraySpawnTimer = 0.0f;
			}

			if (EnemiesSpawned >= 40)
			{
				WaveTimer += DeltaTime;
				if (WaveTimer > 20.0f)
				{
					WaveNumber++;
					WaveIntermission = false;
					UE_LOG(LogTemp, Warning, TEXT("Next Wave had BEGUN!"));
					WaveTimer = 0.0f;
					EnemiesSpawned = 0;
					UGameplayStatics::PlaySound2D(GetWorld(), OnNextWaveSound, 0.5f, 1.0f, 0.0f);
				}
				UE_LOG(LogTemp, Warning, TEXT("Fourth Round is OVER!"));
			}

			break;

		case 6:

			SpawnBossEnemy();
			WaveNumber++;
			UE_LOG(LogTemp, Warning, TEXT("Fifth Round is OVER!"));
			break;

		case 7:

			break;
		}
	}

	else
	{
		WaveTimer += DeltaTime;
		if (WaveTimer > 10.0f)
		{
			WaveNumber++;
			WaveIntermission = false;
			UE_LOG(LogTemp, Warning, TEXT("Next Wave had BEGUN!"));
			WaveTimer = 0.0f;
			EnemiesSpawned = 0;
			UGameplayStatics::PlaySound2D(GetWorld(), OnNextWaveSound, 0.5f, 1.0f, 0.0f);
		}
	}



}

void AGruppe_6_SAGNGameModeBase::SpawnStandardEnemy()
{
	World = GetWorld();

	FVector Location = FVector(SpawnValuesX[rand()%2], SpawnValuesY[rand()%2], 0.0f);

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - Location;


	World->SpawnActor<AStandardEnemy>(StandardEnemy_BP, Location, NewDirection.Rotation());
}

void AGruppe_6_SAGNGameModeBase::SpawnPacmanEnemy()
{
	World = GetWorld();

	FVector Location = FVector(PacmanSpawnValues[rand() % 2], PacmanSpawnValues[rand() % 2], 0.0f);

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - Location;


	World->SpawnActor<APacmanEnemy>(PacmanEnemy_BP, Location, FRotator::ZeroRotator);
}

void AGruppe_6_SAGNGameModeBase::SpawnRandomEnemy()
{
	World = GetWorld();

	FVector Location = FVector(SpawnValuesX[rand() % 2], SpawnValuesY[rand() % 2], 0.0f);

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - Location;


	World->SpawnActor<ARandomEnemy>(RandomEnemy_BP, Location, NewDirection.Rotation());
}

void AGruppe_6_SAGNGameModeBase::SpawnStrayEnemy()
{
	World = GetWorld();

	FVector Location = FVector(SpawnValuesX[rand() % 2], SpawnValuesY[rand() % 2], 0.0f);

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - Location;

	World->SpawnActor<AStrayEnemy>(StrayEnemy_BP, Location, NewDirection.Rotation());
}

void AGruppe_6_SAGNGameModeBase::SpawnSpinningMeleeEnemy()
{
	World = GetWorld();

	FVector Location = FVector(PacmanSpawnValues[rand() % 2], PacmanSpawnValues[rand() % 2], 0.0f);

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - Location;

	World->SpawnActor<ASpinningMeleeEnemy>(SpinningMeleeEnemy_BP, Location, NewDirection.Rotation());
}


void AGruppe_6_SAGNGameModeBase::SpawnBossEnemy()
{
	World = GetWorld();

	FVector Location = FVector(0.0f, 0.0f, 2000.0f);

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FVector NewDirection = PlayerLocation - Location;


	World->SpawnActor<ABossEnemy>(BossEnemy_BP, Location, NewDirection.Rotation());
}
