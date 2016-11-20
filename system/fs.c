#include<xinu.h>
#include<kernel.h>
#include <stddef.h>
#include<string.h>

#if FS
#include<fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define DEBUG 0

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;


#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int inode_id=1;

int fileblock_to_diskblock(int dev, int fd, int fileblock);

int get_next_free_block(){
    int i,block;
    //might as well search linearly
    for(i=0;i<fsd.nblocks;i++){
        if(fs_getmaskbit(i)==0){
            return i;
        }
    }
    fprintf(stderr, "fs::get_next_free_block(): cannot find a free block.\n\r");
    return -1;
}

int fs_open(char *filename, int flags) // TO-DO: what to do with flags?
{   
#if FS
    int i;
    struct inode in;
    int inode_num;
    
    // Look up file name in root dir
    struct directory dir = fsd.root_dir;
    
    for (i = 0; i < dir.numentries; i++){
        // If root dir has entry for filename of interest
        if (strcmp(dir.entry[i].name, filename) == 0){
            // Get its inode_num
            inode_num = dir.entry[i].inode_num;
	
            // Get inode based on inode_num found above
            // fs_get_inode_by_num(int dev, int inode_number, struct inode *in)
            fs_get_inode_by_num(dev0, inode_num, &in);

            // Put stuff in open file table
            oft[next_open_fd].state = FSTATE_OPEN;
            oft[next_open_fd].fileptr = 0;
            oft[next_open_fd].de = &(dir.entry[i]);
	        memcpy(&(oft[next_open_fd].in),&in,sizeof(struct inode));
            
            // Increment next open fd, which is basically the next available index for oft
            next_open_fd++;
            
            return next_open_fd - 1;
        }
    }

    fprintf(stderr, "fs::fs_open(): file does not exist\n\r");
    return SYSERR;
#endif
    if(flags>=0 && flags<=2){
        //get root dir:
        struct directory dir=fsd.root_dir;
        //check if file name is valid. i.e., no file with same name exists
        int i,fileloc=-1;
        for(i=0;i<dir.numentries;i++){
            if(!strcmp(filename,dir.entry[i].name)){
                fileloc=i;
#if 0
                printf("debug:fileloc=%d\n\r",i);
                printf("debug:dir.entry[0].inode_num=%d\n\r",dir.entry[0].inode_num);
#endif
            }
        }
        if(fileloc==-1){
            fprintf(stderr,"error. file %s not found.\n",filename);
        }
        //request entry in filetable:
        int fd=next_open_fd++;
        struct filetable ft;
        ft.state=FSTATE_OPEN;
        ft.fileptr=0;
        ft.de=&(dir.entry[fileloc]);
        strcpy((ft.de)->name, filename);
        struct inode in;
        //read file metadata inode:
        fs_get_inode_by_num(0,ft.de->inode_num,&in);
        //printf("in.id=%d\n",ft.de->inode_num);
        //cp in to ft:
        memcpy(&(ft.in),&in,sizeof(struct inode));
        //copy ft to oft:
        memcpy(oft+fd,&ft,sizeof(struct filetable));

#if 0
        printf("debug: file %s opened. fd=%d,oft[%d].state=%d\n\r",filename,fd,fd,oft[fd].state);
        printf("debug: open: ft.de->ft.in.blocks[0]=%d\n\r",ft.in.blocks[0]);
#endif
         
        return fd;
    }
    else return SYSERR;
}
int fs_close(int fd)
{
    // check if fd is valid
    if ((fd > next_open_fd) || (fd < 0)){
        fprintf(stderr, "fs::fs_close: not valid fd\n\r");
        return SYSERR;
    }

    oft[fd].state = FSTATE_CLOSED;
    return OK;
}
int fs_create(char *filename, int mode)
{
    if(mode==O_CREAT){
        //get root dir:
        struct directory dir=fsd.root_dir;
        //check if file name is valid. i.e., no file with same name exists
        int i;
        for(i=0;i<dir.numentries;i++){
            if(!strcmp(filename,dir.entry[i].name)){
                fprintf(stderr, "fs::fs_create(): file already exists.\n\r");
                return SYSERR;
            }
        }
        //request entry in filetable:
        int fd=next_open_fd++;
        struct filetable ft;
        ft.state=FSTATE_OPEN;
        ft.fileptr=0;
        ft.de=&(dir.entry[dir.numentries++]);
        //printf("dir.num=%d\n\r",dir.numentries);
        strcpy((ft.de)->name, filename);
        //write back to rootdir
        memcpy(&(fsd.root_dir),&dir,sizeof(struct directory));
        struct inode in;
        in.id=inode_id++;
        in.type=INODE_TYPE_FILE;
        //write inode 
        fs_put_inode_by_num(0,in.id,&in);
        //mark destination block:
        int bl = in.id / INODES_PER_BLOCK;
        bl += FIRST_INODE_BLOCK;
        fs_setmaskbit(bl);
        //write inode to filetable
        memcpy(&(ft.in),&in,sizeof(struct inode));
        //write inode_id to dirent:inode_num
        memcpy(&(ft.de->inode_num),&(in.id),sizeof(int));
        memcpy(&(fsd.root_dir.entry[dir.numentries-1].inode_num),&(in.id),sizeof(int));
        //copy ft to oft:
        memcpy(oft+fd,&ft,sizeof(struct filetable));
        //update fsystem
        fsd.inodes_used++;
#if 0
        printf("debug: create:fsd.root_dir.entry[dir.numentries-1].inode_num=%d\n\r",fsd.root_dir.entry[dir.numentries-1].inode_num);
        printf("debug: file %s created. fd=%d,oft[%d].state=%d,ft.in.id=%d\n\r",filename,fd,fd,oft[fd].state,ft.in.id);
#endif
         
        return fd;
    }
    else return SYSERR;
}
int fs_seek(int fd, int offset)
{
    //first, get the file table entry:
    struct filetable ft=oft[fd];
    //update fileptr
    offset=ft.fileptr+offset;
    if(offset<0){
        fprintf(stderr,"fs::fs_seek: offset exceeds file boundary. resetting pointer to beginning of file\n\r");
        offset=0;
    }
    memcpy(&((oft+fd)->fileptr),&offset,sizeof(int));
    return fd;
}
int fs_read(int fd, void *buf, int nbytes)
{
    //printf("fd at fs_read=%d\n",fd);
    int orig_nbytes=nbytes;
    //first, get the file table entry:
    struct filetable ft=oft[fd];
    if(ft.state==FSTATE_CLOSED){
        fprintf(stderr, "error %d. fs::fs_read(): invalid descriptor.\n\r",ft.state);
        return 0;
    }
    struct inode in = ft.in;
    //calculate inode block for fileptr:
    int inodeblk= (ft.fileptr / fsd.blocksz);
    int inodeoffset=(ft.fileptr % fsd.blocksz);
    if (inodeblk<INODEBLOCKS){
        int dst_blk=ft.in.blocks[inodeblk];
#if 0
        struct directory dir=fsd.root_dir;
        printf("in.blocks=%x, rootdir count=%d,nbytes=%d\n",in.blocks,dir.numentries,nbytes);
#endif
        while(nbytes>0){
            //if all data we want is in same block
            if(nbytes<(fsd.blocksz-inodeoffset)){
                bs_bread(0,dst_blk,inodeoffset,buf,nbytes);
                //printf("debug:read:buf=%s,dst_blk=%d,inodeoffset=%d\n",buf,dst_blk,inodeoffset);
                //incr fileptr:
                ft.fileptr+=nbytes;
                buf+=nbytes;
                nbytes=0;
                return orig_nbytes;
            }
            else{
                if(inodeblk==INODEBLOCKS-1){
                    fprintf(stderr, "fs::fs_read(): requested bytes exceeds limit, wrote valid values only.\n\r");
                    return orig_nbytes-nbytes;
                }
                bs_bread(0,dst_blk,inodeoffset,buf,fsd.blocksz-inodeoffset);
#if 0
                printf("debug:fs_read:inodeoffset=%d,nbytes=%d,inodeblk=%d,dst_blk=%d\n",inodeoffset,nbytes,inodeblk,dst_blk);
#endif
                buf+=(fsd.blocksz-inodeoffset);
                nbytes-=(fsd.blocksz-inodeoffset);
                ft.fileptr+=(fsd.blocksz-inodeoffset);
                //write ft to oft
                memcpy(oft+fd,&ft,sizeof(struct filetable));
                //cross over to next block
                dst_blk=in.blocks[++inodeblk];
                inodeoffset=0;
            }
        }
    }
    return orig_nbytes-nbytes;
}
/**
 * return size wrote
 */
int fs_write(int fd, void *buf, int nbytes)
{
    int orig_nbytes=nbytes;
    //first, get the file table entry:
    struct filetable ft=oft[fd];
#if 0
    printf("debug:write:fd=%d\n\r",fd);
#endif
   
    if(ft.state==FSTATE_CLOSED){
        fprintf(stderr, "error %d. fs::fs_write(): invalid descriptor.\n\r",ft.state);
        return 0;
    }
    struct inode in = ft.in;
    //calculate inode block for fileptr:
    int inodeblk= (ft.fileptr / fsd.blocksz);
    int inodeoffset=(ft.fileptr % fsd.blocksz);
    if (inodeblk<INODEBLOCKS){
        int dst_blk;
        while(nbytes>0){
#if 0
            printf("ft.in.id=%d\n",ft.in.id);
#endif
            if(in.blocks[inodeblk]==0){
                //alloc new block
                dst_blk=get_next_free_block();
                memcpy(in.blocks+inodeblk,&dst_blk,sizeof(int));
#if 0
                printf("in.blocks[inodeblk]=%d,expected:%d\n\r",in.blocks[inodeblk],dst_blk);
                
#endif
                //copy in back to ft.in:
                memcpy(&((oft+fd)->in),&(in),sizeof(struct inode));
                ft.in=in;
                //write in back to the inode on disk
                fs_put_inode_by_num(0,in.id,&in);
#if 0
                struct inode temp;
                fs_get_inode_by_num(0,in.id,&temp);
                printf("write. updated inode %d, inode.block[0]=%d\n",temp.id,temp.blocks[0]);
#endif
                //mark that block as visited...
                fs_setmaskbit(dst_blk);
            }
            else if(in.blocks[inodeblk]>0){
                dst_blk=in.blocks[inodeblk]; 
            }
            //if all data we want to write can be put in same block
            if(nbytes<(fsd.blocksz-inodeoffset)){
                bs_bwrite(0,dst_blk,inodeoffset,buf,nbytes);
                //incr fileptr:
                ft.fileptr+=nbytes;
                nbytes=0;
                //write to oft:
                memcpy(oft+fd,&ft,sizeof(struct filetable));
                return orig_nbytes;
            }
            else{
                if(inodeblk==INODEBLOCKS-1){
                    fprintf(stderr, "fs::fs_write(): requested bytes exceeds limit, wrote valid values only.\n\r");
                    return orig_nbytes-nbytes;
                }
                bs_bwrite(0,dst_blk,inodeoffset,buf,fsd.blocksz-inodeoffset);
#if 0
                printf("debug:fs_write:inodeoffset=%d,nbytes=%d,inodeblk=%d,dstblk=%d\n\r",inodeoffset,nbytes,inodeblk,dst_blk);
                char tempbuf[512];
                memcpy(tempbuf,buf,512);
                printf("debug:wrote:%s\n\r",tempbuf);
#endif
                
                buf+=(fsd.blocksz-inodeoffset);
                nbytes-=(fsd.blocksz-inodeoffset);
                ft.fileptr+=(fsd.blocksz-inodeoffset);
                //write to oft:
                memcpy(oft+fd,&ft,sizeof(struct filetable));
                //cross over to next block
                //dst_blk=in.blocks[++inodeblk];
                inodeblk++;
                inodeoffset=0;
            }
        }
    }
    return orig_nbytes-nbytes;
}


int fs_mkfs(int dev, int num_inodes) {
  int i;

  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8;

  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs getmem failed.\n");
    return SYSERR;
  }

  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }

  fsd.inodes_used = 0;

  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));

  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  //custom code: mod 12/2/14:
  //add root dir:
  
  fs_setmaskbit(RT_BLK);
  bs_bwrite(dev0, RT_BLK, 0, &(fsd.root_dir), sizeof(struct directory));


  return 1;
}

int fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

#endif /* FS */
