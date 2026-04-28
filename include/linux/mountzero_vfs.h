/*
 * MountZero VFS - VFS Header
 * 
 * VFS-level hook declarations for path redirection and directory injection.
 */

#ifndef __LINUX_MOUNTZERO_VFS_H
#define __LINUX_MOUNTZERO_VFS_H

#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/namei.h>

#ifdef CONFIG_MOUNTZERO

/* VFS Hook Functions - actual implementations in mountzero_vfs.c */

/* Hook into getname() to redirect path lookups */
extern struct filename *mountzero_vfs_getname_hook(struct filename *name);

/* Get virtual path for an inode (reverse lookup for d_path spoofing) */
extern char *mountzero_vfs_get_virtual_path_for_inode(struct inode *inode);

/* Inject virtual directory entries during readdir */
extern int mountzero_vfs_inject_dents(struct file *file, void __user **dirent,
                                       int *count, loff_t *pos);

/* Spoof statfs results for redirected paths */
extern int mountzero_vfs_spoof_statfs(const char __user *pathname, struct kstatfs *buf);

/* Spoof SELinux context xattr for redirected files */
extern ssize_t mountzero_vfs_spoof_xattr(struct dentry *dentry, const char *name,
                                          void *value, size_t size);

/* Spoof mmap metadata (dev/ino) for redirected files */
extern void mountzero_vfs_spoof_mmap_metadata(struct inode *inode, dev_t *dev,
                                               unsigned long *ino);

#else /* !CONFIG_MOUNTZERO */

/* When MountZero is disabled, these hooks are useless.
 * We do NOT provide static inline stubs for the functions that have
 * real definitions in mountzero_vfs.c, because that file will not be
 * compiled. Any code that calls these should be guarded with
 * CONFIG_MOUNTZERO as well.
 *
 * However, to avoid compilation errors in common VFS files (like namei.c)
 * that may call these hooks unconditionally, we provide minimal stubs
 * that do nothing and are safe to call.
 */

static inline struct filename *mountzero_vfs_getname_hook(struct filename *name)
{
    return name;
}

static inline char *mountzero_vfs_get_virtual_path_for_inode(struct inode *inode)
{
    return NULL;
}

static inline int mountzero_vfs_inject_dents(struct file *file, void __user **dirent,
                                              int *count, loff_t *pos)
{
    return 0;
}

static inline int mountzero_vfs_spoof_statfs(const char __user *pathname, struct kstatfs *buf)
{
    return 0;
}

static inline ssize_t mountzero_vfs_spoof_xattr(struct dentry *dentry, const char *name,
                                                 void *value, size_t size)
{
    return -EOPNOTSUPP;
}

static inline void mountzero_vfs_spoof_mmap_metadata(struct inode *inode, dev_t *dev,
                                                      unsigned long *ino)
{
    /* do nothing */
}

#endif /* CONFIG_MOUNTZERO */

#endif /* __LINUX_MOUNTZERO_VFS_H */
