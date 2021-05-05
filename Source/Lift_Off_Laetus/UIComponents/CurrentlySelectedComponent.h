#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentlySelected.generated.h"

UCLASS( Abstract )
class LIFT_OFF_LAETUS_API UCurrentlySelectedArrow : public UUserWidget {
	GENERATED_BODY()
public:
	
	
protected:
	UPROPERTY(meta = (BindWidget))
		class UImage* currentlySelectedArrowImage;

};