/*==============================================================================

	3D衝突判定[collision.h]
                                                             Author:郭　哲宇
															 Date  :2025/06/24
==============================================================================*/
#ifndef COLLISION_H
#define COLLISION_H

#include "main.h"
#include <list>

class Line;
class Plane;
class Sphere;
class Box;
class Capsule;
class Shape;
class Triangle;
class CollisionMesh;

//=============================================================================
//	衝突判定クラス
//=============================================================================

class Collision
{
private:
	std::vector<Line*> m_Lines;
	std::vector<Plane*> m_Planes;
	std::vector<Sphere*> m_Spheres;
	std::vector<Box*> m_Boxes;
	std::vector<Capsule*> m_Capsules;
	std::vector<Shape*> m_Shapes;
	std::vector<CollisionMesh*> m_Mesh;
	
public:
	void RegisterAuto(Shape* shape);
	void Remove();
	void Clear();
	std::vector<Shape*> ShapeCollision(const Line& line, const int type, Shape* ignoreShape = nullptr, Vector3* outHitPos = nullptr, float* outHitDis = nullptr);
	std::vector<Shape*> ShapeCollision(const Shape& target, Shape* ignoreShape = nullptr);

	//レイキャスト(ポイント)
	bool Raycast(const Vector3& point, const Vector3& direction, std::vector<Shape*>& shape, float distance, Vector3* outHitPos = nullptr, float* outHitDis = nullptr, Shape* ignoreShape = nullptr);
	
	//衝突コンポーネントのFoodPositionからレイキャスト
	bool ShapeRaycast(const Shape& shape, const Vector3& direction, std::vector<Shape*>& shapes, float distance);
	//マルチレイキャスト
	bool MultiRaycast(const Shape& shape, const Vector3& direction, std::vector<Shape*>& shapes, float distance);
	//法線出力
	bool RaycastNormal(const Shape& shape, const Vector3& direction, float distance, Vector3& outNormal);

	//vectorから衝突コンポーネントを削除
	template<typename T>
	void RemoveDestroyed(std::vector<T*>& container)
	{
		container.erase(
			std::remove_if(container.begin(), container.end(),
				[](T* obj)
				{
					return obj->GetpOwner()->GetDestroy();
				}),
			container.end());
	}
};

//点----------------------------------------------------------------------------------------------------------

//点と線分の衝突判定
bool PointInLine(const Vector3& point, const Line& line);
//点が三角形ポリゴン内にあるか
bool PointInTriangle(const Vector3& point, const Vector3& p1, const Vector3& p2, const Vector3& p3);
//点と平面の衝突判定
bool PointInPlane(const Vector3& point, const Plane& plane);
//点と球体の衝突判定
bool PointInSphere(const Vector3& point, const Sphere& sphere);
//点とボックスの衝突判定
bool PointInBox(const Vector3& point, const Box& box);

//点と線(0：線分/1：線)の最短距離を求める関数
float PointToLineDistance(const Vector3& point, const Line& line, const int type);
//点と無限平面の最短距離を求める関数
float PointToPlaneDistance(const Vector3& point, const Plane& plane);

//点と線(0：線分/1：線)の最近点
Vector3 ClosestPointToLine(const Vector3& point, const Line& line, const int type);
//点と球体の最近点
Vector3 ClosestPointToSphere(const Vector3& point, const Sphere& sphere);
//点と三角形の最近点
Vector3 ClosestPointToTriangle(const Vector3& p, const Triangle& tri);
//点とボックスの最近点
Vector3 ClosestPointToBox(const Vector3& point, const Box& box);


//線----------------------------------------------------------------------------------------------------------

//線と線の最短距離の二乗(線分/無限直線 vs 線分/無限直線 vs無限直線)
float LineToLineDistance(const Line& line1, const Line& line2, Vector3& outClosest1, Vector3& outClosest2, const int type);
//線とボックスの最短距離の二乗(normal: 衝突面の法線,penetration: めり込み量)
float LineToBoxDistance(const Line& line, const Box& box, Vector3& outClosestLine, Vector3& outCloseseBox, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//線と三角形の最短距離
float LineToTriangleDistance(const Line& line, const Triangle& tri, Vector3& outClosestLine, Vector3& outClosestTri, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//線分と線分の衝突判定
bool LineToLine(const Line& line1, const Line& line2, const int type, Vector3* outHitPoint = nullptr, float* outHitDist = nullptr);
//線と平面の衝突判定(0：線分/1：線)
bool LineToPlane(const Line& line, const Plane& plane, const int type, Vector3* outHitPoint = nullptr, float* outHitDist = nullptr);
//線と球の衝突判定(0：線分/1：線)
bool LineToSphere(const Line& line, const Sphere& sphere, const int type, Vector3* outHitPoint = nullptr, float* outHitDist = nullptr);
//線分と立方体の衝突判定(0：線分/1：線)
bool LineToBox(const Line& line, const Box& box, const int type, Vector3* outHitPoint = nullptr, float* outHitDist = nullptr);
//線分と三角形の衝突判定(0：線分/1：線)
bool LineToTriangle(const Line& line, const Triangle& tri, const int type, Vector3* outHitPoint = nullptr, float* outHitDist = nullptr);
//線分と複数三角形の衝突判定(0：線分/1：線)
bool LineToMesh(const Line& line, const CollisionMesh& mesh, const int type, Vector3* outHitPoint = nullptr, float* outHitDist = nullptr);

//平面----------------------------------------------------------------------------------------------------------

//三角と三角
bool TriangleToTriangle(const Vector3& t1_1, const Vector3& t1_2, const Vector3& t1_3, const Vector3& t2_1, const Vector3& t2_2, const Vector3& t2_3);
bool TriangleToTriangle(const Vector3& t0_0, const Vector3& t0_1, const Vector3& t0_2, const Vector3& t1_0, const Vector3& t1_1, const Vector3& t1_2, Vector3& outLineStart, Vector3& outLineEnd);
// 平面と平面の衝突判定
bool PlaneToPlane(const Plane& plane1, const Plane& plane2);
bool PlaneToPlane(const Plane& plane1, const Plane& plane2, Vector3& outStart, Vector3& outEnd);
//平面と球体の衝突判定
bool PlaneToSphere(const Plane& plane, const Sphere& sphere, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//平面とボックスの衝突判定
bool PlaneToBox(const Plane& plane, const Box& box);
//有限平面とボックスの衝突判定
bool FinitePlaneToBox(const Plane& plane, const Box& box, Vector3* outNormal = nullptr, float* outPenetration = nullptr);

//球体----------------------------------------------------------------------------------------------------------

//球体と球体の衝突判定
bool SphereToSphere(const Sphere& sphere1, const Sphere& sphere2, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//球体とボックスの衝突判定
bool SphereToBox(const Sphere& sphere, const Box& box, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//球体と三角形の衝突判定
bool SphereToTriangle(const Sphere& sphere, const Triangle& tri, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//球体と複数三角形の衝突判定
bool SphereToMesh(const Sphere& sphere, const CollisionMesh& mesh, Vector3* outNormal = nullptr, float* outPenetration = nullptr);

//ボックス----------------------------------------------------------------------------------------------------------

//ボックスとボックスの衝突判定
bool BoxToBox(const Box& box1, const Box& box2, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//ボックスと三角形の衝突判定
bool BoxToTriangle(const Box& box, const Triangle& tri, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//ボックスと複数三角形の衝突判定
bool BoxToMesh(const Box& box, const CollisionMesh& mesh, Vector3* outNormal = nullptr, float* outPenetration = nullptr);

//カプセル----------------------------------------------------------------------------------------------------------

//線とカプセルの衝突判定(線分/線)
bool LineToCapsule(const Line& line, const Capsule& capsule, const int type, Vector3* outHitPoint = nullptr, float* outHitDist = nullptr);
//平面とカプセルの衝突判定
bool PlaneToCapsule(const Plane& plane, const Capsule& capsule, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//球体とカプセルの衝突判定
bool SphereToCapsule(const Sphere& sphere, const Capsule& capsule, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//ボックスとカプセルの衝突判定
bool BoxToCapsule(const Box& box, const Capsule& capsule, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//カプセルとカプセルの衝突判定
bool CapsuleToCapsule(const Capsule& capsule1, const Capsule& capsule2, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//カプセルと三角形の衝突判定
bool CapsuleToTriangle(const Capsule& capsule, const Triangle& tri, Vector3* outNormal = nullptr, float* outPenetration = nullptr);
//カプセルと複数三角形の衝突判定
bool CapsuleToMesh(const Capsule& capsule, const CollisionMesh& mesh, Vector3* outNormal = nullptr, float* outPenetration = nullptr);


//bool PlaneToSphere(const Plane& plane, const Sphere& sphere);
//bool FinitePlaneToBox(const Plane& plane, const Box& box);
//bool SphereToSphere(const Sphere& sphere1, const Sphere& sphere2);
//bool SphereToBox(const Sphere& sphere, const Box& box);
//bool BoxToBox(const Box& box1, const Box& box2);
//bool PlaneToCapsule(const Plane& plane, const Capsule& capsule);
//bool SphereToCapsule(const Sphere& sphere, const Capsule& capsule);
//bool BoxToCapsule(const Box& box, const Capsule& capsule);
//bool CapsuleToCapsule(const Capsule& capsule1, const Capsule& capsule2);

//bool PlaneToSphere(const Plane& plane, const Sphere& sphere, Vector3& outNormal);
//bool FinitePlaneToBox(const Plane& plane, const Box& box, Vector3& outNormal);
//bool SphereToSphere(const Sphere& sphere1, const Sphere& sphere2, Vector3& outNormal);
//bool SphereToBox(const Sphere& sphere, const Box& box, Vector3& outNormal);
//bool BoxToBox(const Box& box1, const Box& box2, Vector3& outNormal);
//bool PlaneToCapsule(const Plane& plane, const Capsule& capsule, Vector3& outNormal);
//bool SphereToCapsule(const Sphere& sphere, const Capsule& capsule, Vector3& outNormal);
//bool BoxToCapsule(const Box& box, const Capsule& capsule, Vector3& outNormal);
//bool CapsuleToCapsule(const Capsule& capsule1, const Capsule& capsule2, Vector3& outNormal);


#endif //COLLISION_H
