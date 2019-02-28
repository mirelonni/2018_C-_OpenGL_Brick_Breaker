#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateCircle(std::string name, glm::vec3 center, float radius, int steps, glm::vec3 color, bool fill)
{

	std::vector<VertexFormat> ballVertices;

	ballVertices.push_back(VertexFormat(center, color));
	
	std::vector<unsigned short> indices;
	indices.push_back(0);

	float angle = 0.0f;
	int i = 1;

	while (angle <= 360.0f) {
		ballVertices.push_back(VertexFormat(glm::vec3(radius * cos(angle * M_PI / 180.0f), radius * sin(angle * M_PI / 180.0f), 0), color));
		indices.push_back(i);
		angle += 360.0f / steps;
		i++;
	}

	if (360 % steps != 0) {
		
		indices.push_back(1); // add the first ball point if not a perfect circle
	}

	Mesh* square = new Mesh(name);
	
	square->SetDrawMode(GL_TRIANGLE_FAN);
	
	square->InitFromData(ballVertices, indices);
	return square;
}

Mesh* Object2D::CreateBrick(std::string name, glm::vec3 center, float width, float height, glm::vec3 color, bool fill)
{
	
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center, color),
		VertexFormat(glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(glm::vec3(-width / 2, height / 2, 0), color),
		VertexFormat(glm::vec3(-width / 2, -height / 2, 0), color),
		VertexFormat(glm::vec3(width / 2, -height / 2, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4, 1};
	
	square->SetDrawMode(GL_TRIANGLE_FAN);
	
	square->InitFromData(vertices, indices);
	return square;
}