#pragma once
#include "AbstractBlock.h"
class QuestionBlock : public AbstractBlock
{
public:
	void Start() override;
protected:
	void InitAnimation() override;
};

