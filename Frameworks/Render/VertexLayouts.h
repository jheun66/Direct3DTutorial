#pragma once

// VertexLayouts

struct Vertex
{
	Float3 position;

	Vertex() : position(0, 0, 0)
	{

	}

};

struct VertexColor
{
	Float3 position;
	Float4 color;

	VertexColor() :position(0, 0, 0), color(1, 1, 1, 1)
	{

	}
};