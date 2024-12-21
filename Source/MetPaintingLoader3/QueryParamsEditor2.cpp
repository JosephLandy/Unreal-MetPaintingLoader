// Fill out your copyright notice in the Description page of Project Settings.


#include "QueryParamsEditor2.h"

#include "EditorUtilityWidgetComponents.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Components/SinglePropertyView.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpResponse.h"

void UQueryParamsEditor2::NativeConstruct()
{
	Super::NativeConstruct();
	// QueryParamsDetailsView->SetObject(this);
	GetDepartments();
	
}

void UQueryParamsEditor2::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	QueryParamsDetailsView->SetObject(this);
}

// void UQueryParamsEditor2::NativePreConstruct()
// {
// 	Super::NativePreConstruct();
// 	QueryParamsView->SetObject(this);
// }


void UQueryParamsEditor2::OnGetDepartmentsComplete(TSharedPtr<IHttpRequest> HttpRequest,
                                                   TSharedPtr<IHttpResponse> HttpResponse, bool bSuccess)
{
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Departments Request failed"));
		return;
	}
	
	FJsonObjectConverter::JsonObjectStringToUStruct(HttpResponse->GetContentAsString(), &Departments, 0, 0);
	DepartmentsComboBox->ClearOptions();
	DepartmentsComboBox->AddOption("All");
	for (FDepartmentEntry Element : Departments.departments)
	{
		DepartmentsComboBox->AddOption(Element.displayName);
	}
}

FString UQueryParamsEditor2::GenerateQueryParams()
{
	FString QueryParams = "hasImages=true"; // Always include "hasImages" in the search
	// unfortuneately, hasImages = true does not 

	// Helper macro to append query parameters conditionally
	auto AppendParam = [&](const FString& Key, auto Value, bool UseFlag) {
		if (UseFlag)
		{
			if (!QueryParams.IsEmpty())
			{
				QueryParams += "&"; // Append '&' between parameters
			}
			QueryParams += FString::Printf(TEXT("%s=%s"), *Key, *FString::FromInt(Value));
		}
	};

	auto AppendBoolParam = [&](const FString& Key, const bool& Value, bool UseFlag) {
		if (UseFlag)
		{
			if (!QueryParams.IsEmpty())
			{
				QueryParams += "&"; // Append '&' between parameters
			}

			if (Value)
			{
				QueryParams += FString::Printf(TEXT("%s=%s"), *Key, TEXT("true"));
			}
			else
			{
				QueryParams += FString::Printf(TEXT("%s=%s"), *Key, TEXT("false"));
			}
			
		}
	};

	auto AppendStringParam = [&](const FString& Key, const FString& Value, bool UseFlag) {
		if (UseFlag)
		{
			if (!QueryParams.IsEmpty())
			{
				QueryParams += "&";
			}
			QueryParams += FString::Printf(TEXT("%s=%s"), *Key, *FGenericPlatformHttp::UrlEncode(Value));
		}
	};
	AppendBoolParam("isHighlight", SearchParameters.isHighlight, SearchParameters.Use_isHighlight);
	AppendBoolParam("title", SearchParameters.title, SearchParameters.Use_title);
	AppendBoolParam("tags", SearchParameters.tags, SearchParameters.Use_tags);
	// AppendParam("departmentId", SearchParameters.departmentId, SearchParameters.Use_departmentId);
	AppendBoolParam("isOnView", SearchParameters.isOnView, SearchParameters.Use_isOnView);
	AppendBoolParam("artistOrCulture", SearchParameters.artistOrCulture, SearchParameters.Use_artistOrCulture);
	AppendStringParam("medium", SearchParameters.medium, SearchParameters.Use_medium);
	AppendStringParam("geoLocation", SearchParameters.geoLocation, SearchParameters.Use_geoLocation);
	AppendParam("dateBegin", SearchParameters.dateBegin, SearchParameters.Use_dateBegin);
	AppendParam("dateEnd", SearchParameters.dateEnd, SearchParameters.Use_dateEnd);
	
	FString SelectedDep = DepartmentsComboBox->GetSelectedOption();
	UE_LOG(LogTemp, Log, TEXT("Selected department: %s"), *SelectedDep);
	FDepartmentEntry* Selected = Departments.departments.FindByPredicate(
		[SelectedDep](const FDepartmentEntry& Entry) { return Entry.displayName == SelectedDep; });
	if (Selected)
	{
		AppendParam("departmentId", Selected->departmentId, true);
	}
	// q must be last!!!
	AppendStringParam("q", SearchParameters.q, SearchParameters.Use_q);
	
	return QueryParams;
}

void UQueryParamsEditor2::GetDepartments()
{
	auto Req = FHttpModule::Get().CreateRequest();
	Req->SetVerb("GET");
	Req->SetURL("https://collectionapi.metmuseum.org/public/collection/v1/departments");
	
	Req->OnProcessRequestComplete().BindUObject(this, &UQueryParamsEditor2::OnGetDepartmentsComplete);
	Req->ProcessRequest();
}

