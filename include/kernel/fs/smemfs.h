#ifndef __CASIO_SMEM_H__
# define __CASIO_SMEM_H__

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <kernel/fs/file.h>



/*******************************************/
/**                                       **/
/**        Common SMEM driver part        **/
/**                                       **/
/*******************************************/
extern void smemfs_initialize(void);
extern void smemfd_uninitialize(void);




/*******************************************/
/**                                       **/
/** USB Power Graphic II SMEM driver part **/
/**                                       **/
/*******************************************/
// Internal superblock use by the USB3 abstractions
struct smemfs_USB3_superblock
{
	struct smemfs_USB3_inode *root_inode;
	struct smemfs_USB3_inode *fake_root_inode;
};

// Internal struct used to store SMEM dump
struct smemfs_USB3_inode
{
	// File name
	char name[32];

	// Internal file's informations
	int type;
	size_t fsize;
	size_t dsize;

	// Internal abstraction informations
	struct smemfs_USB3_inode *child;
	struct smemfs_USB3_inode *sibling;
	struct smemfs_USB3_inode *parent;
};

// Superblock primitives
extern void *	smemfs_USB3_mount(void);
extern int	smemfs_USB3_umount(void);

// File primitives
extern ssize_t	smemfs_USB3_read(void *inode, void *buf, size_t count, off_t pos);

// Inode primitive
extern void *	smemfs_USB3_find_parent(void *inode);
extern void *	smemfs_USB3_find_first_child(void *inode);
extern void *	smemfs_USB3_find_next_sibling(void *inode);
extern int	smemfs_USB3_get_name(void *inode, char *name, size_t count);
extern mode_t	smemfs_USB3_get_mode(void *inode);

// Superblock
extern struct smemfs_USB3_inode *smemfs_USB3_alloc_inode(void);

/*******************************************/
/**                                       **/
/** USB Power Graphic II SMEM driver part **/
/**                                       **/
/*******************************************/
//TODO: update this part !!
// Define internal SMEM informations /flags
#define CASIO_SMEM_NAME_LENGHT			12
#define CASIO_SMEM_ROOT_ID			0xffff
#define CASIO_SMEM_BLOCK_ENTRY_MAGIC		0x4200
#define CASIO_SMEM_HEADER_INFO_EXIST		0x51
#define CASIO_SMEM_HEADER_INFO_DELETE		0x01
#define CASIO_SMEM_HEADER_TYPE_DIRECTORY	0x10
#define CASIO_SMEM_HEADER_TYPE_FILE		0x20
#define CASIO_SMEM_FRAGMENT_INFO_EXIST		CASIO_SMEM_HEADER_INFO_EXIST
#define CASIO_SMEM_FRAGMENT_INFO_DELETE		CASIO_SMEM_HEADER_INFO_DELETE
#define CASIO_SMEM_FRAGMENT_MAGIC		0x0120
#define CASIO_SMEM_ROOT_ID			0xffff

// Define block data. 
struct casio_smem_block_s
{
	uint16_t magic_start;		// always 0x4200
	uint16_t entry_number;		// indicate the block number
	uint32_t offset;		// offset in the memory (sector ?)

	struct {
		uint8_t used;		// Indicate if the block is used (0xff yes, 0x00 no)
		uint8_t fileTable;	// Indicate if the block is used by File Header Table.
		uint16_t id;		// Indicate the block ID in the File Header Table.
	} info;

	uint8_t magic_end[20];		// Content always 0xff 0xff 0xff 0xff 0x00 0x00 0x00 0x01 0xff 0xff...
};

// Define file header.
struct casio_smem_header_s
{
	uint8_t info;			// 0x51 if entry exist, 0x01 if entry refer to delete file. (?)
	uint8_t type;			// 0x10 for a directory, 0x20 for preheader.
	uint16_t id;			// Indicate the block entry ID (directory)

	struct {
		uint16_t type;		// 0xffff for root, 0x0110 for directory
		uint16_t id;		// directory ID, 0xffff for root
	} parent;

	uint16_t name[12];		// File name
};

// Define frament data.
struct casio_smem_fragment_s
{
	uint8_t info;			// 0x51 if entry exit, 0x01 if entry refer to delete file.
	uint8_t unknown0;		// always 0x30
	uint16_t id;			// Fragment ID.
	uint16_t magic;			// always 0x0120
	uint16_t file_id;		// File ID.
	uint16_t unknown1;		// 0x0002 (?)
	uint16_t frag_total;		// total number of fragments for this file.
	uint16_t frag_current_number;	// Current fragment number.
	uint16_t data_block_id;		// ID of blocks containing data for this fragment
	uint16_t data_offset;		// offset of fragment data in the given block

	// WARNING :
	// The size is (real_size-1), so *do not forget* to add 1 to
	// have the real size of data!
	uint16_t data_size;		// size of this fragment data

	uint8_t fill[12];		// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, ...
};

// Define internal typedef
typedef struct casio_smem_block_s smemfs_sector_t;
typedef struct casio_smem_header_s smemfs_inode_t;
typedef struct casio_smem_fragment_s smemfs_fragdata_t;
typedef struct casio_smem_header_s smemfs_header_t;


// Define internal superblock
struct smemfs_USB2_superblock
{
	smemfs_inode_t *inode_table;
	smemfs_sector_t  *sector_table;
};


// Superblock primitives
extern void *	smemfs_USB2_mount(void);
extern int	smemfs_USB2_umount(void);

// File primitives
extern ssize_t	smemfs_USB2_read(void *inode, void *buf, size_t count, off_t pos);

// Inode primitive
extern void *	smemfs_USB2_find_parent(void *inode);
extern void *	smemfs_USB2_find_first_child(void *inode);
extern void *	smemfs_USB2_find_next_sibling(void *inode);
extern int	smemfs_USB2_get_name(void *inode, char *name, size_t count);
extern mode_t	smemfs_USB2_get_mode(void *inode);


// Internal helper !
#define WALK_FLAG_ID_CHECK_PARENT	(1 << 0)
#define WALK_FLAG_ID_CHECK_DIRECTORY	(0 << 0)
extern smemfs_inode_t *smemfs_USB2_walk(smemfs_inode_t *current,
					smemfs_inode_t *entry, uint16_t folder_id, int flags);

#endif /*_CASIO_SMEM_H__*/
