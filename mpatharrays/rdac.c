#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <structs.h>
#include <sg_include.h>
#include <debug.h>
#include "libmpatharrays.h"

#define INQUIRY_CMD	0x12
#define INQUIRY_CMDLEN	6

#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 8
#endif

/**
 * debug function
 * source: http://grapsus.net/blog/post/Hexadecimal-dump-in-C
 * */
void hexdump(void *mem, unsigned int len)
{
        unsigned int i, j;
        
	printf("Dumping memory address 0x%x\n", mem);

        for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
        {
                /* print offset */
                if(i % HEXDUMP_COLS == 0)
                {
                        printf("0x%06x: ", i);
                }
 
                /* print hex data */
                if(i < len)
                {
                        printf("%02x ", 0xFF & ((char*)mem)[i]);
                }
                else /* end of block, just aligning for ASCII dump */
                {
                        printf("   ");
                }
                
                /* print ASCII dump */
                if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
                {
                        for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
                        {
                                if(j >= len) /* end of block, not really printing */
                                {
                                        putchar(' ');
                                }
                                else if(isprint(((char*)mem)[j])) /* printable char */
                                {
                                        putchar(0xFF & ((char*)mem)[j]);        
                                }
                                else /* other char */
                                {
                                        putchar('.');
                                }
                        }
                        putchar('\n');
                }
        }
}

/*
 * Send an inquiry to the specified page
 */
int
send_inq(int page, unsigned char * sense_buffer, const char *dev, int fd)
{
        unsigned char sb[128];
        unsigned char inqCmdBlk[INQUIRY_CMDLEN] = {INQUIRY_CMD, 1, page, 0,
                                                sizeof(sb), 0};
        struct sg_io_hdr io_hdr;
        int ret = 0;

        memset(&io_hdr, 0, sizeof (struct sg_io_hdr));
        memset(sense_buffer, 0, 256);
        io_hdr.interface_id = 'S';
        io_hdr.cmd_len = sizeof (inqCmdBlk);
        io_hdr.mx_sb_len = sizeof (sb);
        io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
        io_hdr.dxfer_len = sizeof (unsigned char) * 256;
        io_hdr.dxferp = sense_buffer;
        io_hdr.cmdp = inqCmdBlk;
        io_hdr.sbp = sb;
        io_hdr.timeout = 60000;
        io_hdr.pack_id = 0;
        if (ioctl(fd, SG_IO, &io_hdr) < 0) {
<<<<<<< HEAD
		return 0;
        }
        if (io_hdr.info & SG_INFO_OK_MASK) {
		return 0;
=======
	//	printf("Sending inq command failed\n");
		return NULL;
//                pp_rdac_log(0, "sending inquiry command failed");
     //           goto out;
        }
        if (io_hdr.info & SG_INFO_OK_MASK) {
//		printf("error\n");
		return NULL;
   //             pp_rdac_log(0, "inquiry command indicates error");
 //              goto out;
>>>>>>> fce1916b6fa66b11269033937324ebc0da7a4d79
        }

	return 1;
}


<<<<<<< HEAD
extern int 
rdac_inquirer(struct mpa_pathinq * mpap, struct path * pp)
{
	unsigned char sense_buffer[256];

	if( !mpap->array_id ) {
=======
extern int rdac_inquirer(struct mparray_pathinq * mpap, struct path * pp)
{
	unsigned char * sense_buffer;

	if( !mpap->array_id ) {
		printf("ALLOCATE\n");
>>>>>>> fce1916b6fa66b11269033937324ebc0da7a4d79
		mpap->controller_id = malloc(sizeof(unsigned char));
		mpap->array_id = malloc(sizeof(unsigned char) * 16);
		mpap->array_label = malloc(sizeof(unsigned char) * 60);
	}
<<<<<<< HEAD
	
	/* Get the array ID from VPD page 0xC8 */
	if( send_inq(0xc8, sense_buffer, pp->dev, pp->fd) ) {
=======
	if( (sense_buffer = send_inq(0xc8, pp->dev, pp->fd))!= NULL) {
>>>>>>> fce1916b6fa66b11269033937324ebc0da7a4d79
		/* Copy the 16 byte array identifier out of the sense buffer */
		memcpy(mpap->array_id, sense_buffer+90, 16);
	} else
		goto error;

<<<<<<< HEAD
        /* Get the controller slot ID */
        if ( send_inq(0xc4, sense_buffer, pp->dev, pp->fd) ) {
		/* Controller Slot A */
                if (sense_buffer[29] == 0x31)
                        memset( mpap->controller_id, 0x41, 1);
		/* Controller Slot B */
                else if (sense_buffer[29] == 0x32)
                        memset( mpap->controller_id, 0x42, 1);
		/* We've hit an error */
		else
			goto error;
        } else 
		goto error;

       /* Get ownership info from Volume Access Control page (0xC9) */
        if ( send_inq(0xc9, sense_buffer, pp->dev, pp->fd) ) {
                /* Volume Current Ownership */
                if ( (sense_buffer[8] & 0x01) == 0x01 )
                        mpap->owner = 1;
                else
=======
	free(sense_buffer);
	
        /* Get the controller slot ID */
        if ( (sense_buffer = send_inq(0xc4, pp->dev, pp->fd)) != NULL) {
                if (sense_buffer[29] == 0x31) {
                        memset( mpap->controller_id, 0x41, 1);
                } else if (sense_buffer[29] == 0x32) {
                        memset( mpap->controller_id, 0x42, 1);
                }
        } else 
		goto error;

        free(sense_buffer);

       /* Get ownership infos */
        if ( (sense_buffer = send_inq(0xc9, pp->dev, pp->fd)) != NULL) {
                /* Volume Current Ownership */
                if ( (sense_buffer[8] & 0x01) == 0x01 ) {
                        mpap->owner = 1;
                } else
>>>>>>> fce1916b6fa66b11269033937324ebc0da7a4d79
                        mpap->owner = 0;

                /* Volume preferred path */
                switch ( sense_buffer[9] & 0x0F ) {
                case 0x01:
                        mpap->preferred = 1;
                        break;
                case 0x02:
                        mpap->preferred = 0;
                        break;
                default:
                        mpap->preferred = -1;
                        break;
                }
        } else
		goto error;
<<<<<<< HEAD
	
	return 1;
error:
=======
        free(sense_buffer);
	
	return 1;
error:
	free(sense_buffer);
>>>>>>> fce1916b6fa66b11269033937324ebc0da7a4d79
	return 0;
}
