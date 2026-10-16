# CMPUT 350 HW 1

# CMPUT 350 Project 1a

## AI usage disclosure

Tool: Claude (claude.ai). Full chat logs: 
1. <https://claude.ai/share/cda2dce6-4845-4a01-9a25-fa49c1a8e462>
2. <https://claude.ai/share/3e0ba032-cf12-4edb-8c36-a563bc2e4df5>
3. <https://claude.ai/share/bd875010-d741-47dd-b3f7-f20d45f5fb70>

I worked alone, so I am responsible for understanding all code below.
AI-assisted code is also marked with "AI-assisted" comments in the source.

### MathUtil.h

| Item | Origin |
|---|---|
| File skeleton and function signatures | Course-provided template. I kept all signatures as given |
| Point2D operators (+, -, *, +=, -=, ==), Dot, Cross | Written by me. AI showed the pattern for `operator+` and `+=` |
| `Distance` | Written by me. My first version squared before subtracting; AI pointed out the error and I fixed it |
| `Normalize` | Written by me. The zero-length guard was suggested by AI |
| `*=` and `/=` changed from `int` to `float` | AI suggestion. The ball demo divides by a float, and `int` would truncate it |
| `Distance` return type `double` to `float` | AI suggestion, to match the handout |
| `Rect(float left, float top, ...)` fix | AI found the bug (x and y were swapped); I fixed it |
| `Rect` operators `\|=` (Rect, Point2D, Line), `&=`, `+=`, `+`, `Inset` | AI explained the approach and gave the code; I typed it out myself and worked through each line |
| `IsInside` | AI-guided. My first version had the comparisons reversed on the right and bottom edges; AI found it and I fixed it |
| `#include <algorithm>`, `static` to `inline` on the free operators | AI suggestions (portability, avoiding unused-function warnings) |
| `Overlaps` | AI-provided code, based on the "prove no gap" method from my lecture 6 notes |
| `Line`, `Circle` | Template stubs, left unchanged (not needed for 1a) |

### Prompts used (MathUtil)
1. "ok this time i screwed up i need to finish this in 12 hrs so according to the AI policy please help me finish this and let me learn to do this code and hopefully get more knowledge on smfl and c++"
2. "ok so lets start the stuff step by step starting with step 1"
3. "ok this is actually getting confusing, where do i even start i start with the game engine?"
4. "actually explain this skeleton before i write"
5. "ok so is mathUtil a good start?, where there are many other files"
6. "so where and how to start cause newsflash im bad at c++ or horrendous in some sense"
7. "huh whats these functions about"
8. "so is this correct"
9. "just tell me if point 2d is ok before i move on"
10. "kept the function calls the same since i dont wanrt to change whats given by the assignment and use these notes to help me" (I also uploaded my lecture revision notes)
11. "so how does rect work even remember the ai policy to put those in the readME"
12. "guide me towards writing it so i can answer the questions, you do have the main doc for the assg righ"
13. "ok i may have been vague but just tell me what each function does even and how these are usually written"
14. "ok make the explainations more specific, simple, and towards the functions in the order i see them at so i know what to write"
15. "how is it and also i worte all this by hand after reading your response"
16. "but still wont the code work as is without the imports and inline changes since i dont know what those suggestions are"

I also pasted my MathUtil.h several times for review, and pasted compiler and test output.

### Reflection (MathUtil)
- **What worked:** The AI explanations helped me understand the vector math and how every Rect operation reduces to min/max on the four edges. Writing a small test file and comparing the output to hand-calculated values made bugs obvious.
- **What AI got wrong:** Early on it told me to delete the `int` `Rect(Point2D, int, int)` constructor, claiming `Rect(pos, 15)` was ambiguous. That was wrong: the `int` version is what avoids ambiguity with `Rect(center, radius)`. The real pitfall is that `Rect(pos, 15)` silently makes a 15x0 box, so the radius must be passed as a float. I kept the template's constructors.
- **What I had to fix myself:** squaring before subtracting in `Distance`, the swapped x/y in a `Rect` constructor, and reversed comparisons in `IsInside`.
- **What I learned:** const methods, returning `*this` from `+=`, what a dot product is, and how the Rect union/intersection operations work on edges.
- **Extent of AI use:** for MathUtil, AI gave explanations and most of the Rect operator code and I typed it in and tested it. The Point2D operators were mostly my own work. The majority of the rest of the project (Bullet, Enemy, Player, main) is planned to be my own code.

### DrawContext.cpp

| Item | Origin |
|---|---|
| Function signatures and file skeleton | Course-provided |
| `DrawCircle`, `DrawRect`, `FrameRect`, `DrawText` | Written by me, using the SFML 3.1 headers in `extern/`. AI pointed out bugs (missing argument in `draw`, `*mFont` for the font reference, outline thickness) and gave small SFML usage examples (hollow shape, ConvexShape triangle, Text) that I adapted |
| `DrawLine` | I wrote the code. AI explained the thick-line idea (the direction vector, its 90-degree perpendicular, and half-width offsets) and gave the skeleton with blanks that I filled in |
| `DrawCenteredText` | **Not implemented as centering.** It is a copy of `DrawText` (optional in the handout). The ball demo's labels are therefore not centered |
| `GetWindowWidth/Height` | Template stubs, unchanged |

Prompts (excerpts): "ok seriously i did tell you NO code unless we need it, i dont want ai plagerism"; "well thats why im asking for your help and i said code examples can help"; "wait so direction and offset what vector formula is this"; "im in a slump for line do i need it".

Reflection: AI's first answer dumped a full implementation after I'd asked for guidance only; I discarded it and wrote the file myself from the SFML headers. AI's explanation of perpendicular vectors (dot product equals zero) was the useful part. Bugs I made that I fixed: `sf::Color(c)` conversion, empty `draw()` calls, `LineStrip` ignores width, a variable shadowing the color parameter, and an offset along the line instead of perpendicular to it.

### GameEngine.h / GameEngine.cpp

| Item | Origin |
|---|---|
| Class declaration, method signatures, stub constructor/Run comments | Course-provided template |
| Member types: `shared_ptr` window/font, `unique_ptr` DrawContext, two vectors of `shared_ptr<GameObject>` | My first guess was wrong (window/font unique, objects weak). AI corrected it using the handout and my lecture notes' ownership rules |
| Constructor, destructor, `AddGameObject` | Written by me (window creation fixed after AI pointed out I had made a local window instead of setting `mWindow`) |
| `ProcessEvents` | Written by me with AI's explanation of the SFML 3 event pattern |
| `Run()` steps 0-1 (erase-remove, pending-list swap) | AI explained the idioms and why they are needed; I wrote the code. AI also provided a complete reference version of `Run()` earlier in the chat, which I did not use as-is |
| `Run()` steps 3-7 (update loops, two render loops with `dynamic_pointer_cast`) | Written by me after AI's explanation of the cast and why there are two loops |
| `ProcessCollisions` | AI provided a code skeleton (collect collidable objects, pair loop, bounds copy, `Overlaps`, notify both). My first version had the inner loop starting at 0, which caused self-collisions and double notifications; AI found that and I changed it to start at `i + 1` |

### main.cpp
Course-provided (ball demo and Galaga branch). The `mBallSsample` flag selects the mode: `true` runs the ball demo, `false` runs Galaga. <Galaga branch edited by me to add 40 enemies, once done.>

### Galaga (Enemy, Bullet, Player, Stars)
<fill in as each is done. Stars is course-provided and unchanged.>

### Reflection (engine)
- **What worked:** Explanations of the erase-remove idiom, the pending-list swap, and the pair loop were the most useful AI contributions. The ball demo running correctly confirmed the engine loop order, add/remove, and collision notification.
- **What AI got wrong or caused:** It gave full code several times after I asked for guidance only, and it first suggested a constructor-order for `DrawContext` that I had to adjust. The inner-loop bug in `ProcessCollisions` was mine, and AI caught it.
- **Bugs I fixed myself:** local window instead of the member, `erase` called with one argument (undefined behavior), duplicated `LateUpdate`, and the loop-start bug above.
- **What I learned:** shared ownership vs. a single owner, why objects are added through a pending list, and why bounds must be copied.

### Project 1b

## Project 1b: AI usage, Chat 1 (engine, Player, Bullet, start of Enemy)

Tool: Claude (claude.ai). Chat link: TODO (add the share link for this chat)
The AI acted as a TA: explanations, skeletons with blanks, and code review. Labels:
explained = AI explained, I wrote it. skeleton = AI gave structure with blanks, I filled them.
provided = AI gave the code, I typed it. Every function also has an `AI-ASSISTED:` comment in
the source (`grep -rn "AI-ASSISTED" engine galaga`).

| Item | Origin | What AI did | What I did |
|---|---|---|---|
| NotificationManager (`Register`, `Unregister`, `Notify`) | skeleton, partly provided | Explained weak_ptr listeners and copy-before-iterate; gave skeletons; gave the answers for the `Unregister` lambda (`!p \|\| p == target`) and the `Notify` cleanup (`mListeners[message]`, `expired`) | Wrote `Register` and the `Notify` loop; typed the rest |
| GameObject / GraphicsObject / GameContext | explained | Explained forward declarations, defaults, includes | Wrote them from the handout |
| `Line::Crosses` `const Line &` fix | explained | Pointed it out | Changed it |
| GameEngine `Run()` changes (context fields, `CurrObject`, `currentFrame`, `RenderUI`, per-object offset/rotation) | skeleton | Listed the fields and loops; gave blanks for the context setup | Wrote the changes |
| Engine iterators (`cbegin`, `cend`, `begin`, `end`) | provided | Gave the one-line bodies | Typed them; fixed qualifiers |
| Key notification maps, `InstallKey...`, `ProcessEvents` KeyPressed/KeyReleased | skeleton (map types provided) | Gave map types and the pattern with blanks | Filled blanks; set key repeat off |
| `WorldBounds`, `WorldShapes`, `ShapesCollide` | skeleton | Gave skeletons with blanks | Filled the blanks |
| `ProcessCollisions` narrow phase | skeleton | Gave the structure in small pieces and the line-owner `else` skeleton | Filled blanks; wrote the loop |
| `CollisionTests.h`: `CircleCircle`, `CircleRect`, `RectRect`, `LineRect` | skeleton | Explained the ideas; gave skeletons | Filled the blanks |
| `CollisionTests.h`: `LineCircle` | skeleton, nearly provided | Derived the quadratic; gave the formulas and almost the whole body | Filled the blanks; fixed the `Dot` calls |
| Bezier `GetPoint`/`GetSlope` | skeleton / explained | Gave formulas; gave `GetPoint` skeleton | Wrote the code |
| Bezier `FindSegment` | skeleton | Gave structure and hints that were nearly the answers | Filled the blanks |
| Bezier constructor and instance functions | skeleton / explained | Explained initializer lists; said to delegate to the static versions | Wrote them |
| DrawContext `Transform`, setters | explained | Pointed to Lecture 7 and the rotate-then-shift order | Wrote them |
| DrawContext `ReverseTransform`, `Transform(Rect)` | skeleton | Gave structure with blanks | Filled the blanks |
| DrawContext draw functions (transform points before drawing) | skeleton | Said to transform points; gave blanks and the `ConvexShape` skeleton | Filled the blanks; wrote the rest |
| Stars `GetLocation`/`GetRotation` | provided | Gave the return values and where they go | Typed them |
| Player (shapes, movement flags, firing, render, notifications) | skeleton / explained | Explained local vs world coordinates and the key-down/key-up flag design; gave skeletons (including `expired`) | Wrote most of it |
| Bullet constructor and `UpdateShape` | provided | Gave the lines | Typed them |
| Bullet `Update`, `RenderForeground`, `CollisionEnter`, `IsAlive`, `Kill` | explained | Described the design; corrected my first versions | Wrote them |
| Enemy.h, constructor, getters, `IsAlive`, `Kill` | explained / skeleton | Listed members; gave the constructor with a blank | Wrote them |
| `Enemy::Follow` | skeleton | Explained constant speed (step = speed / slope length); gave blanks | Filled the blanks |
| All doc comment text (`@brief`, `@param`) | provided | Wrote all of it | Pasted and checked it |
| `test_collide.cpp`, `test_bezier.cpp`, key-test `main` | provided | Wrote them entirely | Ran them; `test_collide.cpp` was deleted |

### Bugs in the AI's suggestions
- Gave the expected Bezier slope at t=0.5 as (7.5, 0). The correct value is (15, 0); my code was right.
- Said the stub `GetLocation`/`GetRotation` bodies in `GraphicsObject.cpp` would let `Ball` compile. That was wrong: a `= 0` declaration keeps the class abstract.
- Called something a "bug" in `Bullet::Update`'s off-screen check that was not a bug; the real issue was the immediate-kill on hit.
- Gave blanks so close to the answer (`FindSegment`, `LineCircle`) that they were effectively provided code.
- Padded one reply with unrelated side notes after I asked only for docs.

### Bugs I made (found in this chat)
- NotificationManager: unfilled blanks, `vec`/`vector` mismatch, `ReceiveNotification` called on the manager instead of the object.
- `InstallKeyUpNotification` used the key-down map; the key-up branch searched the wrong map; `iter->first` instead of `iter->second` (twice); an `#ifdef KEYBOARD_H` hack that skipped the whole class.
- Bezier: missing factors of 3, last slope coefficient 6 instead of 3, global `t` instead of `localT`.
- DrawContext: `ReverseTransform` in the wrong order; `Transform(Rect)` with swapped x/y and `box(b, b)` misuse; duplicate `DrawRect`; wrong outline thickness; duplicate `position` in `DrawCircle`; `DrawLine` corners still using `from`/`to`.
- Called `DrawContext::Transform` as if static in `ProcessCollisions`.
- Used `Transform(hit)` instead of `ReverseTransform(hit)` in the line-hit branch. TODO: confirm it is fixed.
- Player: `pop_back` instead of `push_back`; shapes in world coordinates; `Colors` typo; swapped `CurrObject` and manager; clamp bound 768 instead of 748.
- Bullet: `null` instead of `nullptr`; killed the bullet immediately on hit.
- Ran an old binary and read stale output as a result (more than once).

### Reflection (Chat 1)
Skeletons with blanks worked best: I saw the structure, filled in the details, and could explain them.
Testing small pieces separately (notification test, Bezier values, collision shape tests) caught bugs
early. The AI gave one wrong expected value and some blanks that were nearly the answer. My mistakes
were mostly copying a pattern without adapting it (wrong map, wrong member, wrong coordinate space)
and running stale builds. TODO: add in your own words what you learned about local vs world space.

Doc comment text for the engine, Player, and Bullet was written by AI and checked by me. The
throwaway tests were written by AI and are not part of the submission.

## Reflection (Chat 2)

**What worked:** The skeletons with blanks were the most useful part. I had to read the
surrounding code and work out the right response for each blank, so I wrote the logic myself
instead of copying it. Counting frames with a shared clock (mStateStartFrame and elapsed) also
made states possible, and together with fixing the missing return after ChangeState it let me
finally see the game-over screen.

**What the AI got wrong:** The bug that annoyed me most was "PlayerDied" being caught by the
generic "Died" branch, because it also contains the word "Died". Lives never dropped until I
moved the PlayerDied branch above it. Other skeletons needed testing too (no return after
ChangeState, unreliable respawn timing).

**What I got wrong:** My biggest mistake was creating a second GameEngine inside the Galaga
constructor. That made a second window instead of using the engine's view through the context,
and it taught me that objects get engine access from GameContext, not by making their own.

**What I still don't fully understand:** The `rfind(..., 0)` trick for "starts with" and the
math in Follow (step = 6 / slope length for constant speed). I can use them, but I'd need to
re-read them to explain them.

**Known gaps:** listed in project.txt, section 4.