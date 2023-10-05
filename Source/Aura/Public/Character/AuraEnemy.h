// Copyright Charles A. McCown

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/InteractionHintInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IInteractionHintInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	// Enemy Interface
	virtual void HighlightActor() override;
	virtual void ClearHighlightActor() override;

protected:
	virtual void BeginPlay() override;
};
