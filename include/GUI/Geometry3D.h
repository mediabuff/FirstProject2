namespace System
{
namespace Gui
{
namespace Media3D
{

class GUIEXT Geometry3D : public DependencyObject
{
protected:
	CTOR Geometry3D(DependencyClass* depClass);

	virtual void AddShapeDesc(physx::PxScene* scene, physx::PxRigidActor* actorDesc)
	{
		// TODO abstract
	}

public:

	virtual void Render(ManagedRenderContext renderContext) abstract;

	DEPENDENCY_CLASS_ABSTRACT(Geometry3D, DependencyObject)
};

}	// Media3D
}	// Gui
}	// System
