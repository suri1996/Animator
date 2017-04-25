#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"


#include <FL/gl.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{
	XPOS, YPOS, ZPOS, TURN, STATE, NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))


// To make a SampleModel, we inherit off of ModelerView

class SampleModel : public ModelerView
{
public:
	SampleModel(int x, int y, int w, int h, char *label)
		: ModelerView(x, y, w, h, label) { }

	virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{
	return new SampleModel(x, y, w, h, label);
}

static double leftArmOffset = 0;
static double leftArmIncrement = 0;
static double rightArmOffset = 0;
static double rightArmIncrement = 0;
static double headOffset = 0;

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
	const float AMBIENT_R = 0.1;
	const float AMBIENT_G = 0.1;
	const float AMBIENT_B = 0.1;

	const float FLOOR_TX = -5;
	const float FLOOR_TY = -3.5;
	const float FLOOR_TZ = -5;

	const double BOX_X = 10;
	const double BOX_Y = 0.01;
	const double BOX_Z = 10;

	const float BOTTOM_TX = 0;
	const float BOTTOM_TY = -1.8;
	const float BOTTOM_TZ = 0;
	const float BOTTOM_SPHERE_SIZE = 1.7;

	const float MIDDLE_TX = 0;
	const float MIDDLE_TY = 2.5;
	const float MIDDLE_TZ = 0;
	const float MIDDLE_SPHERE_SIZE = 1.3;

	const float TOP_TX = 0;
	const float TOP_TY = 1.8;
	const float TOP_TZ = 0;
	const float TOP_SPHERE_SIZE = 0.9;

	const float ARM_CYLINDER_HEIGHT = 1;
	const float ARM_CYLINDER_R = 0.05;

	const float LARM_TX = 1.1;
	const float RARM_TX = -1.1;
	const float ARM_TY = 0.4;
	const float ARM_TZ = 0;

	const float HAND_TX = 0;
	const float HAND_TY = 0;
	const float HAND_TZ = 1.05;
	const float HAND_SPHERE_SIZE = 0.24;

	const float LEYE_TX = 0.4;
	const float REYE_TX = -0.4;
	const float EYE_TY = 0.3;
	const float EYE_TZ = 0.8;
	const float EYE_SPHERE_SIZE = 0.1;

	const float NOSE_CYLINDER_HEIGHT = 1;
	const float NOSE_CYLINDER_R1 = 0.2;
	const float NOSE_CYLINDER_R2 = 0;

	const float NOSE_TX = 0;
	const float NOSE_TY = 0;
	const float NOSE_TZ = 0.8;

	if (ModelerApplication::Instance()->Animating()) {
		if (leftArmOffset <= -60) {
			leftArmIncrement = 1.1;
		}
		else if (leftArmOffset >= 0) {
			leftArmIncrement = -1.1;
		}

		if (rightArmOffset >= 60) {
			rightArmIncrement = -1.1;
		}
		else if (rightArmOffset <= 0) {
			rightArmIncrement = 1.1;
		}

		leftArmOffset += leftArmIncrement;
		rightArmOffset += rightArmIncrement;

		headOffset += 2.5;
		if (headOffset > 360) {
			headOffset -= 360;
		}
	}

	const double LARM_ANGLE = 20 + leftArmOffset;
	const double RARM_ANGLE = 160 + rightArmOffset;
	const double HEAD_ANGLE = 0 + headOffset;

	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();

	// set background color
	setAmbientColor(AMBIENT_R, AMBIENT_G, AMBIENT_B);

	// draw floor
	setDiffuseColor(COLOR_RED);
	glPushMatrix(); //open floor
	glTranslated(FLOOR_TX, FLOOR_TY, FLOOR_TZ);
	drawBox(BOX_X, BOX_Y, BOX_Z);
	glPopMatrix(); //close floor

				   // draw the sample model
	setDiffuseColor(1.0f, 1.0f, 1.0f);

	glPushMatrix(); //open model
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	glRotated(VAL(TURN), 0.0, 1.0, 0.0);

	if (!VAL(STATE)) {

		glPushMatrix(); //open bottom sphere
		glTranslated(BOTTOM_TX, BOTTOM_TY, BOTTOM_TZ);
		drawSphere(BOTTOM_SPHERE_SIZE);

		glPushMatrix(); //open middle sphere
		glTranslated(MIDDLE_TX, MIDDLE_TY, MIDDLE_TZ);
		drawSphere(MIDDLE_SPHERE_SIZE);

		glPushMatrix(); //open right arm

		setDiffuseColor(0.65f, 0.16f, 0.16f);

		glTranslated(RARM_TX, ARM_TY, ARM_TZ);
		glRotated(90, 0.0, 1.0, 0.0);
		glRotated(RARM_ANGLE, 1.0, 0.0, 0.0);
		drawCylinder(ARM_CYLINDER_HEIGHT, ARM_CYLINDER_R, ARM_CYLINDER_R);

		setDiffuseColor(1.0f, 1.0f, 1.0f);

		glPushMatrix();//open right hand
		glTranslated(HAND_TX, HAND_TY, HAND_TZ);
		drawSphere(HAND_SPHERE_SIZE);
		glPopMatrix(); //close right hand

		glPopMatrix(); //close right arm

		glPushMatrix(); //open left arm

		setDiffuseColor(0.65f, 0.16f, 0.16f);

		glTranslated(LARM_TX, ARM_TY, ARM_TZ);
		glRotated(90, 0.0, 1.0, 0.0);
		glRotated(LARM_ANGLE, 1.0, 0.0, 0.0);
		drawCylinder(ARM_CYLINDER_HEIGHT, ARM_CYLINDER_R, ARM_CYLINDER_R);

		setDiffuseColor(1.0f, 1.0f, 1.0f);

		glPushMatrix(); //open left hand
		glTranslated(HAND_TX, HAND_TY, HAND_TZ);
		drawSphere(HAND_SPHERE_SIZE);
		glPopMatrix(); //close left hand

		glPopMatrix(); //close left arm

		glPushMatrix(); // open top sphere
		glTranslated(TOP_TX, TOP_TY, TOP_TZ);
		glRotated(VAL(TURN), 0.0, 1.0, 0.0);
		glRotated(HEAD_ANGLE, 0.0, 1.0, 0.0);
		drawSphere(TOP_SPHERE_SIZE);

		setDiffuseColor(0.0f, 0.0f, 0.0f);

		glPushMatrix(); //open left eye
		glTranslated(LEYE_TX, EYE_TY, EYE_TZ);
		drawSphere(EYE_SPHERE_SIZE);
		glPopMatrix(); //close left eye

		glPushMatrix(); //open right eye
		glTranslated(REYE_TX, EYE_TY, EYE_TZ);
		drawSphere(EYE_SPHERE_SIZE);
		glPopMatrix(); //close right eye

		setDiffuseColor(1.0f, 0.0f, 0.0f);

		glPushMatrix(); //open nose
		glTranslated(NOSE_TX, NOSE_TY, NOSE_TZ);
		drawCylinder(NOSE_CYLINDER_HEIGHT, NOSE_CYLINDER_R1, NOSE_CYLINDER_R2);
		glPopMatrix(); //close nose

		glPopMatrix(); // close top sphere

		glPopMatrix(); //close middle sphere

		glPopMatrix(); //close bottom sphere
	}
	else {

		glPushMatrix(); //open bottom box
		glTranslated(-1.3, -3.3, -1.5);
		drawBox(2.6, 2.6, 2.6);

		glPushMatrix(); //open middle box
		glTranslated(0.25, 2.6, 0.25);
		drawBox(2.1, 2.1, 2.1);

		glPushMatrix(); //open right arm

		setDiffuseColor(0.65f, 0.16f, 0.16f);

		glTranslated(0, 1, 1);
		glRotated(90, 0.0, 1.0, 0.0);
		glRotated(160, 1.0, 0.0, 0.0);
		drawCylinder(ARM_CYLINDER_HEIGHT, ARM_CYLINDER_R, ARM_CYLINDER_R);

		setDiffuseColor(1.0f, 1.0f, 1.0f);

		glPushMatrix();//open right hand
		glTranslated(HAND_TX, HAND_TY, HAND_TZ);
		drawSphere(HAND_SPHERE_SIZE);
		glPopMatrix(); //close right hand

		glPopMatrix(); //close right arm

		glPushMatrix(); //open left arm

		setDiffuseColor(0.65f, 0.16f, 0.16f);

		glTranslated(2.1, 1, 1);
		glRotated(90, 0.0, 1.0, 0.0);
		glRotated(20, 1.0, 0.0, 0.0);
		drawCylinder(ARM_CYLINDER_HEIGHT, ARM_CYLINDER_R, ARM_CYLINDER_R);

		setDiffuseColor(1.0f, 1.0f, 1.0f);

		glPushMatrix(); //open left hand
		glTranslated(HAND_TX, HAND_TY, HAND_TZ);
		drawSphere(HAND_SPHERE_SIZE);
		glPopMatrix(); //close left hand

		glPopMatrix(); //close left arm

		glPushMatrix(); //open top box
		glTranslated(0.25, 2.1, 0.25);
		drawBox(1.6, 1.6, 1.6);

		setDiffuseColor(0.0f, 0.0f, 0.0f);

		glPushMatrix(); //open left eye
		glTranslated(1.2, 1.1, 1.6);
		drawSphere(EYE_SPHERE_SIZE);
		glPopMatrix(); //close left eye

		glPushMatrix(); //open right eye
		glTranslated(0.4, 1.1, 1.6);
		drawSphere(EYE_SPHERE_SIZE);
		glPopMatrix(); //close right eye

		setDiffuseColor(1.0f, 0.0f, 0.0f);

		glPushMatrix(); //open nose
		glTranslated(0.8, 0.7, 1.4);
		drawCylinder(NOSE_CYLINDER_HEIGHT, NOSE_CYLINDER_R1, NOSE_CYLINDER_R2);
		glPopMatrix(); //close nose

		glPopMatrix(); //close top box

		glPopMatrix(); //close middle box

		glPopMatrix();//close bottom box
	}

	glPopMatrix(); //close model
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	controls[TURN] = ModelerControl("Turn", -90, 90, 10, 0);
	controls[STATE] = ModelerControl("State", 0, 1, 1, 0);

	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}