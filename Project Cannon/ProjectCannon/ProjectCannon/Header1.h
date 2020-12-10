#pragma once
#include "pch.h"
#include <iostream>
#include <cmath>
#ifndef Target_H

#define PI 3.141592654
class Target {
public:
	int distance;
	double orientationY;
	double targetX;
	double targetY;
	int calibration;
	Target(int Distance, double servoOrientationY) {
		distance = Distance;
		orientationY = servoOrientationY;
	}

	void setDistance(int Distance);
	void setCalibration(int cal);
	void setOrientationY(double servoOrientationY);
	void mapTarget();
};

#pragma region TargetMemberFunctions



void Target::setDistance(int Distance) { //Sets the target distance
	distance = Distance;
}

void Target::setCalibration(int cal) { //The angle of the servo input might differ from the actual angel relative to the ground. If servo angle is 30 -> Might be 45 in reality.
	calibration = cal;
}

void Target::setOrientationY(double servoOrientationY) { //Takes the servoangle and sets the Target objects variables
	orientationY = servoOrientationY + calibration;
}

void Target::mapTarget() { //Finds the x and y coordinates of the target
	targetX = distance * cos(orientationY*(PI/180));
	targetY = distance * sin(orientationY*(PI/180));
}
#pragma endregion

class Projectile {
public:
	double projectileX;
	double projectileY;
	double vX;
	double vY;
	int angle;
	double time;
	double cannonLength = 0.096;
	int startVelocity = 30;
	Projectile(Target t) {
		angle = t.orientationY;
	}
	
	double dragAdjustment = 0; //Constant which makes up (approximately) for noise factors such as drag and friction when firing from a set distance 
	double positionAdjustmentX; 
	double positionAdjustmnetY;

	void setStartVelocity(int v); //Eliminating too many variables by setting up the same initial speed of a determined projectile with a certain measure of spring force
	void setDragAdjustment(double drag); //Distance shift between lidar location and projectile location
	void setPositionAdjustmentY(double posY, Target target);
	//void setPositionAdjustmentX(double posX, Target target);
	void setCannonLength(double l);
	void findProjectileY();
	void findT(Target t);
	void calcVx(Target target, int i);
	void calcVy(Target target, int i);
	
	 
};
#pragma region ProjectileMemberFunctions
void Projectile::setCannonLength(double l){
	cannonLength = l;
}

void Projectile::setStartVelocity(int v) {
	startVelocity = v;
}

void Projectile::calcVy(Target target, int i) { //For Parametric equation 
	vY = startVelocity*sin((angle + i)*PI / 180);
}
void Projectile::calcVx(Target target, int i) {
	vX = startVelocity*cos((angle + i)*PI/180);
}
/*void Projectile::setPositionAdjustmentX(double posX, Target target) { //posX is the horizontal location difference between the lidar and the projectile (when it leaves the barrel)
	projectileX = target.targetX + posX;
}

void Projectile::setPositionAdjustmentY(double posY, Target target) { //posy is the vertical ....
	projectileY = target.targetY + posY;
}
*/
void Projectile::setDragAdjustment(double drag) { 
	dragAdjustment = drag;
}
void Projectile::findProjectileY() {
	projectileY = vY * time - 9.81*time*time;
}

void Projectile::findT(Target t) {
	time = ((t.targetX - 0.019 + dragAdjustment) / vX);
}
#pragma endregion
#pragma region CalculationFunction
void angleTest(Target target, Projectile p, int i) {

	if (target.targetY <= p.projectileY && p.projectileY <= target.targetY + 0.2) {
		std::cout << std::endl;
		std::cout << "Launch at: " << p.angle + i << " degrees, hit at location y = " << p.projectileY << ", target is located at y = " << target.targetY;
	}
}

Projectile calculationOrder(Target target, Projectile p, int i) {
	//target.mapTarget();
	p.calcVx(target, i);
	p.calcVy(target, i);
	p.findT(target);
	p.findProjectileY();
	return p;
}

#pragma endregion
#endif