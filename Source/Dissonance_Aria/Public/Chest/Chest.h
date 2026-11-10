

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/InteractableInterface.h"
#include "Chest.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UPDA_Equipment;

UCLASS()
class DISSONANCE_ARIA_API AChest : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* chestMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* boxComponent;

	bool pickedUp = false;

	UPROPERTY(EditAnywhere)
	UPDA_Equipment* equipment;
public:	
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Interact_Implementation(APlayerCharacterExploration* player) override;

	UFUNCTION(BlueprintCallable, Category = "SetUp")
	void SetUp(bool newPickedUp);

#pragma region Overlap
	void StartedOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
		UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	void EndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
		UPrimitiveComponent* otherComp, int32 otherBodyIndex);
#pragma endregion
};
