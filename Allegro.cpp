#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>

#include "Allegro.hpp"
#include "GauntletApp.hpp"
#include "ConfigFile.hpp"
#include "SfxManager.hpp"

using namespace gauntlet;

const char *Allegro::graphicsInfoFile = "graphics.ini";
const double Allegro::BUFFER_WIDTH = 1920.0;
const double Allegro::BUFFER_HEIGHT = 1200.0;

Allegro::Allegro():
display(NULL),
timer(NULL),
buffer(NULL),
event_queue(NULL),
isDisplayWindowed(true)
{}

Allegro::~Allegro()
{}

bool Allegro::Initialize()
{
	//Start the allegro features we need:
	if(!al_init()) { ShowError("Allegro couldn't init()!"); return false; }

	if(!al_init_native_dialog_addon()){ ShowError("Init Native Dialog Addon failed"); return false; }

	if(!al_install_keyboard()){ ShowError("Couldn't install keyboard."); return false; 	}

	if(!al_install_mouse()){ ShowError("Couldn't install mouse."); return false; }

	if(!al_install_joystick()){ ShowError("Couldn't install joystick."); return false; }

	if(!al_init_image_addon()){ ShowError("Couldn't init image addon"); return false; }

	if(!al_install_audio()){ ShowError("Couldn't install audio"); return false; }

	if(!al_init_acodec_addon()){ ShowError("Failed to start audio acodec addon"); return false; }
	else
	{
		if(!al_reserve_samples(SfxManager::NUM_OF_ALLEGRO_SAMPLES_RESERVED)){ ShowError("Couldn't reserve sound samples!"); return false; }
	}
	//this intentionally returns void
	al_init_font_addon();
	if(!al_init_ttf_addon()){ ShowError("Couldn't init ttf addon"); return false; }
	
	al_init_primitives_addon();

	//Create the Allegro structs we're going to be using all the time
	event_queue = al_create_event_queue();
	if(!event_queue){ ShowError("Event queue couldn't be created"); return false; }

	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	al_set_new_display_option(ALLEGRO_VSYNC, ALLEGRO_REQUIRE, 1); //required for Windows

	ConfigFile video(graphicsInfoFile);
	if(!video.IsOpen())
	{
		video.Close();
		RecreateGraphicsIniFile();
		video.Open(graphicsInfoFile);
	}
	
	//display is windowed
	isDisplayWindowed = !video.GetBooleanValue(NULL, "Fullscreen");

	int displayX = video.GetIntValue(NULL, "x");
	int displayY = video.GetIntValue(NULL, "y");
		
	if(displayX == 0 || displayY == 0)
	{
		const int SENSIBLE_DEFAULT_X_RESOLUTION = 960;
		const int SENSIBLE_DEFAULT_Y_RESOLUTION = 600;
		displayX = SENSIBLE_DEFAULT_X_RESOLUTION;
		displayY = SENSIBLE_DEFAULT_Y_RESOLUTION;
	}

	RecreateDisplay(displayX, displayY);

	buffer = al_create_bitmap(BUFFER_WIDTH, BUFFER_HEIGHT);
	SetBufferAsDrawTarget();
	ClearBufferToColor(0,0,0);
	
	timer = al_create_timer(app->dt);
	if(timer)
	{
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
	}
	else
	{
		ShowError("Couldn't create timer"); return false;
	}

	//Register other event sources
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_joystick_event_source());	

	return true;

}

void Allegro::Destroy()
{
	al_destroy_bitmap(buffer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
}

void Allegro::RecreateDisplay(int displayWidth, int displayHeight)
{
	XYPair<int> windowPosition;
	if(!isDisplayWindowed)
	{
		//Override resolution with monitor resolution

		//Determine monitor game is running on

		//Get window position
		XYPair<int> windowPosition(0, 0);

		if(display)
			al_get_window_position(display, &windowPosition.x, &windowPosition.y);
		else
			al_get_new_window_position(&windowPosition.x, &windowPosition.y);


		//find where top left corner is is in
		int monitorInUse = 0;
		ALLEGRO_MONITOR_INFO info;
		for(int monitor = 0; monitor < al_get_num_video_adapters(); monitor++)
		{
			monitorInUse = monitor;
			al_get_monitor_info(monitor, &info);
			if(windowPosition.x >= info.x1 && windowPosition.x < info.x2 &&
				windowPosition.y >= info.y2 && windowPosition.y < info.y2)
			{
				al_set_new_window_position(info.x1, info.y1);
				break;
				
			}
		}

		al_get_monitor_info(monitorInUse, &info);
		displayWidth = info.x2 - info.x1;
		displayHeight = info.y2 - info.y1;
	}

	displaySize.x = displayWidth;
	displaySize.y = displayHeight;

	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);
	if(display)
		al_destroy_display(display);

	display = al_create_display(displayWidth, displayHeight);
	if(!display)
		ShowError("Tried to create a display of an illegal size");


	//Recalculate Backbuffer to Display
	CalculateBufferToDisplay();

	al_set_window_title(display, app->title);
	al_flip_display();

}

bool Allegro::SetMouseCursor(ALLEGRO_BITMAP *cursor, int xFocus, int yFocus)
{
	auto pointer = al_create_mouse_cursor(cursor, xFocus, yFocus);
	if(pointer)
	{
		return al_set_mouse_cursor(display, pointer);
	}
	else return false;
}

void Allegro::ClearBufferToColor(unsigned char r, unsigned char g, unsigned char b)
{
	al_clear_to_color(al_map_rgb(r, g, b));
}

void Allegro::ShowError(const char *message)
{
	al_show_native_message_box(display,
		"Pony Gauntlet", "Fatal Error",
		message, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	printf("%s\n", message);
}

void Allegro::SetBufferAsDrawTarget()
{
	al_set_target_bitmap(buffer);
}

void Allegro::DrawBufferToDisplay()
{
	al_set_target_backbuffer(display);
	al_draw_scaled_bitmap(buffer, 
		0, 0,	/* Buffer */
		BUFFER_WIDTH, BUFFER_HEIGHT, 
		
		bufferToDisplayOffset.x, bufferToDisplayOffset.y, /* Buffer To Display*/
		displaySize.x, displaySize.y, 0);

	al_flip_display();
	al_rest(0.001);
}

void Allegro::RecreateGraphicsIniFile()
{
	ConfigFile graphics(graphicsInfoFile);
	graphics.CreateEmpty();

	graphics.SetValue(NULL, "Fullscreen", "false");
	graphics.SetValue(NULL, "x", 960);
	graphics.SetValue(NULL, "y", 600);
	graphics.Save();
}

void Allegro::CalculateBufferToDisplay()
{
	int width = al_get_display_width(display);
	int height = al_get_display_height(display);
	displaySize.x = width;
	displaySize.y = height;

	double sx = static_cast<double>(BUFFER_WIDTH)/static_cast<double>(displaySize.x);
	double sy = static_cast<double>(BUFFER_HEIGHT)/static_cast<double>(displaySize.y);

	double scale = (sx > sy)? sx: sy;	//<-black bars
	//double scale = (sx < sy)? sx: sy;	//<-parts of screen are invisible unless accounted for)

	//bufferToDisplayOffset.w = ceil( static_cast<double>(BUFFER_WIDTH)/ scale);
	//bufferToDisplayOffset.h = ceil( static_cast<double>(BUFFER_HEIGHT)/scale);
	bufferToDisplayOffset.x = (static_cast<double>(displaySize.x) - ceil( static_cast<double>(BUFFER_WIDTH)/ scale))/2;
	bufferToDisplayOffset.y = (static_cast<double>(displaySize.y) - ceil( static_cast<double>(BUFFER_HEIGHT)/scale))/2;

	displayToBuffer = scale;

	//bufferToDisplayOffset.w = 1/scale;
	//bufferToDisplayOffset.h = bufferHeight/bufferHeight;

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

	//Resize to same size to fix a resize bug when toggling fullscreen_window
	al_resize_display(display, width, height);
	if(isDisplayWindowed)
	{
		al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, 0);
	}
	else
	{
		al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, 1);
	}
}