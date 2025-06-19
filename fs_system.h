#ifndef FS_SYSTEM_H
#define FS_SYSTEM_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum FileSystemErrorType {
    FS_NO_ERROR = 200,
    FS_ERROR_INVALID_PATH = 201,
    FS_ERROR_PERMISSION_DENIED = 202,
    FS_ERROR_FILE_NOT_FOUND = 203,
    FS_ERROR_UNKNOWNOPERETOR = 204
};

int settings_init();
int check_json_structure_and_restructure(const json& j);
json load_and_validate_settings();

#endif