// Copyright Charles A. McCown


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(25.f);
	InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());		
	}

	if(Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
	}

	if(Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& Properties)
{
	Properties.ContextHandle = Data.EffectSpec.GetContext();
	Properties.Source.AbilitySystemComponent = Properties.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(Properties.Source.AbilitySystemComponent) &&
		Properties.Source.AbilitySystemComponent->AbilityActorInfo.IsValid() &&
		Properties.Source.AbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.Source.AvatarActor = Properties.Source.AbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		Properties.Source.Controller = Properties.Source.AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		if(Properties.Source.Controller == nullptr &&
			Properties.Source.AvatarActor != nullptr)
		{
			if(APawn* Pawn = Cast<APawn>(Properties.Source.AvatarActor))
			{
				Properties.Source.Controller = Pawn->GetController();
			}
		}
		if(Properties.Source.Controller)
		{
			Properties.Source.Character = Cast<ACharacter>(Properties.Source.Controller->GetPawn());
		}
	}
	
	if(Data.Target.AbilityActorInfo.IsValid() &&
		Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.Target.AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Properties.Target.Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		Properties.Target.Character = Cast<ACharacter>(Properties.Target.AvatarActor);
		Properties.Target.AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Properties.Target.AvatarActor);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);
	
	if(Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	
	if(Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
