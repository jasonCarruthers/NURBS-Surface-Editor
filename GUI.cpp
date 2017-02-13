#include <GL/glut.h>
#include <string>
#include "Main.h"
#include "General.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Curves.h"
#include "GUI.h"
#include "Graph.h"
#include "Tree.h"




/*
 * Global variables
 */
MyRectangle *curveSpace;
MyRectangle *verticalPanel;
CheckboxHandler *checkboxHandler;
Checkbox *controlPointCheckbox;
Checkbox *controlMeshCheckbox;
Checkbox *surfaceMeshCheckbox;
Checkbox *gouraudShadingCheckbox;
SliderHandler *sliderHandler;
Slider *resolutionSlider;
Slider *orderKSlider;
Slider *orderLSlider;
Slider *weightSlider;
ButtonHandler *buttonHandler;
Button *insertRowButton;
Button *insertColumnButton;
Button *deleteRowButton;
Button *deleteColumnButton;
Button *increaseXButton;
Button *decreaseXButton;
Button *increaseYButton;
Button *decreaseYButton;
Button *increaseZButton;
Button *decreaseZButton;



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Button
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Button::Button(const std::string &newText, const MyRectangle &newRect, void *newActionFcn)
{
	mText = newText;
	mRect = new MyRectangle(newRect);
	mRect->SetDimensions(Vector2I(2*TEXT_OFFSET.GetX() + GetTextDimensions(mText).GetX(), 2*TEXT_OFFSET.GetY() + GetTextDimensions(mText).GetY()));
	mActionFcn = newActionFcn;

	mPressedState = Button::NotPressed;
	mHoveredState = Button::NotHovered;
}

/*
* Accessors
*/
Button::HoveredState Button::GetHoveredState() const
{
	return mHoveredState;
}

Button::PressedState Button::GetPressedState() const
{
	return mPressedState;
}

MyRectangle Button::GetRect() const
{
	return *mRect;
}

std::string Button::GetText() const
{
	return mText;
}

void Button::Draw() const
{
	/*
	* ----------------------------------------------------------------------------------------
	* Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	* NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	*		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	*		 corner of screen.
	* ----------------------------------------------------------------------------------------
	*/
	glColor3f(1.0f, 1.0f, 1.0f); /*Set text color*/
	/*Draw button name text*/
	Vector2F rasterPos = GetRasterPosition(Vector2I(mRect->GetTopLeft().GetX() + TEXT_OFFSET.GetX(), mRect->GetTopLeft().GetY() - TEXT_OFFSET.GetY()), mText.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < mText.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, mText[i]);

	/*Reposition view frustrum at the "normal" spot.
	NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	glutPostRedisplay();
	/*
	* ----------------------------------------------------------------------------------------
	* End drawing text
	* ----------------------------------------------------------------------------------------
	*/

	/*Draw rect*/
	if (mPressedState == NotPressed && mHoveredState == NotHovered)
		mRect->Draw();
	else if (mPressedState == Pressed && mHoveredState == NotHovered)
		mRect->Draw(Color3(mRect->GetColor().GetR() / 2.0f, mRect->GetColor().GetG() / 2.0f, mRect->GetColor().GetB() / 2.0f));
	else if (mPressedState == NotPressed && mHoveredState == Hovered)
		mRect->Draw(Color3(0.5f * mRect->GetColor().GetR() + 0.5f * HOVERED_COLOR.GetR(), 0.5f * mRect->GetColor().GetG() + 0.5f * HOVERED_COLOR.GetG(), 0.5f * mRect->GetColor().GetB() + 0.5f * HOVERED_COLOR.GetB()));
	else /*(mPressedState == Pressed && mHoveredState == Hovered)*/
		mRect->Draw(Color3(0.25f * mRect->GetColor().GetR() + 0.5f * HOVERED_COLOR.GetR(), 0.25f * mRect->GetColor().GetG() + 0.5f * HOVERED_COLOR.GetG(), 0.25f * mRect->GetColor().GetB() + 0.5f * HOVERED_COLOR.GetB()));
}

/*
* Mutators
*/
void Button::SetHoveredState(Button::HoveredState newHoveredState)
{
	/*Change the button's hovered state*/
	mHoveredState = newHoveredState;
}

void Button::SetPressedState(Button::PressedState newPressedState)
{
	/*If the button is being hovered over (so the mouse is over it) and is being pressed,
	  perform some action by calling the function pointed to by mActionFcn.*/
	if (mHoveredState == Button::Hovered &&
		mPressedState == Button::Pressed && newPressedState == Button::NotPressed)
		((void(*)())mActionFcn)();

	/*Change the button's pressed state*/
	mPressedState = newPressedState;
}

void Button::SetRect(const MyRectangle &newRect)
{
	mRect->Set(newRect);
}

void Button::SetText(const std::string &newText)
{
	mText = newText;
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class ButtonHandler
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
ButtonHandler::ButtonHandler()
{
}

/*
* Accessors
*/
std::vector<Button*>* ButtonHandler::GetButtonVec()
{
	return &mButtonVec;
}

void ButtonHandler::MonitorButtons(int mouseX, int mouseY, int button, int action) const
{
	for (unsigned int i = 0; i < mButtonVec.size(); i++)
	{
		/*Update based on mouse move...*/
		if (button == MAGIC_GARBAGE && action == MAGIC_GARBAGE)
		{
			/*Update hover state appropriately*/
			if (mButtonVec[i]->GetRect().ContainsPoint(mouseX, WINDOW_HEIGHT - mouseY))
				mButtonVec[i]->SetHoveredState(Button::Hovered);
			else if (mButtonVec[i]->GetHoveredState() == Button::Hovered)
				mButtonVec[i]->SetHoveredState(Button::NotHovered);

			/*If the button is pressed but is no longer being hovered over, then it should be
			  unpressed.*/
			else if (mButtonVec[i]->GetPressedState() == Button::Pressed &&
					 mButtonVec[i]->GetHoveredState() == Button::NotHovered)
				mButtonVec[i]->SetPressedState(Button::NotPressed);
		}

		/*Update based on mouse click...*/
		else
		{
			/*Update pressed state appropriately*/
			if (button == GLUT_LEFT_BUTTON && action == GLUT_UP &&
				mButtonVec[i]->GetHoveredState() == Button::Hovered &&
				mButtonVec[i]->GetPressedState() == Button::Pressed)
				mButtonVec[i]->SetPressedState(Button::NotPressed);
			else if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN &&
				mButtonVec[i]->GetHoveredState() == Button::Hovered)
				mButtonVec[i]->SetPressedState(Button::Pressed);
		}
	}
}

void ButtonHandler::DrawButtons() const
{
	for (unsigned int i = 0; i < mButtonVec.size(); i++)
		mButtonVec[i]->Draw();
}


/*
* Mutators
*/
void ButtonHandler::InsertButton(const Button &newButton)
{
	Button *temp = new Button(newButton);
	mButtonVec.push_back(temp);
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class RadioButtonGroup
* -------------------------------------------------------------------------------------------------------
*/
/*
* Initialization of static variables
*/
const Color3 RadioButtonGroup::EDGE_COLOR = Color3(1.0f, 0.0f, 0.0f);
const Color3 RadioButtonGroup::FILL_COLOR = Color3(1.0f, 1.0f, 1.0f);

/*
* Constructor(s)
*/
RadioButtonGroup::RadioButtonGroup()
{
	mRadioButtonInFocusIndex = MAGIC_GARBAGE;
}

/*
* Accessors
*/
std::pair<std::string, Vector2I*> RadioButtonGroup::GetRadioButtonInFocus() const
{
	/*Create a deep copy of the radio button to return.*/
	std::pair<std::string, Vector2I*> *retPair = new std::pair<std::string, Vector2I*>;
	retPair->first = mRadioButtonVec[mRadioButtonInFocusIndex].first;
	retPair->second = new Vector2I(mRadioButtonVec[mRadioButtonInFocusIndex].second->GetX(), mRadioButtonVec[mRadioButtonInFocusIndex].second->GetY());
	
	return *retPair;
}

unsigned int RadioButtonGroup::GetNumRadioButtons() const
{
	return mRadioButtonVec.size();
}

void RadioButtonGroup::Draw() const
{
	Circle edgeCircle;
	Circle fillCircle;

	/*
	* ----------------------------------------------------------------------------------------
	* Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	* NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	*		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	*		 corner of screen.
	* ----------------------------------------------------------------------------------------
	*/
	glColor3f(0.0f, 0.0f, 0.0f); /*Set text color*/
	Vector2F rasterPos;
	std::string str;
	/*Draw the names of each radio button.*/
	for (unsigned int i = 0; i < mRadioButtonVec.size(); i++)
	{
		str = mRadioButtonVec[i].first;

		/*Always draw the name below the radio button it belongs to.*/
		Vector2I textDimensions = GetTextDimensions(str);
		rasterPos = GetRasterPosition(Vector2I(mRadioButtonVec[i].second->GetX() - EDGE_RADIUS - textDimensions.GetX()/2, mRadioButtonVec[i].second->GetY() - EDGE_RADIUS - 5), str.size());
		glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
		for (unsigned int i = 0; i < str.size(); i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);
	}
	/*Reposition view frustrum at the "normal" spot.
	NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	glutPostRedisplay();
	/*
	* ----------------------------------------------------------------------------------------
	* End drawing text
	* ----------------------------------------------------------------------------------------
	*/

	/*Draw the edges of all of the radio buttons.*/
	for (unsigned int i = 0; i < mRadioButtonVec.size(); i++)
	{
		edgeCircle = Circle(*(mRadioButtonVec[i].second), EDGE_RADIUS, EDGE_COLOR);
		edgeCircle.Draw();
	}

	/*Draw the filled in portion of the radio button in focus.*/
	if (mRadioButtonInFocusIndex >= 0 && (unsigned int)mRadioButtonInFocusIndex < mRadioButtonVec.size())
	{
		fillCircle = Circle(*(mRadioButtonVec[mRadioButtonInFocusIndex].second), FILL_RADIUS, FILL_COLOR);
		fillCircle.Draw();
	}
}

/*
* Mutators
*/
void RadioButtonGroup::Insert(const std::string &newRadioButtonName, const Vector2I &newRadioButtonPosition, void *newAction)
{
	std::pair<std::string, Vector2I*> *newRadioButton = new std::pair<std::string, Vector2I*>;

	newRadioButton->first = newRadioButtonName;
	newRadioButton->second = new Vector2I(newRadioButtonPosition.GetX(), newRadioButtonPosition.GetY());

	mRadioButtonVec.push_back(*newRadioButton);
	mActionVec.push_back(newAction);
}

void RadioButtonGroup::SetRadioButtonInFocus(unsigned int vecIndex)
{
	if (vecIndex >= 0 && vecIndex < mRadioButtonVec.size() && mRadioButtonInFocusIndex != vecIndex)
		((void(*)())mActionVec[vecIndex])();
	
	mRadioButtonInFocusIndex = vecIndex;
}

/*
* Overloaded operators
*/
std::pair<std::string, Vector2I*> &RadioButtonGroup::operator[](int index)
{
	std::pair<std::string, Vector2I*> *defaultRetVal = new std::pair<std::string, Vector2I*>;
	defaultRetVal->first = "";
	defaultRetVal->second = new Vector2I(0, 0);

	if (index < 0 || index >= (int)mRadioButtonVec.size())
		return *defaultRetVal; /*If index is out of bounds, return some garbage value.*/
	return mRadioButtonVec[index];
}
const std::pair<std::string, Vector2I*> &RadioButtonGroup::operator[](int index) const
{
	std::pair<std::string, Vector2I*> *defaultRetVal = new std::pair<std::string, Vector2I*>;
	defaultRetVal->first = "";
	defaultRetVal->second = new Vector2I(0, 0);

	if (index < 0 || index >= (int)mRadioButtonVec.size())
		return *defaultRetVal; /*If index is out of bounds, return some garbage value.*/
	return mRadioButtonVec[index];
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class RadioButtonGroupHandler
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
RadioButtonGroupHandler::RadioButtonGroupHandler()
{
}

/*
* Accessors
*/
std::vector<RadioButtonGroup*>* RadioButtonGroupHandler::GetRadioButtonGroupVec()
{
	return &mRadioButtonGroupVec;
}

void RadioButtonGroupHandler::MonitorRadioButtonGroups(int mouseX, int mouseY, int button, int action) const
{
	/*Update based on mouse click...*/
	for (unsigned int group = 0; group < mRadioButtonGroupVec.size(); group++)
	{
		for (unsigned int radioButton = 0; radioButton < mRadioButtonGroupVec[group]->GetNumRadioButtons(); radioButton++)
		{
			Circle tempRadioButton = Circle(*((*(mRadioButtonGroupVec[group]))[radioButton].second), RadioButtonGroup::EDGE_RADIUS, RadioButtonGroup::EDGE_COLOR);

			/*Update pressed state appropriately.*/
			if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN &&
				tempRadioButton.ContainsPoint(Vector2I(mouseX, WINDOW_HEIGHT - mouseY)))
				mRadioButtonGroupVec[group]->SetRadioButtonInFocus(radioButton);
		}
	}
}

void RadioButtonGroupHandler::DrawRadioButtonGoups() const
{
	for (unsigned int i = 0; i < mRadioButtonGroupVec.size(); i++)
		mRadioButtonGroupVec[i]->Draw();
}


/*
* Mutators
*/
void RadioButtonGroupHandler::InsertRadioButtonGroup(const RadioButtonGroup &newRadioButtonGroup)
{
	RadioButtonGroup *temp = new RadioButtonGroup(newRadioButtonGroup);
	mRadioButtonGroupVec.push_back(temp);
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Checkbox
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Checkbox::Checkbox(const MyRectangle &newRect, void *newCheckedFcn, void *newUncheckedFcn)
{
	mRect = new MyRectangle(newRect);
	mCheckedFcn = newCheckedFcn;
	mUncheckedFcn = newUncheckedFcn;
	mHoveredState = Checkbox::NotHovered;
	mPressedState = Checkbox::NotPressed;
}

/*
* Accessors
*/
Checkbox::HoveredState Checkbox::GetHoveredState() const
{
	return mHoveredState;
}

Checkbox::PressedState Checkbox::GetPressedState() const
{
	return mPressedState;
}

MyRectangle Checkbox::GetRect() const
{
	return *mRect;
}


/*
* Mutators
*/
void Checkbox::SetHoveredState(Checkbox::HoveredState newHoveredState)
{
	mHoveredState = newHoveredState;

	/*If checkbox is being hovered over and it's not already pressed,
	  set it to Checkbox::HOVERED_COLOR.*/
	if (mPressedState == NotPressed && mHoveredState == Hovered)
		mRect->SetColor(Checkbox::HOVERED_COLOR);

	/*If checkbox is not being hovered over, set checkbox color to Checkbox::NORMAL_COLOR.*/
	else if (mPressedState == NotPressed && mHoveredState == NotHovered)
		mRect->SetColor(Checkbox::NORMAL_COLOR);

	/*If checkbox is in its pressed state and is being hovered over, set its
	  color to Checkbox::HOVERED_AND_PRESSED_COLOR*/
	else if (mPressedState == Pressed && mHoveredState == Hovered)
		mRect->SetColor(Checkbox::HOVERED_AND_PRESSED_COLOR);

	/*If checkbox is in its pressed state and is not being hovered over, set its
	  color to Checkbox::PRESSED_COLOR*/
	else if (mPressedState == Pressed && mHoveredState == NotHovered)
		mRect->SetColor(Checkbox::PRESSED_COLOR);
}

void Checkbox::SetPressedState(Checkbox::PressedState newPressedState)
{
	mPressedState = newPressedState;

	/*If checkbox is being pressed, set checkbox color to Checkbox::PRESSED_COLOR and
	  also call this checkbox's callback function for checkbox becoming checked.*/
	if (mPressedState == Pressed)
	{
		mRect->SetColor(Checkbox::PRESSED_COLOR);
		((void(*)())mCheckedFcn)();
	}

	/*If checkbox is not being pressed, set checkbox color to Checkbox::HOVERED_COLOR since the checkbox
	  must still be hovered over because the user has to mouse hover in order to toggle
	  the checkbox to mPressedState=NotPressed. Also, call this checkbox's callback function
	  for checkbox becoming unchecked.*/
	else if (mPressedState == NotPressed)
	{
		mRect->SetColor(Checkbox::HOVERED_COLOR);
		((void(*)())mUncheckedFcn)();
	}
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class CheckboxHandler
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
CheckboxHandler::CheckboxHandler()
{
}

/*
* Accessors
*/
std::vector<Checkbox*>* CheckboxHandler::GetCheckboxVec()
{
	return &mCheckboxVec;
}

void CheckboxHandler::MonitorCheckboxes(int mouseX, int mouseY, int button, int action) const
{
	for (unsigned int i = 0; i < mCheckboxVec.size(); i++)
	{
		/*Update based on mouse move...*/
		if (button == MAGIC_GARBAGE && action == MAGIC_GARBAGE)
		{
			/*Update hover state appropriately*/
			if (mCheckboxVec[i]->GetRect().ContainsPoint(mouseX, WINDOW_HEIGHT - mouseY))
				mCheckboxVec[i]->SetHoveredState(Checkbox::Hovered);
			else if (mCheckboxVec[i]->GetHoveredState() == Checkbox::Hovered)
				mCheckboxVec[i]->SetHoveredState(Checkbox::NotHovered);
		}

		/*Update based on mouse click...*/
		else
		{
			/*Update pressed state appropriately*/
			if (button == GLUT_LEFT_BUTTON && action == GLUT_UP &&
				mCheckboxVec[i]->GetHoveredState() == Checkbox::Hovered &&
				mCheckboxVec[i]->GetPressedState() == Checkbox::Pressed)
				mCheckboxVec[i]->SetPressedState(Checkbox::NotPressed);
			else if (button == GLUT_LEFT_BUTTON && action == GLUT_UP &&
				mCheckboxVec[i]->GetHoveredState() == Checkbox::Hovered)
				mCheckboxVec[i]->SetPressedState(Checkbox::Pressed);
		}
	}
}

void CheckboxHandler::DrawCheckboxes() const
{
	for (unsigned int i = 0; i < mCheckboxVec.size(); i++)
	{
		mCheckboxVec[i]->GetRect().Draw();
	}
}

/*
* Mutators
*/
void CheckboxHandler::InsertCheckbox(const Checkbox &newCheckbox)
{
	Checkbox *temp = new Checkbox(newCheckbox);
	mCheckboxVec.push_back(temp);
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Slider
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Slider::Slider(const std::string &newName, MyRectangle &newRect, const Vector2F &newRange,
				const Vector2I &newSliderPos, void *newSliderMoveFcn)
{
	/*Set name*/
	mName = newName;

	/*Set bounding rect*/
	mRect.Set(newRect);
	if (mRect.GetDimensions().GetX() == mRect.GetDimensions().GetY())
		mRect.SetDimensions(mRect.GetDimensions().GetX(), mRect.GetDimensions().GetY() + 1);
	
	/*Set orientation*/
	mSliderOrientation = (mRect.GetDimensions().GetX() < mRect.GetDimensions().GetY()) ? Slider::Vertical : Slider::Horizontal;

	/*Set range*/
	mRange.SetX(newRange.GetX());
	mRange.SetY(newRange.GetY());

	/*Set slider knob position*/
	mSlider.SetCenter(newSliderPos);
	mSlider.Set(MyRectangle(newSliderPos, Vector2I(40, 40), Color3(0.0f, 0.0f, 0.0f)));

	/*Set track rect*/
	Vector2I trackDimensions = (mSliderOrientation == Slider::Vertical) ?
								Vector2I(4, mRect.GetDimensions().GetY()) :
								Vector2I(mRect.GetDimensions().GetX(), 4);
	mSliderTrack.SetDimensions(trackDimensions);
	mSliderTrack.SetCenter(mRect.GetCenter());
	mSliderTrack.SetColor(0.0f, 0.0f, 0.0f);

	/*Set slider move function (what action to take when the slider knob moves)*/
	mSliderMoveFcn = newSliderMoveFcn;

	/*Set pressed state to false*/
	mIsPressed = false;
}

/*
* Accessors
*/
std::string Slider::GetName() const
{
	return mName;
}

MyRectangle Slider::GetRect() const
{
	return mRect;
}

Vector2F Slider::GetRange() const
{
	return mRange;
}

Slider::Orientation Slider::GetSliderOrientation() const
{
	return mSliderOrientation;
}

Vector2I Slider::GetSliderPosition() const
{
	return mSlider.GetCenter();
}

/*Gets value based on slider position and is relative to mRange*/
float Slider::GetValue() const
{
	float retVal = mRange.GetX();
	retVal += ((mSliderOrientation == Slider::Vertical) ?
		(float)(mSlider.GetCenter().GetY() - (mRect.GetTopLeft().GetY() - mRect.GetDimensions().GetY())) / mRect.GetDimensions().GetY() :
		(float)(mSlider.GetCenter().GetX() - mRect.GetTopLeft().GetX()) / mRect.GetDimensions().GetX()) *
		(mRange.GetY() - mRange.GetX());

	return retVal;
}

bool Slider::GetPressedState() const
{
	return mIsPressed;
}

void Slider::CallSliderMoveFcn(float t) const
{
	((void(*)(float))mSliderMoveFcn)(t);
}

void Slider::Draw() const
{
	/*
	* ----------------------------------------------------------------------------------------
	* Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	* NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	*		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	*		 corner of screen.
	* ----------------------------------------------------------------------------------------
	*/
	glColor3f(1.0f, 1.0f, 1.0f); /*Set text color*/
	Vector2F rasterPos;
	std::string str;
	/*Draw slider name text*/
	str = mName;
	rasterPos = GetRasterPosition(Vector2I(mRect.GetTopLeft().GetX() + mRect.GetDimensions().GetX() / 2 - 30, mRect.GetTopLeft().GetY() + 50), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Draw end of range text*/
	str = FloatToString(mRange.GetY());
	rasterPos = GetRasterPosition(Vector2I(mRect.GetTopLeft().GetX() + mRect.GetDimensions().GetX() / 2 - 5, mRect.GetTopLeft().GetY() + 25), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Draw beginning of range text*/
	str = FloatToString(mRange.GetX());
	rasterPos = GetRasterPosition(Vector2I(mRect.GetTopLeft().GetX() + mRect.GetDimensions().GetX() / 2 - 5, mRect.GetTopLeft().GetY() - mRect.GetDimensions().GetY() - 25), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Draw t value text*/
	str = FloatToString(GetValue());
	rasterPos = GetRasterPosition(Vector2I(mSlider.GetCenter().GetX() - 10, mSlider.GetCenter().GetY()), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Reposition view frustrum at the "normal" spot.
	NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	glutPostRedisplay();
	/*
	* ----------------------------------------------------------------------------------------
	* End drawing text
	* ----------------------------------------------------------------------------------------
	*/

	/*Draw the rest of the slider components*/
	mRect.Draw();
	mSliderTrack.Draw();
	mSlider.Draw();
}

/*
* Mutators
*/
void Slider::SetName(const std::string &newName)
{
	mName = newName;
}

void Slider::SetRect(const MyRectangle &newRect)
{
	mRect.Set(newRect);
}

void Slider::SetRange(const Vector2F &newRange)
{
	mRange.SetX(newRange.GetX());
	mRange.SetY(newRange.GetY());
}

void Slider::SetSliderPosition(const Vector2I &newSliderPos)
{
	mSlider.SetCenter(newSliderPos);
}

void Slider::SetPressedState(bool newPressedState)
{
	mIsPressed = newPressedState;
}

void Slider::SetSliderMoveFcn(void *newSliderMoveFcn)
{
	mSliderMoveFcn = newSliderMoveFcn;
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class SliderHandler
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
SliderHandler::SliderHandler()
{
}

/*
* Accessors
*/
std::vector<Slider*> &SliderHandler::GetSliderVec()
{
	return mSliderVec;
}

void SliderHandler::MonitorSliders(int mouseX, int mouseY, int button, int action) const
{
	/*The mouseY is relative to the top left corner of the screen, so first make it
	  relative to the bottom left corner of the screen*/
	mouseY = WINDOW_HEIGHT - mouseY;

	for (unsigned int i = 0; i < mSliderVec.size(); i++)
	{
		/*Update based on mouse move...*/
		if (button == MAGIC_GARBAGE && action == MAGIC_GARBAGE)
		{
			/*If this slider is being pressed, move the slider knob based to the 
			  x or y component of the mouse (bounded by the slider's bounding rect);
			  the chosen component is based on this slider's orientation.*/
			if (mSliderVec[i]->GetRect().ContainsPoint(mouseX, mouseY) &&
				mSliderVec[i]->GetPressedState() == true)
			{
				/*Get slider-mouse position delta*/
				int deltaX = (mSliderVec[i]->GetSliderOrientation() == Slider::Vertical) ?
								0 : mouseX - mSliderVec[i]->GetRect().GetCenter().GetX();
				int deltaY = (mSliderVec[i]->GetSliderOrientation() == Slider::Vertical) ?
					mouseY - mSliderVec[i]->GetRect().GetCenter().GetY() : 0;
				
				/*Bound the slider knob to stay within the slider bounding rect*/
				if (mSliderVec[i]->GetRect().GetCenter().GetX() + deltaX < mSliderVec[i]->GetRect().GetTopLeft().GetX())
					deltaX = mSliderVec[i]->GetRect().GetCenter().GetX() - mSliderVec[i]->GetRect().GetTopLeft().GetX();
				else if (mSliderVec[i]->GetRect().GetCenter().GetX() + deltaX > mSliderVec[i]->GetRect().GetTopLeft().GetX() + mSliderVec[i]->GetRect().GetDimensions().GetX())
					deltaX = (mSliderVec[i]->GetRect().GetTopLeft().GetX() + mSliderVec[i]->GetRect().GetDimensions().GetX()) - mSliderVec[i]->GetRect().GetCenter().GetX();
				else if (mSliderVec[i]->GetRect().GetCenter().GetY() + deltaY > mSliderVec[i]->GetRect().GetTopLeft().GetY())
					deltaY = mSliderVec[i]->GetRect().GetTopLeft().GetY() - mSliderVec[i]->GetRect().GetCenter().GetY();
				else if (mSliderVec[i]->GetRect().GetCenter().GetY() + deltaY < mSliderVec[i]->GetRect().GetTopLeft().GetY() - mSliderVec[i]->GetRect().GetDimensions().GetY())
					deltaY = mSliderVec[i]->GetRect().GetCenter().GetY() - (mSliderVec[i]->GetRect().GetTopLeft().GetY() - mSliderVec[i]->GetRect().GetDimensions().GetY());

				/*Change slider knob position*/
				Vector2I newSliderKnobPos = Vector2I(mSliderVec[i]->GetRect().GetCenter().GetX() + deltaX, mSliderVec[i]->GetRect().GetCenter().GetY() + deltaY);
				mSliderVec[i]->SetSliderPosition(newSliderKnobPos);

				/*Call callback function for when the slider knob moves*/
				mSliderVec[i]->CallSliderMoveFcn(mSliderVec[i]->GetValue());
			}
		}

		/*Update based on mouse click...*/
		else
		{
			/*As long as the mouse is over this slider's knob and the left mouse button is being
			  pressed, then this slider is in its mIsPressed=true state*/
			if (mSliderVec[i]->GetRect().ContainsPoint(mouseX, mouseY) &&
				button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
				mSliderVec[i]->SetPressedState(true);
			
			/*Otherwise, this slider is in its mIsPressed=false state*/
			else
				mSliderVec[i]->SetPressedState(true);
		}
	}
}

void SliderHandler::DrawSliders() const
{
	for (unsigned int i = 0; i < mSliderVec.size(); i++)
		mSliderVec[i]->Draw();
}

/*
* Mutators
*/
void SliderHandler::InsertSlider(const Slider &newSlider)
{
	Slider *temp = new Slider(newSlider);
	mSliderVec.push_back(temp);
}




/*
 * Global functions
 */
void InitGUI()
{
	/*Init panels*/
	verticalPanel = new MyRectangle(Vector2I(0, WINDOW_HEIGHT), Vector2I(350, WINDOW_HEIGHT),
									Color3(0.4f, 0.4f, 0.4f));

	/*Init curve space*/
	//curveSpace = new MyRectangle(Vector2I(verticalPanel->GetWidth(), WINDOW_HEIGHT), Vector2I(WINDOW_WIDTH - verticalPanel->GetWidth(), WINDOW_HEIGHT), Color3(0.0f, 0.0f, 0.0f));
	curveSpace = new MyRectangle(Vector2I(0, 0), Vector2I(WINDOW_WIDTH, WINDOW_HEIGHT), Color3(0.0f, 0.0f, 0.0f));

	/*Init checkboxes*/
	controlPointCheckbox = new Checkbox(MyRectangle(Vector2I(20, 120), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), ControlPointVisibilityEnabled, ControlPointVisibilityDisabled);
	controlMeshCheckbox = new Checkbox(MyRectangle(Vector2I(20, 90), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), ControlMeshVisibiliytEnabled, ControlMeshVisibiliytDisabled);
	surfaceMeshCheckbox = new Checkbox(MyRectangle(Vector2I(20, 60), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), SurfaceMeshVisibilityEnabled, SurfaceMeshVisibilityDisabled);
	gouraudShadingCheckbox = new Checkbox(MyRectangle(Vector2I(20, 30), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), GouraudShadingVisibilityEnabled, GouraudShadingVisibilityDisabled);

	/*Init checkbox handler*/
	checkboxHandler = new CheckboxHandler();
	checkboxHandler->InsertCheckbox(*controlPointCheckbox);
	checkboxHandler->InsertCheckbox(*controlMeshCheckbox);
	checkboxHandler->InsertCheckbox(*surfaceMeshCheckbox);
	checkboxHandler->InsertCheckbox(*gouraudShadingCheckbox);

	/*Init sliders*/
	resolutionSlider = new Slider("Resolution", MyRectangle(Vector2I(20, WINDOW_HEIGHT - 75), Vector2I(60, 250), Color3(0.3f, 0.5f, 1.0f)), Vector2F(40.0f, 400.0f));
	resolutionSlider->SetSliderPosition(resolutionSlider->GetRect().GetCenter());
	resolutionSlider->SetSliderMoveFcn(SetResolution);

	orderKSlider = new Slider("Order K", MyRectangle(Vector2I(100, WINDOW_HEIGHT - 75), Vector2I(60, 250), Color3(0.3f, 0.5f, 1.0f)), Vector2F((float)MIN_ORDER, (float)MIN_ORDER));
	orderKSlider->SetSliderPosition(orderKSlider->GetRect().GetCenter());
	orderKSlider->SetSliderMoveFcn(SetOrderK);

	orderLSlider = new Slider("Order L", MyRectangle(Vector2I(180, WINDOW_HEIGHT - 75), Vector2I(60, 250), Color3(0.3f, 0.5f, 1.0f)), Vector2F((float)MIN_ORDER, (float)MIN_ORDER));
	orderLSlider->SetSliderPosition(orderLSlider->GetRect().GetCenter());
	orderLSlider->SetSliderMoveFcn(SetOrderL);

	weightSlider = new Slider("Weight", MyRectangle(Vector2I(260, WINDOW_HEIGHT - 75), Vector2I(60, 250), Color3(0.3f, 0.5f, 1.0f)), Vector2F(1.0f, 5.0f));
	weightSlider->SetSliderPosition(weightSlider->GetRect().GetCenter());
	weightSlider->SetSliderMoveFcn(SetWeight);

	/*Init slider handler*/
	sliderHandler = new SliderHandler();
	sliderHandler->InsertSlider(*resolutionSlider);
	sliderHandler->InsertSlider(*orderKSlider);
	sliderHandler->InsertSlider(*orderLSlider);
	sliderHandler->InsertSlider(*weightSlider);

	/*Init buttons*/
	insertRowButton = new Button("Insert Row", MyRectangle(Vector2I(20, 220), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), InsertRow);
	insertColumnButton = new Button("Insert Column", MyRectangle(Vector2I(20, 180), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), InsertColumn);
	deleteRowButton = new Button("Delete Row", MyRectangle(Vector2I(160, 220), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), DeleteRow);
	deleteColumnButton = new Button("Delete Column", MyRectangle(Vector2I(160, 180), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), DeleteColumn);
	increaseXButton = new Button("x:+20", MyRectangle(Vector2I(170, 125), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), IncreaseX);
	decreaseXButton = new Button("x:-20", MyRectangle(Vector2I(250, 125), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), DecreaseX);
	increaseYButton = new Button("y:+20", MyRectangle(Vector2I(170, 80), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), IncreaseY);
	decreaseYButton = new Button("y:-20", MyRectangle(Vector2I(250, 80), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), DecreaseY);
	increaseZButton = new Button("z:+20", MyRectangle(Vector2I(170, 35), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), IncreaseZ);
	decreaseZButton = new Button("z:-20", MyRectangle(Vector2I(250, 35), Vector2I(0, 0), Color3(0.1f, 0.1f, 0.8f)), DecreaseZ);

	/*Init button handler*/
	buttonHandler = new ButtonHandler();
	buttonHandler->InsertButton(*insertRowButton);
	buttonHandler->InsertButton(*insertColumnButton);
	buttonHandler->InsertButton(*deleteRowButton);
	buttonHandler->InsertButton(*deleteColumnButton);
	buttonHandler->InsertButton(*increaseXButton);
	buttonHandler->InsertButton(*decreaseXButton);
	buttonHandler->InsertButton(*increaseYButton);
	buttonHandler->InsertButton(*decreaseYButton);
	buttonHandler->InsertButton(*increaseZButton);
	buttonHandler->InsertButton(*decreaseZButton);
}

void DrawGUI()
{
	/*
	 * ----------------------------------------------------------------------------------------
	 * Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	 * NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	 *		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	 *		 corner of screen.
	 * ----------------------------------------------------------------------------------------
	 */
	glColor3f(0.0, 1.0, 0.0); /*Set text color*/

	Vector2F rasterPos;
	std::string str;
	/*
	* controlPointCheckbox
	*/
	str = "Control Points";
	rasterPos = GetRasterPosition(Vector2I(50, 110), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*
	* controlMeshCheckbox
	*/
	str = "Control Mesh";
	rasterPos = GetRasterPosition(Vector2I(50, 80), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*
	 * surfaceMeshCheckbox
	 */
	str = "Surface Mesh";
	rasterPos = GetRasterPosition(Vector2I(50, 50), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*
	* gouraudShadingCheckbox
	*/
	str = "Gouraud Shading";
	rasterPos = GetRasterPosition(Vector2I(50, 20), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());	
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Reposition view frustrum at the "normal" spot.
	  NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	/*
	 * ----------------------------------------------------------------------------------------
	 * End drawing text
	 * ----------------------------------------------------------------------------------------
	 */


	/*Draw vertical panel(s)*/
	verticalPanel->Draw();

	/*Draw checkbox(es) monitored by the checkboxHandler*/
	checkboxHandler->DrawCheckboxes();
	
	/*Draw slider(s) monitored by the sliderHandler*/
	sliderHandler->DrawSliders();

	/*Draw button(s) monitored by the buttonHandler*/
	buttonHandler->DrawButtons();
}

/*Takes a desired position and converts it to the actual position where the
 *text should be drawn; text is drawn relative to its center, and with varying
 *string lengths this can be annoying.
 *
 *NOTE: Text is positioned with the glRasterPos(...) function, so the transformation
 *		from desiredPos to actualPos vector will take that into consideration.
 *
 *NOTE: Position vectors are relative to the bottom left corner of
 *		the screen, with positive y up. Also, the desiredPos position vector
 *		is relative to the left edge of the text, centered vertically. Since
 *		OpenGL draws text relative to the center of the string, this function
 *		transforms what the desired vector position of the left-center edge of
 *		text is into what the actual center-center vector position of the text
 *		should be.
 */
Vector2F GetRasterPosition(const Vector2I &desiredPos, int strLen)
{
	const int CHAR_WIDTH = 12; /*Assumes GLUT_BITMAP_TIMES_ROMAN_10 font*/
	const int CHAR_HEIGHT = 7; /*Assumes GLUT_BITMAP_TIMES_ROMAN_10 font*/

	Vector2F retVal;
	retVal = Vector2F(-1.0f + (1.0f / (WINDOW_WIDTH/2)) * (desiredPos.GetX()/* + CHAR_WIDTH * strLen/2.0f*/),
				      -1.0f + (1.0f / (WINDOW_HEIGHT/2)) * (desiredPos.GetY() - CHAR_HEIGHT/2.0f));
	return retVal;
}

Vector2I GetTextDimensions(const std::string &text)
{
	const int CHAR_WIDTH = 5; /*Assumes GLUT_BITMAP_TIMES_ROMAN_10 font*/
	const int CHAR_HEIGHT = 7; /*Assumes GLUT_BITMAP_TIMES_ROMAN_10 font*/

	unsigned int maxWidth = 0;
	unsigned int height = 1;
	int searchVal = 0;
	do
	{
		int temp = text.find('\n', searchVal);
		int delta = temp - searchVal;
		if (delta > 0)
		{
			height++;
			if ((unsigned int)delta > maxWidth)
				maxWidth = delta;
		}
		else if (delta == -1 && height == 1)
			maxWidth = text.size();
		searchVal = (temp == -1) ? -1 : searchVal + delta;
	} while (searchVal != -1);

	Vector2I *retVal = new Vector2I(maxWidth * CHAR_WIDTH, height * CHAR_HEIGHT);
	return *retVal;
}

/*Sets control points visible for all surfaces.*/
void ControlPointVisibilityEnabled()
{
	controlPointsVisible = true;
	nurbsSurface->SetControlPointVisibility(true);
	nurbsSurface->ClearCalcDraw();
}

/*Sets control points NOT visible for all surfaces.*/
void ControlPointVisibilityDisabled()
{
	controlPointsVisible = false;
	nurbsSurface->SetControlPointVisibility(false);
	nurbsSurface->ClearCalcDraw();
}

/*Sets control mesh visible for all surfaces.*/
void ControlMeshVisibiliytEnabled()
{
	controlMeshVisible = true;
	nurbsSurface->SetControlMeshVisibility(true);
	nurbsSurface->ClearCalcDraw();
}

/*Sets control mesh NOT visible for all surfaces.*/
void ControlMeshVisibiliytDisabled()
{
	controlMeshVisible = false;
	nurbsSurface->SetControlMeshVisibility(false);
	nurbsSurface->ClearCalcDraw();
}

/*Sets surface mesh visible for all surfaces.*/
void SurfaceMeshVisibilityEnabled()
{
	surfaceMeshVisible = true;
	nurbsSurface->SetSurfaceMeshVisibility(true);
	nurbsSurface->ClearCalcDraw();
}

/*Sets surface mesh NOT visible for all surfaces.*/
void SurfaceMeshVisibilityDisabled()
{
	surfaceMeshVisible = false;
	nurbsSurface->SetSurfaceMeshVisibility(false);
	nurbsSurface->ClearCalcDraw();
}

/*Sets Gouraud shading visible for all surfaces.*/
void GouraudShadingVisibilityEnabled()
{
	gouraudShadingVisible = true;
	nurbsSurface->SetGouraudShadingVisibility(true);
	nurbsSurface->ClearCalcDraw();
}

/*Sets Gouraud shading NOT visible for all surfaces.*/
void GouraudShadingVisibilityDisabled()
{
	gouraudShadingVisible = false;
	nurbsSurface->SetGouraudShadingVisibility(false);
	nurbsSurface->ClearCalcDraw();
}

/*Inserts a new row into the NURBS surface in focus.*/
void InsertRow()
{
	nurbsSurface->InsertSegment(NURBSSurface::Row, nurbsSurface->GetNumRows());
	nurbsSurface->SetProjectedControlPoints();

	/*Set weights of control points in new row to one.*/
	std::vector<std::vector<float>> *weightVec = &(nurbsSurface->GetWeightVec());
	std::vector<float> *newWeightRow = new std::vector<float>;
	weightVec->push_back(*newWeightRow);
	for (unsigned int column = 0; column < weightVec[0].size(); column++)
		(*weightVec)[weightVec->size() - 1].push_back(1.0f);

	/*Set V knots.*/
	nurbsSurface->CalcVKnots();

	nurbsSurface->ClearCalcDraw();

	/*Update the upper bound of the orderL slider.*/
	std::vector<Slider*> *sH = &(sliderHandler->GetSliderVec());
	(*sH)[2]->SetRange(Vector2F((float)MIN_ORDER, (float)nurbsSurface->GetNumRows() + 1));
}

/*Inserts a new column into the NURBS surface in focus.*/
void InsertColumn()
{
	nurbsSurface->InsertSegment(NURBSSurface::Column, nurbsSurface->GetNumColumns());
	nurbsSurface->SetProjectedControlPoints();

	/*Set weights of control points in new column to one.*/
	std::vector<std::vector<float>> *weightVec = &(nurbsSurface->GetWeightVec());
	for (unsigned int row = 0; row < weightVec->size(); row++)
		(*weightVec)[row].push_back(1.0f);

	/*Set U knots.*/
	nurbsSurface->CalcUKnots();

	nurbsSurface->ClearCalcDraw();

	/*Update the upper bound of the orderK slider.*/
	std::vector<Slider*> *sH = &(sliderHandler->GetSliderVec());
	(*sH)[1]->SetRange(Vector2F((float)MIN_ORDER, (float)nurbsSurface->GetNumColumns() + 1));	
}

/*Deletes a row from the NURBS surface in focus.*/
void DeleteRow()
{
	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	nurbsSurface->DeleteSegment(NURBSSurface::Row, nurbsSurface->GetNumRows() - 1);
	nurbsSurface->SetProjectedControlPoints();
	orderLSlider->SetRange(Vector2F((float)MIN_ORDER, (float)nurbsSurface->GetNumRows() + 1));
	nurbsSurface->ClearCalcDraw();
}

/*Deletes a column from the NURBS surface in focus.*/
void DeleteColumn()
{
	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	nurbsSurface->DeleteSegment(NURBSSurface::Column, nurbsSurface->GetNumColumns() - 1);
	nurbsSurface->SetProjectedControlPoints();
	orderKSlider->SetRange(Vector2F((float)MIN_ORDER, (float)nurbsSurface->GetNumColumns() + 1));
	nurbsSurface->ClearCalcDraw();
}

/*Increase x component of the control point in focus.*/
void IncreaseX()
{
	/*If no control point is in focus, return.*/
	if (rowOfCPInFocus == MAGIC_GARBAGE || colOfCPInFocus == MAGIC_GARBAGE)
		return;

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	/*Increase the x component of the control point in focus.*/
	std::vector<std::vector<Vector3F>> *cpVec = &(nurbsSurface->GetControlPointVec());
	(*cpVec)[rowOfCPInFocus][colOfCPInFocus].SetX((*cpVec)[rowOfCPInFocus][colOfCPInFocus].GetX() + 20.0f);

	/*Recalculate the projected control points.*/
	nurbsSurface->SetProjectedControlPoints();

	/*Redraw the curve.*/
	nurbsSurface->ClearCalcDraw();
}

/*Decrease x component of the control point in focus.*/
void DecreaseX()
{
	/*If no control point is in focus, return.*/
	if (rowOfCPInFocus == MAGIC_GARBAGE || colOfCPInFocus == MAGIC_GARBAGE)
		return;

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	/*Decrease the x component of the control point in focus.*/
	std::vector<std::vector<Vector3F>> *cpVec = &(nurbsSurface->GetControlPointVec());
	(*cpVec)[rowOfCPInFocus][colOfCPInFocus].SetX((*cpVec)[rowOfCPInFocus][colOfCPInFocus].GetX() - 20.0f);

	/*Recalculate the projected control points.*/
	nurbsSurface->SetProjectedControlPoints();

	/*Redraw the curve.*/
	nurbsSurface->ClearCalcDraw();
}

/*Increase y component of the control point in focus.*/
void IncreaseY()
{
	/*If no control point is in focus, return.*/
	if (rowOfCPInFocus == MAGIC_GARBAGE || colOfCPInFocus == MAGIC_GARBAGE)
		return;

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	/*Increase the y component of the control point in focus.*/
	std::vector<std::vector<Vector3F>> *cpVec = &(nurbsSurface->GetControlPointVec());
	(*cpVec)[rowOfCPInFocus][colOfCPInFocus].SetY((*cpVec)[rowOfCPInFocus][colOfCPInFocus].GetY() + 20.0f);

	/*Recalculate the projected control points.*/
	nurbsSurface->SetProjectedControlPoints();

	/*Redraw the curve.*/
	nurbsSurface->ClearCalcDraw();
}

/*Decrease y component of the control point in focus.*/
void DecreaseY()
{
	/*If no control point is in focus, return.*/
	if (rowOfCPInFocus == MAGIC_GARBAGE || colOfCPInFocus == MAGIC_GARBAGE)
		return;

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	/*Decrease the y component of the control point in focus.*/
	std::vector<std::vector<Vector3F>> *cpVec = &(nurbsSurface->GetControlPointVec());
	(*cpVec)[rowOfCPInFocus][colOfCPInFocus].SetY((*cpVec)[rowOfCPInFocus][colOfCPInFocus].GetY() - 20.0f);

	/*Recalculate the projected control points.*/
	nurbsSurface->SetProjectedControlPoints();

	/*Redraw the curve.*/
	nurbsSurface->ClearCalcDraw();
}

/*Incease z component of the control point in focus.*/
void IncreaseZ()
{
	/*If no control point is in focus, return.*/
	if (rowOfCPInFocus == MAGIC_GARBAGE || colOfCPInFocus == MAGIC_GARBAGE)
		return;

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	/*Increase the z component of the control point in focus.*/
	std::vector<std::vector<Vector3F>> *cpVec = &(nurbsSurface->GetControlPointVec());
	(*cpVec)[rowOfCPInFocus][colOfCPInFocus].SetZ((*cpVec)[rowOfCPInFocus][colOfCPInFocus].GetZ() + 20.0f);

	/*Recalculate the projected control points.*/
	nurbsSurface->SetProjectedControlPoints();

	/*Redraw the curve.*/
	nurbsSurface->ClearCalcDraw();
}

/*Decrease z component of the control point in focus.*/
void DecreaseZ()
{
	/*If no control point is in focus, return.*/
	if (rowOfCPInFocus == MAGIC_GARBAGE || colOfCPInFocus == MAGIC_GARBAGE)
		return;

	DrawCircleAroundControlPointInFocus(Color3(0, 0, 0));

	/*Decrease the z component of the control point in focus.*/
	std::vector<std::vector<Vector3F>> *cpVec = &(nurbsSurface->GetControlPointVec());
	(*cpVec)[rowOfCPInFocus][colOfCPInFocus].SetZ((*cpVec)[rowOfCPInFocus][colOfCPInFocus].GetZ() - 20.0f);

	/*Recalculate the projected control points.*/
	nurbsSurface->SetProjectedControlPoints();

	/*Redraw the curve.*/
	nurbsSurface->ClearCalcDraw();
}


void DrawCircleAroundObjectInFocus(const Vector2I &center, const Color3 &color)
{
	Circle tempCircle = Circle(center, 10, color);
	tempCircle.Draw();
}

void DrawCircleAroundControlPointInFocus(const Color3 &color)
{
	if (rowOfCPInFocus != MAGIC_GARBAGE && colOfCPInFocus != MAGIC_GARBAGE)
		DrawCircleAroundObjectInFocus(nurbsSurface->GetProjectedControlPointVec()[rowOfCPInFocus][colOfCPInFocus], color);
}