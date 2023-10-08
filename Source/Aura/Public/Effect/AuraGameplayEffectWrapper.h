// Copyright Charles A. McCown

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraEffectActor.h"
#include "UObject/Object.h"
#include "AuraGameplayEffectWrapper.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, HideDropdown)
class AURA_API UAuraGameplayEffectWrapper : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EGameplayEffectDurationType EffectDurationType = EGameplayEffectDurationType::Instant;
};
