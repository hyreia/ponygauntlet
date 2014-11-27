#ifndef ALLEGRO_HPP
#define ALLEGRO_HPP

struct ALLEGRO_DISPLAY;
struct ALLEGRO_TIMER;
struct ALLEGRO_EVENT_QUEUE;
struct ALLEGRO_BITMAP;

#include "XYPair.hpp"

namespace gauntlet
{
	class Allegro
	{
	public:
		/* Zeroes out and NULLs members */
		Allegro();
		/* Does nothing */
		~Allegro();

		/* Makes Allegro5 ready to use, creates display and clears to black */
		bool Initialize();
		/* Destroy all allegro objects that need to free memory */
		void Destroy();

		/* Intentionally public members TODO: fix this */
		ALLEGRO_DISPLAY *display;
		ALLEGRO_TIMER *timer;
		ALLEGRO_EVENT_QUEUE *event_queue;

		/* Creates display, destroys any pre-existing display and rejoins it to the event_queue */
		void RecreateDisplay(int displayWidth, int displayHeight);

		/* Returns true if it worked. It might not work if the cursor is too big */
		bool SetMouseCursor(ALLEGRO_BITMAP *cursor, int xFocus, int yFocus);

		void ClearBufferToColor(unsigned char r, unsigned char g, unsigned char b);

		void ShowError(const char *message);

		/* call before drawing */
		void SetBufferAsDrawTarget();
		/* Call when done drawing */
		void DrawBufferToDisplay();

		/* Buffer (game display) may be bigger than actual window, this translates mouse position received from ALLEGRO_EVENTS
		 * into the mouse's position in the buffer */
		inline void TranslateMousePositionToBufferPosition(int &x, int &y)
		{
			x = static_cast<int>(displayToBuffer*x);
			y = static_cast<int>(displayToBuffer*y);
		}

		/* size of buffer being drawn to */
		static const double BUFFER_WIDTH, BUFFER_HEIGHT;
	private:

		static const char *graphicsInfoFile;
		void RecreateGraphicsIniFile();

		/* Size of window/fullscreen of game in pixels */
		XYPair<int> displaySize;
		bool isDisplayWindowed;

		/* Called after display is (re)created to calculate displayToBuffer and bufferToDisplayOffset */
		void CalculateBufferToDisplay();

		/* Product of a point in the display and displayToBuffer gets equivalent point on the buffer */
		double displayToBuffer;

		/* Position on display to draw buffer, necessary for strange non 16:10 window shapes */
		XYPair<double> bufferToDisplayOffset;

		/* Surface drawn to before being drawn to display, abstracts display size away and is effective double buffer */
		ALLEGRO_BITMAP *buffer;

	};
};

#endif