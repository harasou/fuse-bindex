/* vim: set ft=c ts=8 sw=8 noet : */

#include <alloca.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

/* Bind directory */
static struct {
	char *dpath;
	size_t dpath_len;
} src;

/* Convert the pathname on fuse to the real path name */
#define CONVERT_REALPATH(path, real_path)                                      \
	char *real_path = (char *)alloca(src.dpath_len + strlen(path) + 1);    \
	sprintf(real_path, "%s%s", src.dpath, path)

#define DO_SYSCALL(systemcall, path)                                           \
	CONVERT_REALPATH(path, real_path);                                     \
	if (systemcall(real_path) == -1) {                                     \
		return -errno;                                                 \
	}                                                                      \
	return 0

#define DO_SYSCALL2(systemcall, path, arg1)                                    \
	CONVERT_REALPATH(path, real_path);                                     \
	if (systemcall(real_path, arg1) == -1) {                               \
		return -errno;                                                 \
	}                                                                      \
	return 0

#define DO_SYSCALL3(systemcall, path, arg1, arg2)                              \
	CONVERT_REALPATH(path, real_path);                                     \
	if (systemcall(real_path, arg1, arg2) == -1) {                         \
		return -errno;                                                 \
	}                                                                      \
	return 0

int bindex_init(char *dpath)
{

	struct stat buf;

	if (stat(dpath, &buf) < 0) {
		return -errno;
	}
	if ((buf.st_mode & S_IFMT) != S_IFDIR) {
		return -1;
	}

	src.dpath     = dpath;
	src.dpath_len = strlen(dpath);

	return 0;
}

int bindex_readlink(const char *path, char *buf, size_t size)
{
	ssize_t n;

	CONVERT_REALPATH(path, real_path);
	if ((n = readlink(real_path, buf, size - 1)) == -1) {
		return -errno;
	}
	buf[n] = '\0';
	return 0;
}

int bindex_mknod(const char *path, mode_t mode, dev_t rdev)
{
	DO_SYSCALL3(mknod, path, mode, rdev);
}

int bindex_mkdir(const char *path, mode_t mode)
{
	DO_SYSCALL2(mkdir, path, mode);
}

int bindex_unlink(const char *path)
{
	DO_SYSCALL(unlink, path);
}

int bindex_rmdir(const char *path)
{
	DO_SYSCALL(rmdir, path);
}

int bindex_symlink(const char *from, const char *to)
{
	CONVERT_REALPATH(to, real_to);
	if (symlink(from, real_to) == -1) {
		return -errno;
	}
	return 0;
}

int bindex_rename(const char *from, const char *to)
{
	CONVERT_REALPATH(to, real_to);
	DO_SYSCALL2(rename, from, real_to);
}

int bindex_link(const char *from, const char *to)
{
	CONVERT_REALPATH(to, real_to);
	DO_SYSCALL2(link, from, real_to);
}

int bindex_chmod(const char *path, mode_t mode)
{
	DO_SYSCALL2(chmod, path, mode);
}

int bindex_chown(const char *path, uid_t uid, gid_t gid)
{
	DO_SYSCALL3(chown, path, uid, gid);
}

int bindex_truncate(const char *path, off_t size)
{
	DO_SYSCALL2(truncate, path, size);
}

int bindex_utime(const char *path, struct utimbuf *buf)
{
	DO_SYSCALL2(utime, path, buf);
}

int should_execute(struct stat *buf)
{
	return S_ISREG(buf->st_mode) &&
	       buf->st_mode & ((S_IXUSR | S_IXGRP | S_IXOTH));
}

int mkonetimefd(void)
{
	int fd, n;
	char template[]		    = "/tmp/.bindex.XXXXXX";
	char tempfd[32]		    = {0};
	char temppath[PATH_MAX + 1] = {0};

	if ((fd = mkstemp(template)) == -1) {
		return -1;
	}

	sprintf(tempfd, "/proc/%u/fd/%u", getpid(), fd);
	if ((n = readlink(tempfd, temppath, PATH_MAX)) == -1) {
		return -1;
	}
	if (unlink(temppath) == -1) {
		return -1;
	}

	return fd;
}

int bindex_open(const char *path, struct fuse_file_info *fi)
{
	extern char **environ;
	struct stat buf;
	int fd, pid, status;
	char *argv[2];

	CONVERT_REALPATH(path, real_path);
	if (lstat(real_path, &buf) < 0) {
		return -errno;
	}
	if (should_execute(&buf)) {

		if ((fd = mkonetimefd()) == -1) {
			return -errno;
		}

		if ((pid = fork()) < 0) {
			return -errno;
		}
		/* child */
		if (pid == 0) {
			dup2(fd, 1);
			dup2(fd, 2);
			argv[0] = (char *)real_path;
			argv[1] = NULL;
			execve(real_path, argv, environ);
		}
		/* parent */
		else {
			if (waitpid(pid, &status, 0) < 0) {
				return -errno;
			}
			if (lseek(fd, 0, SEEK_SET) == -1) {
				return -errno;
			}
			fi->fh = fd;
		}
	} else {
		if ((fd = open(real_path, fi->flags)) == -1) {
			return -errno;
		}
		fi->fh = fd;
	}
	return 0;
}

int bindex_getattr(const char *path, struct stat *stbuf)
{

	struct fuse_file_info fi;
	struct stat buf;

	CONVERT_REALPATH(path, real_path);
	if (lstat(real_path, stbuf) < 0) {
		return -errno;
	}

	if (should_execute(stbuf)) {
		if (bindex_open(path, &fi) < 0) {
			return -errno;
		}
		if (fstat(fi.fh, &buf) < 0) {
			return -errno;
		}
		if (close(fi.fh) < 0) {
			return -errno;
		}
		stbuf->st_size = buf.st_size;
		stbuf->st_mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
	}

	return 0;
}

int bindex_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi)
{
	return read(fi->fh, buf, size);
}

int bindex_release(const char *path, struct fuse_file_info *fi)
{
	if (close(fi->fh) == -1) {
		return -errno;
	}
	return 0;
}

int bindex_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		   off_t offset, struct fuse_file_info *fi)
{
	DIR *dir;
	struct dirent *dp;

	CONVERT_REALPATH(path, real_path);
	if ((dir = opendir(real_path)) == NULL) {
		return -errno;
	}
	errno = 0;
	while ((dp = readdir(dir)) != NULL) {
		filler(buf, dp->d_name, NULL, 0);
	}
	if (errno != 0) {
		return -errno;
	}
	if (closedir(dir) == -1) {
		return -errno;
	}
	return 0;
}
