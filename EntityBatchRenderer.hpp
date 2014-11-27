#ifndef ENTITY_BATCH_RENDERER_HPP
#define ENTITY_BATCH_RENDERER_HPP

#include <vector>
#include <memory>
#include "XYPair.hpp"
#include "Bitmap.hpp"
#include "GameEntity.hpp"
#include "GameCharacter.hpp"

namespace gauntlet
{

enum EntityRendereringDataIndexes
{
	SOURCE_X,
	SOURCE_Y,
	SOURCE_W,
	SOURCE_H,
	DEST_X,
	DEST_Y,
	DEST_W,
	DEST_H,
	SIZE_OF_SOURCE_AND_DESTINATION_ARRAY,
	RTINT = 0,
	GTINT,
	BTINT,
	ATINT,
	SIZE_OF_COLOR_ARRAY,
	ROTATION_IN_RADIANS = 0,
	ROTATION_CX,
	ROTATION_CY,
	SIZE_OF_ROTATION_ARRAY
};

struct EntityDrawingInfo
{
	/* Use EntityRendereringDataIndexes to get the value wanted*/
	double pos[SIZE_OF_SOURCE_AND_DESTINATION_ARRAY];

	/* Use EntityRendereringDataIndexes to get the value wanted*/
	unsigned char color[SIZE_OF_COLOR_ARRAY];

	/* Use EntityRendereringDataIndexes to get the value wanted*/
	double rotation[SIZE_OF_ROTATION_ARRAY];
	int flags;

	inline void Draw(std::shared_ptr<Bitmap> bitmap);
};

struct EntityDrawBatch
{
	std::vector<EntityDrawingInfo> batch;
	std::shared_ptr<Bitmap> bitmap;
};

class EntityBatchRenderer
{
public:
	void QueueDrawing(GameEntity *entity);
	void QueueDrawing(GameCharacter *character, const double &alpha);
	void DrawQueued(XYPair<double> cameraOffset);
	void ClearDrawList();
private:
		std::vector<EntityDrawBatch> entityDrawList;
};

};
#endif