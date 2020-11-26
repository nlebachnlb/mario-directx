#pragma once
#include "GameObject.h"
#include "Tile.h"

const int PIPE_DEFAULT_TILE_SIZE = 48;

enum class PipeDirection
{
	Left, Right, Up, Down, Horizontal, Vertical
};

class Pipe : public CGameObject
{
public:
	void Awake() override;
	void Start() override;
	void PreRender() override;
	void Render(Vector2 translation) override;

	void SetHeadSrc(IntPoint src0, IntPoint src1);
	void SetBodySrc(IntPoint src0, IntPoint src1);
	void SetTileset(Tile tileset);

	void SetSize(int w, int h);

protected:
	IntPoint headSrc[2], bodySrc[2];
	Tile tileset;
	PipeDirection direction;
	int width, height;
};

