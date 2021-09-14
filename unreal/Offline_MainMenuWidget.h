// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "Offline_MainMenuWidget.generated.h"


UCLASS()
class GAMELIFT_UE4_API UOffline_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOfflineMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void OnLoginClicked();
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString ApiGatewayEndpoint;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString LoginURI;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		FString StartSessionURI;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
		FString user;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
		FString pass;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
		FString IpAddress;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
		FString Port;


private:
	FHttpModule* Http;

	void LoginRequest(FString usr, FString pwd);
	void OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void StartSessionRequest(FString idt);
	void OnStartSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
