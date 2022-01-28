typedef int buffer_item;
#define BUFFER_SIZE 5

extern buffer_item buffer[BUFFER_SIZE];

int insert_item(buffer_item item);

int remove_item(buffer_item* item);

int getFirstInQueueItem();

void printBufferContents();

