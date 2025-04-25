#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Components/CapsuleComponent.h>
#include <InputMappingContext.h>
#include "PlayerMissile.h"
#include "Spaceship.generated.h"

UCLASS()
class SIMPLEGAMEPLUS_API ASpaceship : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpaceship();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* shield;

	UPROPERTY(EditAnywhere)
	float shieldDuration = .5f;

	UPROPERTY(EditAnywhere)
	float laserDuration = 2.0f;

	UPROPERTY(EditAnywhere)
	int enemiesToDefeatUntilPower = 10;

	UPROPERTY(EditAnywhere)
	int numAllowedActiveMissiles = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MissleLaunchOffset;

	// Input------------------------------------------
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* defaultInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* input_fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* input_move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* input_shield;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APlayerMissile> missileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ALaser> laserClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveRight(const FInputActionValue& value);

	UFUNCTION()
	void FireWeapon();

	UFUNCTION()
	void RaiseShield();

	UFUNCTION()
	void LowerShield();

	UFUNCTION()
	void OnShieldOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerHit();
};
