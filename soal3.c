
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/statfs.h>
//thanks to blog fandyuhuy
static const char *dirpath = "/home/dell/Unduhan"; //Destinasi folder yang akan di mountkan

char globalpath[1000];
int bendera=0;

static int hm_getattr(const char *path, struct stat *stbuf)
{
    printf("xmp_getattr: %s\n", path);
    int res;

    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = lstat(fpath, stbuf);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_readlink(const char *path, char *buf, size_t size)
{
    printf("xmp_readlink: %s\n", path);
    int res;
    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = readlink(fpath, buf, size - 1);
    if(res == -1)
        return -errno;

    buf[res] = '\0';
    return 0;
}


static int hm_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
    printf("xmp_getdir: %s\n", path);
    DIR *dp;
    struct dirent *de;
    int res = 0;
    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);

    dp = opendir(fpath);
    if(dp == NULL)
        return -errno;

    while((de = readdir(dp)) != NULL) {
        res = filler(h, de->d_name, de->d_type);
        if(res != 0)
            break;
    }

    closedir(dp);
    return res;
}

static int hm_mknod(const char *path, mode_t mode, dev_t rdev)
{
    printf("xmp_mknod: %s\n", path);
    int res;
    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);

    res = mknod(fpath, mode, rdev);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_mkdir(const char *path, mode_t mode)
{
    printf("xmp_mkdir: %s\n", path);
    int res;

char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = mkdir(fpath, mode);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_unlink(const char *path)
{
    printf("xmp_unlink: %s\n", path);
    int res;
char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = unlink(fpath);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_rmdir(const char *path)
{
    printf("xmp_rmdir: %s\n", path);
    int res;

char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = rmdir(fpath);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_symlink(const char *from, const char *to)
{
    printf("xmp_symlink: %s\n", path);
    int res;
char fpath [1000], ffrom[1000],fto[1000];
    sprintf(fto,"%s%s",dirpath,to);
    sprintf(ffrom,"%s%s",dirpath,from);
   
   
    res = symlink(ffrom, fto);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_rename(const char *from, const char *to)
{
    printf("xmp_rename: %s\n", path);
    int res;

    char fpath [1000], ffrom[1000],fto[1000];
    sprintf(fto,"%s%s",dirpath,to);
    sprintf(ffrom,"%s%s",dirpath,from);
    res = rename(ffrom, fto);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_link(const char *from, const char *to)
{
    printf("xmp_link: %s\n", path);
    int res;

    char fpath [1000], ffrom[1000],fto[1000];
    sprintf(fto,"%s%s",dirpath,to);
    sprintf(ffrom,"%s%s",dirpath,from);
    res = link(ffrom, fto);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_chmod(const char *path, mode_t mode)
{
    printf("xmp_chmod: %s\n", path);
    int res;

    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = chmod(fpath, mode);
    if(res == -1)
        return -errno;
   
    return 0;
}

static int hm_chown(const char *path, uid_t uid, gid_t gid)
{
    printf("xmp_chown: %s\n", path);
    int res;

    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = lchown(fpath, uid, gid);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_truncate(const char *path, off_t size)
{
    printf("xmp_truncate: %s\n", path);
    int res;

    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);   
    res = truncate(fpath, size);
    if(res == -1)
        return -errno;

    return 0;
}

static int hm_utime(const char *path, struct utimbuf *buf)
{
    printf("xmp_utime: %s\n", path);
    int res;
   
    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = utime(fpath, buf);
    if(res == -1)
        return -errno;

    return 0;
}


static int hm_open(const char *path, int flags)
{
    printf("xmp_open: %s\n", path);
    int res;

    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = open(fpath, flags);
    if(res == -1)
        return -errno;

    close(res);
    return 0;
}

static int hm_read(const char *path, char *buf, size_t size, off_t offset)
{
    printf("xmp_read: %s\n", path);
    int fd;
    int res;

    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    fd = open(fpath, O_RDONLY);
    if(fd == -1)
        return -errno;

    strcpy(globalpath,fpath);

    res = pread(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
   
    close(fd);
    return res;
}

static int hm_write(const char *path, const char *buf, size_t size,
                     off_t offset)
{
    printf("xmp_write: %s\n", path);
    int fd;
    int res;

        chmod(globalpath,0000);
    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    fd = open(fpath, O_WRONLY);
    if(fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if(res == -1)
        res = -errno;
   
    close(fd);
    return res;
}

/*static int xmp_statfs(struct fuse_statfs *fst)
{
    struct statfs st;
    int rv = statfs("/",&st);
    if(!rv) {
        fst->block_size  = st.f_bsize;
        fst->blocks      = st.f_blocks;
        fst->blocks_free = st.f_bavail;
        fst->files       = st.f_files;
        fst->files_free  = st.f_ffree;
        fst->namelen     = st.f_namelen;
    }
    return rv;
}
*/
static int hm_release(const char *path, int flags)
{
    printf("xmp_release: %s\n", path);
    /* Just a stub.  This method is optional and can safely be left
       unimplemented */
    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    (void) fpath;
    (void) flags;
    return 0;
}

static int hm_fsync(const char *path, int isdatasync)
{
    printf("xmp_fsync: %s\n", path);
    /* Just a stub.  This method is optional and can safely be left
       unimplemented */
    char fpath [1000];
    sprintf(fpath,"%s%s",dirpath,path);
    (void) fpath;
    (void) isdatasync;
    return 0;
}

static struct fuse_operations hm_oper = {
    .getattr    = hm_getattr,
    .readlink    = hm_readlink,
    .getdir    = hm_getdir,
    .mknod    = hm_mknod,
    .mkdir    = hm_mkdir,
    .symlink    = hm_symlink,
    .unlink    = hm_unlink,
    .rmdir    = hm_rmdir,
    .rename    = hm_rename,
    .link    = hm_link,
    .chmod    = hm_chmod,
    .chown    = hm_chown,
    .truncate    = hm_truncate,
    .utime    = hm_utime,
    .open    = hm_open,
    .read    = hm_read,
    .write    = hm_write,
   // .statfs    = xmp_statfs,
    .release    = hm_release,
    .fsync    = hm_fsync
   
};

int main(int argc, char *argv[])
{
    fuse_main(argc, argv, &hm_oper);
    return 0;
} 
