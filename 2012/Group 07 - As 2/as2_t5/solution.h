typedef struct task_struct {
	unsigned long id;
	long state;
	void *function;
} task_t;

extern void *task_alloc(void);
extern void task_free(void *ptr);
