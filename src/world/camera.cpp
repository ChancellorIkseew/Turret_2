#include "camera.hpp"
//
#include "coords/transforms.hpp"
#include "window/input/input.hpp"

constexpr float MIN_MAP_SCALE = 0.5f, MAX_MAP_SCALE = 5.0f;
constexpr float SCALE_FACTOR = 1.2f;
constexpr float MOTION_SPEED_MODIFIER = 20.0f;
constexpr TileCoord MAX_MAP_STRUCTURE_SIZE(6, 6);

Camera::Camera(const TileCoord mapSize) :
	windowSize(1024, 720), mapScale(MIN_MAP_SCALE), startTile(0, 0), endTile(0, 0),
	tileMapSize(mapSize), pixelMapSize(t1::pixel(mapSize)) { }

void Camera::interact(const MainWindow& mainWindow) {
	resize(mainWindow);
	scale();
	moveByMouse();
	moveByWASD();
	avoidEscapeFromMap();
	updateMapRegion(mainWindow);
}

void Camera::moveByMouse() {
	if (Input::jactive(BindName::MidMB)) {
		movingStartMouseCoord = fromScreenToMap(Input::getMouseCoord());
		return;
	}

	if (Input::active(BindName::MidMB)) {
		const PixelCoord delta = movingStartMouseCoord - fromScreenToMap(Input::getMouseCoord());
		cameraCoord = cameraCoord + delta;
	}
}

void Camera::moveByWASD() {
	PixelCoord delta(0.0f, 0.0f);

	if (Input::active(BindName::Move_up))
		delta.y -= 1.0f;
	if (Input::active(BindName::Move_left))
		delta.x -= 1.0f;
	if (Input::active(BindName::Move_down))
		delta.y += 1.0f;
	if (Input::active(BindName::Move_right))
		delta.x += 1.0f;

	if (delta != PixelCoord(0.0f, 0.0f))
		cameraCoord = cameraCoord + (delta * MOTION_SPEED_MODIFIER * mapScale);
}

void Camera::avoidEscapeFromMap() {
	/*
	sf::Vector2f viewCenter = cameraView.getCenter();
	viewCenter.x = std::clamp(viewCenter.x, 0.0f, pixelMapSize.x);
	viewCenter.y = std::clamp(viewCenter.y, 0.0f, pixelMapSize.y);
	cameraView.setCenter(viewCenter);
	*/
}

void Camera::scale() {
	switch (Input::getMouseWheelScroll())
	{
	case MouseWheelScroll::none:
		return;
	case MouseWheelScroll::up:
		if (mapScale >= MIN_MAP_SCALE)
			mapScale /= SCALE_FACTOR;
		break;
	case MouseWheelScroll::down:
		if (mapScale <= MAX_MAP_SCALE)
			mapScale *= SCALE_FACTOR;
		break;
	}
	//cameraView.setSize(windowSize * mapScale);
}


void Camera::resize(const MainWindow& mainWindow) {
	if (windowSize == mainWindow.getSize())
		return;
	windowSize = mainWindow.getSize();
}


void Camera::updateMapRegion(const MainWindow& mainWindow) {
	endTile = t1::tile(fromScreenToMap(mainWindow.getSize()));
	startTile = t1::tile(fromScreenToMap(PixelCoord(0.0f, 0.0f)));
	buildingsStartTile = startTile - MAX_MAP_STRUCTURE_SIZE;
	// Correction is needed to correct big_buildings drawing.

	if (startTile.x < 0)
		startTile.x = 0;
	if (startTile.y < 0)
		startTile.y = 0;

	if (buildingsStartTile.x < 0)
		buildingsStartTile.x = 0;
	if (buildingsStartTile.y < 0)
		buildingsStartTile.y = 0;

	if (endTile.x > tileMapSize.x)
		endTile.x = tileMapSize.x;
	if (endTile.y > tileMapSize.y)
		endTile.y = tileMapSize.y;
}

PixelCoord Camera::fromMapToScreen(const PixelCoord mapCoord) const {
	return (mapCoord - cameraCoord) / mapScale;
}
PixelCoord Camera::fromScreenToMap(const PixelCoord screenCoord) const {
	return screenCoord * mapScale + cameraCoord;
}