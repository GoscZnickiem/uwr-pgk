#include "mainScene.hpp"
#include "core/appdata.hpp"
#include "core/input.hpp"
#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <regex>
#include <thread>


static glm::vec3 project(const glm::vec3& a,const glm::vec3& b) {
	float dotProduct = glm::dot(a, b);
	float lengthSquared = glm::length(b) * glm::length(b);
	return (dotProduct / lengthSquared) * b;
};

static float lenSqared(const glm::vec3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

static constexpr float radians(float angle) {
	return angle * glm::pi<float>()/180;
}

static constexpr float degrees(float angle) {
	return angle * 180/glm::pi<float>();
}

static constexpr float atan(float y, float x) {
	if (x > 0) {
		return std::atan(y / x);
	} else if (x < 0 && y >= 0) {
		return std::atan(y / x) + glm::pi<float>();
	} else if (x < 0 && y < 0) {
		return std::atan(y / x) - glm::pi<float>();
	} else if (x == 0 && y > 0) {
		return glm::pi<float>() / 2;
	} else if (x == 0 && y < 0) {
		return -glm::pi<float>() / 2;
	}
	return 0.0;
}

MainScene::MainScene() {
	std::regex filePattern(R"(([NS])(\d{2})([WE])(\d{3})\.hgt)");
	std::smatch match;

    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(AppData::readDirectory)) {
            if (!entry.is_regular_file()) continue;
			std::string fileName = entry.path().filename().string();
			if (!std::regex_match(fileName, match, filePattern)) continue;
			std::string fullName = entry.path().string();

			const char latSign = match[1].str()[0];
			const int lat = std::stoi(match[2].str());
			const char lonSign = match[3].str()[0];
			const int lon = std::stoi(match[4].str());

			const int y = latSign == 'N' ? lat + 90 : 90 - lat;
			const int x = lonSign == 'E' ? lon + 180 : 180 - lon;

			if(y < AppData::latBounds.first || y > AppData::latBounds.second) continue;
			if(AppData::lonBounds.first <= AppData::lonBounds.second) {
				if(x < AppData::lonBounds.first || x > AppData::lonBounds.second) continue;
			} else {
				if(x < AppData::lonBounds.first && x > AppData::lonBounds.second) continue;
			}

			auto& c = chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
			c.state = HeightMap::State::UNLOADED;
			c.sourceFile = fullName;

			const float sinxc = std::sin(radians(static_cast<float>(x - 180)));
			const float cosxc = std::cos(radians(static_cast<float>(x - 180)));
			const float sinyc = std::sin(radians(static_cast<float>(y - 90)));
			const float cosyc = std::cos(radians(static_cast<float>(y - 90)));

			c.worldPos = earthRadius * glm::vec3{cosyc * cosxc , sinyc, -cosyc * sinxc};
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: unable to access directory: " << e.what() << std::endl;
    }


	chunkUpdater = std::thread{[this]() {
		while(true) {
			int borderLeft; int borderRight; int borderBottom; int borderTop;
			bool is2d;
			glm::vec3 camPos;
			float maxDistanceSquared;
			{
				std::lock_guard<std::mutex> lock(mutex);
				if(!runChunkUpdater) return;
				borderLeft = areaXmin;
				borderRight = areaXmax;
				borderBottom = areaYmin;
				borderTop = areaYmax;
				is2d = view2D;
				camPos = camera.position;
				maxDistanceSquared = horizont * horizont;
			}
			auto dist = [](const HeightMap& a, const HeightMap& b){
				return lenSqared(a.worldPos - b.worldPos);
			};
			if(is2d) {
				chunkRequester(borderLeft, borderRight, borderBottom, borderTop, 
					dist, [](const HeightMap& c){ return c.state == HeightMap::State::UNLOADED; });
			} else {
				chunkRequester(borderLeft, borderRight, borderBottom, borderTop, 
					dist, [&](const HeightMap& c){
						return c.state == HeightMap::State::UNLOADED && lenSqared(camPos - c.worldPos) <= maxDistanceSquared; 
					});
			}
		}
	}};

	cameraPos = {13, 50};
	scale = 0.5f;

	camera.direction = {-1, 0, 0};
	cameraHeight = 50.0f;

	glGenVertexArrays(1, &sphereVao);
}

MainScene::~MainScene() {
	{
		std::lock_guard<std::mutex> lock(mutex);
		runChunkUpdater = false;
	}
	chunkUpdater.join();
	glDeleteVertexArrays(1, &sphereVao);
}

void MainScene::update() {
	{
	std::lock_guard<std::mutex> lock(mutex);
		if(view2D) {
			// camera movement
			const float speed = AppData::deltaT / scale * 1.f;
			if(Input::isKeyPressed("W")) cameraPos.y += speed;
			if(Input::isKeyPressed("S")) cameraPos.y -= speed;
			if(Input::isKeyPressed("A")) { cameraPos.x -= speed; if(cameraPos.x < -180) cameraPos.x += 360; }
			if(Input::isKeyPressed("D")) { cameraPos.x += speed; if(cameraPos.x >= 180) cameraPos.x -= 360; }

			if(Input::isKeyClicked("+") || Input::getScroll() > 0) {
				scale *= 1.25f;
				if(scale > 10.f) scale = 10.f;
			}
			if(Input::isKeyClicked("-") || Input::getScroll() < 0) {
				scale *= 0.8f;
				if(scale < 0.01f) scale = 0.01f;
			}

			// render area
			areaYmin = static_cast<int>( std::floor(cameraPos.y - 1 / scale) ) + 90 - 1;
			if(areaYmin < 0) areaYmin = 0;
			if(areaYmin >= 180) areaYmin = 179;
			areaYmax = static_cast<int>( std::ceil(cameraPos.y + 1 / scale) ) + 90 + 1;
			if(areaYmax < 0) areaYmax = 0;
			if(areaYmax >= 180) areaYmax = 179;

			const float extent = projection == Projection::ORTOGONAL ? 1 :
				1 / std::cos(radians(static_cast<float>(std::max(std::abs(areaYmax - 90), std::abs(areaYmin - 90))) + 0.5f));
			areaXmin = static_cast<int>( std::floor(cameraPos.x - 1 / scale / aspectRatio.x * extent) ) + 180 - 1;
			if(areaXmin < 0) areaXmin = 0;
			if(areaXmin >= 360) areaXmin = 359;
			areaXmax = static_cast<int>( std::ceil(cameraPos.x + 1 / scale / aspectRatio.x * extent) ) + 180 + 1;
			if(areaXmax < 0) areaXmax = 0;
			if(areaXmax >= 360) areaXmax = 359;


			if(Input::isKeyClicked("SPACE")) {
				switch(projection) {
					case Projection::ORTOGONAL: projection = Projection::EQUIRECTANGULAR; break;
					case Projection::EQUIRECTANGULAR: projection = Projection::SIMPLE; break;
					case Projection::SIMPLE: projection = Projection::ORTOGONAL; break;
				}
			}
		} else {
			// camera movement
			const float heightFactor = std::clamp(cameraHeight / (earthRadius * 2), 0.f, 1.f);
			glm::vec3 dir{0, 0, 0};
			auto a = glm::normalize(camera.direction - project(camera.direction, camera.up));
			if(Input::isKeyPressed("W")) { dir += a; }
			if(Input::isKeyPressed("S")) { dir -= a; }
			if(Input::isKeyPressed("A")) { dir += glm::cross(camera.up, a); }
			if(Input::isKeyPressed("D")) { dir -= glm::cross(camera.up, a); }
			if(dir.x != 0 || dir.y != 0 || dir.z != 0) {
				dir = glm::normalize(dir);

				const auto rotAxis = glm::normalize(glm::cross(camera.position, dir));
				const float spdFactor = AppData::deltaT * std::lerp(0.0001f, 1.f, heightFactor);
				const auto rotMat = glm::rotate(glm::mat4(1.f), spdFactor, rotAxis);
				camera.up = glm::normalize(glm::vec3( rotMat * glm::vec4(camera.position, 1.f) )); 
				camera.direction = glm::normalize(glm::vec3( rotMat * glm::vec4(camera.direction, 1.f) ));
			}

			const float vertSpeed = std::lerp(0.1f, 5000.f, heightFactor);
			if(Input::isKeyPressed("+")) {
				cameraHeight += vertSpeed * AppData::deltaT;
			}
			if(Input::getScroll() > 0) {
				cameraHeight += vertSpeed;
			}
			if(Input::isKeyPressed("-")) {
				cameraHeight -= vertSpeed * AppData::deltaT;
			}
			if(Input::getScroll() < 0) {
				cameraHeight -= vertSpeed;
			}
			if(cameraHeight < 0) cameraHeight = 0;
			if(cameraHeight > earthRadius * 2) cameraHeight = earthRadius * 2;

			camera.position = (earthRadius + cameraHeight) * camera.up;
			camera.update();

			const float latitude = degrees(std::asin(camera.up.y));
			const float longitude = -degrees(atan(camera.position.z, camera.position.x));
			cameraPos = {longitude, latitude};

			// render area
			horizont = std::sqrt((earthRadius + cameraHeight) * (earthRadius + cameraHeight) - earthRadius * earthRadius) +
				std::sqrt((earthRadius + 9.f) * (earthRadius + 9.f) - earthRadius * earthRadius);

			const float bonus = std::acos(earthRadius / (earthRadius + 9.f));
			const float tanDist = std::acos(earthRadius / (earthRadius + cameraHeight));
			const float totalDistDeg = degrees(tanDist + bonus);

			areaXmin = static_cast<int>( std::floor(cameraPos.x - totalDistDeg) ) + 180 - 1;
			if(areaXmin < 0) areaXmin += 360;
			areaXmax = static_cast<int>( std::ceil(cameraPos.x + totalDistDeg) ) + 180 + 1;
			if(areaXmax >= 360) areaXmax -= 360;

			areaYmin = static_cast<int>( std::floor(cameraPos.y - totalDistDeg) ) + 90 - 1;
			if(areaYmin < 0) {
				areaYmin = 0;
				areaXmin = 0;
				areaXmax = 359;
			}
			if(areaYmin >= 180) areaYmin = 179;
			areaYmax = static_cast<int>( std::ceil(cameraPos.y + totalDistDeg) ) + 90 + 1;
			if(areaYmax < 0) areaYmax = 0;
			if(areaYmax >= 180) {
				areaYmax = 179;
				areaXmin = 0;
				areaXmax = 359;
			}

			if(cameraHeight > 15.f) {
				camera.nearPlane = 1.f;
				camera.farPlane = 500000.f;
			} else {
				camera.nearPlane = 0.01f;
				camera.farPlane = 10000.f;
			}
		}
	}

	// LOD control (kontrola nad lodem)
	if(Input::isKeyPressed("0")) AppData::lod = 0;
	if(Input::isKeyPressed("1")) AppData::lod = 1;
	if(Input::isKeyPressed("2")) AppData::lod = 2;
	if(Input::isKeyPressed("3")) AppData::lod = 3;
	if(Input::isKeyPressed("4")) AppData::lod = 4;
	if(Input::isKeyPressed("5")) AppData::lod = 5;
	if(Input::isKeyPressed("6")) AppData::lod = 6;
	if(Input::isKeyPressed("7")) AppData::lod = 7;
	if(Input::isKeyPressed("8")) AppData::lod = 8;
	if(Input::isKeyPressed("9")) AppData::lod = 9;

	if(Input::isKeyClicked("TAB")) {
		Input::setMousePosLock(view2D);
		const float sinx = std::sin(radians(cameraPos.x));
		const float cosx = std::cos(radians(cameraPos.x));
		const float siny = std::sin(radians(cameraPos.y));
		const float cosy = std::cos(radians(cameraPos.y));
		auto dirFromCenter = glm::vec3{cosy * cosx, siny, -cosy * sinx};
		camera.direction = glm::normalize(-dirFromCenter + (std::abs(dirFromCenter.y) == 1 ? glm::vec3{0,0,1} : glm::vec3{0,1,0}));
		camera.up = dirFromCenter;
		camera.position = dirFromCenter * (earthRadius + cameraHeight);
		view2D = !view2D;
	}
}

void MainScene::render() {
	const Shader* shader;
	if(view2D) {
		switch(projection) {
			case Projection::ORTOGONAL: shader = &AppData::Data().shaders.map2Dort; break;
			case Projection::EQUIRECTANGULAR: shader = &AppData::Data().shaders.map2Dequirect; break;
			case Projection::SIMPLE: shader = &AppData::Data().shaders.map2Dsimple; break;
		}
		shader->bind();
		shader->setUniform("cameraPos", cameraPos);
		shader->setUniform("scale", aspectRatio * scale);
	} else {
		const auto& globeShader = &AppData::Data().shaders.globe;
		globeShader->bind();
		globeShader->setUniform("camInverse", glm::inverse(camera.getProjectionMatrix() * camera.getViewMatrix()));
		globeShader->setUniform("camPos", camera.position);
		auto[winx, winy] = AppData::Data().window.getWindowSize();
		globeShader->setUniform("resolution", winx, winy);
		glBindVertexArray(sphereVao);
		glDepthMask(GL_FALSE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDepthMask(GL_TRUE);
		glBindVertexArray(0);

		shader = &AppData::Data().shaders.map3D;
		shader->bind();
		shader->setUniform("projection", camera.getProjectionMatrix());
		shader->setUniform("view", camera.getViewMatrix());
	}
	for(int x = areaXmin; x != areaXmax; x++) {
		if(x == 360) { x = -1; continue; }
		for(int y = areaYmin; y != areaYmax; y++) {
			auto& c = chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
			if(c.state != HeightMap::State::LOADED) continue;
			shader->setUniform("position", x-180, y-90);
			c.render();
		}
	}

	HeightMap::GenerateRequestedBuffers();
	HeightMap::UnloadRequestedMaps();
	HeightMap::LoadRequestedMaps();
}

void MainScene::atResize(int width, int height) {
	aspectRatio = {static_cast<float>(height)/static_cast<float>(width), 1};
	camera.aspectRatio = 1/aspectRatio.x;
}


void MainScene::chunkRequester(int borderLeft, int borderRight, int borderBottom, int borderTop,
					 std::function<float(const HeightMap&, const HeightMap&)> dist,
					 std::function<bool(const HeightMap&)> pred) {
	
	const std::size_t centerX = static_cast<std::size_t>(borderLeft + borderRight) / 2;
    const std::size_t centerY = static_cast<std::size_t>(borderBottom + borderTop) / 2;
	const auto& center = chunks[centerX][centerY];

	static std::vector<std::pair<float, std::pair<int, int>>> chunkDistances;
	chunkDistances.clear();

	for (int x = borderLeft; x <= borderRight; x++) {
		if(x == 360) { x = -1; continue; }
        for (int y = borderBottom; y <= borderTop; y++) {
			auto& c = chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
            if (pred(c)) {
                const float distance = dist(c, center);
                chunkDistances.emplace_back(distance, std::make_pair(x, y));
            }
        }
    }

	std::sort(chunkDistances.begin(), chunkDistances.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

	for (const auto& chunkInfo : chunkDistances) {
        const std::size_t x = static_cast<std::size_t>(chunkInfo.second.first);
        const std::size_t y = static_cast<std::size_t>(chunkInfo.second.second);
        chunks[x][y].requestLoad();
    }

	for (int x = 0; x < 360; x++) {
        for (int y = 0; y < 180; y++) {
			auto& c = chunks[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
			if(borderLeft <= borderRight) {
				if(x < borderLeft || x > borderRight || y < borderBottom || y > borderTop)
					c.requestUnload();
			}
			else if ((x < borderLeft && x > borderRight) || y < borderBottom || y > borderTop)
				c.requestUnload();
        }
    }
}
