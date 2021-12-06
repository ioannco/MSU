/*
 * FIFO очередь сообщений
 *
 * <--- [ [тип1 | тело]  [тип2 | тело ]  [тип1 | тело] .. ] --->
 */

struct msgbuf
{
    long mtype;     // тип
    char data[256]; // данные сообщения
};

#include <sys/ipc.h>
#include <unistd.h>

    /* cоздание/подключение к очереди
     *
     * int msgget (key_t key, int msg_flg) -> msgid
     *
     * msg_flg IPC_CREAT | 0666 - создать если не существует со всеми правами
     *         IPC_PRIVATE      - произвольный процесс не может подключиться
     *         IPC_EXECL        - если существует, выдает ошибка
     */

    /* управление очередью
     *
     * int msgctl (int msgid, int cmd, struct msgid_ds * buf)
     *
     * cmd     IPC_SET          - загрузить из структуры buf параметры очереди
     *         IPC_STAT         - скопировать структуру buf параметры очереди
     *         IPC_RMID         - удаление очереди
     */

    /*
     * отправка сообщения
     *
     * int msgsnd (int msgid, void * buf, size_t msg_size, int msg_flg)
     *
     * msg_flg IPC_NOWAIT       - если не смог отправить, вернуть -1
     */

    /*
     * получение сообщения
     *
     * int msgrcv (int msgid, void * buf, size_t max_size, long type, int msg_flg)
     *
     * msg_flg MSG_NOERROR      - получить сообщение даже если размер превышает лимит
     *
     * type  > 0 - ждем и получаем сообщ типа type
     * type == 0 - получить сообщение любого типа
     * type  < 0 - получить сообщение типа меньше |type|
     */
#define CNT_MAX 50

struct msg_buf
{
    long type;
    int cnt;
};

int pingpong_1 ()
{
    key_t key = ftok ("file", 'm');
    int msgid = msgget (key, IPC_CREAT | 0666);
    struct msg_buf mes;
    msgid = msgget (key, IPC_CREAT | 0666);

    mes.type = 1;
    mes.cnt = 0;

    msgsnd (msgid, &mes, sizeof (int), 0);

    while (mes.cnt < CNT_MAX)
    {
        msgrcv (msgid, &mes, sizeof (int), 2, 0);

        mes.cnt++;
        mes.type = 1;

        msgsnd (msgid, &mes, sizeof (int), 0);
    }

    msgrcv (msgid, &mes, sizeof (int), 2, 0);
    msg.type = 1;
    mes.cnt = -1;
    msgsnd (msgid, &mes, sizeof (int), 0);

    return 0;
}

int pingpong_2() {}

