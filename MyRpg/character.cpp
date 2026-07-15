#include "character.h"
#include "collision.h"
#include <algorithm>
void Character::set(double x, double y, Animation *anims, int animationTotal)
{
	currentX = x, currentY = y;
	animations = anims;
	animationCount = animationTotal;
}
void Character::set(double x, double y) { currentX = x, currentY = y; }
void Character::set(Animation *anims, int animationTotal)
{
	animations = anims;
	animationCount = animationTotal;
}
void Character::updateAnimation()
{
	// 锟斤拷锟斤拷锟斤拷烁锟斤拷锟杰伙拷状态
	if (!canReduceHp)
	{
		timer += DELTA;
		if (timer >= 1.5 * DELTA_TIME)
		{
			timer = 0;
			canReduceHp = true;
		}
	}

	if (animationCount == 0)
		return;

	// 锟竭斤拷锟斤拷锟斤拷锟�
	// 世界边界钳制
	currentX = std::clamp(currentX, 0.0, static_cast<double>(WORLD_W));
	currentY = std::clamp(currentY, 0.0, static_cast<double>(WORLD_H));

	// 脚底阴影
	static IMAGE shadowImg;
	static bool shadowLoaded = false;
	if (!shadowLoaded)
	{
		loadimage(&shadowImg, L"source/characters/shadow/shadow.png", 9, 3, true);
		shadowLoaded = true;
	}
	int sw = 9 * ZOOM_RATE, sh = 3 * ZOOM_RATE;
	putAlpha((int)currentX - sw / 2, (int)currentY - sh / 2, &shadowImg, sw, sh);

	animations[currentAnimation].play(currentX, currentY, flip, canReduceHp);

	// 锟斤拷锟斤拷血锟斤拷
	setlinecolor(WHITE);
	setlinestyle(PS_SOLID, 2);
	double rate = getHpRate();
	double length = 30.0;
	double width = 5.0;
	double hpLeft = getX() - length / 2;
	double hpTop = getY() - getHeight() * 1.0 - width;
	double hpRight = getX() - length / 2 + length * rate;
	double hpBottom = getY() - getHeight() * 1.0;
	setfillcolor(BLACK);
	fillrectangle(static_cast<int>(hpLeft), static_cast<int>(hpTop), static_cast<int>(hpLeft + length),
				  static_cast<int>(hpBottom));
	setfillcolor(GREEN);
	fillrectangle(static_cast<int>(hpLeft), static_cast<int>(hpTop), static_cast<int>(hpRight),
				  static_cast<int>(hpBottom));
}
void Character::updateSpeed()
{
	// 默锟较硷拷锟斤拷锟竭硷拷
	if (isMoving() || haveT())
	{
		if (currentSpeed < maxSpeed)
		{
			currentSpeed += acceleration;
			if (currentSpeed > maxSpeed)
				currentSpeed = maxSpeed;
		}
	}
	else
	{
		if (currentSpeed > 0.0)
		{
			currentSpeed -= acceleration;
			if (currentSpeed < 0.0)
				currentSpeed = 0.0;
		}
	}
}
void Character::move()
{
	if (isMoveLeft)
	{
		if (d == dir::right)
		{
			changeFlip();
			d = dir::left;
		}
	}
	if (isMoveRight)
	{
		if (d == dir::left)
		{
			changeFlip();
			d = dir::right;
		}
	}

	int deltaX = isMoveRight - isMoveLeft;
	int deltaY = isMoveDown - isMoveUp;

	double deltaLength = sqrt(deltaX * deltaX + deltaY * deltaY);

	updateSpeed(); // 统一锟斤拷锟斤拷/锟斤拷锟斤拷锟竭硷拷

	if (deltaLength != 0 && currentSpeed > 0.0)
	{
		double nx = currentX + deltaX * currentSpeed * ZOOM_RATE / deltaLength;
		double ny = currentY + deltaY * currentSpeed * ZOOM_RATE / deltaLength;
		if (!collideObstacle((int)nx, (int)ny, collision * ZOOM_RATE, gObstacles))
		{
			currentX = nx;
			currentY = ny;
		}
	}
}
void Character::move2(int x, int y, bool slowDown)
{
	double deltaX = static_cast<double>(x) - currentX;
	double deltaY = static_cast<double>(y) - currentY;
	double deltaLength = sqrt(deltaX * deltaX + deltaY * deltaY);

	// 锟斤拷锟斤拷锟斤拷小锟斤拷锟斤拷锟叫断ｏ拷锟斤拷止锟斤拷锟节接斤拷
	const double minDistance = ZOOM_RATE * 5.0;		  // 锟斤拷小锟斤拷锟街撅拷锟斤拷
	const double slowDownDistance = ZOOM_RATE * 10.0; // 锟斤拷始锟斤拷锟劫的撅拷锟斤拷

	if (deltaLength > minDistance)
	{
		// 只锟斤拷x锟斤拷锟斤拷锟斤拷锟斤拷锟皆诧拷锟绞憋拷锟斤拷卸锟斤拷锟斤拷锟�
		if (deltaX < -MOVE_DIRECTION_DEAD_ZONE)
		{
			setLeft(true);
			setRight(false);
			if (d != dir::left)
			{
				changeFlip();
				d = dir::left;
			}
		}
		else if (deltaX > MOVE_DIRECTION_DEAD_ZONE)
		{
			setRight(true);
			setLeft(false);
			if (d != dir::right)
			{
				changeFlip();
				d = dir::right;
			}
		}
		else
		{
			setLeft(false);
			setRight(false);
		}

		// 锟斤拷锟斤拷锟狡讹拷锟斤拷志
		if (deltaY < -1e-3)
		{
			setUp(true);
			setDown(false);
		}
		else if (deltaY > 1e-3)
		{
			setDown(true);
			setUp(false);
		}
		else
		{
			setUp(false);
			setDown(false);
		}

		// 锟斤拷锟捷撅拷锟诫动态锟斤拷锟斤拷锟劫讹拷
		double speedFactor = 1.0;
		if (slowDown && deltaLength < slowDownDistance)
		{
			// 锟节硷拷锟劫撅拷锟斤拷锟节ｏ拷锟劫讹拷锟金渐斤拷锟斤拷
			speedFactor = (deltaLength - minDistance) / (slowDownDistance - minDistance);
		}

		// 锟斤拷态锟斤拷锟劫ｏ拷锟斤拷锟斤拷锟角撅拷锟斤拷锟斤拷锟斤拷
		if (currentSpeed < maxSpeed)
		{
			currentSpeed += acceleration;
			if (currentSpeed > maxSpeed)
				currentSpeed = maxSpeed;
		}

		// 应锟矫撅拷锟斤拷锟斤拷锟截碉拷实锟斤拷锟劫讹拷
		double actualSpeed = currentSpeed * speedFactor;

		if (deltaLength != 0 && actualSpeed > 0.0)
		{
			double nx = currentX + deltaX * actualSpeed * ZOOM_RATE / deltaLength;
			double ny = currentY + deltaY * actualSpeed * ZOOM_RATE / deltaLength;
			if (!collideObstacle((int)nx, (int)ny, collision * ZOOM_RATE, gObstacles))
			{
				currentX = nx;
				currentY = ny;
			}
		}
	}
	else
	{
		// 锟斤拷锟斤拷太锟斤拷锟斤拷停止锟狡讹拷锟斤拷锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷
		currentSpeed = 0.0;
		haveTarget = false;
		setUp(false);
		setDown(false);
		setLeft(false);
		setRight(false);
	}
}
void Character::setCurrentAnimation(int index)
{
	if (currentAnimation != index)
	{
		currentAnimation = index;
		animations[index].setCurrentFrame(0);
	}
}
int Character::getCurrentAnimation() const { return currentAnimation; }
void Character::changeFlip() { flip = !flip; }
double Character::getX() const { return currentX; }
double Character::getY() const { return currentY; }
int Character::getTx() const { return targetX; }
int Character::getTy() const { return targetY; }
int Character::getCenterX() const { return static_cast<int>(getX()); }
int Character::getCenterY() const { return static_cast<int>(getY() - getHeight() * 0.5); }
int Character::getCollision() const { return collision * ZOOM_RATE; } // 锟斤拷取锟斤拷撞锟斤拷夥段�
int Character::getHeight() const { return height * ZOOM_RATE; }
int Character::getAttackOffset() const { return attackOffset * ZOOM_RATE; }
int Character::getAttackRange() const { return attackRange * ZOOM_RATE; }
int Character::getAttackX() const
{
	return getCenterX() + ((getDir() == dir::right) - (getDir() == dir::left)) * getAttackOffset();
}
int Character::getAttackY() const { return getCenterY(); }
Character::dir Character::getDir() const { return d; }
void Character::setUp(bool isMUp) { isMoveUp = isMUp; }
void Character::setDown(bool isMDown) { isMoveDown = isMDown; }
void Character::setLeft(bool isMLeft) { isMoveLeft = isMLeft; }
void Character::setRight(bool isMRight) { isMoveRight = isMRight; }
void Character::stopMove()
{
	isMoveUp = false;
	isMoveDown = false;
	isMoveLeft = false;
	isMoveRight = false;
	currentSpeed = 0.0;
}
void Character::setDir(dir di) { d = di; }
void Character::setHeight(int h) { height = h; }
bool Character::isMoving() const { return isMoveUp || isMoveDown || isMoveLeft || isMoveRight; }
bool Character::haveT() const { return haveTarget; }
void Character::setTarget(int x, int y)
{
	targetX = x;
	targetY = y;
	haveTarget = true;
}
void Character::setMaxSpeed(double value) { maxSpeed = value; }
void Character::setAcceleration(double value) { acceleration = value; }
void Character::setHaveTarget(bool hasTarget) { haveTarget = hasTarget; }
void Character::setAttackOffset(int offset) { attackOffset = offset; }
void Character::setAttackRange(int range) { attackRange = range; }
bool Character::isAlive() { return hp > 0; }
void Character::hurt()
{
	// 受伤后进入短暂无敌时间
	if (canReduceHp)
	{
		canReduceHp = false;
		hp--;
	}
}
void Character::setHp(int value)
{
	fullHp = value;
	hp = value;
}
void Character::setCanAddHp(bool canAdd) { canAddHp = canAdd; }
void Character::addHp(int amount)
{
	if (hp < fullHp && canAddHp)
	{
		canAddHp = false;
		hp += amount;
	}
	if (hp >= fullHp)
		hp = fullHp;
}
bool Character::canHurt() const { return canReduceHp; }
double Character::getHpRate() const { return fullHp > 0 ? hp * 1.0 / fullHp : 0.0; }
