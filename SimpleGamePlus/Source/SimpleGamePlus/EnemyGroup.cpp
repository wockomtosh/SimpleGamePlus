// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGroup.h"

int AEnemyGroup::numEnemiesDestroyedForLaser = 0;

// Sets default values
AEnemyGroup::AEnemyGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyGroup::BeginPlay()
{
	Super::BeginPlay();
	
	AEnemyGroup::numEnemiesDestroyedForLaser = 0;
	StartNewWave();
}

// Called every frame
void AEnemyGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyGroup::StartNewWave()
{

	UWorld* world = GetWorld();
	if (world)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();

		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numColumns; j++)
			{
				FVector enemyLocation = FVector(i * -enemySpacingOffset, (j * enemySpacingOffset) - enemyGroupLeftEdgeOffset, 100);

				AEnemy* newEnemy = world->SpawnActor<AEnemy>(enemyClass, enemyLocation, FRotator::ZeroRotator, spawnParams);
				if (newEnemy)
				{
					newEnemy->enemyGroup = this;
					enemyList.Add(newEnemy);
				}
			}
		}
	}

	FTimerHandle StartMoveHandle;
	GetWorldTimerManager().SetTimer(StartMoveHandle, this, &AEnemyGroup::MoveEnemies, .5f);

	FTimerHandle FireHandle;
	GetWorldTimerManager().SetTimer(FireHandle, this, &AEnemyGroup::EnemyFireLoop, .5f + currentFireRate);
}

void AEnemyGroup::MoveEnemies()
{
	float currentSpeed = baseEnemySpeed;
	int maxEnemies = numRows * numColumns;
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::FromInt(enemyList.Num()));
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::FromInt(maxEnemies * (2.0f / 5.0f)));
	if (enemyList.Num() < (maxEnemies / 8))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("x4"));
		currentSpeed *= 4;
	}
	else if (enemyList.Num() < (maxEnemies * (2.0f / 5.0f)))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("x3"));
		currentSpeed *= 3;
	}
	else if (enemyList.Num() < (maxEnemies * (3.0f / 5.0f)))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("x2"));
		currentSpeed *= 2;
	}

	FVector currentVelocity = FVector(0, movingRight ? currentSpeed : -currentSpeed, 0);
	for (AEnemy* enemy : enemyList)
	{
		enemy->velocity = currentVelocity;
	}
}

void AEnemyGroup::MoveEnemiesDown()
{
	FVector currentVelocity = FVector(-downMoveSpeed, 0, 0);
	for (AEnemy* enemy : enemyList)
	{
		enemy->velocity = currentVelocity;
	}
}

void AEnemyGroup::EnemyFireLoop()
{
	// Get random enemy to fire a missile
	enemyList[FMath::RandRange(0, enemyList.Num() - 1)]->FireMissile();

	// Set timer for it to start again based on current fire rate
	FTimerHandle FireHandle;
	GetWorldTimerManager().SetTimer(FireHandle, this, &AEnemyGroup::EnemyFireLoop, currentFireRate);
}

void AEnemyGroup::OnEnemyCollidedWithBoundary()
{
	// Drop closer quickly
	MoveEnemiesDown();

	// Set a timer to start moving sideways again
	movingRight = !movingRight;
	FTimerHandle stopDownHandle;
	GetWorldTimerManager().SetTimer(stopDownHandle, this, &AEnemyGroup::MoveEnemies, downMoveDuration);
}

void AEnemyGroup::OnEnemyDestroyed(AEnemy* enemy)
{
	enemyList.Remove(enemy);
	numEnemiesDestroyedForLaser++;
	if (enemyList.Num() == 0)
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);
		FTimerHandle waveHandle;
		GetWorldTimerManager().SetTimer(waveHandle, this, &AEnemyGroup::NotifyWaveComplete, 1, false);
	}
	else
	{
		MoveEnemies();
	}
}

void AEnemyGroup::NotifyWaveComplete()
{
	currentWave++;

	currentFireRate -= .5f;
	if (currentFireRate < .1f)
	{
		currentFireRate = .1f;
	}

	StartNewWave();
}

