#ifndef __DEW_HASH_H__
#define __DEW_HASH_H__

#ifdef __KERNEL__ /* Comment #1 */

#include <linux/kernel.h>

/* Comment #2 */
int dew_hash_get(struct file* file, unsigned int left_size, unsigned char *md5);

#else

/* Comment #3 */
int dew_hash_get(const char *file_path, unsigned int left_size, unsigned char *md5);

#endif
#endif // __DEW_HASH_H__
