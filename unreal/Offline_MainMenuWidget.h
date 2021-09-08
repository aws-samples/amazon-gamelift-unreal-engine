// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Http.h"
#include "Offline_MainMenuWidget.generated.h"


UCLASS()
class GAMELIFT_UE4_API UOffline_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOffline_MainMenuWidget(const FObjectInitializer& ObjectInitializer);
	

	UFUNCTION(BlueprintCallable)
		void OnLoginClicked();
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString user;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString pass;


private:
	FHttpModule* Http;

	void LoginRequest(FString user, FString pass);
	void OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnGetDataResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
