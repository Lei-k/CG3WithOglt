#include "oglt_mesh.h"

oglt::Mesh::Mesh()
{
	startIndex = 0;
	size = 0;
	materialIndex = 0;
	mtlMapMode = NONE;
}

oglt::Mesh::~Mesh()
{
}

void oglt::Mesh::sortTrianglesByMtl()
{
}

void oglt::Mesh::buildPolygonsPerMtl(bool clearTriangles)
{
	if (ESZ(triangles) <= 0) {
		fprintf(stderr, "Error: no triangles data can build the polygons.");
		return;
	}

	Polygon polygon;
	polygon.startIndex = triangles[0].startIndex;
	polygon.materialIndex = triangles[0].materialIndex;
	FOR(i, ESZ(triangles)) {
		if (polygon.materialIndex != triangles[i].materialIndex) {
			polygon.size = triangles[i].startIndex - polygon.startIndex;
			polygons.push_back(polygon);
			polygon.startIndex = triangles[i].startIndex;
			polygon.materialIndex = triangles[i].materialIndex;
		}
	}

	polygon.size = triangles[ESZ(triangles) - 1].startIndex - polygon.startIndex + 3;
	// push the remain polygon
	polygons.push_back(polygon);

	if (clearTriangles) {
		triangles.clear();
	}
}
