#include "structs.h"
#include "test_util.h"
#include "CUnit/Basic.h"

static void test_init_exam(void) {
    uint32_t students[] = {100000, 100001, 100002};
    bool availabilities[] = {true, true, false, true, false};
    exam *exam = init_exam(1, 2, 555000, // Exam ID + Faculty ID + Teacher ID
                           students, 3,// Students + Enrollement (n of students)
                           classroom, // Room type
                           availabilities, 5, // Availabilities + max timeslots
                           2, NULL, 0); // Max exams + deps + deps size
    CU_ASSERT_EQUAL(exam->exam_id, 1);
    CU_ASSERT_EQUAL(exam->faculty, 2);
    CU_ASSERT_EQUAL(exam->teacher_id, 555000);
    CU_ASSERT_EQUAL(exam->enrollment, 3);
    CU_ASSERT_EQUAL(exam->students[0], 100000);
    CU_ASSERT_EQUAL(exam->students[1], 100001);
    CU_ASSERT_EQUAL(exam->students[2], 100002);
    CU_ASSERT_EQUAL(exam->room_type, classroom);
    CU_ASSERT_EQUAL(exam->room_id, NOT_ASSIGNED);
    CU_ASSERT_EQUAL(exam->timeslot, NOT_SCHEDULED);
    CU_ASSERT_EQUAL(exam->deps_size, 0);
    CU_ASSERT_EQUAL(exam->availabilities[0], true);
    CU_ASSERT_EQUAL(exam->availabilities[1], true);
    CU_ASSERT_EQUAL(exam->availabilities[2], false);
    CU_ASSERT_EQUAL(exam->availabilities[3], true);
    CU_ASSERT_EQUAL(exam->availabilities[4], false);
    CU_ASSERT_EQUAL(exam->conflicts[0], false);
    CU_ASSERT_EQUAL(exam->conflicts[1], false);
    free_exam(exam);
}

static void test_init_exam_with_deps(void) {
    uint32_t students[] = {100000, 100001, 100002};
    bool availabilities[] = {true, true, false, true, false};
    uint16_t deps[] = {2, 3};
    exam *exam = init_exam(1, 2, 555000, students, 3, classroom, availabilities,
                           5, 2, deps, 2);
    CU_ASSERT_EQUAL(exam->exam_id, 1);
    CU_ASSERT_EQUAL(exam->faculty, 2);
    CU_ASSERT_EQUAL(exam->teacher_id, 555000);
    CU_ASSERT_EQUAL(exam->enrollment, 3);
    CU_ASSERT_EQUAL(exam->students[0], 100000);
    CU_ASSERT_EQUAL(exam->students[1], 100001);
    CU_ASSERT_EQUAL(exam->students[2], 100002);
    CU_ASSERT_EQUAL(exam->room_type, classroom);
    CU_ASSERT_EQUAL(exam->room_id, NOT_ASSIGNED);
    CU_ASSERT_EQUAL(exam->timeslot, NOT_SCHEDULED);
    CU_ASSERT_EQUAL(exam->deps_size, 2);
    CU_ASSERT_EQUAL(exam->deps[0], 2);
    CU_ASSERT_EQUAL(exam->deps[1], 3);
    CU_ASSERT_EQUAL(exam->availabilities[0], true);
    CU_ASSERT_EQUAL(exam->availabilities[1], true);
    CU_ASSERT_EQUAL(exam->availabilities[2], false);
    CU_ASSERT_EQUAL(exam->availabilities[3], true);
    CU_ASSERT_EQUAL(exam->availabilities[4], false);
    CU_ASSERT_EQUAL(exam->conflicts[0], false);
    CU_ASSERT_EQUAL(exam->conflicts[1], false);
    free_exam(exam);
}

static void test_init_array_exams(void) {
    uint32_t students[] = {100000, 100001, 100002};
    bool availabilities[] = {true, true, false, true, false};
    exam *exam1 = init_exam(1, 2, 555000, students, 3, classroom,
                            availabilities, 5, 2, NULL, 0);
    exam *exam2 = init_exam(1, 2, 555000, students, 3, classroom,
                            availabilities, 5, 2, NULL, 0);
    exam *exams[] = {exam1, exam2};
    array_exams *array_exams = init_array_exams(2, exams);
    CU_ASSERT_EQUAL(2, array_exams->size);
    CU_ASSERT_PTR_EQUAL(exam1, array_exams->data[0]);
    CU_ASSERT_PTR_EQUAL(exam2, array_exams->data[1]);
    free_exams(array_exams);
}

static void test_init_room(void) {
    room *room = init_room(1, classroom, 1000,// Room ID + Room type + capacity
                           1, 2); // Faculty +  max_timeslot
    CU_ASSERT_EQUAL(room->room_id, 1);
    CU_ASSERT_EQUAL(room->type, classroom);
    CU_ASSERT_EQUAL(room->capacity, 1000);
    CU_ASSERT_EQUAL(room->faculty, 1);
    CU_ASSERT_EQUAL(room->assignation[0], NOT_ASSIGNED);
    CU_ASSERT_EQUAL(room->assignation[1], NOT_ASSIGNED)
    free_room(room);
}

static void test_init_array_rooms(void) {
    room *room1 = init_room(1, classroom, 1000, 0, 2);
    room *room2 = init_room(2, classroom, 1000, 0, 2);
    room *rooms[] = {room1, room2};
    array_rooms *array_rooms = init_array_rooms(2, rooms);
    CU_ASSERT_EQUAL(2, array_rooms->size);
    CU_ASSERT_PTR_EQUAL(room1, array_rooms->data[0]);
    CU_ASSERT_PTR_EQUAL(room2, array_rooms->data[1]);
    free_rooms(array_rooms);
}

static void test_get_rooms_sizes(void) {
    init_test_array_rooms();
    size_t **size = get_rooms_sizes(FACULTY_SIZE, rooms);
    CU_ASSERT_EQUAL(size[0][classroom], 0);
    CU_ASSERT_EQUAL(size[0][lab], 0);
    CU_ASSERT_EQUAL(size[0][computer_room], 1);
    CU_ASSERT_EQUAL(size[1][classroom], 3);
    CU_ASSERT_EQUAL(size[1][lab], 1);
    CU_ASSERT_EQUAL(size[1][computer_room], 0);
    clean_array_rooms();
}

static void test_get_rooms_matrix(void) {
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->size, rooms_limit);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[0][computer_room][0], room5);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[1][classroom][0], room1);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[1][classroom][1], room2);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[1][classroom][2], room3);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[1][lab][0], room4);
    clean_test_room();
}

int structs_test_suite(void) {
    CU_TestInfo tests[] = {
        {"init_exam()", test_init_exam},
        {"init_exam() with deps", test_init_exam_with_deps},
        {"init_array_exams()", test_init_array_exams},
        {"init_room()", test_init_room},
        {"init_array_rooms()", test_init_array_rooms},
        {"get_rooms_sizes()", test_get_rooms_sizes},
        {"get_rooms_matrix()", test_get_rooms_matrix},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"structs.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}