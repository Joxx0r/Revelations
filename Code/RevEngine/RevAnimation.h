#ifndef _REV_ANIMATION_H
#define _REV_ANIMATION_H

#include <vector>

class RevTransformationNode;

class RevAnimation
{
public:
	RevAnimation(void);
	virtual ~RevAnimation(void);

	void Init(const int& nrOfExpectedBones);
	void AddTransformationNode(RevTransformationNode* node);

	std::vector< RevTransformationNode* >* GetRevTransformationNodes();

protected:
	std::vector< RevTransformationNode* > m_transformationNodes;
};

#endif