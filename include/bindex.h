#pragma once

#define FUSE_USE_VERSION 28

#include <fuse.h>

int bindex_init(char *dpath);
int bindex_getattr(const char *path, struct stat *stbuf);
int bindex_readlink(const char *path, char *buf, size_t size);
int bindex_mknod(const char *path, mode_t mode, dev_t rdev);
int bindex_mkdir(const char *path, mode_t mode);
int bindex_unlink(const char *path);
int bindex_rmdir(const char *path);
int bindex_symlink(const char *from, const char *to);
int bindex_rename(const char *from, const char *to);
int bindex_link(const char *from, const char *to);
int bindex_chmod(const char *path, mode_t mode);
int bindex_chown(const char *path, uid_t uid, gid_t gid);
int bindex_truncate(const char *path, off_t size);
int bindex_utime(const char *path, struct utimbuf *buf);
int bindex_open(const char *path, struct fuse_file_info *fi);
int bindex_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi);
int bindex_release(const char *path, struct fuse_file_info *fi);
int bindex_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		   off_t offset, struct fuse_file_info *fi);

struct fuse_operations bindex_oper = {
    .getattr  = bindex_getattr,
    .readlink = bindex_readlink,
    .mknod    = bindex_mknod,
    .mkdir    = bindex_mkdir,
    .unlink   = bindex_unlink,
    .rmdir    = bindex_rmdir,
    .symlink  = bindex_symlink,
    .rename   = bindex_rename,
    .link     = bindex_link,
    .chmod    = bindex_chmod,
    .chown    = bindex_chown,
    .truncate = bindex_truncate,
    .utime    = bindex_utime,
    .open     = bindex_open,
    .read     = bindex_read,
    .release  = bindex_release,
    .readdir  = bindex_readdir,
};
