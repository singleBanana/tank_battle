/*
* Copyright (c) 2008 Vitaliano Palmieri Neto
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "PhysicsDebugDraw.h"
#include "gameplay.h"
#include "box2ddef.h"

#include <vector>

PhysicsDebugDraw::PhysicsDebugDraw()
{
	m_HGE = GamePlay::GetInstance()->GetHGE();
}

void PhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for (int i = 0; i<vertexCount; i++)
	{
		const int nextVertex = (i + 1) % vertexCount;
		m_HGE->Gfx_RenderLine(vertices[i].x * BOX2D_SCALE_RATIO,
			vertices[i].y * BOX2D_SCALE_RATIO,
			vertices[nextVertex].x * BOX2D_SCALE_RATIO,
			vertices[nextVertex].y * BOX2D_SCALE_RATIO,
			ARGB(255, color.r * 255, color.g * 255, color.b * 255));
	}
}

void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for (int i = 0; i<vertexCount; i++)
	{
		const int nextVertex = (i + 1) % vertexCount;
		m_HGE->Gfx_RenderLine(vertices[i].x * BOX2D_SCALE_RATIO,
			vertices[i].y * BOX2D_SCALE_RATIO,
			vertices[nextVertex].x * BOX2D_SCALE_RATIO,
			vertices[nextVertex].y * BOX2D_SCALE_RATIO,
			ARGB(255, color.r * 255, color.g * 255, color.b * 255));
	}
}

void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const int cNumPoints = 24 + (int)(radius * BOX2D_SCALE_RATIO) / 20;
	const float cPI = 3.14159f;

	std::vector<b2Vec2> vertices;

	const float cWedgeAngle = (float)((2 * cPI) / cNumPoints);

	for (int i = 0; i<cNumPoints; i++)
	{
		const float theta = i * cWedgeAngle;
		const float x = (float)(center.x * BOX2D_SCALE_RATIO + radius * BOX2D_SCALE_RATIO * cos(theta));
		const float y = (float)(center.y * BOX2D_SCALE_RATIO - radius * BOX2D_SCALE_RATIO * sin(theta));

		vertices.push_back(b2Vec2(x, y));
	}

	const unsigned int cSize = vertices.size();
	for (unsigned int k = 0; k<cSize; k++)
	{
		const int nextVertex = (k + 1) % cSize;
		m_HGE->Gfx_RenderLine(vertices[k].x,
			vertices[k].y,
			vertices[nextVertex].x,
			vertices[nextVertex].y,
			ARGB(255, color.r * 255, color.g * 255, color.b * 255));
	}
}

void PhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const int cNumPoints = 24 + (int)(radius * BOX2D_SCALE_RATIO) / 20; //24;
	const float cPI = 3.14159f;

	std::vector<b2Vec2> vertices;

	const float cWedgeAngle = (float)((2 * cPI) / cNumPoints);

	for (int i = 0; i<cNumPoints; i++)
	{
		const float theta = i * cWedgeAngle;
		const float x = (float)(center.x * BOX2D_SCALE_RATIO + radius * BOX2D_SCALE_RATIO * cos(theta));
		const float y = (float)(center.y * BOX2D_SCALE_RATIO - radius * BOX2D_SCALE_RATIO * sin(theta));

		vertices.push_back(b2Vec2(x, y));
	}

	const unsigned int cSize = vertices.size();
	for (unsigned int k = 0; k<cSize; k++)
	{
		const int nextVertex = (k + 1) % cSize;
		m_HGE->Gfx_RenderLine(vertices[k].x,
			vertices[k].y,
			vertices[nextVertex].x,
			vertices[nextVertex].y,
			ARGB(200, color.r * 128, color.g * 128, color.b * 128));
	}
}

void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	m_HGE->Gfx_RenderLine(p1.x * BOX2D_SCALE_RATIO, p1.y * BOX2D_SCALE_RATIO, p2.x * BOX2D_SCALE_RATIO, p2.y * BOX2D_SCALE_RATIO, 
		ARGB(255, color.r * 255, color.g * 255, color.b * 255));
}

void PhysicsDebugDraw::DrawTransform(const b2Transform& xf)
{
	/*const b2Vec2 v1 = xf.position;
	const float k_axisScale = 0.4f;

	b2Vec2 v2 = v1 + k_axisScale * xf.R.col1;
	m_HGE->Gfx_RenderLine(v1.x, v1.y, v2.x, v2.y, ARGB(255, 255, 0, 0));

	v2 = v1 + k_axisScale * xf.R.col2;
	m_HGE->Gfx_RenderLine(v1.x, v1.y, v2.x, v2.y, ARGB(255, 255, 0, 0));*/
}