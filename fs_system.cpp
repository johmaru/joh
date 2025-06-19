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

int check_json_structure_and_restructure(const json& j) {
    json updatedJson = j;
    bool needsUpdate = false;

    if (!updatedJson.contains("language")) {
        updatedJson["language"] = "en";
        needsUpdate = true;
    }

    if (needsUpdate) {
        auto documentsPath = getDocumentFolder();
        std::filesystem::path settingsPath;

        switch (joh_main::getPlatformName()) {
            case OS_WINDOWS:
                settingsPath = documentsPath / "joh_settings" / "settings.json";
                break;
            case OS_LINUX:
                settingsPath = documentsPath / ".joh_settings" / "settings.json";
                break;
            default:
                std::cerr << "Unknown operating system. Cannot update settings." << std::endl;
                return FS_ERROR_UNKNOWNOPERETOR;
        }

        try {
            std::ofstream file(settingsPath);
            file << updatedJson.dump(4);
            if (file.fail()) {
                std::cerr << "Error writing updated settings to file: " << settingsPath << std::endl;
                return FS_ERROR_INVALID_PATH;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception while updating settings: " << e.what() << std::endl;
            return FS_ERROR_INVALID_PATH;
        }
    }

    return FS_NO_ERROR;
}

json load_and_validate_settings() {
    auto documentsPath = getDocumentFolder();
    std::filesystem::path settingsPath;

    switch (joh_main::getPlatformName()) {
        case OS_WINDOWS:
            settingsPath = documentsPath / "joh_settings" / "settings.json";
            break;
        case OS_LINUX:
            settingsPath = documentsPath / ".joh_settings" / "settings.json";
            break;
        default:
            std::cerr << "Unknown operating system. Cannot load settings." << std::endl;
            return FS_ERROR_UNKNOWNOPERETOR;
    }

    if (!std::filesystem::exists(settingsPath)) {
        std::cerr << "Settings file does not exist: " << settingsPath << std::endl;
        return FS_ERROR_INVALID_PATH;
    }

    try {
        std::ifstream file(settingsPath);
        json j;
        file >> j;

        if (file.fail()) {
            std::cerr << "Error reading settings from file: " << settingsPath << std::endl;
            return FS_ERROR_INVALID_PATH;
        }

        if (check_json_structure_and_restructure(j) != FS_NO_ERROR) {
            std::cerr << "Error checking or updating JSON structure." << std::endl;
            return FS_ERROR_INVALID_PATH;
        }

        return j;
    } catch (const std::exception& e) {
        std::cerr << "Exception while loading settings: " << e.what() << std::endl;
        return FS_ERROR_INVALID_PATH;
    }
}