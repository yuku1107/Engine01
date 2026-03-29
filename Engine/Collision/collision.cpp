/*==============================================================================

	3D衝突判定[collision.cpp]
                                                             Author:郭　哲宇
															 Date  :2025/06/24
==============================================================================*/
#include "main.h"
#include "Engine/Collision/collision.h"
#include "Engine/Components/shape.h"
#include "meshField.h"

void Collision::RegisterAuto(Shape* shape)
{
	if (Line* line = dynamic_cast<Line*>(shape))
	{
		m_Lines.push_back(line);
	}
	else if (Plane* plane = dynamic_cast<Plane*>(shape))
	{
		m_Planes.push_back(plane);
	}
	else if (Sphere* sphere = dynamic_cast<Sphere*>(shape))
	{
		m_Spheres.push_back(sphere);
	}
	else if (Box* box = dynamic_cast<Box*>(shape))
	{
		m_Boxes.push_back(box);
	}
	else if (Capsule* capsule = dynamic_cast<Capsule*>(shape))
	{
		m_Capsules.push_back(capsule);
	}
	else if (CollisionMesh* mesh = dynamic_cast<CollisionMesh*>(shape))
	{
		m_Mesh.push_back(mesh);
	}

	m_Shapes.push_back(shape);
}

std::vector<Shape*> Collision::ShapeCollision(const Line& line, const int type, Shape* ignoreShape, Vector3* outHitPos, float* outHitDist)
{
	std::vector<Shape*> result;
	float nearestDist = FLT_MAX;
	Vector3 nearestPos {};

	for (auto shape : m_Shapes) 
	{
		if (shape == nullptr)
		{
			continue;
		}

		if (shape == ignoreShape)
		{
			continue;
		}

		float tempHitDist {};
		Vector3 tempHitPos {};

		if (shape->Intersect(line, type, &tempHitPos, &tempHitDist))
		{
			if (tempHitDist < nearestDist)
			{
				nearestDist = tempHitDist;
				nearestPos = tempHitPos;
			}

			result.push_back(shape);
		}
	}

	if (outHitPos)
	{
		*outHitPos = nearestPos;
	}
	if (outHitDist)
	{
		*outHitDist = nearestDist;
	}

	return result;
}

std::vector<Shape*> Collision::ShapeCollision(const Shape& target, Shape* ignoreShape)
{
	std::vector<Shape*> result;

	for (auto shape : m_Shapes)
	{
		if (shape == nullptr)
		{
			continue;
		}

		if (shape == &target)
		{
			continue;
		}

		if (shape == ignoreShape)
		{
			continue;
		}

		if (shape->Intersect(target))
		{
			result.push_back(shape);
		}
	}

	return result;
}

bool Collision::Raycast(const Vector3& point, const Vector3& direction, std::vector<Shape*>& shapes, float distance, Vector3* outHitPos, float* outHitDist, Shape* ignoreShape)
{
	Line ray(point, direction, distance);

	shapes = ShapeCollision(ray, 0, ignoreShape, outHitPos, outHitDist);

	return !shapes.empty();
}

bool Collision::ShapeRaycast(const Shape& shape, const Vector3& direction, std::vector<Shape*>& shapes, float distance)
{
	Line ray(shape.GetFoodPosition(), direction, distance);

	shapes = ShapeCollision(ray, 0, const_cast<Shape*>(&shape));

	return !shapes.empty();
}

bool Collision::MultiRaycast(const Shape& shape, const Vector3& direction, std::vector<Shape*>& shapes, float distance)
{
	const int numRays = 6;

	//中央
	Line ray(shape.GetFoodPosition(), direction, distance);

	shapes = ShapeCollision(ray, 0, const_cast<Shape*>(&shape));
	
	if (!shapes.empty())
	{
		for (auto coordinate: shapes)
		{
			if (coordinate->GetCollisionResponse()== Shape::Block)
			{
				return true;
			}
		}
	}

	//円周
	for (int i = 0; i < numRays; i++)
	{
		float angle = XM_2PI * i / numRays;
		Vector3 dir = Vector3(cosf(angle), 0.0f, sinf(angle)) * (shape.GetScale().x * 0.41f);
		Vector3 end = shape.GetFoodPosition() + dir;
		Line ray(shape.GetFoodPosition(), end);

		shapes = ShapeCollision(ray, 0, const_cast<Shape*>(&shape));

		if (!shapes.empty())
		{
			for (auto coordinate : shapes)
			{
				if (coordinate->GetCollisionResponse() == Shape::Block)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

bool Collision::RaycastNormal(const Shape& shape, const Vector3& direction, float distance, Vector3& outNormal)
{
	Line ray(shape.GetFoodPosition(), direction, distance);

	std::vector<Shape*> hits = ShapeCollision(ray, 0, const_cast<Shape*>(&shape));

	if (hits.empty())
	{
		return false;
	}

	float closestDistSq = FLT_MAX;
	Shape* closestShape = nullptr;
	Vector3 hitPoint;

	for (Shape* hit : hits)
	{
		Vector3 tempPoint;

		if (hit->Intersect(ray, 0, &tempPoint)) // 交差点取得可能なら
		{
			float distSq = (tempPoint - shape.GetFoodPosition()).lengthSq();

			if (distSq < closestDistSq)
			{
				closestDistSq = distSq;
				hitPoint = tempPoint;
				closestShape = hit;
			}
		}
	}

	if (closestShape)
	{
		outNormal = closestShape->GetNormal(hitPoint);
		return true;
	}

	return false;
}

void Collision::Remove()
{
	RemoveDestroyed(m_Lines);
	RemoveDestroyed(m_Planes);
	RemoveDestroyed(m_Spheres);
	RemoveDestroyed(m_Boxes);
	RemoveDestroyed(m_Capsules);
	RemoveDestroyed(m_Mesh);
	RemoveDestroyed(m_Shapes);
}

void Collision::Clear()
{
	m_Lines.clear();
	m_Planes.clear();
	m_Spheres.clear();
	m_Boxes.clear();
	m_Capsules.clear();
	m_Mesh.clear();
	m_Shapes.clear();
}

//点
bool PointInLine(const Vector3& point, const Line& line)
{
	//線の始点から点までのベクトル
	Vector3 lineToPoint = point - line.GetStart();
	//線の始点から終点までのベクトル
	Vector3 lineVector = line.GetEnd() - line.GetStart();
	//点と線の法線ベクトルを求める
	Vector3 normal = crossProduct(lineToPoint, lineVector);
	//法線ベクトルが0ベクトルでないかを確認
	if (normal.x == 0 && normal.y == 0 && normal.z == 0)
	{
		//法線ベクトルが0ベクトルの場合、点と線の始点の距離を求める
		if (lineToPoint.length() < lineVector.length())
		{
			//点と線の始点の距離が、線の長さより小さい場合、点は線上にある
			return true;
		}
		else
		{
			//点と線の始点の距離が、線の長さより大きい場合、点は線上にない
			return false;
		}
	}
	else
	{
		//法線ベクトルが0ベクトルでない場合、点は線上にない
		return false;
	}
}

bool PointInTriangle(const Vector3& point, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	Vector3 v0 = p3 - p1;
	Vector3 v1 = p2 - p1;
	Vector3 v2 = point - p1;

	//バリセンター座標 point = u * p1 + v * p2 + w * p3
	float d00 = v0.dot(v0);
	float d01 = v0.dot(v1);
	float d11 = v1.dot(v1);
	float d20 = v2.dot(v0);
	float d21 = v2.dot(v1);

	float denom = d00 * d11 - d01 * d01;

	//面になってない
	if (fabs(denom) < 1e-6)
	{
		return false; 
	}

	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	return(u >= 0 && v >= 0 && w >= 0);
}

bool PointInPlane(const Vector3& point, const Plane& plane)
{
	return PointInTriangle(point, plane.GetVertices(0), plane.GetVertices(1), plane.GetVertices(2)) || PointInTriangle(point, plane.GetVertices(2), plane.GetVertices(3), plane.GetVertices(0));
}

bool PointInSphere(const Vector3& point, const Sphere& sphere)
{
	
	//点と球の中心の距離を求める
	Vector3 pointToSphere = point - sphere.GetPosition();
	//点と球の中心の距離が、球の半径より小さい場合、点は球内にある
	if (pointToSphere.length() < sphere.GetRadius())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PointInBox(const Vector3& point, const Box& box)
{
	//点と箱の中心の距離を求める
	Vector3 pointToBox = point - box.GetPosition();
	//点と箱の中心の距離が、箱の幅、高さ、奥行きより小さい場合、点は箱内にある
	if (abs(pointToBox.x) < box.GetScale().x / 2 && abs(pointToBox.y) < box.GetScale().y / 2 && abs(pointToBox.z) < box.GetScale().z / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float PointToLineDistance(const Vector3& point, const Line& line, const int type)
{
	//線の始点から点までのベクトル
	Vector3 lineToPoint = point - line.GetStart();
	//線の始点から終点までのベクトル
	Vector3 lineVector = line.GetEnd() - line.GetStart();
	//tを求める
	float t = lineToPoint.dot(lineVector) / lineVector.dot(lineVector);

	float distance;

	if (type == 0) //線分
	{
		//線の始点外側
		if (t < 0.0f)
		{
			distance = (line.GetStart() - point).length();
		}
		//線の終点外側
		else if (t > 1.0f)
		{
			distance = (line.GetEnd() - point).length();
		}
		else
		{
			//線の始点から線のベクトルをt倍した点を求める
			Vector3 p1 = line.GetStart() + lineVector * t;
			//点と線の始点から線のベクトルをt倍した点までの距離を求める
			distance = (point - p1).length();
		}
	}
	else if (type == 1) //線
	{
		Vector3 p1 = line.GetStart() + lineVector * t;
		distance = (point - p1).length();
	}

	return distance;
}

float PointToPlaneDistance(const Vector3& point, const Plane& plane)
{
	// 内積計算: 点と法線ベクトルの内積
	float dotProduct = point.dot(plane.GetNormal());

	//分子
	float numerator = fabsf(dotProduct + plane.GetD());

	//分母: 法線ベクトルの長さ
	
	float denominator = plane.GetNormal().length();

	float distance = numerator / denominator;

	//内積が0の場合、点は平面上にある
	if (distance == 0)
	{
		return 0;
	}
	else
	{
		return distance;
	}
}

Vector3 ClosestPointToLine(const Vector3& point, const Line& line, const int type)
{
	//線の始点から点までのベクトル
	Vector3 lineToPoint = point - line.GetStart();
	//線の始点から終点までのベクトル
	Vector3 lineVector = line.GetEnd() - line.GetStart();
	//tを求める
	float t = lineToPoint.dot(lineVector) / lineVector.dot(lineVector);

	if (type == 0) //線分
	{
		//線の始点外側
		if (t < 0.0f)
		{
			return line.GetStart();
		}
		//線の終点外側
		else if (t > 1.0f)
		{
			return line.GetEnd();
		}
		else
		{
			//線の始点から線のベクトルをt倍した点を求める
			return line.GetStart() + lineVector * t;
		}
	}
	else if (type == 1) //線
	{
		return  line.GetStart() + lineVector * t;
	}
	
	return  line.GetStart() + lineVector * t;
}

Vector3 ClosestPointToSphere(const Vector3& point, const Sphere& sphere)
{
	XMVECTOR center= sphere.GetPosition().ToXMVECTOR();
	XMVECTOR direction = point.ToXMVECTOR() - center;
	float length = XMVectorGetX(XMVector3Length(direction));

	XMVECTOR ClosestPoint;

	//点と球体中心と一致
	if (length < 1e-6f)
	{
		ClosestPoint= center + XMVectorSet(sphere.GetRadius(), 0.0f, 0.0f, 0.0f);

		return XMVECTORToVector3(ClosestPoint);
	}

	XMVECTOR normalized = XMVector3Normalize(direction);

	ClosestPoint = center + normalized * sphere.GetRadius();

	return XMVECTORToVector3(ClosestPoint);
	
}

Vector3 ClosestPointToTriangle(const Vector3& p, const Triangle& tri)
{
	const Vector3& a = tri.GetA();
	const Vector3& b = tri.GetB();
	const Vector3& c = tri.GetC();

	// 三角形の各辺ベクトル
	Vector3 ab = b - a;
	Vector3 ac = c - a;
	Vector3 ap = p - a;

	float d1 = ab.dot(ap);
	float d2 = ac.dot(ap);
	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		return a;
	}

	Vector3 bp = p - b;
	float d3 = ab.dot(bp);
	float d4 = ac.dot(bp);
	if (d3 >= 0.0f && d4 <= d3)
	{
		return b;
	}

	Vector3 cp = p - c;
	float d5 = ab.dot(cp);
	float d6 = ac.dot(cp);
	if (d6 >= 0.0f && d5 <= d6)
	{
		return c;
	}

	// 面内の最近接点（Barycentric座標）
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return a + ab * v;
	}

	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return a + ac * w;
	}

	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + (c - b) * w;
	}

	// 三角形面内
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return a + ab * v + ac * w;
}

Vector3 ClosestPointToBox(const Vector3& point, const Box& box)
{
	Vector3 dis = point - box.GetPosition();

	Vector3 closest = box.GetPosition();

	for (int i = 0; i < 3; i++)
	{
		// 点とBox中心の差ベクトルを軸に投影
		float dist = dis.dot(box.GetAxis(i));

		dist = Clamp(dist, -box.GetHalfsize()[i], box.GetHalfsize()[i]);

		// 軸方向に投影ベクトルを加算
		closest += box.GetAxis(i) * dist;
	}


	return closest;
}

//線
float LineToLineDistance(const Line& line1, const Line& line2, Vector3& outClosest1, Vector3& outClosest2, const int type)
{
	Vector3 outPoint1, outPoint2;
	float outT1, outT2;

	const Vector3 vector1 = line1.GetEnd() - line1.GetStart();
	const Vector3 vector2 = line2.GetEnd() - line2.GetStart();
	const Vector3 vector12 = line1.GetStart() - line2.GetStart();

	const float a = vector1.dot(vector1); 
	const float b = vector1.dot(vector2); 
	const float c = vector2.dot(vector2); 
	const float d = vector1.dot(vector12); 
	const float e = vector2.dot(vector12); 

	const float denom = a * c - b * b;

	float t1, t2;

	const float EPSILON = 1e-6f;

	// 線分がほぼ平行な場合
	if (denom < EPSILON)
	{
		// u と v がほぼ平行 → 任意の t を使って s を計算
		t1 = 0.0f;
		t2 = e / c;
	}
	else
	{
		t1 = (b * e - c * d) / denom;
		t2 = (a * e - b * d) / denom;
	}

	if (type == 0) //線分 vs 線分
	{
		// t1, t2 を [0,1] にクランプして線分範囲内に収める
		t1 = Clamp(t1, 0.0f, 1.0f);
		t2 = Clamp(t2, 0.0f, 1.0f);
	}
	else if (type == 1) // 無限直線 vs 線分
	{
		// t1, t2 を [0,1] にクランプして線分範囲内に収める
		t2 = Clamp(t2, 0.0f, 1.0f);
	}
	

	outT1 = t1;
	outT2 = t2;

	// 最近接点を計算
	outPoint1 = line1.GetStart() + vector1 * t1;
	outPoint2 = line2.GetStart() + vector2 * t2;

	outClosest1 = outPoint1;
	outClosest2 = outPoint2;

	// 最近接点間の距離の二乗を返す
	return (outPoint1 - outPoint2).dot(outPoint1 - outPoint2);
}

float LineToBoxDistance(const Line& line, const Box& box, Vector3& outClosestLine, Vector3& outCloseseBox, Vector3* outNormal, float* outPenetration)
{
	float minDistSq = FLT_MAX;
	Vector3 bestPointLine, bestPointBox;

	const int NUM_STEPS = 10; // 精度を上げたければ増やす

	for (int i = 0; i <= NUM_STEPS; ++i)
	{
		float t = static_cast<float>(i) / static_cast<float>(NUM_STEPS);
		Vector3 pointOnLine = line.GetStart() + (line.GetEnd() - line.GetStart()) * t;

		Vector3 pointOnBox = ClosestPointToBox(pointOnLine, box);
		float distSq = (pointOnLine - pointOnBox).dot(pointOnLine - pointOnBox);

		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			bestPointLine = pointOnLine;
			bestPointBox = pointOnBox;
		}
	}

	outClosestLine = bestPointLine;
	outCloseseBox = bestPointBox;

	Vector3 n = bestPointLine - bestPointBox;
	float len = n.length();
	if (len > 0.0001f)
	{
		if (outNormal)
		{
			*outNormal = n / len;
		}

		if (outPenetration)
		{
			*outPenetration = sqrtf(minDistSq);
		}
	}
	else
	{
		if (outNormal)
		{
			*outNormal = Vector3(1, 0, 0); // デフォルト法線
		}

		if (outPenetration)
		{
			*outPenetration = 0.0f;
		}
	}
	

	return minDistSq;
}

float LineToTriangleDistance(const Line& line, const Triangle& tri, Vector3& outClosestLine, Vector3& outClosestTri, Vector3* outNormal, float* outPenetration)
{
	float minDistSq = FLT_MAX;
	Vector3 bestPointLine, bestPointTri;

	const int NUM_STEPS = 50; // 精度を上げるなら増やす

	Vector3 edge1 = tri.GetB() - tri.GetA();
	Vector3 edge2 = tri.GetC() - tri.GetA();

	Vector3 lineStart = line.GetStart();
	Vector3 lineEnd = line.GetEnd();

	for (int i = 0; i <= NUM_STEPS; ++i)
	{
		float t = static_cast<float>(i) / static_cast<float>(NUM_STEPS);
		Vector3 pointOnLine = lineStart + (lineEnd - lineStart) * t;

		// 三角形の最近接点
		Vector3 pointOnTri = ClosestPointToTriangle(pointOnLine, tri);

		float distSq = (pointOnLine - pointOnTri).dot(pointOnLine - pointOnTri);
		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			bestPointLine = pointOnLine;
			bestPointTri = pointOnTri;
		}
	}

	outClosestLine = bestPointLine;
	outClosestTri = bestPointTri;

	Vector3 dir = bestPointLine - bestPointTri;
	float len = dir.length();
	if (len > 1e-6f)
	{
		if (outNormal)
		{
			*outNormal = dir / len;
		}

		if (outPenetration)
		{
			*outPenetration = len;
		}
	}
	else
	{
		// ほぼ同一点の場合は三角形法線を使用
		if (outNormal)
		{
			*outNormal = crossProduct(tri.GetB() - tri.GetA(), tri.GetC() - tri.GetA()).normalized();
		}

		if (outPenetration)
		{
			*outPenetration = 0.0f;
		}
	}

	return minDistSq;
}

bool LineToLine(const Line& line1, const Line& line2, const int type, Vector3* outHitPoint, float* outHitDist)
{
	//2本の線の始点間のベクトル
	Vector3 Line2ToLine1 = line1.GetStart() - line2.GetStart();

	float a = line1.GetDirection().dot(line1.GetDirection());
	float e = line2.GetDirection().dot(line2.GetDirection());
	float f = line2.GetDirection().dot(Line2ToLine1);

	float denom = a * e - line1.GetDirection().dot(line2.GetDirection()) * line1.GetDirection().dot(line2.GetDirection());

	if (fabs(denom) < 1e-6f)
	{
		return false;
	}

	float c = line1.GetDirection().dot(Line2ToLine1);
	float b = line1.GetDirection().dot(line2.GetDirection());

	float Line1_t = (b * f - c * e) / denom; // line1上の最接近点の位置
	float Line2_t = (a * f - b * c) / denom; // line2上の最接近点の位置

	Vector3 closestPointLine1 = line1.GetStart() + line1.GetDirection() * Line1_t;
	Vector3 closestPointLine2 = line2.GetStart() + line2.GetDirection() * Line2_t;

	// 最短距離の中点
	Vector3 resultPoint;

	switch (type)
	{
	case 0: resultPoint = (closestPointLine1 + closestPointLine2) * 0.5f; break;
	case 1: resultPoint = closestPointLine1; break;
	case 2: resultPoint = closestPointLine2; break;
	default: resultPoint = closestPointLine1; break;
	}

	if (outHitPoint)
	{
		*outHitPoint = resultPoint;
	}

	if (outHitDist)
	{
		*outHitDist = (resultPoint - line1.GetStart()).length();
	}

	return true;
}

bool LineToPlane(const Line& line, const Plane& plane, const int type, Vector3* outHitPoint, float* outHitDist)
{
	Vector3 lineDir = line.GetEnd() - line.GetStart();

	//分母
	float denom = lineDir.dot(plane.GetNormal());

	//平行チェック
	if (fabs(denom) < 1e-6f)
	{
		return false;
	}

	//線と平面の交点t
	float t = (plane.GetPosition() - line.GetStart()).dot(plane.GetNormal()) / denom;
	
	//線分の場合
	if (type == 0)
	{
		if (t < 0.0f || t>1.0f)
		{
			return false; 
		}
	}

	//交点座標
	Vector3 intersection = line.GetStart() + lineDir * t;

	//交点座標が平面にない
	if (!PointInPlane(intersection, plane))
		return false;

	// 出力
	if (outHitPoint)
	{
		*outHitPoint = intersection;
	}

	if (outHitDist)
	{
		*outHitDist = (intersection - line.GetStart()).length();
	}

	return true;
}

bool LineToSphere(const Line& line, const Sphere& sphere, const int type, Vector3* outHitPoint, float* outHitDist)
{
	Vector3 dir = line.GetDirection();
	Vector3 m = line.GetStart() - sphere.GetPosition();

	float a = dir.dot(dir);
	float b = m.dot(dir);
	float c = m.dot(m) - sphere.GetRadius() * sphere.GetRadius();

	float discriminant = b * b - a * c;

	if (discriminant < 0.0f)
		return false;

	float sqrtD = sqrtf(discriminant);

	float t = (-b - sqrtD) / a;   // 正しい式

	// 線分チェック
	if (type == 0)
	{
		if (t < 0.0f || t > 1.0f)   // ← 方向ベクトルが end-start ならこれ
			return false;
	}

	Vector3 hitPoint = line.GetStart() + dir * t;

	if (outHitPoint)
	{
		*outHitPoint = hitPoint;
	}

	if (outHitDist)
	{
		*outHitDist = (hitPoint - line.GetStart()).length();
	}

	return true;
}

bool LineToBox(const Line& line, const Box& box, const int type, Vector3* outHitPoint, float* outHitDist)
{
	//XMFLOAT3に変換
	XMFLOAT3 line_start = { line.GetStart().x,line.GetStart().y,line.GetStart().z};
	XMFLOAT3 line_dir = { line.GetDirection().x,line.GetDirection().y,line.GetDirection().z};
	XMFLOAT3 box_center = { box.GetPosition().x,box.GetPosition().y,box.GetPosition().z};

	XMFLOAT3 box_axis[3] =
	{
		{box.GetAxis(0).x,box.GetAxis(0).y,box.GetAxis(0).z},
		{box.GetAxis(1).x,box.GetAxis(1).y,box.GetAxis(1).z},
		{box.GetAxis(2).x,box.GetAxis(2).y,box.GetAxis(2).z}
	};

	//XMVECTORに変換
	XMVECTOR start = XMLoadFloat3(&line_start);
	XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&line_dir));
	XMVECTOR center = XMLoadFloat3(&box_center);

	XMVECTOR axis[3] = 
	{
		XMVector3Normalize(XMLoadFloat3(&box_axis[0])), //右
		XMVector3Normalize(XMLoadFloat3(&box_axis[1])), //上
		XMVector3Normalize(XMLoadFloat3(&box_axis[2])), //前
	};

	//ボックスの半サイズ
	float e[3] = { box.GetHalfsize().x, box.GetHalfsize().y, box.GetHalfsize().z};

	//線とボックス中心の差分(center-start)
	XMVECTOR diff = XMVectorSubtract(center, start);

	//最初の交差点
	float tMin = 0.0f;
	//最後の交差点
	float tMax = FLT_MAX;

	if (type == 0)
	{
		float lineLength = (line.GetEnd() - line.GetStart()).length();
		tMax = lineLength;
	}
	else
	{
		tMax = FLT_MAX;
	}
	

	for (int i = 0; i < 3; ++i) 
	{
		float eAxis = e[i];
		float axisDotD = XMVectorGetX(XMVector3Dot(axis[i], dir));
		float axisDotDiff = XMVectorGetX(XMVector3Dot(axis[i], diff));

		if (fabs(axisDotD) < 1e-6f) 
		{
			// レイはこの軸と平行
			if (fabs(axisDotDiff) > eAxis)
			{
				if (outHitPoint)
				{
					*outHitPoint = { 0.0f,0.0f,0.0f };
				}

				return false;
			}
		}
		else 
		{

			//交点範囲 t1, t2
			float t1 = (axisDotDiff - eAxis) / axisDotD;
			float t2 = (axisDotDiff + eAxis) / axisDotD;

			if (t1 > t2)
			{
				std::swap(t1, t2);
			}

			tMin = std::max(tMin, t1);
			tMax = std::min(tMax, t2);

			if (tMin > tMax)
			{
				if (outHitPoint)
				{
					*outHitPoint = { 0.0f,0.0f,0.0f };
				}

				return false;
			}
		}
	}

	// 交点を求める
	Vector3 hitPoint;

	XMVECTOR hit = XMVectorMultiplyAdd(dir, XMVectorReplicate(tMin), start);
	XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, hit);
	hitPoint = Vector3(result.x, result.y, result.z);

	if (outHitPoint)
	{
		*outHitPoint = hitPoint;
	}

	if (outHitDist)
	{
		*outHitDist = (hitPoint - line.GetStart()).length();
	}

	return true;
}

bool LineToTriangle(const Line& line, const Triangle& tri, const int type, Vector3* outHitPoint, float* outHitDist)
{
	Vector3 start = line.GetStart();
	Vector3 dir = line.GetEnd() - start;

	float denom = dir.dot(tri.GetNormal());

	if (fabs(denom) < 1e-6f)
		return false;

	float t = (tri.GetA() - start).dot(tri.GetNormal()) / denom;

	if (type == 0)
	{
		if (t < 0.0f || t > 1.0f)
			return false;
	}

	Vector3 intersection = start + dir * t;

	if (!PointInTriangle(intersection,
		tri.GetA(), tri.GetB(), tri.GetC()))
		return false;

	if (outHitPoint)
	{
		*outHitPoint = intersection;
	}

	if (outHitDist)
	{
		*outHitDist = (intersection - start).length();
	}

	return true;
}

bool LineToMesh(const Line& line, const CollisionMesh& mesh, const int type, Vector3* outHitPoint, float* outHitDist)
{
	bool hitAny = false;
	float closestDist = FLT_MAX;
	Vector3 tempHit;

	for (const auto& tri : mesh.GetTriangles())
	{
		if (LineToTriangle(line, tri, type, &tempHit))
		{
			float dist = (tempHit - line.GetStart()).length();

			if (dist < closestDist) // 一番近い交点を採用
			{
				closestDist = dist;

				if (outHitPoint)
					*outHitPoint = tempHit;

				hitAny = true;
			}
		}
	}

	if (hitAny && outHitDist)
	{
		*outHitDist = closestDist;
	}

	return hitAny;
}

//平面
bool TriangleToTriangle(const Vector3& t0_0, const Vector3& t0_1, const Vector3& t0_2, const Vector3& t1_0, const Vector3& t1_1, const Vector3& t1_2)
{
	//面1
	XMVECTOR v0 = t0_0.ToXMVECTOR();
	XMVECTOR v1 = t0_1.ToXMVECTOR();
	XMVECTOR v2 = t0_2.ToXMVECTOR();
	//面2
	XMVECTOR u0 = t1_0.ToXMVECTOR();
	XMVECTOR u1 = t1_1.ToXMVECTOR();
	XMVECTOR u2 = t1_2.ToXMVECTOR();

	//面1の法線
	XMVECTOR ve1 = v1 - v0;
	XMVECTOR ve2 = v2 - v0;
	XMVECTOR n1 = XMVector3Cross(ve1, ve2);
	n1 = XMVector3Normalize(n1);

	//面2の法線
	XMVECTOR ue1 = u1 - u0;
	XMVECTOR ue2 = u2 - u0;
	XMVECTOR n2 = XMVector3Cross(ue1, ue2);
	n2 = XMVector3Normalize(n2);

	//平面に対して三角形の頂点の距離
	auto PlaneTest = [](XMVECTOR n, XMVECTOR p, XMVECTOR t0, XMVECTOR t1, XMVECTOR t2)->bool
		{
			float d0 = XMVectorGetX(XMVector3Dot(n, t0 - p));
			float d1 = XMVectorGetX(XMVector3Dot(n, t1 - p));
			float d2 = XMVectorGetX(XMVector3Dot(n, t2 - p));
			return (d0 * d1 < 0.0f) || (d0 * d1 < 0.0f) || (d1 * d2 < 0.0f);
		};

	//面1に対して面2の三頂点がすべで同じ側にあるなら交差なし
	if (!PlaneTest(n1, v0, u0, u1, u2))
	{
		return false;
	}

	//面2に対して面1の三頂点がすべで同じ側にあるなら交差なし
	if (!PlaneTest(n2, u0, v0, v1, v2))
	{
		return false;
	}

	//境界線方向ベクトル
	XMVECTOR dir = XMVector3Cross(n1, n2);
	if (XMVector3Equal(dir, XMVectorZero()))
	{
		return false; //平行、同一平面
	}

	//投影軸(最大成分の軸)
	float dx = fabsf(XMVectorGetX(dir));
	float dy = fabsf(XMVectorGetY(dir));
	float dz = fabsf(XMVectorGetZ(dir));
	int axis = (dx > dy && dx > dz) ? 0 : (dy > dz ? 1 : 2);

	// 各三角形を線分に射影して重なりがあるか確認
	auto Project = [](XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, int axis, float& min, float& max) 
		{
			auto get = [](const XMFLOAT3& v, int axis) -> float 
				{
				
					switch (axis) 
					{
					case 0: return v.x;
					case 1: return v.y;
					case 2: return v.z;
					default: return 0.0f; // 安全策
					}
				};

			float p[3] = 
			{
				get(a, axis),
				get(b, axis),
				get(c, axis)
			};
			
			min = max = p[0];
			for (int i = 1; i < 3; ++i) 
			{
				if (p[i] < min) min = p[i];
				if (p[i] > max) max = p[i];
			}
		};

	XMFLOAT3 v0f = t0_0.ToXMFLOAT3(); 
	XMFLOAT3 v1f = t0_1.ToXMFLOAT3();
	XMFLOAT3 v2f = t0_2.ToXMFLOAT3();

	XMFLOAT3 u0f = t1_0.ToXMFLOAT3();
	XMFLOAT3 u1f = t1_1.ToXMFLOAT3();
	XMFLOAT3 u2f = t1_2.ToXMFLOAT3();

	float min1, max1, min2, max2;
	Project(v0f, v1f, v2f, axis, min1, max1);
	Project(u0f, u1f, u2f, axis, min2, max2);

	if (max1 < min2 || max2 < min1) return false;

	return true;

}

bool TriangleToTriangle(const Vector3& t0_0, const Vector3& t0_1, const Vector3& t0_2, const Vector3& t1_0, const Vector3& t1_1, const Vector3& t1_2, Vector3& outLineStart, Vector3& outLineEnd)
{
	//面1
	XMVECTOR v0 = t0_0.ToXMVECTOR();
	XMVECTOR v1 = t0_1.ToXMVECTOR();
	XMVECTOR v2 = t0_2.ToXMVECTOR();
	//面2
	XMVECTOR u0 = t1_0.ToXMVECTOR();
	XMVECTOR u1 = t1_1.ToXMVECTOR();
	XMVECTOR u2 = t1_2.ToXMVECTOR();

	//面1の法線
	XMVECTOR ve1 = v1 - v0;
	XMVECTOR ve2 = v2 - v0;
	XMVECTOR n1 = XMVector3Cross(ve1, ve2);
	n1 = XMVector3Normalize(n1);

	//面2の法線
	XMVECTOR ue1 = u1 - u0;
	XMVECTOR ue2 = u2 - u0;
	XMVECTOR n2 = XMVector3Cross(ue1, ue2);
	n2 = XMVector3Normalize(n2);

	//平面に対して三角形の頂点の距離
	auto PlaneTest = [](XMVECTOR n, XMVECTOR p, XMVECTOR t0, XMVECTOR t1, XMVECTOR t2)->bool
		{
			float d0 = XMVectorGetX(XMVector3Dot(n, t0 - p));
			float d1 = XMVectorGetX(XMVector3Dot(n, t1 - p));
			float d2 = XMVectorGetX(XMVector3Dot(n, t2 - p));
			return (d0 * d1 < 0.0f) || (d0 * d1 < 0.0f) || (d1 * d2 < 0.0f);
		};

	//面1に対して面2の三頂点がすべで同じ側にあるなら交差なし
	if (!PlaneTest(n1, v0, u0, u1, u2))
	{
		return false;
	}

	//面2に対して面1の三頂点がすべで同じ側にあるなら交差なし
	if (!PlaneTest(n2, u0, v0, v1, v2))
	{
		return false;
	}

	// 平面交線の方向ベクトル
	XMVECTOR dir = XMVector3Cross(n1, n2);
	if (XMVector3Equal(dir, XMVectorZero())) return false;

	// 交差線を基準に、2つの三角形と交差する線分を求める（Clipping）

	// 三角形1の交点
	std::vector<XMVECTOR> isects1;
	auto IntersectEdge = [&](XMVECTOR a, XMVECTOR b) {
		XMVECTOR ab = b - a;
		XMVECTOR n = n2;
		float da = XMVectorGetX(XMVector3Dot(n, a - u0));
		float db = XMVectorGetX(XMVector3Dot(n, b - u0));
		if ((da > 0.0f && db < 0.0f) || (da < 0.0f && db > 0.0f)) {
			float t = da / (da - db);
			XMVECTOR p = a + t * ab;
			isects1.push_back(p);
		}
		};
	IntersectEdge(v0, v1);
	IntersectEdge(v1, v2);
	IntersectEdge(v2, v0);

	// 三角形2の交点
	std::vector<XMVECTOR> isects2;
	auto IntersectEdge2 = [&](XMVECTOR a, XMVECTOR b) {
		XMVECTOR ab = b - a;
		XMVECTOR n = n1;
		float da = XMVectorGetX(XMVector3Dot(n, a - v0));
		float db = XMVectorGetX(XMVector3Dot(n, b - v0));
		if ((da > 0.0f && db < 0.0f) || (da < 0.0f && db > 0.0f)) {
			float t = da / (da - db);
			XMVECTOR p = a + t * ab;
			isects2.push_back(p);
		}
		};
	IntersectEdge2(u0, u1);
	IntersectEdge2(u1, u2);
	IntersectEdge2(u2, u0);

	// 交点が最低2つ必要（線が形成できる）
	if (isects1.empty() || isects2.empty()) return false;

	std::vector<XMVECTOR> allPoints;
	allPoints.insert(allPoints.end(), isects1.begin(), isects1.end());
	allPoints.insert(allPoints.end(), isects2.begin(), isects2.end());

	// 最も離れている2点を交線とする
	float maxDistSq = -1.0f;
	XMVECTOR p0 = XMVectorZero();
	XMVECTOR p1 = XMVectorZero();

	for (size_t i = 0; i < allPoints.size(); ++i) {
		for (size_t j = i + 1; j < allPoints.size(); ++j) {
			float d = XMVectorGetX(XMVector3LengthSq(allPoints[i] - allPoints[j]));
			if (d > maxDistSq) {
				maxDistSq = d;
				p0 = allPoints[i];
				p1 = allPoints[j];
			}
		}
	}

	XMFLOAT3 outStart;
	XMFLOAT3 outEnd;

	DirectX::XMStoreFloat3(&outStart, p0);
	DirectX::XMStoreFloat3(&outEnd, p1);

	outLineStart = Vector3(outStart.x, outStart.y, outStart.z);
	outLineEnd = Vector3(outEnd.x, outEnd.y, outEnd.z);

	return true;
}

bool PlaneToPlane(const Plane& plane1, const Plane& plane2)
{
	return  TriangleToTriangle(plane1.GetVertices(0), plane1.GetVertices(1), plane1.GetVertices(2), plane2.GetVertices(0), plane2.GetVertices(1), plane2.GetVertices(2)) 
		|| TriangleToTriangle(plane1.GetVertices(0), plane1.GetVertices(1), plane1.GetVertices(2), plane2.GetVertices(2), plane2.GetVertices(3), plane2.GetVertices(0))
		|| TriangleToTriangle(plane1.GetVertices(2), plane1.GetVertices(3), plane1.GetVertices(0), plane2.GetVertices(0), plane2.GetVertices(1), plane2.GetVertices(2))
		|| TriangleToTriangle(plane1.GetVertices(2), plane1.GetVertices(3), plane1.GetVertices(0), plane2.GetVertices(2), plane2.GetVertices(3), plane2.GetVertices(0));
}

bool PlaneToPlane(const Plane& plane1,const Plane& plane2,Vector3& outStart,Vector3& outEnd)
{
	// 各三角形ペアに対して交差テスト＋交差線を調査
	struct Tri
	{
		Vector3 a, b, c;
	};

	// 平面を2枚の三角形に分解
	Tri plane1Tris[2] = {
		{ plane1.GetVertices(0), plane1.GetVertices(1), plane1.GetVertices(2) },
		{ plane1.GetVertices(2), plane1.GetVertices(3), plane1.GetVertices(0) },
	};

	Tri plane2Tris[2] = {
		{ plane2.GetVertices(0), plane2.GetVertices(1), plane2.GetVertices(2) },
		{ plane2.GetVertices(2), plane2.GetVertices(3), plane2.GetVertices(0) },
	};

	std::vector<Vector3> intersections;

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			Vector3 p0, p1;
			if (TriangleToTriangle(
				plane1Tris[i].a, plane1Tris[i].b, plane1Tris[i].c,
				plane2Tris[j].a, plane2Tris[j].b, plane2Tris[j].c,
				p0, p1))
			{
				intersections.push_back(p0);
				intersections.push_back(p1);
			}
		}
	}

	if (intersections.size() < 2)
		return false;

	// 最も遠い2点を選んで交差線とする
	float maxDistSq = -1.0f;
	for (size_t i = 0; i < intersections.size(); ++i)
	{
		for (size_t j = i + 1; j < intersections.size(); ++j)
		{
			float distSq = (intersections[i] - intersections[j]).lengthSq();
			if (distSq > maxDistSq)
			{
				maxDistSq = distSq;
				outStart = intersections[i];
				outEnd = intersections[j];
			}
		}
	}

	return true;
}

bool PlaneToSphere(const Plane& plane, const Sphere& sphere, Vector3* outNormal, float* outPenetration)
{
	//平面の法線
	XMVECTOR normal = plane.GetNormal().ToXMVECTOR();

	if (outNormal)
	{
		*outNormal = plane.GetNormal();
	}

	//球体と平面の距離
	float distance = XMVectorGetX(XMVector3Dot(normal, sphere.GetPosition().ToXMVECTOR())) + plane.GetD();

	if (fabsf(distance) < sphere.GetRadius())
	{
		if (distance >= 0)
		{
			if (outPenetration)
			{
				*outPenetration = sphere.GetRadius() - distance;
			}
		}
		else
		{
			if (outPenetration)
			{
				*outPenetration = sphere.GetRadius() + distance; // 法線向き反転
			}
		}

		return true;
	}

	return false;
}

bool PlaneToBox(const Plane& plane, const Box& box)
{
	//平面の法線
	XMVECTOR normal = plane.GetNormal().ToXMVECTOR();

	//ボックスと平面の距離
	float distance = XMVectorGetX(XMVector3Dot(normal, box.GetPosition().ToXMVECTOR())) + plane.GetD();

	//ボックスの平面法線に投影される半径
	float radius = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		float projection = XMVectorGetX(XMVector3Dot(normal, box.GetAxis(i).ToXMVECTOR()));
		radius += box.GetHalfsize()[i] * fabsf(projection);
	}

	return fabsf(distance) <= radius;
}

bool FinitePlaneToBox(const Plane& plane, const Box& box, Vector3* outNormal, float* outPenetration)
{
	// 平面の法線とD
	XMVECTOR planeNormal = plane.GetNormal().ToXMVECTOR();
	float planeD = plane.GetD();

	// 各頂点の距離を取得
	float minDistance = FLT_MAX;
	float maxDistance = -FLT_MAX;

	for (int i = 0; i < 8; ++i)
	{
		float dist = XMVectorGetX(XMVector3Dot(box.GetVertices(i).ToXMVECTOR(), planeNormal)) + planeD;
		if (dist < minDistance)
		{
			minDistance = dist;
		}
		if (dist > maxDistance)
		{
			maxDistance = dist;
		}
	}

	// 平面をまたいでいないなら交差なし
	if (minDistance > 0 || maxDistance < 0)
	{
		return false;
	}

	// エッジを全探索（各頂点と隣接する軸方向に +1 した点）
	for (int i = 0; i < 8; ++i)
	{
		for (int axis = 0; axis < 3; ++axis) // x, y, z の方向
		{
			int j = i ^ (1 << axis); // axisビット反転で隣接頂点

			if (i < j) // 重複チェック防止
			{
				const Vector3& p0 = box.GetVertices(i);
				const Vector3& p1 = box.GetVertices(j);

				Line line = Line(p0, p1);

				Vector3 intersection;

				if (LineToPlane(line, plane, 0, &intersection))
				{
					if (PointInPlane(intersection, plane))
					{
						// 最小押し戻し量を計算
						if (fabsf(minDistance) < fabsf(maxDistance))
						{
							if (outNormal)
							{
								*outNormal = plane.GetNormal();
							}

							if (outPenetration)
							{
								*outPenetration = -minDistance; // 法線方向に押し戻す
							}
						}
						else
						{
							if (outNormal)
							{
								*outNormal = -plane.GetNormal();   // 反対方向
							}

							if (outPenetration)
							{
								*outPenetration = -maxDistance; // 法線方向に押し戻す
							}
						}

						return true;
					}
				}
			}
		}
	}

	return false;
}

//球体
bool SphereToSphere(const Sphere& sphere1, const Sphere& sphere2, Vector3* outNormal, float* outPenetration)
{
	Vector3 diff = sphere2.GetPosition() - sphere1.GetPosition();
	float distSq = diff.lengthSq();
	float radiusSum = sphere1.GetRadius() + sphere2.GetRadius();

	if (distSq <= radiusSum * radiusSum)
	{
		float dist = sqrtf(distSq);
		if (dist > 1e-6f)
		{
			if (outNormal)
			{
				*outNormal = diff / dist;
			}
		}
		else
		{
			if (outNormal)
			{
				*outNormal = Vector3(0, 1, 0);
			}
			dist = 0.0f;
		}

		// 押し戻し量 = 半径合計 - 距離
		if (outPenetration)
		{
			*outPenetration = radiusSum - dist;
		}
		return true;
	}

	return false;
}

bool SphereToBox(const Sphere& sphere, const Box& box, Vector3* outNormal, float* outPenetration)
{
	Vector3 closest = ClosestPointToBox(sphere.GetPosition(), box);
	Vector3 diff = sphere.GetPosition() - closest;
	float distSq = diff.lengthSq();
	float radius = sphere.GetRadius();

	if (distSq <= radius * radius)
	{
		float dist = sqrtf(distSq);

		if (dist > 1e-6f)
		{
			if (outNormal)
			{
				*outNormal = diff / dist;
			}
		}
		else
		{
			if (outNormal)
			{
				*outNormal = (sphere.GetPosition() - box.GetPosition()).normalized();
			}
			dist = 0.0f;
		}

		// 押し戻し量 = 半径 - 距離
		if (outPenetration)
		{
			*outPenetration = radius - dist;
		}
		return true;
	}

	return false;
}

bool SphereToTriangle(const Sphere& sphere, const Triangle& tri, Vector3* outNormal, float* outPenetration)
{
	// 三角形の最近接点
	Vector3 closest = ClosestPointToTriangle(sphere.GetPosition(),tri);

	Vector3 dir = sphere.GetPosition() - closest;
	float distSq = dir.lengthSq();

	float radius = sphere.GetRadius();
	if (distSq <= radius * radius)
	{
		if (outNormal)
		{
			*outNormal = (distSq > 1e-6f) ? dir.normalized() : tri.GetNormal();
		}
		if (outPenetration)
		{
			*outPenetration = radius - sqrtf(distSq);
		}
		return true;
	}
	return false;
}

bool SphereToMesh(const Sphere& sphere, const CollisionMesh& mesh, Vector3* outNormal, float* outPenetration)
{
	Vector3 totalNormal(0, 0, 0);
	float totalPenetration = 0.0f;
	int hitCount = 0;

	for (const auto& tri : mesh.GetTriangles())
	{
		Vector3 hitNormal;
		float penetration = 0.0f;

		if (SphereToTriangle(sphere, tri, &hitNormal, &penetration))
		{
			totalNormal += hitNormal;
			totalPenetration += penetration;
			hitCount++;
		}
	}

	if (hitCount > 0)
	{
		if (outNormal)
		{
			*outNormal = totalNormal.normalized();
		}
		if (outPenetration)
		{
			*outPenetration = totalPenetration / hitCount;
		}
		return true;
	}
	return false;
}

//ボックス
bool BoxToBox(const Box& box1, const Box& box2, Vector3* outNormal, float* outPenetration)
{
	const float EPSILON = 1e-6f;

	float R[3][3], AbsR[3][3];
	float ra, rb;

	// 最小押し戻し
	float minOverlap = FLT_MAX;
	Vector3 minAxis;

	// ローカル軸
	Vector3 axisA[3] =
	{
		box1.GetAxis(0),
		box1.GetAxis(1),
		box1.GetAxis(2)
	};

	Vector3 axisB[3] =
	{
		box2.GetAxis(0),
		box2.GetAxis(1),
		box2.GetAxis(2)
	};

	// R行列（A軸 dot B軸）
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			R[i][j] = axisA[i].dot(axisB[j]);
		}
	}

	// 中心差をAのローカルへ
	Vector3 tVec = box2.GetPosition() - box1.GetPosition();
	float t[3] =
	{
		tVec.dot(axisA[0]),
		tVec.dot(axisA[1]),
		tVec.dot(axisA[2])
	};

	// 安全マージン (AbsR[i][j] = |R[i][j]| + ε)
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
		}
	}

	// ---------------------------
	// 1. A軸でテスト
	for (int i = 0; i < 3; ++i)
	{
		ra = box1.GetHalfsize()[i];
		rb = box2.GetHalfsize().x * AbsR[i][0] + box2.GetHalfsize().y * AbsR[i][1] + box2.GetHalfsize().z * AbsR[i][2];

		float overlap = ra + rb - fabsf(t[i]);
		if (overlap < 0)
		{
			return false;
		}

		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			minAxis = axisA[i] * ((t[i] < 0.0f) ? -1.0f : 1.0f);  // 軸向き調整
		}
	}

	// ---------------------------
	// 2. B軸でテスト
	for (int i = 0; i < 3; ++i)
	{
		ra = box1.GetHalfsize().x * AbsR[0][i] + box1.GetHalfsize().y * AbsR[1][i] + box1.GetHalfsize().z * AbsR[2][i];
		rb = box2.GetHalfsize()[i];

		float proj = t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i];
		float overlap = ra + rb - fabsf(proj);
		if (overlap < 0)
		{
			return false;
		}

		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			minAxis = axisB[i] * ((proj < 0.0f) ? -1.0f : 1.0f);
		}
	}

	// ---------------------------
	// 3. A × B軸でテスト
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			// 軸 = A[i] × B[j]
			Vector3 axis = axisA[i].cross(axisB[j]);
			if (axis.lengthSq() < EPSILON)
			{
				continue; // 平行なので無視
			}

			axis = axis.normalized();

			ra = box1.GetHalfsize()[(i + 1) % 3] * AbsR[(i + 2) % 3][j] +
				box1.GetHalfsize()[(i + 2) % 3] * AbsR[(i + 1) % 3][j];

			rb = box2.GetHalfsize()[(j + 1) % 3] * AbsR[i][(j + 2) % 3] +
				box2.GetHalfsize()[(j + 2) % 3] * AbsR[i][(j + 1) % 3];

			float proj = fabsf(t[(i + 1) % 3] * R[(i + 2) % 3][j] - t[(i + 2) % 3] * R[(i + 1) % 3][j]);
			float overlap = ra + rb - proj;

			if (overlap < 0)
			{
				return false;
			}

			if (overlap < minOverlap)
			{
				minOverlap = overlap;
				minAxis = axis * ((tVec.dot(axis) < 0.0f) ? -1.0f : 1.0f);
			}
		}
	}

	// 最小侵入ベクトル
	if (outNormal)
	{
		*outNormal = minAxis.normalized();
	}

	if (outPenetration)
	{
		*outPenetration = minOverlap;
	}

	return true;
}

bool BoxToTriangle(const Box& box, const Triangle& tri, Vector3* outNormal, float* outPenetration)
{
	// 最近接点同士を求める
	Vector3 closestBox = ClosestPointToBox(ClosestPointToTriangle(box.GetPosition(), tri), box);
	Vector3 closestTri = ClosestPointToTriangle(closestBox, tri);

	Vector3 dir = closestBox - closestTri;
	float distSq = dir.lengthSq();

	if (distSq <= 1e-6f)  // めり込み
	{
		if (outNormal)
		{
			*outNormal = tri.GetNormal();  
		}
	
		if (outPenetration)
		{
			*outPenetration = 0.0f; 
		}
		return true;
	}

	if (distSq < 0.0001f)
	{
		return false; 
	}

	if (outNormal)
	{
		*outNormal = dir.normalized();
	}

	if (outPenetration)
	{
		*outPenetration = -sqrtf(distSq); 
	}
	return true;
}

bool BoxToMesh(const Box& box, const CollisionMesh& mesh, Vector3* outNormal, float* outPenetration)
{
	Vector3 totalNormal(0, 0, 0);
	float totalPenetration = 0.0f;
	int hitCount = 0;

	for (const auto& tri : mesh.GetTriangles())
	{
		Vector3 hitNormal;
		float penetration = 0.0f;

		if (BoxToTriangle(box, tri, &hitNormal, &penetration))
		{
			totalNormal += hitNormal;
			totalPenetration += penetration;
			hitCount++;
		}
	}

	if (hitCount > 0)
	{
		if (outNormal)
		{
			*outNormal = totalNormal.normalized();
		}
		if (outPenetration)
		{
			*outPenetration = totalPenetration / hitCount;
		}
		return true;
	}
	return false;
}

//カプセル
bool LineToCapsule(const Line& line, const Capsule& capsule, const int type, Vector3* outHitPoint, float* outHitDist)
{
	Line lineC(capsule.GetPointA(), capsule.GetPointB());

	Vector3 closestLine, closestCapsule;
	float distanceSq = LineToLineDistance(line, lineC, closestLine, closestCapsule, type);

	float radius = capsule.GetRadius();

	if (distanceSq <= radius * radius)
	{
		Vector3 hitPoint = (closestLine + closestCapsule) * 0.5f;

		if (outHitPoint)
			*outHitPoint = hitPoint;

		if (outHitDist)
			*outHitDist = (hitPoint - line.GetStart()).length();

		return true;
	}

	return false;
}

bool PlaneToCapsule(const Plane& plane, const Capsule& capsule, Vector3* outNormal, float* outPenetration)
{
	Vector3 A = capsule.GetPointA();
	Vector3 B = capsule.GetPointB();
	float radius = capsule.GetRadius();

	Vector3 planeNormal = plane.GetNormal();
	float d = plane.GetD();

	// 線分の最近点を計算
	Vector3 segDir = B - A;
	float segLen = segDir.length();
	Vector3 segNorm = segDir / segLen;

	float t = -(A.dot(planeNormal) + d) / segNorm.dot(planeNormal);
	t = Clamp(t, 0.0f, segLen);

	Vector3 pointOnCapsule = A + segNorm * t;

	// 平面からの距離
	float distToPlane = pointOnCapsule.dot(planeNormal) + d;

	if (fabs(distToPlane) <= radius)
	{
		if (outNormal)
		{
			*outNormal = planeNormal;
		}

		if (outPenetration)
		{
			*outPenetration = radius - distToPlane;
		}
		return true;
	}

	return false;
}

bool SphereToCapsule(const Sphere& sphere, const Capsule& capsule, Vector3* outNormal, float* outPenetration)
{
	Vector3 A = capsule.GetPointA();
	Vector3 B = capsule.GetPointB();
	float capsuleRadius = capsule.GetRadius();
	Vector3 sphereCenter = sphere.GetPosition();
	float sphereRadius = sphere.GetRadius();

	Vector3 ab = B - A;
	Vector3 ac = sphereCenter - A;
	float t = ac.dot(ab) / ab.dot(ab);
	t = Clamp(t, 0.0f, 1.0f);

	Vector3 closestPoint = A + ab * t;

	Vector3 diff = sphereCenter - closestPoint;
	float distSq = diff.lengthSq();
	float combinedRadius = sphereRadius + capsuleRadius;

	if (distSq <= combinedRadius * combinedRadius)
	{
		float dist = sqrtf(distSq);

		if (dist > 1e-6f)
		{
			if (outNormal)
			{
				*outNormal = diff / dist; // 正規化
			}
			if (outPenetration)
			{
				*outPenetration = combinedRadius - dist; // ←押し戻し量
			}
		}
		else
		{
			// ほぼ同一点 → 適当な上向きで押し戻す
			if (outNormal)
			{
				*outNormal = Vector3(0, 1, 0);
			}
			if (outPenetration)
			{
				*outPenetration = combinedRadius;
			}
		}

		return true;
	}

	return false;
}

bool BoxToCapsule(const Box& box, const Capsule& capsule, Vector3* outNormal, float* outPenetration)
{
	Line LineC(capsule.GetPointA(), capsule.GetPointB());
	Vector3 ColsestLine, ClosestBox, Normal;
	float Penetration = 0.0f;

	float distanceSq = LineToBoxDistance(LineC, box, ColsestLine, ClosestBox, &Normal, &Penetration);

	if (distanceSq <= capsule.GetRadius() * capsule.GetRadius())
	{
		Vector3 dir = ColsestLine - ClosestBox;

		if (outNormal)
		{
			if (dir.lengthSq() > 1e-6f)
			{
				*outNormal = dir.normalized();
			}
			else
			{
				*outNormal = Normal.normalized();
			}
		}

		if (outPenetration)
		{
			float dist = sqrtf(distanceSq);
			*outPenetration = capsule.GetRadius() - dist;
		}

		return true;
	}

	return false;
}

bool CapsuleToCapsule(const Capsule& capsule1, const Capsule& capsule2, Vector3* outNormal, float* outPenetration)
{
	Line line1(capsule1.GetPointA(), capsule1.GetPointB());
	Line line2(capsule2.GetPointA(), capsule2.GetPointB());

	Vector3 closest1, closest2;
	float distanceSq = LineToLineDistance(line1, line2, closest1, closest2, 0);

	float combinedRadius = capsule1.GetRadius() + capsule2.GetRadius();

	if (distanceSq <= combinedRadius * combinedRadius)
	{
		float dist = sqrtf(distanceSq);
		Vector3 dir = closest1 - closest2;

		if (dist > 1e-6f)
		{
			if (outNormal)
			{
				*outNormal = dir / dist;
			}

			if (outPenetration)
			{
				*outPenetration = combinedRadius - dist; // ←押し戻し量
			}
		}
		else
		{
			// 完全に重なった場合はとりあえず上方向
			if (outNormal)
			{
				*outNormal = Vector3(0, 1, 0);
			}

			if (outPenetration)
			{
				*outPenetration = combinedRadius;
			}
		}

		return true;
	}

	return false;
}

bool CapsuleToTriangle(const Capsule& capsule, const Triangle& tri, Vector3* outNormal, float* outPenetration)
{
	Line line(capsule.GetPointA(), capsule.GetPointB());
	Vector3 closestLine, closestTri;

	float distanceSq = LineToTriangleDistance(line, tri, closestLine, closestTri);

	float radius = capsule.GetRadius();
	if (distanceSq <= radius * radius)
	{
		Vector3 dir = closestLine - closestTri;
		if (outNormal)
		{
			*outNormal = (dir.lengthSq() > 1e-6f) ? dir.normalized() : tri.GetNormal();
		}

		if (outPenetration)
		{
			*outPenetration = radius - sqrtf(distanceSq);
		}

		return true;
	}
	return false;
}

bool CapsuleToMesh(const Capsule& capsule, const CollisionMesh& mesh, Vector3* outNormal, float* outPenetration)
{
	Vector3 totalNormal(0, 0, 0);
	float totalPenetration = 0.0f;
	int hitCount = 0;

	for (const auto& tri : mesh.GetTriangles())
	{
		Vector3 hitNormal;
		float penetration = 0.0f;

		if (CapsuleToTriangle(capsule, tri, &hitNormal, &penetration))
		{
			totalNormal += hitNormal;
			totalPenetration += penetration;
			hitCount++;
		}
	}

	if (hitCount > 0)
	{
		if (outNormal)
		{
			*outNormal = totalNormal.normalized();
		}
		if (outPenetration)
		{
			*outPenetration = totalPenetration / hitCount;
		}
		return true;
	}

	return false;
}




//
//bool PlaneToSphere(const Plane& plane, const Sphere& sphere)
//{
//	//平面の法線
//	XMVECTOR normal = plane.GetNormal().ToXMVECTOR();
//
//	//球体と平面の距離
//	float distance = XMVectorGetX(XMVector3Dot(normal, sphere.GetPosition().ToXMVECTOR())) + plane.GetD();
//
//	return fabsf(distance) < sphere.GetRadius();
//}
//bool FinitePlaneToBox(const Plane& plane, const Box& box)
//{
//
//	// 平面の法線とD
//	XMVECTOR planeNormal = plane.GetNormal().ToXMVECTOR();
//	float planeD = plane.GetD();
//
//	// 各頂点の距離を取得
//	bool hasFront = false;
//	bool hasBack = false;
//
//	for (int i = 0; i < 8; ++i)
//	{
//		float dist = XMVectorGetX(XMVector3Dot(box.GetVertices(i).ToXMVECTOR(), planeNormal)) + planeD;
//		if (dist > 0) hasFront = true;
//		if (dist < 0) hasBack = true;
//	}
//
//	// 平面をまたいでいないなら交差なし
//	if (!(hasFront && hasBack))
//	{
//		return false;
//	}
//
//	// エッジを全探索（各頂点と隣接する軸方向に +1 した点）
//	for (int i = 0; i < 8; ++i)
//	{
//		for (int axis = 0; axis < 3; ++axis) // x, y, z の方向
//		{
//			int j = i ^ (1 << axis); // axisビット反転で隣接頂点
//
//			if (i < j) // 重複チェック防止
//			{
//				const Vector3& p0 = box.GetVertices(i);
//				const Vector3& p1 = box.GetVertices(j);
//
//				Line line = Line(p0, p1);
//
//				Vector3 intersection;
//
//				if (LineToPlane(line, plane, 0, intersection))
//				{
//					if (PointInPlane(intersection, plane))
//					{
//						return true;
//					}
//				}
//			}
//		}
//	}
//
//	return false;
//}
//bool SphereToSphere(const Sphere& sphere1, const Sphere& sphere2)
//{
//	Vector3 diff = sphere1.GetPosition() - sphere2.GetPosition();
//
//	float distSq = diff.lengthSq();
//
//	float radiusSum = sphere1.GetRadius() + sphere2.GetRadius();
//
//	return distSq <= (radiusSum * radiusSum);
//}
//bool SphereToBox(const Sphere& sphere, const Box& box)
//{
//	Vector3 closest = ClosestPointToBox(sphere.GetPosition(), box);
//
//	Vector3 diff = sphere.GetPosition() - closest;
//
//	float distSq = diff.lengthSq();
//
//	return distSq <= (sphere.GetRadius() * sphere.GetRadius());
//}
//bool BoxToBox(const Box& box1, const Box& box2)
//{
//	const float EPSILON = 1e-6f;
//
//	// 回転行列 R (a軸からb軸へ)
//	float R[3][3], AbsR[3][3];
//
//	// ローカル軸
//	Vector3 axisA[3] =
//	{
//		box1.GetAxis(0),
//		box1.GetAxis(1),
//		box1.GetAxis(2)
//	};
//
//	Vector3 axisB[3] =
//	{
//		box2.GetAxis(0),
//		box2.GetAxis(1),
//		box2.GetAxis(2)
//	};
//
//	// R行列（A軸 dot B軸）
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			R[i][j] = axisA[i].dot(axisB[j]);
//		}
//	}
//
//	// b中心 → a中心ベクトルをaのローカルに投影
//	Vector3 tVec = box2.GetPosition() - box1.GetPosition();
//
//	float t[3] =
//	{
//		tVec.dot(axisA[0]),
//		tVec.dot(axisA[1]),
//		tVec.dot(axisA[2])
//	};
//
//	// 安全マージン (AbsR[i][j] = |R[i][j]| + ε)
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
//		}
//	}
//
//
//	float ra, rb;
//
//	// ---------------------------
//	// 1. A軸でテスト
//	for (int i = 0; i < 3; ++i)
//	{
//		ra = box1.GetHalfsize()[i];
//		rb = box2.GetHalfsize().x * AbsR[i][0] + box2.GetHalfsize().y * AbsR[i][1] + box2.GetHalfsize().z * AbsR[i][2];
//		if (fabsf(t[i]) > ra + rb)
//		{
//			return false;
//		}
//	}
//
//	// ---------------------------
//	// 2. B軸でテスト
//	for (int i = 0; i < 3; ++i)
//	{
//		ra = box1.GetHalfsize().x * AbsR[0][i] + box1.GetHalfsize().y * AbsR[1][i] + box1.GetHalfsize().z * AbsR[2][i];
//		rb = box2.GetHalfsize()[i];
//		float proj = fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]);
//		if (proj > ra + rb)
//		{
//			return false;
//		}
//	}
//
//	// ---------------------------
//	// 3. 交差軸（A × B）
//	// i = Aの軸, j = Bの軸
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			ra = box1.GetHalfsize()[(i + 1) % 3] * AbsR[(i + 2) % 3][j] + box1.GetHalfsize()[(i + 2) % 3] * AbsR[(i + 1) % 3][j];
//			rb = box2.GetHalfsize()[(j + 1) % 3] * AbsR[i][(j + 2) % 3] + box2.GetHalfsize()[(j + 2) % 3] * AbsR[i][(j + 1) % 3];
//
//			float proj = fabsf(t[(i + 1) % 3] * R[(i + 2) % 3][j] - t[(i + 2) % 3] * R[(i + 1) % 3][j]);
//			if (proj > ra + rb)
//			{
//				return false;
//			}
//		}
//	}
//
//	// 全ての軸で分離できなかった → 衝突あり
//	return true;
//}
//bool CapsuleToCapsule(const Capsule& capsule1, const Capsule& capsule2)
//{
//	Line line1(capsule1.GetPointA(), capsule1.GetPointB());
//	Line line2(capsule2.GetPointA(), capsule2.GetPointB());
//
//	Vector3 dummy1, dummy2;
//	float distanceSq = LineToLineDistance(line1, line2, dummy1, dummy1, 0);
//
//	float distance = sqrtf(distanceSq);
//
//	return distance <= (capsule1.GetRadius() + capsule2.GetRadius());
//}
//bool BoxToCapsule(const Box& box, const Capsule& capsule)
//{
//	Line LineC(capsule.GetPointA(), capsule.GetPointB());
//	Vector3 ColsestLine, ClosestBox, Normal;
//	float Penetration = 0.0f;
//
//	float distanceSq = LineToBoxDistance(LineC, box, ColsestLine, ClosestBox, Normal, Penetration);
//
//	if (distanceSq <= capsule.GetRadius() * capsule.GetRadius())
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//bool PlaneToCapsule(const Plane& plane, const Capsule& capsule)
//{
//	Vector3 A = capsule.GetPointA();
//	Vector3 B = capsule.GetPointB();
//	float radius = capsule.GetRadius();
//
//	// 平面の法線ベクトル
//	Vector3 planeNormal = plane.GetNormal(); // 単位ベクトル
//	float d = plane.GetD();        // 平面の式：n・P + d = 0
//
//	// 線分の両端点から平面への距離
//	float distA = A.dot(planeNormal) + d;
//	float distB = B.dot(planeNormal) + d;
//
//	// 平面に最も近い点（線分と平面の最近点）
//	Vector3 segDir = B - A;
//	float segLen = segDir.length();
//	Vector3 segNorm = segDir / segLen;
//
//	float t = -(A.dot(planeNormal) + d) / segNorm.dot(planeNormal); // 平面との交差点
//	t = Clamp(t, 0.0f, segLen); // 線分上に制限
//
//	Vector3 pointOnCapsule = A + segNorm * t;
//	Vector3 pointOnPlane = pointOnCapsule - planeNormal * ((pointOnCapsule.dot(planeNormal) + d));
//
//	// 平面内に入っているか？
//	if (!PointInPlane(pointOnPlane, plane))
//	{
//		return false;
//	}
//
//	// 最短距離
//	float distance = fabs((pointOnCapsule.dot(planeNormal) + d));
//
//	if (distance <= radius)
//	{
//		return true;
//	}
//
//	return false;
//}
//bool SphereToCapsule(const Sphere& sphere, const Capsule& capsule)
//{
//	// カプセルの軸（線分）
//	Vector3 A = capsule.GetPointA();
//	Vector3 B = capsule.GetPointB();
//	float capsuleRadius = capsule.GetRadius();
//	Vector3 sphereCenter = sphere.GetPosition();
//	float sphereRadius = sphere.GetRadius();
//
//	// 球体中心から線分ABへの最短距離
//	Vector3 ab = B - A;
//	Vector3 ac = sphereCenter - A;
//	float t = ac.dot(ab) / ab.dot(ab);
//	t = Clamp(t, 0.0f, 1.0f);
//
//	Vector3 closestPoint = A + ab * t;
//
//	// 最短距離
//	float distSquared = (sphereCenter - closestPoint).lengthSq();
//	float combinedRadius = sphereRadius + capsuleRadius;
//
//	return distSquared <= combinedRadius * combinedRadius;
//}
//

//
//bool BoxToCapsule(const Box& box, const Capsule& capsule, Vector3& outNormal)
//{
//	Line LineC(capsule.GetPointA(), capsule.GetPointB());
//	Vector3 ColsestLine, ClosestBox, Normal;
//	float Penetration = 0.0f;
//
//	float distanceSq = LineToBoxDistance(LineC, box, ColsestLine, ClosestBox, Normal, Penetration);
//
//	if (distanceSq <= capsule.GetRadius() * capsule.GetRadius())
//	{
//		Vector3 dir = ColsestLine - ClosestBox;
//		if (dir.lengthSq() > 1e-6f)
//		{
//			outNormal = dir.normalized();
//		}
//		else
//		{
//			outNormal = Normal.normalized();
//		}
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//bool CapsuleToCapsule(const Capsule& capsule1, const Capsule& capsule2, Vector3& outNormal)
//{
//	Line line1(capsule1.GetPointA(), capsule1.GetPointB());
//	Line line2(capsule2.GetPointA(), capsule2.GetPointB());
//
//	Vector3 closest1, closest2;
//	float distanceSq = LineToLineDistance(line1, line2, closest1, closest2, 0);
//
//	float combinedRadius = capsule1.GetRadius() + capsule2.GetRadius();
//
//	if (distanceSq <= combinedRadius * combinedRadius)
//	{
//		Vector3 dir = closest1 - closest2;
//
//		if (dir.lengthSq() > 1e-6f)
//		{
//			outNormal = dir.normalized();
//		}
//		else
//		{
//			outNormal = Vector3(0, 1, 0);
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
//bool PlaneToSphere(const Plane& plane, const Sphere& sphere, Vector3& outNormal)
//{
//	//平面の法線
//	XMVECTOR normal = plane.GetNormal().ToXMVECTOR();
//
//	outNormal = plane.GetNormal();
//
//	//球体と平面の距離
//	float distance = XMVectorGetX(XMVector3Dot(normal, sphere.GetPosition().ToXMVECTOR())) + plane.GetD();
//
//	if (fabsf(distance) < sphere.GetRadius())
//	{
//		return true;
//	}
//
//	return false;
//}
//
//bool FinitePlaneToBox(const Plane& plane, const Box& box, Vector3& outNormal)
//{
//	// 平面の法線とD
//	XMVECTOR planeNormal = plane.GetNormal().ToXMVECTOR();
//	float planeD = plane.GetD();
//
//	// 各頂点の距離を取得
//	bool hasFront = false;
//	bool hasBack = false;
//
//	for (int i = 0; i < 8; ++i)
//	{
//		float dist = XMVectorGetX(XMVector3Dot(box.GetVertices(i).ToXMVECTOR(), planeNormal)) + planeD;
//		if (dist > 0)
//		{
//			hasFront = true;
//		}
//		if (dist < 0)
//		{
//			hasBack = true;
//		}
//	}
//
//	// 平面をまたいでいないなら交差なし
//	if (!(hasFront && hasBack))
//	{
//		return false;
//	}
//
//	// エッジを全探索（各頂点と隣接する軸方向に +1 した点）
//	for (int i = 0; i < 8; ++i)
//	{
//		for (int axis = 0; axis < 3; ++axis) // x, y, z の方向
//		{
//			int j = i ^ (1 << axis); // axisビット反転で隣接頂点
//
//			if (i < j) // 重複チェック防止
//			{
//				const Vector3& p0 = box.GetVertices(i);
//				const Vector3& p1 = box.GetVertices(j);
//
//				Line line = Line(p0, p1);
//
//				Vector3 intersection;
//
//				if (LineToPlane(line, plane, 0, intersection))
//				{
//					if (PointInPlane(intersection, plane))
//					{
//						outNormal = plane.GetNormal();
//						return true;
//					}
//				}
//			}
//		}
//	}
//
//	return false;
//}
//
//bool SphereToSphere(const Sphere& sphere1, const Sphere& sphere2, Vector3& outNormal)
//{
//	Vector3 diff = sphere2.GetPosition() - sphere1.GetPosition();
//
//	float distSq = diff.lengthSq();
//
//	float radiusSum = sphere1.GetRadius() + sphere2.GetRadius();
//
//	if (distSq <= radiusSum * radiusSum)
//	{
//		float dist = sqrtf(distSq);
//		if (dist > 1e-6f)
//		{
//			outNormal = diff / dist;
//		}
//		else
//		{
//			// 完全重なっている場合はY軸上向き
//			outNormal = Vector3(0, 1, 0);
//			dist = 0.0f;
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
//bool SphereToBox(const Sphere& sphere, const Box& box, Vector3& outNormal)
//{
//	Vector3 closest = ClosestPointToBox(sphere.GetPosition(), box);
//	Vector3 diff = sphere.GetPosition() - closest;
//	float distSq = diff.lengthSq();
//	float radius = sphere.GetRadius();
//
//	if (distSq <= radius * radius)
//	{
//		float dist = sqrtf(distSq);
//
//		if (distSq > 1e-6f)
//		{
//			outNormal = diff / dist;
//		}
//		else
//		{
//			outNormal = (sphere.GetPosition() - box.GetPosition()).normalized();
//		}
//		return true;
//	}
//
//	return false;
//}
//
//bool BoxToBox(const Box& box1, const Box& box2, Vector3& outNormal)
//{
//	const float EPSILON = 1e-6f;
//
//	float R[3][3], AbsR[3][3];
//	float ra, rb;
//
//	// 最小押し戻し
//	float minOverlap = FLT_MAX;
//	Vector3 minAxis;
//
//	// ローカル軸
//	Vector3 axisA[3] =
//	{
//		box1.GetAxis(0),
//		box1.GetAxis(1),
//		box1.GetAxis(2)
//	};
//
//	Vector3 axisB[3] =
//	{
//		box2.GetAxis(0),
//		box2.GetAxis(1),
//		box2.GetAxis(2)
//	};
//
//	// R行列（A軸 dot B軸）
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			R[i][j] = axisA[i].dot(axisB[j]);
//		}
//	}
//
//	// 中心差をAのローカルへ
//	Vector3 tVec = box2.GetPosition() - box1.GetPosition();
//	float t[3] =
//	{
//		tVec.dot(axisA[0]),
//		tVec.dot(axisA[1]),
//		tVec.dot(axisA[2])
//	};
//
//	// 安全マージン (AbsR[i][j] = |R[i][j]| + ε)
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
//		}
//	}
//
//	// ---------------------------
//	// 1. A軸でテスト
//	for (int i = 0; i < 3; ++i)
//	{
//		ra = box1.GetHalfsize()[i];
//		rb = box2.GetHalfsize().x * AbsR[i][0] + box2.GetHalfsize().y * AbsR[i][1] + box2.GetHalfsize().z * AbsR[i][2];
//
//		float overlap = ra + rb - fabsf(t[i]);
//		if (overlap < 0)
//		{
//			return false;
//		}
//
//		if (overlap < minOverlap)
//		{
//			minOverlap = overlap;
//			minAxis = axisA[i] * ((t[i] < 0.0f) ? -1.0f : 1.0f);  // 軸向き調整
//		}
//	}
//
//	// ---------------------------
//	// 2. B軸でテスト
//	for (int i = 0; i < 3; ++i)
//	{
//		ra = box1.GetHalfsize().x * AbsR[0][i] + box1.GetHalfsize().y * AbsR[1][i] + box1.GetHalfsize().z * AbsR[2][i];
//		rb = box2.GetHalfsize()[i];
//
//		float proj = t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i];
//		float overlap = ra + rb - fabsf(proj);
//		if (overlap < 0)
//		{
//			return false;
//		}
//
//		if (overlap < minOverlap)
//		{
//			minOverlap = overlap;
//			minAxis = axisB[i] * ((proj < 0.0f) ? -1.0f : 1.0f);
//		}
//	}
//
//	// ---------------------------
//	// 3. A × B軸でテスト
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			// 軸 = A[i] × B[j]
//			Vector3 axis = axisA[i].cross(axisB[j]);
//			if (axis.lengthSq() < EPSILON)
//			{
//				continue; // 平行なので無視
//			}
//
//			axis = axis.normalized();
//
//			ra = box1.GetHalfsize()[(i + 1) % 3] * AbsR[(i + 2) % 3][j] +
//				box1.GetHalfsize()[(i + 2) % 3] * AbsR[(i + 1) % 3][j];
//
//			rb = box2.GetHalfsize()[(j + 1) % 3] * AbsR[i][(j + 2) % 3] +
//				box2.GetHalfsize()[(j + 2) % 3] * AbsR[i][(j + 1) % 3];
//
//			float proj = fabsf(t[(i + 1) % 3] * R[(i + 2) % 3][j] - t[(i + 2) % 3] * R[(i + 1) % 3][j]);
//			float overlap = ra + rb - proj;
//
//			if (overlap < 0)
//			{
//				return false;
//			}
//
//			if (overlap < minOverlap)
//			{
//				minOverlap = overlap;
//				minAxis = axis * ((tVec.dot(axis) < 0.0f) ? -1.0f : 1.0f);
//			}
//		}
//	}
//
//	// 最小侵入ベクトル
//	outNormal = minAxis.normalized();
//
//	return true;
//}
//
//bool PlaneToCapsule(const Plane& plane, const Capsule& capsule, Vector3& outNormal)
//{
//	Vector3 A = capsule.GetPointA();
//	Vector3 B = capsule.GetPointB();
//	float radius = capsule.GetRadius();
//
//	// 平面の法線ベクトル
//	Vector3 planeNormal = plane.GetNormal(); // 単位ベクトル
//	float d = plane.GetD();        // 平面の式：n・P + d = 0
//
//	// 線分の両端点から平面への距離
//	float distA = A.dot(planeNormal) + d;
//	float distB = B.dot(planeNormal) + d;
//
//	// 平面に最も近い点（線分と平面の最近点）
//	Vector3 segDir = B - A;
//	float segLen = segDir.length();
//	Vector3 segNorm = segDir / segLen;
//
//	float t = -(A.dot(planeNormal) + d) / segNorm.dot(planeNormal); // 平面との交差点
//	t = Clamp(t, 0.0f, segLen); // 線分上に制限
//
//	Vector3 pointOnCapsule = A + segNorm * t;
//	Vector3 pointOnPlane = pointOnCapsule - planeNormal * ((pointOnCapsule.dot(planeNormal) + d));
//
//	// 平面内に入っているか？
//	if (!PointInPlane(pointOnPlane, plane))
//	{
//		return false;
//	}
//
//	// 最短距離
//	float distance = fabs((pointOnCapsule.dot(planeNormal) + d));
//
//	if (distance <= radius)
//	{
//		outNormal = plane.GetNormal();
//		return true;
//	}
//
//	return false;
//}
//
//bool SphereToCapsule(const Sphere& sphere, const Capsule& capsule, Vector3& outNormal)
//{
//	// カプセルの軸（線分）
//	Vector3 A = capsule.GetPointA();
//	Vector3 B = capsule.GetPointB();
//	float capsuleRadius = capsule.GetRadius();
//	Vector3 sphereCenter = sphere.GetPosition();
//	float sphereRadius = sphere.GetRadius();
//
//	// 球体中心から線分ABへの最短距離
//	Vector3 ab = B - A;
//	Vector3 ac = sphereCenter - A;
//	float t = ac.dot(ab) / ab.dot(ab);
//	t = Clamp(t, 0.0f, 1.0f);
//
//	Vector3 closestPoint = A + ab * t;
//
//	// 最短距離
//	Vector3 diff = sphereCenter - closestPoint;
//	float distSq = diff.lengthSq();
//	float combinedRadius = sphereRadius + capsuleRadius;
//
//	if (distSq <= combinedRadius * combinedRadius)
//	{
//
//		if (distSq > 1e-6f)
//		{
//			outNormal = diff.normalized();
//		}
//		else
//		{
//			outNormal = Vector3(0, 1, 0);
//		}
//
//		return true;
//	}
//
//	return false;
//}
//
