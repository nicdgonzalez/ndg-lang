#include "hashtable.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t _hash(struct HashTable *ht, const char *_key) {
  uint64_t hash = 1;
  for (uint8_t *key = (uint8_t *) _key; *key; key++) {
    hash *= 163;
    hash += *key;
  }
  return (hash %= ht->capacity);
}

static bool _is_prime(int64_t _value) {
  if ((_value == 0) || (_value == 1))
    return false;

  for (int64_t i = 2; i < (_value / 2); i++)
    if ((_value % i) == 0)
      return false;

  return true;
}

static int64_t _get_prime(int64_t _value) {
  if ((_value % 2) == 0)
    _value++;

  while (!(_is_prime(_value)))
    _value += 2;

  return _value;
}

static void _hashtable_init(struct HashTable *ht, uint64_t _capacity) {
  ht->capacity = _get_prime(_capacity);
  ht->count = 0;
  ht->table = (struct Bucket *) calloc(ht->capacity, sizeof(struct Bucket));

  for (uint64_t index = ht->capacity; index--; ) {
    ht->table[index].previous = &(ht->table[index]);
    ht->table[index].next = NULL;
    ht->table[index].key = NULL;
    ht->table[index].value = NULL;
  }
  return;
}

void hashtable_init(struct HashTable *ht) {
  _hashtable_init(ht, 149);
  return;
}

void hashtable_insert(struct HashTable *ht, const char *_key, void *_value) {
  uint64_t index = _hash(ht, _key);
  struct Bucket bucket = ht->table[index];

  // There is already an entry at this index location.
  if (bucket.key != NULL) {
    // Key already exists; update the value.
    if (bucket.key == _key) {
      bucket.value = _value;
    }
    // This is a *Hash Collision* - resolve by chaining.
    else {
      while (bucket.next != NULL) {
        bucket = *(bucket.next);
      }
      (bucket.next)->previous = &bucket;
      (bucket.next)->next = NULL;
      (bucket.next)->key = _key;
      (bucket.next)->value = _value;
    }
  }
  // This index location is empty; create a new entry for the data.
  else {
    bucket.previous = &bucket;
    bucket.next = NULL;
    bucket.key = _key;
    bucket.value = _value;
  }

  // If the table is at least 75% of its maximum capacity, expand it.
  if (ht->count < (ht->capacity * 0.75))
    return;

  struct HashTable expanded;
  _hashtable_init(&expanded, (ht->capacity * 2));

  // Move all existing entries to the new expanded Hash Table.
  for (uint64_t index = ht->capacity; index--; ) {
    bucket = ht->table[index];

    if (bucket.key != NULL) {
      hashtable_insert(&expanded, bucket.key, bucket.value);

      for (bucket = *bucket.next; bucket.key != NULL; bucket = *bucket.next)
        hashtable_insert(&expanded, bucket.key, bucket.value);
    }
  }

  // Swap the existing Hash Table with the new expanded one.
  struct HashTable tmp = *ht;
  *ht = expanded;
  expanded = tmp;

  return;
}

void * hashtable_search(struct HashTable *ht, const char *_key) {
  uint64_t index = _hash(ht, _key);
  struct Bucket bucket = ht->table[index];

  if (bucket.key == _key)
    return bucket.value;

  while (bucket.next != NULL) {
    bucket = *bucket.next;
    if (bucket.key == _key)
      return bucket.value;
  }

  return NULL;
}

void hashtable_remove(struct HashTable *ht, const char *_key) {
  uint64_t index = _hash(ht, _key);
  struct Bucket bucket = ht->table[index];

  while (bucket.next != NULL) {
    if (bucket.key == _key) {
      bucket = ht->table[index] = *bucket.next;
      bucket.previous = &bucket;
      return;
    }
  }

  if (bucket.key == _key) {
    bucket.key = NULL;
    bucket.value = NULL;
  }

  return;
}
