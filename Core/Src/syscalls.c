/**
  ******************************************************************************
  * @file    syscalls.c
  * @brief   Minimal syscall stubs for newlib-nano.
  ******************************************************************************
  */

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>

int _close(int file)
{
  (void)file;
  errno = EBADF;
  return -1;
}

int _fstat(int file, struct stat *st)
{
  (void)file;
  if (st != 0)
  {
    st->st_mode = S_IFCHR;
  }
  return 0;
}

int _isatty(int file)
{
  (void)file;
  return 1;
}

off_t _lseek(int file, off_t ptr, int dir)
{
  (void)file;
  (void)ptr;
  (void)dir;
  return 0;
}

int _open(const char *path, int flags, ...)
{
  (void)path;
  (void)flags;
  errno = ENOSYS;
  return -1;
}

ssize_t _read(int file, void *ptr, size_t len)
{
  (void)file;
  (void)ptr;
  (void)len;
  return 0;
}

ssize_t _write(int file, const void *ptr, size_t len)
{
  (void)file;
  (void)ptr;
  return (ssize_t)len;
}
