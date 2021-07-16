// Copy-right is reserved by iqiyi VR-group

#pragma once

#include "CoreMinimal.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "ComponentAttachTrack/ComponentAttachSection.h"
#include "ComponentAttachEvalTemplate.generated.h"


/**
 * 
 */
USTRUCT()
struct FComponentAttachEvalTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()
public:
	FComponentAttachEvalTemplate();
	FComponentAttachEvalTemplate(const UComponentAttachSection* InSection);

	// 运行时播放得计算处理
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void SetupOverrides() override { EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag); }

	UPROPERTY()
	const UComponentAttachSection* Section = nullptr;
	UPROPERTY(Transient)
	const UObject* PlayerObj = nullptr;
};
