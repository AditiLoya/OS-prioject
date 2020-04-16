#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<linux/fs.h>
#include<ext2fs/ext2_fs.h>
#include<sys_types.h>
#include<sts/stat.h>
#include<unistd.h>
#include<uuid/uuid.h>

int main(int argc, char *argv[]) {
	unsigned int block_size;
	unsigned int size;
	int i;
	FILE *fp;
	ext2_super_block sb;
	fp = fopen(argv[1], "r");
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
	sb.s_free_blocks_count;	
	sb.s_free_inodes_count;
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
fclose(fp);
return 0;
}



