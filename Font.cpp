#include "Font.hpp"

#include <string>
#include <vector>
#include <allegro5\allegro_font.h>

using namespace gauntlet;

//Intentionally just put the few fonts we have with images
const char *Font::fontDirectory = "images";

Font::Font():
font(NULL)
{}

Font::~Font()
{
	if(font) al_destroy_font(font);
	font = NULL;
}

void Font::Initialize(const char *filename, int size)
{
	if(font) al_destroy_font(font);

	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_drop_path_tail(path);
	al_append_path_component(path, "data");
	al_append_path_component(path, fontDirectory);
	

	const char *filePath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
	std::string filePathAndFile = filePath;
	al_set_path_filename(path, filename);

	font = al_load_font( al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP), size, 0);

	al_destroy_path(path);
}

void Font::InitAsDebugFont()
{
	if(font) al_destroy_font(font);

	font = al_create_builtin_font();
}

int Font::LineHeight()
{
	if(font) return al_get_font_line_height(font);
	else return 0;
}

int Font::WidthOfText(const char *text)
{
	if(font) return al_get_text_width(font, text);
	else return 0;
}

void Font:: DrawTxt(double x, double y, int flags, float r, float g, float b, float a, char const *text)
{
	if(font) al_draw_text(font, al_map_rgba_f(r, g, b, a), x, y, flags, text);
}

#pragma warning(push)
#pragma warning(disable: 4996)

void Font:: DrawTxtf(double x, double y, int flags, float r, float g, float b, float a, char const *text, ...)
{
	va_list ap;
    va_start (ap, text);

    size_t size = 1024;
    char stackbuf[1024];
    std::vector<char> dynamicbuf;
    char *buf = &stackbuf[0];
	std::string ret;

    while (true) {
        // Try to vsnprintf into our buffer.
        int needed = vsnprintf (buf, size, text, ap);
        // NB. C99 (which modern Linux and OS X follow) says vsnprintf
        // failure returns the length it would have needed.  But older
        // glibc and current Windows return -1 for failure, i.e., not
        // telling us how much was needed.

        if (needed <= (int)size && needed >= 0) {
            // It fit fine so we're done.
            ret = std::string (buf, (size_t) needed);
			break;
        }

        // vsnprintf reported that it wanted to write more characters
        // than we allotted.  So try again using a dynamic buffer.  This
        // doesn't happen very often if we chose our initial size well.
        size = (needed > 0) ? (needed+1) : (size*2);
        dynamicbuf.resize (size);
        buf = &dynamicbuf[0];
    }

	
    va_end (ap);
    
	if(font) al_draw_text(font, al_map_rgba_f(r, g, b, a), x, y, flags, ret.c_str());
}

#pragma warning(pop)



#pragma warning(push)
#pragma warning(disable: 4996)
/** Returns where the bottom of the text box would be*/
int Font::DrawTextWrapped(double x, double y, double width, const char *text, float r, float g, float b, float a, int flags, bool willDraw)
{
	/* Borrowed from 
	http://allegrostan.wordpress.com/2012/01/22/a-rather-nice-word-wrapped-text-routine-for-allegro-5/ 
	*/
    
	ALLEGRO_COLOR fc = al_map_rgba_f(r, g, b, a);

     char stext[2048]; // Copy of the passed text.


     char * pch; // A pointer to each word.
     char word[255]; // A string containing the word (for convienence)
     char breakchar[12]; // Contains the break line character "\n "
     char Lines[40][1024]; // A lovely array of strings to hold all the lines (40 max atm)
     char TempLine[1024]; // Holds the string data of the current line only.
     int CurrentLine = 0; // Counts which line we are currently using.
     int q; // Used for loops

     // Setup our strings
     strncpy(stext, text, 2048);
	 strncpy(stext, text, 2048);
     strncpy(breakchar,"\n ", 12);
     strncpy(TempLine,"", 1024);
     for(q = 0;q < 40;q+=1)
     {
          sprintf(Lines[q],"");
     }
     //-------------------- Code Begins

     pch = strtok (stext," ");                               // Get the first word.
     do
     {
          strcpy(word,"");                                  // Truncate the string, to ensure there's no crazy stuff in there from memory.
          sprintf(word,"%s ",pch);
          sprintf(TempLine,"%s%s",TempLine,word);             // Append the word to the end of TempLine
          // This code checks for the new line character.
          if(strcmp(word,breakchar) == 0)
          {
                CurrentLine+=1;                                 // Move down a Line
                strcpy(TempLine,"");                            // Clear the tempstring
          }
          else
          {
                if(al_get_text_width(font, TempLine) >= (width))   // Check if text is larger than the area.
                {
                      strcpy(TempLine,word);                      // clear the templine and add the word to it.
                      CurrentLine+=1;                             // Move to the next line.
                }
                if(CurrentLine < 40)
                {
                      strcat(Lines[CurrentLine],word);                // Append the word to whatever line we are currently on.
                }
          }
          pch = strtok (NULL, " ");                           // Get the next word.
     }while (pch != NULL);
     // ---------------------------------- Time to draw.
     if(willDraw == true)                                       //Check whether we are actually drawing the text.
     {
          for(q = 0;q <=CurrentLine;q+=1)                     // Move through each line and draw according to the passed flags.
          {
                if(flags == ALLEGRO_ALIGN_LEFT)
                     al_draw_text(font,fc, x, y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_LEFT,Lines[q]);
                if(flags == ALLEGRO_ALIGN_CENTRE)
                     al_draw_text(font,fc, x + (width/2), y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_CENTRE,Lines[q]);
                if(flags == ALLEGRO_ALIGN_RIGHT)
                     al_draw_text(font,fc, x + width, y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_RIGHT,Lines[q]);
          }

     }
     return((CurrentLine+1) * al_get_font_line_height(font) + y);  // Return the actual height of the text in pixels.	
}
#pragma warning(pop)