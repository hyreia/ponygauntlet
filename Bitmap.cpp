#include "Bitmap.hpp"
#include <allegro5/allegro.h>

using namespace gauntlet;

std::unordered_map<std::string, std::weak_ptr<Bitmap> > Bitmap::bitmapsLoadedFromFiles;

Bitmap::Bitmap(): bitmap(NULL)
{}

Bitmap::~Bitmap()
{
	//If it has a filename, it was loaded from file and now it's going away
	//So we need to remove it from the list of bitmaps loaded from file
	if(!filename.empty())
	{
		//remove from bitmapsFromFile list
		if(bitmapsLoadedFromFiles.count(filename) == 1)
		{
			bitmapsLoadedFromFiles.erase(filename);
		}
	}


	if(bitmap)
	{
		al_destroy_bitmap(bitmap);
		bitmap = NULL;
	}
}

//Bitmap::Bitmap(ALLEGRO_BITMAP *_bitmap):
//bitmap(_bitmap)
//{}

std::shared_ptr<Bitmap> Bitmap::Load(const char *filename, bool willConvertMaskToAlpha)
{
	std::string file = filename;
	if( bitmapsLoadedFromFiles.count(file) == 1)
	{
		return std::shared_ptr<Bitmap>( bitmapsLoadedFromFiles.at(file));
	}
	else
	{
		//Add to list and return
		std::shared_ptr<Bitmap> temp(new Bitmap(filename));
		if(temp && willConvertMaskToAlpha) temp->ConvertMaskToAlpha();
		bitmapsLoadedFromFiles.insert( std::pair<std::string, std::weak_ptr<Bitmap> >(file, temp));
		return temp;
	}
}

Bitmap::Bitmap(const char *filename):
bitmap(NULL)
{
	this->filename = filename;

	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_drop_path_tail(path);
	al_append_path_component(path, "data");
	al_append_path_component(path, "images");
	//al_append_path_component(path, "..\data\images");
	

	const char *filePath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
	std::string filePathAndFile = filePath;
	filePathAndFile.append(filename);

	bitmap = al_load_bitmap(filePathAndFile.c_str());
	if(bitmap && al_get_bitmap_flags(bitmap) & ALLEGRO_MEMORY_BITMAP)
	{
		printf("WARNING: MEMORY BITMAP BEING USED.\n");
	}
	if(!bitmap)
	{
		printf("WARNING: Bitmap %s failed to load.\n", filename);
	}

	al_destroy_path(path);
}

/*
Bitmap::Bitmap(int width, int height, int flags)
{
	if(flags != 0)
		al_set_new_bitmap_flags(flags);

	bitmap = al_create_bitmap(width, height);
	//printf("Bitmap of size %d %d\n", width, height);
	if(bitmap && al_get_bitmap_flags(bitmap) & ALLEGRO_MEMORY_BITMAP)
	{
		printf("WARNING: MEMORY BITMAP BEING USED.\n");
	}

	if(flags != 0)
		al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
}*/	

void Bitmap::Lock()
{
	al_lock_bitmap(bitmap, al_get_bitmap_format(bitmap), ALLEGRO_LOCK_READONLY);
}

void Bitmap::Unlock()
{
	al_unlock_bitmap(bitmap);
}

int Bitmap::Width(){ return al_get_bitmap_width(bitmap); }
int Bitmap::Height(){ return al_get_bitmap_height(bitmap); }

void Bitmap::ConvertMaskToAlpha(const int r, const int g, const int b)
{ al_convert_mask_to_alpha(bitmap, al_map_rgb(r, g, b)); }

void Bitmap::Draw(double dx, double dy, int flags)
{
	al_draw_bitmap(bitmap, floor(dx), floor(dy), flags);
	
}

void Bitmap::HoldDrawing(bool hold)
{
	al_hold_bitmap_drawing(hold);
}


void Bitmap::Draw(double sx, double sy, double sw, double sh,
	double dx, double dy, int flags)
{
	al_draw_bitmap_region(bitmap, floor(sx), floor(sy), floor(sw), floor(sh), floor(dx), floor(dy), flags);
}

void Bitmap::DrawTinted(float red, float green, float blue, float alpha,
	double sx, double sy, double sw, double sh,
	double dx, double dy, int flags)
{
	al_draw_tinted_bitmap_region(bitmap, al_map_rgba_f(red, green, blue, alpha),
		floor(sx), floor(sy), floor(sw), floor(sh), floor(dx), floor(dy), flags);
}

void Bitmap::DrawScaled(double sx, double sy, double sw, double sh,
	double dx, double dy, double dw, double dh,
	int flags)
{
	al_draw_scaled_bitmap(bitmap, floor(sx), floor(sy), floor(sw), floor(sh), floor(dx), floor(dy), floor(dw), floor(dh), flags);
}

void Bitmap::DrawRotated(double sx, double sy, double sw, double sh,
double dx, double dy, double cx, double cy, double radians, int flags)
{
	al_draw_tinted_scaled_rotated_bitmap_region(bitmap, floor(sx), floor(sy), floor(sw), floor(sh), 
		al_map_rgb(255, 255, 255), float(cx), float(cy),
		floor(dx), floor(dy), 1.0f, 1.0f, float(radians), flags);
}

void Bitmap::Draw(float sx, float sy, float sw, float sh,
unsigned char r, unsigned char g, unsigned char b, unsigned char a, 
float cx, float cy, 
float dx, float dy,
float dw, float dh,
float radians, int flags)
{
	al_draw_tinted_scaled_rotated_bitmap_region(bitmap, sx, sy, sw, sh, al_map_rgba(r, g, b, a),
		cx, cy, dx, dy, dw/sw, dh/sh, radians, flags);
}

void Bitmap::DrawRotated(double cx, double cy, double dx, double dy, double angle, int flags)
{
	al_draw_rotated_bitmap(bitmap, float(cx), float(cy), floor(dx), floor(dy), float(angle), flags);
}