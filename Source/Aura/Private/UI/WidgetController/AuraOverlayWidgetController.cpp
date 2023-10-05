// Copyright Charles A. McCown


#include "UI/WidgetController/AuraOverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UAuraOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* Attributes = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(Attributes->GetHealth());
	OnMaxHealthChanged.Broadcast(Attributes->GetMaxHealth());
	OnManaChanged.Broadcast(Attributes->GetMana());
	OnMaxManaChanged.Broadcast(Attributes->GetMaxMana());

}

void UAuraOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* Attributes = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attributes->GetHealthAttribute()).AddUObject(this, &UAuraOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attributes->GetMaxHealthAttribute()).AddUObject(this, &UAuraOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attributes->GetManaAttribute()).AddUObject(this, &UAuraOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		Attributes->GetMaxManaAttribute()).AddUObject(this, &UAuraOverlayWidgetController::MaxManaChanged);
}

void UAuraOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
