#ifndef __CAMEAR_H__
#define __CAMEAR_H__

class Camera
{
	public:
	Camera() = default;
	~Camera() = default;
	void set(double x, double y);
	void update();
	double getX() const;
	double getY() const;
	int getW() const;
	int getH() const;
private:
	double Cax = 0.0, Cay = 0.0; // 相机位置
	int Caw = 0, Cah = 0;         // 相机宽高
	int W = 0, H = 0; // 偏移量
};

#endif
