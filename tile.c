/* $Id$ */

#include "stdafx.h"
#include "tile.h"

void SetMapExtraBits(TileIndex tile, byte bits)
{
	assert(tile < MapSize());
	SB(_m[tile].extra, 0, 2, bits & 3);
}

uint GetMapExtraBits(TileIndex tile)
{
	assert(tile < MapSize());
	return GB(_m[tile].extra, 0, 2);
}

/** Converts the heights of 4 corners into a tileh, and returns the minimum height of the tile
  * @param n,w,e,s the four corners
  * @param h uint pointer to write the height to
  * @return the tileh
*/
uint GetTileh(uint n, uint w, uint e, uint s, uint *h)
{
	uint min = n;
	uint r;

	if (min >= w) min = w;
	if (min >= e) min = e;
	if (min >= s) min = s;

	r = 0;
	if ((n -= min) != 0) r += (--n << 4) + 8;
	if ((e -= min) != 0) r += (--e << 4) + 4;
	if ((s -= min) != 0) r += (--s << 4) + 2;
	if ((w -= min) != 0) r += (--w << 4) + 1;

	if (h != NULL) *h = min * 8;

	return r;
}

uint GetTileSlope(TileIndex tile, uint *h)
{
	uint a;
	uint b;
	uint c;
	uint d;

	assert(tile < MapSize());

	if (TileX(tile) == MapMaxX() || TileY(tile) == MapMaxY()) {
		if (h != NULL) *h = 0;
		return 0;
	}

	a = TileHeight(tile);
	b = TileHeight(tile + TileDiffXY(1, 0));
	c = TileHeight(tile + TileDiffXY(0, 1));
	d = TileHeight(tile + TileDiffXY(1, 1));

	return GetTileh(a, b, c, d, h);
}

uint GetTileZ(TileIndex tile)
{
	uint h;
	GetTileSlope(tile, &h);
	return h;
}

uint GetTileMaxZ(TileIndex t)
{
	uint max;
	uint h;

	h = TileHeight(t);
	max = h;
	h = TileHeight(t + TileDiffXY(1, 0));
	if (h > max) max = h;
	h = TileHeight(t + TileDiffXY(0, 1));
	if (h > max) max = h;
	h = TileHeight(t + TileDiffXY(1, 1));
	if (h > max) max = h;
	return max * 8;
}
