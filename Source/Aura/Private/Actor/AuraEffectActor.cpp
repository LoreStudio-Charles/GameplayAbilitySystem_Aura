// Copyright Charles A. McCown

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "AbilitySystemComponent.h"
#include "Effect/AuraGameplayEffectWrapper.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* OtherActor, UAuraGameplayEffectWrapper* EffectClass)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	if(TargetAbilitySystemComponent == nullptr) return;

	check(EffectClass->GameplayEffectClass);
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(EffectClass->GameplayEffectClass, 1.0f, GameplayEffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());

	const bool bIsInfinite =  GameplayEffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && EffectClass->EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetAbilitySystemComponent);
	}
}

void AAuraEffectActor::OnOverlap(AActor* OtherActor)
{
	for (auto& Effect : GameplayEffects)
	{
		if(Effect->EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(OtherActor, Effect);	
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* OtherActor)
{
	for (auto& Effect : GameplayEffects)
	{
		if(Effect->EffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(OtherActor, Effect);		
		}

		if(Effect->EffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
			if(!IsValid(TargetAbilitySystemComponent)) return;

			TArray<FActiveGameplayEffectHandle> HandlesToRemove;
			for (auto HandlePair : ActiveEffectHandles)
			{
				if (TargetAbilitySystemComponent == HandlePair.Value)
				{
					TargetAbilitySystemComponent->RemoveActiveGameplayEffect(HandlePair.Key, 1);
					HandlesToRemove.Add(HandlePair.Key);
				}
			}
			for(auto Handle: HandlesToRemove)
			{
				ActiveEffectHandles.FindAndRemoveChecked(Handle);
			}
		}
	}
}

