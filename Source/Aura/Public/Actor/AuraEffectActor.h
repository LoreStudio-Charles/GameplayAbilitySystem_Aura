// Copyright Charles A. McCown

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAuraGameplayEffectWrapper;
class UAuraGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Applied Effects")
	TArray<TObjectPtr<UAuraGameplayEffectWrapper>> GameplayEffects;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* OtherActor);
	
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* OtherActor);
	
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

private:
	void ApplyEffectToTarget(AActor* OtherActor, UAuraGameplayEffectWrapper* EffectClass);
	
};