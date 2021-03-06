namespace System
{
namespace x3d
{

class X3DEXT RigidBody : public X3DNode
{
public:

	CTOR RigidBody();

	X3DSFIELD(SFBool, enabled, Enabled)
	X3DSFIELD(SFVec3f, linearVelocity, LinearVelocity)

		/*
	bool getEnabled()
	{
		return m_enabled->getValue();
	}

	void setEnabled(bool enabled)
	{
		return m_enabled->setValue(enabled);
	}
	*/

	/*
	Vec3f getLinearVelocity()
	{
		return m_linearVelocity->getValue();
	}
	*/

	Vec3f getPosition()
	{
		return m_position->getValue();
	}

	void setPosition(Vec3f position)
	{
		m_position->setValue(position);
	}

	Rotation getOrientation()
	{
		return m_orientation->getValue();
	}

	void setOrientation(Rotation orientation)
	{
		m_orientation->setValue(orientation);
	}

	SFRotation* getOrientationField()
	{
		return m_orientation;
	}

	Vec3f getAngularVelocity()
	{
		return m_angularVelocity->getValue();
	}

	Vec3f getCenterOfMass()
	{
		return m_centerOfMass->getValue();
	}

	float getMass()
	{
		return m_mass->getValue();
	}

	SFFloat* getMassField()
	{
		return m_mass;
	}

	MFNode* getGeometryField()
	{
		return m_geometry;
	}

	bool getUseGlobalGravity()
	{
		return m_useGlobalGravity->getValue();
	}

	bool getFixed()
	{
		return m_fixed->getValue();
	}

	void setFixed(bool fixed)
	{
		m_fixed->setValue(fixed);
	}

	SFBool* getFixedField()
	{
		return m_fixed;
	}

	void CreateActor(physx::PxScene* nxScene);

	physx::PxRigidActor* getActor() const throw()
	{
		return m_actor;
	}

	void UpdatePosOrient();

	/// <returns>The X3D NodeType</returns>
	X3DNODE(RigidBody, X3DNode)

protected:

	virtual void OnFieldChanged(X3DField* field) override;

private:

	physx::PxRigidActor* m_actor;

	SFFloat* m_angularDampingFactor;// 0.001   [0,1]
	SFVec3f* m_angularVelocity;//      0 0 0   (-?,?)
	SFBool* m_autoDamp;//             FALSE
	SFBool* m_autoDisable;//          FALSE
	SFVec3f* m_centerOfMass;//         0 0 0   (-?,?)
	SFFloat* m_disableAngularSpeed;//  0       [0,?)
	SFFloat* m_disableLinearSpeed;//   0       [0,?)
	SFFloat* m_disableTime;//          0       [0,?)
//	SFBool* m_enabled;//              TRUE
	SFVec3f* m_finiteRotationAxis;//   0 0 0   [-1,1]
	SFBool* m_fixed;//                FALSE
	MFVec3f* m_forces;//               []
	MFNode* m_geometry;//             []      [X3DNBodyCollidableNode]
#if 0
	SFMatrix3f* m_inertia;//	           1 0 0
						//				   0 1 0
						//				   0 0 1
#endif
	SFFloat* m_linearDampingFactor;//  0.001   [0,1]
//	SFVec3f* m_linearVelocity;//       0 0 0   (-?,?)
	SFFloat* m_mass;//                 1       (0,?)

	union MassDensiteModel
	{
		Sphere* m_Sphere;
		Cone* m_Cone;
		Box* m_Box;

		operator X3DGeometryNode* ()
		{
			return m_Sphere;
		}
	};

	TypedSFNode<MassDensiteModel, X3DGeometryNode*>* m_massDensityModel;	// NULL    [Sphere, Box, Cone]
	SFRotation* m_orientation;//          0 0 1 0 [0,1]
	SFVec3f* m_position;//             0 0 0   (-?,?)
	MFVec3f* m_torques;//              []
	SFBool* m_useFiniteRotation;//    FALSE
	SFBool* m_useGlobalGravity;//     TRUE

	bool m_globalPoseValid : 1;

	/*
  SFFloat    [in,out] angularDampingFactor 0.001   [0,1]
  SFVec3f    [in,out] angularVelocity      0 0 0   (-?,?)
  SFBool     [in,out] autoDamp             FALSE
  SFBool     [in,out] autoDisable          FALSE
  SFVec3f    [in,out] centerOfMass         0 0 0   (-?,?)
  SFFloat    [in,out] disableAngularSpeed  0       [0,?)
  SFFloat    [in,out] disableLinearSpeed   0       [0,?)
  SFFloat    [in,out] disableTime          0       [0,?)
  SFBool     [in,out] enabled              TRUE
  SFVec3f    [in,out] finiteRotationAxis   0 0 0   [-1,1]
  SFBool     [in,out] fixed                FALSE
  MFVec3f    [in,out] forces               []
  MFNode     [in,out] geometry             []      [X3DNBodyCollidableNode]
  SFMatrix3f [in,out] inertia	           1 0 0
                                           0 1 0
                                           0 0 1
  SFFloat    [in,out] linearDampingFactor  0.001   [0,1]
  SFVec3f    [in,out] linearVelocity       0 0 0   (-?,?)
  SFFloat    [in,out] mass                 1       (0,?)
  SFNode     [in,out] massDensityModel     NULL    [Sphere, Box, Cone]
  SFNode     [in,out] metadata             NULL    [X3DMetadataObject]
  SFRotation [in,out] orientation          0 0 1 0 [0,1]
  SFVec3f    [in,out] position             0 0 0   (-?,?)
  MFVec3f    [in,out] torques              []
  SFBool     [in,out] useFiniteRotation    FALSE
  SFBool     [in,out] useGlobalGravity     TRUE
  */
};

}	// x3d
}	// System
