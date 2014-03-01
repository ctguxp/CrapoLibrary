// Perch Lake Computer System

#include "pch.h"
#include "CultureInfoEntry.h"

#define NUM_CULTURE_ENTRIES 283

void* BinarySearch(const void *key, const void *array, size_t array_length
                  ,size_t member_size, BinarySearchComparer comparer)
  {
  const char *base = (const char*)array;
  size_t lim;
  int cmp;
  const void *p;

  for (lim = array_length; lim; lim >>= 1)
    {
    p = base + (lim >> 1) * member_size;
    cmp = (* comparer) (key, p);

    if (!cmp)
      return (void *) p;
    else if (cmp > 0)
      {
      base = (const char *) p + member_size;
      lim--;
      }
    }

  return nullptr;
  }

int CultureLcidLocator(const void *a, const void *b)
{
	const int *lcid = (const int*)a;
	const CultureInfoEntry *bb = (const CultureInfoEntry *)b;

	return *lcid - bb->lcid;
}

const CultureInfoEntry* CultureInfoEntryFromLCID(int lcid)
{
	const CultureInfoEntry *ci;

	ci = (const CultureInfoEntry *)BinarySearch(&lcid, culture_entries, NUM_CULTURE_ENTRIES, sizeof (CultureInfoEntry), CultureLcidLocator);

	return ci;
}
