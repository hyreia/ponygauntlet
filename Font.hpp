#ifndef FONT_HPP
#define FONT_HPP

struct ALLEGRO_FONT;


namespace gauntlet
{
	enum FONT_TYPE
	{
		DEBUG_FONT,
		MESSAGE_FONT
	};

	enum
	{
		TEXT_ALIGN_LEFT = 0,
		TEXT_ALIGN_CENTER = 1,
		TEXT_ALIGN_RIGHT = 2
	};

	class Font
	{
	private:
		static const char *fontDirectory;
		
		ALLEGRO_FONT *font;
	public:
		Font();
		~Font();
		void Initialize(const char *filename, int size);
		void InitAsDebugFont();
		



		int LineHeight();
		int WidthOfText(const char *text);

		/* DrawTxt instead of DrawText because Microsoft! */
		void DrawTxt(double x, double y, int flags, float r, float g, float b, float a, char const *text);
		void DrawTxtf(double x, double y, int flags, float r, float g, float b, float a, char const *text, ...);

		int DrawTextWrapped(double x, double y, double width, const char *text, float r, float g, float b, float a, int flags, bool willDraw);
	};

};

#endif