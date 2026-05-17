// ХАК-ЗАГЛУШКА ДЛЯ FMT: Отключаем проверки строк до подключения Geode
#define FMT_ENFORCE_COMPILE_STRING_CHECK 0
#define FMT_USE_CONSTEXPR 0
#define FMT_NO_FMT_UTC_TIMESTAMP 1

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

bool g_isNoclipActive = false;

class $modify(MyPlayLayer, PlayLayer) {
    // Хукаем выход из паузы
    void resumeAndStopPause() {
        PlayLayer::resumeAndStopPause();
        // Включаем ноуклип, если сняли игру с паузы
        g_isNoclipActive = true;
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        // Если ноуклип уже включен — летим сквозь блоки дальше
        if (g_isNoclipActive) return;

        // Вместо смерти принудительно открываем стандартное меню паузы GD!
        if (!g_isNoclipActive) {
            this->pushButton(0, true); // Прожимает кнопку паузы
            return;
        }

        PlayLayer::destroyPlayer(p0, p1);
    }
};
