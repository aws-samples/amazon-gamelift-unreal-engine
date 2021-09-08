// Fill out your copyright notice in the Description page of Project Settings.

#include <string>
#include "Offline_MainMenuWidget.h"
//#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Json.h"
#include "JsonUtilities.h"

// TODO Update with your own API Gateway Endpoint
const static string API_GATEWAY_ENDPOINT = TEXT("https://your-endpoint.execute-api.us-west-2.amazonaws.com/test");
const static string LOGIN_URI = TEXT("/login");
const static string STARTSESSION_URI = TEXT("/startsession");

UOffline_MainMenuWidget::UOffline_MainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	Http = &FHttpModule::Get();
}


void UOffline_MainMenuWidget::OnLoginClicked() {
	UE_LOG(LogTemp, Warning, TEXT("Login button clicked\nuser: %s"), *user);
	UE_LOG(LogTemp, Warning, TEXT("pass: %s"), *pass);

	LoginRequest(user, pass);
}

void UOffline_MainMenuWidget::LoginRequest(FString usr, FString pwd) {
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("username"), *FString::Printf(TEXT("%s"), *usr));
	JsonObject->SetStringField(TEXT("password"), *FString::Printf(TEXT("%s"), *pwd));

	FString JsonBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> LoginHttpRequest = Http->CreateRequest();


	LoginHttpRequest->SetVerb("POST");
	LoginHttpRequest->SetURL(*FString::Printf(API_GATEWAY_ENDPOINT + LOGIN_URI));
	LoginHttpRequest->SetHeader("Content-Type", "application/json");
	LoginHttpRequest->SetContentAsString(JsonBody);
	LoginHttpRequest->OnProcessRequestComplete().BindUObject(this, &UOffline_MainMenuWidget::OnLoginResponse);
	LoginHttpRequest->ProcessRequest();


}


void UOffline_MainMenuWidget::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (bWasSuccessful) {
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			FString IdToken = JsonObject->GetStringField("id_token");

			//UE_LOG(LogTemp, Warning, TEXT("%s"), *IdToken);

			TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GetDataHttpRequest = Http->CreateRequest();


			GetDataHttpRequest->SetVerb("GET");
			GetDataHttpRequest->SetURL(*FString::Printf(API_GATEWAY_ENDPOINT + STARTSESSION_URI));
			GetDataHttpRequest->SetHeader("Content-Type", "application/json");
			GetDataHttpRequest->SetHeader("Authorization", IdToken);
			GetDataHttpRequest->OnProcessRequestComplete().BindUObject(this, &UOffline_MainMenuWidget::OnGetDataResponse);
			GetDataHttpRequest->ProcessRequest();
		}

	}


}

void UOffline_MainMenuWidget::OnGetDataResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (bWasSuccessful) {
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			FString IP = JsonObject->GetStringField("ip");
			FString Port = JsonObject->GetStringField("port");

			UE_LOG(LogTemp, Warning, TEXT("Success!\nIP: %s\nPort: %s"), *IP, *Port);

			//ConnectToServer(IP, Port);
		}

	}
}

