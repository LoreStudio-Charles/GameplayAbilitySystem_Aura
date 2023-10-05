// Copyright Charles A. McCown


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WC_Params)
{
	PlayerState = WC_Params.PlayerState;
	PlayerController = WC_Params.PlayerController;
	AbilitySystemComponent = WC_Params.AbilitySystemComponent;
	AttributeSet = WC_Params.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}
