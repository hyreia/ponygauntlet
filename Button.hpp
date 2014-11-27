#ifndef BUTTON_HPP
#define BUTTON_HPP

/* A button is an abstract form of input that can be set to down or set to up.
It's state is checked in two ways: has the button been pressed and is the button held?
Pressed translated to: has the button gone down since I last checked it?
Held translates to: is the button's current state down? 

A button is the form of input that that the game normally concerns itself with.
Each player's controller has buttons. Externally the game can set these either through
Input from the current computer's UI or Networking from another's computer. It doesn't
matter what the source of the button's new state is and the game doesn't care. 
*/

namespace gauntlet
{

struct Button
{
private:
	bool pressed, held;
public:
	Button():
	pressed(false), held(false)
	{}
	/* Has button been pressed since last checked */
	bool IsPressed()
	{
		if(pressed)
		{
			pressed = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsHeld()
	{
		return held;
	}

/* Button was let up*/
void Up()
{
	pressed = false;
	held = false;
}

/* Button was pressed down */
void Down()
{
	pressed = true;
	held = true;
}

};


};


#endif