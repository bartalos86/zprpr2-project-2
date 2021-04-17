#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 50

typedef struct guest
{
    char name[51];
    char address[101];
    int reserve_begin;
    int reserve_end;
    struct guest *next;
} Guest;

typedef struct room
{
    int number;
    int bed_count;
    double price;
    Guest *guest_list;
    struct room *next;
} Room;

//Helper functions

void free_guests(Guest **guestsHead)
{
    Guest *temp = *guestsHead, *prev;
    while (temp->next != NULL)
    {
        prev = temp;
        temp = temp->next;

        free(prev);
    }

    free(temp);

    *guestsHead = NULL;
}

void free_rooms(Room **roomsHead)
{
    Room *temp = *roomsHead, *prev;

    while (temp->next != NULL)
    {
        prev = temp;
        temp = temp->next;
        free_guests(&(prev->guest_list));
        free(prev);
    }
    free_guests(&(temp->guest_list));

    *roomsHead = NULL;
    free(temp);
}

void write_to_file(FILE **hotel, Room *rooms_head, int overwrite)
{

    if (*hotel == NULL)
    {
        return;
    }

    //TODO: ez nem jol mukodik - Open again for overwriting
    //We must erase the file when overwriting, only erase if it was opened before
    if (overwrite && *hotel != NULL)
    {
        fclose(*hotel);

        if ((*hotel = fopen("hotel.txt", "w+")) == NULL)
        {
            return;
        }

        /* rewind(*hotel);
        int lineCount = 0;
        char lineBuffer[101];
        while(fgets(lineBuffer,100,*hotel) != NULL){
            lineCount++;
        }
        rewind(*hotel);

        printf("%d", lineCount);
        char buff[] = {-1};

        for (int i = 0; i < lineCount*100; i++)
        {
            // fputs("", *hotel);
            fwrite(buff, 1, 1, *hotel);

        }*/
        //fwrite("", 100, lineCount, *hotel);
    }

    rewind(*hotel);

    for (Room *room = rooms_head; room != NULL; room = room->next)
    {
        fprintf(*hotel, "---\n%d\n%d\n%g\n", room->number, room->bed_count, room->price);

        for (Guest *guest = room->guest_list; guest != NULL; guest = guest->next)
        {
            fprintf(*hotel, "#\n%s\n%s\n%d\n%d\n", guest->name, guest->address, guest->reserve_begin, guest->reserve_end);
        }
    }

    fflush(*hotel);
}

void n(FILE **hotel, Room **rooms_head)
{

    if (*rooms_head != NULL)
    {
        free_rooms(rooms_head);
        *rooms_head = NULL;
    }

    if (*hotel == NULL)
    {
        *hotel = fopen("hotel.txt", "r+");

        if (*hotel == NULL)
        {
            printf("Zaznamy neboli nacitane\n");
        }
    }

    int tempRoomNum, tempPCBed, tempStartDate, tempEndDate;
    double tempPrice;
    char tempName[51], tempAddress[101];

    int roomsLoaded = 0;
    Room *prevRoom = NULL;

    rewind(*hotel);

    while (fscanf(*hotel, "---\n%d\n%d\n%lf\n", &tempRoomNum, &tempPCBed, &tempPrice) > 0)
    {
        Room *room = malloc(sizeof(Room));
        room->number = tempRoomNum;
        room->bed_count = tempPCBed;
        room->price = tempPrice;
        room->next = NULL;

        Guest *prevGuest = NULL;

        while (fscanf(*hotel, "#\n%[^\n]\n%[^\n]\n%d\n%d\n", tempName, tempAddress, &tempStartDate, &tempEndDate) > 0)
        {
            Guest *guest = malloc(sizeof(Guest));

            strcpy(guest->name, tempName);
            strcpy(guest->address, tempAddress);
            guest->reserve_begin = tempStartDate;
            guest->reserve_end = tempEndDate;
            guest->next = NULL;

            if (prevGuest == NULL)
            {
                room->guest_list = guest;
                prevGuest = guest;
            }
            else
            {
                prevGuest->next = guest;
                prevGuest = guest;
            }
        }

        if (prevRoom == NULL)
        {
            *rooms_head = room;
            prevRoom = room;
        }
        else
        {
            prevRoom->next = room;
            prevRoom = room;
        }

        // fscanf(*hotel, "---");

        roomsLoaded++;
    }

    printf("Nacitalo sa %d zaznamov\n", roomsLoaded);
}

void v(Room *rooms_head)
{

    if (rooms_head == NULL)
        return;


    //TODO: Bubble sort
    for (Room *room = rooms_head; room != NULL; room = room->next)
    {
        for (Room *inroom = room; inroom != NULL; inroom = inroom->next)
        {
            if (inroom->next != NULL)
            {
                Room *next = inroom->next;
                if (inroom->number > next->number)
                {
                    Room *temp = malloc(sizeof(Room));
                    memcpy(temp, inroom, sizeof(Room));
                    inroom->number = next->number;

                    inroom->bed_count = next->bed_count;
                    inroom->price = next->price;
                    inroom->guest_list = next->guest_list;

                    next->number = temp->number;
                    next->bed_count = temp->bed_count;
                    next->price = temp->price;
                    next->guest_list = temp->guest_list;
                    free(temp);
                }
            }
        }
    }

    for (Room *room = rooms_head; room != NULL; room = room->next)
    {
        printf("Izba cislo: %d\nPocet lozok: %d\nCena: %g\nZoznam hosti:\n", room->number, room->bed_count, room->price);

        for (Guest *guest = room->guest_list; guest != NULL; guest = guest->next)
        {
            printf("Meno: %s\nAdresa: %s\nZaciatok rezervacie: %d\nKoniec rezervacie: %d\n", guest->name, guest->address, guest->reserve_begin, guest->reserve_end);

            if (guest->next != NULL)
                printf("############################\n");
        }

        if (room->next != NULL)
            printf("----------------------------\n----------------------------\n");
    }

    //free_rooms(&sorted_rooms);
}

void r(FILE **hotel, Room **rooms_head)
{
    if(*hotel == NULL){
        return;
    }

    int number, bedCount, guestCount;
    double price;

    scanf("%d\n%d\n%lf\n%d\n", &number, &bedCount, &price, &guestCount);
    Room *lastRoom, *current = *rooms_head;
    while (current != NULL)
    {
        lastRoom = current;
        current = current->next;
    }

    Room *newRoom = malloc(sizeof(Room));
    newRoom->number = number;
    newRoom->bed_count = bedCount;
    newRoom->price = price;
    newRoom->next = NULL;

    if(lastRoom != NULL)
    lastRoom->next = newRoom;

    char nameBuffer[51], addressBuffer[101];
    int startDate, endDate;

    Guest *guest_head = NULL;
    Guest *prevGuest = NULL;

    //printf("Guest count: %d\n", guestCount);

    for (int i = 0; i < guestCount; i++)
    {
        Guest *currentGuest = malloc(sizeof(Guest));
        // pirntf("one\n");
        if (i == guestCount - 1)
            fscanf(stdin, "%[^\n]\n%[^\n]\n%d\n%d", nameBuffer, addressBuffer, &startDate, &endDate);
        else
            fscanf(stdin, "%[^\n]\n%[^\n]\n%d\n%d\n", nameBuffer, addressBuffer, &startDate, &endDate);

        strcpy(currentGuest->name, nameBuffer);
        strcpy(currentGuest->address, addressBuffer);
        currentGuest->reserve_begin = startDate;
        currentGuest->reserve_end = endDate;

        if (guest_head == NULL)
            guest_head = currentGuest;

        if (prevGuest != NULL)
        {
            prevGuest->next = currentGuest;
        }

        prevGuest = currentGuest;
    }

    prevGuest->next = NULL;

    newRoom->guest_list = guest_head;

    if(*rooms_head == NULL){
        *rooms_head = newRoom;
    }

        write_to_file(hotel, *rooms_head, 0);
    
}

void z(FILE **hotel, Room **room_head)
{

    if (*room_head == NULL || *hotel == NULL)
    {
        printf("Situacia bez presného zadania\n");
        return;
    }

    int roomNum;
    scanf("%d", &roomNum);

    Room *prev = NULL;
    int roomFound = 0;
    for (Room *current = *room_head; current != NULL; current = current->next)
    {
        if (current->number == roomNum)
        {
            if (current == *room_head)
            {
                Room *head = *room_head;
                *room_head = current->next;

                head->next = NULL;
                roomFound = 1;
                free_rooms(&head);
            }
            else
            {
                if (prev != NULL)
                    prev->next = current->next;

                current->next = NULL;
                roomFound = 1;
                free_rooms(&current);
            }

            if (roomFound)
            {
                printf("Rezervacia izby cislo %d bola zrusena.\n", roomNum);
                break;
            }
        }

        prev = current;
    }
    write_to_file(hotel, *room_head, 1);
}

void h(Room *rooms_head)
{
    if (rooms_head == NULL)
    {
        printf("Situacia bez presného zadania\n");
        return;
    }

    int date;
    scanf("%d", &date);

    typedef struct number
    {
        int number;
        struct number *next;
    } Number;

    Number *foundList = NULL;

    for (Room *room = rooms_head; room != NULL; room = room->next)
    {
        int isFound = 0;
        //fprintf(hotel,"---\n%d\n%d\n%g\n", room->number, room->bed_count, room->price);

        for (Guest *guest = room->guest_list; guest != NULL; guest = guest->next)
        {
            if (guest->reserve_begin <= date && guest->reserve_end >= date)
            {
                isFound = 1;
            }
            //fprintf(hotel,"#\n%s\n%s\n%d\n%d\n", guest->name, guest->address, guest->reserve_begin, guest->reserve_end);
        }

        if (isFound)
        {

            int roomNum = room->number;

            if (foundList == NULL)
            {
                foundList = malloc(sizeof(Number));
                foundList->number = roomNum;
                foundList->next = NULL;
            }
            else
            {
                Number *current = foundList;
                int isAdded = 0;
                while (current->next != NULL)
                {

                    printf("c: %d n: %d\n", current->number, current->next->number);
                    if (roomNum > current->number)
                    {

                        Number *newNum = malloc(sizeof(Number));
                        newNum->number = roomNum;
                        if (current == foundList)
                        {
                            newNum->next = current;
                            foundList = newNum;
                        }
                        else
                        {
                            newNum->next = current->next;
                            current->next = newNum;
                        }

                        isAdded = 1;
                        break;
                    }

                    current = current->next;
                }

                //Add to end
                if (!isAdded)
                {
                    Number *newNum = malloc(sizeof(Number));
                    newNum->next = NULL;
                    newNum->number = roomNum;
                    current->next = newNum;
                }
            }

            // prevFount = room->number;
        }
    }

    //Print and free memory
    Number *prev = NULL;
    for (Number *number = foundList; number != NULL; number = number->next)
    {
        if (prev != NULL)
            free(prev);

        printf("%d\n", number->number);
        prev = number;
    }

    if (prev != NULL)
        free(prev);

    if (foundList == NULL)
    {
        printf("K datumu %d neevidujeme rezervaciu.\n", date);
    }
}

void a(FILE **hotel, Room *rooms_head)
{
    if (*hotel == NULL || rooms_head == NULL)
    {
        printf("Situacia bez presného zadania\n");
        return;
    }

    int roomNumber, newBedCount;
    scanf("%d\n%d", &roomNumber, &newBedCount);
    for (Room *room = rooms_head; room != NULL; room = room->next)
    {
        if (room->number == roomNumber)
        {
            room->bed_count = newBedCount;
            printf("Izba cislo %d ma lozkovu kapacitu %d.\n", roomNumber, newBedCount);
        }
    }

    write_to_file(hotel, rooms_head, 1);
}

int main()
{

    Room *rooms_head = NULL;
    FILE *hotel = NULL;

    char input;
    int exit = 0;

    while (!exit)
    {
        fflush(stdin);
        scanf("%c", &input);
        fflush(stdin);
        switch (input)
        {
        case 'n':
            n(&hotel, &rooms_head);
            break;
        case 'v':
            v(rooms_head);
            break;
        case 'r':
            r(&hotel, &rooms_head);
            break;
        case 'z':
            z(&hotel, &rooms_head);
            break;
        case 'h':
            h(rooms_head);
            break;
        case 'a':
            a(&hotel, rooms_head);
            break;
        case 'k':
            exit = 1;
            break;
        }
    }

    if (rooms_head != NULL)
        free_rooms(&rooms_head);

    if (hotel != NULL)
        fclose(hotel);
}