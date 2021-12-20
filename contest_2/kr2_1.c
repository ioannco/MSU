//
// Created by Ivan on 12/20/2021.
//

#include <unistd.h>
#include <fcntl.h>

int main (int argc, char ** argv)
{
    // числа, в которых сохраняем данные из файла
    int num_integer = 0;
    char num_char = 0;

    // дескриптор
    int fd = 0;

    // переменная для подсчета количества байт (size_t = unsigned long)
    size_t counter = 0;

    // суммарный размер инта и чара (может быть 5 а может и 9)
    long size = sizeof (int) + sizeof (char);

    // просто итератор
    size_t i = 0;

    // открываем файл на чтение и запись, при ошибке возвращаем 1
    if ((fd = open (argv[1], O_RDWR)) == -1)
        return 1;

    // считаем количество байт
    while (read (fd, &num_char, 1) > 0)
        counter++;

    // закрываем файл и выходим если размер не кратен суммарному размеру инта и чара (значит ошибка данных)
    if (counter % size != 0)
    {
        close (fd);
        return 1;
    }

    // возвращаем позицию курсора в начало, потому что мы его перевели в конец когда считали
    lseek (fd, 0, SEEK_SET);

    // считываем пока не дойдем до конца, проверяем при считывании очередной пары чара и инта
    while (read (fd, &num_char, sizeof (char)) > 0)
    {
        // считываем инт (должен считаться, так как мы проверяли на кратность)
        read (fd, &num_integer, sizeof (int));

        // переводим курсор на суммарный размер инта и чара назад, чтобы перезаписать
        lseek (fd, -size, SEEK_CUR);

        // перезаписываем в обратном порядке
        write (fd, &num_integer, sizeof (int));
        write (fd, &num_char,    sizeof (char));
    }

    // обязательно закрываем файл
    close (fd);

    // говорим что все ок
    return 0;
}