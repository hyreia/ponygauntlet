#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <string>
#include <memory>
#include <unordered_map>

struct ALLEGRO_BITMAP;

namespace gauntlet
{

class Bitmap;

/* Instead of storing all the bitmaps somewhere else, the Bitmap class has
a static unordered map of Bitmaps. These weak pointers disappear with no reference
to them. As such, when one is Load()ed if the shared ptr goes out of bounds it
is automatically freed. Because of this, a shared pointer needs to be laying around
of any bitmaps that may be used on that screen.

On the plus side, by creating a Bitmap central hub like this, no Bitmap is ever 
loaded more than once. */
class Bitmap
{
private:
	
	Bitmap(const char *filename);
	static std::unordered_map<std::string, std::weak_ptr<Bitmap> > bitmapsLoadedFromFiles;
		
	std::string filename;
public:
	/* mask will turn default mask color to transparent alpha*/
	static std::shared_ptr<Bitmap> Load(const char *filename, bool willConvertMaskToAlpha = true);

	Bitmap();
		
	//Bitmap class does not take ownership of ALLEGRO_BITMAP bitmap; will not destroy bitmap
	Bitmap(ALLEGRO_BITMAP *bitmap);
	Bitmap::Bitmap(int width, int height, int flags = 0);
	~Bitmap();
	ALLEGRO_BITMAP *bitmap;

	void Lock();
	void Unlock();
	void HoldDrawing(bool hold);

	int Width();
	int Height();

	void ConvertMaskToAlpha(const int r = 64, const int g = 64, const int b = 64);

	void Draw(double dx, double dy, int flags = 0);
	void Draw(double sx, double sy, double sw, double sh,
		double dx, double dy, int flags = 0);
	void DrawTinted(float red, float green, float blue, float alpha,
		double sx, double sy, double sw, double sh,
		double dx, double dy,
		int flags = 0);
	
	void Draw(float sx, float sy, float sw, float sh,
	unsigned char r, unsigned char g, unsigned char b, unsigned char a, 
	float cx, float cy, 
	float dx, float dy,
	float dw, float dh,
	float radians, int flags);

	void DrawRotated(double sx, double sy, double sw, double sh,
		double dx, double dy, double cx, double cy, double radians, int flags = 0);

	void DrawScaled(double sx, double sy, double sw, double sh,
		double dx, double dy, double dw, double dh,
		int flags = 0);

	/* Whole bitmap is drawn, c is drawn at d, angle is in radians */
	void DrawRotated(double cx, double cy, double dx, double dy, double angle, int flags = 0);
};

};
#endif