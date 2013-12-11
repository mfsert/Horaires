/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 29/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "util.h"
#include "structs.h"
#include "heuristics.h"
#include "conflict.h"

// This macro let you choose an example between [1,2]
#define EXAMPLE 1

// Following the macro above, defines some parameters
// concerning the chosen example
#if EXAMPLE == 1
#define MAX_TIMESLOT 5
#define MAX_EXAM 8
#elif EXAMPLE == 2
#define MAX_TIMESLOT 2
#define MAX_EXAM 2
#endif

/**
 * Creates and initializes an problem instance, having a solution.
 *
 * @return An array of exams
 */
exam *get_example1() {
    // exam1 - Analyse
    exam *exam1 = init_exam(1, 555000,                      // exam id, teacher id
                            3, 10000, 10001, 10002,         // nb + enrollments
                            MAX_TIMESLOT, 1, 1, 1, 0, 0, // nb + availabilities
                            MAX_EXAM, classroom);        // nb of exams + room type

    // exam2 - Fonctionnement des ordis
    exam *exam2 = init_exam(2, 555001,
                            2, 10000, 10003,
                            MAX_TIMESLOT, 0, 1, 1, 0, 0,
                            MAX_EXAM, classroom);

    // exam3 - MATH1
    exam *exam3 = init_exam(3, 555002,
                            2, 10002, 10004,
                            MAX_TIMESLOT, 0, 1, 1, 1, 0,
                            MAX_EXAM, classroom);

    // exam4 - Anglais
    exam *exam4 = init_exam(4, 555003,
                            4, 10003, 10004, 10005, 10006,
                            MAX_TIMESLOT, 0, 0, 1, 0, 0,
                            MAX_EXAM, classroom);

    // exam5 - Anglais
    exam *exam5 = init_exam(5, 555001,
                            3, 10000, 10001, 10003,
                            MAX_TIMESLOT, 0, 0, 1, 1, 0,
                            MAX_EXAM, lab);

    // exam6 - chimir
    exam *exam6 = init_exam(6, 555004,
                            1, 10004,
                            MAX_TIMESLOT, 0, 0, 0, 1, 1,
                            MAX_EXAM, lab);

    // exam7 - algèbre
    exam *exam7 = init_exam(7, 555005,
                            1, 10001,
                            MAX_TIMESLOT, 0, 0, 0, 0, 1,
                            MAX_EXAM, classroom);
    // exam8 - jesaispaslire
    exam *exam8 = init_exam(8, 555006,
                            1, 10002,
                            MAX_TIMESLOT, 1, 1, 0, 0, 1,
                            MAX_EXAM, classroom);

    return init_exams(MAX_EXAM, exam1, exam2, exam3, exam4, exam5, exam6, exam7,
                      exam8);
}

/**
 * Creates and initializes an problem instance, having a solution.
 *
 * @return An array of exams
 */
exam *get_example2() {
    // exam1 - Analyse
    exam *exam1 = init_exam(1, 555000,               // exam id, teacher id
                            3, 10000, 10001, 10002,  // nb + enrollments
                            MAX_TIMESLOT, 1, 0,      // nb + availabilities
                            MAX_EXAM, classroom); // nb of exams

    // exam2 - Fonctionnement des ordis
    exam *exam2 = init_exam(2, 555001,
                            2, 10000, 10003,
                            MAX_TIMESLOT, 1, 1,
                            MAX_EXAM, classroom);

    return init_exams(MAX_EXAM, exam1, exam2);
}

/**
 * Calls the right example following the macro EXAMPLE.
 *
 * @return An array of exams
 */
exam *get_example() {
    switch (EXAMPLE) {
        case 1:
            return get_example1();

        case 2:
            return get_example2();
    }

    return NULL;
}

/**
 * Create an array of rooms to use with the two first example.
 *
 * @return An array of rooms
 */
room *get_rooms() {
    // R1 - Salon bleu
    room *room1 = init_room(1, classroom, 1, 0, MAX_TIMESLOT);
    // R2 - Plisnier
    room *room2 = init_room(2, classroom, 2, 0, MAX_TIMESLOT);
    // R3 - Van Gogh
    room *room3 = init_room(3, classroom, 5, 0, MAX_TIMESLOT);
    // R4 - Pascal
    room *room4 = init_room(4, lab, 4, 0, MAX_TIMESLOT);

    return init_rooms(4, room3, room1, room2, room4);
}

uint16_t **get_room_indices(uint16_t room_size, uint8_t faculty_size, room *rooms) {

    uint16_t **room_indices = calloc(faculty_size, sizeof(uint16_t *));

    for(uint8_t f = 0; f < faculty_size; f++)
        room_indices[f] = calloc(MAX_ROOM_TYPE, sizeof(uint16_t));

    for(uint8_t f = 0; f < room_size; f++)
        room_indices[rooms[f].faculty][rooms[f].type]++;

    return room_indices;
}

room ***get_rooms_matrix(uint16_t room_size, uint8_t faculty_size, room *rooms, uint16_t **room_indices) {

    uint16_t **cpt = calloc(faculty_size, sizeof(uint16_t *));
    room ***rooms_matrix = calloc(faculty_size, sizeof(room**));

    for (uint8_t i = 0; i < faculty_size; i++) {
        rooms_matrix[i] = calloc(MAX_ROOM_TYPE, sizeof(room*));
        cpt[i] = calloc(MAX_ROOM_TYPE, sizeof(uint16_t));
        for (uint8_t j = 0; j < MAX_ROOM_TYPE; j++)
            rooms_matrix[i][j] = calloc(room_indices[i][j], sizeof(room));
    }

    for (uint8_t i = 0; i < room_size; i++) {
        uint16_t index = cpt[rooms[i].faculty][rooms[i].type]++;
        rooms_matrix[rooms[i].faculty][rooms[i].type][index] = rooms[i];
    }

    return rooms_matrix;
}

/**
 * Prints which timeslot have been attributed to each exams.
 *
 * @param exams An array of scheduled exams
 */
void print_summary_schedule(exam *exams) {
    printf("Summary\n");
    printf("=======\n");

    for (int i = 0; i < MAX_EXAM; i++) {
        printf("Exam %d : %d\n", i + 1, exams[i].timeslot);
    }

    printf("\n");
}

/**
 * Prints which detailed information from each exams,
 * for each timeslot available.
 *
 * @param exams An array of scheduled exams
 */
void print_detailed_schedule(exam *exams) {
    printf("Detailed schedule\n");
    printf("=================\n");

    for (int i = 0; i < MAX_TIMESLOT; i++) {
        printf("Timeslot %d\n", i + 1);
        printf("------------\n\n");

        for (int j = 0; j < MAX_EXAM; j++) {
            if (exams[j].timeslot == i) {
                printf("  Exam %d :\n", j + 1);
                printf("      -> Prof : %d\n", exams[j].teacher_id);

                printf("      -> Timeslots available : (");

                for (int k = 0; k < MAX_TIMESLOT; k++) {
                    printf("%d ", exams[j].availabilities[k]);
                }

                printf(")\n");

                printf("      -> Conflicts detected : (");

                for (int k = 0; k < MAX_EXAM; k++) {
                    printf("%d ", exams[j].conflicts[k]);
                }

                printf(")\n");

                printf("      -> Students :\n");

                for (int k = 0; k < exams[j].enrollment; k++) {
                    printf("            %d\n", exams[j].students[k]);
                }

                printf("      -> Room : %u\n", exams[j].room_id);
            }
        }
    }
}

/**
 * Main function, execute the heuristics on a simple example.
 *
 */
int main() {
    uint8_t faculty_size = 1;
    // Collect a sample of exams
    exam *exams = get_example();

    room *rooms = get_rooms();
    uint16_t **indices = get_room_indices(4, faculty_size, rooms);
    room ***rooms_matrix = get_rooms_matrix(4, faculty_size, rooms, indices);

    // Preprocessing to the coloring graph heuristics
    compute_conflicts(exams, MAX_EXAM);

    // Main heuristic
    bool a = color_graph_backtrack(exams, MAX_EXAM, rooms_matrix, indices, faculty_size, MAX_TIMESLOT);

    printf("%s\n", (a == true) ? "A schedule has been found!\n" :
           "No schedule has been found!\n");

    // Some outputs
    if (a == true) {
        print_summary_schedule(exams);

        print_detailed_schedule(exams);
    }

    return 0;
}
