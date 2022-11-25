#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 24
#define START_ID 1000
struct student {
   char name[MAX];
   int id;
   int score;
};

int main(int argc, char *argv[])
{
    int fd, id;
    struct student record;
    struct flock lock;
 
    if (argc < 2) {
        fprintf(stderr,  "Usage : %s file\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(2);
    }
    printf("\nEnter ID (Search):");
    while (scanf("%d", &id) == 1) {
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (id-START_ID)*sizeof(record);
        lock.l_len = sizeof(record);
        if (fcntl(fd,F_SETLKW, &lock) == -1) {   /* 읽기 잠금 */
            perror(argv[1]);
            exit(3); 
        }

        lseek(fd, (id-START_ID)*sizeof(record), SEEK_SET);
        if ((read(fd, (char *) &record, sizeof(record)) > 0) && 
            (record.id != 0))
            printf("Name:%s\t ID:%d\t Score:%d\n", 
                   record.name, record.id, record.score); 
        else printf("Record %d Not Found\n", id);

        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK, &lock);    /* 잠금 해제 */
        printf("\nEnter ID (Search):");
    }

    close(fd);
    exit(0);
}

