// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoriaDeveloperSettings.h"

UMemoriaDeveloperSettings::UMemoriaDeveloperSettings(const FObjectInitializer& ObjectInitializer)
{
	typedef ETeamAttitude::Type EA;
	TeamAttitudes = {
		{EA::Friendly, EA::Neutral,  EA::Neutral,  EA::Neutral,  EA::Neutral },//Neutral
		{EA::Neutral, EA::Friendly, EA::Hostile,  EA::Friendly, EA::Hostile},//Team1
		{EA::Neutral, EA::Hostile,  EA::Friendly, EA::Hostile,  EA::Hostile},//Team2
		{EA::Neutral, EA::Friendly, EA::Hostile,  EA::Friendly, EA::Friendly },//Team3
		{EA::Neutral, EA::Hostile,  EA::Hostile,  EA::Friendly, EA::Friendly }//Team4
	};
}

const UMemoriaDeveloperSettings* UMemoriaDeveloperSettings::Get()
{
	return GetDefault<UMemoriaDeveloperSettings>();
}

ETeamAttitude::Type UMemoriaDeveloperSettings::GetAttitude(FGenericTeamId Of, FGenericTeamId Towards)
{
	const TArray<FTeamAttitude>& teamAttitudes = UMemoriaDeveloperSettings::Get()->TeamAttitudes;
	bool ofValid = teamAttitudes.IsValidIndex(Of.GetId());
	bool towardsValid = teamAttitudes.IsValidIndex(Towards.GetId());

	if (ofValid && towardsValid) {
		const TArray<TEnumAsByte<ETeamAttitude::Type>>& ofAttitudes = teamAttitudes[Of.GetId()].Attitude;
		if (ofAttitudes.IsValidIndex(Towards.GetId())) {
			return ofAttitudes[Towards.GetId()];
		}
	}

	return ETeamAttitude::Neutral;
}
