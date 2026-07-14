#include "camera.h"
#include "config.h"
#include <algorithm>

Camera &Camera::getInstance()
{
	static Camera instance;
	return instance;
}

void Camera::init(int ww, int wh, int sw, int sh)
{
	worldW = ww;
	worldH = wh;
	screenW = sw;
	screenH = sh;
	currentX = ww / 2.0 - sw / 2.0;
	currentY = wh / 2.0 - sh / 2.0;
}

void Camera::setTarget(double x, double y)
{
	targetX = x;
	targetY = y;
}

void Camera::update()
{
	double desiredX = targetX - screenW / 2.0;
	double desiredY = targetY - screenH / 2.0;

	currentX += (desiredX - currentX) * CAMERA_LERP;
	currentY += (desiredY - currentY) * CAMERA_LERP;

	currentX = std::clamp(currentX, 0.0, static_cast<double>(worldW - screenW));
	currentY = std::clamp(currentY, 0.0, static_cast<double>(worldH - screenH));
}

int Camera::getOffsetX() const { return static_cast<int>(currentX); }
int Camera::getOffsetY() const { return static_cast<int>(currentY); }
