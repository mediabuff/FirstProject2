#include "StdAfx.h"
#include "X3D2.h"
#include "OGLSceneRenderer.h"

#include "Group.h"
#include "Layer.h"
#include "LayerSet.h"
#include "Transform.h"
#include "Viewpoint.h"
#include "NavigationInfo.h"

//#include "Draw/BasicCommands.h"	// TODO remove

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <gl/wglext.h>
#include "Graphics/openglext.h"
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <Cg/cgD3D9.h>

#include <algorithm>

#include "Fog.h"
//#include "PointLight.h"	// TODO remove

namespace System
{

namespace Graphics
{
GfxExt int ClipPolygonAgainstPlane(int vertexCount, const Vertex *vertex, uint32 texmask, const gm::vector4f& plane, signed char *location, Vertex *result);
}

namespace x3d
{

class Face
{
public:

	CTOR Face(int v0, int v1, int v2);

	void CalcMinZ(Graphics::Vertex* vertices);

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

	/*
void X3DDrawContext::AddFace(const Face& face)
{
	m_faces.push_back(face);
}
*/

#if 0
void X3DDrawContext::Vertex(LDraw::vector3f v)
{
	ASSERT(0);
#if 0
	LDraw::GraphicsO_3D* p = m_pGraphics3D;
	ASSERT(!p->m_state->m_modelview.Matrix.isSingular());

	v = p->m_state->m_modelview.Matrix.transform(v);
	m_vertices.push_back(v);
	m_colors.push_back(m_currentColor);
#endif
}
#endif

/*
void X3DDrawContext::GenFace(LDraw::vector3f v0, LDraw::vector3f v1, LDraw::vector3f v2)
{
	LDraw::GraphicsO_3D* p = ((LDraw::Graphics3DImmediate*)m_pGraphics3D)->m_pG;

	v0 = p->m_state->m_modelview.Matrix.transform(v0);
	v1 = p->m_state->m_modelview.Matrix.transform(v1);
	v2 = p->m_state->m_modelview.Matrix.transform(v2);

	m_vertices

	m_faces.push_back(Face(v0, v1, v2));
}
*/

class bspnode
{
public:

	gm::vector4f plane;
	list<Face> sameList;
	list<Face> oppList;

	bspnode* negativeSide;
	bspnode* positiveSide;
};

void clip_convex_polygon_against_plane(const gm::vector4f plane, const vector<int>& polygon, vector<int>& front, vector<int>& back, vector<gm::vector3f>& vertices, vector<gm::vector4f>& colors)
{
	gm::vector3f prev_pt = vertices[polygon[polygon.size()-1]];
	float prev_side = prev_pt[0]*plane[0] + prev_pt[1]*plane[1] + prev_pt[2]*plane[2] + plane[3];

	for (unsigned int i = 0; i < polygon.size(); i++)
	{
		int index_pt = polygon[i];
		gm::vector3f pt = vertices[index_pt];
		float side = pt[0]*plane[0] + pt[1]*plane[1] + pt[2]*plane[2] + plane[3];

		if (side >= 0)
		{
			if (prev_side < 0)
			{
				gm::vector3f dv = pt - prev_pt;

				/*
				float d = plane[0]*dv[0] + plane[1]*dv[1] + plane[2]*dv[2] + plane[3];
				ASSERT(d != 0);
				
				float t = (plane[0]*pt[0] + plane[1]*pt[1] + plane[2]*pt[2] + plane[3]) / d;

				LDraw::vector3f intersect_pt = pt - dv*t;
*/
				float d = plane[0]*dv[0] + plane[1]*dv[1] + plane[2]*dv[2];// + plane[3];
				ASSERT(d != 0);
				
				float t = -(plane[0]*prev_pt[0] + plane[1]*prev_pt[1] + plane[2]*prev_pt[2] + plane[3]) / d;

				gm::vector3f intersect_pt = prev_pt + dv*t;

				gm::vector4f color = colors[index_pt];
				gm::vector4f intersect_color = color;// - dv*t;

				front.push_back(vertices.size());
				back.push_back(vertices.size());

				vertices.push_back(intersect_pt);
				colors.push_back(intersect_color);
			}

			front.push_back(index_pt);
		}
		else if (side < 0)
		{
			if (prev_side >= 0)
			{
				gm::vector3f dv = pt - prev_pt;

				float d = plane[0]*dv[0] + plane[1]*dv[1] + plane[2]*dv[2];// + plane[3];
				ASSERT(d != 0);
				
				float t = -(plane[0]*prev_pt[0] + plane[1]*prev_pt[1] + plane[2]*prev_pt[2] + plane[3]) / d;

				gm::vector3f intersect_pt = prev_pt + dv*t;

				gm::vector4f color = colors[index_pt];
				gm::vector4f intersect_color = color;// - dv*t;

				front.push_back(vertices.size());
				back.push_back(vertices.size());
				vertices.push_back(intersect_pt);
				colors.push_back(intersect_color);
			}

			back.push_back(index_pt);
		}

		prev_pt = pt;
		prev_side = side;
	}
}

inline bool fuzzy_eq(float a, float b)
{
	float diff = a - b;
	return diff > -0.0001 && diff < 0.0001;
}

void BSPPartitionFaceListWithPlane(gm::vector4f plane, const Face* pFace, vector<gm::vector3f>& vertices, vector<gm::vector4f>& colors, const list<Face>& polygonList, list<Face>& sameList, list<Face>& oppList, list<Face>& negativeList, list<Face>& positiveList)
{
	list<Face>::const_iterator it = polygonList.begin();
	while (it != polygonList.end())
	{
		const Face& face = *it;

		if (&face == pFace)
		{
			sameList.push_back(face);
		}
		else
		{
			gm::vector3f a = vertices[face.vertexIndex[0]];
			gm::vector3f b = vertices[face.vertexIndex[1]];
			gm::vector3f c = vertices[face.vertexIndex[2]];

			float d_0 = (a[0]*plane[0] + a[1]*plane[1] + a[2]*plane[2] + plane[3]);
			float d_1 = (b[0]*plane[0] + b[1]*plane[1] + b[2]*plane[2] + plane[3]);
			float d_2 = (c[0]*plane[0] + c[1]*plane[1] + c[2]*plane[2] + plane[3]);

			if (fuzzy_eq(d_0, 0) && fuzzy_eq(d_1, 0) && fuzzy_eq(d_2, 0))
			{
				sameList.push_back(face);
			}
			else
			{
				if (d_0 <= 0 && d_1 <= 0 && d_2 <= 0)
				{
					negativeList.push_back(face);
				}
				else if (d_0 >= 0 && d_1 >= 0 && d_2 >= 0)
				{
					positiveList.push_back(face);
				}
				else
				{
					vector<int> polygon;
					polygon.reserve(3);
					polygon.push_back(face.vertexIndex[0]);
					polygon.push_back(face.vertexIndex[1]);
					polygon.push_back(face.vertexIndex[2]);

					vector<int> frontPolygon;
					frontPolygon.reserve(4);

					vector<int> backPolygon;
					backPolygon.reserve(4);

					clip_convex_polygon_against_plane(plane, polygon, frontPolygon, backPolygon, vertices, colors);

					if (backPolygon.size() >= 3)
					{
						ASSERT(backPolygon.size() >= 3);
						ASSERT(backPolygon.size() <= 4);

						for (int i = 1; i < backPolygon.size()-1; i += 1)
						{
							negativeList.push_back(Face(backPolygon[0], backPolygon[i], backPolygon[1+i]));
						}
					}

					if (frontPolygon.size() >= 3)
					{
						ASSERT(frontPolygon.size() >= 3);
						ASSERT(frontPolygon.size() <= 4);

						for (int i = 1; i < frontPolygon.size()-1; i += 1)
						{
							positiveList.push_back(Face(frontPolygon[0], frontPolygon[i], frontPolygon[1+i]));
						}
					}
				}
			}
		}

		++it;
	}
}

const Face& BSPChoosePlane(const vector<gm::vector3f>& vertices, const list<Face>& polygonList)
{
	return polygonList.front();

	/*
	list<Face>::const_iterator it = polygonList.begin();
	while (it != polygonList.end())
	{
		const Face& face = *it;
		LDraw::vector3f a = vertices[face.vertexIndex[0]];
		LDraw::vector3f b = vertices[face.vertexIndex[1]];
		LDraw::vector3f c = vertices[face.vertexIndex[2]];

		LDraw::vector3f plane_normal = LDraw::cross(b-a,c-a);
		plane_normal.normalize();

		float plane_d = -((plane_normal[0] * a[0]) + (plane_normal[1] * a[1]) + (plane_normal[2] * a[2]));
		LDraw::vector4f plane = LDraw::vector4f(plane_normal[0], plane_normal[1], plane_normal[2], plane_d);

		int clipcount = 0;

		list<Face>::const_iterator it2 = polygonList.begin();
		while (it2 != polygonList.end())
		{
			if (it != it2)
			{
				const Face& face2 = *it2;
				LDraw::vector3f a = vertices[face2.vertexIndex[0]];
				LDraw::vector3f b = vertices[face2.vertexIndex[1]];
				LDraw::vector3f c = vertices[face2.vertexIndex[2]];

				float d_0 = plane[0]*a[0] + plane[1]*a[1] + plane[2]*a[2] + plane[3];
				float d_1 = plane[0]*b[0] + plane[1]*b[1] + plane[2]*b[2] + plane[3];
				float d_2 = plane[0]*c[0] + plane[1]*c[1] + plane[2]*c[2] + plane[3];

				if (d_0 <= 0 && d_1 <= 0 && d_2 <= 0)
				{
				}
				else if (d_0 >= 0 && d_1 >= 0 && d_2 >= 0)
				{
				}
				else
					clipcount++;
			}

			++it2;
		}

		if (clipcount == 0) return *it;

		++it;
	}

//	MessageBeep(-1);
	ASSERT(0);
	//const Face& face = polygonList.front();
	return polygonList.front();
	*/
}

bspnode* buildbsp(vector<gm::vector3f>& vertices, vector<gm::vector4f>& colors, const list<Face>& polygonList)
{
	const Face& face = BSPChoosePlane(vertices, polygonList);
	//polygonList.pop_front();

	gm::vector3f a = vertices[face.vertexIndex[0]];
	gm::vector3f b = vertices[face.vertexIndex[1]];
	gm::vector3f c = vertices[face.vertexIndex[2]];

	gm::vector3f plane_normal = gm::cross(b-a,c-a);
	plane_normal.normalize();

	float plane_d = -((plane_normal[0] * a[0]) + (plane_normal[1] * a[1]) + (plane_normal[2] * a[2]));
	gm::vector4f plane = gm::vector4f(plane_normal[0], plane_normal[1], plane_normal[2], plane_d);

	bspnode* node = new bspnode;
	node->plane = plane;

	list<Face> negativeList;
	list<Face> positiveList;
	BSPPartitionFaceListWithPlane(plane, &face, vertices, colors, polygonList, node->sameList, node->oppList, negativeList, positiveList);

	ASSERT(!node->sameList.empty());

	if (!negativeList.empty())
		node->negativeSide = buildbsp(vertices, colors, negativeList);
	else
		node->negativeSide = NULL;

	if (!positiveList.empty())
		node->positiveSide = buildbsp(vertices, colors, positiveList);
	else
		node->positiveSide = NULL;

	return node;
}

void drawfaces(Graphics::Graphics3D* p3D, const vector<gm::vector3f>& vertices, const vector<gm::vector4f>& colors, const list<Face>& polygonList)
{
	ASSERT(0);
#if 0
	list<Face>::const_iterator it = polygonList.begin();
	while (it != polygonList.end())
	{
		const Face& face = *it;

		int ind0 = face.vertexIndex[0];
		int ind1 = face.vertexIndex[1];
		int ind2 = face.vertexIndex[2];

		p3D->glColor(colors[ind0]);
		p3D->glVertex(vertices[ind0]);

		p3D->glColor(colors[ind1]);
		p3D->glVertex(vertices[ind1]);

		p3D->glColor(colors[ind2]);
		p3D->glVertex(vertices[ind2]);

		/*
		const LDraw::Vertex& a = vertices[face.vertexIndex[0]];
		const LDraw::Vertex& b = vertices[face.vertexIndex[1]];
		const LDraw::Vertex& c = vertices[face.vertexIndex[2]];

		p3D->triangle2(
			a,
			b,
			c);
*/

		++it;
	}
#endif
}

void drawbsp(Graphics::Graphics3D* p3D, gm::vector3f viewpoint, const vector<gm::vector3f>& vertices, const vector<gm::vector4f>& colors, bspnode* node)
{
	if (node == NULL) return;

	float d = (viewpoint[0]*node->plane[0] + viewpoint[1]*node->plane[1] + viewpoint[2]*node->plane[2] + node->plane[3]);

	if (d > 0)
	{
		drawbsp(p3D, viewpoint, vertices, colors, node->negativeSide);
		drawfaces(p3D, vertices, colors, node->sameList);
		drawbsp(p3D, viewpoint, vertices, colors, node->positiveSide);
	}
	else if (d < 0)
	{
		drawbsp(p3D, viewpoint, vertices, colors, node->positiveSide);
		drawfaces(p3D, vertices, colors, node->sameList);
		drawbsp(p3D, viewpoint, vertices, colors, node->negativeSide);
	}
	else
	{
		drawbsp(p3D, viewpoint, vertices, colors, node->positiveSide);
		drawbsp(p3D, viewpoint, vertices, colors, node->negativeSide);
	}
}

class z_sort
{
public:
	bool operator () (int a, int b) const
	{
		return m_faces[a].minz > m_faces[b].minz;
	}
	Face* m_faces;
};

Face::Face(int v0, int v1, int v2)
{
	vertexIndex[0] = v0;
	vertexIndex[1] = v1;
	vertexIndex[2] = v2;
}

void Face::CalcMinZ(Graphics::Vertex* vertices)
{
	minz = std::min(vertices[vertexIndex[0]].p[2], std::min(vertices[vertexIndex[1]].p[2], vertices[vertexIndex[2]].p[2]));
	maxz = std::max(vertices[vertexIndex[0]].p[2], std::max(vertices[vertexIndex[1]].p[2], vertices[vertexIndex[2]].p[2]));
}

double fps = 12;

#if USE_GL

void glRotate(float angle, gmVector3t<float>& v);
void glRotate(float angle, gmVector3t<double>& v);
void glTranslate(gmVector2f& v);
void glTranslate(gmVector3t<float>& v);
void glTranslate(gmVector3t<double>& v);

#endif

/////////////////////////////////////////////////////////////////////////////
// COGLSceneRenderer

#if 0
STDMETHODIMP COGLSceneRenderer::CInputPin::Receive(ILMediaSample *pSample)
{
	EnterCriticalSection(&m_pLock);
	/*

	while (1)
	{
		EnterCriticalSection(&m_pFilter->m_pLock);
		if (m_pFilter->m_state == LState_Running)
			break;
		LeaveCriticalSection(&m_pFilter->m_pLock);
	}
	LeaveCriticalSection(&m_pFilter->m_pLock);

	*/

//	MessageBeep(-1);

	LONGLONG timeStart;
	LONGLONG timeEnd;
	pSample->GetTime(&timeStart, &timeEnd);

	CComQIPtr<ILBIFSSample> bifsSample = pSample;

	CComPtr<IUnknown> unk;
	bifsSample->GetScene(&unk);
	CComQIPtr<ILX3DScene> scene = unk;

	m_pFilter->m_scene = static_cast<CComObject<CLX3DScene>*>(scene.p);

	if (m_pFilter->m_scene->m_viewpointStack.size() == 0)
	{
		MessageBeep(-1);
		m_pFilter->InitStacks();
	}

// BIFS samples are not double buffered, so we do de this differently than when displaying double buffered video

	::PostMessage(m_pFilter->m_hWnd, WM_USER+100, 0, 0);	// Display the sample

// Wait for the duration of the sample
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pFilter->m_clock->AdviseTime(m_pFilter->m_tStart/*baseTime*/, timeEnd/*streamTime*/, hEvent, NULL);
	WaitForSingleObject(hEvent, INFINITE);

	CloseHandle(hEvent);

	//m_pSample = static_cast<CVideoSample*>(pSample);

	LeaveCriticalSection(&m_pLock);

	return S_OK;
}

void COGLSceneRenderer::InitStacks()
{
	ASSERT(0);
#if 0
	for (unsigned int i = 0; i < m_scene->m_nodes.GetSize(); i++)
	{
		if (Viewpoint* viewpoint = dynamic_cast<Viewpoint*>(m_scene->m_nodes[i]))
		{
			if (m_scene->m_viewpointStack.GetSize() == 0)
			{
				Viewpoint* pViewpoint = viewpoint;
				pViewpoint->m_set_bind->m_value = true;
				pViewpoint->Change(pViewpoint->m_set_bind);

				//m_scene->m_viewpointStack.Add(viewpoint.Detach());
			}
		}
		else if (NavigationInfo* navigationinfo = dynamic_cast<NavigationInfo*>(m_scene->m_nodes[i]))
		{
			if (m_scene->m_navigationinfoStack.GetSize() == 0)
			{
				m_scene->m_navigationinfoStack.Add(navigationinfo);
			}
		}
		else if (Fog* fog = dynamic_cast<Fog*>(m_scene->m_nodes[i]))
		{
			if (m_scene->m_fogStack.GetSize() == 0)
			{
				m_scene->m_fogStack.Add(fog);
			}
		}
	}

	if (m_scene->m_viewpointStack.GetSize() == 0)
	{
		// Create a default viewpoint with default parameters
		Viewpoint* pViewpoint = dynamic_cast<Viewpoint*>(m_scene->createNode(WSTR("Viewpoint")));

		pViewpoint->m_set_bind->m_value = true;
		pViewpoint->Change(pViewpoint->m_set_bind);

	//	m_viewpointStack.Add(viewpoint.Detach());
	}

	{
		Viewpoint* pViewpoint = static_cast<Viewpoint*>(m_scene->m_viewpointStack[0]);
	// Initialize these
		m_newPosition = pViewpoint->m_position->m_value;
		m_newOrientation = pViewpoint->m_orientation->m_value;
	}

	if (m_scene->m_navigationinfoStack.GetSize() == 0)
	{
		// Create a default viewpoint with default parameters
		NavigationInfo* navigationinfo = dynamic_cast<NavigationInfo*>(m_scene->createNode(WSTR("NavigationInfo")));

		m_scene->m_navigationinfoStack.Add(navigationinfo);
	}
#endif
}
#endif

double GetLineAngle(float x1, float y1, float x2, float y2)
{
	double dx = x2-x1;
	double dy = y2-y1;

	double distance2 = sqrt(dx*dx + dy*dy);

	if (distance2 == 0.0) distance2 = 0.00001;
//	if (dy < 0) distance2 =-distance2;
	double angle = acos(dx/distance2);
//	if (dy < 0) angle += M_PI;

	return angle;
}

/*
R = radius
M = mass
*/

gm::matrix3f& MomentOfInertiaSphere(gm::matrix3f& I, double R, double M)
{
	I = gm::matrix3f::getIdentity();

	double i = (2.0/5.0) * M * R*R;
	I[0][0] = i;
	I[1][1] = i;
	I[2][2] = i;

	return I;
}

double SphereDensity(double R, double M)
{
	return M / ((4.0/3.0) * M_PI * R*R*R);
}

#define KMH(x)		(x*1000/(60*60))

#define MAX_SPEED	(KMH(140))
#define MIN_REVERSE_SPEED	(-KMH(50))

bool gas = false;
double speed = 0;	// meters per sec
//double heading = 0;
double wheel_rotation = 0;

//#define MAX_SPEED	(KMH(180))
//#define MIN_REVERSE_SPEED	(-KMH(20))

double acceleration = 12;
double decceleration = 20;

double heading = 0;//M_PI;
double bank = 0;

double steering = heading;

gm::vector3f posv(0, 20, 0);
gm::vector3f carv(0, 20, 5);

const double G = 6.67 * 10e-11;

double mass = 40;
gm::matrix3f m;	// mass
//gmMatrix3 I;		// inertia

int playcount = 0;
long lastt;

gm::matrix3f InverseBodyInertiaTensor;
gm::vector3f AngularVelocity;
gm::matrix3f Ia_inv;

CState states[2];

void OrthonormalizeOrientation(gm::matrix3f& Orientation)
{
    gm::vector3f X(Orientation[0][0],Orientation[1][0],Orientation[2][0]);
    gm::vector3f Y(Orientation[0][1],Orientation[1][1],Orientation[2][1]);
    gm::vector3f Z;

    X.normalize();
    Z = gm::cross(X,Y).normalize();
    Y = gm::cross(Z,X).normalize();

    Orientation[0][0] = X[0]; Orientation[0][1] = Y[0]; Orientation[0][2] = Z[0];
    Orientation[1][0] = X[1]; Orientation[1][1] = Y[1]; Orientation[1][2] = Z[1];
    Orientation[2][0] = X[2]; Orientation[2][1] = Y[2]; Orientation[2][2] = Z[2];
}
#if 0
void COGLSceneRenderer::Integrate(double deltaTime)
{
	CState& source = states[0];
	CState& target = states[1];

	// A property of mass is its resistance to a change of velocity. This is quantified by Newtons second law which says that the acceleration is:

//	F, a, T and alpha can be represented as vectors and m and I as matrices

	// Calculate the Inverse of Inertia Tensor in absolute world coordinates
	Ia_inv =  source.Orientation * InverseBodyInertiaTensor.getInverse() * source.Orientation.getInverse();

	// calculate angular velocity from angular momentum
	AngularVelocity = Ia_inv.transform(source.AngularMomentum);

// integrate position
	LDraw::vector3f endpt;
	endpt = source._position + source.V * deltaTime;//(dest_points[i][0], dest_points[i][1], dest_points[i][2]);

	// integrate orientation
	target.Orientation = source.Orientation + (deltaTime * LDraw::matrix3f::tilde(AngularVelocity)*source.Orientation);

	LDraw::vector3f Gravity;		// net force 
	Gravity[0] = 0;	// gravitation
	Gravity[1] = -9.8f;	// gravitation
	Gravity[2] = 0;	// gravitation

	//gmVector3t<float> a;		// acceleration
	//F = m.transform(a);
	//V += F/m
	// F/m = m_inv * F
	//a = m.inverse().transform(F);

	// integrate velocity

	LDraw::vector3f F;
	F = Gravity*mass;
	LDraw::vector3f acceleration = (1/mass)*F;	// F = ma so a = F/m
	target.V = source.V + acceleration * deltaTime;
	/*
	V[0] += a[0]/fps;
	V[1] += a[1]/fps;
	V[2] += a[2]/fps;
	*/

	/*
	V[0] += mass*a[0]/fps;
	V[1] += mass*a[1]/fps;
	V[2] += mass*a[2]/fps;
	*/

	LDraw::vector3f torque(0,0,0);// = cross(gmVector3t<float>(0,0,0), F);

	// integrate angular momentum
	target.AngularMomentum += source.AngularMomentum + torque * deltaTime;

	// reorthogonalize
//	gmMatrix3 C = (R*R.transpose()).inverse()/2;
//	gmMatrix3 C = (I - R * R.transpose())/2;
//	R = R * C;
	OrthonormalizeOrientation(target.Orientation);

//	gmMatrix3 ne = Orientation * Orientation.transpose();
//	ASSERT(ne.IsNeutral());

	/*
	gmVector3 angAcc;	// angular acceleration
	angAcc[0] = 0;
	angAcc[1] = 0;
	angAcc[2] = -0.2;
	*/

	//double m = 0.2;

	//_rot += I.transform(_torque);

	//_rot[0] += I.transform
}

void COGLSceneRenderer::CheckForCollision(CState* state)
{
	ASSERT(0);
#if 0
	CState& source = states[0];
	CState& target = states[1];

	{
		for (int i = 0; i < m_scene->m_nodes.GetSize(); i++)
		{
			CLGeometryImplImpl* Wall = m_scene->m_nodes[i];//m_scene->FindDef(L"WALL");
			if (Wall)
			{
				gmVector3t<float> startpt = source._position;
				gmVector3t<float> endpt = target._position;

//						endpt = _position+_velocity;//(dest_points[i][0], dest_points[i][1], dest_points[i][2]);

				gmVector3t<float> plane_normal;
				gmVector3t<float> plane_pt;

				int n = 0;
				double d2;
				if (Wall->Within(startpt, endpt, plane_normal, plane_pt, d2))
				{
#if 0
					if (n == 1)
					{
						MessageBeep(-1);
					}

					n++;
#endif
					MessageBeep(-1);
#if 0
					gmVector3t<float> new_pt;
					new_pt = plane_pt + plane_normal / 20;//*(fabs(speed) + 0.0001) / 300;
					endpt = new_pt;
				//	V = - V/2;
#else
					gmVector3t<float> CollisionNormal = plane_normal;

					gmVector3t<float> ray = plane_pt - source._position;
					ray.normalize();
					gmVector3t<float> R = ray * 0.5;//_position - plane_pt;// = Position - Configuration.CMPosition;
				//	R.normalize();
				//	R /= 2;

					gmVector3t<float> Velocity = source.V + cross(AngularVelocity, R);//Configuration.CMVelocity + CrossProduct(Configuration.AngularVelocity,R);

					float CoefficientOfRestitution = 0.5;

					float d0 = dot(Velocity,CollisionNormal);
					float ImpulseNumerator = -(1 + CoefficientOfRestitution) * d0;

					gmVector3t<float> In = cross(R,CollisionNormal);
					float d1 = dot(cross(Ia_inv.transform(In),R),CollisionNormal);
					float ImpulseDenominator = (1/mass) + d1;

					ASSERT(ImpulseDenominator != 0);
					gmVector3t<float> Impulse = (ImpulseNumerator/ImpulseDenominator) * CollisionNormal;

					// apply impulse to primary quantities
					target.V = source.V + (1/mass) * Impulse;	//	CMVelocity += Body.OneOverMass * Impulse;
					target.AngularMomentum = source.AngularMomentum + cross(R,Impulse);	//	AngularMomentum += CrossProduct(R,Impulse);
					// compute affected auxiliary quantities
#if 0
					Configuration.AngularVelocity = Configuration.InverseWorldInertiaTensor *
						Configuration.AngularMomentum;
#endif
			//		gmVector3t<float> r(0.5, 0.5, 0.5);	// distance from the centre of rotation
			//		gmVector3t<float> F = m.transform(a);
			//		_torque = cross(r, F);
#endif
					endpt = plane_pt + plane_normal*0.501;
			//		endpt = _position;
				//	position->m_value = _position;
				//	return 0;
				//	break;

				}
			}
		}
	}
#endif
}
#endif

void DoProximity(X3DViewpointNode* pViewpoint, X3DNode* node, gm::matrix4f transform, double m_currentVideoTime)
{
	ASSERT(0);
#if 0

	if (ProximitySensor* pSensor = dynamic_cast<ProximitySensor*>(node))
	{
		//CLProximitySensor* pSensor = static_cast<CLProximitySensor*>(sensor.p);

		LDraw::vector3f position = transform.getInverse().transform(pViewpoint->getPosition());

		bool bInside =
			position[0] >= pSensor->m_center->m_value[0] - pSensor->m_size->m_value[0]/2 &&
			position[0] < pSensor->m_center->m_value[0] + pSensor->m_size->m_value[0]/2 &&
			position[1] >= pSensor->m_center->m_value[1] - pSensor->m_size->m_value[1]/2 &&
			position[1] < pSensor->m_center->m_value[1] + pSensor->m_size->m_value[1]/2 &&
			position[2] >= pSensor->m_center->m_value[2] - pSensor->m_size->m_value[2]/2 &&
			position[2] < pSensor->m_center->m_value[2] + pSensor->m_size->m_value[2]/2;

		//position += pSensor->m_center->m_value;
		position = pViewpoint->getPosition();
	//	position[2] -= 6;


		if (bInside != pSensor->m_isActive->m_value)
		{
			// TODO, interpolate to get the correct time of entry/exit

			if (bInside)
			{
				pSensor->m_enterTime->m_value = m_currentVideoTime;
				pSensor->Change(pSensor->m_enterTime);

				pSensor->m_isActive->m_value = true;
				pSensor->Change(pSensor->m_isActive);
			}
			else
			{
				pSensor->m_exitTime->m_value = m_currentVideoTime;
				pSensor->Change(pSensor->m_exitTime);

				pSensor->m_isActive->m_value = false;
				pSensor->Change(pSensor->m_isActive);
			}
		}

		if (bInside)
		{
		//	MessageBeep(-1);

/*
			LDraw::matrix4f mat = LDraw::matrix4f::getRotate(gmDegrees(pViewpoint->m_orientation->m_value.m_angle), -pViewpoint->m_orientation->m_value.m_axis);

	//		mat = transform.getInverse().transform(pViewpoint->m_position->m_value);

			mat = mat * transform.getInverse();
			//

			LDraw::matrix3f mat3(	mat[0][0], mat[1][0], mat[2][0], 
									mat[0][1], mat[1][1], mat[2][1],
									mat[0][2], mat[1][2], mat[2][2]);

			Quat4d q = MatrixToQuaternion(mat3);

			//pSensor->m_orientation_changed->m_value = pViewpoint->m_orientation->m_value;
			pSensor->m_orientation_changed->m_value = q.QuaternionToAxisAngle();
*/
		
			pSensor->m_orientation_changed->m_value = pViewpoint->getOrientation();//AxisAngle(0, 0, -1, 0);
			pSensor->Change(pSensor->m_orientation_changed);

			pSensor->m_position_changed->m_value = position;//pViewpoint->m_position->m_value;
			pSensor->Change(pSensor->m_position_changed);
		}
	}
	else if (Transform* p = dynamic_cast<Transform*>(node))
	{
		transform = transform * p->GetTransform();
	}

	MFNode* children = dynamic_cast<MFNode*>(node->getField(WSTR("children")));
	if (children)
	{
		for (unsigned int i = 0; i < children->m_items.GetSize(); i++)
		{
			if (children->m_items[i])
			{
				DoProximity(pViewpoint, children->m_items[i], transform, m_currentVideoTime);
			}
		}
	}
#endif
}

#if 0
void COGLSceneRenderer::OnTimer(UINT/*UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled*/)
{
#if 0
	for (unsigned int i = 0; i < m_scene->m_nodes.GetSize(); i++)
	{
		X3DNode* node = m_scene->m_nodes[i];

	//	CComQIPtr<ILX3DTimeDependentNode> timeDependant = node;
	//	if (timeDependant)
		{
			node->DoTime(m_currentVideoTime);
		}
	}

	X3DViewpointNode* pViewpoint = NULL;
	if (m_scene->m_viewpointStack.size() > 0)
	{
		pViewpoint = m_scene->m_viewpointStack.top();
	}

	if (pViewpoint)
	{
		pViewpoint->setPosition(m_newPosition);
		pViewpoint->setOrientation(m_newOrientation);

		DoProximity(pViewpoint, m_scene->m_root, LDraw::matrix4f::getIdentity(), m_currentVideoTime);

		/*
		for (i = 0; i < m_scene->m_nodes.GetSize(); i++)
		{
			X3DNode* node = m_scene->m_nodes[i];

		}
		*/
	}

	m_currentVideoTime += 1.0 / fps;

	ASSERT(0);
#if 0
	InvalidateRender();
#endif

#if 0
	CState& source = states[0];
	CState& target = states[1];

//	long thist = GetTickCount();
/*
	if (thist == lastt)
		return 0;

	double fps = 1000.0 / (thist-lastt);
	lastt = thist;

	if (fps == 0)	// ??
		return 0;
	*/

	fps = 20;
	double deltaTime = 1/fps;

	//AdvancePlay();
	//UpdateAllViews();
	//MessageBeep(-1);
//#if 0	// TODO, have something like this

	X3DNode* ball = m_scene->FindDef(L"BALL");
	if (ball)
	{
		{
			Integrate(deltaTime);

#if 0
			CheckForCollision()
#endif

			CComPtr<ILSFVec3f> translation;
			ball->getField(L"translation", (ILX3DField**)&translation);

			CComPtr<ILSFRotation> rotation;
			ball->getField(L"rotation", (ILX3DField**)&rotation);

			SFVec3f* position = static_cast<SFVec3f*>(translation.p);
			SFRotation* orientation = static_cast<SFRotation*>(rotation.p);

			orientation->m_value = MatrixToQuaternion(target.Orientation).QuaternionToAxisAngle();

#if 0
			_position = endpt;
			position->m_value = _position;
#endif
			/*
			Quat4d q = EulerToQuaternion(_rot[0], _rot[1], _rot[2]);
			orientation->m_value = q.QuaternionToAxisAngle();
			*/
		}
	}

#if 0
	double fps = 60;

	int gas = 0;

	if (GetAsyncKeyState(VK_SPACE) & (1<<15))
	{
		gas = 1;
	}
	else if (GetAsyncKeyState(VK_DOWN) & (1<<15))
	{
		gas = -1;
	}

	if (GetAsyncKeyState(VK_LEFT) & (1<<15))
	{
		steering += gmRadians(5);
	//	if (steering > gmRadians(60))
	//		steering = gmRadians(60);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & (1<<15))
	{
		steering -= gmRadians(5);
	//	if (steering < gmRadians(-60))
	//		steering = gmRadians(-60);
	}

	if (gas > 0)
	{
		speed += acceleration / fps;
		if (speed > MAX_SPEED)
			speed = MAX_SPEED;
	}
	else if (gas < 0)
	{
		speed -= acceleration / fps;
		if (speed < MIN_REVERSE_SPEED)
			speed = MIN_REVERSE_SPEED;
	}
	else
	{
		if (speed > 0)
		{
			speed -= decceleration / fps;
			if (speed < 0)
				speed = 0;
		}
		else if (speed < 0)
		{
			speed += decceleration / fps;
			if (speed > 0)
				speed = 0;
		}
	}

	//heading = steering;
	if (steering < heading)
	{
		heading -= (speed/fps)*5 * (M_PI/180);
		if (heading < steering)
			heading = steering;
	}
	else if (steering > heading)
	{
		heading += (speed/fps)*5 * (M_PI/180);
		if (heading > steering)
			heading = steering;
	}

	CComPtr<ILX3DNode> car = m_scene->FindDef(L"CAR");
	if (car)
	{
		WCHAR buf[64];

		CComPtr<ILX3DNode> overstell = m_scene->FindDef(L"OVERSTELL");
		CComPtr<ILX3DNode> speedom = m_scene->FindDef(L"SPEEDOM");
		if (speedom)
		{
		//	swprintf(buf, L"%3.3d km/h", (int)(speed * 60*60 / 1000));
		}
		
		ILX3DNode* wheels[4];
		wheels[0] = m_scene->FindDef(L"WHEEL0");
		wheels[1] = m_scene->FindDef(L"WHEEL1");
		wheels[2] = m_scene->FindDef(L"WHEEL2");
		wheels[3] = m_scene->FindDef(L"WHEEL3");

		{
			CComPtr<ILSFVec3f> translation;
			car->getField(L"translation", (ILX3DField**)&translation);

			CComPtr<ILSFRotation> rotation;
			car->getField(L"rotation", (ILX3DField**)&rotation);

			SFVec3f* position = static_cast<SFVec3f*>(translation.p);
			SFRotation* orientation = static_cast<SFRotation*>(rotation.p);

			//translation->getValue(posv);

			double distance = speed / fps;
			double gravity = - 9 / fps;

			gmVector3t<float> velocity;
			velocity[0] = -sin(heading)*distance;
			velocity[1] = gravity;
			velocity[2] = -cos(heading)*distance;

		//	Quat4d q = EulerToQuaternion(heading, 0, bank);

			double carlength = -5;

			BOOL collision;

			gmVector3t<float> dest_points[2];

			gmVector3t<float> points[2];
			points[0] = gmVector3t<float>(posv[0], posv[1], posv[2]);
			points[1] = gmVector3t<float>(carv[0], carv[1], carv[2]);

			dest_points[0] = gmVector3t<float>(posv[0] + velocity[0], posv[1] + velocity[1], posv[2] + velocity[2]);
			dest_points[1] = gmVector3t<float>(carv[0] + velocity[0], carv[1] + velocity[1], carv[2] + velocity[2]);

			int count = 0;
			do
			{

				collision = FALSE;

			//	double azimuth = heading;
			//	double elevation = bank;

				/*
				if (bank != 0)
				{
					MessageBeep(-1);
				}
				*/

				/*
				carv[2] = posv[2] + cos(azimuth) * cos(elevation) * -carlength;
				carv[0] = posv[0] + sin(azimuth) * cos(elevation) * -carlength;
				carv[1] = posv[1] + sin(elevation) * -carlength;
				*/

				for (int i = 0; i < m_scene->m_nodes.GetSize(); i++)
				{
					CComQIPtr<CLGeometryImplImpl> Wall = m_scene->m_nodes[i];//m_scene->FindDef(L"WALL");
					if (Wall)
					{
					//	while (fabs(distance(dest_points[1], dest_points[0])-carlength) > 0.0001)
						for (int i = 0; i < 2; i++)
						{
							/*
							float destx = posv[0] - sin(heading)*velocity;
							float desty = posv[1] - gravity;
							float destz = posv[2] - cos(heading)*velocity;
							*/

						//	BOOL b = FALSE;
						//	do
							{
						//		b = FALSE;

								gmVector3t<float> startpt = points[i];//(points[i][0], points[i][1], points[i][2]);
								gmVector3t<float> endpt = dest_points[i];//(dest_points[i][0], dest_points[i][1], dest_points[i][2]);

								gmVector3t<float> plane_normal;
								gmVector3t<float> plane_pt;

								if (Wall->Within(startpt, endpt, plane_normal, plane_pt))
								{
								//	b = TRUE;

									//if (i == 1)
									{
								//		MessageBeep(-1);
									}

								//	b = TRUE;

									gmVector3t<float> new_pt;
									new_pt = plane_pt + plane_normal / 20;//*(fabs(speed) + 0.0001) / 300;

#if 0
									if (i == 1)
									{
										/*
										gmVector3t<float> v = new_pt - dest_points[0];
										v.normalize();

										dest_points[1] = dest_points[0] + v*5;
										*/

										gmVector3t<float> v = new_pt - dest_points[0];
										v.normalize();

										dest_points[1] = dest_points[0] + v*5;
									}
									else
									{
										gmVector3t<float> v = new_pt - dest_points[1];
										v.normalize();

										dest_points[0] = dest_points[1] + v*5;
									}


									{
										gmVector3t<float> bar = dest_points[0] - dest_points[1];
										ASSERT(abs(bar.length()-5) < 0.0001);
									}
#endif
#if 0
									collision = TRUE;
									goto out;
	#endif

									if (FALSE)
									{
									//	dest_points[i] = new_pt;

	#if 0
										gmVector3t<float> offset = intersection - dest_points[i];

									//	points[0] = dest_points[0];
									//	points[1] = dest_points[1];

										dest_points[0] += offset;
										dest_points[1] += offset;

										goto out;
										collision = TRUE;
	#endif
									}
									else
									{
										dest_points[i] = new_pt;
										//collision = 
									}

		//#if 0
		//#endif
									//end = start;
									//speed = -speed/1.5;
								//	collision = TRUE;
								}
							}
						//	while (b);
						}
					}
				}
out:
				if (collision)
				{
					gmVector3t<float> start = dest_points[1];
					gmVector3t<float> bar = dest_points[0] - dest_points[1];
					double len = bar.length();

				//	ASSERT(fabs(len-5) < 0.0001);
#if 0

					if (fabs(len-5) > 0.0001)
					{
						gmVector3t<float> direction = bar;
						direction.normalize();
						double half = (5/len);

						dest_points[0] -= direction*half;
						//dest_points[1] += -direction*half;

						{
							gmVector3t<float> bar = dest_points[0] - dest_points[1];
							double len2 = bar.length();
						}
					}
#endif
				}

				count++;
			}
			while (collision);

			//if (collision)
			{
			//	bank += 0.01;
			//	double bank2 = GetLineAngle(dest_points[0][2], dest_points[0][1], dest_points[1][2], dest_points[1][1]);
				//double bank2 = GetLineAngle(dest_points[1][2], dest_points[1][1], dest_points[0][2], dest_points[0][1]);

				double bank2 = GetLineAngle(dest_points[0][2], dest_points[0][1], dest_points[1][2], dest_points[1][1]);

				/*
				if (bank2 >= M_PI)
				{
					bank2 -= M_PI;
					MessageBeep(-1);
				}
				*/

				/*
				if (bank2 > M_PI/2)
				{
					bank2 += M_PI;
				}
				*/

				bank = bank2;
			}

			//bank2 = - gmRadians(10);
			/*
			while (bank2 < 0)
				bank2 += M_PI;
			while (bank2 > M_PI)
				bank2 -= M_PI;
				*/
		//	bank2 -= M_PI;	// Account for negative (-2) as car length
			//heading

//			heading = GetLineAngle(dest_points[1][0], dest_points[1][2], dest_points[0][0], dest_points[0][2]) - M_PI/4;

			posv[0] = dest_points[0][0];//destx;
			posv[1] = dest_points[0][1];//desty;
			posv[2] = dest_points[0][2];//destz;

			carv[0] = dest_points[1][0];//destx;
			carv[1] = dest_points[1][1];//desty;
			carv[2] = dest_points[1][2];//destz;

			/*
			gmVector2 normal2 = gmVector2(p2[1] - p1[1], -(p2[0] - p1[0]));

			posv[0] = end[0];//destx;
			posv[1] = end[1];//desty;
			posv[2] = end[2];//destz;

			gmVector3t<float> normal = cross(a-b, c-b);
			*/

			position->m_value = posv;

			swprintf(buf, L"%d", (int)(bank*180/M_PI));

			if (heading != 0 || bank != 0)
			{
				Quat4d q = EulerToQuaternion(0, heading, bank);
				orientation->m_value = q.QuaternionToAxisAngle();
			}
			else
			{
				orientation->m_value.m_v[0] = 0;
				orientation->m_value.m_v[1] = 1;
				orientation->m_value.m_v[2] = 0;
				orientation->m_value.m_a = 0;
			}
#if 0
			orientation->m_value.m_v[0] = 0;
			orientation->m_value.m_v[1] = 1;
			orientation->m_value.m_v[2] = 0;
			orientation->m_value.m_a = heading;
#endif

			/*


			gmVector3 plane_normal = gmVector3(0, 0, -1);
			double plane_distance = dot(plane_normal, gmVector3(5, 5, 2));

			int pos_class = ClassifyPoint(plane_normal, plane_distance, start);
			int dest_class = ClassifyPoint(plane_normal, plane_distance, end);

			if (pos_class != dest_class)
			{
				CComPtr<ILX3DNode> crash = m_scene->FindDef(L"CRASH");
				if (crash)
				{
					CLAudioClip* pAudioClip = static_cast<CLAudioClip*>(static_cast<CLSound*>(crash.p)->m_source->m_value.p);
					pAudioClip->m_startTime->m_value = m_currentVideoTime;
					pAudioClip->Change(pAudioClip->m_startTime);
				}

				gmVector3 ray = start - end;
				double d = dot(plane_normal, ray);
				if (d != 0)
				{
					double t = - (dot(plane_normal, start) + plane_distance ) / d;

			//	end = start + (ray * t);
			//	side = pos_class;

					end = start;

			//	MessageBeep(-1);

					speed = -speed/2;
				}
			}
			*/

#if 0
			double rot = (steering - heading) * speed*2;// / 1000;
			if (rot)
			{
				SFRotation* rotation = static_cast<CLTransform*>(overstell.p)->m_R;

				Quat4d q = EulerToQuaternion(0, gmRadians(rot), 0/**/);//rotation->m_value.AxisAngleToQuaternion();//x, y, z, angle);
			//	q.CombineQuaternion(/*x, y, z, angle,*/ 0, gmRadians(rot), 0);
				rotation->m_value = q.QuaternionToAxisAngle();//x, y, z, angle);
			}
#endif

			CLViewpoint* pViewpoint = static_cast<CLViewpoint*>(m_scene->m_viewpointStack[m_scene->m_viewpointStack.size()-1]);

			if (FALSE)
			{
				pViewpoint->m_position->m_value[0] = posv[0];
				pViewpoint->m_position->m_value[1] = posv[1]+1.5;
				pViewpoint->m_position->m_value[2] = posv[2]+4;

				pViewpoint->m_orientation->m_value = orientation->m_value;
			}

			int gear;

			double gear_speed[7] =
			{
				KMH(0),
				KMH(15),
				KMH(30),
				KMH(60),
				KMH(100),
				KMH(150),
				KMH(180),
			};

			gmVector3t<float> velocity3 = velocity;//end-start;

			{
				double gearspeed = (int)(fabs(speed) * 6) / (int)MAX_SPEED;

			//	ASSERT(speed >= gearspeed);
				double diff = fabs(speed) - gearspeed;

				for (int i = 6; i >= 1; i--)
				{
					if (fabs(speed) >= gear_speed[i])
					{
						break;
					}
				}
				gear = i;
				ASSERT(gear >= 0 && gear < 7);
			}

			CComPtr<ILX3DNode> engine = m_scene->FindDef(L"ENGINE");
			if (engine)
			{
				CLSound* pSound = static_cast<CLSound*>(engine.p);
				CLAudioClip* pAudioClip = static_cast<CLAudioClip*>(pSound->m_source->m_value.p);

				if (pAudioClip->m_pAudioFilter)
				{
					pAudioClip->m_pAudioFilter->m_pInput->m_ds3DBuffer->SetMinDistance(60, DS3D_DEFERRED);
					pAudioClip->m_pAudioFilter->m_pInput->m_ds3DBuffer->SetPosition(posv[0], 1, posv[2], DS3D_DEFERRED);
					pAudioClip->m_pAudioFilter->m_pInput->m_ds3DBuffer->SetVelocity(velocity3[0], velocity3[1], velocity3[2], DS3D_DEFERRED);
					pAudioClip->m_pAudioFilter->m_pInput->m_dsBuffer8->SetFrequency(10000 + gear*100 + (speed - gear_speed[gear])*400);
				}
			}

			m_scene->m_soundListener->SetPosition(pViewpoint->m_position->m_value[0], pViewpoint->m_position->m_value[1], pViewpoint->m_position->m_value[2], DS3D_DEFERRED);

		//	m_scene->m_soundListener->SetOrientation(pViewpoint->m_position->m_value[0], pViewpoint->m_position->m_value[1], pViewpoint->m_position->m_value[2], DS3D_DEFERRED);

			m_scene->m_soundListener->CommitDeferredSettings();
		}

		double wheel_radius = 0.5;
		double circumference = 2*wheel_radius * M_PI;

		int i;
#if 0
		for (i = 2; i < 4; i++)
		{
			CComPtr<ILSFRotation> rotation;
			wheels[i]->getField(L"rotation", (ILX3DField**)&rotation);

			SFRotation* orientation = static_cast<SFRotation*>(rotation.p);

			double rotateX = steering;
			double rotateY = 0;//(speed / fps) * (circumference / 2*M_PI);
			double rotateZ = 0;

			Quat4d q = orientation->m_value.AxisAngleToQuaternion();//x, y, z, angle);
			//q.CombineQuaternion(/*x, y, z, angle,*/ (rotateZ), (rotateY), (rotateX));
			orientation->m_value = q.QuaternionToAxisAngle();//x, y, z, angle);

			/*
			float v[4];
			rotation->getValue(v);

			v[0] = 0;
			v[1] = 1;
			v[2] = 0;
			v[3] += (speed / fps) * (circumference / 2*M_PI);

			rotation->setValue(v);
			*/

		}
#endif
		double rotateY = (speed / fps) * (circumference / 2*M_PI);
		wheel_rotation += rotateY;

		for (i = 0; i < 4; i++)
		{
			CComPtr<ILSFRotation> rotation;
			wheels[i]->getField(L"rotation", (ILX3DField**)&rotation);

			SFRotation* orientation = static_cast<SFRotation*>(rotation.p);

			double rotateX = 0;//steering;
			double rotateZ = 0;

			double bank;
			if (i >= 2)
				bank = steering - heading;
			else
				bank = 0;

			//Quat4d q = orientation->m_value.AxisAngleToQuaternion();//x, y, z, angle);
			Quat4d q = EulerToQuaternion(0, 0, bank);
			q.CombineQuaternion(0, wheel_rotation, 0);
			//q.CombineQuaternion(/*x, y, z, angle,*/ (rotateZ), (rotateY), (rotateX));
			orientation->m_value = q.QuaternionToAxisAngle();//x, y, z, angle);

			/*
			float v[4];
			rotation->getValue(v);

			v[0] = 0;
			v[1] = 1;
			v[2] = 0;
			v[3] += (speed / fps) * (circumference / 2*M_PI);

			rotation->setValue(v);
			*/

		}

		static_cast<CLText*>(speedom.p)->m_string->m_items[0] = buf;
	}
#endif

	/*
	if (wParam == 1)
	{
		return 0;
	}
	*/

	for (int i = 0; i < m_scene->m_nodes.GetSize(); i++)
	{
		CComQIPtr<X3DNodeImplImpl> node(m_scene->m_nodes[i]);

	//	CComQIPtr<ILX3DTimeDependentNode> timeDependant = node;
	//	if (timeDependant)
		{
			node->DoTime(m_currentVideoTime);
		}
	}

	CLViewpoint* pViewpoint = NULL;
	if (m_scene->m_viewpointStack.size() > 0)
	{
		pViewpoint = static_cast<CLViewpoint*>(m_scene->m_viewpointStack[m_scene->m_viewpointStack.size()-1]);
	}

	if (pViewpoint)
	{
		for (i = 0; i < m_scene->m_nodes.GetSize(); i++)
		{
			X3DNode* node = m_scene->m_nodes[i];

			CLProximitySensor* pSensor = dynamic_cast<ILProximitySensor*>(node);
			if (pSensor)
			{
				//CLProximitySensor* pSensor = static_cast<CLProximitySensor*>(sensor.p);

				bool bInside =
					pViewpoint->m_position->m_value[0] >= pSensor->m_center->m_value[0] - pSensor->m_size->m_value[0]/2 &&
					pViewpoint->m_position->m_value[0] < pSensor->m_center->m_value[0] + pSensor->m_size->m_value[0]/2 &&
					pViewpoint->m_position->m_value[1] >= pSensor->m_center->m_value[1] - pSensor->m_size->m_value[1]/2 &&
					pViewpoint->m_position->m_value[1] < pSensor->m_center->m_value[1] + pSensor->m_size->m_value[1]/2 &&
					pViewpoint->m_position->m_value[2] >= pSensor->m_center->m_value[2] - pSensor->m_size->m_value[2]/2 &&
					pViewpoint->m_position->m_value[2] < pSensor->m_center->m_value[2] + pSensor->m_size->m_value[2]/2;

				if (bInside != pSensor->m_isActive->m_value)
				{
					MessageBeep(-1);

					if (bInside)
					{
						pSensor->m_enterTime->m_value = m_currentVideoTime;
						pSensor->Change(pSensor->m_enterTime);

						pSensor->m_isActive->m_value = TRUE;
						pSensor->Change(pSensor->m_isActive);
					}
					else
					{
						pSensor->m_exitTime->m_value = m_currentVideoTime;
						pSensor->Change(pSensor->m_exitTime);

						pSensor->m_isActive->m_value = FALSE;
						pSensor->Change(pSensor->m_isActive);
					}
				}
			}
		}
	}

	CWindowImpl<COGLSceneRenderer>::Invalidate();

	m_currentVideoTime += 1.0 / 33.0;

#if 0	// TODO, have something like this
	DWORD currentTime = GetTickCount();

	m_currentSpeed = 1000.0/(currentTime-oldTime);
	oldTime = currentTime;
#endif

	playcount++;
#endif
//	return 0;
#endif
}

COGLSceneRenderer::COGLSceneRenderer() : Gui::FrameworkElement(&s_Class)
{
	m_dragging = 0;
	m_display = 0;	// Smooth
//	m_display = 2;
	m_currentSpeed = 0;
	m_nKeyRepeat = 0;

	m_currentVideoTime = 0;

	m_scene = NULL;

	m_glState = new LDraw::GLState;
	m_clientState = new LDraw::ClientState;

#if USE_GL
	m_hrc = NULL;
	m_hOwner = NULL;
	m_style = WS_OVERLAPPED | WS_VISIBLE;
	SetRectEmpty(&m_rect);// = CRect(0,0,0,0);
#endif

#if 0
	ASSERT(m_hWnd == NULL);
//	RECT rc = {0,0,0,0};
	Create(NULL, LDraw::Rect(), NULL, WS_POPUP);
#endif

	//SetRetainedMode(false);
}

COGLSceneRenderer::~COGLSceneRenderer()
{
}
#endif

#if 0
void COGLSceneRenderer::SetOwnerWindow(UI::VisualContentSource* pWindow)
{
	if (GetOwnerWindow())
	{
		// killtimer
	}

	FrameworkElement::SetOwnerWindow(pWindow);

	if (GetOwnerWindow())
	{
		UI::SetTimer(1000/fps, bind1st_of_2(mem_fun(&COGLSceneRenderer::OnTimer), this));
	}
}

void COGLSceneRenderer::Run()
{
//	SetTimer(1, 1000/fps/*60*/);
}

bool COGLSceneRenderer::ProcessWindowMessage( MSWindows::Hwnd hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult/*, DWORD dwMsgMapID*/ )
{
	if (uMsg == WM_TIMER)
	{
		bool bHandled = true;
		lResult = OnTimer(uMsg, wParam, lParam, bHandled);
		return bHandled;
	}
	else
		return false;
}
#endif

Graphics::VertexDeclaration m_vertexDeclaration0;

// OpenGL
unsigned int m_glFixedVertexShader;
unsigned int m_glFixedFragmentShader;

unsigned int m_glFixedShaderProgram;

// D3D9
ID3DXEffect* m_d3d9_effect;
D3DXHANDLE m_d3d9_technique0;

D3DXHANDLE m_d3d9_modelViewVariable;
D3DXHANDLE m_d3d9_projectionVariable;
D3DXHANDLE m_d3d9_modelViewProjectionVariable;
D3DXHANDLE m_d3d9_modelViewITVariable;
D3DXHANDLE m_d3d9_modelViewProjectionITVariable;
D3DXHANDLE m_d3d9_nlightsVariable;

// D3D10

/*
ID3D10Effect* m_d3d10_effect;

ID3D10EffectVectorVariable* m_d3d10_viewPositionVariable;

ID3D10EffectMatrixVariable* m_d3d10_viewVariable;
ID3D10EffectMatrixVariable* m_d3d10_modelVariable;
ID3D10EffectMatrixVariable* m_d3d10_projectionVariable;
ID3D10EffectMatrixVariable* m_d3d10_modelViewVariable;
ID3D10EffectMatrixVariable* m_d3d10_modelViewProjectionVariable;
ID3D10EffectMatrixVariable* m_d3d10_modelViewITVariable;
ID3D10EffectMatrixVariable* m_d3d10_modelViewProjectionITVariable;

ID3D10EffectVariable* m_d3d10_materialVariable;

ID3D10EffectScalarVariable* m_d3d10_nlightsVariable;
ID3D10EffectVariable* m_d3d10_lightsVariable;

ID3D10EffectScalarVariable* m_d3d10_ntexturesVariable;
ID3D10EffectShaderResourceVariable* m_d3d10_texturesVariable;

ID3D10EffectScalarVariable* m_d3d10_fogTypeVariable;
ID3D10EffectVectorVariable* m_d3d10_fogColorVariable;
ID3D10EffectScalarVariable* m_d3d10_fogDistanceSquaredVariable;
*/

class TextFile
{
public:
	CTOR TextFile(StringIn url)
	{
		Net::UrlBinding file;
		_Ptr<IO::Stream> stream = file.BindToObject(url, nullptr);
		if (stream != nullptr)
		{
			int filesize = stream->GetSize32();
			if (filesize < 0) raise(Exception("File too large"));

			m_buffer = new char[filesize+1];
			stream->Read(m_buffer, filesize);
			m_buffer[filesize] = 0;

			m_size = filesize;
		}
	}

	char* m_buffer;
	int m_size;
};

#if 0
class DrawCommand : public LDraw::Command
{
public:

	virtual void Render(LDraw::GraphicsO* pG);

	X3DScene* m_scene;
	LDraw::SizeD m_actualSize;
};

void DrawCommand::Render(Graphics::GraphicsO* pG)
{
	ASSERT(0);
#if 0
	RenderImplImpl* render = dynamic_cast<RenderImplImpl*>(m_scene->m_root);
	if (render)
	{
		X3DDrawContext xdc;
		//xdc.m_pGraphics = pGraphics;

		xdc.m_glFixedShaderProgram = m_glFixedShaderProgram;

		/*
		pGraphics->TranslateTransform(100, 100);
		pGraphics->ScaleTransform(1, -1);
		*/
	//	pGraphics->ScaleTransform(0.3, 0.3);

		//pGraphics->ScaleTransform(m_computedSize.Width, 1/m_computedSize.Height);
		//glViewport(0, 0, rc.right, rc.bottom);

		if (true)
		{
		//	pGraphics->TranslateTransform(-0.5, -0.5);

			/*
			if (pGraphics->m_p->GetMode() == 1)
			{
				m_pGraphics3D = new LDraw::GraphicsO_3D(dynamic_cast<LDraw::GraphicsO*>(pGraphics->m_p));
			}
			else
			*/

		//	LDraw::Graphics3D* m_pGraphics3D;
		//	m_pGraphics3D = p3D;

			Viewpoint* pViewpoint = NULL;

			if (m_scene->m_viewpointStack.GetSize() > 0)
			{
				pViewpoint = static_cast<Viewpoint*>(m_scene->m_viewpointStack[m_scene->m_viewpointStack.GetSize()-1]);
			}
			else
			{
				// hmm...
			}

			LDraw::GraphicsO_3D* m_pGraphics3D = new LDraw::GraphicsO_3D(pG);

			//LDraw::Graphics3D graphics(pGraphics);
			xdc.m_pGraphics3D = m_pGraphics3D;
		//	m_pGraphics3D->m_pGraphics = pGraphics;

		//	LDraw::SizeD actualSize = get_ActualSize();

			double w = m_actualSize.Width;
			double h = m_actualSize.Height;

#if 0
			m_pGraphics3D->Viewport(0, 0, w, h);
#endif

			m_pGraphics3D->SetMatrixMode(GL_PROJECTION);
			m_pGraphics3D->LoadIdentity();
	//		glLoadMatrixd(projm);
		//
			double fov;
			if (pViewpoint)
			{
				fov = pViewpoint->getFieldOfView();
			}
			else
			{
				fov = M_PI/4;
			}

			if (true)
			{
			//	m_pGraphics3D->gluPerspective(gmDegrees(fov), (double)w / h, 0.08, 0.2);

				double aspect = (double)w / h;
				double zNear = 0.5;
				double zFar = 500;

			//	double top = zNear * tan( gmDegrees(fov) * M_PI / 360.0 );
				double top = zNear * tan(fov / 2);
				double bottom = -top;

				double left = bottom * aspect;
				double right = top * aspect;

				m_pGraphics3D->Frustum(left, right, bottom, top, zNear, zFar);

			//	m_pGraphics3D->glTranslatef(0, 0, -1);
			}
			else
			{
#if 0
				float aspect = (GLfloat)h / (GLfloat)w;

				int pixelWidth = 400;
				int pixelHeight = 300;

		//	glFrustum(-(float)pixelWidth/2, (float)pixelWidth/2, -aspect*pixelWidth/2, aspect*pixelWidth/2, 1.0, 10.0);
			//glFrustum(-1, 1, -aspect, aspect, 1.0, 1000.0);

				aspect = (float)pixelWidth / (float)pixelHeight;
				gluOrtho2D(-(float)pixelWidth/2, (float)pixelHeight/2, -(float)pixelHeight/2, (float)pixelHeight/2);
#endif
			}

			m_pGraphics3D->SetMatrixMode(GL_MODELVIEW);
			m_pGraphics3D->LoadIdentity();

			// NavigationInfo
			bool headlight;

			if (m_scene->m_navigationinfoStack.GetSize() > 0)
			{
				NavigationInfo* pNavigationInfo = static_cast<NavigationInfo*>(m_scene->m_navigationinfoStack[0]);

				headlight = pNavigationInfo->getHeadlight();
			}
			else
			{
				// Default value
				headlight = false;
			}

			if (m_scene->m_fogStack.GetSize() > 0)
			{
				Fog* pFog = static_cast<Fog*>(m_scene->m_fogStack[0]);

				pFog->Apply(&xdc);
			}

			m_pGraphics3D->Enable(GL_LIGHTING);
			m_pGraphics3D->Enable(GL_NORMALIZE);

			if (headlight)
			{
#if 0
				float light_direction[4] = { 0, 0, 1, 0};	// directional
				float color[4] = {1, 1, 1, 1};
				float ambient[4] = {0, 0, 0, 1};

				m_pGraphics3D->Enable(GL_LIGHT0+xdc.m_nLight);

				m_pGraphics3D->Lightfv(GL_LIGHT0+xdc.m_nLight, GL_POSITION, light_direction);
				m_pGraphics3D->Lightfv(GL_LIGHT0+xdc.m_nLight, GL_AMBIENT, ambient);
				m_pGraphics3D->Lightfv(GL_LIGHT0+xdc.m_nLight, GL_DIFFUSE, color);
				m_pGraphics3D->Lightfv(GL_LIGHT0+xdc.m_nLight, GL_SPECULAR , color);
				
				xdc.m_nLight++;
#endif
			}

			if (pViewpoint)
			{
				D3DXMATRIX global_transformMatrixInverse = LDraw::matrix4f::getIdentity();

				X3DNode* parent = pViewpoint->GetFirstParentNodeOfFieldName(WSTR("children"));
				while (parent)
				{
					Transform* transform = dynamic_cast<Transform*>(parent);
					if (transform)
					{
						global_transformMatrixInverse *= transform->GetTransform();
					}

					parent = parent->GetFirstParentNodeOfFieldName(WSTR("children"));
				}

				AxisAngle orientation = pViewpoint->getOrientation();
				LDraw::vector3f position = pViewpoint->getPosition();
				LDraw::vector3f axis = orientation.m_axis;
			//	axis.normalize(); // should be done already

				/*
				m_pGraphics3D->glRotate(gmDegrees(orientation.m_angle), -axis);
				m_pGraphics3D->glTranslate(-position);
				*/

				global_transformMatrixInverse *= LDraw::matrix4f::getTranslate(position);
				global_transformMatrixInverse *= LDraw::matrix4f::getRotate(gmDegrees(orientation.m_angle), axis);

				m_pGraphics3D->MultMatrixf(global_transformMatrixInverse.getInverse());
			}
			else
			{
				// TODO
			//	glTranslatef(0, 0, -180);
				//glTranslatef(0, 0, -10);
			}

			/*
			{
				gmVector3d v = m_pGraphics->m_matrix[1].transform(gmVector3d(0,0,-1));
				double m_w = v[2];
			}
			*/

			if (false)
			{
				double eqn[4] = { 0.5, 1.0, 0.0, 0.0 };

				m_pGraphics3D->ClipPlane(GL_CLIP_PLANE0, eqn);
				m_pGraphics3D->Enable(GL_CLIP_PLANE0);
			}

			if (false)
			{
				double eqn[4] = { 0, 1.0, 0.0, 0.0 };

				m_pGraphics3D->ClipPlane(GL_CLIP_PLANE1, eqn);
				m_pGraphics3D->Enable(GL_CLIP_PLANE1);
			}

			/*
			if (false)
			{
				double eqn[4] = { 1.0, 0.0, 0.0, 0.0 };

				m_pGraphics->glClipPlane(GL_CLIP_PLANE1, eqn);
				m_pGraphics->glEnable(GL_CLIP_PLANE1);
			}
			*/

			if (true)
			{
				m_pGraphics3D->DepthMask(true);
				m_pGraphics3D->Enable(GL_DEPTH_TEST);
			}

		//	m_pGraphics3D->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//	m_pGraphics3D->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	Face* faces;
		//	int nfaces = 0;

			m_pGraphics3D->GetFloatv(GL_MODELVIEW_MATRIX, xdc.m_modelView);
			m_pGraphics3D->GetFloatv(GL_PROJECTION_MATRIX, xdc.m_projection);

			xdc.m_modelViewI = xdc.m_modelView.getInverse();
			xdc.m_projectionI = xdc.m_projection.getInverse();

			xdc.m_modelViewT = xdc.m_modelView.getTranspose();
			xdc.m_projectionT = xdc.m_projection.getTranspose();

			xdc.m_modelViewIT = xdc.m_modelViewI.getTranspose();
			xdc.m_projectionIT = xdc.m_projectionI.getTranspose();

			if (true)
			{
				render->DrawGlobalLights(&xdc);

				render->Draw(&xdc);
			}
			else
			{
				ASSERT(0);
#if 0
			xdc.m_vertices.clear();
			xdc.m_colors.clear();
			xdc.m_indices.clear();
			xdc.m_faces.clear();

			render->DrawGlobalLights(&xdc);
			render->Draw(&xdc);

			/*
			for (int i = 0; i < xdc.m_faces.size(); i++)
			{
				Face& face = xdc.m_faces[i];

				face.minz = std::_MIN(
					xdc.m_vertices[face.vertexIndex[0]].m_winz,
					std::_MIN(xdc.m_vertices[face.vertexIndex[1]].m_winz,
									xdc.m_vertices[face.vertexIndex[2]].m_winz));
			}
			*/

			bspnode* rootbsp = buildbsp(xdc.m_vertices, xdc.m_colors, xdc.m_faces);

			LDraw::vector3f viewpoint = pViewpoint->getPosition();	// TODO

			LDraw::Graphics3D* p3D = xdc.m_pGraphics3D;
			xdc.m_pGraphics3D->Disable(GL_DEPTH_TEST);
//				m_pGraphics3D->glEnable(GL_DEPTH_TEST);
			xdc.m_pGraphics3D->DepthMask(false);
		//	pDC->m_pGraphics3D->glDisable(GL_DEPTH_TEST);
			xdc.m_pGraphics3D->Enable(GL_BLEND);
			xdc.m_pGraphics3D->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			xdc.m_pGraphics3D->MatrixMode(GL_MODELVIEW);
			xdc.m_pGraphics3D->LoadIdentity();

			xdc.m_pGraphics3D->Disable(GL_LIGHTING);
			xdc.m_pGraphics3D->glBegin(GL_TRIANGLES);
			drawbsp(xdc.m_pGraphics3D, viewpoint, xdc.m_vertices, xdc.m_colors, rootbsp);
			xdc.m_pGraphics3D->glEnd();

#if 0
			z_sort sorter;
			sorter.m_faces = xdc.m_faces.begin();

			std::sort(xdc.m_indices.begin(), xdc.m_indices.end(), sorter);
		//	xdc.m_pGraphics3D->glDrawElements(GL_TRIANGLES, xdc.m_indicesCount, GL_UNSIGNED_INT, xdc.m_indices);

			LDraw::Vertex verticesBuffer[60];
			signed char loc[40];

			for (int i = 0; i < xdc.m_indices.size(); i++)
			{
				int faceIndex = xdc.m_indices[i];
				const Face& face = xdc.m_faces[faceIndex];

				LDraw::Vertex v[3];

				LDraw::Vertex v2[3];

				LDraw::vector4f plane(0, 0, 1, 0);

				int vertexCount = 3;

				vertexCount = LDraw::ClipPolygonAgainstPlane(vertexCount, v, 0, plane, loc, verticesBuffer);
			}

			LDraw::GraphicsO_3D* p3D = ((LDraw::Graphics3DImmediate*)xdc.m_pGraphics3D)->m_pG;

			xdc.m_pGraphics3D->glDisable(GL_DEPTH_TEST);
//				m_pGraphics3D->glEnable(GL_DEPTH_TEST);
			xdc.m_pGraphics3D->glDepthMask(false);
		//	pDC->m_pGraphics3D->glDisable(GL_DEPTH_TEST);
			xdc.m_pGraphics3D->glEnable(GL_BLEND);
			xdc.m_pGraphics3D->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for (int i = 0; i < xdc.m_indices.size(); i++)
			{
				int faceIndex = xdc.m_indices[i];
				const Face& face = xdc.m_faces[faceIndex];

				p3D->triangle2(
					xdc.m_vertices[face.vertexIndex[0]],
					xdc.m_vertices[face.vertexIndex[1]],
					xdc.m_vertices[face.vertexIndex[2]]);
			}
#endif
#endif
			}

#if 0
			p->ReleaseGraphics();
#endif
		}
#if 0
		else
		{
		//	ASSERT(0);
		//	xdc.m_pGraphics2D = pGraphics;

		//	pGraphics->SetSmoothingMode(LDraw::SmoothingModeAntiAlias);

			pGraphics->TranslateTransform(100, 100);
			pGraphics->ScaleTransform(1, -1);

			render->Draw(&xdc);
		}
#endif
	}
#endif
}
#endif

ID3D10EffectTechnique* m_d3d10_technique0;
ID3D10InputLayout* g_pVertexLayout;

ID3D10EffectTechnique* m_d3d10_techniqueTex;
ID3D10InputLayout* g_pVertexLayoutTex;

/*
void SceneVisual::InitStacks()
{
	for (unsigned int i = 0; i < m_scene->m_nodes.GetSize(); i++)
	{
		if (Viewpoint* viewpoint = dynamic_cast<Viewpoint*>(m_scene->m_nodes[i]))
		{
			if (m_scene->m_viewpointStack.GetSize() == 0)
			{
				Viewpoint* pViewpoint = viewpoint;
				pViewpoint->m_set_bind->m_value = true;
				pViewpoint->Change(pViewpoint->m_set_bind);

				//m_scene->m_viewpointStack.Add(viewpoint.Detach());
			}
		}
		else if (NavigationInfo* navigationinfo = dynamic_cast<NavigationInfo*>(m_scene->m_nodes[i]))
		{
			if (m_scene->m_navigationinfoStack.GetSize() == 0)
			{
				m_scene->m_navigationinfoStack.Add(navigationinfo);
			}
		}
		else if (Fog* fog = dynamic_cast<Fog*>(m_scene->m_nodes[i]))
		{
			if (m_scene->m_fogStack.GetSize() == 0)
			{
				m_scene->m_fogStack.Add(fog);
			}
		}
	}

	if (m_scene->m_viewpointStack.GetSize() == 0)
	{
		// Create a default viewpoint with default parameters
		Viewpoint* pViewpoint = dynamic_cast<Viewpoint*>(m_scene->createNode(WSTR("Viewpoint")));

		pViewpoint->m_set_bind->m_value = true;
		pViewpoint->Change(pViewpoint->m_set_bind);

	//	m_viewpointStack.Add(viewpoint.Detach());
	}

	{
		Viewpoint* pViewpoint = static_cast<Viewpoint*>(m_scene->m_viewpointStack[0]);
	// Initialize these
		m_newPosition = pViewpoint->m_position->m_value;
		m_newOrientation = pViewpoint->m_orientation->m_value;
	}

	if (m_scene->m_navigationinfoStack.GetSize() == 0)
	{
		// Create a default viewpoint with default parameters
		NavigationInfo* navigationinfo = dynamic_cast<NavigationInfo*>(m_scene->createNode(WSTR("NavigationInfo")));

		m_scene->m_navigationinfoStack.Add(navigationinfo);
	}
}
*/

Gui::DependencyClass* SceneVisual3D::get_Class()
{
	static Gui::DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static Gui::DependencyProperty* properties[] =
	{
		get_CameraProperty(),
		get_MultiSampleProperty(),
		get_SelectionProperty(),
	};

	return &depclass;
}

Gui::DependencyClass* SceneVisual3D::pClass(get_Class());

IMP_DEPENDENCY_PROPERTY(ICamera*, SceneVisual3D, Camera, nullptr)
IMP_DEPENDENCY_PROPERTY(bool, SceneVisual3D, MultiSample, true)
IMP_DEPENDENCY_PROPERTY(X3DNodeSelection*, SceneVisual3D, Selection, nullptr)

SceneVisual3D::SceneVisual3D() : Gui::FrameworkElement(get_Class())
{
}

SceneVisual3D::SceneVisual3D(X3DScene* scene) : Gui::FrameworkElement(get_Class())
{
	set_Scene(scene);
}

void SceneVisual3D::OnComputedPropertyValueChanged(Gui::PropertyValue* pProperty, bool handled)
{
	if (pProperty->m_dp == get_SelectionProperty())
	{
		get_Selection()->SelectionChanged.Connect(EventHandler(this, &SceneVisual3D::OnSelectionChanged));
//		MessageBeep(-1);
		InvalidateVisual();
	}

	Gui::FrameworkElement::OnComputedPropertyValueChanged(pProperty, handled);
}

void SceneVisual3D::OnSelectionChanged(X3DNodeSelection* selection)
{
	InvalidateVisual();
}

class Hit
{
public:

	CTOR Hit(X3DNode* node, Vec3f localpoint) : m_node(node), m_localpoint(localpoint),
		m_axis(0)
	{
	}

	CTOR Hit(X3DNode* node, Vec3f localpoint, int axis, double axis_t) : m_node(node), m_localpoint(localpoint),
		m_axis(axis), m_axis_t(axis_t)
	{
	}

	X3DNode* m_node;
	Vec3f m_localpoint;
	int m_axis;
	double m_axis_t;
};

class HitTestResult : public IHitTest
{
public:
	virtual void OnHit(X3DNode* node, int type, Vec3f v) override;
	virtual void OnHit(X3DNode* node, int type, Vec3f localpoint, int axis, double t) override;

	vector<Hit> m_hits;
};

void HitTestResult::OnHit(X3DNode* node, int type, Vec3f localpoint)
{
	m_hits.push_back(Hit(node, localpoint));
}

void HitTestResult::OnHit(X3DNode* node, int type, Vec3f localpoint, int axis, double axis_t)
{
	m_hits.push_back(Hit(node, localpoint, axis, axis_t));
}

HitTestResult hittest;
Vec3f m_startlocation;

void SceneVisual3D::OnMouseLeftButtonDown(Gui::MouseButtonEventArgs* args)
{
	Vec2f mousepos = args->GetPosition(this);

	X3DScene* scene = get_Scene();

	Gui::Window* container = dynamic_cast<Gui::Window*>(GetRoot());

	// TODO
	gm::SizeF winsize = container->GetSize();
	float w = winsize.Width;
	float h = winsize.Height;

	ICamera* camera = get_Camera();

	X3DViewpointNode* pViewpoint = m_scene->get_Viewpoint();
	if (camera == nullptr)
	{
		camera = pViewpoint;
	}

	if (camera == nullptr)
	{
		return;
	}

	if (!m_mousedown)
	{
		D3D10_VIEWPORT vp;
		vp.Width = int(w);
		vp.Height = int(h);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		float zNear = 0.5f;
		float zFar = 800.0f;

		D3DXMATRIX proj = camera->GetProjectionMatrixRH(w, h, zNear, zFar);
		D3DXMATRIX view = GetViewMatrix();
	//	D3DXMATRIX view = camera->GetViewMatrix();

		D3DXMATRIX model;
		D3DXMatrixIdentity(&model);

	//	D3DXVECTOR3 out;
	//	D3DXVECTOR3 v(mousepos[0], mousepos[1], 0);
	//	D3DXVec3Unproject(&out, &v, &vp, &proj, &view, &world);

	//	out.x *= w;
	//	out.y *= h;

		hittest.m_hits.clear();

		Vec3f out;

		X3DScene* scene = get_Scene();
		scene->m_root->HitTest(proj, view, model, vp, 1, mousepos, out, &hittest);
		if (hittest.m_hits.size())
		{
			m_startmousepos = mousepos;
			m_oldmousepos = mousepos;

			m_startlocation = dynamic_cast<IPositionable3f*>(hittest.m_hits[0].m_node)->getPosition3f();

			m_mousedown = 1;
			CaptureMouse();
		}
	}
}

void SceneVisual3D::OnMouseLeftButtonUp(Gui::MouseButtonEventArgs* args)
{
	if (m_mousedown)
	{
		m_mousedown = 0;
		ReleaseMouseCapture();
	}
}

void SceneVisual3D::OnMouseRightButtonDown(Gui::MouseButtonEventArgs* args)
{
	if (!m_mousedown)
	{
		m_mousedown = 2;
		CaptureMouse();
	}
}

void SceneVisual3D::OnMouseRightButtonUp(Gui::MouseButtonEventArgs* args)
{
	if (m_mousedown)
	{
		m_mousedown = 0;
		ReleaseMouseCapture();
	}
}

void SceneVisual3D::OnMouseMove(Gui::MouseEventArgs* args)
{
	Vec2f mousepos = args->GetPosition(this);

	X3DScene* scene = get_Scene();

	Gui::Window* container = dynamic_cast<Gui::Window*>(GetRoot());

	// TODO
	gm::SizeF winsize = container->GetSize();
	float w = winsize.Width;
	float h = winsize.Height;

	ICamera* camera = get_Camera();

	X3DViewpointNode* pViewpoint = m_scene->get_Viewpoint();
	if (camera == NULL)
	{
		camera = pViewpoint;
	}

	if (m_mousedown == 1)
	{
		D3D10_VIEWPORT vp;
		vp.Width = int(w);
		vp.Height = int(h);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		float zNear = 0.5f;
		float zFar = 800.0f;

		D3DXMATRIX proj = camera->GetProjectionMatrixRH(w, h, zNear, zFar);
		D3DXMATRIX view = GetViewMatrix();

		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);

		D3DXVECTOR3 startmousevec;
		D3DXVec3Unproject(&startmousevec, &D3DXVECTOR3(m_startmousepos[0], m_startmousepos[1], hittest.m_hits[0].m_localpoint[2]), &vp, &proj, &view, &world);

		D3DXVECTOR3 mousevec;
		D3DXVec3Unproject(&mousevec, &D3DXVECTOR3(mousepos[0], mousepos[1], hittest.m_hits[0].m_localpoint[2]), &vp, &proj, &view, &world);

		IPositionable3f* node = dynamic_cast<IPositionable3f*>(hittest.m_hits[0].m_node);
		ISizeable3f* sizeable = dynamic_cast<ISizeable3f*>(hittest.m_hits[0].m_node);

	//	Vec3f translate(_v1.x - _v0.x, _v1.y - _v0.y, _v1.z - _v0.z);
		Vec3f location = node->getPosition3f();

		gm::matrix4f mat;

		if (hittest.m_hits[0].m_axis)
		{
			int m_transformMode = 2;

			if (m_transformMode & 1)	// Translate
			{
			//	m_dragX = 0;
			//	m_dragY = 0;
			//	m_dragZ = 0;

				if (hittest.m_hits[0].m_axis == 1)
				{
					float translateX = mousevec[0] - startmousevec[0];
				//	m_dragX = translateX;

					mat = gm::matrix4f::getTranslate(translateX, 0, 0);
				}
				else if (hittest.m_hits[0].m_axis == 2)
				{
					float translateY = mousevec[1] - startmousevec[1];
				//	m_dragY = translateY;

					mat = gm::matrix4f::getTranslate(0, translateY, 0);
				}
				else if (hittest.m_hits[0].m_axis == 3)
				{
					float translateZ = mousevec[2] - startmousevec[2];
				//	m_dragZ = translateZ;

					mat = gm::matrix4f::getTranslate(0, 0, translateZ);
				}

				node->setPosition3f(mat.transform(m_startlocation));
			}
			else	// Scale
			{
				Vec3f pivotPoint(0, 0, 0);

				float scaleX = 1.0f;
				float scaleY = 1.0f;
				float scaleZ = 1.0f;

				if (hittest.m_hits[0].m_axis == 1)
				{
					scaleX = (mousevec[0] / startmousevec[0]);

					mat = gm::matrix4f::getTranslate(pivotPoint) *
							gm::matrix4f::getScale(scaleX, 1, 1) *
							gm::matrix4f::getTranslate(-pivotPoint);
				}
				else if (hittest.m_hits[0].m_axis == 2)
				{
					scaleY = mousevec[1] / startmousevec[1];

					mat = gm::matrix4f::getTranslate(pivotPoint) *
							gm::matrix4f::getScale(1, scaleY, 1) *
							gm::matrix4f::getTranslate(-pivotPoint);
				}
				else if (hittest.m_hits[0].m_axis == 3)
				{
					scaleZ = mousevec[2] / startmousevec[2];

					mat = gm::matrix4f::getTranslate(pivotPoint) *
							gm::matrix4f::getScale(1, 1, scaleZ) *
							gm::matrix4f::getTranslate(-pivotPoint);
				}

				sizeable->setSize3f(Vec3f(scaleX, scaleY, scaleZ));
			}
		}
		else
		{
			D3DXVECTOR3 diff = mousevec - startmousevec;
			mat = gm::matrix4f::getTranslate(*(Vec3f*)&diff);
			node->setPosition3f(mat.transform(m_startlocation));
		}

		/*
		SFRotation* orientation = scene->get_Viewpoint()->getOrientationField();

		Quat4d q = orientation->getValue().ToQuat();

		Vec2f diff = mousepos - m_mousepos;

		q.Combine(0, diff[0]/30, diff[1]/30);

		orientation->setValue(q.ToAxisAngle());
		*/
	}
	else if (m_mousedown == 2)
	{
		SFVec3f* positionField = scene->get_Viewpoint()->getPositionField();
		SFRotation* orientationField = scene->get_Viewpoint()->getOrientationField();

		Vec3f position = positionField->getValue();
		Rotation orientation = orientationField->getValue();

		Vec2f diff = mousepos - m_oldmousepos;

		D3DXMATRIX m;
		D3DXMatrixRotationAxis(&m, (D3DXVECTOR3*)&orientation.m_axis, orientation.m_angle);

		Vec3f right(m._11, m._12, m._13);
		Vec3f top(m._21, m._22, m._23);
		Vec3f front(m._31, m._32, m._33);

		position += right*diff[0];
		position += top*diff[1];

		positionField->setValue(position);
	}

	m_oldmousepos = mousepos;
}

X3DScene* SceneVisual3D::get_Scene()
{
	return m_scene;
}

void SceneVisual3D::set_Scene(X3DScene* scene)
{
	if (m_scene)
	{
		ASSERT(0);
	}

	m_scene = scene;

	if (m_scene)
	{
		if (m_scene->m_defaultView == NULL)
		{
			m_scene->m_defaultView = this;
		}

		m_scene->m_root->addActors();

		m_scene->get_Invalidated().Connect(EventHandler(this, &SceneVisual3D::OnSceneInvalidate));
	}
}

X3DEXT void ReloadEffect()
{
//	m_d3d10_effect = NULL;
}

D3DXMATRIX SceneVisual3D::GetViewMatrix()
{
	D3DXMATRIX view;

	ICamera* camera = get_Camera();
	X3DViewpointNode* pViewpoint = get_Scene()->get_Viewpoint();

	if (camera)
	{
		D3DXMATRIX global_transformMatrixInverse = camera->GetViewMatrixI();
		D3DXMatrixInverse(&view, nullptr, &global_transformMatrixInverse);
	//	D3DXMatrixIdentity(&view);
	}
	else if (pViewpoint)
	{
		camera = pViewpoint;

		D3DXMATRIX global_transformMatrixInverse;// = LDraw::matrix4f::getIdentity();
		D3DXMatrixIdentity(&global_transformMatrixInverse);

		X3DNode* parent = pViewpoint->GetFirstParentNodeOfFieldName("children");
		while (parent)
		{
			Transform* transform = dynamic_cast<Transform*>(parent);
			if (transform)
			{
				global_transformMatrixInverse *= transform->GetTransform();
			}

			parent = parent->GetFirstParentNodeOfFieldName("children");
		}

		AxisAngle orientation = pViewpoint->getOrientation();
		Vec3f position = pViewpoint->getPosition();
		Vec3f axis = orientation.m_axis;
	//	axis.normalize(); // should be done already

		/*
		m_pGraphics3D->glRotate(gmDegrees(orientation.m_angle), -axis);
		m_pGraphics3D->glTranslate(-position);
		*/

		global_transformMatrixInverse = MatrixTranslation(position) * global_transformMatrixInverse;
		global_transformMatrixInverse = MatrixRotationAxis(axis, orientation.m_angle) * global_transformMatrixInverse;

		//m_pGraphics3D->MultMatrixf(global_transformMatrixInverse.getInverse());

		D3DXMatrixInverse(&view, nullptr, &global_transformMatrixInverse);
		//world = global_transformMatrixInverse.getInverse();
	}

	return view;
}

void SceneVisual3D::Render(Gui::ManagedRenderContext renderContext)
{
	if (get_Selection() == nullptr)
	{
		set_Selection(new X3DNodeSelection());
	}

	X3DScene* scene = get_Scene();

	Graphics::Direct3D10_I* d3d10 = renderContext.m_p->GetRT()->m_d3d10;
	if (d3d10)
	{
		d3d10->Load2();

		// Set the input layout
	//	d3d10->m_device->IASetInputLayout(g_pVertexLayout);
	}
	else	// OpenGL
	{
		Graphics::OpenGL_I* pGL = renderContext.m_p->GetRT()->m_opengl;

		if (m_glFixedVertexShader == 0)
		{
			TextFile source(WSTR("D:/users/sigurd/documents/vsprojects/mmstudio/FixedVertexShader.glsl"));

			m_glFixedVertexShader = pGL->m_glprocs->glCreateShader(GL_VERTEX_SHADER);
			ASSERT(::glGetError() == 0);

			if (m_glFixedVertexShader)
			{
				pGL->m_glprocs->glShaderSource(m_glFixedVertexShader, 1/*number of strings*/, (const char**)&source.m_buffer, &source.m_size);
				ASSERT(::glGetError() == 0);

				pGL->m_glprocs->glCompileShader(m_glFixedVertexShader);

				GLint compiled;
				pGL->m_glprocs->glGetShaderiv(m_glFixedVertexShader, GL_COMPILE_STATUS, &compiled);

				if (!compiled)
				{
					char buffer[512];
					int length;
					pGL->m_glprocs->glGetShaderInfoLog(m_glFixedVertexShader, 512, &length, buffer);

					VERIFY(0);
				}
			}
		}

		if (m_glFixedFragmentShader == 0)
		{
			TextFile source(WSTR("D:/users/sigurd/documents/vsprojects/mmstudio/FixedFragmentShader.glsl"));

			m_glFixedFragmentShader = pGL->m_glprocs->glCreateShader(GL_FRAGMENT_SHADER);
			ASSERT(::glGetError() == 0);

			if (m_glFixedFragmentShader)
			{
				pGL->m_glprocs->glShaderSource(m_glFixedFragmentShader, 1/*number of strings*/, (const char**)&source.m_buffer, &source.m_size);
				ASSERT(::glGetError() == 0);

				pGL->m_glprocs->glCompileShader(m_glFixedFragmentShader);

				GLint compiled;
				pGL->m_glprocs->glGetShaderiv(m_glFixedFragmentShader, GL_COMPILE_STATUS, &compiled);

				if (!compiled)
				{
					VERIFY(0);
				}
			}
		}

		if (m_glFixedShaderProgram == 0)
		{
			m_glFixedShaderProgram = pGL->m_glprocs->glCreateProgram();

			pGL->m_glprocs->glAttachShader(m_glFixedShaderProgram, m_glFixedVertexShader);

			pGL->m_glprocs->glLinkProgram(m_glFixedShaderProgram);
			ASSERT(::glGetError() == 0);

			GLint linked;
			pGL->m_glprocs->glGetProgramiv(m_glFixedShaderProgram, GL_LINK_STATUS, &linked);
			if (!linked)
			{
				VERIFY(0);
			}
		}
	}

	if (m_scene)
	{
		RenderImplImpl* render = dynamic_cast<RenderImplImpl*>(m_scene->m_root);
		if (render)
		{
			X3DDrawContext xdc;
			xdc.m_renderContext = dynamic_cast<Gui::ImmediateRenderContext*>(renderContext.m_p);
			xdc.m_glFixedShaderProgram = m_glFixedShaderProgram;

		//	xdc.m_modelView = renderContext.m_p->m_modelViewMatrix;

			ICamera* camera = get_Camera();

			X3DViewpointNode* pViewpoint = scene->get_Viewpoint();
			NavigationInfo* pNavigationInfo = scene->get_NavigationInfo();

			Gui::Window* container = dynamic_cast<Gui::Window*>(GetRoot());

			// TODO
			gm::SizeF winsize = container->GetSize();
			float w = winsize.Width;
			float h = winsize.Height;

#if 0
			m_pGraphics3D->Viewport(0, 0, w, h);
#endif

			D3DXMATRIX world;

			if (camera)
			{
				D3DXMATRIX global_transformMatrixInverse = camera->GetViewMatrixI();
				D3DXMatrixInverse(&world, NULL, &global_transformMatrixInverse);
			}
			else if (pViewpoint)
			{
				camera = pViewpoint;

				D3DXMATRIX global_transformMatrixInverse;// = LDraw::matrix4f::getIdentity();
				D3DXMatrixIdentity(&global_transformMatrixInverse);

				X3DNode* parent = pViewpoint->GetFirstParentNodeOfFieldName("children");
				while (parent)
				{
					Transform* transform = dynamic_cast<Transform*>(parent);
					if (transform)
					{
						global_transformMatrixInverse *= transform->GetTransform();
					}

					parent = parent->GetFirstParentNodeOfFieldName("children");
				}

			//	AxisAngle orientation = pViewpoint->getOrientation();
			//	Vec3f position = pViewpoint->getPosition();
			//	Vec3f axis = orientation.m_axis;
			//	axis.normalize(); // should be done already

				/*
				m_pGraphics3D->glRotate(gmDegrees(orientation.m_angle), -axis);
				m_pGraphics3D->glTranslate(-position);
				*/

			//	global_transformMatrixInverse = MatrixTranslation(position) * global_transformMatrixInverse;
			//	global_transformMatrixInverse = MatrixRotationAxis(axis, orientation.m_angle) * global_transformMatrixInverse;
				global_transformMatrixInverse = camera->GetViewMatrixI() * global_transformMatrixInverse;
				D3DXMatrixInverse(&world, NULL, &global_transformMatrixInverse);
			}

			xdc.m_listener.OrientFront = D3DXVECTOR3(0, 0, 1);
			xdc.m_listener.OrientTop = D3DXVECTOR3(0, 1, 0);
			xdc.m_listener.Position = D3DXVECTOR3(0, 0, 0);
			xdc.m_listener.Velocity = D3DXVECTOR3(0, 0, 0);
			xdc.m_listener.pCone = NULL;

			xdc.m_zNear = 0.5f;
			xdc.m_zFar = 800.0f;

			if (camera)
			{
				D3DXMATRIX projection = camera->GetProjectionMatrixRH(w, h, xdc.m_zNear, xdc.m_zFar);

				Gui::ImmediateRenderContext* imm = xdc.m_renderContext;

				imm->m_modelViewIMatrix_Valid = false;
				imm->m_modelViewTMatrix_Valid = false;
				imm->m_modelViewITMatrix_Valid = false;

				imm->m_modelViewProjectionMatrix_Valid = false;

				D3DXMatrixIdentity(&imm->m_modelMatrix);
				imm->m_viewMatrix = world;
				imm->m_modelViewMatrix = world;

				imm->m_projectionMatrix = projection;

			//	imm->m_modelViewProjectionMatrix = world * projection;

	#if 0
				if (false)
				{
					double aspect = (double)w / h;
					double zNear = 0.5;
					double zFar = 500;

					D3DXMATRIX mat;
					D3DXMatrixPerspectiveFovLH(&mat, fov, aspect, zNear, zFar);
					/*
				//	m_pGraphics3D->gluPerspective(gmDegrees(fov), (double)w / h, 0.08, 0.2);

				//	double top = zNear * tan( gmDegrees(fov) * M_PI / 360.0 );
					double top = zNear * tan(fov / 2);
					double bottom = -top;

					double left = bottom * aspect;
					double right = top * aspect;

					m_pGraphics3D->Frustum(left, right, bottom, top, zNear, zFar);
					*/

					xdc.m_projection = *(LDraw::matrix4f*)&mat;
					xdc.m_modelView = LDraw::matrix4f::getIdentity();
					xdc.m_modelView = LDraw::matrix4f::getTranslate(0, 0, 2);
				//	m_pGraphics3D->GetFloatv(GL_MODELVIEW_MATRIX, xdc.m_modelView);
				//	m_pGraphics3D->GetFloatv(GL_PROJECTION_MATRIX, xdc.m_projection);

					if (pViewpoint)
					{
						D3DXMATRIX global_transformMatrixInverse = LDraw::matrix4f::getIdentity();

						X3DNode* parent = pViewpoint->GetFirstParentNodeOfFieldName(WSTR("children"));
						while (parent)
						{
							Transform* transform = dynamic_cast<Transform*>(parent);
							if (transform)
							{
								global_transformMatrixInverse *= transform->GetTransform();
							}

							parent = parent->GetFirstParentNodeOfFieldName(WSTR("children"));
						}

						AxisAngle orientation = pViewpoint->getOrientation();
						LDraw::vector3f position = pViewpoint->getPosition();
						LDraw::vector3f axis = orientation.m_axis;
					//	axis.normalize(); // should be done already

						/*
						m_pGraphics3D->glRotate(gmDegrees(orientation.m_angle), -axis);
						m_pGraphics3D->glTranslate(-position);
						*/

						global_transformMatrixInverse *= LDraw::matrix4f::getTranslate(position);
						global_transformMatrixInverse *= LDraw::matrix4f::getRotate(gmDegrees(orientation.m_angle), axis);

						//m_pGraphics3D->MultMatrixf(global_transformMatrixInverse.getInverse());

						xdc.m_modelView = global_transformMatrixInverse.getInverse();
					}

					xdc.m_modelViewI = xdc.m_modelView.getInverse();
					xdc.m_projectionI = xdc.m_projection.getInverse();

					xdc.m_modelViewT = xdc.m_modelView.getTranspose();
					xdc.m_projectionT = xdc.m_projection.getTranspose();

					xdc.m_modelViewIT = xdc.m_modelViewI.getTranspose();
					xdc.m_projectionIT = xdc.m_projectionI.getTranspose();
				}
	#endif

				X3DBackgroundNode* background = scene->get_Background();
				if (background)
				{
					background->Draw(&xdc);
				}

				Fog* fog = m_scene->get_Fog();
				if (fog)
				{
					d3d10->m_d3d10_fogTypeVariable->SetInt(1);
					d3d10->m_d3d10_fogColorVariable->SetFloatVector(fog->getColor());

					float visibilityRange = fog->getVisibilityRange();
					d3d10->m_d3d10_fogDistanceSquaredVariable->SetFloat(visibilityRange*visibilityRange);
				}
				else
				{
					d3d10->m_d3d10_fogTypeVariable->SetInt(0);
				}

				bool headlight = false;

				if (pNavigationInfo)
				{
					headlight = pNavigationInfo->getHeadlight();
				}

				if (headlight)
				{
					Graphics::Light light;
					light.m_type = 1;	// Directional
					light.m_ambient = Vec4f(0, 0, 0, 1.0f);
					light.m_diffuse = Vec4f(1, 1, 1, 1.0f);
				//	light.m_position = Vec4f(Vec3f(0, 0, 1), 0/*directional*/);
					light.m_position = Vec4f(Vec3f(0, 0, 0), 1/*positional*/);
					light.m_constant_attenuation = 1;
					light.m_linear_attenuation = 0;
					light.m_quadratic_attenuation = 0;

					xdc.m_renderContext->m_lights.push_back(light);
					++xdc.m_renderContext->m_nLight;
				}

				render->DrawGlobalLights(&xdc);

				ID3D10DepthStencilState* depthStencilState;
				UINT stencilRef;
				renderContext.m_p->GetRT()->m_d3d10->m_device1->OMGetDepthStencilState(&depthStencilState, &stencilRef);

				renderContext.m_p->GetRT()->m_d3d10->m_device1->OMSetDepthStencilState(Graphics::Direct10::s.m_pDepthStencilState[true], 0/*stencilRef*/);

				if (true)
				{
					bool frontccw = true;
					bool solid = true;
					bool multiSampleEnable = get_MultiSample();
					renderContext.m_p->GetRT()->m_d3d10->m_device1->RSSetState(Graphics::Direct10::s.m_pRasterizerState[frontccw][solid][multiSampleEnable]);
				}

				render->Draw(&xdc);

				if (false)
				{
					if (!m_origoAxis.IsValid(xdc.m_renderContext))
					{
						float scale = 500.0f;

						Vec3f vertices[] =
						{
							Vec3f(0,0,0), Vec3f(scale, 0, 0),
							Vec3f(0,0,0), Vec3f(0, scale, 0),
							Vec3f(0,0,0), Vec3f(0, 0, scale),
						};

						Vec3f colors[] =
						{
							Vec3f(1, 0, 0),
							Vec3f(0, 1, 0),
							Vec3f(0, 0, 1),
						};

						m_origoAxis.Create(xdc.m_renderContext, vertices, _countof(vertices), colors, false);
					}

					renderContext.m_p->GetRT()->m_d3d10->m_device1->OMSetDepthStencilState(Graphics::Direct10::s.m_pDepthStencilState[false], 0/*stencilRef*/);
					m_origoAxis.Draw(xdc.m_renderContext);
				}

				renderContext.m_p->GetRT()->m_d3d10->m_device1->OMSetDepthStencilState(Graphics::Direct10::s.m_pDepthStencilState[false], 0/*stencilRef*/);
				DrawOverlayChildren(&xdc, scene->getRootNodes());

				renderContext.m_p->GetRT()->m_d3d10->m_device1->OMSetDepthStencilState(depthStencilState, stencilRef);
			}
			else
			{
			}
		}
	}
}

void SceneVisual3D::OnTick(double tim)
{
	uint64 tickcount = GetTickCount64();

	m_fps = 1000.0f / (tickcount - m_lasttickcount);
	m_lasttickcount = tickcount;

	if (m_scene)
	{
		m_scene->m_fps = m_fps;
		m_scene->OnTick(tim);
	}
}

void SceneVisual3D::OnSetRoot(Gui::IRootVisual* root)
{
	Gui::TimeManager::manager.OnTick.Connect(EventHandler(this, &x3d::SceneVisual3D::OnTick));
}

void SceneVisual3D::OnSceneInvalidate(X3DScene* scene)
{
	Gui::IRootVisual* root = GetRoot();
	if (root)
	{
		root->InvalidateRender();
	}
}

#if 0
void COGLSceneRenderer::OnRender(UI::Graphics* pGraphics)
{
//	return;

//	MessageBeep(-1);

//	pGraphics->FillRectangle(new LDraw::SolidBrush(LDraw::Color(0, 0, 0)), 0, 0, m_computedSize.Width, m_computedSize.Height);

//	LDraw::Graphics3D* p3D = GetOwnerWindow()->GetGraphics3D();//new LDraw::Graphics3DImmediate(m_glState, m_clientState, static_cast<LDraw::GraphicsO*>(pGraphics->m_p));

	LDraw::GraphicsI* p3D = pGraphics->m_pO->m_device;
	LDraw::Direct3D9_I* pD3D9 = dynamic_cast<LDraw::Direct3D9_I*>(pGraphics->m_pO->m_device);
	LDraw::OpenGL_I* pGL = dynamic_cast<LDraw::OpenGL_I*>(pGraphics->m_pO->m_device);

	if (pD3D9)
	{
		if (m_d3d9_effect == NULL)
		{
			DWORD dwShaderFlags = 0;

		//	dwShaderFlags |= D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY;
		//	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
		//	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
		//	dwShaderFlags |= D3DXSHADER_NO_PRESHADER;

			HRESULT hr;
			
			ID3DXBuffer* errors = NULL;
			hr = D3DXCreateEffectFromFile(
				pD3D9->m_d3dDevice, 
				L"C:/mmstudio/x3d_techniques9.fx", 
				NULL, // CONST D3DXMACRO* pDefines,
				NULL, // LPD3DXINCLUDE pInclude,
				dwShaderFlags, 
				NULL, // LPD3DXEFFECTPOOL pPool,
				&m_d3d9_effect,
				&errors);
			if (FAILED(hr))
			{
				if (errors)
				{
					const char* str = (const char*)errors->GetBufferPointer();
					ASSERT(0);
					errors->Release();
				}
			}

			m_d3d9_technique0 = m_d3d9_effect->GetTechniqueByName("Render");
		//	m_res->m_d3d9_technique1 = m_res->m_d3d9_effect->GetTechniqueByName("RenderLinearGradient");
		//	m_res->m_d3d9_technique4 = m_res->m_d3d9_effect->GetTechniqueByName("RenderTextSolidColor");

			m_d3d9_modelViewVariable = m_d3d9_effect->GetParameterByName(NULL, "modelView");
			m_d3d9_projectionVariable = m_d3d9_effect->GetParameterByName(NULL, "projection");
			m_d3d9_modelViewProjectionVariable = m_d3d9_effect->GetParameterByName(NULL, "modelViewProjection");
			m_d3d9_modelViewITVariable = m_d3d9_effect->GetParameterByName(NULL, "modelViewIT");
			m_d3d9_modelViewProjectionITVariable = m_d3d9_effect->GetParameterByName(NULL, "modelViewProjectionIT");
			m_d3d9_nlightsVariable = m_d3d9_effect->GetParameterByName(NULL, "nlights");
		}

		if (m_vertexDeclaration0 == NULL)
		{
			LDraw::VertexDeclElement elements[] =
			{
				{"POSITION", 0, 0, LDraw::DECLTYPE_FLOAT3, 0},
				{"NORMAL", 0, 3*4, LDraw::DECLTYPE_FLOAT3, 0},
			};

			m_vertexDeclaration0 = p3D->CreateVertexDeclaration(elements, 2, NULL);
		}
	}
	else if (pGL)
	{
		if (m_glFixedVertexShader == 0)
		{
			TextFile source(WSTR("c:/mmstudio/FixedVertexShader.glsl"));

			m_glFixedVertexShader = pGL->m_glprocs->glCreateShader(GL_VERTEX_SHADER);
			ASSERT(::glGetError() == 0);

			if (m_glFixedVertexShader)
			{
				pGL->m_glprocs->glShaderSource(m_glFixedVertexShader, 1/*number of strings*/, (const char**)&source.m_buffer, &source.m_size);
				ASSERT(::glGetError() == 0);

				pGL->m_glprocs->glCompileShader(m_glFixedVertexShader);

				GLint compiled;
				pGL->m_glprocs->glGetShaderiv(m_glFixedVertexShader, GL_COMPILE_STATUS, &compiled);

				if (!compiled)
				{
					char buffer[512];
					int length;
					pGL->m_glprocs->glGetShaderInfoLog(m_glFixedVertexShader, 512, &length, buffer);

					VERIFY(0);
				}
			}
		}

		if (m_glFixedFragmentShader == 0)
		{
			TextFile source(WSTR("c:/mmstudio/FixedFragmentShader.glsl"));

			m_glFixedFragmentShader = pGL->m_glprocs->glCreateShader(GL_FRAGMENT_SHADER);
			ASSERT(::glGetError() == 0);

			if (m_glFixedFragmentShader)
			{
				pGL->m_glprocs->glShaderSource(m_glFixedFragmentShader, 1/*number of strings*/, (const char**)&source.m_buffer, &source.m_size);
				ASSERT(::glGetError() == 0);

				pGL->m_glprocs->glCompileShader(m_glFixedFragmentShader);

				GLint compiled;
				pGL->m_glprocs->glGetShaderiv(m_glFixedFragmentShader, GL_COMPILE_STATUS, &compiled);

				if (!compiled)
				{
					VERIFY(0);
				}
			}
		}

		if (m_glFixedShaderProgram == 0)
		{
			m_glFixedShaderProgram = pGL->m_glprocs->glCreateProgram();

			pGL->m_glprocs->glAttachShader(m_glFixedShaderProgram, m_glFixedVertexShader);

			pGL->m_glprocs->glLinkProgram(m_glFixedShaderProgram);
			ASSERT(::glGetError() == 0);

			GLint linked;
			pGL->m_glprocs->glGetProgramiv(m_glFixedShaderProgram, GL_LINK_STATUS, &linked);
			if (!linked)
			{
				VERIFY(0);
			}
		}
	}

	/*
	if (m_pGraphics3D == NULL)
	{
		m_pGraphics3D = new LDraw::GraphicsO_3D;
	}
	*/

#if USE_GL
	return;
#endif

	if (m_scene)
	{

		DrawCommand* pCmd = new DrawCommand;
		pCmd->m_scene = m_scene;

		ASSERT(0);
#if 0
		pCmd->m_actualSize = get_ActualSize();
#endif

		dynamic_cast<LDraw::GraphicsMetafile*>(pGraphics->m_p)->m_pCommands->AddCommand(pCmd);
	}

#if 0
	p->m_stream << uint16(0);

	LDraw::Render3DCommand* pCmd = new LDraw::Render3DCommand(m_glState, list);
	dynamic_cast<LDraw::GraphicsMetafile*>(pGraphics->m_p)->m_pCommands.AddCommand(pCmd);
#endif
}
#endif

#if 0
void COGLSceneRenderer::handleEvent(System::Event* evt)
{
	if (evt->get_eventPhase() != System::CAPTURING_PHASE)
	{
		if (*evt->get_type() == L"timer")
		{
			evt->stopPropagation();
			OnTimer();
		}
		else if (*evt->get_type() == L"mousedown")
		{
			evt->stopPropagation();
			UI::MouseEvent* mouseevt = static_cast<UI::MouseEvent*>(evt);

			LDraw::PointD mousepos = ScreenToClient(mouseevt->get_ScreenX(), mouseevt->get_ScreenY());

			LDraw::SizeD actualSize = get_ActualSize();

			double winx = mousepos.X;
			double winy = actualSize.Height - mousepos.Y;//-1;

			hittest(winx, winy);
		}
		else if (*evt->get_type() == L"mousemove")
		{
			evt->stopPropagation();
			UI::MouseEvent* mouseevt = static_cast<UI::MouseEvent*>(evt);

			LDraw::PointD mousepos = ScreenToClient(mouseevt->get_ScreenX(), mouseevt->get_ScreenY());

			LDraw::SizeD actualSize = get_ActualSize();

			double winx = mousepos.X;
			double winy = actualSize.Height - mousepos.Y;//-1;

			hittest(winx, winy);
		}
	}
}
#endif

#if 0
void COGLSceneRenderer::hittest(float winx, float winy)
{
#if 0
	LDraw::SizeD actualSize = get_ActualSize();

	LDraw::Graphics3DImmediate* pG = NULL;//new LDraw::Graphics3DImmediate(m_glState, m_clientState, NULL);
//	pG->AddRef();

	float w = actualSize.Width;
	float h = actualSize.Height;

	double viewport[4] = { 0, 0, w, h };

	uint selectBuf[512];

	pG->SelectBuffer(512, selectBuf);
	pG->RenderMode(GL_SELECT);
	pG->InitNames();
//	glPushName(0);

	X3DDrawContext xdc;
	xdc.m_pGraphics3D = pG;
	{
		Viewpoint* pViewpoint = NULL;

		if (m_scene->m_viewpointStack.GetSize() > 0)
		{
			pViewpoint = static_cast<Viewpoint*>(m_scene->m_viewpointStack[m_scene->m_viewpointStack.GetSize()-1]);
		}
		else
		{
			// hmm...
		}

		//	glViewport(m_viewR[view].left, m_viewR[view].top, w, h);

		pG->MatrixMode(GL_PROJECTION);
		pG->LoadIdentity();
		LDraw::gluPickMatrix(pG, winx, winy, 3, 3, viewport);
//		glLoadMatrixd(projm);
	//
		double fov;
		if (pViewpoint)
		{
			fov = pViewpoint->m_fieldOfView->m_value;
		}
		else
		{
			fov = M_PI/4;
		}

		{
	//	pG->gluPerspective(gmDegrees(fov), (float)w / (float)h, 1.0, 10000.0);

			double aspect = (double)w / h;
			double zNear = 1;
			double zFar = 500;

			double top = zNear * tan( gmDegrees(fov) * M_PI / 360.0 );
			double bottom = -top;

			double left = bottom * aspect;
			double right = top * aspect;

			pG->Frustum(left, right, bottom, top, 0.075/*zNear*/, zFar);

		}

		pG->MatrixMode(GL_MODELVIEW);
		pG->LoadIdentity();
		//glLoadMatrixf((float*)modelm);

		if (pViewpoint)
		{
			SFRotation* orientation = pViewpoint->m_orientation;
			SFVec3f* position = pViewpoint->m_position;
			LDraw::vector3f norientation = orientation->m_value.m_axis;
			norientation.normalize();

			pG->glRotate(gmDegrees(orientation->m_value.m_angle), -norientation);
			pG->glTranslate(-position->m_value);
		}
		else
		{
			// TODO
			pG->glTranslatef(0, 0, -180);
		}

		pG->PolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (m_scene)
		{
			RenderImplImpl* render = dynamic_cast<RenderImplImpl*>(m_scene->m_root);
		//	CComQIPtr<CLRenderImplImpl> render = static_cast<CLX3DScene*>(m_scene)->m_root;
			if (render)
			{
				render->Draw(&xdc);
			}
		}

		pG->Flush();
	}

	int hits = pG->RenderMode(GL_RENDER);

	// Assume not hit
	for (int i = 0; i < m_scene->m_overList.GetSize(); i++)
	{
		ASSERT(m_scene->m_overList[i]->getIsOver() == true);
		m_scene->m_overList[i]->setIsOver(false);
	}

	if (hits > 0)
	{
		MessageBeep(-1);

		uint* ptr = selectBuf;

		uint names = *ptr++;
		float z1 = *ptr++ / 0x7fffffff;
		float z2 = *ptr++ / 0x7fffffff;

		double winz = z2;

		LDraw::matrix4f modelMatrix;
		LDraw::matrix4f projMatrix;

		pG->GetFloatv(GL_MODELVIEW_MATRIX, (float*)&modelMatrix);
		pG->GetFloatv(GL_PROJECTION_MATRIX, (float*)&projMatrix);

		//double objx, objy, objz;
		LDraw::vector3f obj = LDraw::vector3f(winx, winy, winz).UnProject(modelMatrix, projMatrix, viewport);

		for (int n = 0; n < names; n++)
		{
			//MessageBeep(-1);
			int name = *ptr++;

			TouchSensor* pTouchSensor = xdc.m_namedNodes[name];

			int j;
			for (j = 0; j < m_scene->m_overList.GetSize(); j++)
			{
				if (m_scene->m_overList[j] == pTouchSensor)
				{
					break;
				}
			}
			if (j == m_scene->m_overList.GetSize())
			{
				pTouchSensor->setIsOver(true);
				m_scene->m_overList.Add(pTouchSensor);
				m_scene->m_overList[j]->Change(m_scene->m_overList[j]->getIsOverField());
			}
			else
				pTouchSensor->setIsOver(true);
		}
	}

	for (int i = m_scene->m_overList.GetSize()-1; i >= 0; i--)
	{
		if (m_scene->m_overList[i]->getIsOver() == false)
		{
			m_scene->m_overList[i]->Change(m_scene->m_overList[i]->getIsOverField());
			m_scene->m_overList.RemoveAt(i);//&m_scene->m_overList[i], &m_scene->m_overList[i+1]);
		}
	}
#endif
}
#endif

#if USE_GL

LRESULT COGLSceneRenderer::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	PAINTSTRUCT ps;
	BeginPaint(m_hWnd, &ps);

	ASSERT(m_hrc);

	wglMakeCurrent(ps.hdc, m_hrc);

	RECT rc;
	GetClientRect(m_hWnd, &rc);

//	Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);

	int w = rc.right;
	int h = rc.bottom;

	if (m_hrc)
	{
#if 0
		double projm[16] = {0};
		projm[0*4+0] = 1;
		projm[1*4+1] = 1;
		projm[2*4+2] = 1;
		projm[3*4+3] = 1;

		gluPerspective2(projm, fov, (GLfloat)w / (GLfloat)h, 1.0, 10000.0);

		// Get camera position/direction
		gmVector3 campos, camdir, center;

		center[0] = 0;
		center[1] = 0;
		center[2] = 0;

		camdir[0] = 0;
		camdir[1] = 0;
		camdir[2] = 1;
		double camangle = 0;

		campos[0] = 0;
		campos[1] = 0;
		campos[2] = 10;
#endif

		/*
		C3DLayer* cameraRef = l3d->GetCamera();
		if (cameraRef)
		{
			long lfov = 45<<16;
			cameraRef->FindParam('fov ', 'cam ')->GetParamTags((PARAM_SLIDER_Value), &lfov, (TAG_DONE));
			fov = lfov/65536.0;

			cameraRef->FindParam('posi', 0)->GetParamTags(
				(PARAM_VECTOR_Value), v, (TAG_DONE));
			campos[0] = v[0];
			campos[1] = v[1];
			campos[2] = v[2];
			
			cameraRef->FindParam('rota', 0)->GetParamTags(
				(PARAM_VECTOR_Value), v, (TAG_DONE));
			camdir[0] = v[0];
			camdir[1] = v[1];
			camdir[2] = v[2];
		}
		*/
	/*

		matrix4f cammat2 = matrix4f::identity();

		cammat2 *= matrix4f::rotate(camdir[1], gmVector3(1, 0, 0));
		cammat2 *= matrix4f::rotate(camdir[2]+180, gmVector3(0, 0, 1));
		cammat2 *= matrix4f::rotate(-(camdir[0]+180), gmVector3(0, 1, 0));
	*/

#if 0
		cammat2 *= matrix4f::rotate(camdir[1], gmVector3(1, 0, 0));
		cammat2 *= matrix4f::rotate(camdir[2], gmVector3(0, 0, 1));
		cammat2 *= matrix4f::rotate(camdir[0], gmVector3(0, 1, 0));

		cammat2 *= matrix4f::translate(-campos[0], -campos[1], -campos[2]);

		matrix4f cammat = cammat2;//.inverse();
#endif
		/*
		cammat2 *= matrix4f::rotate(camdir[1], gmVector3(1, 0, 0));
		cammat2 *= matrix4f::rotate(camdir[0], gmVector3(0, 1, 0));
		cammat2 *= matrix4f::rotate(camdir[2], gmVector3(0, 0, 1));
		*/

	//	cammat2 *= matrix4f::translate(-campos[0], -campos[1], -campos[2]);

	//	matrix4f cammat = cammat2;//.inverse();

		/*
		cammat2 =
			
			matrix4f::translate(campos[0], campos[1], campos[2]) *
			matrix4f::translate(center[0], center[1], center[2]) *
			matrix4f::rotate(gmDegrees(camangle), camdir) *
			matrix4f::translate(-center[0], -center[1], -center[2]);// * P
			*/
#if 0
		cammat2 =
			
			matrix4f::translate(-center[0], -center[1], -center[2]) *
			matrix4f::rotate(gmDegrees(camangle), camdir) *
			matrix4f::translate(center[0], center[1], center[2]) *
			matrix4f::translate(campos[0], campos[1], campos[2]);

		float modelm[4][4];
		if (TRUE)
		{
			cammat2.copyTo(modelm);
		}
		/*
		else
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					modelm[i*4+j] = cammat2[i][j];
		}
		*/
#endif

		
#if 0
		C3DLayer* pEnv = l3d->GetEnvironment();
		if (pEnv)
		{
			// Set Ambient color
			{
				COLORREF ambClr;
				long ambIntensity;
				pEnv->FindParam('ambc', 'env ')->GetParamTags((PARAM_COLOR_Value), &ambClr, (TAG_DONE));
				pEnv->FindParam('ambb', 'env ')->GetParamTags((PARAM_SLIDER_Value), &ambIntensity, (TAG_DONE));
				float ambClrf[4] =
				{
					GetRValue(ambClr)/255.0, GetGValue(ambClr)/255.0, GetBValue(ambClr)/255.0,
						(ambIntensity/65536.0)/100.0
				};
				
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambClrf);
			}
			
			// Set Fog
			long bFogActive = FALSE;
			pEnv->FindParam('foga', 'env ')->GetParamTags((PARAM_CHECKBOX_Value), &bFogActive, (TAG_DONE));
			
			if (bFogActive)
			{
				COLORREF fogClr;
				long fogIntensity;
				long fogNear, fogFar;
				pEnv->FindParam('fogc', 'env ')->GetParamTags((PARAM_COLOR_Value), &fogClr, (TAG_DONE));
				pEnv->FindParam('fogi', 'env ')->GetParamTags((PARAM_SLIDER_Value), &fogIntensity, (TAG_DONE));
				pEnv->FindParam('fgnr', 'env ')->GetParamTags((PARAM_SLIDER_Value), &fogNear, (TAG_DONE));
				pEnv->FindParam('fgfr', 'env ')->GetParamTags((PARAM_SLIDER_Value), &fogFar, (TAG_DONE));
				
				float fogClrf[4] =
				{
					GetRValue(fogClr)/255.0, GetGValue(fogClr)/255.0, GetBValue(fogClr)/255.0,
						0.0
				};
				
				glEnable(GL_FOG);
				
				//glFogi(GL_FOG_MODE, GL_LINEAR/GL_EXP/GL_EXP2);	// GL_EXP default
				
				glFogf(GL_FOG_START, fogNear/65536.0);	// 0.0 default near distance
				glFogf(GL_FOG_END, fogFar/65536.0);	// 1.0 default far distance
				
				glFogf(GL_FOG_DENSITY, (fogIntensity/65536.0)/100.0);
				glFogfv(GL_FOG_COLOR, fogClrf);
			}
		}
#endif

		/*
		BOOL bAnyLights = l3d->AnyObjectsOfType(OBJT_LIGHT);
		*/

		//	glEnable(GL_CULL_FACE);

		CLViewpoint* pViewpoint = NULL;

		if (m_scene->m_viewpointStack.size() > 0)
		{
			pViewpoint = static_cast<CLViewpoint*>(m_scene->m_viewpointStack[m_scene->m_viewpointStack.size()-1]);
		}
		else
		{
			// hmm...
		}

		//	glViewport(m_viewR[view].left, m_viewR[view].top, w, h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
//		glLoadMatrixd(projm);
	//
		double fov;
		if (pViewpoint)
		{
			fov = pViewpoint->m_fieldOfView->m_value;
		}
		else
		{
			fov = M_PI/4;
		}

		if (true)
		{
			gluPerspective(gmDegrees(fov), (GLfloat)w / (GLfloat)h, 1.0, 10000.0);
		}
		else
		{
			float aspect = (GLfloat)h / (GLfloat)w;

			int pixelWidth = 400;
			int pixelHeight = 300;

	//	glFrustum(-(float)pixelWidth/2, (float)pixelWidth/2, -aspect*pixelWidth/2, aspect*pixelWidth/2, 1.0, 10.0);
		//glFrustum(-1, 1, -aspect, aspect, 1.0, 1000.0);

			aspect = (float)pixelWidth / (float)pixelHeight;
			gluOrtho2D(-(float)pixelWidth/2, (float)pixelHeight/2, -(float)pixelHeight/2, (float)pixelHeight/2);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glLoadMatrixf((float*)modelm);

		if (TRUE)	// Do this here??
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
		}

		CX3DDrawContext xdc;

		if (true)
		{
			glEnable(GL_DEPTH_TEST);
		}

		glEnable(GL_NORMALIZE);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);	// This takes more time

		if (m_display == 0)
		{
			glShadeModel(GL_SMOOTH);
			glPolygonMode(GL_FRONT/*_AND_BACK*/, GL_FILL);
			glEnable(GL_LIGHTING);
			//glEnable(GL_POLYGON_SMOOTH);
		}
		else if (m_display == 1)
		{
			glShadeModel(GL_FLAT);
			glPolygonMode(GL_FRONT/*_AND_BACK*/, GL_FILL);
			glEnable(GL_LIGHTING);
			//glDisable(GL_POLYGON_SMOOTH);
		}
		else if (m_display == 2)
		{
			glPolygonMode(GL_FRONT/*_AND_BACK*/, GL_LINE);
			glDisable(GL_LIGHTING);
			glDisable(GL_LINE_SMOOTH);
		}

		//glLineWidth(0.0001f);

		// NavigationInfo
		bool headlight;

		if (m_scene->m_navigationinfoStack.GetSize() > 0)
		{
			CLNavigationInfo* pNavigationInfo = static_cast<CLNavigationInfo*>(m_scene->m_navigationinfoStack[0]);

			headlight = static_cast<CLSFBool*>(pNavigationInfo->m_headlight)->m_value;
		}
		else
		{
			// Default values
			headlight = false;
		}

	//	headlight = FALSE;	// TODO remove

		if (TRUE)//TRUE/*bAnyLights*/)
		{
		}

		if (headlight)
		{
			GLfloat light_direction[4] = { 0, 0, 1, 0};	// directional
			GLfloat color[4] = {1, 1, 1, 1};
			GLfloat ambient[4] = {0, 0, 0, 1};

			glEnable(GL_LIGHT0+xdc.m_nLight);
			/*
			glLightfv(GL_LIGHT0+xdc.m_nLight, GL_POSITION, light_direction);
			glLightfv(GL_LIGHT0+xdc.m_nLight, GL_AMBIENT, ambient);
			glLightfv(GL_LIGHT0+xdc.m_nLight, GL_DIFFUSE, color);
			glLightfv(GL_LIGHT0+xdc.m_nLight, GL_SPECULAR , color);
			*/

			xdc.m_nLight++;
		}

		glClearColor(
			0,//lpbmi->bmiColors[0].rgbRed/256.0,
			0,//lpbmi->bmiColors[0].rgbGreen/256.0, 
			0,//lpbmi->bmiColors[0].rgbBlue/256.0,
			0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	glClear(GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	// Rem?

		if (pViewpoint)
		{
			SFRotation* orientation = pViewpoint->m_orientation;
			SFVec3f* position = pViewpoint->m_position;
			gmVector3t<float> norientation = orientation->m_value.m_v;
			norientation.normalize();

			glRotate(gmDegrees(orientation->m_value.m_a), -norientation);
			glTranslate(-position->m_value);
		}
		else
		{
			// TODO
		//	glTranslatef(0, 0, -180);
			//glTranslatef(0, 0, -10);
		}

		if (true)
		{
			double eqn[4] = { 0.0, 1.0, 0.0, 0.0 };

			glClipPlane(GL_CLIP_PLANE0, eqn);
			glEnable(GL_CLIP_PLANE0);
		}

		if (FALSE)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glBegin(GL_LINES);

			glVertex3f(posv[0], posv[1], posv[2]);
			glVertex3f(carv[0], carv[1], carv[2]);

			glEnd();
		}

		/*
		if (bAnyLights)
		{
			l3d->SetLights(0);
		}
		*/

	//	glEnable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);

		if (m_scene)
		{
			CLRenderImplImpl* render = dynamic_cast<CLRenderImplImpl*>(m_scene->m_root);
			if (render)
			{
				render->Draw(&xdc);
				glFlush();

#if 0
				glClear(GL_DEPTH_BUFFER_BIT);

				glEnable(GL_LINE_STIPPLE);
			//	glLineStipple(1, 0xAAAA);	// 101010..
				glLineStipple(1, 0x9249);	// 100100..
				render->DrawOverlay(&xdc);
				glDisable(GL_LINE_STIPPLE);

				glFlush();
#endif
			}
		}
	}

	glFinish();
	SwapBuffers(wglGetCurrentDC());

	EndPaint(m_hWnd, &ps);

	return 0;
}
#endif

#if USE_GL

LRESULT COGLSceneRenderer::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return TRUE;
}

LRESULT COGLSceneRenderer::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	POINT point;
	point.x = (short)LOWORD(lParam);
	point.y = (short)HIWORD(lParam);

	return 0;
#if 0
#if 0
	if (m_dragging == 1)
	{
		m_slider.OnMouseMove(point);
		UpdateWindow();

		double position = m_slider.GetPos();

		CComQIPtr<ILMediaSeeking> seeking = m_filterGraph;
		seeking->Seek(position);
	}
#endif

	if (m_dragging)
	{
		CPoint offset = point - m_startpoint;

		if (m_dragging == 1)	// change XY position
		{
			CLViewpoint* pViewpoint = static_cast<CLViewpoint*>(m_viewpointStack[0]);

			SFRotation* orientation = static_cast<SFRotation*>(pViewpoint->m_orientation);
			SFVec3f* position = static_cast<SFVec3f*>(pViewpoint->m_position);

			double moveY = (double)-offset.y/20;
			double moveX = (double)offset.x/20;

			matrix4f repos = matrix4f::identity();

			repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v);
			repos *= matrix4f::translate(moveX, moveY, 0);
			repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v).inverse();

			position->m_value = repos.transform(m_initialPosition);

			FireViewChange();
		}
		else if (m_dragging == 2) // change XZ position
		{
			CLViewpoint* pViewpoint = static_cast<CLViewpoint*>(m_viewpointStack[0]);

			SFRotation* orientation = static_cast<SFRotation*>(pViewpoint->m_orientation);
			SFVec3f* position = static_cast<SFVec3f*>(pViewpoint->m_position);

			double moveX = (double)offset.x/20;
			double moveZ = (double)offset.y/20;

			matrix4f repos = matrix4f::identity();

			repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v);
			repos *= matrix4f::translate(moveX, 0, moveZ);
			repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v).inverse();

			position->m_value = repos.transform(m_initialPosition);

			FireViewChange();
		}
		else if (m_dragging == 3)
		{
			double r = 600;	// 360

			double rotateY = (double)offset.x*360/r;
			double rotateX = (double)offset.y*360/r;

			CLViewpoint* pViewpoint = static_cast<CLViewpoint*>(m_viewpointStack[0]);

			SFRotation* orientation = static_cast<SFRotation*>(pViewpoint->m_orientation);
			SFVec3f* position = static_cast<SFVec3f*>(pViewpoint->m_position);

			// Orientation
			if (rotateY != 0 || rotateX != 0)
			{
				/*
				float x = m_initialOrientation.m_v[0];
				float y = m_initialOrientation.m_v[1];
				float z = m_initialOrientation.m_v[2];
				float angle = m_initialOrientation.m_a;
				*/

				Quat4d q = m_initialOrientation.AxisAngleToQuaternion(/*x, y, z, angle*/);

				q.CombineQuaternion(/*x, y, z, angle,*/ 0, gmRadians(rotateY), gmRadians(rotateX));

				orientation->m_value = q.QuaternionToAxisAngle();//Quat4d(x, y, z, angle));

				/*
				orientation->m_value.m_v[0] = x;
				orientation->m_value.m_v[1] = y;
				orientation->m_value.m_v[2] = z;
				orientation->m_value.m_a = angle;
				*/

				orientation->m_value.m_v.normalize();
			}

			// Position
			{
				// Rotate position around centerOfRotation
				matrix4f repos = matrix4f::identity();

				repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v);
				repos *= matrix4f::rotate(rotateY, gmVector3(0,1,0));
				repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v).inverse();

				position->m_value = repos.transform(m_initialPosition);
			}

			// Position
			{
				// Rotate position around centerOfRotation
				matrix4f repos = matrix4f::identity();

				repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v);
				repos *= matrix4f::rotate(rotateX, gmVector3(1,0,0));
				repos *= matrix4f::rotate(rotateY, gmVector3(0,1,0));
				repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v).inverse();

				position->m_value = repos.transform(m_initialPosition);
			}

			FireViewChange();
		}
	}
	else
#endif

		ASSERT(0);
#if 0

	if (m_dragging == 0)
	{
		RECT client;
		GetClientRect(&client);
		int w = client.right;
		int h = client.bottom;
	//	wglMakeCurrent(hdc, m_hrc);

		double winx = point.x;
		double winy = client.bottom-point.y-1;

		GLint viewport[4] = { 0, 0, w, h };

		GLuint selectBuf[512];

		glSelectBuffer(512, selectBuf);
		glRenderMode(GL_SELECT);
		glInitNames();
	//	glPushName(0);

		CX3DDrawContext xdc;
		{
			CLViewpoint* pViewpoint = NULL;

			if (m_scene->m_viewpointStack.size() > 0)
			{
				pViewpoint = static_cast<CLViewpoint*>(m_scene->m_viewpointStack[m_scene->m_viewpointStack.size()-1]);
			}
			else
			{
				// hmm...
			}

			//	glViewport(m_viewR[view].left, m_viewR[view].top, w, h);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPickMatrix(winx, winy, 3, 3, viewport);
	//		glLoadMatrixd(projm);
		//
			double fov;
			if (pViewpoint)
			{
				fov = pViewpoint->m_fieldOfView->m_value;
			}
			else
			{
				fov = M_PI/4;
			}

			gluPerspective(gmDegrees(fov), (GLfloat)w / (GLfloat)h, 1.0, 10000.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			//glLoadMatrixf((float*)modelm);

			if (pViewpoint)
			{
				SFRotation* orientation = pViewpoint->m_orientation;
				SFVec3f* position = pViewpoint->m_position;
				gmVector3t<float> norientation = orientation->m_value.m_v;
				norientation.normalize();

				glRotate(gmDegrees(orientation->m_value.m_a), -norientation);
				glTranslate(-position->m_value);
			}
			else
			{
				// TODO
				glTranslatef(0, 0, -180);
			}

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			if (m_scene)
			{
				CLRenderImplImpl* render = dynamic_cast<CLRenderImplImpl*>(m_scene->m_root);
			//	CComQIPtr<CLRenderImplImpl> render = static_cast<CLX3DScene*>(m_scene)->m_root;
				if (render)
				{
					render->Draw(&xdc);
				}
			}

			glFlush();
		}

		GLint hits = glRenderMode(GL_RENDER);

		// Assume not hit
		for (int i = 0; i < m_scene->m_overList.size(); i++)
		{
			ASSERT(m_scene->m_overList[i]->m_isOver->m_value == TRUE);
			m_scene->m_overList[i]->m_isOver->m_value = FALSE;
		}

		if (hits > 0)
		{
			GLuint* ptr = selectBuf;

			GLuint names = *ptr++;
			float z1 = *ptr++ / 0x7fffffff;
			float z2 = *ptr++ / 0x7fffffff;

			double winz = z2;

			double modelMatrix[16];
			double projMatrix[16];

			glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

			double objx, objy, objz;
			gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport, &objx, &objy, &objz);

			for (int n = 0; n < names; n++)
			{
				//MessageBeep(-1);
				int name = *ptr++;

				CLTouchSensor* pTouchSensor = xdc.m_namedNodes[name];

				for (int j = 0; j < m_scene->m_overList.size(); j++)
				{
					if (m_scene->m_overList[j] == pTouchSensor)
					{
						break;
					}
				}
				if (j == m_scene->m_overList.size())
				{
					pTouchSensor->m_isOver->m_value = TRUE;
					m_scene->m_overList.push_back(pTouchSensor);
					m_scene->m_overList[i]->Change(m_scene->m_overList[i]->m_isOver);
				}
				else
					pTouchSensor->m_isOver->m_value = TRUE;
			}
		}

		for (i = m_scene->m_overList.size()-1; i >= 0; i--)
		{
			if (m_scene->m_overList[i]->m_isOver->m_value == FALSE)
			{
				m_scene->m_overList[i]->Change(m_scene->m_overList[i]->m_isOver);
				m_scene->m_overList.erase(&m_scene->m_overList[i], &m_scene->m_overList[i+1]);
			}
		}

		/*
					if (pTouchSensor != m_scene->m_pOver)
					{
						if (m_scene->m_pOver)
						{
							m_scene->m_pOver->m_isOver->m_value = FALSE;
						}

						m_scene->m_pOver = pTouchSensor;

						if (m_scene->m_pOver)
						{
							m_scene->m_pOver->m_isOver->m_value = TRUE;
							m_scene->m_pOver->Change(m_scene->m_pOver->m_isOver);
						}
					}
			*/
	}
#endif
	return 0;
}

LRESULT COGLSceneRenderer::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	ASSERT(0);
#if 0
	CWindowImpl<COGLSceneRenderer>::SetFocus();
#endif
	if (!m_dragging)
	{
		ASSERT(0);
#if 0
		CWindowImpl<COGLSceneRenderer>::SetCapture();
		m_dragging = 1;

		for (int i = 0; i < m_scene->m_overList.size(); i++)
		{
			CLTouchSensor* pTouchSensor = m_scene->m_overList[i];

			pTouchSensor->m_isActive->m_value = TRUE;
			pTouchSensor->Change(pTouchSensor->m_isActive);

			pTouchSensor->m_touchTime->m_value = m_currentVideoTime;
			pTouchSensor->Change(pTouchSensor->m_touchTime);
		}
#endif
	}

	return 0;
}

LRESULT COGLSceneRenderer::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (m_dragging)
	{
		ReleaseCapture();
		m_dragging = 0;

		for (int i = 0; i < m_scene->m_overList.size(); i++)
		{
			m_scene->m_overList[i]->m_isActive->m_value = FALSE;
			m_scene->m_overList[i]->Change(m_scene->m_overList[i]->m_isActive);
		}
	}

	return 0;
}

LRESULT COGLSceneRenderer::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	int nVirtKey = (int) wParam;    // virtual-key code 
	int lKeyData = lParam;          // key data 

#if 0
	if (nVirtKey == VK_LEFT)
	{
		steering += 3 * M_PI/180;
	}
	else if (nVirtKey == VK_RIGHT)
	{
		steering -= 3 * M_PI/180;
	}
	else
	{
		gas = TRUE;
	}
#endif

#if 0
	m_nKeyRepeat++;
	int repeatCount = m_nKeyRepeat;//lKeyData & 0xffff;

	LONG bCtrl = (GetAsyncKeyState(VK_CONTROL) & (1<<15));
	LONG bShift = (GetAsyncKeyState(VK_SHIFT) & (1<<15));

	double moveX = 0;
	double moveZ = 0;

	double rotateX = 0;
	double rotateY = 0;
	double rotateZ = 0;

	double rotate = 0.4 + 0.2*(repeatCount-1);
	if (rotate > 2) rotate = 2;

	BOOL bRunning = bShift;
	double speed = bRunning? 4: 2;

	switch (nVirtKey)
	{
	case VK_UP:
		{
			if (bCtrl)
			{
				rotateX = -rotate;
			}
			else
			{
				moveZ -= speed;
			}
		}
		break;

	case VK_DOWN:
		{
			if (bCtrl)
			{
				rotateX = rotate;
			}
			else
			{
				moveZ += speed;
			}
		}
		break;

	case VK_LEFT:
		{
			if (bCtrl)
			{
				moveX -= speed;
			}
			else
			{
				rotateY = rotate;
			}
		}
		break;

	case VK_RIGHT:
		{
			if (bCtrl)
			{
				moveX += speed;
			}
			else
			{
				rotateY = -rotate;
			}
		}
		break;
	}

	CLViewpoint* pViewpoint = static_cast<CLViewpoint*>(m_scene->m_viewpointStack[m_scene->m_viewpointStack.size()-1]);

	SFRotation* orientation = static_cast<SFRotation*>(pViewpoint->m_orientation);
	SFVec3f* position = static_cast<SFVec3f*>(pViewpoint->m_position);

	// Orientation
	if (rotateY != 0 || rotateX != 0 || rotateZ != 0)
	{
		/*
		float x = orientation->m_value.m_v[0];
		float y = orientation->m_value.m_v[1];
		float z = orientation->m_value.m_v[2];
		float angle = orientation->m_value.m_a;
		*/

		Quat4d q = orientation->m_value.AxisAngleToQuaternion();//x, y, z, angle);

		q.CombineQuaternion(/*x, y, z, angle,*/ gmRadians(rotateZ), gmRadians(rotateY), gmRadians(rotateX));

		orientation->m_value = q.QuaternionToAxisAngle();//x, y, z, angle);

		/*
		orientation->m_value.m_v[0] = x;
		orientation->m_value.m_v[1] = y;
		orientation->m_value.m_v[2] = z;
		orientation->m_value.m_a = angle;
		*/

		orientation->m_value.m_v.normalize();
	}

	// Position
	{
		matrix4f repos = matrix4f::identity();

		repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v);
		repos *= matrix4f::translate(moveX, 0, moveZ);
		repos *= matrix4f::rotate(gmDegrees(orientation->m_value.m_a), -orientation->m_value.m_v).inverse();

		position->m_value = repos.transform(position->m_value);
	}
#endif

	FireViewChange();

	return 0;
}

LRESULT COGLSceneRenderer::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	gas = FALSE;
//	steering = 0;
#if 0
	m_nKeyRepeat = 0;
#endif
	return 0;
}

LRESULT COGLSceneRenderer::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	lastt = GetTickCount();

	float dX2 = 1;
	float dY2 = 1;
	float dZ2 = 1;
  float Density = 0.4;
  mass = 8*Density*dX2*dY2*dZ2;

  CState& state = states[0];

  state.Orientation = gmMatrix3::identity();
// There is also an equivalent for rotation, in one dimension
//	MomentOfInertiaSphere(I, 0.5, mass);
	InverseBodyInertiaTensor = gmMatrix3::identity();
  InverseBodyInertiaTensor[0][0] = float(3)/(mass*(dY2*dY2+dZ2*dZ2));
  InverseBodyInertiaTensor[1][1] = float(3)/(mass*(dX2*dX2+dZ2*dZ2));
  InverseBodyInertiaTensor[2][2] = float(3)/(mass*(dX2*dX2+dY2*dY2));

	m = gmMatrix3::identity();
	m[0][0] = mass;
	m[1][1] = mass;
	m[2][2] = mass;

	HDC hdc = GetDC(m_hWnd);
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	CreateContext(hdc, rc);

	const unsigned char* glVendor = glGetString(GL_VENDOR);
	const unsigned char* glRenderer = glGetString(GL_RENDERER);
	const unsigned char* glVersion = glGetString(GL_VERSION);
	const unsigned char*	extstrGL = glGetString(GL_EXTENSIONS);

//	MessageBox((char*)extstrGL, "", MB_OK);

	m_currentVideoTime = 0;

	HRESULT hr;

//	ASSERT(0);
#if 0
	hr = DirectSoundCreate8(NULL, &m_scene->m_ds8, NULL);
	ASSERT(SUCCEEDED(hr));

	hr = m_scene->m_ds8->SetCooperativeLevel(m_hWnd, DSSCL_NORMAL);
	ASSERT(SUCCEEDED(hr));
	m_scene->GetListener(&m_scene->m_soundListener);
#endif

	SetTimer(m_hWnd, 1, 1000/fps/*60*/, NULL);
	//SetTimer(2, 1000/fps);
#if 0
	SetTimer(1, 33);
	joySetThreshold(JOYSTICKID1, 10);
	if (joySetCapture(m_hWnd, JOYSTICKID1, 30, TRUE) == JOYERR_NOERROR)
		m_bstrCaption = _T("Joystick Mode");
#endif

#if 0
	HDC hDC = GetDC();

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
			1,                     // version number
			//PFD_DRAW_TO_BITMAP |   // support window
			PFD_DRAW_TO_WINDOW |   // support window
			PFD_SUPPORT_OPENGL |   // support OpenGL
			0,      // not double buffered
			PFD_TYPE_RGBA,         // RGBA type
			24,                    // 32-bit color depth
			0, 0, 0, 0, 0, 0,      // color bits ignored
			0,                     // no alpha buffer
			0,                     // shift bit ignored
			0,                     // no accumulation buffer
			0, 0, 0, 0,            // accum bits ignored
			32,                    // 16// 32-bit z-buffer
			0,                     // no stencil buffer
			0,                     // no auxiliary buffer
			PFD_MAIN_PLANE,        // main layer
			0,                     // reserved
			0, 0, 0                // layer masks ignored
	};

	//m_hDC = GetDC();

	int  iPixelFormat;
	
	// get the device context's best-available-match pixel format
	iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	
	// make that the device context's current pixel format
	if (!SetPixelFormat(hDC, iPixelFormat, &pfd))
	{
		//AfxMessageBox("!SetPixelFormat()");
		//return;
		ASSERT(0);
	}
	
	// create a rendering context
	if (!(m_hglrc = wglCreateContext (hDC)))
	{
		//AfxMessageBox("!wglCreateContext()");
		//return;
		ASSERT(0);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// TODO
	//ASSERT(glGetError() == GL_NO_ERROR);

	ReleaseDC(hDC);
#endif

	return 0;
}

LRESULT COGLSceneRenderer::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	::wglMakeCurrent(NULL,  NULL);

	if (m_hrc)
	{
		::wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}

	return 0;
}

BOOL COGLSceneRenderer::bSetupPixelFormat(HDC hdc)
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0 )
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

void COGLSceneRenderer::CreateContext(HDC hdc, RECT& rc)
{
	PIXELFORMATDESCRIPTOR pfd;
//	GLfloat     fMaxObjSize, fAspect;
//	GLfloat     fNearPlane, fFarPlane;

	if (!bSetupPixelFormat(hdc))
		return;

#if 0
	CreateRGBPalette(hdc);

	::SelectPalette(hdc, m_hPal, FALSE);
	::RealizePalette(hdc);
#endif

	int n = ::GetPixelFormat(hdc);
	::DescribePixelFormat(hdc, n, sizeof(pfd), &pfd);


	m_hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, m_hrc);

#if 0
	glClearDepth(10.0f);
	glEnable(GL_DEPTH_TEST);

	if (rc.bottom)
		fAspect = (GLfloat)rc.right/rc.bottom;
	else    // don't divide by zero, not that we should ever run into that...
		fAspect = 1.0f;
	fNearPlane = 3.0f;
	fFarPlane = 20.0f;
	fMaxObjSize = 3.0f;
	m_fRadius = fNearPlane + fMaxObjSize / 2.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f, fAspect, fNearPlane, fFarPlane);
	glMatrixMode(GL_MODELVIEW);
#endif
}

LRESULT COGLSceneRenderer::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	::wglMakeCurrent(NULL,  NULL);
	if (m_hrc)
	{
		::wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
	HDC hdc = GetDC(m_hWnd);
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	CreateContext(hdc, rc);
	glViewport(0, 0, rc.right, rc.bottom);

#if 0
	CRect client;
	GetClientRect(&client);

#if 0
	if (m_hglrc)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hglrc);
		m_hglrc = NULL;
	}

	if (m_hBitmap)
	{
		SelectObject(m_hDC, m_hOldBitmap);
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	int w = client.Width();
	int h = client.Height();

	LPBITMAPINFO lpbmi = (LPBITMAPINFO)GlobalAlloc(GMEM_ZEROINIT, sizeof(BITMAPINFOHEADER)/*+sizeof(RGBQUAD)*256*/);
	lpbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmi->bmiHeader.biPlanes = 1;
	lpbmi->bmiHeader.biBitCount = 24;
	lpbmi->bmiHeader.biCompression = BI_RGB;
	lpbmi->bmiHeader.biWidth = w;
	lpbmi->bmiHeader.biHeight = h;

	LPBYTE bits;
	m_hBitmap = CreateDIBSection(NULL, lpbmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);

	if (m_hBitmap)
	{
		m_hOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap);
		
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd
				1,                     // version number
				PFD_DRAW_TO_BITMAP |   // support window
				//PFD_DRAW_TO_WINDOW |   // support window
				PFD_SUPPORT_OPENGL |   // support OpenGL
				0,      // not double buffered
				PFD_TYPE_RGBA,         // RGBA type
				24,                    // 32-bit color depth
				0, 0, 0, 0, 0, 0,      // color bits ignored
				0,                     // no alpha buffer
				0,                     // shift bit ignored
				0,                     // no accumulation buffer
				0, 0, 0, 0,            // accum bits ignored
				32,                    // 16// 32-bit z-buffer
				0,                     // no stencil buffer
				0,                     // no auxiliary buffer
				PFD_MAIN_PLANE,        // main layer
				0,                     // reserved
				0, 0, 0                // layer masks ignored
		};

		//m_hDC = GetDC();
	
		int  iPixelFormat;
		
		// get the device context's best-available-match pixel format
		iPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
		
		// make that the device context's current pixel format
		if (!SetPixelFormat(m_hDC, iPixelFormat, &pfd))
		{
			//AfxMessageBox("!SetPixelFormat()");
			//return;
			ASSERT(0);
		}
		
		// create a rendering context
		if (!(m_hglrc = wglCreateContext (m_hDC)))
		{
			//AfxMessageBox("!wglCreateContext()");
			//return;
			ASSERT(0);
		}

#if 0
		// make it the calling thread's current rendering context
		if (!wglMakeCurrent (m_hDC, m_hglrc))
		{
			//AfxMessageBox("!wglMakeCurrent()");
			//return;
			ASSERT(0);
		}

		const GLubyte *extstrGL;

		extstrGL = glGetString(GL_EXTENSIONS);

		::MessageBox(NULL, (char*)extstrGL, "", MB_OK);
#endif
	}
#endif
#endif

	return 0;
}
#endif	// USE_GL

#if 0
void COGLSceneRenderer::SetScene(X3DScene *pScene)
{
	m_scene = pScene;
	if (pScene)
	{
		InitStacks();
	}
}

void COGLSceneRenderer::Move(int m_dragging, LDraw::PointI offset)
{
	ASSERT(0);
#if 0
	Viewpoint* pViewpoint = static_cast<Viewpoint*>(m_scene->m_viewpointStack[m_scene->m_viewpointStack.GetSize()-1]);

		if (m_dragging == 1)	// change XY position
		{
		//	SFRotation* orientation = pViewpoint->m_orientation;
		//	SFVec3f* position = pViewpoint->m_position;

			double moveY = (double)-offset.Y/20;
			double moveX = (double)offset.X/20;

			LDraw::matrix4f repos = LDraw::matrix4f::getIdentity();

			repos *= LDraw::matrix4f::getRotate(gmDegrees(m_newOrientation.m_angle), -m_newOrientation.m_axis);
			repos *= LDraw::matrix4f::getTranslate(moveX, moveY, 0);
			repos *= LDraw::matrix4f::getRotate(gmDegrees(m_newOrientation.m_angle), -m_newOrientation.m_axis).getInverse();

			m_newPosition = repos.transform(m_initialPosition);

		//	FireViewChange();
		}
		else if (m_dragging == 2) // change XZ position
		{
		//	SFRotation* orientation = static_cast<SFRotation*>(pViewpoint->m_orientation);
		//	SFVec3f* position = static_cast<SFVec3f*>(pViewpoint->m_position);

			double moveX = (double)offset.X/20;
			double moveZ = (double)offset.Y/20;

			LDraw::matrix4f repos = LDraw::matrix4f::getIdentity();

			repos *= LDraw::matrix4f::getRotate(gmDegrees(m_newOrientation.m_angle), -m_newOrientation.m_axis);
			repos *= LDraw::matrix4f::getTranslate(moveX, 0, moveZ);
			repos *= LDraw::matrix4f::getRotate(gmDegrees(m_newOrientation.m_angle), -m_newOrientation.m_axis).getInverse();

			m_newPosition = repos.transform(m_initialPosition);

		//	FireViewChange();
		}
		else if (m_dragging == 3)
		{
			double r = 600;	// 360

			double rotateY = (double)offset.X*360/r;
			double rotateX = (double)offset.Y*360/r;

		//	SFRotation* orientation = pViewpoint->m_orientation;
		//	SFVec3f* position = pViewpoint->m_position;

			// Orientation
			if (rotateY != 0 || rotateX != 0)
			{
				/*
				float x = m_initialOrientation.m_v[0];
				float y = m_initialOrientation.m_v[1];
				float z = m_initialOrientation.m_v[2];
				float angle = m_initialOrientation.m_a;
				*/

				Quat4d q = m_initialOrientation.AxisAngleToQuaternion(/*x, y, z, angle*/);

				q.CombineQuaternion(/*x, y, z, angle,*/ 0, gmRadians(rotateY), gmRadians(rotateX));

				m_newOrientation = q.QuaternionToAxisAngle();//Quat4d(x, y, z, angle));

				/*
				orientation->m_value.m_v[0] = x;
				orientation->m_value.m_v[1] = y;
				orientation->m_value.m_v[2] = z;
				orientation->m_value.m_a = angle;
				*/

				m_newOrientation.m_axis.normalize();
			}

			/*
			// Position
			{
				// Rotate position around centerOfRotation
				LDraw::matrix4f repos = LDraw::matrix4f::getIdentity();

				repos *= LDraw::matrix4f::getRotate(gmDegrees(orientation->m_value.m_angle), -orientation->m_value.m_axis);
				repos *= LDraw::matrix4f::getRotate(rotateY, LDraw::vector3f(0,1,0));
				repos *= LDraw::matrix4f::getRotate(gmDegrees(orientation->m_value.m_angle), -orientation->m_value.m_axis).getInverse();

				m_newPosition = repos.transform(m_initialPosition);
			}
			*/

			// Position
			{
				// Rotate position around centerOfRotation
				LDraw::matrix4f repos = LDraw::matrix4f::getIdentity();

				repos *= LDraw::matrix4f::getRotate(gmDegrees(m_newOrientation.m_angle), -m_newOrientation.m_axis);
				repos *= LDraw::matrix4f::getRotate(rotateX, LDraw::vector3f(1,0,0));
				repos *= LDraw::matrix4f::getRotate(rotateY, LDraw::vector3f(0,1,0));
				repos *= LDraw::matrix4f::getRotate(gmDegrees(m_newOrientation.m_angle), -m_newOrientation.m_axis).getInverse();

				m_newPosition = repos.transform(m_initialPosition);
			}

		//	FireViewChange();
		}
#endif
}
#endif

#if 0
void COGLSceneRenderer::BeginMove()
{
	X3DViewpointNode* pViewpoint = m_scene->get_Viewpoint();

	m_initialOrientation = pViewpoint->getOrientation();
	m_initialPosition = pViewpoint->getPosition();
}

void COGLSceneRenderer::OnArrange(LDraw::SizeD finalSize)
{
//	CLXUIVisual::Arrange(finalSize);

#if USE_GL
	UI::WindowBase2* pWindowBase = dynamic_cast<UI::WindowBase2*>(get_OwnerWindow());
	if (pWindowBase)
	{
		UI::CUIFrameImpl* pOwnerWindow = dynamic_cast<UI::CUIFrameImpl*>(pWindowBase->m_pSite);
		if (pOwnerWindow)
		{
			if (pOwnerWindow->GetHwnd())
			{
				set_WindowStyle(WS_CHILD | WS_VISIBLE);
				set_Owner((ULONG_PTR)pOwnerWindow->GetHwnd());

				LDraw::RectD bounds = GetWindowBounds();

				RECT rect;
				rect.left = bounds.X;
				rect.top = bounds.Y;
				rect.right = bounds.GetRight();
				rect.bottom = bounds.GetBottom();

				SetRect(rect);
			}
		}
	}
#endif
}
#endif

}	// x3d
}	// System
