#include "folders.hpp"

namespace fs = std::filesystem;
const fs::path io::folders::RES = "res";
const fs::path io::folders::SAVES = "saves";
const fs::path io::folders::SCREENSHOTS = "screenshots";
const fs::path io::folders::LANG = RES / "lang";
const fs::path io::folders::CURSORS = RES / "cursors";
const fs::path io::folders::IMAGES = RES / "images";
const fs::path io::folders::FONTS = RES / "fonts";
const fs::path io::folders::SOUNDS = RES / "sounds";
const fs::path io::folders::CONTENT = RES / "content";
const fs::path io::folders::GENERATION = CONTENT / "generation";
const fs::path io::folders::GENERATION_DEFAULT = GENERATION / "default";
const fs::path io::folders::WAVES = CONTENT / "waves";
