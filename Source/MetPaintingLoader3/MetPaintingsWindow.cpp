// Fill out your copyright notice in the Description page of Project Settings.


#include "MetPaintingsWindow.h"

#include "EditorUtilityWidgetComponents.h"
#include "HttpModule.h"



void UMetPaintingsWindow::NativeConstruct()
{
	Super::NativeConstruct();

	TEMPEditorComboTest1->AddOption("Penis");
	TEMPEditorComboTest1->AddOption("Penis2");
	TEMPEditorComboTest1->AddOption("Penis3");
}

void UMetPaintingsWindow::QueryAPI(FString search)
{
	auto Req = FHttpModule::Get().CreateRequest();
	Req->SetVerb("GET");
	// Req->SetOption()
	// Req->SetURL(URLBase + FString::FromInt(ObjectID));
}

void UMetPaintingsWindow::GetDepartments()
{
	auto Req = FHttpModule::Get().CreateRequest();
	Req->SetVerb("GET");
	Req->SetURL("https://collectionapi.metmuseum.org/public/collection/v1/departments");
	
}

// void UMetPaintingsWindow::OnGetDepartmentsComplete()
// {
// }

