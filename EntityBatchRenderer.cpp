#include "EntityBatchRenderer.hpp"
#include <allegro5/allegro.h>

using namespace gauntlet;

void EntityBatchRenderer::QueueDrawing(GameEntity *entity)
{
	auto bitmap = entity->GetSourceBitmap();
	auto imageBox = entity->GetImageBox();
	auto imageSource = entity->ImageSource();
	auto imageFlags = entity->ImageDrawingFlags();

	EntityDrawingInfo info;
	info.pos[SOURCE_X] = imageSource.x;
	info.pos[SOURCE_Y] = imageSource.y;
	info.pos[SOURCE_W] = imageBox.w;
	info.pos[SOURCE_H] = imageBox.h;
	info.pos[DEST_X] = imageBox.x;
	info.pos[DEST_Y] = imageBox.y;
	info.pos[DEST_W] = imageBox.w;
	info.pos[DEST_H] = imageBox.h;
	info.color[RTINT] = 255;
	info.color[GTINT] = 255;
	info.color[BTINT] = 255;
	info.color[ATINT] = 255;
	info.flags = imageFlags;
	info.rotation[ROTATION_IN_RADIANS] = 0;
	info.rotation[ROTATION_CX] = 0;
	info.rotation[ROTATION_CY] = 0;

	if(bitmap == entityDrawList.back().bitmap)
	{
		entityDrawList.back().batch.push_back(info);
	}
	else
	{
		EntityDrawBatch newBatch;
		newBatch.bitmap = bitmap;
		newBatch.batch.push_back(info);
		entityDrawList.push_back(newBatch);
	}
}

void EntityBatchRenderer::QueueDrawing(GameCharacter *character, const double &alpha)
{
	auto bitmap = character->GetSourceBitmap();
	auto imageBox = character->GetImageBoxAtDeltaTime(alpha);
	auto imageSource = character->ImageSource();
	auto imageFlags = character->ImageDrawingFlags();

	EntityDrawingInfo info;
	info.pos[SOURCE_X] = imageSource.x;
	info.pos[SOURCE_Y] = imageSource.y;
	info.pos[SOURCE_W] = imageBox.w;
	info.pos[SOURCE_H] = imageBox.h;
	info.pos[DEST_X] = imageBox.x;
	info.pos[DEST_Y] = imageBox.y;
	info.pos[DEST_W] = imageBox.w;
	info.pos[DEST_H] = imageBox.h;
	info.color[RTINT] = 255;
	info.color[GTINT] = 255;
	info.color[BTINT] = 255;
	info.color[ATINT] = 255;
	info.flags = imageFlags;
	info.rotation[ROTATION_IN_RADIANS] = 0;
	info.rotation[ROTATION_CX] = 0;
	info.rotation[ROTATION_CY] = 0;

	if(bitmap == entityDrawList.back().bitmap)
	{
		entityDrawList.back().batch.push_back(info);
	}
	else
	{
		EntityDrawBatch newBatch;
		newBatch.bitmap = bitmap;
		newBatch.batch.push_back(info);
		entityDrawList.push_back(newBatch);
	}	
}

void EntityDrawingInfo::Draw(std::shared_ptr<Bitmap> bitmap)
{
	bitmap->Draw(pos[SOURCE_X], pos[SOURCE_Y], pos[SOURCE_W], pos[SOURCE_H],
		color[RTINT], color[GTINT], color[BTINT], color[ATINT],
		rotation[ROTATION_CX], rotation[ROTATION_CY],

		pos[DEST_X], pos[DEST_Y], pos[DEST_W], pos[DEST_H],
		rotation[ROTATION_IN_RADIANS], flags);
}

void EntityBatchRenderer::DrawQueued(XYPair<double> cameraOffset)
{
	//Translate all the drawing
	ALLEGRO_TRANSFORM camera;
	al_identity_transform(&camera);
	al_translate_transform(&camera, -cameraOffset.x, -cameraOffset.y);
	al_use_transform(&camera);

	//for(auto 
	//while(!entityDrawList.empty())
	for(auto listOfBatches = entityDrawList.begin(); listOfBatches != entityDrawList.end();
		listOfBatches++)
	{
		listOfBatches->bitmap->HoldDrawing(true);
		for(auto batchList = entityDrawList.front().batch.begin(); batchList != entityDrawList.front().batch.end();
			batchList++)
		{
			auto currentBitmap = entityDrawList.front().bitmap;
			batchList->Draw(currentBitmap);
		}

		listOfBatches->bitmap->HoldDrawing(false);
	}

	//Reset drawing translation
	al_identity_transform(&camera);
	al_use_transform(&camera);

}

void EntityBatchRenderer::ClearDrawList()
{
	entityDrawList.clear();
}