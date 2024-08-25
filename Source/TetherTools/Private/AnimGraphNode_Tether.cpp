// Copyright (c) Jared Taylor. All Rights Reserved.

#include "AnimGraphNode_Tether.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimGraphNode_Tether)

#define LOCTEXT_NAMESPACE "A3Nodes"

FLinearColor UAnimGraphNode_Tether::GetNodeTitleColor() const
{
	return FLinearColor(1.f, 0.0f, 0.5f);  // Pinkish-Purple (Raspberry?)
}

FText UAnimGraphNode_Tether::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GetControllerDescription();
}

FText UAnimGraphNode_Tether::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_Tether_Tooltip", "Modular framework for integrating physics solvers and constraints into animation graph. Ideal for simulating objects attached to characters, such as accessories or equipment, with customizable and extendable options for realistic, physics-driven animations.");
}

FString UAnimGraphNode_Tether::GetNodeCategory() const
{
	return TEXT("Tether");
}

FText UAnimGraphNode_Tether::GetControllerDescription() const
{
	return LOCTEXT("Tether", "Tether");
}

#undef LOCTEXT_NAMESPACE
