#include "Pipe.h"

void Pipe::Awake()
{
	SetTag(ObjectTags::Solid);

	Collider2D* box = new Collider2D();
	box->AttachToEntity(this);
	this->colliders->push_back(box);
	this->rigidbody->SetDynamic(false);
	DebugOut(L"Pipe\n");
}

void Pipe::Start()
{
	renderOrder = -1;
}

void Pipe::PreRender()
{
}

void Pipe::Render(Vector2 translation)
{
	int size = PIPE_DEFAULT_TILE_SIZE;
	auto pos = transform.Position;
	switch (direction)
	{
	case PipeDirection::Up:
	case PipeDirection::Down:
	case PipeDirection::Vertical:
	{
		if (direction != PipeDirection::Vertical)
		{
			auto headOffset = direction == PipeDirection::Up ? 0 : (height - 1) * size;
			tileset->SetSourceRect(headSrc[0].x, headSrc[0].y, size, size);
			tileset->Draw(pos.x + translation.x, pos.y + headOffset + translation.y);

			tileset->SetSourceRect(headSrc[1].x, headSrc[1].y, size, size);
			tileset->Draw(pos.x + size + translation.x, pos.y + headOffset + translation.y);
		}

		int startY = direction == PipeDirection::Up ? 1 : 0;
		int endY = direction == PipeDirection::Down ? height - 1 : height;
		for (int y = startY; y < endY; ++y)
		{
			tileset->SetSourceRect(bodySrc[0].x, bodySrc[0].y, size, size);
			tileset->Draw(pos.x + translation.x, pos.y + size * y + translation.y);

			tileset->SetSourceRect(bodySrc[1].x, bodySrc[1].y, size, size);
			tileset->Draw(pos.x + size + translation.x, pos.y + size * y + translation.y);
		}
	}
	break;
	}
}

void Pipe::SetHeadSrc(IntPoint id0, IntPoint id1)
{
	headSrc[0] = id0;
	headSrc[1] = id1;
}

void Pipe::SetBodySrc(IntPoint id0, IntPoint id1)
{
	bodySrc[0] = id0;
	bodySrc[1] = id1;
}

void Pipe::SetTileset(Tile tileset)
{
	this->tileset = tileset;
}

void Pipe::SetSize(int w, int h)
{
	this->width = w / PIPE_DEFAULT_TILE_SIZE;
	this->height = h / PIPE_DEFAULT_TILE_SIZE;
	DebugOut(L"Pipe: %d, %d\n", width, height);
	colliders->at(0)->SetBoxSize(Vector2(w, h));
	colliders->at(0)->SetLocalPosition(Vector2(w * 0.5f, h * 0.5f));
}

void Pipe::SetDirection(PipeDirection direction)
{
	this->direction = direction;
}
