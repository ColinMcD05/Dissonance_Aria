

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/InteractableInterface.h"
#include "Chest.generated.h"

UCLASS()
class DISSONANCE_ARIA_API AChest : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
private:
	bool pickedUp;

public:	
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Interact() override;
};
