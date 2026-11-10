#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define RECORD_SIZE 36 /**< Record size in file format (bytes). */
#define FIELD3_LEN 16 /**< field3 size in bytes. */

/**
 * @brief Structure representing a single dataset record.
 */
typedef struct {
    uint64_t id;                        /**< Unique identifier (unsigned 64-bit). */
    float    field1;                    /**< 32-bit floating-point value. */
    int64_t  field2;                    /**< Signed 64-bit integer. */
    char     field3[FIELD3_LEN];        /**< Up to 15 useful chars + '\0'. */
} Record;

/* -------------------------------------------------------
 *  Record I/O functions
 * ------------------------------------------------------- */

/**
 * @brief Reads a single record from the binary file.
 *
 * @param file   File opened in binary read mode.
 * @param record Pointer to the Record where the read data will be stored.
 * @return       1 on success, 0 on EOF, -1 on error.
 */
int read_record(FILE *file, Record *record);

/**
 * @brief Writes a single record to the binary file at the current position.
 *
 * @param file   File opened in binary write mode.
 * @param record Pointer to the Record to be written.
 * @return       1 on success, -1 on error.
 */
int write_record(FILE *file, const Record *record);

/**
 * @brief Loads all records from the file into a dynamically allocated array.
 *
 * The function reads the file from the beginning (rewinds the stream),
 * allocates the required memory, and returns it via @p records.
 *
 * @param file    File opened in binary read mode.
 * @param records Pointer to a Record*; will be set to the allocated array.
 *                Must be non-NULL.
 * @param count   Pointer to size_t where the number of records read is stored.
 *                Must be non-NULL.
 * @return        0 on success, -1 on error
 */
int load_records_from_file(FILE *file, Record **records, size_t *count);

/**
 * @brief Saves an array of records to the binary file.
 *
 * @param file    File opened in binary write mode.
 * @param records Array of Record to save.
 * @param count   Number of records in the array.
 * @return        0 on success, -1 on error.
 */
int save_records_to_file(FILE *file, const Record *records, size_t count);

/**
 * @brief Prints a record.
 *
 * @param record Pointer to the Record to print.
 */
void print_record(const Record *record);

/* -------------------------------------------------------
 *  Comparison functions  
 * ------------------------------------------------------- */

/**
 * @brief Compares two Records by id.
 *
 * @param a_ptr Pointer to the first Record.
 * @param b_ptr Pointer to the second Record.
 * @return  Negative if a < b, 0 if equal, positive otherwise.
 */
int compar_by_id(const void *a_ptr, const void *b_ptr);

/**
 * @brief Compares two Records by field1 (float).
 *
 * @param a_ptr Pointer to the first Record.
 * @param b_ptr Pointer to the second Record.
 * @return  Negative if a < b, 0 if equal, positive otherwise.
 */
int compar_by_field1(const void *a_ptr, const void *b_ptr);

/**
 * @brief Compares two Records by field2 (int64_t).
 *
 * @param a_ptr Pointer to the first Record.
 * @param b_ptr Pointer to the second Record.
 * @return  Negative if a < b, 0 if equal, positive otherwise.
 */
int compar_by_field2(const void *a_ptr, const void *b_ptr);

/**
 * @brief Compares two Records by field3 (string).
 *
 * @param a_ptr Pointer to the first Record.
 * @param b_ptr Pointer to the second Record.
 * @return  Negative if a < b, 0 if equal, positive otherwise.
 */
int compar_by_field3(const void *a_ptr, const void *b_ptr);

#endif

