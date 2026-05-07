# gainlib

Lightweight cross-platform C++ engine targeting **OpenGL ES 2.0**. Originally built for mobile (Android / iOS) game prototypes; now also runs on desktop Linux and macOS via a thin GL-backend shim.

The whole library is ~25 source files. The architecture is:

```
Gain::Core            — tick loop, scene/layer registry, touch dispatch
  └─ Gain::Layer      — z-ordered render container (a Base subclass)
       └─ Gain::Base  — drawable + animatable; subclass to add a renderer
                        (built-ins: Rect, Bitmap, Text, ImageBitmap, ...)
Gain::GainApp         — your app subclass; receives Initialize() + Update()
                        + touch events
```

Apps subclass `Gain::GainApp`, push render clients (any `Gain::Base` subclass) onto a `Gain::Layer`, and the engine drives `Update(sec, deltaSec)` per frame plus a single `Layer::render()` per layer per frame.

---

## Building

Requires CMake ≥ 3.14, C++17, and a platform GL backend.

```sh
cmake -B build && cmake --build build
```

The build produces a static library `Gain` you can link from your own CMake project:

```cmake
add_subdirectory(path/to/gainlib)        # or find_package(Gain CONFIG)
target_link_libraries(your_app PRIVATE Gain::Gain)
```

### Dependencies

- **glm** — auto-detected via `find_package(glm CONFIG)`. If not installed, the build falls back to fetching glm 1.0.1 via `FetchContent`.
  - `brew install glm` (macOS)
  - `apt install libglm-dev` (Debian / Ubuntu)
- **OpenGL ES 2.0** — platform-provided.
  - **Linux**: `libgles2-mesa-dev` (or your distro equivalent), `libglfw3-dev` if you use GLFW.
  - **Android**: `libGLESv2` is in the NDK; this CMakeLists also links `log` and `android`.
  - **macOS**: routes through `OpenGL.framework` (legacy 2.1 profile, a strict superset of GLES 2.0). `mappings.h` includes `<OpenGL/gl.h>` under `__APPLE__`.
  - **iOS**: planned; would gate on `TARGET_OS_IPHONE` and use `<OpenGLES/ES2/gl.h>`. Not currently wired.

---

## Minimal example

```cpp
#include <gain.h>

class HelloApp : public Gain::GainApp {
public:
    void Initialize() override {
        auto* layer = new Gain::Layer();
        Gain::GetCore()->addRenderClient(layer);

        auto* rect = new Gain::Rect(0.0f, 0.0f, 0.4f, 0.4f);  // normalized coords
        rect->setColor(0.95f, 0.30f, 0.30f, 1.0f);
        rect->setPlacement(Gain::MID_CENTER);
        layer->addRenderClient(rect);

        rotating_ = rect;
    }

    void Update(float sec, float /*deltaSec*/) override {
        if (rotating_) rotating_->setRotation(sec);
    }

private:
    Gain::Base* rotating_ = nullptr;
};
```

You provide a platform `main` that creates a window + GL context, calls `GAIN_init(width, height, new HelloApp())`, then loops on `GAIN_step()` and your platform's swap-buffers + event-poll. See `VanillaApp` (https://github.com/G-A-I-N/VanillaApp) for a complete Linux/GLFW entry point.

---

## Coordinate conventions

- **Normalized [-1, 1]** is the rendering space. The shader applies `Core->ratio` (window aspect) automatically so `(0, 0)` is the centre of the viewport regardless of window dimensions.
- **Storage y grows downward** to match touch-event and screen-pixel conventions; the y-flip into NDC happens in shaders.
- **Scenes** are layer stacks, indexed `SCENE_FIRST_INDEX`..`SCENE_LAST_INDEX` (configurable via `MAX_SCENES`). `addRenderClient(layer, SCENE_DEFAULT_BACK / FRONT)` pins layers to the always-back or always-front scenes.

---

## Status

This library is under active production-readiness work. Known rough edges currently:

- Built-in `Logger` writes via `printf` and stderr; no log-level filtering yet.
- Many setup paths report failure via stdout rather than return codes.
- Mouse / keyboard input is not natively integrated — desktop apps wire GLFW callbacks themselves.

PRs welcome.

## License

Apache License 2.0 — see `LICENSE`.
