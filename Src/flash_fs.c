/*
 * flash_fs.c
 *
 *  Created on: 24.08.2016
 *      Author: Sebastian Förster
 *      License: GPLv2
 */


// use this link: http://fuxx.h1.ru/bin2c/index.en.php
// to convert a file to a C array
#include "files/my_config_files.h"
#include "flash_fs.h"

#define FILE_FS_FILE_DESC_OFFSET	10

#define FILE_FS_NUMBER_OF_FILES		2

static const char *file_names[FILE_FS_NUMBER_OF_FILES] =
{
		"config.txt",
		"config_dbg.txt"
};

flash_file my_files[FILE_FS_NUMBER_OF_FILES] =
{
	{
			config_txt, sizeof(config_txt), -1, 0
	},
	{
			config_dbg_txt, sizeof(config_dbg_txt), -1, 0
	}
};

void flash_fs_init(void)
{
	//reset all files
	for(int i = 0; i < FILE_FS_NUMBER_OF_FILES ; i++ )
	{
		my_files[i].file_num = -1;
	}
}

int flash_open_file(const char *file_name)
{
	//search for the filename
	for(int i = 0; i < FILE_FS_NUMBER_OF_FILES ; i++ )
	{
		if(strcmp(file_name, file_names[i]) == 0)
		{
			my_files[i].file_num = i + FILE_FS_FILE_DESC_OFFSET; //open file
			my_files[i].file_pointer = 0; //point to file start
			return my_files[i].file_num; //return file desc
		}
	}

	return -1; //no file found
}

int flash_close_file(int file_desc)
{
	for(int i = 0; i < FILE_FS_NUMBER_OF_FILES ; i++ )
	{
		if(file_desc == my_files[i].file_num)
		{
			my_files[i].file_num = -1; //close file
			return 0;
		}
	}

	return -1;
}

int flash_read_text_file(int file_desc, char *buf, int len)
{
	for(int i = 0; i < FILE_FS_NUMBER_OF_FILES ; i++ )
	{
		if(file_desc == my_files[i].file_num)
		{
			for (int DataIdx = 0; DataIdx < len; DataIdx++)
			{
				if(my_files[i].file_pointer < my_files[i].size)
				{
					//delete '\r' in line ending -> win and unix merge
					//because of this hack, the driver is not for binary files
					if(my_files[i].file[my_files[i].file_pointer] != '\r')
					{
						*buf++ = my_files[i].file[my_files[i].file_pointer];
					}
					my_files[i].file_pointer++;

					//end reading if a line ends
					//because of this hack, the driver is not for binary files
					if(my_files[i].file[my_files[i].file_pointer - 1] == '\n')
						return DataIdx;

				}
				else {
					return DataIdx;
				}
			} //from zero to length

			return len;
		}//actual file -> opened file
	}//all files

	return 0;
}

int flash_seek_file(int file_desc, int ptr, int dir)
{
	for(int i = 0; i < FILE_FS_NUMBER_OF_FILES ; i++ )
	{
		if(file_desc == my_files[i].file_num)
		{
			//save file pointer
			int temp_index = my_files[i].file_pointer;

			//from start of file
			if(dir == 0)
			{
				temp_index = ptr;
			}
			//from actual pos
			else if(dir == 1)
			{
				temp_index += ptr;
			}
			//from end of file
			else if(dir == 2)
			{
				temp_index = (int)my_files[i].size + ptr;
			}

			if(temp_index >= 0 && temp_index < my_files[i].size)
				my_files[i].file_pointer = temp_index;

			return my_files[i].file_pointer;
		}//actual file -> opened file
	}//all files

	return 0;
}
