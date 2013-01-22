static void my_init_hook(void);
static int my_read_hook(struct inode *inode, off_t offset, char **ptr, size_t *len, cbdata_t cbdata);
struct fs_hooks my_hooks;