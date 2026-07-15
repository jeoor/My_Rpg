#ifndef __CAMERA_H__
#define __CAMERA_H__
class Camera
{
  public:
	static Camera &getInstance();
	Camera(const Camera &) = delete;
	Camera &operator=(const Camera &) = delete;

	void init(int worldW, int worldH, int screenW, int screenH);
	void setTarget(double x, double y);
	void update();
	int getOffsetX() const;
	int getOffsetY() const;

  private:
	Camera() = default;

	int worldW = 0, worldH = 0;
	int screenW = 0, screenH = 0;
	double targetX = 0.0, targetY = 0.0;
	double currentX = 0.0, currentY = 0.0;
};
#endif
