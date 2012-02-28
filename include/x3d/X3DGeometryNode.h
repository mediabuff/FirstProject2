namespace System
{
namespace x3d
{

class X3DEXT X3DGeometryNode : public X3DChildNode
{
protected:
	CTOR X3DGeometryNode(NodeType* nodeType) : X3DChildNode(nodeType)
	{
	}

public:

	virtual void AddShapeDesc(physx::PxRigidActor* actor)
	{
		// TODO abstract
	}

	static NodeType* GetNodeType();
};

}	// x3d
}	// System
