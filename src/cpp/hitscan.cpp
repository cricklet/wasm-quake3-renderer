#include "hitscan.h"
#include "bsp.h"

static optional<float> distanceToTriangle(
  const glm::vec3& location,
  const glm::vec3& direction,
  const glm::vec3& vertex0,
  const glm::vec3& vertex1,
  const glm::vec3& vertex2
) {
  // Thank you: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
  const float EPSILON = 0.0000001;
  glm::vec3 edge1, edge2, h, s, q;
  float a,f,u,v;
  edge1 = vertex1 - vertex0;
  edge2 = vertex2 - vertex0;

  h = cross(direction, edge2);
  a = dot(edge1, h);
  if (a > -EPSILON && a < EPSILON) {
    return {};    // This ray is parallel to this triangle.
  }

  f = 1.0/a;
  s = location - vertex0;
  u = f * dot(s, h);

  if (u < 0.0 || u > 1.0) {
    return {};
  }
  q = cross(s, edge1);
  v = f * dot(direction, q);
  if (v < 0.0 || u + v > 1.0) {
    return {};
  }

  // At this stage we can compute t to find out where the intersection point is on the line.
  float t = f * dot(edge2, q);
  if (t > EPSILON) {
    // Ray intersection
    // => outIntersectionPoint = location + direction * t;
    return t;
  }

  // This means that there is a line intersection but not a ray intersection.
  return {};
}

optional<HitScanResult> HitScan::findFaceIndex(const BSPMap* map, glm::vec3 location, glm::vec3 direction) {
  int numFaces = map->numFaces();
  optional<HitScanResult> bestHitResult = {};

  for (int faceIndex = 0; faceIndex < numFaces; faceIndex ++) {
    const BSP::face_t* face = map->faces() + faceIndex;
    const BSP::vertex_t* startVertex = map->vertices() + face->vertex;
    const BSP::meshvert_t* startMesh = map->meshverts() + face->meshvert;

    for (int meshVertIndex = 0; meshVertIndex < face->n_meshverts; meshVertIndex += 3) {
      const BSP::meshvert_t* mesh = startMesh + meshVertIndex;
      const float* v1 = (startVertex + (mesh + 0)->offset)->position;
      const float* v2 = (startVertex + (mesh + 1)->offset)->position;
      const float* v3 = (startVertex + (mesh + 2)->offset)->position;

      optional<float> distance = distanceToTriangle(
        location, direction,
        glm::vec3{ v1[0], v1[1], v1[2] },
        glm::vec3{ v2[0], v2[1], v2[2] },
        glm::vec3{ v3[0], v3[1], v3[2] });

      if (distance && (!bestHitResult || distance < bestHitResult->distance)) {
        bestHitResult = {
          face, mesh, *distance
        };
      }
    }
  }

  return bestHitResult;
}