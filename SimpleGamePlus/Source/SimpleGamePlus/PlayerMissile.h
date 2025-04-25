// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "PlayerMissile.generated.h"

UCLASS()
class SIMPLEGAMEPLUS_API APlayerMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMissile();

	static int numActiveMissiles;

	UPROPERTY(EditAnywhere)
	UBoxComponent* collisionComponent;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* projectileMovementComponent;

	UPROPERTY(EditAnywhere)
	float xDistanceUntilOffscreen = 200.0f;

	UPROPERTY(EditAnywhere)
	bool offscreen = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& fireDirection);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
