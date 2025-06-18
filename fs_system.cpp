#include <cstdlib>
#include <fstream>
#ifdef _WIN32
#include <shlobj.h>
#include <windows.h>
#endif

#include "fs_system.h"
#include "joh_main.h"
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct settings_json {
    std::string language;
};

class Init_SettingsData{
private:
json settings_json_data;

public:
    Init_SettingsData() {
        settings_json settings;
        settings.language = "en";
        
        settings_json_data["language"] = settings.language;
        
    }

    json getJson() const {
        return settings_json_data;
    }

    std::string getJsonString(int indent = 4) const {
        return settings_json_data.dump(indent);
    }
};


std::filesystem::path getDocumentFolder() {
    switch (joh_main::getPlatformName()) {
        case OS_WINDOWS:
#ifdef _WIN32
            {
                wchar_t path[MAX_PATH];
                if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, 0, path))) {
                    return std::filesystem::path(path);
                } else {
                    std::cerr << "Failed to get Documents folder path." << std::endl;
                    return std::filesystem::path();
                }
                const char* userProfile = std::getenv("USERPROFILE");
                if (userProfile) {
                    return std::filesystem::path(userProfile) / "Documents";
                }
            }
#endif
        break;

        case OS_LINUX:
            {
                const char* home = std::getenv("HOME");
                if (home) {
                    return std::filesystem::path(home) / "Documents";
                }
            }
            break;

        case OS_UNKNOWN:
            std::cerr << "Unknown operating system. Cannot get Documents folder path." << std::endl;
            break;
    }

    return std::filesystem::path();
}


int settings_init() {

    auto documentsPath = getDocumentFolder();
    
    switch (joh_main::getPlatformName()) {
        
        case OS_WINDOWS:
            {
                std::filesystem::path settingsPath = documentsPath / "joh_settings";
                if (!std::filesystem::exists(settingsPath)) {
                    try {
                        std::filesystem::create_directories(settingsPath);
                    } catch (const std::filesystem::filesystem_error& e) {
                        std::cerr << "Error creating settings directory: " << e.what() << std::endl;
                        return FS_ERROR_INVALID_PATH;
                    }
                }
            }
        break;

        case OS_LINUX:
            {
                std::filesystem::path settingsPath = documentsPath / ".joh_settings";
                if (!std::filesystem::exists(settingsPath)) {
                    try {
                        std::filesystem::create_directories(settingsPath);
                    } catch (const std::filesystem::filesystem_error& e) {
                        std::cerr << "Error creating settings directory: " << e.what() << std::endl;
                        return FS_ERROR_INVALID_PATH;
                    }
                }
                
                {
                    std::filesystem::path settingsFilePath = settingsPath / "settings.json";
                    Init_SettingsData settingsData;
                    
                    std::ofstream file(settingsFilePath);
                    file << settingsData.getJsonString();

                    if (file.fail()) {
                        std::cerr << "Error writing settings to file: " << settingsFilePath << std::endl;
                        return FS_ERROR_INVALID_PATH;
                    }
                }
            }
        break;
        
        case OS_UNKNOWN:
            std::cerr << "Unknown operating system. Cannot initialize settings." << std::endl;
            return FS_ERROR_UNKNOWNOPERETOR;
    }

    return FS_NO_ERROR;
}