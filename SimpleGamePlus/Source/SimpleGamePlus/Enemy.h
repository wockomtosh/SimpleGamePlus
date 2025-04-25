// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Enemy.generated.h"

class AEnemyGroup;

UCLASS()
class SIMPLEGAMEPLUS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere)
	AEnemyGroup* enemyGroup;

	UPROPERTY(EditAnywhere)
	UBoxComponent* collisionComponent;

	UPROPERTY(EditAnywhere)
	FVector velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector missileLaunchOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemyMissile> missileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void FireMissile();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
