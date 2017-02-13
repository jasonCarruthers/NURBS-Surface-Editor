#ifndef HFILE_GUI
#define HFILE_GUI



#include <iostream>
#include <string>
#include "Rectangle.h"


/*
 * Classes
 */
class Button
{
public:
	enum HoveredState
	{
		Hovered,
		NotHovered,
		Num__HoverStates,
	};
	enum PressedState
	{
		Pressed,
		NotPressed,
		Num__PressedStates,
	};
public:
	const Color3 HOVERED_COLOR = Color3(1.0f, 1.0f, 0.0f);
	const Vector2I TEXT_OFFSET = Vector2I(10, 10); /*Offset between button edges and the text.*/
public:
	/*
	 * Constructor(s)
	 */
	Button(const std::string &newText, const MyRectangle &newRect, void *newActionFcn = NULL);

	/*
	 * Accessors
	 */
	HoveredState GetHoveredState() const;
	PressedState GetPressedState() const;
	MyRectangle GetRect() const;
	std::string GetText() const;
	void Draw() const;

	/*
	 * Mutators
	 */
	void SetHoveredState(HoveredState newHoveredState);
	virtual void SetPressedState(PressedState newPressedState);
	void SetRect(const MyRectangle &newRect);
	void SetText(const std::string &newText);

protected:
	HoveredState mHoveredState;
	PressedState mPressedState;
	void *mActionFcn;
	MyRectangle *mRect;
	std::string mText;
};


class ButtonHandler
{
public:
	/*
	* Constructor(s)
	*/
	ButtonHandler();

	/*
	* Accessors
	*/
	std::vector<Button*>* GetButtonVec();
	void MonitorButtons(int mouseX, int mouseY, int button = MAGIC_GARBAGE, int action = MAGIC_GARBAGE) const;
	void DrawButtons() const;

	/*
	* Mutators
	*/
	void InsertButton(const Button &newButton);

private:
	std::vector<Button*> mButtonVec;
};


class RadioButtonGroup
{
public:
	static const unsigned int EDGE_RADIUS = 5;
	static const unsigned int FILL_RADIUS = 3;
	const static Color3 EDGE_COLOR;
	const static Color3 FILL_COLOR;
public:
	/*
	* Constructor(s)
	*/
	RadioButtonGroup();

	/*
	* Accessors
	*/
	std::pair<std::string, Vector2I*> GetRadioButtonInFocus() const;
	unsigned int GetNumRadioButtons() const;
	void Draw() const;

	/*
	* Mutators
	*/
	void Insert(const std::string &newRadioButtonName, const Vector2I &newRadioButtonPosition, void *newAction);
	void SetRadioButtonInFocus(unsigned int vecIndex);

	/*
	* Overloaded operators
	*/
	std::pair<std::string, Vector2I*> &operator[](int index);
	const std::pair<std::string, Vector2I*> &operator[](int index) const;

private:
	std::vector<std::pair<std::string, Vector2I*>> mRadioButtonVec;
	std::vector<void*> mActionVec;
	int mRadioButtonInFocusIndex;
};


class RadioButtonGroupHandler
{
public:
	/*
	* Constructor(s)
	*/
	RadioButtonGroupHandler();

	/*
	* Accessors
	*/
	std::vector<RadioButtonGroup*>* GetRadioButtonGroupVec();
	void MonitorRadioButtonGroups(int mouseX, int mouseY, int button = MAGIC_GARBAGE, int action = MAGIC_GARBAGE) const;
	void DrawRadioButtonGoups() const;

	/*
	* Mutators
	*/
	void InsertRadioButtonGroup(const RadioButtonGroup &newRadioButtonGroup);

private:
	std::vector<RadioButtonGroup*> mRadioButtonGroupVec;
};


class Checkbox
{
public:
	enum HoveredState
	{
		Hovered,
		NotHovered,
		Num__HoverStates,
	};
	enum PressedState
	{
		Pressed,
		NotPressed,
		Num__PressedStates,
	};
public:
	const Color3 NORMAL_COLOR = Color3(1.0f, 1.0f, 1.0f);
	const Color3 HOVERED_COLOR = Color3(1.0f, 1.0f, 0.0f);
	const Color3 PRESSED_COLOR = Color3(0.2f, 0.7f, 0.2f);
	const Color3 HOVERED_AND_PRESSED_COLOR = Color3(0.5f * HOVERED_COLOR.GetR() + 0.5f * PRESSED_COLOR.GetR(),
													0.5f * HOVERED_COLOR.GetG() + 0.5f * PRESSED_COLOR.GetG(),
													0.5f * HOVERED_COLOR.GetB() + 0.5f * PRESSED_COLOR.GetB());
public:
	/*
	* Constructor(s)
	*/
	Checkbox(const MyRectangle &newRect = MyRectangle(), void *newCheckedFcn = NULL, void *newUncheckedFcn = NULL);

	/*
	* Accessors
	*/
	HoveredState GetHoveredState() const;
	PressedState GetPressedState() const;
	MyRectangle GetRect() const;

	/*
	* Mutators
	*/
	void SetHoveredState(HoveredState newHoveredState);
	void SetPressedState(PressedState newPressedState);

private:
	MyRectangle *mRect;
	HoveredState mHoveredState;
	PressedState mPressedState;
	void *mCheckedFcn; /*Pointer to function to call when checkbox becomes checked*/
	void *mUncheckedFcn; /*Pointer to function to call when checkbox becomes unchecked*/
};


class CheckboxHandler
{
public:
	/*
	* Constructor(s)
	*/
	CheckboxHandler();

	/*
	* Accessors
	*/
	std::vector<Checkbox*>* GetCheckboxVec();
	void MonitorCheckboxes(int mouseX, int mouseY, int button = MAGIC_GARBAGE, int action = MAGIC_GARBAGE) const;
	void DrawCheckboxes() const;

	/*
	* Mutators
	*/
	void InsertCheckbox(const Checkbox &newCheckbox);

private:
	std::vector<Checkbox*> mCheckboxVec;
};



class Slider
{
public:
	enum Orientation
	{
		Horizontal,
		Vertical,
		Num__Orientations,
	};
public:
	/*
	* Constructor(s)
	*/
	Slider(const std::string &newName,
			MyRectangle &newRect = MyRectangle(),
			const Vector2F &newRange = Vector2F(0.0f, 1.0f),
			const Vector2I &newSliderPos = Vector2I(0, 0),
			void *newSliderMoveFcn = NULL);

	/*
	* Accessors
	*/
	std::string GetName() const;
	MyRectangle GetRect() const;
	Vector2F GetRange() const;
	Orientation GetSliderOrientation() const;
	Vector2I GetSliderPosition() const;
	float GetValue() const; /*Gets value based on slider position and is relative to mRange.*/
	bool GetPressedState() const;
	void CallSliderMoveFcn(float t) const;
	void Draw() const;

	/*
	* Mutators
	*/
	void SetName(const std::string &newName);
	void SetRect(const MyRectangle &newRect);
	void SetRange(const Vector2F &newRange);
	void SetSliderPosition(const Vector2I &newSliderPos);
	void SetPressedState(bool newPressedState);
	void SetSliderMoveFcn(void *newSliderMoveFcn);

private:
	std::string mName;
	MyRectangle mRect; /*Bounding rect*/
	Vector2F mRange; /*First value is beginning of range, inclusive, and second value is end of range, inclusive.*/
	Orientation mSliderOrientation; /* Slider orientation--that is, whether the slider slides vertically or
									   horizontally--is along the axis of the larger of the two dimensions of
									   mRect.If mRect is a square, one of the dimensions will be made 1 pixel
									   larger.*/
	MyRectangle mSliderTrack; /*A thin rect that the slider knob "slides along".*/
	MyRectangle mSlider; /*A small rect representing the slider knob.*/
	bool mIsPressed;
	void *mSliderMoveFcn; /*What action to take when the slider knob moves.*/
};


class SliderHandler
{
public:
	/*
	* Constructor(s)
	*/
	SliderHandler();

	/*
	* Accessors
	*/
	std::vector<Slider*> &GetSliderVec();
	void MonitorSliders(int mouseX, int mouseY, int button = MAGIC_GARBAGE, int action = MAGIC_GARBAGE) const;
	void DrawSliders() const;

	/*
	* Mutators
	*/
	void InsertSlider(const Slider &newSlider);

private:
	std::vector<Slider*> mSliderVec;
};



/*
* Global function prototypes
*/
void InitGUI();
void DrawGUI();
Vector2F GetRasterPosition(const Vector2I &desiredPos, int strLen);
Vector2I GetTextDimensions(const std::string &text);

void ControlPointVisibilityEnabled(); /*Sets control points visible for all surfaces.*/
void ControlPointVisibilityDisabled(); /*Sets control points NOT visible for all surfaces.*/
void ControlMeshVisibiliytEnabled(); /*Sets control mesh visible for all surfaces.*/
void ControlMeshVisibiliytDisabled(); /*Sets control mesh NOT visible for all surfaces.*/
void SurfaceMeshVisibilityEnabled(); /*Sets surface mesh visible for all surfaces.*/
void SurfaceMeshVisibilityDisabled(); /*Sets surface mesh NOT visible for all surfaces.*/
void GouraudShadingVisibilityEnabled(); /*Sets Gouraud shading visible for all surfaces.*/
void GouraudShadingVisibilityDisabled(); /*Sets Gouraud shading NOT visible for all surfaces.*/
void InsertRow(); /*Inserts a new row into the NURBS surface in focus.*/
void InsertColumn(); /*Inserts a new column into the NURBS surface in focus.*/
void DeleteRow(); /*Deletes a row from the NURBS surface in focus.*/
void DeleteColumn(); /*Deletes a column from the NURBS surface in focus.*/
void IncreaseX(); /*Increase x component of the control point in focus.*/
void DecreaseX(); /*Decrease x component of the control point in focus.*/
void IncreaseY(); /*Increase y component of the control point in focus.*/
void DecreaseY(); /*Decrease y component of the control point in focus.*/
void IncreaseZ(); /*Increase z component of the control point in focus.*/
void DecreaseZ(); /*Decrease z component of the control point in focus.*/

void DrawCircleAroundObjectInFocus(const Vector2I &center, const Color3 &color = Color3(1.0f, 1.0f, 0.0f));
void DrawCircleAroundControlPointInFocus(const Color3 &color = Color3(1, 1, 0));

/*
 * Global variables
 */
extern MyRectangle *curveSpace; /*The screen space where the user can draw curves: everywhere
								  on the screen except where panels and other relevent
								  GUI objects are.*/
extern MyRectangle *verticalPanel;
extern CheckboxHandler *checkboxHandler;
extern Checkbox *controlPointCheckbox;
extern Checkbox *controlMeshCheckbox;
extern Checkbox *surfaceMeshCheckbox;
extern Checkbox *gouraudShadingCheckbox;
extern SliderHandler *sliderHandler;
extern Slider *resolutionSlider;
extern Slider *orderKSlider;
extern Slider *orderLSlider;
extern Slider *weightSlider;
extern ButtonHandler *buttonHandler;
extern Button *insertRowButton;
extern Button *insertColumnButton;
extern Button *deleteRowButton;
extern Button *deleteColumnButton;
extern Button *increaseXButton;
extern Button *decreaseXButton;
extern Button *increaseYButton;
extern Button *decreaseYButton;
extern Button *increaseZButton;
extern Button *decreaseZButton;


#endif /*HFILE_GUI*/