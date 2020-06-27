#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<linux/fs.h>
#include<ext2fs/ext2_fs.h>
#include<sys_types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<uuid/uuid.h>
#include<math.h>

int main(int argc, char *argv[]) {
	unsigned int block_size;
	unsigned int size;
	int i;
	FILE *fp;
	struct ext2_super_block sb;
	struct ext2_group_desc bgds;
	struct ext2_inode in;
	struct ext2_dir_entry_2 dirent;
	fp = fopen(argv[1], "w");
	if(fp == NULL){
		printf("Could not open");
		exit(0);
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	if(argc == 3)
		block_size = atoi(argv[2]);
	else
		block_size = 4096;
	fseek(fd, 1024, SEEK_CUR);
	sb.s_log_block_size = block_size >> 11;
	sb.s_log_cluster_size = block_size >> 11;
	sb.s_inode_size = 256;
	sb.s_blocks_per_group = 8 * block_size;
	unsigned int inodes_per_block_group = block_size / 8;
	sb.s_inodes_per_group = inodes_per_block_group * block_size / sb.s_inode_size;
	unsigned int block_groups = ceil((float)sb.s_blocks_count / sb.s_blocks_per_group);
	sb.s_inodes_count = sb.s_inodes_per_group * block_groups;
	sb.s_blocks_count = size / block_size;		
	sb.s_r_blocks_count = sb.s_blocks_count * 0.05;
	sb.s_free_blocks_count = sb.s_blocks_count;	
	sb.s_free_inodes_count = sb.s_inodes_count;
	if(sb.s_log_block_size)	
		sb.s_first_data_block = 0;
	else
		sb.s_first_data_block = 1; 
	sb.s_clusters_per_group = sb.s_blocks_per_group;
	sb.s_mtime = 0;
	sb.s_wtime = time(NULL);
	sb.s_mnt_count = 0;
	sb.s_max_mnt_count = -1;
	sb.s_magic = EXT2_SUPER_MAGIC;
	sb.s_state = EXT2_VALID_FS;
	sb.s_errors = EXT2_ERRORS_CONTINUE;
	sb.s_minor_rev_leveL = 0;
	sb.s_lastcheck = time(NULL);
	sb.s_checkinterval = 0;
	sb.s_creator_os = EXT2_OS_LINUX;
	sb.s_rev_level = EXT2_DYNAMIC_REV;
	sb.s_def_resuid = 0;
	sb.s_def_resgid = 0;
	sb.s_first_ino = 11;
	sb.s_block_group_nr = 0;
	sb.s_feature_compat = 56;
	sb.s_feature_incompat = 2;
	sb.s_feature_ro_compat = 3;
	uuid_generate(sb.s_uuid);
	for(i = 0; i < 16; i++)
		sb.s_volume_name[i] = '\0';
	for(i = 0; i < 64; i++)
		sb.s_last_mounted[0] = '\0';
	sb.s_algorithm_usage_bitmap = 0;
	sb.s_prealloc_blocks = 0;
	sb.s_prealloc_dir_blocks = 0;
	sb.s_reserved_gdt_blocks = 256;
	for(i = 0; i < 16; i++)
		sb.s_journal_uuid[i] = 0;
	sb.s_journal_inum = 0;
	sb.s_journal_dev = 0;
	sb.s_last_orphan = 0;
	uuid_generate((unsigned char *) sb.s_hash_seed);
	sb.s_def_hash_version = 1;
	sb.s_jnl_backup_type = 0;
	sb.s_desc_size = 0;
	sb.s_default_mount_opts = 12;
 	sb.s_first_meta_bg = 0;
	sb.s_mkfs_time = time(NULL);
	for(int i = 0; i < 17; i++){
		sb.s_jnl_blocks[i] = 0;}
	sb.s_blocks_count_hi = 0;
	sb.s_r_blocks_count_hi = 0;
	sb.s_free_blocks_hi = 0;
	sb.s_min_extra_isize = 32;
	sb.s_want_extra_isize = 32;
	sb.s_flags = 1;
	sb.s_raid_stride = 0;
	sb.s_mmp_update_interval = 0;
	sb.s_mmp_block = 0;
	sb.s_raid_stripe_width = 0;
	sb.s_log_groups_per_flex = 0;
	sb.s_reserved_pad = 0;
	sb.s_kbytes_written = 0;
	sb.s_snapshot_inum = 0;
	sb.s_snapshot_id = 0;
	sb.s_snapshot_r_blocks_count = 0;
	sb.s_snapshot_list = 0;
	sb.s_error_count = 0;
	sb.s_first_error_time = 0;
	sb.s_first_error_ino = 0;
	sb.s_first_error_block = 0;
	for(i = 0; i < 32; i++)
		sb.s_first_error_func[i] = 0;
	sb.s_first_error_line = 0;
	sb.s_last_error_time = 0;
	sb.s_last_error_ino = 0;
	sb.s_last_error_line = 0;
	sb.s_last_error_block = 0;
	for(i = 0; i < 32; i++)	
		sb.s_last_error_func[i] = 0;
	for(i = 0; i < 64; i++)
		sb.s_mount_opts[i] = 0;
	sb.s_usr_quota_inum = 0;
	sb.s_grp_quota_inum = 0;
	sb.s_overhead_blocks = 0;
	for(i = 0; i < 2; i++)
		sb.s_backup_bgs[i] = 0;
	for(i = 0; i < 106; i++)
		sb.s_reserved[i] = 0;
	sb.s_checksum = 0;

	fwrite(&sb, sizeof(ext2_super_block), 1, fp);
	fseek(fp, block_size, SEEK_SET);
	
	bgds.bg_block_bitmap = sb.s_reserved_gdt_blocks + 2;
	bgds.bg_inode_bitmap = bgds.bg_block_bitmap + 1;
	bgds.bg_inode_table = bgds.bg_inode_bitmap + 1;
	bgds.bg_free_blocks_count = sb.s_blocks_count;
	bgds.bg_free_inodes_count = sb.s_inodes_count;
	bgds.bg_used_dirs_count = 2;
	bgds.bg_flags = 4;
	bgds.bg_exclude_bitmap_lo = 0;
	bgds.bg_block_bitmap_csum_lo = 0;
	bgds.bg_inode_bitmap_csum_lo = 0;
	bgds.bg_itable_unused = 0;
	bgds.bg_checksum = 0;

	fwrite(&bgds, sizeof(ext2_group_desc), 1, fp);
	fseek(fp, bgdesc.bg_inode_table * block_size + 2*sizeof(struct ext2_inode), SEEK_SET);

	in.i_mode = ;
	in.i_uid = 0;
	in.i_size = 4096;
	in.i_atime = time(NULL);
	in.i_ctime = time(NULL);
	in.i_mtime = time(NULL);
	in.i_dtime = 0;
	in.i_gid;
	in.i_links_count = 1;
	in.i_blocks = 8;
	in.i_flags = 0;
	in.osd1.linux1.l_i_version = 0;
	in.osd1.hurd1.h_i_translator = 0;
	in.i_generation = 0;
	in.i_file_acl = 0;
	in.i_size_high = 0;
	in.i_faddr = 0;
	in.osd2.linux2.l_i_blocks_hi = 0;
	in.osd2.linux2.l_i_file_acl_high = 0;
	in.osd2.linux2.l_i_uid_high = 0;
	in.osd2.linux2.l_i_gid_high = 0;
	in.osd2.linux2.l_i_checksum_lo = 0;
	in.osd2.linux2.l_i_reserved = 0;
	in.osd2.hurd2.h_i_frag = 0;
	in.osd2.hurd2.h_i_fsize = 0;
	in.osd2.hurd2.h_i_mode_high = 0;
	in.osd2.hurd2.h_i_uid_high = 0;
	in.osd2.hurd2.h_i_gid_high = 0;
	in.osd2.hurd2.h_i_author = -1;
	in.i_block[0] = ;
	for(i = 1; i < EXT2_N_BLOCKS; i++)
		in.i_block[i] = 0;

	fwrite(&in, sizeof(ext2_inode), 1, fp);
	fseek(fp, inode.i_block[0] * block_size, SEEK_SET);
	dirent.inode = 2;
	dirent.rec_len = 12;
	dirent.name_len = 1;
	dirent.file_type = 2;
	dirent.name = ".";

	fwrite(&dirent, sizeof(ext2_dir_entry_2), 1, fp);
	fseek(fp, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	dirent.inode = 2;
	dirent.rec_len = 12;
	dirent.name_len = 2;
	dirent.file_type = 2;
	dirent.name = "..";

	fwrite(&dirent, sizeof(ext2_dir_entry_2), 1, fp);
	fseek(fp, dirent.rec_len - sizeof(struct ext2_dir_entry_2), SEEK_CUR);
	dirent.inode = 11;
	dirent.rec_len = 4072;
	dirent.name_len = 10;
	dirent.file_type = 2;
	dirent.name = "lost+found";
	fwrite(&dirent, sizeof(ext2_dir_entry_2), 1, fp);

fclose(fp);
return 0;
}



