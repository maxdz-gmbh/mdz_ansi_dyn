/**
 * \ingroup mdz_ansi_dyn library
 *
 * \author maxdz Software GmbH
 *
 * \par license
 * This file is subject to the terms and conditions defined in file 'LICENSE.txt', which is part of this source code package.
 *
 * \par description
 * mdz_ansi_dyn is dynamically-sized contiguous single-byte string, containing ASCII (0..127) and "ANSI" (128 - 255) characters. 
 *
 * String may contain 0-terminators ('\0') inside, and must end with 0-terminator.
 *
 * Capacity - how many bytes of memory is reserved for string content.
 * Size - how many characters are actually residing in a string.
 *
 * "reserve" functions allocate/reallocate memory dynamically using malloc()/realloc().
 * "attach" functionality allows attaching contiguous block of memory to string, for using string functions on it.
 *
 * \par portability
 * Source code of library conforms to ANSI C 89/90 Standard.
 *
 * \version 0.1
 *
 * \date 2024-09
 *
 */

#ifndef MDZ_ANSI_16_H
#define MDZ_ANSI_16_H

#include <stddef.h>

#include "mdz_bool.h"
#include "mdz_attach_type.h"
#include "mdz_ansi_compare_result.h"
#include "mdz_ansi_replace_type.h"
#include "mdz_error.h"

typedef struct mdz_AnsiDyn mdz_AnsiDyn;

typedef void* (*mdz_AllocFunc) (size_t);
typedef void* (*mdz_ReallocFunc) (void*, size_t);
typedef void  (*mdz_FreeFunc) (void*);

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * \defgroup Init/destroy functions
   */

  /**
   * Initialize mdz_ansi_dyn library and license. This function should be called before any other function of the library.
   * \param pnFirstNameHash - user first name hash code
   * \param pnLastNameHash  - user last name hash code
   * \param pnEmailHash     - user e-mail hash code
   * \param pnLicenseHash   - license hash code
   * \return:
   * mdz_true - if the initialization succeeded, otherwise mdz_false
   */
  mdz_bool mdz_ansi_dyn_init(const unsigned long* pnFirstNameHash, const unsigned long* pnLastNameHash, const unsigned long* pnEmailHash, const unsigned long* pnLicenseHash);

  /**
   * Set allocation functions for dynamic memory-management. If functions are not set - library is only using static memory, thus no dynamic memory allocations.
   * \param pAllocFunc   - pointer to allocate memory function. Used in string-creation functions like mdz_ansi_dyn_create(). Can be NULL
   * \param pReallocFunc - pointer to re-allocate memory function. Used in string-resizing fuinctons like mdz_ansi_dyn_insert(). Can be NULL
   * \param pFreeFunc    - pointer to free memory function. Used in string-destroying functions like mdz_ansi_dyn_destroy(). Can be NULL
   */

  void mdz_ansi_dyn_setAllocFunctions(mdz_AllocFunc pAllocFunc, mdz_ReallocFunc pReallocFunc, mdz_FreeFunc pFreeFunc);

  /**
   * Create empty string with Capacity nCapacity and Size 0. This function uses dynamic-memory allocator malloc() function set in mdz_ansi_dyn_setAllocFunctions()
   * \param nCapacity - initial capacity of string data in bytes
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_CAPACITY   - nCapacity > maximal-possible capacity (SIZE_MAX minus size of string Metadata, returned by mdz_ansi_dyn_sizeMetadata() and 0-terminator byte)
   * MDZ_ERROR_ALLOC_FUNC - dynamic-memory allocator malloc() function is not set (using mdz_ansi_dyn_setAllocFunctions())
   * MDZ_ERROR_ALLOCATION - could not alocate memory
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * NULL   - function failed
   * Result - pointer to string for use in other mdz_ansi_dyn functions
   */

  mdz_AnsiDyn* mdz_ansi_dyn_create(size_t nCapacity, enum mdz_error* penError);

  /**
   * Attach string to pre-allocated pcBuffer of nBufferSize bytes. If penError is not NULL, error will be written there
   * \param pcBuffer     - pointer to pre-allocated buffer to attach. Thus minimal pcBuffer size is size of string Metadata, returned by mdz_ansi_dyn_sizeMetadata() + 0-terminator byte (in this case Capacity is 0)
   * \param nBufferSize  - size of pcBuffer in bytes; should be at least size of Metadata (returned by mdz_ansi_dyn_sizeMetadata()) + 0-terminator byte, in this case Capacity is 0
   * \param penError     - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE     - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA        - pcBuffer is NULL
   * MDZ_ERROR_CAPACITY    - nBufferSize < size of string Metadata (returned by mdz_ansi_dyn_sizeMetadata()) + 0-terminator byte
   * MDZ_ERROR_NONE        - function succeeded
   * \return:
   * NULL   - function failed
   * Result - pointer to string for use in other mdz_ansi_dyn functions
   */
  mdz_AnsiDyn* mdz_ansi_dyn_attach(char* pcBuffer, size_t nBufferSize, enum mdz_error* penError);

  /**
   * Destroy string. If string is created using mdz_ansi_dyn_create() - free() function set in mdz_ansi_dyn_setAllocFunctions() will be used for destroying.
   * \param psAnsi - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \return:
   * MDZ_ERROR_FREE_FUNC     - could not destroy string created with mdz_ansi_dyn_create(), because free() function is not set (using mdz_ansi_dyn_setAllocFunctions())
   * MDZ_ERROR_NONE          - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_destroy(mdz_AnsiDyn* psAnsi);

  /**
   * \defgroup Status functions
   */

  /**
   * Return size of string Metadata
   * \return:
   * Size - of string Metadata
   */
  size_t mdz_ansi_dyn_sizeMetadata(void);

  /**
   * Return Size of string Data in characters/bytes.
   * \param psAnsi - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \return:
   * 0    - if psAnsi is NULL
   * Size - otherwise
   */
  size_t mdz_ansi_dyn_size(const mdz_AnsiDyn* psAnsi);

  /**
   * Return Capacity of string Data in characters/bytes.
   * \param psAnsi - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \return:
   * 0    - if psAnsi is NULL
   * Size - otherwise
   */
  size_t mdz_ansi_dyn_capacity(const mdz_AnsiDyn* psAnsi);

  /**
   * Return pointer to string Data.
   * \param psAnsi - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \return:
   * 0    - if psAnsi is NULL
   * Data - otherwise
   */
  char* mdz_ansi_dyn_data(mdz_AnsiDyn* psAnsi);

  /**
   * Return const pointer to string Data.
   * \param psAnsi - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \return:
   * 0    - if psAnsi is NULL
   * Data - otherwise
   */
  const char* mdz_ansi_dyn_dataConst(const mdz_AnsiDyn* psAnsi);

  /**
   * \defgroup Insert/remove functions
   */

  /**
   * Insert pcItems from nLeftPos position. Data and pcItems cannot overlap. New Size is written in psAnsi. If there is not enouhgh Capacity, realloc() function set in mdz_ansi_dyn_setAllocFunctions() will be used
   * \param ppsAnsi  - pointer to address of string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach(). If after reallocation string address will be changed, new address will be stored here
   * \param nLeftPos - 0-based position to insert. If nLeftPos == Size items are appended. nLeftPos > Size is not allowed
   * \param pcItems  - items to insert. Cannot be NULL
   * \param nCount   - number of items to insert. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE      - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA         - psAnsi is NULL
   * MDZ_ERROR_CAPACITY     - Capacity is 0 or too large
   * MDZ_ERROR_TERMINATOR   - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS        - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT   - nCount is 0
   * MDZ_ERROR_BIGLEFT      - nLeftPos > Size
   * MDZ_ERROR_BIGCOUNT     - Size + nCount > Capacity
   * MDZ_ERROR_OVERLAP      - [Data; Data + Size + nCount] area and pcItems overlap
   * MDZ_ERROR_ATTACHED     - string has not enough capacity for insertion, but is attached (using mdz_ansi_dyn_attach()), thus cannot be reallocated
   * MDZ_ERROR_REALLOC_FUNC - could not realloc string created with mdz_ansi_dyn_create(), because realloc() function is not set (using mdz_ansi_dyn_setAllocFunctions())
   * MDZ_ERROR_ALLOCATION   - string reallocation failed
   * MDZ_ERROR_NONE         - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_insert(mdz_AnsiDyn** ppsAnsi, size_t nLeftPos, const char* pcItems, size_t nCount);

  /**
   * \defgroup Find functions
   */

  /**
   * Find first occurrence of cItem. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \param cItem     - character to find
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if cItem not found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_dyn_findSingle(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, char cItem, enum mdz_error* penError);

  /**
   * Find first occurrence of pcItems using optimized Boyer-Moore-Horspool search. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_BIGCOUNT   - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if pcItems not found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_dyn_find(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find last occurrence of cItem. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search from the end of Data
   * \param cItem     - character to find
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if cItem not found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_dyn_rfindSingle(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, char cItem, enum mdz_error* penError);

  /**
   * Find last occurrence of pcItems using optimized Boyer-Moore-Horspool search. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search from the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_BIGCOUNT   - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if pcItems not found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_dyn_rfind(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find first occurrence of any item of pcItems. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if no item of pcItems found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_dyn_firstOf(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find first non-occurrence of any item of pcItems. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if no item of pcItems found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_dyn_firstNotOf(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find last occurrence of any item of pcItems. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if no item of pcItems found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_dyn_lastOf(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find last non-occurrence of any item of pcItems. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if no item of pcItems found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_dyn_lastNotOf(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * \defgroup Insert/remove functions
   */

  /**
   * Remove nCount item(s) starting from 0-based nLeftPos position
   * \param psAnsi   - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos - 0-based start position to remove item(s) from. Use 0 to remove from the beginning of Data
   * \param nCount   - number of item(s) to remove. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGLEFT    - nLeftPos >= Size
   * MDZ_ERROR_BIGCOUNT   - nCount is bigger than remove area (between nLeftPos and Size)
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_removeFrom(mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nCount);

  /**
   * Remove all ocurrences of nCount item(s) matching to pcItems, residing between nLeftPos and nRightPos
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based start position to remove item(s) from. Use 0 to search from the beginning of Data
   * \param nRightPos - 0-based end position to remove item(s) up to. Use Size-1 to search till the end of Data
   * \param pcItems   - items to remove. Cannot be NULL
   * \param nCount    - number of item(s) to remove. Cannot be 0
   * \param bFromLeft - mdz_true if search for items to remove from left side, otherwise from right
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_BIGCOUNT   - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_remove(mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, mdz_bool bFromLeft);

  /**
   * Remove items which are contained in pcItems from left, until first non-contained in pcItems item is reached.
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based start position to trim item(s) from left. Use 0 to trim from the beginning of Data
   * \param nRightPos - 0-based end position to trim item(s) up to. Use Size-1 to trim till the end of Data
   * \param pcItems   - items to trim. Cannot be NULL
   * \param nCount    - number of items to trim. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_trimLeft(mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount);

  /**
   * Remove items which are contained in pcItems from right, until first non-contained in pcItems item is reached.
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based end position to trim item(s) up to. Use 0 to trim till the beginning of Data
   * \param nRightPos - 0-based start position to trim item(s) from right. Use Size-1 to trim from the end of Data
   * \param pcItems   - items to trim. Cannot be NULL
   * \param nCount    - number of items to trim. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_trimRight(mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount);

  /**
   * Remove items which are contained in pcItems from left and from right, until first non-contained in pcItems item is reached.
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based start position to trim item(s) from left. Use 0 to trim from the beginning of Data
   * \param nRightPos - 0-based start position to trim item(s) from right. Use Size-1 to trim from the end of Data
   * \param pcItems   - items to trim. Cannot be NULL
   * \param nCount    - number of items to trim. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_trim(mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount);

  /**
   * \defgroup Miscellaneous functions
   */

  /**
   * Compare content of Data with pcItems. If penError is not NULL, error will be written there
   * \param psAnsi          - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos        - 0-based start position to compare from left. Use 0 to compare from the beginning of Data
   * \param pcItems         - items to compare. Cannot be NULL
   * \param nCount          - number of items to compare. Cannot be 0
   * \param bPartialCompare - if mdz_true compare only nCount items, otherwise compare full strings
   * \param penError        - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGLEFT    - nLeftPos >= Size
   * MDZ_ERROR_BIGCOUNT   - nCount is bigger than compare area (between nLeftPos and Size)
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * MDZ_COMPARE_EQUAL or MDZ_COMPARE_NONEQUAL - Result of comparison
   */
  enum mdz_ansi_compare_result mdz_ansi_dyn_compare(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, const char* pcItems, size_t nCount, mdz_bool bPartialCompare, enum mdz_error* penError);

  /**
   * Counts number of pcItems substring occurences in Data. If penError is not NULL, error will be written there
   * \param psAnsi           - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos         - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos        - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \param pcItems          - items to find. Cannot be NULL
   * \param nCount           - number of items to find. Cannot be 0
   * \param bAllowOverlapped - mdz_true if overlapped substrings should be counted, otherwise mdz_false
   * \param bFromLeft        - mdz_true if search for items to count from left side, otherwise from right
   * \param penError         - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIGSIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIGRIGHT   - nRightPos >= Size
   * MDZ_ERROR_BIGLEFT    - nLeftPos > nRightPos
   * MDZ_ERROR_BIGCOUNT   - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if error happened
   * Result   - count of substring occurences. 0 if not found
   */
  size_t mdz_ansi_dyn_count(const mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, mdz_bool bAllowOverlapped, mdz_bool bFromLeft, enum mdz_error* penError);

  /**
   * Replace every occurence of pcItemsBefore with pcItemsAfter in Data. If there is not enouhgh Capacity, realloc() function set in mdz_ansi_dyn_setAllocFunctions() will be used
   * \param ppsAnsi           - pointer to address of string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach(). If after reallocation string address will be changed, new address will be stored here
   * \param nLeftPos          - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos         - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \param pcItemsBefore     - items to find. Cannot be NULL
   * \param nCountBefore      - number of items to find. Cannot be 0
   * \param pcItemsAfter      - pointer to items to replace with. Can be NULL
   * \param nCountAfter       - number of items to replace. Can be 0
   * \param bFromLeft         - mdz_true if search for items to replace from left side, otherwise from right
   * \param enReplacementType - type of replacement when nCountAfter > nCountBefore (thus Size is growing). For now only MDZ_ANSI_REPLACE_DUAL is supported (please refer to description of mdz_ansi_replace_type enum)
   * \return:
   * MDZ_ERROR_LICENSE         - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA            - psAnsi is NULL
   * MDZ_ERROR_CAPACITY        - Capacity is 0 or too large
   * MDZ_ERROR_BIG_SIZE        - Size > Capacity
   * MDZ_ERROR_ZERO_SIZE       - Size is 0 (string is empty)
   * MDZ_ERROR_TERMINATOR      - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS           - pcItemsBefore is NULL
   * MDZ_ERROR_ZERO_COUNT      - nCountBefore is 0
   * MDZ_ERROR_BIG_RIGHT       - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT        - nLeftPos > nRightPos
   * MDZ_ERROR_BIG_COUNT       - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_OVERLAP         - Data overlaps with pcItemsBefore, or Data overlaps with pcItemsAfter
   * MDZ_ERROR_BIG_REPLACE     - new Size after replacement > Capacity
   * MDZ_ERROR_OVERLAP_REPLACE - Data after replacement - overlaps with pcItemsBefore, or Data after replacement - overlaps with pcItemsAfter
   * MDZ_ERROR_ATTACHED        - string has not enough capacity for replacements, but is attached (using mdz_ansi_dyn_attach()), thus cannot be reallocated
   * MDZ_ERROR_REALLOC_FUNC    - could not realloc string created with mdz_ansi_dyn_create(), because realloc() function is not set (using mdz_ansi_dyn_setAllocFunctions())
   * MDZ_ERROR_ALLOCATION      - string reallocation failed
   * MDZ_ERROR_NONE            - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_replace(mdz_AnsiDyn** ppsAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItemsBefore, size_t nCountBefore, const char* pcItemsAfter, size_t nCountAfter, mdz_bool bFromLeft, enum mdz_ansi_replace_type enReplacementType);

  /**
   * Reverses characters in string, like "1234" into "4321".
   * \param psAnsi    - pointer to string returned by mdz_ansi_dyn_create() or mdz_ansi_dyn_attach()
   * \param nLeftPos  - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_dyn_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos >= nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_dyn_reverse(mdz_AnsiDyn* psAnsi, size_t nLeftPos, size_t nRightPos);

#ifdef __cplusplus
}
#endif

#endif
