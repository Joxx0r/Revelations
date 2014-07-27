#ifndef _REV_BONE_INSTANCE_NODE_H
#define _REV_BONE_INSTANCE_NODE_H

class RevAnimationInstanceNode;
class RevBone;

class RevBoneInstanceNode
{
public:
	RevBoneInstanceNode(void);
	virtual ~RevBoneInstanceNode(void);

	void Update( const CU::Matrix44f& parent );
	void UpdateBindPose();
		
	void SetBoneID( const int& boneID );
	void SetAnimationInstanceNode( RevAnimationInstanceNode* node );
	void SetBone( RevBone* bone );

	CU::Matrix44f GetTransformation();
	CU::Matrix44f GetObjectSpaceTransformation();
private:

	int m_boneID;
	CU::Matrix44f m_transformation;
	CU::Matrix44f m_objectSpaceTransformation;
	RevBone* m_bone;

	RevAnimationInstanceNode* m_revAnimationInstance;
};

#endif