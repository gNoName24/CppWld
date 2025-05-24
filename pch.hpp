#ifndef PCH_HPP
#define PCH_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <codecvt>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <random>
#include <type_traits>
#include <unordered_map>
#include <windows.h>
#include <psapi.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include "libs/json.hpp"

extern "C" {
    #include "libs/noise1234.h"
}

#include "imgui.h"
#include "imgui-SFML.h"

#include "SiSFML/SiSFML.hpp"

// Инструменты
#include "Tools/TlMain.hpp"
#include "Tools/TlConsole.hpp"
#include "Tools/TlFile.hpp"

#endif
