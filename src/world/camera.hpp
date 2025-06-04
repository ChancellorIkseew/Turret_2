#pragma once
#include "coords/pixel_coord.hpp"
#include "coords/tile_coord.hpp"
#include "window/window.hpp"

class Camera
{
private:
	//sf::View cameraView = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f));
	PixelCoord windowSize;
	PixelCoord movingStartMouseCoord;
	PixelCoord pixelMapSize;
	TileCoord tileMapSize;

	float mapScale;

	bool isMooving = false;

	void moveByMouse();
	void moveByWASD();
	void avoidEscapeFromMap();
	void scale();
	void resize(const MainWindow& window);
	void updateMapRegion(const MainWindow& window);

	TileCoord buildingsStartTile;
	TileCoord startTile;
	TileCoord endTile;

public:
	Camera(const TileCoord mapSize);
	~Camera() = default;

	void interact(const MainWindow& window);

	///@brief applies correction for building max size
	TileCoord getBuildingsStartTile() const { return buildingsStartTile; }
	TileCoord getStartTile() const { return startTile; }
	TileCoord getEndTile() const { return endTile; }
	//const sf::View& getView() const { return cameraView; }
	float getMapScale() const { return mapScale; }

	inline bool contains(const TileCoord tile) const {
		return	tile.x >= startTile.x && tile.x <= endTile.x &&
				tile.y >= startTile.y && tile.y <= endTile.y;
	}

};