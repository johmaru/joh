#ifndef FS_SYSTEM_H
#define FS_SYSTEM_H

enum FileSystemErrorType {
    FS_NO_ERROR = 200,
    FS_ERROR_INVALID_PATH = 201,
    FS_ERROR_PERMISSION_DENIED = 202,
    FS_ERROR_FILE_NOT_FOUND = 203,
    FS_ERROR_UNKNOWNOPERETOR = 204
};

int settings_init();

#endif