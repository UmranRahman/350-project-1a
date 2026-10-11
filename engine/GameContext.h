#ifndef GAMECONTEXT_H
#define GAMECONTEXT_H

#include "GameObject.h"
#include <memory>

namespace CMPUT350 {

class EngineView;
class DrawContext;
class NotificationManager;
class GameContext {
public:
    EngineView *mEngineView;
    DrawContext *ScreenContext;
    DrawContext *GUIContext;
	std::weak_ptr<GameObject> CurrObject;
	NotificationManager *mNotificationManager;
	int currentFrame;
};

}  // namespace CMPUT350

#endif  // GAMECONTEXT_H
