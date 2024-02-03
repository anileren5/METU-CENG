#include "DrawRelated.h"
#define PI 3.141592654
//Helpers
//Done
void drawLine(Scene *scene, Vec4& v1, Vec4 &v2, Camera *camera) {
    int x0 = v1.x;
    int y0 = v1.y;
    int x1 = v2.x;
    int y1 = v2.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

	double linearDistance = std::sqrt(dx * dx + dy * dy);
	double ratioX = static_cast<double>(dx) / linearDistance;
	double ratioY = static_cast<double>(dy) / linearDistance;

    double depth;

    Color currentColor = *scene->colorsOfVertices[v1.colorId - 1];
    Color unitColorDifference;

    if (dy <= dx) {
        unitColorDifference.r = ((scene->colorsOfVertices[v2.colorId - 1])->r - currentColor.r) / linearDistance;
        unitColorDifference.g = ((scene->colorsOfVertices[v2.colorId - 1])->g - currentColor.g) / linearDistance;
        unitColorDifference.b = ((scene->colorsOfVertices[v2.colorId - 1])->b - currentColor.b) / linearDistance;
    } else {
        unitColorDifference.r = ((scene->colorsOfVertices[v2.colorId - 1])->r - currentColor.r) / linearDistance;
        unitColorDifference.g = ((scene->colorsOfVertices[v2.colorId - 1])->g - currentColor.g) / linearDistance;
        unitColorDifference.b = ((scene->colorsOfVertices[v2.colorId - 1])->b - currentColor.b) / linearDistance;
    }

    while (true) {
        if (x0 >= 0 && x0 < camera->horRes && y0 >= 0 && y0 < camera->verRes) {
            // Calculate depth at the current pixel
            depth = v1.z + (v2.z - v1.z) * (static_cast<double>(x0 - v1.x) / static_cast<double>(x1 - v1.x));

            // Check if the calculated depth is less than the depth in the depth buffer
            if (depth < scene->depth[x0][y0]) {
                // Update the depth buffer and pixel color
                scene->depth[x0][y0] = depth;
                scene->image[x0][y0] = currentColor;
            }
        }

        if (x0 == x1 && y0 == y1) {
            break;
        }

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
            currentColor.r += ratioX * unitColorDifference.r;
            currentColor.g += ratioX * unitColorDifference.g;
            currentColor.b += ratioX * unitColorDifference.b;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
            currentColor.r += ratioY * unitColorDifference.r;
            currentColor.g += ratioY * unitColorDifference.g;
            currentColor.b += ratioY * unitColorDifference.b;
        }
    }
}


double calculateTriangleArea(double xCoord, double yCoord, double x_0, double y_0, double x_1, double y_1){
    double xComponent = xCoord * (y_0 - y_1);
    double yComponent = yCoord * (x_1 - x_0);
    double defaultComponent = (x_0 * y_1) - (y_0 * x_1);
    return xComponent + yComponent + defaultComponent;
}
void triangleRaster(Scene *scene, Vec4 &vertex0, Vec4 &vertex1, Vec4 &vertex2, Camera *camera) {
    // Calculate bounding box
    double xMin = std::min(vertex0.x, std::min(vertex1.x, vertex2.x));
    double yMin = std::min(vertex0.y, std::min(vertex1.y, vertex2.y));
    double xMax = std::max(vertex0.x, std::max(vertex1.x, vertex2.x));
    double yMax = std::max(vertex0.y, std::max(vertex1.y, vertex2.y));

    xMin = round(xMin);
    yMin = round(yMin);
    xMax = round(xMax);
    yMax = round(yMax);

    // Get vertex colors
    Color color0 = *(scene->colorsOfVertices[vertex0.colorId - 1]);
    Color color1 = *(scene->colorsOfVertices[vertex1.colorId - 1]);
    Color color2 = *(scene->colorsOfVertices[vertex2.colorId - 1]);

    for (int xCoord = xMin; xCoord <= xMax; xCoord++) {
        if (xCoord < 0) continue;

        if (xCoord >= camera->horRes) break;

        for (int yCoord = yMin; yCoord <= yMax; yCoord++) {
            if (yCoord < 0) continue;
            if (yCoord >= camera->verRes) break;

            double alpha = calculateTriangleArea(xCoord, yCoord, vertex1.x, vertex1.y, vertex2.x, vertex2.y) /
                           calculateTriangleArea(vertex0.x, vertex0.y, vertex1.x, vertex1.y, vertex2.x, vertex2.y);
            double beta = calculateTriangleArea(xCoord, yCoord, vertex2.x, vertex2.y, vertex0.x, vertex0.y) /
                          calculateTriangleArea(vertex1.x, vertex1.y, vertex2.x, vertex2.y, vertex0.x, vertex0.y);
            double gamma = calculateTriangleArea(xCoord, yCoord, vertex0.x, vertex0.y, vertex1.x, vertex1.y) /
                           calculateTriangleArea(vertex2.x, vertex2.y, vertex0.x, vertex0.y, vertex1.x, vertex1.y);

            if (alpha < 0 || beta < 0 || gamma < 0) {
				//Not in the triangle
                continue;
            }

            // Interpolate depth value
            double depth = alpha * vertex0.z + beta * vertex1.z + gamma * vertex2.z;

            // Perform depth testing
            if (depth < scene->depth[xCoord][yCoord]) {
                // Update depth buffer and pixel color
                scene->depth[xCoord][yCoord] = depth;

                Color currentColor;
                currentColor.r = alpha * color0.r + beta * color1.r + gamma * color2.r;
                currentColor.g = alpha * color0.g + beta * color1.g + gamma * color2.g;
                currentColor.b = alpha * color0.b + beta * color1.b + gamma * color2.b;

                scene->image[xCoord][yCoord] = currentColor;
            }
        }
    }
}

bool facingBack(Vec4 &pointA, Vec4 &pointB, Vec4 &pointC) {
    // Extract Vec3 components with colorId from Vec4
	Vec3 eyeRay = Vec3((pointA.x+pointB.x+pointC.x)/3,(pointA.y+pointB.y+pointC.y)/3,(pointA.z+pointB.z+pointC.z)/3, -1);
    Vec3 vectorAB = Vec3(pointB.x - pointA.x, pointB.y - pointA.y, pointB.z - pointA.z, -1);
    Vec3 vectorAC = Vec3(pointC.x - pointA.x, pointC.y - pointA.y, pointC.z - pointA.z, -1);

    // Calculate vectors from pointA to pointB and pointC
    Vec3 crossProductResult = crossProductVec3(vectorAB, vectorAC);

    // Calculate the dot product of the normal vector and vector from pointA
    double dotProduct = dotProductVec3(crossProductResult, eyeRay);

    // Check if the dot product is less than 0
    return dotProduct < 0;
}


bool isVisible(double slope, double intercept, double &minThreshold, double &maxThreshold) {
    // Calculate the ratio of intercept to slope.
    double ratio = intercept / slope;

    // Check for positive slope.
    if (slope > 0) {
        // Check if the ratio exceeds the maximum threshold.
        if (ratio > maxThreshold) {
            // Point is not visible beyond the maximum threshold.
            return false;
        }
		// Update the minimum threshold if the ratio is greater.
		minThreshold = ratio;
    } 
    // Check for negative slope.
    else if (slope < 0) {
        // Check if the ratio is below the minimum threshold.
        if (ratio < minThreshold) {
            // Point is not visible before the minimum threshold.
            return false;
        }
		// Update the maximum threshold if the ratio is smaller.
		maxThreshold = ratio;
    } 
    // Check for non-positive slope and positive intercept.
    else if (intercept > 0) {
        // Point is not visible if intercept is positive.
        return false;
    }

    // The point is visible within the specified range.
    return true;
}


bool clippedLine(Scene* scene, Vec4& startVertex, Vec4& endVertex, Color& startColor, Color& endColor) {
    bool result = false;
    Vec4 clippedStartVertex = startVertex, clippedEndVertex = endVertex;
    int visibleAxesCount = 0;

    // Initialize thresholds and deltas
    double tNear = 0, tFar = 1;
    double deltas[] = {endVertex.x - startVertex.x, endVertex.y - startVertex.y, endVertex.z - startVertex.z};
    double minThreshold = -1;
    double maxThreshold = 1;

    Color colorDelta = {endColor.r - startColor.r, endColor.g - startColor.g, endColor.b - startColor.b};
    Color* clippedStartColor = new Color(startColor);
    Color* clippedEndColor = new Color(endColor);

    int newColorIdStart = startVertex.colorId;
    int newColorIdEnd = endVertex.colorId;

    // Check visibility along each axis
    for (int axis = 0; axis < 3; axis++) {
        if (isVisible(deltas[axis], minThreshold - startVertex.getNthComponent(axis), tNear, tFar) &&
            isVisible(-deltas[axis], startVertex.getNthComponent(axis) - maxThreshold, tNear, tFar)) {
            visibleAxesCount++;
        }
    }

    // Check if the line segment is visible in all axes
    if (visibleAxesCount == 3) {
        result = true;

        // Clip against near plane (tNear)
        if (tNear > 0.0) {
            startVertex.x += deltas[0] * tNear;
            startVertex.y += deltas[1] * tNear;
            startVertex.z += deltas[2] * tNear;

            clippedStartColor->r += colorDelta.r * tNear;
            clippedStartColor->g += colorDelta.g * tNear;
            clippedStartColor->b += colorDelta.b * tNear;

            scene->colorsOfVertices.push_back(clippedStartColor);
            newColorIdStart = scene->colorsOfVertices.size();
        }

        // Clip against far plane (tFar)
        if (tFar < 1.0) {
            clippedEndVertex.x = startVertex.x + deltas[0] * tFar;
            clippedEndVertex.y = startVertex.y + deltas[1] * tFar;
            clippedEndVertex.z = startVertex.z + deltas[2] * tFar;

            clippedEndColor->r = clippedStartColor->r + colorDelta.r * tFar;
            clippedEndColor->g = clippedStartColor->g + colorDelta.g * tFar;
            clippedEndColor->b = clippedStartColor->b + colorDelta.b * tFar;

            scene->colorsOfVertices.push_back(clippedEndColor);
            newColorIdEnd = scene->colorsOfVertices.size();
        }
    }

    // Update output vectors
    endVertex = clippedEndVertex;
    endVertex.colorId = newColorIdEnd;
    startVertex = clippedStartVertex;
    startVertex.colorId = newColorIdStart;

    return result;
}