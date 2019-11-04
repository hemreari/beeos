#ifndef FS_H
#define FS_H

#include "../cpu/types.h"

#define FS_FILE		0x01
#define FS_DIRECTORY	0x02
#define FS_CHARDEVICE	0x03
#define FS_BLOCKDEVICE	0x04
#define FS_PIPE		0x05
#define FS_SYMLINK		0x06
#define FS_MOUNTPOINT	0x08

struct fs_node;

typedef u32 (*read_type_t) (struct fs_node*, u32, u32, u8*);
typedef u32 (*write_type_t) (struct fs_node*, u32, u32, u8*);
typedef void (*open_type_t) (struct fs_node*);
typedef void (*close_type_t) (struct fs_node*);
typedef struct dirent * (*readdir_type_t) (struct fs_node*, u32);
typedef struct fs_node * (*finddir_type_t) (struct fs_node*, char *name);

typedef struct fs_node {
	 char name[128];	//filename
	 u32 mask;			//permission mask
	 u32 uid;			//owning user
	 u32 gid;			//owning group
	 u32 flags;		//node type
	 u32 inode;		//help to fs to identify files
	 u32 length;		//size of the file(bytes)
	 u32 impl;			//an implementation-defined number(?)
	 read_type_t read;
	 write_type_t write;
	 open_type_t open;
	 close_type_t close;
	 readdir_type_t readdir;
	 finddir_type_t finddir;
	 struct fs_node *prt;	//used by mountpoints and symlinks
} fs_node_t;

struct dirent {
	 char name[128]; //filename
	 u32 ino;		//inode number required by POSIX
};

extern fs_node_t *fs_root; //the root of the fs

u32 read_fs(fs_node_t *node, u32 offset, u32 size, u8 *buffer);
u32 write_fs(fs_node_t *node, u32 offset, u32 size, u8 *buffer);
void open_fs(fs_node_t *node, u8 read, u8 write);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, u32 index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);
