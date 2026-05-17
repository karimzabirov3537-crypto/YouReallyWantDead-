#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

bool g_isNoclipActive = false;
float g_noclipTimer = 0.0f;

class $modify(MyPlayLayer, PlayLayer) {
    // Безопасный игровой цикл для отсчета времени
    void update(float dt) {
        PlayLayer::update(dt);

        if (g_isNoclipActive) {
            g_noclipTimer -= dt;
            if (g_noclipTimer <= 0.0f) {
                g_isNoclipActive = false;
            }
        }
    }

    // Хукаем продолжение игры после выхода из паузы
    void resumeAndStopPause() {
        PlayLayer::resumeAndStopPause();
        
        // Если пауза была вызвана нашей виртуальной "смертью" — даем ноуклип
        if (g_noclipTimer == -1.0f) {
            g_isNoclipActive = true;
            g_noclipTimer = 5.0f; // 5 секунд бессмертия
        }
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        if (g_isNoclipActive) return;

        // Вместо смерти принудительно прожимаем стандартную паузу GD
        if (g_noclipTimer <= 0.0f) {
            g_noclipTimer = -1.0f; // Ставим маркер "второго шанса"
            this->pushButton(0, true); // Вызов стандартного меню паузы игры
            return;
        }

        PlayLayer::destroyPlayer(p0, p1);
    }
};
