#ifndef INCLUDE_FILE
#define INCLUDE_FILE

#include "parser.h"

typedef struct File{
    std::string name;
    struct stat metadata;
//     struct stat {
//     dev_t     st_dev;         // 文件所在设备的设备号
//     ino_t     st_ino;         // 文件的 inode 号
//     mode_t    st_mode;        // 文件类型和权限
//     nlink_t   st_nlink;       // 文件的硬链接数
//     uid_t     st_uid;         // 文件所有者的用户 ID
//     gid_t     st_gid;         // 文件所有者的组 ID
//     dev_t     st_rdev;        // 如果是设备文件，则为设备文件的设备号
//     off_t     st_size;        // 文件大小（以字节为单位）
//     blksize_t st_blksize;     // 文件系统 I/O 缓冲区的优选大小
//     blkcnt_t  st_blocks;      // 文件所占用的磁盘块数
//     struct timespec st_atim;  // 最后访问时间
//     struct timespec st_mtim;  // 最后修改时间
//     struct timespec st_ctim;  // 最后状态变化时间
// };

}File;

#endif