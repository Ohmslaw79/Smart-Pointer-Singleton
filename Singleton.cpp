#include "Singleton.h"
#include <mutex>

/// <summary>
/// Get the Singleton object if it exists or create a new one if it does not
/// </summary>
/// <returns> A shared pointer to the singleton object </returns>
std::shared_ptr<Singleton> Singleton::Get()
{
  static std::mutex Mutex;
  static std::weak_ptr<Singleton> Instance; // Static weak Pointer for singleton

  if(Instance.expired()) // Check if pointer is still valid. This allows all threads to pass without locking if there is already an instance
  {
    Mutex.lock(); // If expired lock to create a new one

    if(Instance.expired())  // Check again if its valid. If any threads queue up at this lock while one is creating the object
                            // this makes sure they pass and grab the created object instead of also creating a new object
    {
      std::shared_ptr<Singleton> InitPtr(new Singleton()); // Shared Pointer used to initialize the weak pointer
      Instance = InitPtr; // Copy the shared_ptr into the weak_ptr instance

      Mutex.unlock();
      return Instance.lock(); // Return a shared pointer created from the weak pointer. 
                              // Returning here is required. If we don't return before leaving the if statement, 
                              // then the object will get destroyed when leaving the scope of the if statement
    }
    Mutex.unlock();
  }
  return Instance.lock(); // Return a shared pointer created from the weak_ptr
}