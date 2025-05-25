#include "../Storage.hpp"

using namespace std;
using namespace TlMain;
using namespace TlConsole;
using namespace TlFile;

using json = nlohmann::json;

#include "Player.hpp"
#include "ChatManager.hpp"
#include "Chunk.hpp"

/*
    Стандарты:
    - Размер одного блока и игрока {1,1}, рендер должен будет растягивать это с учетом видимости камеры
    - Ходьба игрока = 1 блок - 0.5 секунда
    - Бег игрока = 1 блок - 0.75 секунда
*/

struct sfVector2iHash {
    size_t operator()(const sf::Vector2i& v) const {
        return hash<int>()(v.x) ^ (hash<int>()(v.y) << 1);
    }
};

class CppWld : public SiSFML {
public:
    CppWld()
        : SiSFML("CppWldWin", 1280, 720) {
        setWindowName("CppWld");
    }

    Player mainPlayer;

    sf::View camera;
    sf::Vector2f cameraCenter;

    void setup() {
        initImGui();
        valuesUpdate();
        mainPlayer.setNickname("NoName24");

        cameraCenter = sf::Vector2f(display.x / 2.f, display.y / 2.f);

        camera.setSize(sf::Vector2f(1280.f, 720.f));
        camera.setCenter(mainPlayer.position);
        win.setView(camera);

        //chunks[sf::Vector2i(0,0)] = Chunk({0,0});
    }

    void debugInfo() {
        ImGui::Begin("Debug Info");
        if(ImGui::TreeNode("Main")) {
            ImGui::Text("RAM Usage: %d", getMemoryUsage() / 1024);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Window")) {
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Window Name: %s", windowName.c_str());
            ImGui::Text("Window Event: %i", windowEvent);
            ImGui::Text("Frame Count: %i", frameCount);
            ImGui::Text("Display: %i x %i", static_cast<int>(display.x), static_cast<int>(display.y));
            ImGui::Text("Mouse Position: %i x %i", sf::Mouse::getPosition(win).x, sf::Mouse::getPosition(win).y);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("Player")) {
            ImGui::Text("Nickname: %s", mainPlayer.getNickname().c_str());
            ImGui::Text("Position: {%.2f, %.2f}", static_cast<float>(mainPlayer.position.x), static_cast<float>(mainPlayer.position.y));
            ImGui::Text("positionChunk: {%i, %i}", static_cast<int>(mainPlayer.positionChunk.x), static_cast<int>(mainPlayer.positionChunk.y));
            ImGui::TreePop();
        }
        ImGui::End();
    }

    // WORLD
    unordered_map<sf::Vector2i, Chunk, sfVector2iHash> chunks;
    vector<sf::Vector2i> chunksGetAllKeys(const unordered_map<sf::Vector2i, Chunk, sfVector2iHash>& chunks) {
        vector<sf::Vector2i> keys;
        keys.reserve(chunks.size());
        for(const auto& pair : chunks) { keys.push_back(pair.first); }
        return keys;
    }
protected:
    void render() override {
        mainPlayer.controllerTick();

        camera.setSize(to_Vector2f(display));
        sf::Vector2f targetPosition = {
            (display.x / 2.f) + (mainPlayer.position.x * mainPlayer.fieldOfView),
            (display.y / 2.f) - (mainPlayer.position.y * mainPlayer.fieldOfView)
        };
        float smoothness = 0.1f;
        cameraCenter += (targetPosition - cameraCenter) * smoothness;
        camera.setCenter(cameraCenter);
        win.setView(camera);

        // Отрисовка чанков
        /*std::vector<sf::Vector2i> renderChunk = chunksGetAllKeys(chunks);
        for(int renderChunkIndex = 0; renderChunkIndex < renderChunk.size(); renderChunkIndex++) {
            chunks[renderChunk[renderChunkIndex]].chunkRender(*this, mainPlayer);
        }*/
        for(int renderX = mainPlayer.positionChunk.x-(SSG_RenderChunkViewDistance/2); renderX <= mainPlayer.positionChunk.x+(SSG_RenderChunkViewDistance/2); renderX++) {
            for(int renderY = mainPlayer.positionChunk.y-(SSG_RenderChunkViewDistance/2); renderY <= mainPlayer.positionChunk.y+(SSG_RenderChunkViewDistance/2); renderY++) {
                if(chunks.find({renderX, renderY}) != chunks.end()) {
                    chunks[{renderX, renderY}].chunkRender(*this, mainPlayer);
                }
            }
        }

        // Отрисовка игрока
        win.draw(srect(mainPlayer.position * mainPlayer.fieldOfView, {mainPlayer.fieldOfView, mainPlayer.fieldOfView}));

        // Обнаружение пересечения нового чанка
        if( static_cast<int>(mainPlayer.position.x/16) != mainPlayer.positionChunk.x || static_cast<int>(mainPlayer.position.y/16) != mainPlayer.positionChunk.y ) {
            mainPlayer.positionChunk = to_Vector2i(mainPlayer.position)/16;
            for(int cx = -SSG_RenderChunkViewDistance/2; cx <= SSG_RenderChunkViewDistance/2; cx++) {
                for(int cy = -SSG_RenderChunkViewDistance/2; cy <= SSG_RenderChunkViewDistance/2; cy++) {
                    sf::Vector2i ca = {cx, cy};
                    if(chunks.find(mainPlayer.positionChunk+ca) == chunks.end()) {
                        println(1, "Новый чанк не найден в массиве чанков, создается новый / x"+to_string(mainPlayer.positionChunk.x+ca.x)+" y"+to_string(mainPlayer.positionChunk.y+ca.y));
                        chunks[mainPlayer.positionChunk+ca] = Chunk(mainPlayer.positionChunk+ca);
                    }
                }
            }
        }

        mainPlayer.getChat()->renderChat();

        debugInfo();
    }
    void mouseTouchEvent(const sf::Mouse::Button& buttonType, const sf::Vector2i& mousePressPosition) override {}
    void keyboardEvent(sf::Keyboard::Key keyCode, bool pressed) override {
        if(mainPlayer.getChat()->chatFocus) {
            if(!pressed) {
                mainPlayer.controller(keyCode, pressed);
            }
        } else {
            mainPlayer.controller(keyCode, pressed);
        }
        mainPlayer.getChat()->keyEvent(keyCode, pressed);
    }
};
