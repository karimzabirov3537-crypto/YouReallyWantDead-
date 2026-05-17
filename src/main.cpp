#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

bool g_isNoclipActive = false;
bool g_isWaitingForDecision = false;

class $modify(MyPlayLayer, PlayLayer) {
    // Кастомный метод для отключения ноуклипа через 5 секунд
    void disableNoclip(float dt) {
        g_isNoclipActive = false;
        Notification::create("Noclip deactivated!", NotificationIcon::Warning)->show();
    }

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {
        // Если ноуклип уже запущен — просто игнорируем урон и летим сквозь блоки
        if (g_isNoclipActive) return;
        if (g_isWaitingForDecision) return;
        g_isWaitingForDecision = true;

        // Показываем официальное быстрое окно Geode v3 с кнопками Да/Нет
        geode::createQuickPopup(
            "Are you sure?",             // Заголовок окна
            "Do you really want to die?", // Текст вопроса
            "No", "Yes",                 // Кнопки (btn1 = No, btn2 = Yes)
            [this](auto, bool btn2) {
                if (btn2) {
                    // Игрок выбрал "Yes" -> даем ноуклип
                    g_isNoclipActive = true;
                    Notification::create("Saved! Noclip active!", NotificationIcon::Success)->show();

                    // Запускаем встроенный таймер Cocos2d на 5 секунд
                    auto scheduler = cocos2d::CCDirector::sharedDirector()->getScheduler();
                    scheduler->scheduleSelector(
                        schedule_selector(MyPlayLayer::disableNoclip), 
                        this, 
                        0.0f, 0, 5.0f, false
                    );
                } else {
                    // Игрок выбрал "No" -> сбрасываем щиты и позволяем ему умереть
                    g_isNoclipActive = false;
                    g_isWaitingForDecision = false;
                    this->destroyPlayer(this->m_player1, nullptr);
                }
                g_isWaitingForDecision = false;
            }
        );
    }
};
