#pragma once
#include <memory>

/*
  As with any singleton object destruction cannot be completely guarded. As its currently instantiated it is possible to delete
  the underlying object from the shared_ptrs. But as they are smart pointers an explicit delete should never be required, this would just
  help protect against poorly written code

  There are a multitude of different ways to solve this but each has their own benefits and drawbacks. Here are some that I have come up with
  which theoretically should work

  1. Just leave it alone. In order for the underlying object to get deleted the raw pointer would have to be extracted from the smart pointer,
     Then delete would have to be called on it. This would be very poor coding practice and should never be done in the first place so this situation
     is not hard to avoid

  2. Define a protected destructor and friend the smart_pointer class. This protects against any explicit deletions but does require adding
     friend classes, which is something this class has been written to avoid. If you do declare a friend class you can also declare the constructor
     protected instead of private and the "new" in the get function can be replaced with std::make_shared<Singleton>(), which is slightly more proper

  3. Define a public destructor and a static global bDeleted variable. The destructor sets bDeleted to true, the get function checks bDeleted
     along with Instance.expired() and makes a new object if either are true then sets bDeleted to false. This ensures that if its deleted
     any get will return a valid object, even if it was deleted without the smart pointers knowing. However this will cause the entire class 
     to reset and requires a global variable
       - This could be extended to having every method check this bDeleted variable as well. Which would help protect existing smart pointers too
*/

class Singleton
{
private:
  Singleton();

// Delete copy constructor and assignment operator to prevent singleton violations
  Singleton(const Singleton&)       = delete;
  void operator= (const Singleton&) = delete;

public:
  static std::shared_ptr<Singleton> Get();
};

