# CMPUT 350 HW 1

# CMPUT 350 Project 1a

## AI usage disclosure

Tool: Claude (claude.ai). Full chat log: <https://claude.ai/share/cda2dce6-4845-4a01-9a25-fa49c1a8e462>.
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