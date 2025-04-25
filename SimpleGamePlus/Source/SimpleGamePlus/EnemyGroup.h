// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemyGroup.generated.h"

UCLASS()
class SIMPLEGAMEPLUS_API AEnemyGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyGroup();

	static int numEnemiesDestroyedForLaser;

	UPROPERTY(VisibleAnywhere)
	TArray<AEnemy*> enemyList;

	UPROPERTY(EditAnywhere)
	int numRows = 5;

	UPROPERTY(EditAnywhere)
	int numColumns = 8;

	UPROPERTY(EditAnywhere)
	float enemySpacingOffset = 80.0f;

	UPROPERTY(EditAnywhere)
	float enemyGroupLeftEdgeOffset = 1000.0f;

	UPROPERTY(EditAnywhere)
	float baseEnemySpeed = 150.0f;

	UPROPERTY(EditAnywhere)
	float downMoveSpeed = 600.0f;

	UPROPERTY(EditAnywhere)
	float downMoveDuration = .1f;

	UPROPERTY(VisibleAnywhere)
	bool movingRight = true;

	UPROPERTY(VisibleAnywhere)
	bool movingDown = false;

	UPROPERTY(VisibleAnywhere)
	int currentWave = 0;

	UPROPERTY(EditAnywhere)
	float initialFireRate = 3.0f;

	UPROPERTY(VisibleAnywhere)
	float currentFireRate = 3.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy> enemyClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StartNewWave();

	UFUNCTION()
	void MoveEnemies();

	UFUNCTION()
	void MoveEnemiesDown();

	UFUNCTION()
	void EnemyFireLoop();

	UFUNCTION()
	void OnEnemyCollidedWithBoundary();

	UFUNCTION()
	void OnEnemyDestroyed(AEnemy* enemy);

	UFUNCTION()
	void NotifyWaveComplete();
};
