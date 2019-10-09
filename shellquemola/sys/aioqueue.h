#define MAX 10

typedef struct {
	struct aiocb cb;
	int fd;
	int valid;
} aio_pack;

aio_pack aio_array[MAX];

void aio_add(struct aiocb a, int fd);
void aio_check();
