#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "../Storage.hpp"
#include "Player.hpp"

using namespace std;
using namespace TlMain;
using namespace TlConsole;

using json = nlohmann::json;

// using ChunkBlocksDatas = variant<int, float, string>;

class Chunk {
public:
    static constexpr int CHUNK_WIDTH = 16;
    static constexpr int CHUNK_HEIGHT = 16;

    //int data[CHUNK_WIDTH][CHUNK_HEIGHT];
    // Чанк, в котором содержаться блоки со своими данными
    // unordered_map<string, ChunkBlocksDatas> data[CHUNK_WIDTH][CHUNK_HEIGHT];
    json data[CHUNK_WIDTH][CHUNK_HEIGHT];
    sf::Vector2i chunkPosition;
    Chunk() {}
    Chunk(const sf::Vector2i& cP) {
        chunkPosition = cP;
        for(int x = 0; x < CHUNK_WIDTH; x++) {
            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                //data[x][y] = getRandom(0,3);
                data[x][y]["type"] = "grass";
                sf::Vector2i currentChunkBlockPosition = {x + (chunkPosition.x * CHUNK_WIDTH), y + (chunkPosition.y * CHUNK_HEIGHT)};
                /*int noise = (1 + noise2(currentChunkBlockPosition.x * 0.05, currentChunkBlockPosition.y * 0.05)) * 2;
                noise = floor(noise);
                int color[] = {0,0,0};
                switch (noise) {
                    case 1: color[0] = 255; break;
                    case 2: color[1] = 255; break;
                    case 3: color[2] = 255; break;
                    default: color[0] = 31; color[1] = 31; color[2] = 31; break;
                }
                json colorjson = {
                    color[0], color[1], color[2]
                };*/
                int noise = noise2(currentChunkBlockPosition.x * 0.05, currentChunkBlockPosition.y * 0.05) * 255;
                noise = sin(noise/64.)*255.;
                //noise = tan(noise/(sin(noise/128.)*200.))*255.;
                //noise = sin(noise/static_cast<float>(x*16.))*255.;
                noise = floor(noise/50.)*50.;
                json colorjson = {
                    noise, noise, noise
                };
                data[x][y]["color"] = colorjson;
            }
        }
    }

    void chunkRender(SiSFML& SiSFML, Player& mainPlayer) {
        for(int renderX = 0; renderX < CHUNK_WIDTH; renderX++) {
            for(int renderY = 0; renderY < CHUNK_HEIGHT; renderY++) {
                sf::Vector2i currentChunkBlockPosition = {renderX + (chunkPosition.x * CHUNK_WIDTH), renderY + (chunkPosition.y * CHUNK_HEIGHT)}; // Координата блока не в конкретном чанке (0~16), а в мире
                if(!data[renderX][renderY]["type"].empty()) { // Проверка, установлен ли блок по этим координатам, иначе это будет считаться воздухом
                    sf::RectangleShape rect = SiSFML.srect({renderX*mainPlayer.fieldOfView, renderY*mainPlayer.fieldOfView}, {mainPlayer.fieldOfView, mainPlayer.fieldOfView});
                    rect.setFillColor(sf::Color(
                        data[renderX][renderY]["color"][0],
                        data[renderX][renderY]["color"][1],
                        data[renderX][renderY]["color"][2]
                    ));
                    rect.setPosition(
                        rect.getPosition() + sf::Vector2f(
                            chunkPosition.x * (mainPlayer.fieldOfView * CHUNK_WIDTH),
                            -(chunkPosition.y * (mainPlayer.fieldOfView * CHUNK_HEIGHT))
                        )
                    );
                    SiSFML.win.draw(rect);
                }
            }
        }
    }
};

#endif
