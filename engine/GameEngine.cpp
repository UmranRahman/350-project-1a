#include "GameEngine.h"

#include "GameObject.h"
#include <algorithm>
#include <optional>
#include <cstdio>

/// @brief
namespace CMPUT350 {
#include "FontData.h"



GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name) {
    mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode({width, height}), name);
    mWindow->setFramerateLimit(30);
    
    mWindow->setKeyRepeatEnabled(false);

    mFont = std::make_shared<sf::Font>();
    
    // Sample font loading code
    if (!mFont->openFromMemory(&_font, _font_len)) {
        fprintf(stderr, "WARNING: Font did not load.\n");
    }

    mDrawContext = std::make_unique<DrawContext>(mWindow, mFont);
    mGUIDrawContext = std::make_unique<DrawContext>(mWindow, mFont);


}

GameEngine::~GameEngine() {
    // Cleanup resources
    mWindow->close();
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
    mPendingObjects.push_back(gameObject);
}

/**
 * @brief Handles window-close and key events; key presses are passed to every object. 
 * @param context.
 * AI-ASSISTED: skeleton | AI did: gave the structure with blanks, pointed out the first/second bug and the wrong-map bug | I did: filled the rest
 * */
void GameEngine::ProcessEvents(GameContext *context) {
    while (const std::optional event = mWindow->pollEvent())
    {
        if (event->is<sf::Event::Closed>()) {
            mWindow->close();
        }
        else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            auto iter = mKeyDownNotifications.find(static_cast<int>(keyPressed->scancode));
            if (iter != mKeyDownNotifications.end()) {
                for (const auto &message : iter->second) {
                    mNotificationManager.Notify(message);
                }
            }
        }
        else if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            auto iter = mKeyUpNotifications.find(static_cast<int>(keyReleased->scancode));
            if (iter != mKeyUpNotifications.end()) {
                for (const auto &message : iter->second) {
                    mNotificationManager.Notify(message);
                }
            }
        }
    }
}

/**
 * @brief   Returns an object's bounding box in screen space.
 * @param   obj The collision object. Its GetBounds() is in local space.
 * @return  The axis-aligned box after applying the object's location and rotation.
 *          Leaves that object's offset and rotation set on the screen DrawContext.
 * AI-ASSISTED: skeleton | AI did: described the per-object transform and gave the skeleton with one blank, wrote this comment text | I did: filled the blank
 */
Rect GameEngine::WorldBounds(const std::shared_ptr<CollisionObject> &obj) {
    mDrawContext->SetContextOffset(obj->GetLocation());
    mDrawContext->SetContextRotation(obj->GetRotation());
    return mDrawContext->Transform (obj->GetBounds());
}

/**
 * @brief   Returns an object's collision shapes converted to screen space.
 * @param   obj The collision object. Its GetShapes() are in local space.
 * @return  Copies of the shapes after applying the object's location and rotation.
 *          Circles keep their radius. Rects become the bounding box of the rotated rect (approximation).
 *          Leaves that object's offset and rotation set on the screen DrawContext.
 * AI-ASSISTED: skeleton | AI did: explained the per-shape conversion, gave the skeleton and the union-tag check, wrote this comment text | I did: filled the blanks
 */
std::vector<Shape> GameEngine::WorldShapes(const std::shared_ptr<CollisionObject> &obj) {
    mDrawContext->SetContextOffset(obj->GetLocation());
    mDrawContext->SetContextRotation(obj->GetRotation());

    std::vector<Shape> result;
    for (const Shape &s : obj->GetShapes()) {
        if (s.t == ShapeType::kCircle) {
            Circle c = s.shape.circle;
            result.push_back(Circle(mDrawContext->Transform(c.center), c.radius));
        } else if (s.t == ShapeType::kLine) {
            Line l = s.shape.line;
            result.push_back(Line(mDrawContext->Transform(l.p1), mDrawContext->Transform(l.p2)));
        } else {
            Rect r = s.shape.rect;
            result.push_back(mDrawContext->Transform(r));
        }
    }
    return result;
}

// lineOwner: 0 = no line involved, 1 = a is the line, 2 = b is the line
static bool ShapesCollide(const Shape &a, const Shape &b, Point2D &hit, int &lineOwner) {
    lineOwner = 0;
    if (a.t == ShapeType::kLine) {
        lineOwner = 1;
        if (b.t == ShapeType::kCircle) return LineCircle(a.shape.line, b.shape.circle, hit);
        if (b.t == ShapeType::kRect)   return LineRect(a.shape.line, b.shape.rect, hit);
        return a.shape.line.Crosses(b.shape.line, hit);
    }
    if (b.t == ShapeType::kLine) {
        lineOwner = 2;
        if (a.t == ShapeType::kCircle) return LineCircle(b.shape.line, a.shape.circle, hit);
        return LineRect(b.shape.line, a.shape.rect, hit);
    }
    if (a.t == ShapeType::kCircle && b.t == ShapeType::kCircle) return CircleCircle(a.shape.circle, b.shape.circle);
    if (a.t == ShapeType::kCircle) return CircleRect(a.shape.circle, b.shape.rect);
    if (b.t == ShapeType::kCircle) return CircleRect(b.shape.circle, a.shape.rect);
    return RectRect(a.shape.rect, b.shape.rect);
}

/**
 * @brief   Finds colliding pairs of collision objects and notifies them.
 * @param   Unused game context.
 * Each pair is compared once (j > i). The broad phase compares screen-space bounding boxes with Overlaps().
 * Shape-level tests are added in the narrow phase.
 * AI-ASSISTED: provided | AI did: gave the pair loop with j = i + 1 and the copied-bounds idea (1a) | I did: typed and adapted it, added the WorldBounds call
 */
void GameEngine::ProcessCollisions(GameContext *) {
    std::vector<std::shared_ptr<CollisionObject>> collided;
    for (auto &object: mGameObjects){
        auto ball = std::dynamic_pointer_cast<CollisionObject>(object);
        if (ball) {
            collided.push_back(ball);
        }
    }
    for (size_t i = 0; i < collided.size(); ++i) {
        for (size_t j = i+1; j < collided.size(); ++j) {
            Rect a = WorldBounds(collided[i]);
            Rect b = WorldBounds(collided[j]);
        if (Overlaps(a, b)) {
            std::vector<Shape> shapesA = WorldShapes(collided[i]);
            std::vector<Shape> shapesB = WorldShapes(collided[j]);

            bool found = false;
            Point2D hit;
            int lineOwner = 0; //if 1 belongs to collided[i], if 2 then collided[j]
            for (size_t p = 0; p < shapesA.size() && !found; ++p) {
                for (size_t q = 0; q < shapesB.size() && !found; ++q) {
                    found = ShapesCollide(shapesA[p], shapesB[q], hit, lineOwner);
                }
            }
            if (found) {
                if (lineOwner == 0) {
                    collided[i]->CollisionEnter(collided[j]);
                    collided[j]->CollisionEnter(collided[i]);
                }
                else {
                    //AI-ASSISTED: skeleton | AI did: explained the lineOwner logic and gave the else skeleton | I did: filled the blanks
                    auto &owner = (lineOwner == 1) ? collided[i] : collided[j];
                    auto &other = (lineOwner == 1) ? collided[j] : collided[i];
                    mDrawContext->SetContextOffset(owner->GetLocation());
                    mDrawContext->SetContextRotation(owner->GetRotation());
                    Point2D localHit = mDrawContext->ReverseTransform(hit);
                    owner->CollisionEnter(other, localHit);
                    other->CollisionEnter(owner);
                }
            }

            }
        }
    }
    mDrawContext->SetContextOffset(Point2D(0, 0));
    mDrawContext->SetContextRotation(0.0f);
}

/**
 * @brief   Registers a notification name to be sent when the given key is pressed down.
 * @param   key The key code (e.g. CMPUT350::Key::A).
 * @param   notification The message sent to the NotificationManager on key-down. A key may have several.
 * AI-ASSISTED: skeleton | AI did: gave the map type (int -> vector<string>) and the one-line pattern with blanks |
 *              I did: filled the blanks and wrote the function
 */
void GameEngine::InstallKeyDownNotification(int key, const std::string &notification) {
    mKeyDownNotifications[key].push_back(notification);
}

/**
 * @brief   Registers a notification name to be sent when the given key is released.
 * @param   key The key code.
 * @param   notification The message sent to the NotificationManager on key-up.
 * AI-ASSISTED: skeleton | AI did: gave the map type and the one-line pattern with blanks, then pointed out
 *              that I used the key-down map by mistake | I did: filled the blanks and fixed the bug
 */
void GameEngine::InstallKeyUpNotification(int key, const std::string &notification) {
    mKeyUpNotifications[key].push_back(notification);
}

/**
 * @brief   Returns a const iterator to the first game object in the engine.
 * @return  mGameObjects.cbegin(); callers can read the objects but not reorder or replace the pointers.
 * AI-ASSISTED: provided | AI did: told me each iterator returns the matching call on mGameObjects |
 *              I did: typed them and fixed the GameEngine:: / const qualifiers after feedback
 */
EngineView::const_iterator GameEngine::cbegin() const { return mGameObjects.cbegin(); }

/**
 * @brief   Returns a const iterator one past the last game object.
 * @return  mGameObjects.cend().
 * AI-ASSISTED: provided | AI did: told me each iterator returns the matching call on mGameObjects |
 *              I did: typed them and fixed the GameEngine:: / const qualifiers after feedback
 */
EngineView::const_iterator GameEngine::cend() const { return mGameObjects.cend(); }

/**
 * @brief   Returns a const iterator to the first game object (enables range-for over the engine).
 * @return  mGameObjects.begin(), a const_iterator because this function is const.
 * AI-ASSISTED: provided | AI did: told me each iterator returns the matching call on mGameObjects |
 *              I did: typed them and fixed the GameEngine:: / const qualifiers after feedback
 */
EngineView::const_iterator GameEngine::begin() const { return mGameObjects.begin(); }

/**
 * @brief   Returns a const iterator one past the last game object.
 * @return  mGameObjects.end().
 * AI-ASSISTED: provided | AI did: told me each iterator returns the matching call on mGameObjects |
 *              I did: typed them and fixed the GameEngine:: / const qualifiers after feedback
 */
EngineView::const_iterator GameEngine::end() const { return mGameObjects.end(); }


/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 * AI-ASSISTED: explained | AI did: listed which context fields to fill and which loops to touch, gave the skeleton for the context setup blanks | I did: wrote the Run() changes
 */
void GameEngine::Run() {
    //GameContext context{this, mDrawContext.get()};

    GameContext context;
    context.mEngineView = this;
    context.ScreenContext = mDrawContext.get();
    context.GUIContext = mGUIDrawContext.get();
    context.mNotificationManager = &mNotificationManager;

    while (mWindow->isOpen())  // window is open
    {

        context.currentFrame = mFrame;

        // 0. Remove any objects that are now dead
        // AI-assisted: erase-remove idiom explained by AI
        mGameObjects.erase(
            std::remove_if(
                mGameObjects.begin(),
                mGameObjects.end(),
                [](const std::shared_ptr<GameObject> &object) {
                    return !object->IsAlive();
                }
            ), mGameObjects.end()
        );
        // 1. Activate and initialize any objects added during the last frame
        // AI-assisted: swap idea explained by AI
        std::vector<std::shared_ptr<GameObject>> batch;
        batch.swap(mPendingObjects);
        for (auto &object : batch) {
            context.CurrObject = object;
            mGameObjects.push_back(object);
            object->Initialize(&context);
        }

        // 2. Process events
        ProcessEvents(&context);
        
        
        // 3. Update game objects
        for (auto &object : mGameObjects) {
            context.CurrObject = object;
            object->Update(&context);
        }

        // 4. Process collision events
        ProcessCollisions(&context);

        // 5. Late updates
        for (auto &object : mGameObjects) {
            context.CurrObject = object;
            object->LateUpdate(&context);
        }

        // Clear window
        mWindow->clear();

        // 6. Render background
        // AI-assisted: dynamic_pointer_cast explained by AI
        for (auto &object : mGameObjects) {
            context.CurrObject = object;
            auto graphic = std::dynamic_pointer_cast<GraphicsObject>(object);
            if (graphic) {
                //AI-ASSISTED: skeleton | AI did: said to set offset and rotation from the object before each render call, reset afterwards, and gave the blanks | I did: filled the blanks and wrote the loops
                mDrawContext->SetContextOffset(graphic->GetLocation());
                mDrawContext->SetContextRotation(graphic->GetRotation());
                graphic->RenderBackground(&context);
            }
        }

        // 7. Render foreground
        for (auto &object : mGameObjects) {
            context.CurrObject = object;
            auto graphic = std::dynamic_pointer_cast<GraphicsObject>(object);
            if (graphic) {
                //AI-ASSISTED: skeleton | AI did: said to set offset and rotation from the object before each render call, reset afterwards, and gave the blanks | I did: filled the blanks and wrote the loops
                mDrawContext->SetContextOffset(graphic->GetLocation());
                mDrawContext->SetContextRotation(graphic->GetRotation());
                graphic->RenderForeground(&context);
            }
        }

        mDrawContext->SetContextOffset(Point2D(0, 0));
        mDrawContext->SetContextRotation(0.0f);

        for (auto &object : mGameObjects) {
            context.CurrObject = object;
            object->RenderUI(&context);
        }

        // Actually render to window
        mWindow->display();
        ++mFrame;
    }
}

// Sample code for processing events

// bool GameEngine::ProcessEvents(GameContext *context)
//{
//	while (const std::optional event = mWindow->pollEvent())
//	{
//		if (event->is<sf::Event::Closed>())
//		{
//		}
//		else if (event->is<sf::Event::Resized>())
//		{
//		}
//		else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
//		{
//			// use keyPressed->unicode to get character
//		}
//	}
// }

}  // namespace CMPUT350
