#include <cassert>
#include <cstdint>
#include <iostream>
#include "../firmware/water_tank_monitor/cloud_protocol.h"

int main() {
  assert(cloud::entryWasStored(204));
  assert(!cloud::entryWasStored(200));
  assert(!cloud::entryWasStored(401));
  assert(!cloud::entryWasStored(404));
  assert(!cloud::entryWasStored(-1));

  assert(cloud::databaseUrlIsValid("https://tank-default-rtdb.firebaseio.com"));
  assert(cloud::databaseUrlIsValid("https://tank-default-rtdb.firebaseio.com/"));
  assert(cloud::databaseUrlIsValid("https://tank-default-rtdb.europe-west1.firebasedatabase.app"));
  assert(!cloud::databaseUrlIsValid(nullptr));
  assert(!cloud::databaseUrlIsValid(""));
  assert(!cloud::databaseUrlIsValid("http://tank.firebaseio.com"));
  assert(!cloud::databaseUrlIsValid("https://firebaseio.com"));
  assert(!cloud::databaseUrlIsValid("https://tank.firebaseio.com.evil.example"));
  assert(!cloud::databaseUrlIsValid("https://tank.firebaseio.com/devices"));
  assert(!cloud::databaseUrlIsValid("https://tank.firebaseio.com?auth=secret"));
  assert(!cloud::databaseUrlIsValid("https://tank.firebaseio.com@evil.example"));
  assert(!cloud::databaseUrlIsValid("https://user@tank.firebaseio.com"));
  assert(!cloud::databaseUrlIsValid("https://tank.firebaseio.com:443"));
  assert(!cloud::databaseUrlIsValid("https://tank.firebaseio.com//"));

  assert(cloud::identifierIsSafe("Abc_123-XYZ"));
  assert(!cloud::identifierIsSafe(nullptr));
  assert(!cloud::identifierIsSafe(""));
  assert(!cloud::identifierIsSafe("uid/path"));
  assert(!cloud::identifierIsSafe("key&other=value"));

  assert(cloud::sampleIsFresh(6000, 1000));
  assert(!cloud::sampleIsFresh(6001, 1000));
  assert(cloud::sampleIsFresh(500, UINT32_MAX - 1000));
  assert(!cloud::sampleIsFresh(1000, 2000));
  assert(cloud::uploadIntervalElapsed(0, 0, false));
  assert(!cloud::uploadIntervalElapsed(19999, 0, true));
  assert(cloud::uploadIntervalElapsed(20000, 0, true));
  assert(cloud::uploadIntervalElapsed(500, UINT32_MAX - 20000, true));

  std::cout << "All cloud-protocol tests passed.\n";
  return 0;
}
