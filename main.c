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
    int freed = 0;
    while (temp->next != NULL)
    {
        prev = temp;
        temp = temp->next;

        free(prev);
        freed++;
    }

    free(temp);
    freed++;

    printf("%d\n", freed);
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
    free_guests(&temp->guest_list);
    free(temp);
}

void n(FILE **hotel, Room **roomsHead)
{
    if (*roomsHead != NULL)
    {
        free_rooms(roomsHead);
        *roomsHead = NULL;
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

    while (fscanf(*hotel, "%d\n%d\n%lf\n", &tempRoomNum, &tempPCBed, &tempPrice) > 0)
    {
        Room *room = malloc(sizeof(Room));
        room->number = tempRoomNum;
        room->bed_count = tempPCBed;
        room->price = tempPrice;

        Guest *prevGuest = NULL;

        while (fscanf(*hotel, "#\n%[^\n]\n%[^\n]\n%d\n%d\n", tempName, tempAddress, &tempStartDate, &tempEndDate) > 0)
        {
            Guest *guest = malloc(sizeof(Guest));

            strcpy(guest->name, tempName);
            strcpy(guest->address, tempAddress);
            guest->reserve_begin = tempStartDate;
            guest->reserve_end = tempEndDate;

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
            *roomsHead = room;
            prevRoom = room;
        }
        else
        {
            prevRoom->next = room;
            prevRoom = room;
        }

        fscanf(*hotel, "---");

        roomsLoaded++;
    }

    printf("Nacitalo sa %d\n", roomsLoaded);
}

void v(Room *rooms_head)
{

    if(rooms_head == NULL)
        return;

   // Room *sorted_rooms = NULL;
   //TODO: SORT funckiot megcsinalni
    for (Room *room = rooms_head; room != NULL; room = room->next){
        if(room->next != NULL){
            if(room->next->number < room->number){
                Room *temp = room;
                room = room->next;
                temp->next = room;
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
}

int main()
{

    Room *rooms_head = NULL;
    FILE *hotel = NULL;

    n(&hotel, &rooms_head);
    v(rooms_head);
    free_rooms(&rooms_head);
    /* Guest *guest1 = malloc(sizeof(Guest));
    Guest *guest2 = malloc(sizeof(Guest));
    Guest *guest3 = malloc(sizeof(Guest));

    Guest *guest4 = malloc(sizeof(Guest));
    Guest *guest5 = malloc(sizeof(Guest));

    Room *room1 = malloc(sizeof(Room));
    Room *room2 = malloc(sizeof(Room));

    room1->guest_list = guest1;
    room2->guest_list = guest4;
    room1->next = room2;

    guest4->next = guest5;

    guest1->next = guest2;
    guest2->next = guest3;

    free_rooms(&room1);*/
    // free_guests(&guest1);
    //free_guests(&guest2);
}