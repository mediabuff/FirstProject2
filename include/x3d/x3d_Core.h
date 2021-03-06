#ifdef __LERSTAD__
#import "GUI/GUI.h"
#else
#include "GUI/GUI.h"
#endif

#ifdef __LERSTAD__
#import "ZLIBDeflate/ZLIBDeflate.h"
#else
#include "ZLIBDeflate/ZLIBDeflate.h"
#endif

/*
#ifdef __LERSTAD__
#import "LMedia/LMedia.h"
#else
#include "LMedia/LMedia.h"
#endif
*/

#ifdef __LERSTAD__
#import "Networking/Networking.h"
#else
#include "Networking/Networking.h"
#endif

#ifndef X3DEXT
#define X3DEXT DECLSPEC_DLLIMPORT
#endif

#ifndef X3D_COMPONENT_GROUPING
#define X3D_COMPONENT_GROUPING 1
#endif

#ifndef X3D_COMPONENT_LIGHTING
#define X3D_COMPONENT_LIGHTING 1
#endif

#ifndef X3D_COMPONENT_NAVIGATION
#define X3D_COMPONENT_NAVIGATION 1
#endif

#ifndef X3D_COMPONENT_ENVIRONMENTAL_EFFECTS
#define X3D_COMPONENT_ENVIRONMENTAL_EFFECTS 1
#endif

#ifndef X3D_COMPONENT_INTERPOLATION
#define X3D_COMPONENT_INTERPOLATION 1
#endif

#ifndef X3D_COMPONENT_SOUND
#define X3D_COMPONENT_SOUND 1
#endif

#ifndef X3D_COMPONENT_TEXTURING
#define X3D_COMPONENT_TEXTURING 1
#endif

#ifndef X3D_COMPONENT_TEXT
#define X3D_COMPONENT_TEXT 1
#endif

#ifndef X3D_COMPONENT_GEOMETRY2D
#define X3D_COMPONENT_GEOMETRY2D 1
#endif

#ifndef X3D_COMPONENT_GEOMETRY3D
#define X3D_COMPONENT_GEOMETRY3D 1
#endif

#ifndef X3D_COMPONENT_LAYOUT
#define X3D_COMPONENT_LAYOUT 1
#endif

#ifndef X3D_COMPONENT_LAYERING
#define X3D_COMPONENT_LAYERING 1
#endif

#ifndef X3D_COMPONENT_PROGRAMMABLE_SHADERS
#define X3D_COMPONENT_PROGRAMMABLE_SHADERS 1
#endif

#ifndef X3D_COMPONENT_RIGIDBODY
#define X3D_COMPONENT_RIGIDBODY 1
#endif

#ifndef X3D_COMPONENT_BIFS
#define X3D_COMPONENT_BIFS 1
#endif

#include "Quaternion.h"

namespace System
{
namespace Graphics
{
	class Graphics3D;
	class GraphicsO_3D;

#define TEXTURES_MAX	32

struct __DECLSPEC_ALIGN_16__ Vertex
{
public:
	gm::vector3f p;
	float w;

//	vector4f win;

	gm::vector4f texcoord[TEXTURES_MAX];
	/*
	vector4f ambient;
	vector4f emission;
	vector4f diffuse;
	vector4f specular;
	*/
	gm::vector4f color;
	gm::vector4f secondarycolor;
	gm::vector3f normal;
	gm::vector2f m_winxy;
	float m_winz;
//	float m_shininess;
	float m_fog_f;
	float w_inv;
//	bool lightingEnabled;
	uint m_viewclipmask;
};
}
}

#ifdef __LERSTAD__

struct X3DAUDIO_LISTENER
{
    float OrientFront[3]; // orientation of front direction, used only for matrix and delay calculations or listeners with cones for matrix, LPF (both direct and reverb paths), and reverb calculations, must be normalized when used
	float OrientTop[3];   // orientation of top direction, used only for matrix and delay calculations, must be orthonormal with OrientFront when used

    float Position[3]; // position in user-defined world units, does not affect Velocity
    float Velocity[3]; // velocity vector in user-defined world units/second, used only for doppler calculations, does not affect Position

    void* pCone; // sound cone, used only for matrix, LPF (both direct and reverb paths), and reverb calculations, NULL specifies omnidirectionality
};

#endif

#ifndef __LERSTAD__
#undef new
#include <map>
#define new _new
#endif

namespace System
{

	/*
class UndoManager
{
public:

};

class UndoUnit : public DependencyObject
{
public:

};

interface IUndoableObject
{
};
*/

template<class T>
IO::TextWriter& WriteToStream(IO::TextWriter& stream, const gm::vector2<T>& v)
{
	stream << v[0] << "," << v[1];
	return stream;
}

template<class T>
IO::TextWriter& WriteToStream(IO::TextWriter& stream, const gm::vector3<T>& v)
{
	stream << v[0] << "," << v[1] << "," << v[2];
	return stream;
}

template<class T>
IO::TextWriter& WriteToStream(IO::TextWriter& stream, const gm::vector4<T>& v)
{
	stream << v[0] << "," << v[1] << "," << v[2] << "," << v[3];
	return stream;
}

inline IO::TextWriter& WriteToStream(IO::TextWriter& stream, const AxisAngle& v)
{
	stream << v.m_axis[0] << "," << v.m_axis[1] << "," << v.m_axis[2] << "," << v.m_angle;
	return stream;
}

inline IO::TextWriter& WriteToStream(IO::TextWriter& stream, const vector<int>& v)
{
	ASSERT(0);
	return stream;
}

inline IO::TextWriter& WriteToStream(IO::TextWriter& stream, const vector<float>& v)
{
	ASSERT(0);
	return stream;
}

inline IO::TextWriter& WriteToStream(IO::TextWriter& stream, const vector<double>& v)
{
	ASSERT(0);
	return stream;
}

inline IO::TextWriter& WriteToStream(IO::TextWriter& stream, const vector<String>& v)
{
	ASSERT(0);
	return stream;
}

/// X3D namespace
namespace x3d
{

// Forward Declarations

interface X3DMetadataObject;

class SFRotation;
class SFBool;
class SFFloat;
class SFVec3f;
class SFColor;
class SFString;
class MFFloat;
class MFNode;
class MFString;
class MFURL;

class Route;
class NodeType;

class X3DScene;
class X3DNode;
class X3DField;
class X3DShaderNode;
class X3DColorNode;
class X3DCoordinateNode;
class X3DNormalNode;
class X3DTextureCoordinateNode;
class X3DPrototypeInstance;
class X3DSoundSourceNode;
class X3DRigidJointNode;
class Box;
class Cone;
class CollisionCollection;
class Group;
class Sound;
class QuantizationParameter;
class TouchSensor;
class Shape;
class Sphere;
class ShaderProgram;
class Viewpoint;
class NavigationInfo;
class Fog;
class Contact;
class RigidBody;
class WorldInfo;
class X3DLayerNode;
class Layer;
class LayerSet;

class SceneVisual3D;

///////////////////////
typedef gm::vector2f Vec2f;
typedef gm::vector3f Vec3f;
typedef gm::vector4f Vec4f;

typedef gm::vector2d Vec2d;
typedef gm::vector3d Vec3d;
typedef gm::vector4d Vec4d;

typedef gm::vector3f float3;
typedef gm::vector4f float4;

typedef AxisAngle Rotation;

}

inline IO::TextWriter& WriteToStream(IO::TextWriter& stream, const vector<x3d::Vec2f>& v)
{
	ASSERT(0);
	return stream;
}

inline IO::TextWriter& WriteToStream(IO::TextWriter& stream, const vector<x3d::Vec3f>& v)
{
	ASSERT(0);
	return stream;
}

inline IO::TextWriter& WriteToStream(IO::TextWriter& stream, const vector<x3d::X3DNode*>& v)
{
	ASSERT(0);
	return stream;
}

namespace x3d
{

class X3DEXT BoundingBox
{
public:

	CTOR BoundingBox() throw() : m_size(Vec3f(-1,-1,-1))
	{
	}

	CTOR BoundingBox(Vec3f center, Vec3f size) throw() : m_center(center), m_size(size)
	{
	}

	bool IsValid() const throw()
	{
		return m_size[0] >= 0;// && m_size[1] >= 0 && m_size[2] >= 0;
	}

	Vec3f m_center;
	Vec3f m_size;
};

#define X3DSFIELD(type, name, uname)	\
private:	\
	type* m_##name;	\
public:	\
	static TypedX3DFieldDefinition<type>* get_##name##FieldDef();	\
	type::value_t get##uname()	\
	{	\
		return m_##name->getValue();	\
	}	\
	\
	void set##uname(type::value_t value)	\
	{	\
		m_##name->setValue(value);	\
	}	\
	\
	type* get##uname##Field()	\
	{	\
		return m_##name;	\
	}

#define X3DSFIELD_ABSTRACT(type, name, uname)	\
public:	\
	virtual type::value_t get##uname() abstract; \
	virtual void set##uname(type::value_t value) abstract; \
	virtual type* get##uname##Field() abstract;

#define X3DSFIELD_OVERRIDE(type, name, uname)	\
private:	\
	type* m_##name;	\
public:	\
	static TypedX3DFieldDefinition<type>* get_##name##FieldDef();	\
	virtual type::value_t get##uname()  override \
	{	\
		return m_##name->getValue();	\
	}	\
	\
	virtual void set##uname(type::value_t value) override \
	{	\
		m_##name->setValue(value);	\
	}	\
	\
	virtual type* get##uname##Field() override	\
	{	\
		return m_##name;	\
	}

#define X3DMFIELD(type, name, uname)	\
private:	\
	type* m_##name;	\
public:	\
	static TypedX3DFieldDefinition<type>* get_##name##FieldDef();	\
	type* get##uname##Field()	\
	{	\
		return m_##name;	\
	}

#define X3DMFIELD_ABSTRACT(type, name, uname)	\
public:	\
	virtual type* get##uname##Field() abstract;

#define X3DMFIELD_OVERRIDE(type, name, uname)	\
private:	\
	type* m_##name;	\
public:	\
	static TypedX3DFieldDefinition<type>* get_##name##FieldDef();	\
	virtual type* get##uname##Field() override	\
	{	\
		return m_##name;	\
	}

#define IMP_X3DFIELD0(classname, type, fieldname, access)	\
TypedX3DFieldDefinition<type>* classname::get_##fieldname##FieldDef()	\
{	\
	static TypedX3DFieldDefinition<type> fieldname(#fieldname, SAIFieldAccess_##inputOutput, offsetof(classname, m_##fieldname));	\
	return &fieldname;	\
}

#define IMP_X3DFIELD1(classname, type, fieldname, access, defaultvalue)	\
TypedX3DFieldDefinition<type>* classname::get_##fieldname##FieldDef()	\
{	\
	static TypedX3DFieldDefinition<type> fieldname(#fieldname, SAIFieldAccess_##inputOutput, offsetof(classname, m_##fieldname), defaultvalue);	\
	return &fieldname;	\
}

#define IMP_X3DFIELD3(classname, type, fieldname, access, defaultvalue, minvalue, maxvalue)	\
TypedX3DFieldDefinition<type>* classname::get_##fieldname##FieldDef()	\
{	\
	static TypedX3DFieldDefinition<type> fieldname(#fieldname, SAIFieldAccess_##inputOutput, offsetof(classname, m_##fieldname), defaultvalue, minvalue, maxvalue);	\
	return &fieldname;	\
}

#define X3DNODE_ABSTRACT(name, basename)	\
	static NodeType* GetNodeType();	typedef name thisClass; typedef basename baseClass;

#define X3DNODE(name, basename)	\
	private: static NodeType* nodeType;	\
	public:	static NodeType* GetNodeType();	typedef name thisClass; typedef basename baseClass;

enum FieldType
{
	FieldType_SFBool = 1,
	FieldType_MFBool = 2,

	FieldType_MFInt32 = 3,
	FieldType_SFInt32 = 4,

	FieldType_SFFloat = 7,
	FieldType_MFFloat = 8,

	FieldType_SFDouble = 9,
	FieldType_MFDouble = 10,

	FieldType_SFTime = 11,
	FieldType_MFTime = 12,

	FieldType_SFNode = 13,
	FieldType_MFNode = 14,

	FieldType_SFVec2f = 15,
	FieldType_MFVec2f = 16,

	FieldType_SFVec2d = 17,
	FieldType_MFVec2d = 18,

	FieldType_SFVec3f = 19,
	FieldType_MFVec3f = 20,

	FieldType_SFVec3d = 21,
	FieldType_MFVec3d = 22,

	FieldType_MFRotation = 23,
	FieldType_SFRotation = 24,

	FieldType_MFColor = 25,
	FieldType_SFColor = 26,

	FieldType_MFColorRGBA = 27,
	FieldType_SFColorRGBA = 28,

	FieldType_SFImage = 29,
	FieldType_MFImage = 30,

	FieldType_SFString = 31,
	FieldType_MFString = 32,

	// ??
	FieldType_SFURL	= 35,
	FieldType_MFURL	= 36,

	FieldType_SFMatrix3f,
	FieldType_MFMatrix3f,

	FieldType_SFMatrix4f,
	FieldType_MFMatrix4f,

	FieldType_SFVec4f,
	FieldType_MFVec4f,

	FieldType_SFVec4d,
	FieldType_MFVec4d,
};

X3DEXT String ToString(FieldType fieldtype);
X3DEXT bool IsSField(FieldType fieldType);

inline bool IsMField(FieldType fieldType)
{
	return !IsSField(fieldType);
}

enum SAIFieldAccess
{
	SAIFieldAccess_initializeOnly = 0,
	SAIFieldAccess_inputOnly = 1,
	SAIFieldAccess_outputOnly = 2,
	SAIFieldAccess_inputOutput = 3
};

enum EncodingID
{
	Encoding_Scripted = 1,
	Encoding_ASCII = 2,
	Encoding_VRML = 3,
	Encoding_XML = 4,
	Encoding_Binary = 5,
	Encoding_BIFS = 6
};

interface IX3DFieldDefinition
{
	virtual String getName() abstract;
	virtual SAIFieldAccess getAccessType() abstract;
	virtual FieldType getFieldType() abstract;
	virtual String getFieldTypeString() abstract;
};

}	// x3d
}	// System

#include "X3DFieldDefinition.h"

namespace System
{
namespace x3d
{

class X3DEXT X3DFieldDefinitions : public Object
{
public:
	CTOR X3DFieldDefinitions(uint count)
	{
		m_items.reserve(count);
	}

	CTOR X3DFieldDefinitions(X3DFieldDefinition** fields, uint count)
	{
		m_items.reserve(count);
		for (size_t i = 0; i < count; ++i)
		{
			m_items.push_back(fields[i]);
		}
	//	m_items.insert(m_items.begin(), fields, fields+count);
	}

	size_t get_length() const
	{
		return m_items.size();
	}

	X3DFieldDefinition* item(size_t index) const
	{
		if (index < m_items.size())
		{
			return m_items[index];
		}
		else
		{
			return nullptr;
		}
	}

protected:

	friend class NodeType;

	vector<X3DFieldDefinition*> m_items;
};

}	// x3d
}	// System

#include "NodeType.h"
#include "X3DField.h"

namespace System
{
namespace x3d
{

template<class T, int fieldType>
class SFField : public X3DField
{
public:

	typedef SFField<T, fieldType> t;

	virtual FieldType getFieldType() const override
	{
		return fieldType;
	}

	virtual void CopyFrom(X3DField* pFrom) override
	{
		m_value = static_cast<t*>(pFrom)->m_value;
	}

	T getValue() const throw()
	{
		return m_value;
	}
	void setValue(T newVal)
	{
		m_value = newVal;
		// TODO event
	}

public:

	T m_value;
};

class X3DEXT X3DArrayField : public X3DField
{
public:

	CTOR X3DArrayField(X3DNode* ownerNode) : X3DField(ownerNode)
	{
	}

	CTOR X3DArrayField(X3DFieldDefinition* def, X3DNode* ownerNode) : X3DField(def, ownerNode)
	{
	}
	 
	virtual IO::TextWriter& WriteValueToStream(IO::TextWriter& stream) override
	{
		stream << '[';
		WriteValuesToStream(stream);
		stream << ']';
		return stream;
	}

	virtual IO::TextWriter& WriteValuesToStream(IO::TextWriter& stream)
	{
		ASSERT(0);
		return stream;
	}

	virtual size_t get_size() const throw() = 0;
};

#if 0
class Face
{
public:

	CTOR Face(int v0, int v1, int v2);

	void CalcMinZ(LDraw::Vertex* vertices);

	int vertexIndex[3];

	float minz;
	float maxz;

	/*
	inline LDraw::vector3f CalcMidpoint() const
	{
		float x = (vertex[0][0] + vertex[1][0] + vertex[2][0]) / 3;
		float y = (vertex[0][1] + vertex[1][1] + vertex[2][1]) / 3;
		float z = (vertex[0][2] + vertex[1][2] + vertex[2][2]) / 3;
	}
	*/
};
#endif

class X3DEXT X3DDrawContext : public Object
{
public:
	CTOR X3DDrawContext();

	Gui::ImmediateRenderContext* m_renderContext;
	vector<TouchSensor*> m_namedNodes;
	X3DShaderNode* m_shader;

	X3DAUDIO_LISTENER m_listener;

//	vector<uint> m_indices;

	stack<gm::matrix4f> m_transformStack;

	gm::matrix4f m_model;
	gm::matrix4f m_view;
	gm::matrix4f m_projection;
	gm::matrix4f m_modelView;
	gm::matrix4f m_modelViewProjection;

	gm::matrix4f m_modelI;
	gm::matrix4f m_viewI;
	gm::matrix4f m_projectionI;
	gm::matrix4f m_modelViewI;
	gm::matrix4f m_modelViewProjectionI;

	gm::matrix4f m_modelT;
	gm::matrix4f m_viewT;
	gm::matrix4f m_projectionT;
	gm::matrix4f m_modelViewT;
	gm::matrix4f m_modelViewProjectionT;

	gm::matrix4f m_modelIT;
	gm::matrix4f m_viewIT;
	gm::matrix4f m_projectionIT;
	gm::matrix4f m_modelViewIT;
	gm::matrix4f m_modelViewProjectionIT;

	float m_zNear;
	float m_zFar;

	int m_nName;

	unsigned int m_glFixedVertexShader;
	unsigned int m_glFixedFragmentShader;

	unsigned int m_glFixedShaderProgram;

	float m_clientWidth;
	float m_clientHeight;
};

interface ISizeable3f
{
	virtual Vec3f getSize3f() abstract;
	virtual void setSize3f(Vec3f size) abstract;
};

interface IPositionable2f
{
	virtual Vec2f getPosition2f() abstract;
	virtual void setPosition2f(Vec2f position) abstract;
};

interface IPositionable3f : public IPositionable2f
{
	virtual Vec3f getPosition3f() abstract;
	virtual void setPosition3f(Vec3f position) abstract;

	virtual Vec2f getPosition2f() override
	{
		Vec3f v = getPosition3f();
		return Vec2f(v[0], v[1]);
	}

	virtual void setPosition2f(Vec2f position) override
	{
		setPosition3f(position);
	}
};

interface IHitTest
{
	virtual void OnHit(X3DNode* node, int type, Vec3f v) abstract;
	virtual void OnHit(X3DNode* node, int type, Vec3f localpoint, int axis, double t) abstract;
};

class RenderImplImpl
{
public:
	virtual void PreDraw(X3DDrawContext* pDC)
	{
	}

	virtual void Draw(X3DDrawContext* pDC)
	{
	}

	virtual void DrawGlobalLights(X3DDrawContext* pDC)
	{
	}

	virtual void DrawOverlay(X3DDrawContext* pDC)
	{
	}

	virtual int HitTest(const D3DXMATRIX& projection, const D3DXMATRIX& view, const D3DXMATRIX& model, const D3D10_VIEWPORT& vp, int type, Vec2f mousepos, Vec3f pos, IHitTest* hitTest)
	{
		return 0;
	}

	virtual int Moveability()
	{
		// 0=none, 2=2d,3=3d
		return 0;
	}

	virtual int Resizability()
	{
		// 0=none,1=uniform,2=2d,3=3d
		return 0;
	}

	virtual D3DXMATRIX GetTransform()
	{
		return D3DXMATRIX(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
};

class X3DEXT X3DException : public Exception
{
public:
	CTOR X3DException(StringIn str) : Exception(str)
	{
	}

	_EXCEPTION
};

class X3DEXT InvalidNodeException : public X3DException
{
public:
	CTOR InvalidNodeException(StringIn nodename) : X3DException("Invalid node " + nodename), m_nodename(nodename)
	{
	}

	String m_nodename;

	_EXCEPTION
};

interface X3DTextureNodeImplImpl
{
	virtual void CreateTexture(Graphics::RenderTarget* rt) = 0;
	virtual void ApplyTexture(Gui::ImmediateRenderContext* pGraphics) = 0;

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
//	virtual void* GetData() = 0;
};

interface IGeometryRender
{
	virtual void Draw(X3DDrawContext* pDC, X3DTextureNodeImplImpl* pTextureNode) = 0;
//	virtual bool Within(const LDraw::vector3f& start, const LDraw::vector3f& end, LDraw::vector3f& plane_normal, LDraw::vector3f& pt, double& d) = 0;
};

class X3DEXT X3DBoundedImplImpl
{
public:
	void DrawBoundingBox(X3DDrawContext* pDC, BoundingBox bbox);
	virtual BoundingBox CalculateBoundingBox(gm::matrix4f& transform) = 0;

	BoundingBox m_computedBBox;

	bool m_computedBBoxValid;
};

}	// x3d
}	// System

#include "X3DMetadataObject.h"
#include "SFNode.h"
#include "X3DNode.h"
#include "X3DChildNode.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "SFFloat.h"
#include "SFDouble.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "SFVec4f.h"
#include "SFRotation.h"
#include "SFTime.h"
#include "SFColor.h"
#include "SFColorRGBA.h"
#include "SFString.h"
#include "SFImage.h"
#include "MFBool.h"
#include "MFInt32.h"
#include "MFNode.h"
#include "MFVec2f.h"
#include "MFVec3f.h"
#include "MFFloat.h"
#include "MFDouble.h"
#include "MFRotation.h"
#include "MFColor.h"
#include "MFString.h"

namespace System
{
namespace x3d
{

#ifndef __LERSTAD__

template<> IO::TextWriter& TypedX3DFieldDefinition<SFInt32>::WriteDomain(IO::TextWriter& stream)
{
	if (m_minValue == INT_MIN)
		stream << "_";
	else
		stream << m_minValue;

	stream << ",";

	if (m_maxValue == INT_MAX)
		stream << "_";
	else
		stream << m_maxValue;

	return stream;
}

template<> IO::TextWriter& TypedX3DFieldDefinition<SFFloat>::WriteDomain(IO::TextWriter& stream)
{
	if (m_minValue == M_PI)
		stream << "�";
	else if (m_minValue == -M_PI)
		stream << "-�";
	else if (m_minValue == FLT_MIN)
		stream << "_";
	else
		stream << m_minValue;

	stream << ",";

	if (m_maxValue == M_PI)
		stream << "�";
	else if (m_maxValue == FLT_MAX)
		stream << "_";
	else
		stream << m_maxValue;

	return stream;
}

template<> IO::TextWriter& TypedX3DFieldDefinition<SFDouble>::WriteDomain(IO::TextWriter& stream)
{
	if (m_minValue == M_PI)
		stream << "�";
	else if (m_minValue == -M_PI)
		stream << "-�";
	else if (m_minValue == DBL_MIN)
		stream << "_";
	else
		stream << m_minValue;

	stream << ",";

	if (m_maxValue == M_PI)
		stream << "�";
	else if (m_maxValue == -M_PI)
		stream << "-�";
	else if (m_maxValue == DBL_MAX)
		stream << "_";
	else
		stream << m_maxValue;

	return stream;
}

#endif

/*
class X3DGeometryNode;

template<>
Stream& TypedX3DFieldDefinition<TypedSFNode<X3DGeometryNode*> >::WriteDomain(Stream& stream)
{
	stream << "Geometry";
	return stream;
}
*/

interface X3DBoundedObject
{
	X3DSFIELD_ABSTRACT(SFVec3f, bboxCenter, BBoxCenter)
	X3DSFIELD_ABSTRACT(SFVec3f, bboxSize, BBoxSize)

	virtual BoundingBox CalculateBoundingBox(gm::matrix4f& transform) abstract;

//	virtual SFVec3f* getBBoxCenterField() abstract;
//	virtual SFVec3f* getBBoxSizeField() abstract;
};

}
}

#include "X3DGroupingNode.h"
#include "X3DInfoNode.h"
#include "X3DUrlObject.h"
#include "X3DBindableNode.h"
#include "X3DGeometryNode.h"
#include "X3DViewpointNode.h"
#include "Sphere.h"
#include "X3DBackgroundNode.h"

namespace System
{
namespace x3d
{

class X3DEXT X3DTimeDependentNode : public X3DChildNode
{
protected:

	CTOR X3DTimeDependentNode(NodeType* nodeType);

	X3DNODE_ABSTRACT(X3DTimeDependentNode, X3DChildNode)

protected:

	X3DSFIELD(SFTime, startTime, StartTime)
	X3DSFIELD(SFTime, stopTime, StopTime)
	X3DSFIELD(SFTime, pauseTime, PauseTime)
	X3DSFIELD(SFTime, resumeTime, ResumeTime)
	X3DSFIELD(SFTime, elapsedTime, ElapsedTime)
	X3DSFIELD(SFBool, loop, Loop)
	X3DSFIELD(SFBool, isActive, IsActive)

	/*
	SFTime* m_startTime;
	SFTime* m_stopTime;
	SFTime* m_pauseTime;
	SFTime* m_resumeTime;
	SFTime* m_elapsedTime;
	SFBool* m_loop;
	SFBool* m_isActive;
	*/
};

class X3DEXT X3DSensorNode : public X3DChildNode
{
protected:
	CTOR X3DSensorNode(NodeType* nodeType) : X3DChildNode(nodeType)
	{
	}
};

class X3DEXT X3DPointingDeviceSensorNode : public X3DSensorNode
{
protected:
	CTOR X3DPointingDeviceSensorNode(NodeType* nodeType) : X3DSensorNode(nodeType)
	{
	}
};

class X3DEXT X3DTouchSensorNode : public X3DPointingDeviceSensorNode
{
protected:
	CTOR X3DTouchSensorNode(NodeType* nodeType) : X3DPointingDeviceSensorNode(nodeType)
	{
	}
};

class X3DEXT X3DEnvironmentalSensorNode : public X3DSensorNode
{
protected:
	CTOR X3DEnvironmentalSensorNode(NodeType* nodeType) : X3DSensorNode(nodeType)
	{
	}
};

class X3DEXT X3DSoundNode : public X3DChildNode
{
protected:
	CTOR X3DSoundNode(NodeType* nodeType) : X3DChildNode(nodeType)
	{
	}

public:

	virtual X3DSoundSourceNode* getSource();
	virtual void setSource(X3DSoundSourceNode* source);

	virtual void SoundSourceUrlChanged() abstract;

	typedef X3DChildNode baseClass;

protected:

	virtual void OnFieldChanged(X3DField* field) override;

	X3DSoundSourceNode* m_currentSource;
	TypedSFNode<X3DSoundSourceNode*>* m_source;
	IXAudio2SourceVoice* m_sourceVoice;
};

class X3DEXT X3DSoundSourceNode : public X3DTimeDependentNode
{
protected:
	CTOR X3DSoundSourceNode(NodeType* nodeType) : X3DTimeDependentNode(nodeType)
	{
	}

public:

	typedef X3DTimeDependentNode baseClass;

	virtual void AddSoundNode(X3DSoundNode* pSound) abstract;
	virtual void RemoveSoundNode(X3DSoundNode* pSound) abstract;
};

interface IX3DAddField
{
	virtual void AddField(X3DField* field) abstract;
};

class X3DEXT X3DScriptNode : public X3DChildNode, public IX3DAddField
{
protected:
	CTOR X3DScriptNode(NodeType* nodeType) : X3DChildNode(nodeType)
	{
	}

	typedef X3DChildNode baseClass;
};

class X3DEXT X3DGeometricPropertyNode : public X3DNode
{
protected:
	CTOR X3DGeometricPropertyNode(NodeType* nodeType) : X3DNode(nodeType)
	{
	}

	static NodeType* GetNodeType();
};

class X3DEXT X3DVertexAttributeNode : public X3DGeometricPropertyNode
{
protected:
	CTOR X3DVertexAttributeNode(NodeType* nodeType);

public:
	String getName() throw()
	{
		return m_name->getValue();
	}

	static NodeType* GetNodeType();

	uint32 m_offsetInBytes;	// Offset into buffer

protected:

/*
	SFNode   [in,out] metadata NULL [X3DMetadataObject]
	SFString []       name     "" 
*/
	SFString* m_name;
};

class X3DEXT X3DNBodyCollidableNode : public X3DChildNode, public X3DBoundedObject
{
public:

	CTOR X3DNBodyCollidableNode(NodeType* nodeType);

	SFBool* getEnabledField()
	{
		return m_enabled;
	}

	Rotation getRotation()
	{
		return m_rotation->getValue();
	}

	void setRotation(Rotation v)
	{
		m_rotation->setValue(v);
	}

	SFRotation* getRotationField()
	{
		return m_rotation;
	}

	SFVec3f* getTranslationField()
	{
		return m_translation;
	}

	Vec3f getTranslation()
	{
		return m_translation->getValue();
	}

	void setTranslation(Vec3f v)
	{
		m_translation->setValue(v);
	}

	static NodeType* GetNodeType();

protected:

	SFBool* m_enabled;
	SFRotation* m_rotation;
	SFVec3f* m_translation;

	/*
	SFBool     [in,out] enabled     TRUE
	SFNode     [in,out] metadata    NULL     [X3DMetadataObject]
	SFRotation [in,out] rotation    0 0 1 0  [0,1]
	SFVec3f    [in,out] translation 0 0 0    (-?,?)
	SFVec3f    []       bboxCenter  0 0 0    (-?,?)
	SFVec3f    []       bboxSize    -1 -1 -1 [0,?) or -1 -1 -1
	*/
};

interface X3DFogObject
{
	virtual Vec3f getColor() abstract;
	virtual void setColor(Vec3f value) abstract;
	virtual SFColor* getColorField() abstract;

	virtual String getFogType() abstract;
	virtual void setFogType(StringIn value) abstract;
	virtual SFString* getFogTypeField() abstract;

	virtual float getVisibilityRange() abstract;
	virtual void setVisibilityRange(float visibilityRange) abstract;
	virtual SFFloat* getVisibilityRangeField() abstract;

	/*
  SFColor  [in,out] color           1 1 1    [0,1]
  SFString [in,out] fogType         "LINEAR" ["LINEAR"|"EXPONENTIAL"]
  SFFloat  [in,out] visibilityRange 0        [0,-?)
  */
};

}	// x3d
}	// System

#include "FloatVertexAttribute.h"
#include "Matrix3VertexAttribute.h"
#include "Matrix4VertexAttribute.h"

#include "X3DComposedGeometryNode.h"

namespace System
{
namespace x3d
{

class X3DEXT X3DParametricGeometryNode : public X3DGeometryNode
{
protected:
	CTOR X3DParametricGeometryNode(NodeType* nodeType) : X3DGeometryNode(nodeType)
	{
	}

	static NodeType* GetNodeType();
};

class X3DEXT X3DNurbsSurfaceGeometryNode : public X3DParametricGeometryNode
{
protected:
	CTOR X3DNurbsSurfaceGeometryNode(NodeType* nodeType) : X3DParametricGeometryNode(nodeType)
	{
	}

	static NodeType* GetNodeType();
};

class X3DEXT X3DCoordinateNode : public X3DGeometricPropertyNode
{
protected:
	CTOR X3DCoordinateNode(NodeType* nodeType) : X3DGeometricPropertyNode(nodeType)
	{
	}

	static NodeType* GetNodeType();
};

class X3DEXT X3DNormalNode : public X3DGeometricPropertyNode
{
protected:
	CTOR X3DNormalNode(NodeType* nodeType) : X3DGeometricPropertyNode(nodeType)
	{
	}
};

class X3DEXT X3DTextureCoordinateNode : public X3DGeometricPropertyNode
{
protected:
	CTOR X3DTextureCoordinateNode(NodeType* nodeType) : X3DGeometricPropertyNode(nodeType)
	{
	}
};

}	// x3d
}	// System

#include "X3DColorNode.h"
#include "X3DAppearanceNode.h"
#include "X3DShapeNode.h"

namespace System
{
namespace x3d
{

class X3DEXT MFURL : public X3DArrayField
{
public:
	CTOR MFURL(X3DNode* ownerNode) : X3DArrayField(ownerNode)
	{
	}

	virtual FieldType getFieldType() const throw()
	{
		return fieldtype;
	}

	virtual IO::TextWriter& WriteValuesToStream(IO::TextWriter& stream)
	{
		for (size_t i = 0; i < m_items.size(); ++i)
		{
			if (i > 0) stream << " ";
			stream << "\"" << m_items[i].string << "\"";
		}
		return stream;
	}

	class CURL
	{
	public:
		int od;
		String string;

		CTOR CURL()
		{
		}

		CTOR CURL(const CURL& other)
		{
			*this = other;
		}

		CURL& operator = (const CURL& other)
		{
			od = other.od;
			string = other.string;
			return *this;
		}
	};

	virtual void CopyFrom(X3DField* pFrom)
	{
		ASSERT(m_items.GetSize() == 0);

		for (unsigned int i = 0; i < static_cast<MFURL*>(pFrom)->m_items.GetSize(); i++)
		{
			CURL urlfrom = static_cast<MFURL*>(pFrom)->m_items[i];

		//	CURL url;
		//	url.od = urlfrom.od;
		//	url.string = urlfrom.string;

			m_items.Add(urlfrom);
		}
	}

	void append1Value(StringIn stringValue)
	{
		CURL value;
		value.od = 0;
		value.string = stringValue;
		m_items.Add(value);
	}

	void append1Value(long odValue, StringIn stringValue)
	{
		//if (value == NULL) return E_POINTER;	// Allow appending empty string??
		CURL value;
		value.od = odValue;
		value.string = stringValue;
		m_items.Add(value);
	}

	String get1Value(size_t index) const
	{
		return m_items[index].string;
	}

	void clear()
	{
		m_items.clear();
	}

// X3DArrayField
	size_t get_size() const throw()
	{
		return m_items.GetSize();
	}

	static const FieldType fieldtype = FieldType_MFURL;
	typedef vector<CURL> value_t;

public:

	vector<CURL> m_items;
};

/////////////////////////////////////////////////////////////////////////////

class SAIProtoDeclaration;

class X3DEXT X3DPrototypeInstance : public X3DNode
{
public:
	CTOR X3DPrototypeInstance() : X3DNode(nodeType),
		m_node(nullptr),
		m_protoDecl(nullptr)
	{
	}

	virtual X3DNode* GetNode()
	{
		return m_node;
	}

	void ReplaceISFieldsWithInstanceFields(X3DNode* pFrom, X3DNode* pNode);

	X3DNode* m_node;
	SAIProtoDeclaration* m_protoDecl;	// The proto declaration that instantiated us

	static NodeType* nodeType;
};

}	// x3d
}	// System

namespace System
{
namespace x3d
{

////////////////////////////////////////////////////////////////////
// SAI

	/*
	// Clone node, and replace fields with IS with the real values
CloneProtoNode(ILX3DNode* node)
{
	for (int i = 0; i < m_fields.GetSize(); i++)
	{
		if (CComQIPtr<CFieldValue>(m_fields[i])->m_is.Length())
		{
			m_
		}
	}
}
*/

//X3DField* CreateFieldOfType(FieldType fieldType, X3DNode* ownerNode);

class X3DEXT SAIProtoDeclaration : public Object
{
public:
	CTOR SAIProtoDeclaration()
	{
	}

	~SAIProtoDeclaration()
	{
		/*
		for (int i = 0; i < m_fields.GetSize(); i++)
		{
			delete m_fields[i];
		}
		*/
		m_fields.RemoveAll();
	}

	X3DPrototypeInstance* createInstance();

public:

	String m_name;

	vector<X3DField*> m_fields;
	vector<X3DNode*> m_nodes;
};

/*

interface IRouteDestination
{
	virtual void Set() = 0;
};

class X3DEXT RouteDestination : public Object, public IRouteDestination
{
public:

	CTOR RouteDestination(X3DNode* node, X3DField* field) :
		m_node(node),
		m_field(field)
	{
	}

	CTOR RouteDestination(X3DNode* node, StringIn fieldName) :
		m_node(node)
	{
		m_field = node->getField(fieldName);
	}

	virtual void Set() override;

	X3DField* m_field;
};

interface IRoute
{
};
*/

class X3DEXT FieldBinding : public Object, public Gui::IBinding
{
public:

	CTOR FieldBinding(X3DField* field) : m_field(field)
	{
	}

	virtual Type* GetOutputType();

	virtual Variant GetOutput(Gui::IBinding* caller)
	{
		ASSERT(0);
		return nullptr;
	}

	virtual int SetOutput(Gui::IBinding* caller, const Variant& value)
	{
		ASSERT(0);
		return 0;
	}

	virtual int SetInput(Gui::IBinding* caller, const Variant& value);

//	virtual Object* GetIn() = 0;

	virtual void* Connect(Gui::IInputBinding* binding)
	{
		return m_outBindings.insert(m_outBindings.end(), binding).m_ptr;
	}

	X3DField* m_field;
	list<Gui::IInputBinding*> m_outBindings;
};

class X3DEXT Route : public Object//, public IRoute
{
public:
	CTOR Route(Gui::IBinding* source, X3DField* destination) :
		m_source(source),
		m_destination(destination)
	{
		FieldBinding* destBinding = new FieldBinding(destination);
		source->Connect(destBinding);
	}

	/*
	X3DNode* getSourceNode() throw()
	{
		return m_sourceNode;
	}

	X3DField* getSourceField() throw()
	{
		return m_sourceField;
	}
	*/

	/*
	X3DNode* getDestinationNode() throw()
	{
		return m_destinationNode;
	}

	String getDestinationField() const throw()
	{
		return m_destinationField;
	}
	*/

	Gui::IBinding* m_source;
//	X3DNode* m_sourceNode;
	X3DField* m_destination;

//	IRouteDestination* m_destination;
};

}	// x3d
}	// System

#include "X3DLightNode.h"
#include "X3DAppearanceChildNode.h"

namespace System
{
namespace x3d
{

class X3DEXT X3DMaterialNode : public X3DAppearanceChildNode
{
public:

	virtual int ApplyMaterial(X3DDrawContext* pDC) = 0;

//	static NodeType* GetNodeType();
	X3DNODE_ABSTRACT(X3DMaterialNode, X3DAppearanceChildNode)

protected:
	CTOR X3DMaterialNode(NodeType* nodeType) : X3DAppearanceChildNode(nodeType)
	{
	}
};

class X3DEXT X3DTextureTransformNode : public X3DAppearanceChildNode
{
protected:
	CTOR X3DTextureTransformNode(NodeType* nodeType) : X3DAppearanceChildNode(nodeType)
	{
	}
};

class X3DEXT X3DTextureTransform2DNode : public X3DTextureTransformNode
{
protected:
	CTOR X3DTextureTransform2DNode(NodeType* nodeType) : X3DTextureTransformNode(nodeType)
	{
	}
};

class X3DEXT X3DTextureNode : public X3DAppearanceChildNode
{
protected:
	CTOR X3DTextureNode(NodeType* nodeType) : X3DAppearanceChildNode(nodeType)
	{
	}

public:

//	static NodeType* GetNodeType();
	X3DNODE_ABSTRACT(X3DTextureNode, X3DAppearanceChildNode)
};

class X3DEXT X3DTexture2DNode : public X3DTextureNode
{
protected:
	CTOR X3DTexture2DNode(NodeType* nodeType) : X3DTextureNode(nodeType)
	{
	}

public:

	X3DNODE_ABSTRACT(X3DTexture2DNode, X3DTextureNode)

//	static NodeType* GetNodeType();
};

class X3DEXT X3DInterpolatorNode : public X3DChildNode
{
public:

	MFFloat* getKeyField() throw()
	{
		return m_key;
	}

	/*
	SFFloat* getSetFractionField() throw()
	{
		return m_setFraction;
	}
	*/

	X3DSFIELD(SFFloat, setFraction, SetFraction)

	virtual X3DArrayField* getKeyValueField() throw() abstract;
	virtual X3DField* getValueChangedField() throw() abstract;

	X3DNODE_ABSTRACT(X3DInterpolatorNode, X3DChildNode)
//	static NodeType* GetNodeType();

	static TypedX3DFieldDefinition<MFFloat>* get_keyFieldDef();
//	static TypedX3DFieldDefinition<SFFloat>* get_setFractionFieldDef();

protected:

	CTOR X3DInterpolatorNode(NodeType* nodeType);

	MFFloat* m_key;
//	SFFloat* m_setFraction;
};

class X3DEXT X3DViewportNode : public X3DGroupingNode
{
protected:

	CTOR X3DViewportNode(NodeType* nodeType) : X3DGroupingNode(nodeType)
	{
	}

};

class X3DEXT X3DFontStyleNode : public X3DNode
{
protected:

	CTOR X3DFontStyleNode(NodeType* nodeType) : X3DNode(nodeType)
	{
	}
};

}	// x3d
}	// System

#include "X3DLayerNode.h"

//#include "LayerSet.h"
//#include "Layer.h"

namespace System
{
namespace x3d
{

class X3DEXT ScreenGroup : public X3DGroupingNode,
	public RenderImplImpl
{
public:
	CTOR ScreenGroup();

	virtual void Draw(X3DDrawContext* pDC) override;

	X3DNODE(ScreenGroup, X3DGroupingNode)
};

class Valuator : public X3DChildNode
{
public:
	CTOR Valuator() : X3DChildNode(&nodeType)
	{
	}

	static NodeType nodeType;
};

class Conditional : public X3DChildNode
{
public:
	CTOR Conditional() : X3DChildNode(&nodeType)
	{
	}

	static NodeType nodeType;

};

/*
    class ILHAnimHumanoid : public X3DChildNode
    {
    public:
    };

    
    class ILHAnimJoint : public X3DGroupingNode
    {
    public:
    };
    
    
class ILHAnimSegment : public X3DGroupingNode
    {
    public:
    };
    

    
class ILHAnimSite : public X3DGroupingNode
    {
    public:
    };
    

    class ILParticles : public X3DChildNode
    {
    public:
    };
    
    class ILPlanarObstacle : public X3DChildNode
    {
    public:
    };
    
    class ILNurbsPatchSurface : public X3DNurbsSurfaceGeometryNode
    {
    public:
    };
    
*/
    
#if 0
    MIDL_INTERFACE("93F89EE0-39CF-42f8-95D6-23CD43BE01E8")
    ILX3DExecutionContext
    {
    public:
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getSpecificationVersion( 
            /* [retval][out] */ sysstring *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getEncoding( 
            /* [retval][out] */ SAIEncodingID *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getWorldURL( 
            /* [retval][out] */ sysstring *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE createNode( 
            /* [in] */ sysstring name,
            /* [retval][out] */ X3DNode **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getProtoDeclaration( 
            /* [in] */ sysstring name,
            /* [retval][out] */ ILSAIProtoDeclaration **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getRootNodes( 
            /* [retval][out] */ MFNode **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE addRoute( 
            /* [in] */ X3DNode *sourceNode,
            /* [in] */ sysstring sourceField,
            /* [in] */ X3DNode *destinationNode,
            /* [in] */ sysstring destinationField) = 0;
        
    };
    
    MIDL_INTERFACE("AC1EA3D5-93A8-46fe-8BA8-070ED9F16006")
    ILX3DScene : public ILX3DExecutionContext
    {
    public:
        virtual /* [helpstring] */ ErrorCode getDEFName( 
            /* [in] */ sysstring name,
            /* [retval][out] */ X3DNode **pVal) = 0;
        
    };
#endif
    
#if 0
class    ILSAINodeList
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode get_length( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode item( 
            /* [in] */ long index,
            /* [retval][out] */ ILSAINode **pVal) = 0;
        
    };
#endif

#if 0    
    
class    ILSAIPrototypeInstance
    {
    public:
    };
    

    
class    ILSAIProtoDeclaration
    {
    public:
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE createInstance( 
            /* [retval][out] */ ILSAIPrototypeInstance **pVal) = 0;
        
    };
    
    
    MIDL_INTERFACE("95C6DFE5-D3BB-4265-86F9-BA5E8D362097")
    ILSAIFieldDeclaration
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode STDMETHODCALLTYPE get_fieldName( 
            /* [retval][out] */ sysstring *pVal) = 0;
        
    };
    
    
    MIDL_INTERFACE("01DD402E-E50F-4b41-BA96-91D399C0D374")
    ILSAIField
    {
    public:
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getFieldType( 
            /* [retval][out] */ FieldType *pVal) = 0;
        
    };
    
   
    MIDL_INTERFACE("C7569940-E47B-4851-B3E9-CF4D4BAD51FC")
    ILSAINodeType
    {
    public:
        virtual /* [helpstring][propget] */ ErrorCode STDMETHODCALLTYPE get_name( 
            /* [retval][out] */ sysstring *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getFieldDefinitions( 
            /* [retval][out] */ ILX3DFieldDefinitions **pVal) = 0;
        
    };
    
    MIDL_INTERFACE("827FB91A-014A-4b78-8F8A-00BFE124E07B")
    ILSAINode
    {
    public:
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getTypeName( 
            /* [retval][out] */ sysstring *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getType( 
            /* [retval][out] */ ILSAINodeType **pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getField( 
            /* [in] */ sysstring fieldName,
            /* [retval][out] */ X3DField **pVal) = 0;
        
    };
    
    MIDL_INTERFACE("6CF45CDA-39C3-4aef-831D-E97160166D0A")
    ILSAIBrowser
    {
    public:
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE getCurrentSpeed( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE createX3DFromStream(ISequentialByteStream *stream, ILX3DScene **pVal) = 0;
        
    };
    
    MIDL_INTERFACE("D26925E4-C56E-4091-84A5-9C48CBD3543E")
    ILX3DViewer
    {
    public:
        virtual /* [helpstring][propget][id] */ ErrorCode STDMETHODCALLTYPE get_src( 
            /* [retval][out] */ sysstring *pVal) = 0;
        
        virtual /* [helpstring][propput][id] */ ErrorCode STDMETHODCALLTYPE set_src( 
            /* [in] */ sysstring newVal) = 0;
        
        virtual /* [helpstring][id] */ ErrorCode STDMETHODCALLTYPE replaceWorld( 
            /* [in] */ ILX3DScene *scene) = 0;
        
    };
    
    
    MIDL_INTERFACE("AEC55143-2374-4edd-B37A-7E3A99CA2D08")
    IVRMLLoader
    {
    public:
        virtual /* [helpstring] */ ErrorCode STDMETHODCALLTYPE Load( 
            /* [in] */ sysstring pathName,
            /* [retval][out] */ ILX3DScene **pVal) = 0;
        
    };
#endif

void DrawChildren(X3DDrawContext* pDC, MFNode* children);
void DrawOverlayChildren(X3DDrawContext* pDC, MFNode* children);
void DrawGlobalLightsChildren(X3DDrawContext* pDC, MFNode* children);

struct X3DMaterial
{
	float4 diffuseColor;
	float4 ambientColor;
	float4 specularColor;
	float4 emissiveColor;
	float power;
};

struct X3DLight
{
	__declspec(align(16)) int type;
	__declspec(align(16)) float4 diffuseColor;
	__declspec(align(16)) float4 specularColor;
	__declspec(align(16)) float4 ambientColor;
	__declspec(align(16)) float3 position;
	__declspec(align(16)) float3 direction;
	float range;
	float falloff;
	float attenuation0;
	float attenuation1;
	float attenuation2;
	float theta;
	float phi;
	bool on;
};

class X3DEXT ComponentInfo : public Object
{
public:
	String getName()
	{
		return m_name;
	}

	int getLevel()
	{
		return m_level;
	}

	String getTitle();
	String getProviderURL();
	String toX3DString();

	String m_name;
	int m_level;
};

class X3DEXT ProfileInfo : public Object
{
protected:

	CTOR ProfileInfo(StringIn name);

public:
	String getName()
	{
		return m_name;
	}
	String getTitle()
	{
		return m_title;
	}
//	String toX3DString()

	vector<ComponentInfo*>& getComponents();

	String m_name;
	String m_title;
	vector<ComponentInfo*> m_components;

	/*
	static ProfileInfo Interchange;
	static ProfileInfo Interactive;
	static ProfileInfo Immersive;
	*/
};

class X3DEXT ProfileDeclaration : public Object
{
protected:

	CTOR ProfileDeclaration(StringIn name);

public:
	String getName()
	{
		return m_name;
	}

	String m_name;

	static ProfileDeclaration Interchange;
	static ProfileDeclaration Interactive;
	static ProfileDeclaration Immersive;
};

class X3DEXT X3DNodeSelection : public Object
{
public:
#ifndef __LERSTAD__
	std::map<X3DNode*, bool> m_items;
#else
	int pad[2];
#endif

	Event1<X3DNodeSelection*> SelectionChanged;
};

}	// x3d
}	// System

#include "X3DShaderNode.h"
#include "X3DScene.h"
#include "Coordinate.h"
#include "Coordinate2D.h"
#include "Normal.h"
#include "Color.h"
#include "TextureProperties.h"
#include "TextureCoordinateGenerator.h"
#include "TextureCoordinate.h"
#include "MultiTextureCoordinate.h"
#include "TextureTransform.h"
#include "ProximitySensor.h"
#include "TouchSensor.h"
#include "TimeSensor.h"

#include "WorldInfo.h"
#include "MetadataString.h"

#include "SceneView.h"
//#include "OGLSceneRenderer.h"	// TODO rename

#include "SceneRenderFilter.h"	// TODO remove

namespace System
{
namespace x3d
{

class X3DEXT LinePropertiesBIFS : 
	public X3DAppearanceChildNode
{
public:
	CTOR LinePropertiesBIFS();

	/*
	virtual NodeType* getType() const
	{
		return s_nodeType;
	}
	*/

	static NodeType s_nodeType;

	SFColor* m_lineColor;
	SFInt32* m_lineStyle;
	SFFloat* m_width;
};

X3DEXT void Translate(X3DChildNode* node, float x, float y, float z);
X3DEXT void Scale(X3DChildNode* node, float x, float y, float z);
X3DEXT void Rotate(X3DChildNode* node, float x, float y, float z, float angle);

interface IBrowser
{
	virtual ProfileDeclaration* getProfile(StringIn profile) abstract;
	virtual X3DScene* createScene(ProfileDeclaration* profile) abstract;
//	virtual X3DScene* importDocument(Web::Node* node) abstract;
	virtual String getName() abstract;
	virtual void setDescription(StringIn description) abstract;
};

interface X3DScriptImplementation
{
	virtual void setBrowser(IBrowser* browser) abstract;
	virtual void setFields(X3DScriptNode* externalView, map<String, X3DField*>& fields) abstract;
	virtual void initialize() abstract;
	virtual void shutdown() abstract;
	virtual void eventsProcessed() abstract;
};

#if 0
class Browser : public Object, public IBrowser
{
	virtual X3DScene* createScene(ProfileDeclaration* profile) abstract;
//	virtual X3DScene* importDocument(Web::Node* node) abstract;
	virtual String getName() abstract;
	virtual void setDescription(StringIn description) abstract;
};
#endif

X3DEXT void ReloadEffect();

X3DEXT SceneVisual3D* View(X3DScene* scene);
X3DEXT Gui::TreeView* TreeOf(X3DScene* scene);
X3DEXT TextDocument* asvrml(X3DScene* scene);
X3DEXT Object* asxml(X3DScene* scene);

X3DEXT void Show(Gui::TreeView* view);

X3DEXT void DrawAxis(X3DDrawContext* pDC);

X3DEXT Vec3f VecProject(Vec3f obj, const D3D10_VIEWPORT& vp, const D3DXMATRIX& proj, const D3DXMATRIX& view, const D3DXMATRIX& model);

// for ecmascript binding
class X3DEXT X3DConstants
{
public:

	static const FieldType SFBool = FieldType_SFBool;
	static const FieldType MFBool = FieldType_MFBool;
	static const FieldType SFInt32 = FieldType_SFInt32;
	static const FieldType MFInt32 = FieldType_MFInt32;
	static const FieldType SFFloat = FieldType_SFFloat;
	static const FieldType MFFloat = FieldType_MFFloat;
//...

private:
	CTOR X3DConstants();
};

}	// x3d
}	// System
