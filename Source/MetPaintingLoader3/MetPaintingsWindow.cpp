// Fill out your copyright notice in the Description page of Project Settings.


#include "MetPaintingsWindow.h"

#include "EditorUtilityWidgetComponents.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"


void UMetPaintingsWindow::NativeConstruct()
{
	Super::NativeConstruct();
	//
	// TEMPEditorComboTest1->AddOption("Penis");
	// TEMPEditorComboTest1->AddOption("Penis2");
	// TEMPEditorComboTest1->AddOption("Penis3");
	// GetDepartments();
}


void UMetPaintingsWindow::OnGetDepartmentsComplete(TSharedPtr<IHttpRequest> HttpRequest,
	TSharedPtr<IHttpResponse> HttpResponse, bool bSuccess)
{
	// if (!bSuccess)
	// {
	// 	// Todo: Fire a delegate that the image is loaded with a placeholder texture, or do something
	// 	// to indicate that this failed. Not sure how I should handle that. 
	// 	UE_LOG(LogTemp, Error, TEXT("Departments Request failed"));
	// 	return;
	// }
	// FDepartmentsStruct Departments;
	// FJsonObjectConverter::JsonObjectStringToUStruct(HttpResponse->GetContentAsString(), &Departments, 0, 0);
	// for (FDepartmentEntry Element : Departments.departments)
	// {
	// 	TEMPEditorComboTest1->AddOption(Element.displayName);
	// }
}

// void UMetPaintingsWindow::GetDepartments()
// {
// 	auto Req = FHttpModule::Get().CreateRequest();
// 	Req->SetVerb("GET");
// 	Req->SetURL("https://collectionapi.metmuseum.org/public/collection/v1/departments");
// 	
// 	Req->OnProcessRequestComplete().BindUObject(this, &UMetPaintingsWindow::OnGetDepartmentsComplete);
// 	Req->ProcessRequest();
// }


