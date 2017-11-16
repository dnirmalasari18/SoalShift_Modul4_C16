#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
static const char *dirpath = "/home/dell/Dokumen";

static int hm_getattr(const char *path, struct stat *stbuf)
{
	printf("xmp_getattr: %s\n", path);
	int res;
	char fpath[1000];
 //   char newFile[1000];
  //  memcpy(newFile, path, strlen(path));
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int hm_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	printf("xmp_readdir: %s\n", path);
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int hm_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	printf("xmp_read: %s\n", path);
	char fpath[1000];
    char newFile[1000];
    sprintf(fpath,"%s",path);
	int res = 0;
	int fd = 0 ;
    if(strcmp(path,"/") == 0)
	{
		memcpy(newFile, path, strlen(path));
		// path=dirpath;
		sprintf(fpath,"%s",newFile);
	}
	else {
		memcpy(newFile, path, strlen(path));
		//newFile[strlen(path)] = '\0';
		sprintf(fpath, "%s%s",dirpath,newFile);
	}
	int cek=0;
    if(fpath[strlen(fpath)-4]=='.'){
        if(fpath[strlen(fpath)-3] == 'p' && fpath[strlen(fpath)-2]== 'd' && fpath[strlen(fpath)-1]=='f') cek = 1;
        else if(fpath[strlen(fpath)-3] == 'd' && fpath[strlen(fpath)-2]== 'o' && fpath[strlen(fpath)-1]=='c') cek = 1;
        else if(fpath[strlen(fpath)-3] == 't' && fpath[strlen(fpath)-2]== 'x' && fpath[strlen(fpath)-1]=='t') cek = 1;
        }
	printf("cek dari %s adalah %d\n",fpath,cek);
    if(cek==0){
        (void) fi;
        fd = open(fpath, O_RDONLY);
        if (fd == -1)
            return -errno;

        res = pread(fd, buf, size, offset);
        if (res == -1)
            res = -errno;

        close(fd);
        return res;
    }
    else {
        system("zenity --error --title=\"Pesan error\" --text=\"Terjadi Kesalahan! File berisi konten berbahaya.\n\"");
        char temp[1000];
        sprintf(temp,"%s.ditandai",fpath);
        printf("fpathnya %s tempnya %s\n",fpath,temp);
        char aa[1000],bb[1000];
        sprintf(bb,"chmod 000 %s",fpath);
        system(bb);
        mkdir("/home/dell/Dokumen/rahasia",0777);
        
        rename (fpath,temp);
      //kan dari /home/dell/dokumen di move ke /home/dell/Dokumen/rahasia
                //no2
 //      struct stat st = {0};

   //     if (stat("/home/dell/Dokumen/rahasia", &st) == -1) {
        mkdir("/home/dell/Dokumen/rahasia",0755);
     //   }
        char temp2[1000];
        sprintf(temp2,"%s/rahasia%s.ditandai",dirpath,path);
        printf("temp2 %s\n",temp2);
        rename(temp,temp2);
        //sprintf(aa,"rm %s",fpath)
      //  
        //sprintf (aa, "mv %s /home/dell/Dokumen/rahasia",fpath);
        //system(aa);
    }
}

static struct fuse_operations xmp_oper = {
	.getattr	= hm_getattr,
	.readdir	= hm_readdir,
	.read		= hm_read,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &hm_oper, NULL);
}
